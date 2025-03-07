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
**         CMSIS Peripheral Access Layer for ELS
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
 * @file ELS.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for ELS
 *
 * CMSIS Peripheral Access Layer for ELS
 */

#if !defined(ELS_H_)
#define ELS_H_                                   /**< Symbol preventing repeated inclusion */

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
   -- ELS Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ELS_Peripheral_Access_Layer ELS Peripheral Access Layer
 * @{
 */

/** ELS - Register Layout Typedef */
typedef struct {
  __I  uint32_t ELS_STATUS;                        /**< Status register, offset: 0x0 */
  __IO uint32_t ELS_CTRL;                          /**< ELS Control register, offset: 0x4 */
  __IO uint32_t ELS_CMDCFG0;                       /**< ELS command configuration register, offset: 0x8 */
  __IO uint32_t ELS_CFG;                           /**< ELS configuration register, offset: 0xC */
  __IO uint32_t ELS_KIDX0;                         /**< Keystore index 0 - for commands that access a single key, offset: 0x10 */
  __IO uint32_t ELS_KIDX1;                         /**< Keystore index 1 - for commands that access 2 keys, offset: 0x14 */
  __IO uint32_t ELS_KPROPIN;                       /**< key properties request, offset: 0x18 */
       uint8_t RESERVED_0[4];
  __IO uint32_t ELS_DMA_SRC0;                      /**< ELS DMA Source 0, offset: 0x20 */
  __IO uint32_t ELS_DMA_SRC0_LEN;                  /**< ELS DMA Source 0 length, offset: 0x24 */
  __IO uint32_t ELS_DMA_SRC1;                      /**< ELS DMA Source 1, offset: 0x28 */
       uint8_t RESERVED_1[4];
  __IO uint32_t ELS_DMA_SRC2;                      /**< ELS DMA Source 2, offset: 0x30 */
  __IO uint32_t ELS_DMA_SRC2_LEN;                  /**< ELS DMA Source 2 length, offset: 0x34 */
  __IO uint32_t ELS_DMA_RES0;                      /**< ELS DMA Result 0, offset: 0x38 */
  __IO uint32_t ELS_DMA_RES0_LEN;                  /**< ELS DMA Result 0 Size, offset: 0x3C */
  __IO uint32_t ELS_INT_ENABLE;                    /**< Interrupt enable, offset: 0x40 */
  __O  uint32_t ELS_INT_STATUS_CLR;                /**< Interrupt status clear, offset: 0x44 */
  __O  uint32_t ELS_INT_STATUS_SET;                /**< Interrupt status set, offset: 0x48 */
  __I  uint32_t ELS_ERR_STATUS;                    /**< Status register, offset: 0x4C */
  __O  uint32_t ELS_ERR_STATUS_CLR;                /**< Interrupt status clear, offset: 0x50 */
  __I  uint32_t ELS_VERSION;                       /**< ELS Version, offset: 0x54 */
  __I  uint32_t ELS_CONFIG;                        /**< ELS Config, offset: 0x58 */
  __I  uint32_t ELS_PRNG_DATOUT;                   /**< PRNG SW read out register, offset: 0x5C */
  __I  uint32_t ELS_GDET_EVTCNT;                   /**< ELS GDET Event Counter, offset: 0x60 */
  __O  uint32_t ELS_GDET_EVTCNT_CLR;               /**< ELS GDET Event Counter Clear, offset: 0x64 */
       uint8_t RESERVED_2[152];
  __I  uint32_t ELS_SHA2_STATUS;                   /**< ELS SHA2 Status Register, offset: 0x100 */
  __IO uint32_t ELS_SHA2_CTRL;                     /**< SHA2 Control register, offset: 0x104 */
  __IO uint32_t ELS_SHA2_DIN;                      /**< ELS SHA_DATA IN Register 0, offset: 0x108 */
  __I  uint32_t ELS_SHA2_DOUT0;                    /**< ELS ELS_SHA_DATA Out Register 0, offset: 0x10C */
  __I  uint32_t ELS_SHA2_DOUT1;                    /**< ELS SHA_DATA Out Register 1, offset: 0x110 */
  __I  uint32_t ELS_SHA2_DOUT2;                    /**< ELS SHA_DATA Out Register 2, offset: 0x114 */
  __I  uint32_t ELS_SHA2_DOUT3;                    /**< ELS SHA_DATA Out Register 3, offset: 0x118 */
  __I  uint32_t ELS_SHA2_DOUT4;                    /**< ELS SHA_DATA Out Register 4, offset: 0x11C */
  __I  uint32_t ELS_SHA2_DOUT5;                    /**< ELS SHA_DATA Out Register 5, offset: 0x120 */
  __I  uint32_t ELS_SHA2_DOUT6;                    /**< ELS SHA_DATA Out Register 6, offset: 0x124 */
  __I  uint32_t ELS_SHA2_DOUT7;                    /**< ELS SHA_DATA Out Register 7, offset: 0x128 */
  __I  uint32_t ELS_SHA2_DOUT8;                    /**< ELS ELS_SHA_DATA Out Register 8, offset: 0x12C */
  __I  uint32_t ELS_SHA2_DOUT9;                    /**< ELS SHA_DATA Out Register 9, offset: 0x130 */
  __I  uint32_t ELS_SHA2_DOUT10;                   /**< ELS SHA_DATA Out Register 10, offset: 0x134 */
  __I  uint32_t ELS_SHA2_DOUT11;                   /**< ELS SHA_DATA Out Register 11, offset: 0x138 */
  __I  uint32_t ELS_SHA2_DOUT12;                   /**< ELS SHA_DATA Out Register 12, offset: 0x13C */
  __I  uint32_t ELS_SHA2_DOUT13;                   /**< ELS SHA_DATA Out Register 13, offset: 0x140 */
  __I  uint32_t ELS_SHA2_DOUT14;                   /**< ELS SHA_DATA Out Register 14, offset: 0x144 */
  __I  uint32_t ELS_SHA2_DOUT15;                   /**< ELS SHA_DATA Out Register 15, offset: 0x148 */
       uint8_t RESERVED_3[4];
  __I  uint32_t ELS_KS0;                           /**< Status register, offset: 0x150 */
  __I  uint32_t ELS_KS1;                           /**< Status register, offset: 0x154 */
  __I  uint32_t ELS_KS2;                           /**< Status register, offset: 0x158 */
  __I  uint32_t ELS_KS3;                           /**< Status register, offset: 0x15C */
  __I  uint32_t ELS_KS4;                           /**< Status register, offset: 0x160 */
  __I  uint32_t ELS_KS5;                           /**< Status register, offset: 0x164 */
  __I  uint32_t ELS_KS6;                           /**< Status register, offset: 0x168 */
  __I  uint32_t ELS_KS7;                           /**< Status register, offset: 0x16C */
  __I  uint32_t ELS_KS8;                           /**< Status register, offset: 0x170 */
  __I  uint32_t ELS_KS9;                           /**< Status register, offset: 0x174 */
  __I  uint32_t ELS_KS10;                          /**< Status register, offset: 0x178 */
  __I  uint32_t ELS_KS11;                          /**< Status register, offset: 0x17C */
  __I  uint32_t ELS_KS12;                          /**< Status register, offset: 0x180 */
  __I  uint32_t ELS_KS13;                          /**< Status register, offset: 0x184 */
  __I  uint32_t ELS_KS14;                          /**< Status register, offset: 0x188 */
  __I  uint32_t ELS_KS15;                          /**< Status register, offset: 0x18C */
  __I  uint32_t ELS_KS16;                          /**< Status register, offset: 0x190 */
  __I  uint32_t ELS_KS17;                          /**< Status register, offset: 0x194 */
  __I  uint32_t ELS_KS18;                          /**< Status register, offset: 0x198 */
  __I  uint32_t ELS_KS19;                          /**< Status register, offset: 0x19C */
       uint8_t RESERVED_4[4];
  __IO uint32_t ELS_BOOT_ADDR;                     /**< SW control for the ELS boot addr, offset: 0x1A4 */
  __IO uint32_t ELS_DBG_CFG;                       /**< ELS Debug Config SFR, offset: 0x1A8 */
} ELS_Type;

/* ----------------------------------------------------------------------------
   -- ELS Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ELS_Register_Masks ELS Register Masks
 * @{
 */

/*! @name ELS_STATUS - Status register */
/*! @{ */

#define ELS_ELS_STATUS_ELS_BUSY_MASK             (0x1U)
#define ELS_ELS_STATUS_ELS_BUSY_SHIFT            (0U)
/*! ELS_BUSY - High to indicate the ELS is executing a Crypto Sequence */
#define ELS_ELS_STATUS_ELS_BUSY(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_ELS_BUSY_SHIFT)) & ELS_ELS_STATUS_ELS_BUSY_MASK)

#define ELS_ELS_STATUS_ELS_IRQ_MASK              (0x2U)
#define ELS_ELS_STATUS_ELS_IRQ_SHIFT             (1U)
/*! ELS_IRQ - High to indicate the ELS has an active interrupt */
#define ELS_ELS_STATUS_ELS_IRQ(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_ELS_IRQ_SHIFT)) & ELS_ELS_STATUS_ELS_IRQ_MASK)

#define ELS_ELS_STATUS_ELS_ERR_MASK              (0x4U)
#define ELS_ELS_STATUS_ELS_ERR_SHIFT             (2U)
/*! ELS_ERR - High to indicate the ELS has detected an internal error */
#define ELS_ELS_STATUS_ELS_ERR(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_ELS_ERR_SHIFT)) & ELS_ELS_STATUS_ELS_ERR_MASK)

#define ELS_ELS_STATUS_PRNG_RDY_MASK             (0x8U)
#define ELS_ELS_STATUS_PRNG_RDY_SHIFT            (3U)
/*! PRNG_RDY - High to indicate the internal PRNG is ready. */
#define ELS_ELS_STATUS_PRNG_RDY(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_PRNG_RDY_SHIFT)) & ELS_ELS_STATUS_PRNG_RDY_MASK)

#define ELS_ELS_STATUS_ECDSA_VFY_STATUS_MASK     (0x30U)
#define ELS_ELS_STATUS_ECDSA_VFY_STATUS_SHIFT    (4U)
/*! ECDSA_VFY_STATUS - Signature Verify Result Status; 0 == No Verify Run; 1 == Signature Verify
 *    Failed; 2 == Signature Verify Passed; 3 == Invalid , Error
 */
#define ELS_ELS_STATUS_ECDSA_VFY_STATUS(x)       (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_ECDSA_VFY_STATUS_SHIFT)) & ELS_ELS_STATUS_ECDSA_VFY_STATUS_MASK)

#define ELS_ELS_STATUS_PPROT_MASK                (0xC0U)
#define ELS_ELS_STATUS_PPROT_SHIFT               (6U)
/*! PPROT - Current command privilege level */
#define ELS_ELS_STATUS_PPROT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_PPROT_SHIFT)) & ELS_ELS_STATUS_PPROT_MASK)

#define ELS_ELS_STATUS_DRBG_ENT_LVL_MASK         (0x300U)
#define ELS_ELS_STATUS_DRBG_ENT_LVL_SHIFT        (8U)
/*! DRBG_ENT_LVL - Entropy quality of the current DRBG instance. */
#define ELS_ELS_STATUS_DRBG_ENT_LVL(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_DRBG_ENT_LVL_SHIFT)) & ELS_ELS_STATUS_DRBG_ENT_LVL_MASK)

#define ELS_ELS_STATUS_DTRNG_BUSY_MASK           (0x400U)
#define ELS_ELS_STATUS_DTRNG_BUSY_SHIFT          (10U)
/*! DTRNG_BUSY - When set, it indicates the DTRNG is gathering entropy */
#define ELS_ELS_STATUS_DTRNG_BUSY(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_DTRNG_BUSY_SHIFT)) & ELS_ELS_STATUS_DTRNG_BUSY_MASK)

#define ELS_ELS_STATUS_GDET_IRQ_POS_MASK         (0x800U)
#define ELS_ELS_STATUS_GDET_IRQ_POS_SHIFT        (11U)
/*! GDET_IRQ_POS - IRQ for GDET has detected a negative glitch: active high irq */
#define ELS_ELS_STATUS_GDET_IRQ_POS(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_GDET_IRQ_POS_SHIFT)) & ELS_ELS_STATUS_GDET_IRQ_POS_MASK)

#define ELS_ELS_STATUS_GDET_IRQ_NEG_MASK         (0x1000U)
#define ELS_ELS_STATUS_GDET_IRQ_NEG_SHIFT        (12U)
/*! GDET_IRQ_NEG - IRQ for GDET has detected a positive glitch: active high irq */
#define ELS_ELS_STATUS_GDET_IRQ_NEG(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_GDET_IRQ_NEG_SHIFT)) & ELS_ELS_STATUS_GDET_IRQ_NEG_MASK)

#define ELS_ELS_STATUS_STATUS_RSVD_MASK          (0xFFFFE000U)
#define ELS_ELS_STATUS_STATUS_RSVD_SHIFT         (13U)
#define ELS_ELS_STATUS_STATUS_RSVD(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_STATUS_STATUS_RSVD_SHIFT)) & ELS_ELS_STATUS_STATUS_RSVD_MASK)
/*! @} */

/*! @name ELS_CTRL - ELS Control register */
/*! @{ */

#define ELS_ELS_CTRL_ELS_EN_MASK                 (0x1U)
#define ELS_ELS_CTRL_ELS_EN_SHIFT                (0U)
/*! ELS_EN - ELS enable 0=ELS disabled, 1= ELS is enabled */
#define ELS_ELS_CTRL_ELS_EN(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CTRL_ELS_EN_SHIFT)) & ELS_ELS_CTRL_ELS_EN_MASK)

#define ELS_ELS_CTRL_ELS_START_MASK              (0x2U)
#define ELS_ELS_CTRL_ELS_START_SHIFT             (1U)
/*! ELS_START - Write to 1 to start a ELS Operation */
#define ELS_ELS_CTRL_ELS_START(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CTRL_ELS_START_SHIFT)) & ELS_ELS_CTRL_ELS_START_MASK)

#define ELS_ELS_CTRL_ELS_RESET_MASK              (0x4U)
#define ELS_ELS_CTRL_ELS_RESET_SHIFT             (2U)
/*! ELS_RESET - Write to 1 to perform a ELS synchronous Reset */
#define ELS_ELS_CTRL_ELS_RESET(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CTRL_ELS_RESET_SHIFT)) & ELS_ELS_CTRL_ELS_RESET_MASK)

#define ELS_ELS_CTRL_ELS_CMD_MASK                (0xF8U)
#define ELS_ELS_CTRL_ELS_CMD_SHIFT               (3U)
/*! ELS_CMD - ELS Command Field: List of Valid commands:; CIPHER; AUTH_CIPHER; ECSIGN; ECVFY;
 *    ECKXCH; KEYGEN; KEYIN; KEYOUT; KDELETE; KEYPROV; CKDF; HKDF; TLS_INIT; HASH; HMAC; CMAC; DRBG_REQ;
 *    DRBG_TEST; DTRNG_CFG_LOAD; DTRNG_EVAL; GDET_CFG_LOAD; GDET_TRIM
 */
#define ELS_ELS_CTRL_ELS_CMD(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CTRL_ELS_CMD_SHIFT)) & ELS_ELS_CTRL_ELS_CMD_MASK)

#define ELS_ELS_CTRL_BYTE_ORDER_MASK             (0x100U)
#define ELS_ELS_CTRL_BYTE_ORDER_SHIFT            (8U)
/*! BYTE_ORDER - Defines Endianness - 1: BigEndian, 0: Little Endian */
#define ELS_ELS_CTRL_BYTE_ORDER(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CTRL_BYTE_ORDER_SHIFT)) & ELS_ELS_CTRL_BYTE_ORDER_MASK)

#define ELS_ELS_CTRL_CTRL_RFU_MASK               (0xFFFFFE00U)
#define ELS_ELS_CTRL_CTRL_RFU_SHIFT              (9U)
/*! CTRL_RFU - reserved */
#define ELS_ELS_CTRL_CTRL_RFU(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CTRL_CTRL_RFU_SHIFT)) & ELS_ELS_CTRL_CTRL_RFU_MASK)
/*! @} */

/*! @name ELS_CMDCFG0 - ELS command configuration register */
/*! @{ */

#define ELS_ELS_CMDCFG0_CMDCFG0_MASK             (0xFFFFFFFFU)
#define ELS_ELS_CMDCFG0_CMDCFG0_SHIFT            (0U)
/*! CMDCFG0 - refer to reference manual for assignment of this field */
#define ELS_ELS_CMDCFG0_CMDCFG0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CMDCFG0_CMDCFG0_SHIFT)) & ELS_ELS_CMDCFG0_CMDCFG0_MASK)
/*! @} */

/*! @name ELS_CFG - ELS configuration register */
/*! @{ */

#define ELS_ELS_CFG_CFG_RSVD0_MASK               (0xFFFFU)
#define ELS_ELS_CFG_CFG_RSVD0_SHIFT              (0U)
/*! CFG_RSVD0 - reserved */
#define ELS_ELS_CFG_CFG_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CFG_CFG_RSVD0_SHIFT)) & ELS_ELS_CFG_CFG_RSVD0_MASK)

#define ELS_ELS_CFG_ADCTRL_MASK                  (0x3FF0000U)
#define ELS_ELS_CFG_ADCTRL_SHIFT                 (16U)
/*! ADCTRL - maximum aes start delay */
#define ELS_ELS_CFG_ADCTRL(x)                    (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CFG_ADCTRL_SHIFT)) & ELS_ELS_CFG_ADCTRL_MASK)

#define ELS_ELS_CFG_CFG_RSVD1_MASK               (0x7C000000U)
#define ELS_ELS_CFG_CFG_RSVD1_SHIFT              (26U)
/*! CFG_RSVD1 - reserved */
#define ELS_ELS_CFG_CFG_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CFG_CFG_RSVD1_SHIFT)) & ELS_ELS_CFG_CFG_RSVD1_MASK)

#define ELS_ELS_CFG_SHA2_DIRECT_MASK             (0x80000000U)
#define ELS_ELS_CFG_SHA2_DIRECT_SHIFT            (31U)
/*! SHA2_DIRECT - 1=enable sha2 direct mode: direct access from external; bus to els internal sha */
#define ELS_ELS_CFG_SHA2_DIRECT(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CFG_SHA2_DIRECT_SHIFT)) & ELS_ELS_CFG_SHA2_DIRECT_MASK)
/*! @} */

/*! @name ELS_KIDX0 - Keystore index 0 - for commands that access a single key */
/*! @{ */

#define ELS_ELS_KIDX0_KIDX0_MASK                 (0x7FU)
#define ELS_ELS_KIDX0_KIDX0_SHIFT                (0U)
/*! KIDX0 - keystore is indexed as an array of 128 bit key slots */
#define ELS_ELS_KIDX0_KIDX0(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KIDX0_KIDX0_SHIFT)) & ELS_ELS_KIDX0_KIDX0_MASK)
/*! @} */

/*! @name ELS_KIDX1 - Keystore index 1 - for commands that access 2 keys */
/*! @{ */

#define ELS_ELS_KIDX1_KIDX1_MASK                 (0x7FU)
#define ELS_ELS_KIDX1_KIDX1_SHIFT                (0U)
/*! KIDX1 - keystore is indexed as an array of 128 bit key slots */
#define ELS_ELS_KIDX1_KIDX1(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KIDX1_KIDX1_SHIFT)) & ELS_ELS_KIDX1_KIDX1_MASK)
/*! @} */

/*! @name ELS_KPROPIN - key properties request */
/*! @{ */

#define ELS_ELS_KPROPIN_KPROPIN_MASK             (0xFFFFFFFFU)
#define ELS_ELS_KPROPIN_KPROPIN_SHIFT            (0U)
/*! KPROPIN - for commands that create a key - requested properties; of the key that is being created */
#define ELS_ELS_KPROPIN_KPROPIN(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KPROPIN_KPROPIN_SHIFT)) & ELS_ELS_KPROPIN_KPROPIN_MASK)
/*! @} */

/*! @name ELS_DMA_SRC0 - ELS DMA Source 0 */
/*! @{ */

#define ELS_ELS_DMA_SRC0_ADDR_SRC0_MASK          (0xFFFFFFFFU)
#define ELS_ELS_DMA_SRC0_ADDR_SRC0_SHIFT         (0U)
/*! ADDR_SRC0 - defines the System address of the start of the; data to be transferred to the ELS via DMA */
#define ELS_ELS_DMA_SRC0_ADDR_SRC0(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_SRC0_ADDR_SRC0_SHIFT)) & ELS_ELS_DMA_SRC0_ADDR_SRC0_MASK)
/*! @} */

/*! @name ELS_DMA_SRC0_LEN - ELS DMA Source 0 length */
/*! @{ */

#define ELS_ELS_DMA_SRC0_LEN_SIZE_SRC0_LEN_MASK  (0xFFFFFFFFU)
#define ELS_ELS_DMA_SRC0_LEN_SIZE_SRC0_LEN_SHIFT (0U)
/*! SIZE_SRC0_LEN - Size in bytes of the data to be transferred from; the target defined in SFR ELS_DMA_SRC0 */
#define ELS_ELS_DMA_SRC0_LEN_SIZE_SRC0_LEN(x)    (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_SRC0_LEN_SIZE_SRC0_LEN_SHIFT)) & ELS_ELS_DMA_SRC0_LEN_SIZE_SRC0_LEN_MASK)
/*! @} */

/*! @name ELS_DMA_SRC1 - ELS DMA Source 1 */
/*! @{ */

#define ELS_ELS_DMA_SRC1_ADDR_SRC1_MASK          (0xFFFFFFFFU)
#define ELS_ELS_DMA_SRC1_ADDR_SRC1_SHIFT         (0U)
/*! ADDR_SRC1 - defines the System address of the start of the; data to be transferred to the ELS via DMA */
#define ELS_ELS_DMA_SRC1_ADDR_SRC1(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_SRC1_ADDR_SRC1_SHIFT)) & ELS_ELS_DMA_SRC1_ADDR_SRC1_MASK)
/*! @} */

/*! @name ELS_DMA_SRC2 - ELS DMA Source 2 */
/*! @{ */

#define ELS_ELS_DMA_SRC2_ADDR_SRC2_MASK          (0xFFFFFFFFU)
#define ELS_ELS_DMA_SRC2_ADDR_SRC2_SHIFT         (0U)
/*! ADDR_SRC2 - defines the System address of the start of the; data to be transferred to the ELS via DMA */
#define ELS_ELS_DMA_SRC2_ADDR_SRC2(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_SRC2_ADDR_SRC2_SHIFT)) & ELS_ELS_DMA_SRC2_ADDR_SRC2_MASK)
/*! @} */

/*! @name ELS_DMA_SRC2_LEN - ELS DMA Source 2 length */
/*! @{ */

#define ELS_ELS_DMA_SRC2_LEN_SIZE_SRC2_LEN_MASK  (0xFFFFFFFFU)
#define ELS_ELS_DMA_SRC2_LEN_SIZE_SRC2_LEN_SHIFT (0U)
/*! SIZE_SRC2_LEN - Size in bytes of the data to be transferred from; the target defined in SFR ELS_DMA_SRC2 */
#define ELS_ELS_DMA_SRC2_LEN_SIZE_SRC2_LEN(x)    (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_SRC2_LEN_SIZE_SRC2_LEN_SHIFT)) & ELS_ELS_DMA_SRC2_LEN_SIZE_SRC2_LEN_MASK)
/*! @} */

/*! @name ELS_DMA_RES0 - ELS DMA Result 0 */
/*! @{ */

#define ELS_ELS_DMA_RES0_ADDR_RES0_MASK          (0xFFFFFFFFU)
#define ELS_ELS_DMA_RES0_ADDR_RES0_SHIFT         (0U)
/*! ADDR_RES0 - defines the System Start address of where the result; of the ELS operation will be transferred via DMA */
#define ELS_ELS_DMA_RES0_ADDR_RES0(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_RES0_ADDR_RES0_SHIFT)) & ELS_ELS_DMA_RES0_ADDR_RES0_MASK)
/*! @} */

/*! @name ELS_DMA_RES0_LEN - ELS DMA Result 0 Size */
/*! @{ */

#define ELS_ELS_DMA_RES0_LEN_SIZE_RES0_LEN_MASK  (0xFFFFFFFFU)
#define ELS_ELS_DMA_RES0_LEN_SIZE_RES0_LEN_SHIFT (0U)
/*! SIZE_RES0_LEN - Size in bytes of the data to be transferred to */
#define ELS_ELS_DMA_RES0_LEN_SIZE_RES0_LEN(x)    (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DMA_RES0_LEN_SIZE_RES0_LEN_SHIFT)) & ELS_ELS_DMA_RES0_LEN_SIZE_RES0_LEN_MASK)
/*! @} */

/*! @name ELS_INT_ENABLE - Interrupt enable */
/*! @{ */

#define ELS_ELS_INT_ENABLE_INT_EN_MASK           (0x1U)
#define ELS_ELS_INT_ENABLE_INT_EN_SHIFT          (0U)
/*! INT_EN - Interrupt enable bit */
#define ELS_ELS_INT_ENABLE_INT_EN(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_ENABLE_INT_EN_SHIFT)) & ELS_ELS_INT_ENABLE_INT_EN_MASK)

#define ELS_ELS_INT_ENABLE_GDET_INT_EN_MASK      (0x2U)
#define ELS_ELS_INT_ENABLE_GDET_INT_EN_SHIFT     (1U)
/*! GDET_INT_EN - GDET Interrupt enable bit */
#define ELS_ELS_INT_ENABLE_GDET_INT_EN(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_ENABLE_GDET_INT_EN_SHIFT)) & ELS_ELS_INT_ENABLE_GDET_INT_EN_MASK)

#define ELS_ELS_INT_ENABLE_INT_ENA_RSVD_MASK     (0xFFFFFFFCU)
#define ELS_ELS_INT_ENABLE_INT_ENA_RSVD_SHIFT    (2U)
/*! INT_ENA_RSVD - reserved */
#define ELS_ELS_INT_ENABLE_INT_ENA_RSVD(x)       (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_ENABLE_INT_ENA_RSVD_SHIFT)) & ELS_ELS_INT_ENABLE_INT_ENA_RSVD_MASK)
/*! @} */

/*! @name ELS_INT_STATUS_CLR - Interrupt status clear */
/*! @{ */

#define ELS_ELS_INT_STATUS_CLR_INT_CLR_MASK      (0x1U)
#define ELS_ELS_INT_STATUS_CLR_INT_CLR_SHIFT     (0U)
/*! INT_CLR - Interrupt status clear */
#define ELS_ELS_INT_STATUS_CLR_INT_CLR(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_CLR_INT_CLR_SHIFT)) & ELS_ELS_INT_STATUS_CLR_INT_CLR_MASK)

#define ELS_ELS_INT_STATUS_CLR_GDET_INT_CLR_MASK (0x2U)
#define ELS_ELS_INT_STATUS_CLR_GDET_INT_CLR_SHIFT (1U)
/*! GDET_INT_CLR - GDET Interrupt status clear */
#define ELS_ELS_INT_STATUS_CLR_GDET_INT_CLR(x)   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_CLR_GDET_INT_CLR_SHIFT)) & ELS_ELS_INT_STATUS_CLR_GDET_INT_CLR_MASK)

#define ELS_ELS_INT_STATUS_CLR_INT_STSC_RSVD_MASK (0xFFFFFFFCU)
#define ELS_ELS_INT_STATUS_CLR_INT_STSC_RSVD_SHIFT (2U)
/*! INT_STSC_RSVD - reserved */
#define ELS_ELS_INT_STATUS_CLR_INT_STSC_RSVD(x)  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_CLR_INT_STSC_RSVD_SHIFT)) & ELS_ELS_INT_STATUS_CLR_INT_STSC_RSVD_MASK)
/*! @} */

/*! @name ELS_INT_STATUS_SET - Interrupt status set */
/*! @{ */

#define ELS_ELS_INT_STATUS_SET_INT_SET_MASK      (0x1U)
#define ELS_ELS_INT_STATUS_SET_INT_SET_SHIFT     (0U)
/*! INT_SET - Set interrupt by software */
#define ELS_ELS_INT_STATUS_SET_INT_SET(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_SET_INT_SET_SHIFT)) & ELS_ELS_INT_STATUS_SET_INT_SET_MASK)

#define ELS_ELS_INT_STATUS_SET_GDET_INT_NEG_SET_MASK (0x2U)
#define ELS_ELS_INT_STATUS_SET_GDET_INT_NEG_SET_SHIFT (1U)
/*! GDET_INT_NEG_SET - Set GDET interrupt by software */
#define ELS_ELS_INT_STATUS_SET_GDET_INT_NEG_SET(x) (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_SET_GDET_INT_NEG_SET_SHIFT)) & ELS_ELS_INT_STATUS_SET_GDET_INT_NEG_SET_MASK)

#define ELS_ELS_INT_STATUS_SET_GDET_INT_POS_SET_MASK (0x4U)
#define ELS_ELS_INT_STATUS_SET_GDET_INT_POS_SET_SHIFT (2U)
/*! GDET_INT_POS_SET - Set GDET interrupt by software */
#define ELS_ELS_INT_STATUS_SET_GDET_INT_POS_SET(x) (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_SET_GDET_INT_POS_SET_SHIFT)) & ELS_ELS_INT_STATUS_SET_GDET_INT_POS_SET_MASK)

#define ELS_ELS_INT_STATUS_SET_INT_STSS_RSVD_MASK (0xFFFFFFF8U)
#define ELS_ELS_INT_STATUS_SET_INT_STSS_RSVD_SHIFT (3U)
/*! INT_STSS_RSVD - reserved */
#define ELS_ELS_INT_STATUS_SET_INT_STSS_RSVD(x)  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_INT_STATUS_SET_INT_STSS_RSVD_SHIFT)) & ELS_ELS_INT_STATUS_SET_INT_STSS_RSVD_MASK)
/*! @} */

/*! @name ELS_ERR_STATUS - Status register */
/*! @{ */

#define ELS_ELS_ERR_STATUS_BUS_ERR_MASK          (0x1U)
#define ELS_ELS_ERR_STATUS_BUS_ERR_SHIFT         (0U)
/*! BUS_ERR - Bus access error: public or private bus */
#define ELS_ELS_ERR_STATUS_BUS_ERR(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_BUS_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_BUS_ERR_MASK)

#define ELS_ELS_ERR_STATUS_OPN_ERR_MASK          (0x2U)
#define ELS_ELS_ERR_STATUS_OPN_ERR_SHIFT         (1U)
/*! OPN_ERR - Operational error:; ELS has been incorrectly operated */
#define ELS_ELS_ERR_STATUS_OPN_ERR(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_OPN_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_OPN_ERR_MASK)

#define ELS_ELS_ERR_STATUS_ALG_ERR_MASK          (0x4U)
#define ELS_ELS_ERR_STATUS_ALG_ERR_SHIFT         (2U)
/*! ALG_ERR - Algorithm error: An internal algorithm has; produced an unexpected result. */
#define ELS_ELS_ERR_STATUS_ALG_ERR(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_ALG_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_ALG_ERR_MASK)

#define ELS_ELS_ERR_STATUS_ITG_ERR_MASK          (0x8U)
#define ELS_ELS_ERR_STATUS_ITG_ERR_SHIFT         (3U)
/*! ITG_ERR - Data integrity error:; Internal data integrity check failed */
#define ELS_ELS_ERR_STATUS_ITG_ERR(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_ITG_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_ITG_ERR_MASK)

#define ELS_ELS_ERR_STATUS_FLT_ERR_MASK          (0x10U)
#define ELS_ELS_ERR_STATUS_FLT_ERR_SHIFT         (4U)
/*! FLT_ERR - Hardware fault error: Attempt to change the value; of an internal register */
#define ELS_ELS_ERR_STATUS_FLT_ERR(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_FLT_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_FLT_ERR_MASK)

#define ELS_ELS_ERR_STATUS_PRNG_ERR_MASK         (0x20U)
#define ELS_ELS_ERR_STATUS_PRNG_ERR_SHIFT        (5U)
/*! PRNG_ERR - User Read of ELS_PRNG_DATOUT when ELS_STATUS. */
#define ELS_ELS_ERR_STATUS_PRNG_ERR(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_PRNG_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_PRNG_ERR_MASK)

#define ELS_ELS_ERR_STATUS_ERR_LVL_MASK          (0xC0U)
#define ELS_ELS_ERR_STATUS_ERR_LVL_SHIFT         (6U)
/*! ERR_LVL - Indicates Error Level which has been triggerer. */
#define ELS_ELS_ERR_STATUS_ERR_LVL(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_ERR_LVL_SHIFT)) & ELS_ELS_ERR_STATUS_ERR_LVL_MASK)

#define ELS_ELS_ERR_STATUS_DTRNG_ERR_MASK        (0x100U)
#define ELS_ELS_ERR_STATUS_DTRNG_ERR_SHIFT       (8U)
/*! DTRNG_ERR - DTRNG unable to gather entropy with the current; configuration. */
#define ELS_ELS_ERR_STATUS_DTRNG_ERR(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_DTRNG_ERR_SHIFT)) & ELS_ELS_ERR_STATUS_DTRNG_ERR_MASK)

#define ELS_ELS_ERR_STATUS_ERR_STAT_RSVD_MASK    (0xFFFFFE00U)
#define ELS_ELS_ERR_STATUS_ERR_STAT_RSVD_SHIFT   (9U)
#define ELS_ELS_ERR_STATUS_ERR_STAT_RSVD(x)      (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_ERR_STAT_RSVD_SHIFT)) & ELS_ELS_ERR_STATUS_ERR_STAT_RSVD_MASK)
/*! @} */

/*! @name ELS_ERR_STATUS_CLR - Interrupt status clear */
/*! @{ */

#define ELS_ELS_ERR_STATUS_CLR_ERR_CLR_MASK      (0x1U)
#define ELS_ELS_ERR_STATUS_CLR_ERR_CLR_SHIFT     (0U)
/*! ERR_CLR - 1=clear ELS error status bits and exit ELS error state */
#define ELS_ELS_ERR_STATUS_CLR_ERR_CLR(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_CLR_ERR_CLR_SHIFT)) & ELS_ELS_ERR_STATUS_CLR_ERR_CLR_MASK)

#define ELS_ELS_ERR_STATUS_CLR_ERR_STSC_RSVD_MASK (0xFFFFFFFEU)
#define ELS_ELS_ERR_STATUS_CLR_ERR_STSC_RSVD_SHIFT (1U)
/*! ERR_STSC_RSVD - reserved */
#define ELS_ELS_ERR_STATUS_CLR_ERR_STSC_RSVD(x)  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_ERR_STATUS_CLR_ERR_STSC_RSVD_SHIFT)) & ELS_ELS_ERR_STATUS_CLR_ERR_STSC_RSVD_MASK)
/*! @} */

/*! @name ELS_VERSION - ELS Version */
/*! @{ */

#define ELS_ELS_VERSION_Z_MASK                   (0xFU)
#define ELS_ELS_VERSION_Z_SHIFT                  (0U)
/*! Z - extended revision version: possible values 0-9 */
#define ELS_ELS_VERSION_Z(x)                     (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_Z_SHIFT)) & ELS_ELS_VERSION_Z_MASK)

#define ELS_ELS_VERSION_Y2_MASK                  (0xF0U)
#define ELS_ELS_VERSION_Y2_SHIFT                 (4U)
/*! Y2 - minor release version digit0: possible values 0-9 */
#define ELS_ELS_VERSION_Y2(x)                    (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_Y2_SHIFT)) & ELS_ELS_VERSION_Y2_MASK)

#define ELS_ELS_VERSION_Y1_MASK                  (0xF00U)
#define ELS_ELS_VERSION_Y1_SHIFT                 (8U)
/*! Y1 - minor release version digit1: possible values 0-9 */
#define ELS_ELS_VERSION_Y1(x)                    (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_Y1_SHIFT)) & ELS_ELS_VERSION_Y1_MASK)

#define ELS_ELS_VERSION_X_MASK                   (0xF000U)
#define ELS_ELS_VERSION_X_SHIFT                  (12U)
/*! X - major release version: possible values 1-9 */
#define ELS_ELS_VERSION_X(x)                     (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_X_SHIFT)) & ELS_ELS_VERSION_X_MASK)

#define ELS_ELS_VERSION_SW_Z_MASK                (0xF0000U)
#define ELS_ELS_VERSION_SW_Z_SHIFT               (16U)
/*! SW_Z - software extended revision version: possible values 0-9 */
#define ELS_ELS_VERSION_SW_Z(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_SW_Z_SHIFT)) & ELS_ELS_VERSION_SW_Z_MASK)

#define ELS_ELS_VERSION_SW_Y2_MASK               (0xF00000U)
#define ELS_ELS_VERSION_SW_Y2_SHIFT              (20U)
/*! SW_Y2 - software minor release version digit0: possible values 0-9 */
#define ELS_ELS_VERSION_SW_Y2(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_SW_Y2_SHIFT)) & ELS_ELS_VERSION_SW_Y2_MASK)

#define ELS_ELS_VERSION_SW_Y1_MASK               (0xF000000U)
#define ELS_ELS_VERSION_SW_Y1_SHIFT              (24U)
/*! SW_Y1 - software minor release version digit1: possible values 0-9 */
#define ELS_ELS_VERSION_SW_Y1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_SW_Y1_SHIFT)) & ELS_ELS_VERSION_SW_Y1_MASK)

#define ELS_ELS_VERSION_SW_X_MASK                (0xF0000000U)
#define ELS_ELS_VERSION_SW_X_SHIFT               (28U)
/*! SW_X - software major release version: possible values 1-9 */
#define ELS_ELS_VERSION_SW_X(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_VERSION_SW_X_SHIFT)) & ELS_ELS_VERSION_SW_X_MASK)
/*! @} */

/*! @name ELS_CONFIG - ELS Config */
/*! @{ */

#define ELS_ELS_CONFIG_CIPHER_SUP_MASK           (0x1U)
#define ELS_ELS_CONFIG_CIPHER_SUP_SHIFT          (0U)
/*! CIPHER_SUP - cipher command is supported */
#define ELS_ELS_CONFIG_CIPHER_SUP(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_CIPHER_SUP_SHIFT)) & ELS_ELS_CONFIG_CIPHER_SUP_MASK)

#define ELS_ELS_CONFIG_AUTH_CIPHER_SUP_MASK      (0x2U)
#define ELS_ELS_CONFIG_AUTH_CIPHER_SUP_SHIFT     (1U)
/*! AUTH_CIPHER_SUP - auth_cipher command is supported */
#define ELS_ELS_CONFIG_AUTH_CIPHER_SUP(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_AUTH_CIPHER_SUP_SHIFT)) & ELS_ELS_CONFIG_AUTH_CIPHER_SUP_MASK)

#define ELS_ELS_CONFIG_ECSIGN_SUP_MASK           (0x4U)
#define ELS_ELS_CONFIG_ECSIGN_SUP_SHIFT          (2U)
/*! ECSIGN_SUP - ecsign command is supported */
#define ELS_ELS_CONFIG_ECSIGN_SUP(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_ECSIGN_SUP_SHIFT)) & ELS_ELS_CONFIG_ECSIGN_SUP_MASK)

#define ELS_ELS_CONFIG_ECVFY_SUP_MASK            (0x8U)
#define ELS_ELS_CONFIG_ECVFY_SUP_SHIFT           (3U)
/*! ECVFY_SUP - ecvfy command is supported */
#define ELS_ELS_CONFIG_ECVFY_SUP(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_ECVFY_SUP_SHIFT)) & ELS_ELS_CONFIG_ECVFY_SUP_MASK)

#define ELS_ELS_CONFIG_ECKXCH_SUP_MASK           (0x10U)
#define ELS_ELS_CONFIG_ECKXCH_SUP_SHIFT          (4U)
/*! ECKXCH_SUP - dhkey_xch command is supported */
#define ELS_ELS_CONFIG_ECKXCH_SUP(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_ECKXCH_SUP_SHIFT)) & ELS_ELS_CONFIG_ECKXCH_SUP_MASK)

#define ELS_ELS_CONFIG_KEYGEN_SUP_MASK           (0x20U)
#define ELS_ELS_CONFIG_KEYGEN_SUP_SHIFT          (5U)
/*! KEYGEN_SUP - keygen command is supported */
#define ELS_ELS_CONFIG_KEYGEN_SUP(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_KEYGEN_SUP_SHIFT)) & ELS_ELS_CONFIG_KEYGEN_SUP_MASK)

#define ELS_ELS_CONFIG_KEYIN_SUP_MASK            (0x40U)
#define ELS_ELS_CONFIG_KEYIN_SUP_SHIFT           (6U)
/*! KEYIN_SUP - keyin command is supported */
#define ELS_ELS_CONFIG_KEYIN_SUP(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_KEYIN_SUP_SHIFT)) & ELS_ELS_CONFIG_KEYIN_SUP_MASK)

#define ELS_ELS_CONFIG_KEYOUT_SUP_MASK           (0x80U)
#define ELS_ELS_CONFIG_KEYOUT_SUP_SHIFT          (7U)
/*! KEYOUT_SUP - keyout command is supported */
#define ELS_ELS_CONFIG_KEYOUT_SUP(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_KEYOUT_SUP_SHIFT)) & ELS_ELS_CONFIG_KEYOUT_SUP_MASK)

#define ELS_ELS_CONFIG_KDELETE_SUP_MASK          (0x100U)
#define ELS_ELS_CONFIG_KDELETE_SUP_SHIFT         (8U)
/*! KDELETE_SUP - kdelete command is supported */
#define ELS_ELS_CONFIG_KDELETE_SUP(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_KDELETE_SUP_SHIFT)) & ELS_ELS_CONFIG_KDELETE_SUP_MASK)

#define ELS_ELS_CONFIG_KEYPROV_SUP_MASK          (0x200U)
#define ELS_ELS_CONFIG_KEYPROV_SUP_SHIFT         (9U)
/*! KEYPROV_SUP - keyprov command is supported */
#define ELS_ELS_CONFIG_KEYPROV_SUP(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_KEYPROV_SUP_SHIFT)) & ELS_ELS_CONFIG_KEYPROV_SUP_MASK)

#define ELS_ELS_CONFIG_CKDF_SUP_MASK             (0x400U)
#define ELS_ELS_CONFIG_CKDF_SUP_SHIFT            (10U)
/*! CKDF_SUP - ckdf command is supported */
#define ELS_ELS_CONFIG_CKDF_SUP(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_CKDF_SUP_SHIFT)) & ELS_ELS_CONFIG_CKDF_SUP_MASK)

#define ELS_ELS_CONFIG_HKDF_SUP_MASK             (0x800U)
#define ELS_ELS_CONFIG_HKDF_SUP_SHIFT            (11U)
/*! HKDF_SUP - hkdf command is supported */
#define ELS_ELS_CONFIG_HKDF_SUP(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_HKDF_SUP_SHIFT)) & ELS_ELS_CONFIG_HKDF_SUP_MASK)

#define ELS_ELS_CONFIG_TLS_INIT_SUP_MASK         (0x1000U)
#define ELS_ELS_CONFIG_TLS_INIT_SUP_SHIFT        (12U)
/*! TLS_INIT_SUP - tls_init command is supported */
#define ELS_ELS_CONFIG_TLS_INIT_SUP(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_TLS_INIT_SUP_SHIFT)) & ELS_ELS_CONFIG_TLS_INIT_SUP_MASK)

#define ELS_ELS_CONFIG_HASH_SUP_MASK             (0x2000U)
#define ELS_ELS_CONFIG_HASH_SUP_SHIFT            (13U)
/*! HASH_SUP - hash command is supported */
#define ELS_ELS_CONFIG_HASH_SUP(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_HASH_SUP_SHIFT)) & ELS_ELS_CONFIG_HASH_SUP_MASK)

#define ELS_ELS_CONFIG_HMAC_SUP_MASK             (0x4000U)
#define ELS_ELS_CONFIG_HMAC_SUP_SHIFT            (14U)
/*! HMAC_SUP - hmac command is supported */
#define ELS_ELS_CONFIG_HMAC_SUP(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_HMAC_SUP_SHIFT)) & ELS_ELS_CONFIG_HMAC_SUP_MASK)

#define ELS_ELS_CONFIG_CMAC_SUP_MASK             (0x8000U)
#define ELS_ELS_CONFIG_CMAC_SUP_SHIFT            (15U)
/*! CMAC_SUP - cmac command is supported */
#define ELS_ELS_CONFIG_CMAC_SUP(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_CMAC_SUP_SHIFT)) & ELS_ELS_CONFIG_CMAC_SUP_MASK)

#define ELS_ELS_CONFIG_DRBG_REQ_SUP_MASK         (0x10000U)
#define ELS_ELS_CONFIG_DRBG_REQ_SUP_SHIFT        (16U)
/*! DRBG_REQ_SUP - drbg_req command is supported */
#define ELS_ELS_CONFIG_DRBG_REQ_SUP(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_DRBG_REQ_SUP_SHIFT)) & ELS_ELS_CONFIG_DRBG_REQ_SUP_MASK)

#define ELS_ELS_CONFIG_DRBG_TEST_SUP_MASK        (0x20000U)
#define ELS_ELS_CONFIG_DRBG_TEST_SUP_SHIFT       (17U)
/*! DRBG_TEST_SUP - drbg_test command is supported */
#define ELS_ELS_CONFIG_DRBG_TEST_SUP(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_DRBG_TEST_SUP_SHIFT)) & ELS_ELS_CONFIG_DRBG_TEST_SUP_MASK)

#define ELS_ELS_CONFIG_DTRNG_CFG_LOAD_SUP_MASK   (0x40000U)
#define ELS_ELS_CONFIG_DTRNG_CFG_LOAD_SUP_SHIFT  (18U)
/*! DTRNG_CFG_LOAD_SUP - dtrng_cfg_load command is supported */
#define ELS_ELS_CONFIG_DTRNG_CFG_LOAD_SUP(x)     (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_DTRNG_CFG_LOAD_SUP_SHIFT)) & ELS_ELS_CONFIG_DTRNG_CFG_LOAD_SUP_MASK)

#define ELS_ELS_CONFIG_DTRNG_EVAL_SUP_MASK       (0x80000U)
#define ELS_ELS_CONFIG_DTRNG_EVAL_SUP_SHIFT      (19U)
/*! DTRNG_EVAL_SUP - dtrng_eval command is supported */
#define ELS_ELS_CONFIG_DTRNG_EVAL_SUP(x)         (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_DTRNG_EVAL_SUP_SHIFT)) & ELS_ELS_CONFIG_DTRNG_EVAL_SUP_MASK)

#define ELS_ELS_CONFIG_GDET_CFG_LOAD_SUP_MASK    (0x100000U)
#define ELS_ELS_CONFIG_GDET_CFG_LOAD_SUP_SHIFT   (20U)
/*! GDET_CFG_LOAD_SUP - gdet_cfg_load command is supported */
#define ELS_ELS_CONFIG_GDET_CFG_LOAD_SUP(x)      (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_GDET_CFG_LOAD_SUP_SHIFT)) & ELS_ELS_CONFIG_GDET_CFG_LOAD_SUP_MASK)

#define ELS_ELS_CONFIG_GDET_TRIM_SUP_MASK        (0x200000U)
#define ELS_ELS_CONFIG_GDET_TRIM_SUP_SHIFT       (21U)
/*! GDET_TRIM_SUP - gdet_trim command is supported */
#define ELS_ELS_CONFIG_GDET_TRIM_SUP(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_GDET_TRIM_SUP_SHIFT)) & ELS_ELS_CONFIG_GDET_TRIM_SUP_MASK)

#define ELS_ELS_CONFIG_CONFIG_RSVD_MASK          (0xFFC00000U)
#define ELS_ELS_CONFIG_CONFIG_RSVD_SHIFT         (22U)
/*! CONFIG_RSVD - reserved */
#define ELS_ELS_CONFIG_CONFIG_RSVD(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_CONFIG_CONFIG_RSVD_SHIFT)) & ELS_ELS_CONFIG_CONFIG_RSVD_MASK)
/*! @} */

/*! @name ELS_PRNG_DATOUT - PRNG SW read out register */
/*! @{ */

#define ELS_ELS_PRNG_DATOUT_PRNG_DATOUT_MASK     (0xFFFFFFFFU)
#define ELS_ELS_PRNG_DATOUT_PRNG_DATOUT_SHIFT    (0U)
/*! PRNG_DATOUT - 32-bit wide pseudo-random number */
#define ELS_ELS_PRNG_DATOUT_PRNG_DATOUT(x)       (((uint32_t)(((uint32_t)(x)) << ELS_ELS_PRNG_DATOUT_PRNG_DATOUT_SHIFT)) & ELS_ELS_PRNG_DATOUT_PRNG_DATOUT_MASK)
/*! @} */

/*! @name ELS_GDET_EVTCNT - ELS GDET Event Counter */
/*! @{ */

#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_MASK     (0xFFU)
#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_SHIFT    (0U)
/*! GDET_EVTCNT - Number of glitch event recorded */
#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT(x)       (((uint32_t)(((uint32_t)(x)) << ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_SHIFT)) & ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_MASK)

#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_CLR_DONE_MASK (0x100U)
#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_CLR_DONE_SHIFT (8U)
/*! GDET_EVTCNT_CLR_DONE - The GDET event counter has been cleared */
#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_CLR_DONE(x) (((uint32_t)(((uint32_t)(x)) << ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_CLR_DONE_SHIFT)) & ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_CLR_DONE_MASK)

#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_RSVD_MASK (0xFFFFFE00U)
#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_RSVD_SHIFT (9U)
/*! GDET_EVTCNT_RSVD - reserved */
#define ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_RSVD(x)  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_RSVD_SHIFT)) & ELS_ELS_GDET_EVTCNT_GDET_EVTCNT_RSVD_MASK)
/*! @} */

/*! @name ELS_GDET_EVTCNT_CLR - ELS GDET Event Counter Clear */
/*! @{ */

#define ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_MASK (0x1U)
#define ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_SHIFT (0U)
/*! GDET_EVTCNT_CLR - 1=clear GDET event counter clear */
#define ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR(x) (((uint32_t)(((uint32_t)(x)) << ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_SHIFT)) & ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_MASK)

#define ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_RSVD_MASK (0xFFFFFFFEU)
#define ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_RSVD_SHIFT (1U)
/*! GDET_EVTCNT_CLR_RSVD - reserved */
#define ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_RSVD(x) (((uint32_t)(((uint32_t)(x)) << ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_RSVD_SHIFT)) & ELS_ELS_GDET_EVTCNT_CLR_GDET_EVTCNT_CLR_RSVD_MASK)
/*! @} */

/*! @name ELS_SHA2_STATUS - ELS SHA2 Status Register */
/*! @{ */

#define ELS_ELS_SHA2_STATUS_SHA2_BUSY_MASK       (0x1U)
#define ELS_ELS_SHA2_STATUS_SHA2_BUSY_SHIFT      (0U)
#define ELS_ELS_SHA2_STATUS_SHA2_BUSY(x)         (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_STATUS_SHA2_BUSY_SHIFT)) & ELS_ELS_SHA2_STATUS_SHA2_BUSY_MASK)

#define ELS_ELS_SHA2_STATUS_STATUS_RSVD1_MASK    (0xFFFFFFFEU)
#define ELS_ELS_SHA2_STATUS_STATUS_RSVD1_SHIFT   (1U)
/*! STATUS_RSVD1 - reserved */
#define ELS_ELS_SHA2_STATUS_STATUS_RSVD1(x)      (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_STATUS_STATUS_RSVD1_SHIFT)) & ELS_ELS_SHA2_STATUS_STATUS_RSVD1_MASK)
/*! @} */

/*! @name ELS_SHA2_CTRL - SHA2 Control register */
/*! @{ */

#define ELS_ELS_SHA2_CTRL_SHA2_START_MASK        (0x1U)
#define ELS_ELS_SHA2_CTRL_SHA2_START_SHIFT       (0U)
/*! SHA2_START - Write to 1 to Init the SHA2 Module */
#define ELS_ELS_SHA2_CTRL_SHA2_START(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_SHA2_START_SHIFT)) & ELS_ELS_SHA2_CTRL_SHA2_START_MASK)

#define ELS_ELS_SHA2_CTRL_SHA2_RST_MASK          (0x2U)
#define ELS_ELS_SHA2_CTRL_SHA2_RST_SHIFT         (1U)
/*! SHA2_RST - Write to 1 to Reset a SHA2 operation */
#define ELS_ELS_SHA2_CTRL_SHA2_RST(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_SHA2_RST_SHIFT)) & ELS_ELS_SHA2_CTRL_SHA2_RST_MASK)

#define ELS_ELS_SHA2_CTRL_SHA2_INIT_MASK         (0x4U)
#define ELS_ELS_SHA2_CTRL_SHA2_INIT_SHIFT        (2U)
/*! SHA2_INIT - Write to 1 to Init the SHA2 Kernel */
#define ELS_ELS_SHA2_CTRL_SHA2_INIT(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_SHA2_INIT_SHIFT)) & ELS_ELS_SHA2_CTRL_SHA2_INIT_MASK)

#define ELS_ELS_SHA2_CTRL_SHA2_LOAD_MASK         (0x8U)
#define ELS_ELS_SHA2_CTRL_SHA2_LOAD_SHIFT        (3U)
/*! SHA2_LOAD - Write to 1 to Load the SHA2 Kernel */
#define ELS_ELS_SHA2_CTRL_SHA2_LOAD(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_SHA2_LOAD_SHIFT)) & ELS_ELS_SHA2_CTRL_SHA2_LOAD_MASK)

#define ELS_ELS_SHA2_CTRL_SHA2_MODE_MASK         (0x30U)
#define ELS_ELS_SHA2_CTRL_SHA2_MODE_SHIFT        (4U)
/*! SHA2_MODE - SHA2 MODE:; 2'b11 - SHA512; 2'b10 - SHA384; 2'b01 - SHA224; 2'b00 - SHA256 */
#define ELS_ELS_SHA2_CTRL_SHA2_MODE(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_SHA2_MODE_SHIFT)) & ELS_ELS_SHA2_CTRL_SHA2_MODE_MASK)

#define ELS_ELS_SHA2_CTRL_CTRL_RSVD1_MASK        (0x1C0U)
#define ELS_ELS_SHA2_CTRL_CTRL_RSVD1_SHIFT       (6U)
/*! CTRL_RSVD1 - r-eserved */
#define ELS_ELS_SHA2_CTRL_CTRL_RSVD1(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_CTRL_RSVD1_SHIFT)) & ELS_ELS_SHA2_CTRL_CTRL_RSVD1_MASK)

#define ELS_ELS_SHA2_CTRL_SHA2_BYTE_ORDER_MASK   (0x200U)
#define ELS_ELS_SHA2_CTRL_SHA2_BYTE_ORDER_SHIFT  (9U)
/*! SHA2_BYTE_ORDER - Write to 1 to Reverse byte endianess */
#define ELS_ELS_SHA2_CTRL_SHA2_BYTE_ORDER(x)     (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_SHA2_BYTE_ORDER_SHIFT)) & ELS_ELS_SHA2_CTRL_SHA2_BYTE_ORDER_MASK)

#define ELS_ELS_SHA2_CTRL_CTRL_RSVD_MASK         (0xFFFFFC00U)
#define ELS_ELS_SHA2_CTRL_CTRL_RSVD_SHIFT        (10U)
/*! CTRL_RSVD - r-eserved */
#define ELS_ELS_SHA2_CTRL_CTRL_RSVD(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_CTRL_CTRL_RSVD_SHIFT)) & ELS_ELS_SHA2_CTRL_CTRL_RSVD_MASK)
/*! @} */

/*! @name ELS_SHA2_DIN - ELS SHA_DATA IN Register 0 */
/*! @{ */

#define ELS_ELS_SHA2_DIN_SHA_DATIN_MASK          (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DIN_SHA_DATIN_SHIFT         (0U)
/*! SHA_DATIN - Output ELS_SHA_DATIN from ELS Application being executed */
#define ELS_ELS_SHA2_DIN_SHA_DATIN(x)            (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DIN_SHA_DATIN_SHIFT)) & ELS_ELS_SHA2_DIN_SHA_DATIN_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT0 - ELS ELS_SHA_DATA Out Register 0 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT0_SHA_DATA0_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT0_SHA_DATA0_SHIFT       (0U)
/*! SHA_DATA0 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT0_SHA_DATA0(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT0_SHA_DATA0_SHIFT)) & ELS_ELS_SHA2_DOUT0_SHA_DATA0_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT1 - ELS SHA_DATA Out Register 1 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT1_SHA_DATA1_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT1_SHA_DATA1_SHIFT       (0U)
/*! SHA_DATA1 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT1_SHA_DATA1(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT1_SHA_DATA1_SHIFT)) & ELS_ELS_SHA2_DOUT1_SHA_DATA1_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT2 - ELS SHA_DATA Out Register 2 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT2_SHA_DATA2_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT2_SHA_DATA2_SHIFT       (0U)
/*! SHA_DATA2 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT2_SHA_DATA2(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT2_SHA_DATA2_SHIFT)) & ELS_ELS_SHA2_DOUT2_SHA_DATA2_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT3 - ELS SHA_DATA Out Register 3 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT3_SHA_DATA3_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT3_SHA_DATA3_SHIFT       (0U)
/*! SHA_DATA3 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT3_SHA_DATA3(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT3_SHA_DATA3_SHIFT)) & ELS_ELS_SHA2_DOUT3_SHA_DATA3_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT4 - ELS SHA_DATA Out Register 4 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT4_SHA_DATA4_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT4_SHA_DATA4_SHIFT       (0U)
/*! SHA_DATA4 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT4_SHA_DATA4(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT4_SHA_DATA4_SHIFT)) & ELS_ELS_SHA2_DOUT4_SHA_DATA4_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT5 - ELS SHA_DATA Out Register 5 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT5_SHA_DATA5_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT5_SHA_DATA5_SHIFT       (0U)
/*! SHA_DATA5 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT5_SHA_DATA5(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT5_SHA_DATA5_SHIFT)) & ELS_ELS_SHA2_DOUT5_SHA_DATA5_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT6 - ELS SHA_DATA Out Register 6 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT6_SHA_DATA6_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT6_SHA_DATA6_SHIFT       (0U)
/*! SHA_DATA6 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT6_SHA_DATA6(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT6_SHA_DATA6_SHIFT)) & ELS_ELS_SHA2_DOUT6_SHA_DATA6_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT7 - ELS SHA_DATA Out Register 7 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT7_SHA_DATA7_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT7_SHA_DATA7_SHIFT       (0U)
/*! SHA_DATA7 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT7_SHA_DATA7(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT7_SHA_DATA7_SHIFT)) & ELS_ELS_SHA2_DOUT7_SHA_DATA7_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT8 - ELS ELS_SHA_DATA Out Register 8 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT8_SHA_DATA8_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT8_SHA_DATA8_SHIFT       (0U)
/*! SHA_DATA8 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT8_SHA_DATA8(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT8_SHA_DATA8_SHIFT)) & ELS_ELS_SHA2_DOUT8_SHA_DATA8_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT9 - ELS SHA_DATA Out Register 9 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT9_SHA_DATA9_MASK        (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT9_SHA_DATA9_SHIFT       (0U)
/*! SHA_DATA9 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT9_SHA_DATA9(x)          (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT9_SHA_DATA9_SHIFT)) & ELS_ELS_SHA2_DOUT9_SHA_DATA9_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT10 - ELS SHA_DATA Out Register 10 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT10_SHA_DATA10_MASK      (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT10_SHA_DATA10_SHIFT     (0U)
/*! SHA_DATA10 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT10_SHA_DATA10(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT10_SHA_DATA10_SHIFT)) & ELS_ELS_SHA2_DOUT10_SHA_DATA10_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT11 - ELS SHA_DATA Out Register 11 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT11_SHA_DATA11_MASK      (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT11_SHA_DATA11_SHIFT     (0U)
/*! SHA_DATA11 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT11_SHA_DATA11(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT11_SHA_DATA11_SHIFT)) & ELS_ELS_SHA2_DOUT11_SHA_DATA11_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT12 - ELS SHA_DATA Out Register 12 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT12_SHA_DATA12_MASK      (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT12_SHA_DATA12_SHIFT     (0U)
/*! SHA_DATA12 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT12_SHA_DATA12(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT12_SHA_DATA12_SHIFT)) & ELS_ELS_SHA2_DOUT12_SHA_DATA12_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT13 - ELS SHA_DATA Out Register 13 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT13_SHA_DATA13_MASK      (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT13_SHA_DATA13_SHIFT     (0U)
/*! SHA_DATA13 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT13_SHA_DATA13(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT13_SHA_DATA13_SHIFT)) & ELS_ELS_SHA2_DOUT13_SHA_DATA13_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT14 - ELS SHA_DATA Out Register 14 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT14_SHA_DATA14_MASK      (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT14_SHA_DATA14_SHIFT     (0U)
/*! SHA_DATA14 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT14_SHA_DATA14(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT14_SHA_DATA14_SHIFT)) & ELS_ELS_SHA2_DOUT14_SHA_DATA14_MASK)
/*! @} */

/*! @name ELS_SHA2_DOUT15 - ELS SHA_DATA Out Register 15 */
/*! @{ */

#define ELS_ELS_SHA2_DOUT15_SHA_DATA15_MASK      (0xFFFFFFFFU)
#define ELS_ELS_SHA2_DOUT15_SHA_DATA15_SHIFT     (0U)
/*! SHA_DATA15 - Output SHA_DATA from ELS Application being executed */
#define ELS_ELS_SHA2_DOUT15_SHA_DATA15(x)        (((uint32_t)(((uint32_t)(x)) << ELS_ELS_SHA2_DOUT15_SHA_DATA15_SHIFT)) & ELS_ELS_SHA2_DOUT15_SHA_DATA15_MASK)
/*! @} */

/*! @name ELS_KS0 - Status register */
/*! @{ */

#define ELS_ELS_KS0_KS0_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS0_KS0_KSIZE_SHIFT              (0U)
/*! KS0_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS0_KS0_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_KSIZE_SHIFT)) & ELS_ELS_KS0_KS0_KSIZE_MASK)

#define ELS_ELS_KS0_KS0_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS0_KS0_RSVD0_SHIFT              (1U)
/*! KS0_RSVD0 - Reserved 0 */
#define ELS_ELS_KS0_KS0_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_RSVD0_SHIFT)) & ELS_ELS_KS0_KS0_RSVD0_MASK)

#define ELS_ELS_KS0_KS0_KACT_MASK                (0x20U)
#define ELS_ELS_KS0_KS0_KACT_SHIFT               (5U)
/*! KS0_KACT - Key is active */
#define ELS_ELS_KS0_KS0_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_KACT_SHIFT)) & ELS_ELS_KS0_KS0_KACT_MASK)

#define ELS_ELS_KS0_KS0_KBASE_MASK               (0x40U)
#define ELS_ELS_KS0_KS0_KBASE_SHIFT              (6U)
/*! KS0_KBASE - First slot in a multislot key */
#define ELS_ELS_KS0_KS0_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_KBASE_SHIFT)) & ELS_ELS_KS0_KS0_KBASE_MASK)

#define ELS_ELS_KS0_KS0_FGP_MASK                 (0x80U)
#define ELS_ELS_KS0_KS0_FGP_SHIFT                (7U)
/*! KS0_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS0_KS0_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_FGP_SHIFT)) & ELS_ELS_KS0_KS0_FGP_MASK)

#define ELS_ELS_KS0_KS0_FRTN_MASK                (0x100U)
#define ELS_ELS_KS0_KS0_FRTN_SHIFT               (8U)
/*! KS0_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS0_KS0_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_FRTN_SHIFT)) & ELS_ELS_KS0_KS0_FRTN_MASK)

#define ELS_ELS_KS0_KS0_FHWO_MASK                (0x200U)
#define ELS_ELS_KS0_KS0_FHWO_SHIFT               (9U)
/*! KS0_FHWO - Hardware Feature Output */
#define ELS_ELS_KS0_KS0_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_FHWO_SHIFT)) & ELS_ELS_KS0_KS0_FHWO_MASK)

#define ELS_ELS_KS0_KS0_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS0_KS0_RSVD1_SHIFT              (10U)
/*! KS0_RSVD1 - Reserved 1 */
#define ELS_ELS_KS0_KS0_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_RSVD1_SHIFT)) & ELS_ELS_KS0_KS0_RSVD1_MASK)

#define ELS_ELS_KS0_KS0_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS0_KS0_UCMAC_SHIFT              (13U)
/*! KS0_UCMAC - CMAC key */
#define ELS_ELS_KS0_KS0_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UCMAC_SHIFT)) & ELS_ELS_KS0_KS0_UCMAC_MASK)

#define ELS_ELS_KS0_KS0_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS0_KS0_UKSK_SHIFT               (14U)
/*! KS0_UKSK - KSK key */
#define ELS_ELS_KS0_KS0_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UKSK_SHIFT)) & ELS_ELS_KS0_KS0_UKSK_MASK)

#define ELS_ELS_KS0_KS0_URTF_MASK                (0x8000U)
#define ELS_ELS_KS0_KS0_URTF_SHIFT               (15U)
/*! KS0_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS0_KS0_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_URTF_SHIFT)) & ELS_ELS_KS0_KS0_URTF_MASK)

#define ELS_ELS_KS0_KS0_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS0_KS0_UCKDF_SHIFT              (16U)
/*! KS0_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS0_KS0_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UCKDF_SHIFT)) & ELS_ELS_KS0_KS0_UCKDF_MASK)

#define ELS_ELS_KS0_KS0_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS0_KS0_UHKDF_SHIFT              (17U)
/*! KS0_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS0_KS0_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UHKDF_SHIFT)) & ELS_ELS_KS0_KS0_UHKDF_MASK)

#define ELS_ELS_KS0_KS0_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS0_KS0_UECSG_SHIFT              (18U)
/*! KS0_UECSG - Ecc signing key */
#define ELS_ELS_KS0_KS0_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UECSG_SHIFT)) & ELS_ELS_KS0_KS0_UECSG_MASK)

#define ELS_ELS_KS0_KS0_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS0_KS0_UECDH_SHIFT              (19U)
/*! KS0_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS0_KS0_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UECDH_SHIFT)) & ELS_ELS_KS0_KS0_UECDH_MASK)

#define ELS_ELS_KS0_KS0_UAES_MASK                (0x100000U)
#define ELS_ELS_KS0_KS0_UAES_SHIFT               (20U)
/*! KS0_UAES - Aes key */
#define ELS_ELS_KS0_KS0_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UAES_SHIFT)) & ELS_ELS_KS0_KS0_UAES_MASK)

#define ELS_ELS_KS0_KS0_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS0_KS0_UHMAC_SHIFT              (21U)
/*! KS0_UHMAC - Hmac key */
#define ELS_ELS_KS0_KS0_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UHMAC_SHIFT)) & ELS_ELS_KS0_KS0_UHMAC_MASK)

#define ELS_ELS_KS0_KS0_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS0_KS0_UKWK_SHIFT               (22U)
/*! KS0_UKWK - Key wrapping key */
#define ELS_ELS_KS0_KS0_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UKWK_SHIFT)) & ELS_ELS_KS0_KS0_UKWK_MASK)

#define ELS_ELS_KS0_KS0_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS0_KS0_UKUOK_SHIFT              (23U)
/*! KS0_UKUOK - Key unwrapping key */
#define ELS_ELS_KS0_KS0_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UKUOK_SHIFT)) & ELS_ELS_KS0_KS0_UKUOK_MASK)

#define ELS_ELS_KS0_KS0_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS0_KS0_UTLSPMS_SHIFT            (24U)
/*! KS0_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS0_KS0_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UTLSPMS_SHIFT)) & ELS_ELS_KS0_KS0_UTLSPMS_MASK)

#define ELS_ELS_KS0_KS0_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS0_KS0_UTLSMS_SHIFT             (25U)
/*! KS0_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS0_KS0_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UTLSMS_SHIFT)) & ELS_ELS_KS0_KS0_UTLSMS_MASK)

#define ELS_ELS_KS0_KS0_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS0_KS0_UKGSRC_SHIFT             (26U)
/*! KS0_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS0_KS0_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UKGSRC_SHIFT)) & ELS_ELS_KS0_KS0_UKGSRC_MASK)

#define ELS_ELS_KS0_KS0_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS0_KS0_UHWO_SHIFT               (27U)
/*! KS0_UHWO - Hardware out key */
#define ELS_ELS_KS0_KS0_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UHWO_SHIFT)) & ELS_ELS_KS0_KS0_UHWO_MASK)

#define ELS_ELS_KS0_KS0_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS0_KS0_UWRPOK_SHIFT             (28U)
/*! KS0_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS0_KS0_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UWRPOK_SHIFT)) & ELS_ELS_KS0_KS0_UWRPOK_MASK)

#define ELS_ELS_KS0_KS0_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS0_KS0_UDUK_SHIFT               (29U)
/*! KS0_UDUK - Device Unique Key */
#define ELS_ELS_KS0_KS0_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UDUK_SHIFT)) & ELS_ELS_KS0_KS0_UDUK_MASK)

#define ELS_ELS_KS0_KS0_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS0_KS0_UPPROT_SHIFT             (30U)
/*! KS0_UPPROT - Priviledge level */
#define ELS_ELS_KS0_KS0_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS0_KS0_UPPROT_SHIFT)) & ELS_ELS_KS0_KS0_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS1 - Status register */
/*! @{ */

#define ELS_ELS_KS1_KS1_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS1_KS1_KSIZE_SHIFT              (0U)
/*! KS1_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS1_KS1_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_KSIZE_SHIFT)) & ELS_ELS_KS1_KS1_KSIZE_MASK)

#define ELS_ELS_KS1_KS1_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS1_KS1_RSVD0_SHIFT              (1U)
/*! KS1_RSVD0 - Reserved 0 */
#define ELS_ELS_KS1_KS1_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_RSVD0_SHIFT)) & ELS_ELS_KS1_KS1_RSVD0_MASK)

#define ELS_ELS_KS1_KS1_KACT_MASK                (0x20U)
#define ELS_ELS_KS1_KS1_KACT_SHIFT               (5U)
/*! KS1_KACT - Key is active */
#define ELS_ELS_KS1_KS1_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_KACT_SHIFT)) & ELS_ELS_KS1_KS1_KACT_MASK)

#define ELS_ELS_KS1_KS1_KBASE_MASK               (0x40U)
#define ELS_ELS_KS1_KS1_KBASE_SHIFT              (6U)
/*! KS1_KBASE - First slot in a multislot key */
#define ELS_ELS_KS1_KS1_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_KBASE_SHIFT)) & ELS_ELS_KS1_KS1_KBASE_MASK)

#define ELS_ELS_KS1_KS1_FGP_MASK                 (0x80U)
#define ELS_ELS_KS1_KS1_FGP_SHIFT                (7U)
/*! KS1_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS1_KS1_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_FGP_SHIFT)) & ELS_ELS_KS1_KS1_FGP_MASK)

#define ELS_ELS_KS1_KS1_FRTN_MASK                (0x100U)
#define ELS_ELS_KS1_KS1_FRTN_SHIFT               (8U)
/*! KS1_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS1_KS1_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_FRTN_SHIFT)) & ELS_ELS_KS1_KS1_FRTN_MASK)

#define ELS_ELS_KS1_KS1_FHWO_MASK                (0x200U)
#define ELS_ELS_KS1_KS1_FHWO_SHIFT               (9U)
/*! KS1_FHWO - Hardware Feature Output */
#define ELS_ELS_KS1_KS1_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_FHWO_SHIFT)) & ELS_ELS_KS1_KS1_FHWO_MASK)

#define ELS_ELS_KS1_KS1_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS1_KS1_RSVD1_SHIFT              (10U)
/*! KS1_RSVD1 - Reserved 1 */
#define ELS_ELS_KS1_KS1_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_RSVD1_SHIFT)) & ELS_ELS_KS1_KS1_RSVD1_MASK)

#define ELS_ELS_KS1_KS1_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS1_KS1_UCMAC_SHIFT              (13U)
/*! KS1_UCMAC - CMAC key */
#define ELS_ELS_KS1_KS1_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UCMAC_SHIFT)) & ELS_ELS_KS1_KS1_UCMAC_MASK)

#define ELS_ELS_KS1_KS1_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS1_KS1_UKSK_SHIFT               (14U)
/*! KS1_UKSK - KSK key */
#define ELS_ELS_KS1_KS1_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UKSK_SHIFT)) & ELS_ELS_KS1_KS1_UKSK_MASK)

#define ELS_ELS_KS1_KS1_URTF_MASK                (0x8000U)
#define ELS_ELS_KS1_KS1_URTF_SHIFT               (15U)
/*! KS1_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS1_KS1_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_URTF_SHIFT)) & ELS_ELS_KS1_KS1_URTF_MASK)

#define ELS_ELS_KS1_KS1_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS1_KS1_UCKDF_SHIFT              (16U)
/*! KS1_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS1_KS1_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UCKDF_SHIFT)) & ELS_ELS_KS1_KS1_UCKDF_MASK)

#define ELS_ELS_KS1_KS1_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS1_KS1_UHKDF_SHIFT              (17U)
/*! KS1_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS1_KS1_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UHKDF_SHIFT)) & ELS_ELS_KS1_KS1_UHKDF_MASK)

#define ELS_ELS_KS1_KS1_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS1_KS1_UECSG_SHIFT              (18U)
/*! KS1_UECSG - Ecc signing key */
#define ELS_ELS_KS1_KS1_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UECSG_SHIFT)) & ELS_ELS_KS1_KS1_UECSG_MASK)

#define ELS_ELS_KS1_KS1_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS1_KS1_UECDH_SHIFT              (19U)
/*! KS1_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS1_KS1_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UECDH_SHIFT)) & ELS_ELS_KS1_KS1_UECDH_MASK)

#define ELS_ELS_KS1_KS1_UAES_MASK                (0x100000U)
#define ELS_ELS_KS1_KS1_UAES_SHIFT               (20U)
/*! KS1_UAES - Aes key */
#define ELS_ELS_KS1_KS1_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UAES_SHIFT)) & ELS_ELS_KS1_KS1_UAES_MASK)

#define ELS_ELS_KS1_KS1_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS1_KS1_UHMAC_SHIFT              (21U)
/*! KS1_UHMAC - Hmac key */
#define ELS_ELS_KS1_KS1_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UHMAC_SHIFT)) & ELS_ELS_KS1_KS1_UHMAC_MASK)

#define ELS_ELS_KS1_KS1_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS1_KS1_UKWK_SHIFT               (22U)
/*! KS1_UKWK - Key wrapping key */
#define ELS_ELS_KS1_KS1_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UKWK_SHIFT)) & ELS_ELS_KS1_KS1_UKWK_MASK)

#define ELS_ELS_KS1_KS1_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS1_KS1_UKUOK_SHIFT              (23U)
/*! KS1_UKUOK - Key unwrapping key */
#define ELS_ELS_KS1_KS1_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UKUOK_SHIFT)) & ELS_ELS_KS1_KS1_UKUOK_MASK)

#define ELS_ELS_KS1_KS1_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS1_KS1_UTLSPMS_SHIFT            (24U)
/*! KS1_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS1_KS1_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UTLSPMS_SHIFT)) & ELS_ELS_KS1_KS1_UTLSPMS_MASK)

#define ELS_ELS_KS1_KS1_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS1_KS1_UTLSMS_SHIFT             (25U)
/*! KS1_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS1_KS1_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UTLSMS_SHIFT)) & ELS_ELS_KS1_KS1_UTLSMS_MASK)

#define ELS_ELS_KS1_KS1_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS1_KS1_UKGSRC_SHIFT             (26U)
/*! KS1_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS1_KS1_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UKGSRC_SHIFT)) & ELS_ELS_KS1_KS1_UKGSRC_MASK)

#define ELS_ELS_KS1_KS1_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS1_KS1_UHWO_SHIFT               (27U)
/*! KS1_UHWO - Hardware out key */
#define ELS_ELS_KS1_KS1_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UHWO_SHIFT)) & ELS_ELS_KS1_KS1_UHWO_MASK)

#define ELS_ELS_KS1_KS1_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS1_KS1_UWRPOK_SHIFT             (28U)
/*! KS1_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS1_KS1_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UWRPOK_SHIFT)) & ELS_ELS_KS1_KS1_UWRPOK_MASK)

#define ELS_ELS_KS1_KS1_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS1_KS1_UDUK_SHIFT               (29U)
/*! KS1_UDUK - Device Unique Key */
#define ELS_ELS_KS1_KS1_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UDUK_SHIFT)) & ELS_ELS_KS1_KS1_UDUK_MASK)

#define ELS_ELS_KS1_KS1_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS1_KS1_UPPROT_SHIFT             (30U)
/*! KS1_UPPROT - Priviledge level */
#define ELS_ELS_KS1_KS1_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS1_KS1_UPPROT_SHIFT)) & ELS_ELS_KS1_KS1_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS2 - Status register */
/*! @{ */

#define ELS_ELS_KS2_KS2_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS2_KS2_KSIZE_SHIFT              (0U)
/*! KS2_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS2_KS2_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_KSIZE_SHIFT)) & ELS_ELS_KS2_KS2_KSIZE_MASK)

#define ELS_ELS_KS2_KS2_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS2_KS2_RSVD0_SHIFT              (1U)
/*! KS2_RSVD0 - Reserved 0 */
#define ELS_ELS_KS2_KS2_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_RSVD0_SHIFT)) & ELS_ELS_KS2_KS2_RSVD0_MASK)

#define ELS_ELS_KS2_KS2_KACT_MASK                (0x20U)
#define ELS_ELS_KS2_KS2_KACT_SHIFT               (5U)
/*! KS2_KACT - Key is active */
#define ELS_ELS_KS2_KS2_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_KACT_SHIFT)) & ELS_ELS_KS2_KS2_KACT_MASK)

#define ELS_ELS_KS2_KS2_KBASE_MASK               (0x40U)
#define ELS_ELS_KS2_KS2_KBASE_SHIFT              (6U)
/*! KS2_KBASE - First slot in a multislot key */
#define ELS_ELS_KS2_KS2_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_KBASE_SHIFT)) & ELS_ELS_KS2_KS2_KBASE_MASK)

#define ELS_ELS_KS2_KS2_FGP_MASK                 (0x80U)
#define ELS_ELS_KS2_KS2_FGP_SHIFT                (7U)
/*! KS2_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS2_KS2_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_FGP_SHIFT)) & ELS_ELS_KS2_KS2_FGP_MASK)

#define ELS_ELS_KS2_KS2_FRTN_MASK                (0x100U)
#define ELS_ELS_KS2_KS2_FRTN_SHIFT               (8U)
/*! KS2_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS2_KS2_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_FRTN_SHIFT)) & ELS_ELS_KS2_KS2_FRTN_MASK)

#define ELS_ELS_KS2_KS2_FHWO_MASK                (0x200U)
#define ELS_ELS_KS2_KS2_FHWO_SHIFT               (9U)
/*! KS2_FHWO - Hardware Feature Output */
#define ELS_ELS_KS2_KS2_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_FHWO_SHIFT)) & ELS_ELS_KS2_KS2_FHWO_MASK)

#define ELS_ELS_KS2_KS2_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS2_KS2_RSVD1_SHIFT              (10U)
/*! KS2_RSVD1 - Reserved 1 */
#define ELS_ELS_KS2_KS2_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_RSVD1_SHIFT)) & ELS_ELS_KS2_KS2_RSVD1_MASK)

#define ELS_ELS_KS2_KS2_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS2_KS2_UCMAC_SHIFT              (13U)
/*! KS2_UCMAC - CMAC key */
#define ELS_ELS_KS2_KS2_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UCMAC_SHIFT)) & ELS_ELS_KS2_KS2_UCMAC_MASK)

#define ELS_ELS_KS2_KS2_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS2_KS2_UKSK_SHIFT               (14U)
/*! KS2_UKSK - KSK key */
#define ELS_ELS_KS2_KS2_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UKSK_SHIFT)) & ELS_ELS_KS2_KS2_UKSK_MASK)

#define ELS_ELS_KS2_KS2_URTF_MASK                (0x8000U)
#define ELS_ELS_KS2_KS2_URTF_SHIFT               (15U)
/*! KS2_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS2_KS2_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_URTF_SHIFT)) & ELS_ELS_KS2_KS2_URTF_MASK)

#define ELS_ELS_KS2_KS2_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS2_KS2_UCKDF_SHIFT              (16U)
/*! KS2_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS2_KS2_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UCKDF_SHIFT)) & ELS_ELS_KS2_KS2_UCKDF_MASK)

#define ELS_ELS_KS2_KS2_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS2_KS2_UHKDF_SHIFT              (17U)
/*! KS2_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS2_KS2_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UHKDF_SHIFT)) & ELS_ELS_KS2_KS2_UHKDF_MASK)

#define ELS_ELS_KS2_KS2_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS2_KS2_UECSG_SHIFT              (18U)
/*! KS2_UECSG - Ecc signing key */
#define ELS_ELS_KS2_KS2_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UECSG_SHIFT)) & ELS_ELS_KS2_KS2_UECSG_MASK)

#define ELS_ELS_KS2_KS2_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS2_KS2_UECDH_SHIFT              (19U)
/*! KS2_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS2_KS2_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UECDH_SHIFT)) & ELS_ELS_KS2_KS2_UECDH_MASK)

#define ELS_ELS_KS2_KS2_UAES_MASK                (0x100000U)
#define ELS_ELS_KS2_KS2_UAES_SHIFT               (20U)
/*! KS2_UAES - Aes key */
#define ELS_ELS_KS2_KS2_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UAES_SHIFT)) & ELS_ELS_KS2_KS2_UAES_MASK)

#define ELS_ELS_KS2_KS2_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS2_KS2_UHMAC_SHIFT              (21U)
/*! KS2_UHMAC - Hmac key */
#define ELS_ELS_KS2_KS2_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UHMAC_SHIFT)) & ELS_ELS_KS2_KS2_UHMAC_MASK)

#define ELS_ELS_KS2_KS2_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS2_KS2_UKWK_SHIFT               (22U)
/*! KS2_UKWK - Key wrapping key */
#define ELS_ELS_KS2_KS2_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UKWK_SHIFT)) & ELS_ELS_KS2_KS2_UKWK_MASK)

#define ELS_ELS_KS2_KS2_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS2_KS2_UKUOK_SHIFT              (23U)
/*! KS2_UKUOK - Key unwrapping key */
#define ELS_ELS_KS2_KS2_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UKUOK_SHIFT)) & ELS_ELS_KS2_KS2_UKUOK_MASK)

#define ELS_ELS_KS2_KS2_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS2_KS2_UTLSPMS_SHIFT            (24U)
/*! KS2_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS2_KS2_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UTLSPMS_SHIFT)) & ELS_ELS_KS2_KS2_UTLSPMS_MASK)

#define ELS_ELS_KS2_KS2_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS2_KS2_UTLSMS_SHIFT             (25U)
/*! KS2_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS2_KS2_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UTLSMS_SHIFT)) & ELS_ELS_KS2_KS2_UTLSMS_MASK)

#define ELS_ELS_KS2_KS2_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS2_KS2_UKGSRC_SHIFT             (26U)
/*! KS2_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS2_KS2_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UKGSRC_SHIFT)) & ELS_ELS_KS2_KS2_UKGSRC_MASK)

#define ELS_ELS_KS2_KS2_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS2_KS2_UHWO_SHIFT               (27U)
/*! KS2_UHWO - Hardware out key */
#define ELS_ELS_KS2_KS2_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UHWO_SHIFT)) & ELS_ELS_KS2_KS2_UHWO_MASK)

#define ELS_ELS_KS2_KS2_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS2_KS2_UWRPOK_SHIFT             (28U)
/*! KS2_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS2_KS2_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UWRPOK_SHIFT)) & ELS_ELS_KS2_KS2_UWRPOK_MASK)

#define ELS_ELS_KS2_KS2_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS2_KS2_UDUK_SHIFT               (29U)
/*! KS2_UDUK - Device Unique Key */
#define ELS_ELS_KS2_KS2_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UDUK_SHIFT)) & ELS_ELS_KS2_KS2_UDUK_MASK)

#define ELS_ELS_KS2_KS2_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS2_KS2_UPPROT_SHIFT             (30U)
/*! KS2_UPPROT - Priviledge level */
#define ELS_ELS_KS2_KS2_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS2_KS2_UPPROT_SHIFT)) & ELS_ELS_KS2_KS2_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS3 - Status register */
/*! @{ */

#define ELS_ELS_KS3_KS3_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS3_KS3_KSIZE_SHIFT              (0U)
/*! KS3_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS3_KS3_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_KSIZE_SHIFT)) & ELS_ELS_KS3_KS3_KSIZE_MASK)

#define ELS_ELS_KS3_KS3_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS3_KS3_RSVD0_SHIFT              (1U)
/*! KS3_RSVD0 - Reserved 0 */
#define ELS_ELS_KS3_KS3_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_RSVD0_SHIFT)) & ELS_ELS_KS3_KS3_RSVD0_MASK)

#define ELS_ELS_KS3_KS3_KACT_MASK                (0x20U)
#define ELS_ELS_KS3_KS3_KACT_SHIFT               (5U)
/*! KS3_KACT - Key is active */
#define ELS_ELS_KS3_KS3_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_KACT_SHIFT)) & ELS_ELS_KS3_KS3_KACT_MASK)

#define ELS_ELS_KS3_KS3_KBASE_MASK               (0x40U)
#define ELS_ELS_KS3_KS3_KBASE_SHIFT              (6U)
/*! KS3_KBASE - First slot in a multislot key */
#define ELS_ELS_KS3_KS3_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_KBASE_SHIFT)) & ELS_ELS_KS3_KS3_KBASE_MASK)

#define ELS_ELS_KS3_KS3_FGP_MASK                 (0x80U)
#define ELS_ELS_KS3_KS3_FGP_SHIFT                (7U)
/*! KS3_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS3_KS3_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_FGP_SHIFT)) & ELS_ELS_KS3_KS3_FGP_MASK)

#define ELS_ELS_KS3_KS3_FRTN_MASK                (0x100U)
#define ELS_ELS_KS3_KS3_FRTN_SHIFT               (8U)
/*! KS3_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS3_KS3_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_FRTN_SHIFT)) & ELS_ELS_KS3_KS3_FRTN_MASK)

#define ELS_ELS_KS3_KS3_FHWO_MASK                (0x200U)
#define ELS_ELS_KS3_KS3_FHWO_SHIFT               (9U)
/*! KS3_FHWO - Hardware Feature Output */
#define ELS_ELS_KS3_KS3_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_FHWO_SHIFT)) & ELS_ELS_KS3_KS3_FHWO_MASK)

#define ELS_ELS_KS3_KS3_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS3_KS3_RSVD1_SHIFT              (10U)
/*! KS3_RSVD1 - Reserved 1 */
#define ELS_ELS_KS3_KS3_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_RSVD1_SHIFT)) & ELS_ELS_KS3_KS3_RSVD1_MASK)

#define ELS_ELS_KS3_KS3_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS3_KS3_UCMAC_SHIFT              (13U)
/*! KS3_UCMAC - CMAC key */
#define ELS_ELS_KS3_KS3_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UCMAC_SHIFT)) & ELS_ELS_KS3_KS3_UCMAC_MASK)

#define ELS_ELS_KS3_KS3_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS3_KS3_UKSK_SHIFT               (14U)
/*! KS3_UKSK - KSK key */
#define ELS_ELS_KS3_KS3_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UKSK_SHIFT)) & ELS_ELS_KS3_KS3_UKSK_MASK)

#define ELS_ELS_KS3_KS3_URTF_MASK                (0x8000U)
#define ELS_ELS_KS3_KS3_URTF_SHIFT               (15U)
/*! KS3_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS3_KS3_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_URTF_SHIFT)) & ELS_ELS_KS3_KS3_URTF_MASK)

#define ELS_ELS_KS3_KS3_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS3_KS3_UCKDF_SHIFT              (16U)
/*! KS3_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS3_KS3_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UCKDF_SHIFT)) & ELS_ELS_KS3_KS3_UCKDF_MASK)

#define ELS_ELS_KS3_KS3_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS3_KS3_UHKDF_SHIFT              (17U)
/*! KS3_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS3_KS3_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UHKDF_SHIFT)) & ELS_ELS_KS3_KS3_UHKDF_MASK)

#define ELS_ELS_KS3_KS3_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS3_KS3_UECSG_SHIFT              (18U)
/*! KS3_UECSG - Ecc signing key */
#define ELS_ELS_KS3_KS3_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UECSG_SHIFT)) & ELS_ELS_KS3_KS3_UECSG_MASK)

#define ELS_ELS_KS3_KS3_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS3_KS3_UECDH_SHIFT              (19U)
/*! KS3_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS3_KS3_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UECDH_SHIFT)) & ELS_ELS_KS3_KS3_UECDH_MASK)

#define ELS_ELS_KS3_KS3_UAES_MASK                (0x100000U)
#define ELS_ELS_KS3_KS3_UAES_SHIFT               (20U)
/*! KS3_UAES - Aes key */
#define ELS_ELS_KS3_KS3_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UAES_SHIFT)) & ELS_ELS_KS3_KS3_UAES_MASK)

#define ELS_ELS_KS3_KS3_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS3_KS3_UHMAC_SHIFT              (21U)
/*! KS3_UHMAC - Hmac key */
#define ELS_ELS_KS3_KS3_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UHMAC_SHIFT)) & ELS_ELS_KS3_KS3_UHMAC_MASK)

#define ELS_ELS_KS3_KS3_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS3_KS3_UKWK_SHIFT               (22U)
/*! KS3_UKWK - Key wrapping key */
#define ELS_ELS_KS3_KS3_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UKWK_SHIFT)) & ELS_ELS_KS3_KS3_UKWK_MASK)

#define ELS_ELS_KS3_KS3_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS3_KS3_UKUOK_SHIFT              (23U)
/*! KS3_UKUOK - Key unwrapping key */
#define ELS_ELS_KS3_KS3_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UKUOK_SHIFT)) & ELS_ELS_KS3_KS3_UKUOK_MASK)

#define ELS_ELS_KS3_KS3_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS3_KS3_UTLSPMS_SHIFT            (24U)
/*! KS3_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS3_KS3_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UTLSPMS_SHIFT)) & ELS_ELS_KS3_KS3_UTLSPMS_MASK)

#define ELS_ELS_KS3_KS3_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS3_KS3_UTLSMS_SHIFT             (25U)
/*! KS3_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS3_KS3_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UTLSMS_SHIFT)) & ELS_ELS_KS3_KS3_UTLSMS_MASK)

#define ELS_ELS_KS3_KS3_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS3_KS3_UKGSRC_SHIFT             (26U)
/*! KS3_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS3_KS3_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UKGSRC_SHIFT)) & ELS_ELS_KS3_KS3_UKGSRC_MASK)

#define ELS_ELS_KS3_KS3_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS3_KS3_UHWO_SHIFT               (27U)
/*! KS3_UHWO - Hardware out key */
#define ELS_ELS_KS3_KS3_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UHWO_SHIFT)) & ELS_ELS_KS3_KS3_UHWO_MASK)

#define ELS_ELS_KS3_KS3_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS3_KS3_UWRPOK_SHIFT             (28U)
/*! KS3_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS3_KS3_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UWRPOK_SHIFT)) & ELS_ELS_KS3_KS3_UWRPOK_MASK)

#define ELS_ELS_KS3_KS3_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS3_KS3_UDUK_SHIFT               (29U)
/*! KS3_UDUK - Device Unique Key */
#define ELS_ELS_KS3_KS3_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UDUK_SHIFT)) & ELS_ELS_KS3_KS3_UDUK_MASK)

#define ELS_ELS_KS3_KS3_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS3_KS3_UPPROT_SHIFT             (30U)
/*! KS3_UPPROT - Priviledge level */
#define ELS_ELS_KS3_KS3_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS3_KS3_UPPROT_SHIFT)) & ELS_ELS_KS3_KS3_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS4 - Status register */
/*! @{ */

#define ELS_ELS_KS4_KS4_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS4_KS4_KSIZE_SHIFT              (0U)
/*! KS4_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS4_KS4_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_KSIZE_SHIFT)) & ELS_ELS_KS4_KS4_KSIZE_MASK)

#define ELS_ELS_KS4_KS4_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS4_KS4_RSVD0_SHIFT              (1U)
/*! KS4_RSVD0 - Reserved 0 */
#define ELS_ELS_KS4_KS4_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_RSVD0_SHIFT)) & ELS_ELS_KS4_KS4_RSVD0_MASK)

#define ELS_ELS_KS4_KS4_KACT_MASK                (0x20U)
#define ELS_ELS_KS4_KS4_KACT_SHIFT               (5U)
/*! KS4_KACT - Key is active */
#define ELS_ELS_KS4_KS4_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_KACT_SHIFT)) & ELS_ELS_KS4_KS4_KACT_MASK)

#define ELS_ELS_KS4_KS4_KBASE_MASK               (0x40U)
#define ELS_ELS_KS4_KS4_KBASE_SHIFT              (6U)
/*! KS4_KBASE - First slot in a multislot key */
#define ELS_ELS_KS4_KS4_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_KBASE_SHIFT)) & ELS_ELS_KS4_KS4_KBASE_MASK)

#define ELS_ELS_KS4_KS4_FGP_MASK                 (0x80U)
#define ELS_ELS_KS4_KS4_FGP_SHIFT                (7U)
/*! KS4_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS4_KS4_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_FGP_SHIFT)) & ELS_ELS_KS4_KS4_FGP_MASK)

#define ELS_ELS_KS4_KS4_FRTN_MASK                (0x100U)
#define ELS_ELS_KS4_KS4_FRTN_SHIFT               (8U)
/*! KS4_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS4_KS4_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_FRTN_SHIFT)) & ELS_ELS_KS4_KS4_FRTN_MASK)

#define ELS_ELS_KS4_KS4_FHWO_MASK                (0x200U)
#define ELS_ELS_KS4_KS4_FHWO_SHIFT               (9U)
/*! KS4_FHWO - Hardware Feature Output */
#define ELS_ELS_KS4_KS4_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_FHWO_SHIFT)) & ELS_ELS_KS4_KS4_FHWO_MASK)

#define ELS_ELS_KS4_KS4_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS4_KS4_RSVD1_SHIFT              (10U)
/*! KS4_RSVD1 - Reserved 1 */
#define ELS_ELS_KS4_KS4_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_RSVD1_SHIFT)) & ELS_ELS_KS4_KS4_RSVD1_MASK)

#define ELS_ELS_KS4_KS4_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS4_KS4_UCMAC_SHIFT              (13U)
/*! KS4_UCMAC - CMAC key */
#define ELS_ELS_KS4_KS4_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UCMAC_SHIFT)) & ELS_ELS_KS4_KS4_UCMAC_MASK)

#define ELS_ELS_KS4_KS4_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS4_KS4_UKSK_SHIFT               (14U)
/*! KS4_UKSK - KSK key */
#define ELS_ELS_KS4_KS4_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UKSK_SHIFT)) & ELS_ELS_KS4_KS4_UKSK_MASK)

#define ELS_ELS_KS4_KS4_URTF_MASK                (0x8000U)
#define ELS_ELS_KS4_KS4_URTF_SHIFT               (15U)
/*! KS4_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS4_KS4_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_URTF_SHIFT)) & ELS_ELS_KS4_KS4_URTF_MASK)

#define ELS_ELS_KS4_KS4_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS4_KS4_UCKDF_SHIFT              (16U)
/*! KS4_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS4_KS4_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UCKDF_SHIFT)) & ELS_ELS_KS4_KS4_UCKDF_MASK)

#define ELS_ELS_KS4_KS4_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS4_KS4_UHKDF_SHIFT              (17U)
/*! KS4_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS4_KS4_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UHKDF_SHIFT)) & ELS_ELS_KS4_KS4_UHKDF_MASK)

#define ELS_ELS_KS4_KS4_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS4_KS4_UECSG_SHIFT              (18U)
/*! KS4_UECSG - Ecc signing key */
#define ELS_ELS_KS4_KS4_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UECSG_SHIFT)) & ELS_ELS_KS4_KS4_UECSG_MASK)

#define ELS_ELS_KS4_KS4_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS4_KS4_UECDH_SHIFT              (19U)
/*! KS4_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS4_KS4_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UECDH_SHIFT)) & ELS_ELS_KS4_KS4_UECDH_MASK)

#define ELS_ELS_KS4_KS4_UAES_MASK                (0x100000U)
#define ELS_ELS_KS4_KS4_UAES_SHIFT               (20U)
/*! KS4_UAES - Aes key */
#define ELS_ELS_KS4_KS4_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UAES_SHIFT)) & ELS_ELS_KS4_KS4_UAES_MASK)

#define ELS_ELS_KS4_KS4_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS4_KS4_UHMAC_SHIFT              (21U)
/*! KS4_UHMAC - Hmac key */
#define ELS_ELS_KS4_KS4_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UHMAC_SHIFT)) & ELS_ELS_KS4_KS4_UHMAC_MASK)

#define ELS_ELS_KS4_KS4_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS4_KS4_UKWK_SHIFT               (22U)
/*! KS4_UKWK - Key wrapping key */
#define ELS_ELS_KS4_KS4_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UKWK_SHIFT)) & ELS_ELS_KS4_KS4_UKWK_MASK)

#define ELS_ELS_KS4_KS4_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS4_KS4_UKUOK_SHIFT              (23U)
/*! KS4_UKUOK - Key unwrapping key */
#define ELS_ELS_KS4_KS4_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UKUOK_SHIFT)) & ELS_ELS_KS4_KS4_UKUOK_MASK)

#define ELS_ELS_KS4_KS4_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS4_KS4_UTLSPMS_SHIFT            (24U)
/*! KS4_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS4_KS4_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UTLSPMS_SHIFT)) & ELS_ELS_KS4_KS4_UTLSPMS_MASK)

#define ELS_ELS_KS4_KS4_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS4_KS4_UTLSMS_SHIFT             (25U)
/*! KS4_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS4_KS4_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UTLSMS_SHIFT)) & ELS_ELS_KS4_KS4_UTLSMS_MASK)

#define ELS_ELS_KS4_KS4_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS4_KS4_UKGSRC_SHIFT             (26U)
/*! KS4_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS4_KS4_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UKGSRC_SHIFT)) & ELS_ELS_KS4_KS4_UKGSRC_MASK)

#define ELS_ELS_KS4_KS4_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS4_KS4_UHWO_SHIFT               (27U)
/*! KS4_UHWO - Hardware out key */
#define ELS_ELS_KS4_KS4_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UHWO_SHIFT)) & ELS_ELS_KS4_KS4_UHWO_MASK)

#define ELS_ELS_KS4_KS4_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS4_KS4_UWRPOK_SHIFT             (28U)
/*! KS4_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS4_KS4_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UWRPOK_SHIFT)) & ELS_ELS_KS4_KS4_UWRPOK_MASK)

#define ELS_ELS_KS4_KS4_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS4_KS4_UDUK_SHIFT               (29U)
/*! KS4_UDUK - Device Unique Key */
#define ELS_ELS_KS4_KS4_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UDUK_SHIFT)) & ELS_ELS_KS4_KS4_UDUK_MASK)

#define ELS_ELS_KS4_KS4_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS4_KS4_UPPROT_SHIFT             (30U)
/*! KS4_UPPROT - Priviledge level */
#define ELS_ELS_KS4_KS4_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS4_KS4_UPPROT_SHIFT)) & ELS_ELS_KS4_KS4_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS5 - Status register */
/*! @{ */

#define ELS_ELS_KS5_KS5_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS5_KS5_KSIZE_SHIFT              (0U)
/*! KS5_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS5_KS5_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_KSIZE_SHIFT)) & ELS_ELS_KS5_KS5_KSIZE_MASK)

#define ELS_ELS_KS5_KS5_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS5_KS5_RSVD0_SHIFT              (1U)
/*! KS5_RSVD0 - Reserved 0 */
#define ELS_ELS_KS5_KS5_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_RSVD0_SHIFT)) & ELS_ELS_KS5_KS5_RSVD0_MASK)

#define ELS_ELS_KS5_KS5_KACT_MASK                (0x20U)
#define ELS_ELS_KS5_KS5_KACT_SHIFT               (5U)
/*! KS5_KACT - Key is active */
#define ELS_ELS_KS5_KS5_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_KACT_SHIFT)) & ELS_ELS_KS5_KS5_KACT_MASK)

#define ELS_ELS_KS5_KS5_KBASE_MASK               (0x40U)
#define ELS_ELS_KS5_KS5_KBASE_SHIFT              (6U)
/*! KS5_KBASE - First slot in a multislot key */
#define ELS_ELS_KS5_KS5_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_KBASE_SHIFT)) & ELS_ELS_KS5_KS5_KBASE_MASK)

#define ELS_ELS_KS5_KS5_FGP_MASK                 (0x80U)
#define ELS_ELS_KS5_KS5_FGP_SHIFT                (7U)
/*! KS5_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS5_KS5_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_FGP_SHIFT)) & ELS_ELS_KS5_KS5_FGP_MASK)

#define ELS_ELS_KS5_KS5_FRTN_MASK                (0x100U)
#define ELS_ELS_KS5_KS5_FRTN_SHIFT               (8U)
/*! KS5_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS5_KS5_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_FRTN_SHIFT)) & ELS_ELS_KS5_KS5_FRTN_MASK)

#define ELS_ELS_KS5_KS5_FHWO_MASK                (0x200U)
#define ELS_ELS_KS5_KS5_FHWO_SHIFT               (9U)
/*! KS5_FHWO - Hardware Feature Output */
#define ELS_ELS_KS5_KS5_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_FHWO_SHIFT)) & ELS_ELS_KS5_KS5_FHWO_MASK)

#define ELS_ELS_KS5_KS5_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS5_KS5_RSVD1_SHIFT              (10U)
/*! KS5_RSVD1 - Reserved 1 */
#define ELS_ELS_KS5_KS5_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_RSVD1_SHIFT)) & ELS_ELS_KS5_KS5_RSVD1_MASK)

#define ELS_ELS_KS5_KS5_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS5_KS5_UCMAC_SHIFT              (13U)
/*! KS5_UCMAC - CMAC key */
#define ELS_ELS_KS5_KS5_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UCMAC_SHIFT)) & ELS_ELS_KS5_KS5_UCMAC_MASK)

#define ELS_ELS_KS5_KS5_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS5_KS5_UKSK_SHIFT               (14U)
/*! KS5_UKSK - KSK key */
#define ELS_ELS_KS5_KS5_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UKSK_SHIFT)) & ELS_ELS_KS5_KS5_UKSK_MASK)

#define ELS_ELS_KS5_KS5_URTF_MASK                (0x8000U)
#define ELS_ELS_KS5_KS5_URTF_SHIFT               (15U)
/*! KS5_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS5_KS5_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_URTF_SHIFT)) & ELS_ELS_KS5_KS5_URTF_MASK)

#define ELS_ELS_KS5_KS5_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS5_KS5_UCKDF_SHIFT              (16U)
/*! KS5_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS5_KS5_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UCKDF_SHIFT)) & ELS_ELS_KS5_KS5_UCKDF_MASK)

#define ELS_ELS_KS5_KS5_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS5_KS5_UHKDF_SHIFT              (17U)
/*! KS5_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS5_KS5_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UHKDF_SHIFT)) & ELS_ELS_KS5_KS5_UHKDF_MASK)

#define ELS_ELS_KS5_KS5_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS5_KS5_UECSG_SHIFT              (18U)
/*! KS5_UECSG - Ecc signing key */
#define ELS_ELS_KS5_KS5_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UECSG_SHIFT)) & ELS_ELS_KS5_KS5_UECSG_MASK)

#define ELS_ELS_KS5_KS5_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS5_KS5_UECDH_SHIFT              (19U)
/*! KS5_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS5_KS5_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UECDH_SHIFT)) & ELS_ELS_KS5_KS5_UECDH_MASK)

#define ELS_ELS_KS5_KS5_UAES_MASK                (0x100000U)
#define ELS_ELS_KS5_KS5_UAES_SHIFT               (20U)
/*! KS5_UAES - Aes key */
#define ELS_ELS_KS5_KS5_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UAES_SHIFT)) & ELS_ELS_KS5_KS5_UAES_MASK)

#define ELS_ELS_KS5_KS5_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS5_KS5_UHMAC_SHIFT              (21U)
/*! KS5_UHMAC - Hmac key */
#define ELS_ELS_KS5_KS5_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UHMAC_SHIFT)) & ELS_ELS_KS5_KS5_UHMAC_MASK)

#define ELS_ELS_KS5_KS5_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS5_KS5_UKWK_SHIFT               (22U)
/*! KS5_UKWK - Key wrapping key */
#define ELS_ELS_KS5_KS5_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UKWK_SHIFT)) & ELS_ELS_KS5_KS5_UKWK_MASK)

#define ELS_ELS_KS5_KS5_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS5_KS5_UKUOK_SHIFT              (23U)
/*! KS5_UKUOK - Key unwrapping key */
#define ELS_ELS_KS5_KS5_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UKUOK_SHIFT)) & ELS_ELS_KS5_KS5_UKUOK_MASK)

#define ELS_ELS_KS5_KS5_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS5_KS5_UTLSPMS_SHIFT            (24U)
/*! KS5_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS5_KS5_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UTLSPMS_SHIFT)) & ELS_ELS_KS5_KS5_UTLSPMS_MASK)

#define ELS_ELS_KS5_KS5_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS5_KS5_UTLSMS_SHIFT             (25U)
/*! KS5_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS5_KS5_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UTLSMS_SHIFT)) & ELS_ELS_KS5_KS5_UTLSMS_MASK)

#define ELS_ELS_KS5_KS5_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS5_KS5_UKGSRC_SHIFT             (26U)
/*! KS5_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS5_KS5_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UKGSRC_SHIFT)) & ELS_ELS_KS5_KS5_UKGSRC_MASK)

#define ELS_ELS_KS5_KS5_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS5_KS5_UHWO_SHIFT               (27U)
/*! KS5_UHWO - Hardware out key */
#define ELS_ELS_KS5_KS5_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UHWO_SHIFT)) & ELS_ELS_KS5_KS5_UHWO_MASK)

#define ELS_ELS_KS5_KS5_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS5_KS5_UWRPOK_SHIFT             (28U)
/*! KS5_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS5_KS5_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UWRPOK_SHIFT)) & ELS_ELS_KS5_KS5_UWRPOK_MASK)

#define ELS_ELS_KS5_KS5_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS5_KS5_UDUK_SHIFT               (29U)
/*! KS5_UDUK - Device Unique Key */
#define ELS_ELS_KS5_KS5_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UDUK_SHIFT)) & ELS_ELS_KS5_KS5_UDUK_MASK)

#define ELS_ELS_KS5_KS5_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS5_KS5_UPPROT_SHIFT             (30U)
/*! KS5_UPPROT - Priviledge level */
#define ELS_ELS_KS5_KS5_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS5_KS5_UPPROT_SHIFT)) & ELS_ELS_KS5_KS5_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS6 - Status register */
/*! @{ */

#define ELS_ELS_KS6_KS6_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS6_KS6_KSIZE_SHIFT              (0U)
/*! KS6_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS6_KS6_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_KSIZE_SHIFT)) & ELS_ELS_KS6_KS6_KSIZE_MASK)

#define ELS_ELS_KS6_KS6_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS6_KS6_RSVD0_SHIFT              (1U)
/*! KS6_RSVD0 - Reserved 0 */
#define ELS_ELS_KS6_KS6_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_RSVD0_SHIFT)) & ELS_ELS_KS6_KS6_RSVD0_MASK)

#define ELS_ELS_KS6_KS6_KACT_MASK                (0x20U)
#define ELS_ELS_KS6_KS6_KACT_SHIFT               (5U)
/*! KS6_KACT - Key is active */
#define ELS_ELS_KS6_KS6_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_KACT_SHIFT)) & ELS_ELS_KS6_KS6_KACT_MASK)

#define ELS_ELS_KS6_KS6_KBASE_MASK               (0x40U)
#define ELS_ELS_KS6_KS6_KBASE_SHIFT              (6U)
/*! KS6_KBASE - First slot in a multislot key */
#define ELS_ELS_KS6_KS6_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_KBASE_SHIFT)) & ELS_ELS_KS6_KS6_KBASE_MASK)

#define ELS_ELS_KS6_KS6_FGP_MASK                 (0x80U)
#define ELS_ELS_KS6_KS6_FGP_SHIFT                (7U)
/*! KS6_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS6_KS6_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_FGP_SHIFT)) & ELS_ELS_KS6_KS6_FGP_MASK)

#define ELS_ELS_KS6_KS6_FRTN_MASK                (0x100U)
#define ELS_ELS_KS6_KS6_FRTN_SHIFT               (8U)
/*! KS6_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS6_KS6_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_FRTN_SHIFT)) & ELS_ELS_KS6_KS6_FRTN_MASK)

#define ELS_ELS_KS6_KS6_FHWO_MASK                (0x200U)
#define ELS_ELS_KS6_KS6_FHWO_SHIFT               (9U)
/*! KS6_FHWO - Hardware Feature Output */
#define ELS_ELS_KS6_KS6_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_FHWO_SHIFT)) & ELS_ELS_KS6_KS6_FHWO_MASK)

#define ELS_ELS_KS6_KS6_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS6_KS6_RSVD1_SHIFT              (10U)
/*! KS6_RSVD1 - Reserved 1 */
#define ELS_ELS_KS6_KS6_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_RSVD1_SHIFT)) & ELS_ELS_KS6_KS6_RSVD1_MASK)

#define ELS_ELS_KS6_KS6_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS6_KS6_UCMAC_SHIFT              (13U)
/*! KS6_UCMAC - CMAC key */
#define ELS_ELS_KS6_KS6_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UCMAC_SHIFT)) & ELS_ELS_KS6_KS6_UCMAC_MASK)

#define ELS_ELS_KS6_KS6_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS6_KS6_UKSK_SHIFT               (14U)
/*! KS6_UKSK - KSK key */
#define ELS_ELS_KS6_KS6_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UKSK_SHIFT)) & ELS_ELS_KS6_KS6_UKSK_MASK)

#define ELS_ELS_KS6_KS6_URTF_MASK                (0x8000U)
#define ELS_ELS_KS6_KS6_URTF_SHIFT               (15U)
/*! KS6_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS6_KS6_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_URTF_SHIFT)) & ELS_ELS_KS6_KS6_URTF_MASK)

#define ELS_ELS_KS6_KS6_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS6_KS6_UCKDF_SHIFT              (16U)
/*! KS6_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS6_KS6_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UCKDF_SHIFT)) & ELS_ELS_KS6_KS6_UCKDF_MASK)

#define ELS_ELS_KS6_KS6_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS6_KS6_UHKDF_SHIFT              (17U)
/*! KS6_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS6_KS6_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UHKDF_SHIFT)) & ELS_ELS_KS6_KS6_UHKDF_MASK)

#define ELS_ELS_KS6_KS6_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS6_KS6_UECSG_SHIFT              (18U)
/*! KS6_UECSG - Ecc signing key */
#define ELS_ELS_KS6_KS6_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UECSG_SHIFT)) & ELS_ELS_KS6_KS6_UECSG_MASK)

#define ELS_ELS_KS6_KS6_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS6_KS6_UECDH_SHIFT              (19U)
/*! KS6_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS6_KS6_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UECDH_SHIFT)) & ELS_ELS_KS6_KS6_UECDH_MASK)

#define ELS_ELS_KS6_KS6_UAES_MASK                (0x100000U)
#define ELS_ELS_KS6_KS6_UAES_SHIFT               (20U)
/*! KS6_UAES - Aes key */
#define ELS_ELS_KS6_KS6_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UAES_SHIFT)) & ELS_ELS_KS6_KS6_UAES_MASK)

#define ELS_ELS_KS6_KS6_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS6_KS6_UHMAC_SHIFT              (21U)
/*! KS6_UHMAC - Hmac key */
#define ELS_ELS_KS6_KS6_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UHMAC_SHIFT)) & ELS_ELS_KS6_KS6_UHMAC_MASK)

#define ELS_ELS_KS6_KS6_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS6_KS6_UKWK_SHIFT               (22U)
/*! KS6_UKWK - Key wrapping key */
#define ELS_ELS_KS6_KS6_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UKWK_SHIFT)) & ELS_ELS_KS6_KS6_UKWK_MASK)

#define ELS_ELS_KS6_KS6_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS6_KS6_UKUOK_SHIFT              (23U)
/*! KS6_UKUOK - Key unwrapping key */
#define ELS_ELS_KS6_KS6_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UKUOK_SHIFT)) & ELS_ELS_KS6_KS6_UKUOK_MASK)

#define ELS_ELS_KS6_KS6_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS6_KS6_UTLSPMS_SHIFT            (24U)
/*! KS6_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS6_KS6_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UTLSPMS_SHIFT)) & ELS_ELS_KS6_KS6_UTLSPMS_MASK)

#define ELS_ELS_KS6_KS6_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS6_KS6_UTLSMS_SHIFT             (25U)
/*! KS6_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS6_KS6_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UTLSMS_SHIFT)) & ELS_ELS_KS6_KS6_UTLSMS_MASK)

#define ELS_ELS_KS6_KS6_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS6_KS6_UKGSRC_SHIFT             (26U)
/*! KS6_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS6_KS6_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UKGSRC_SHIFT)) & ELS_ELS_KS6_KS6_UKGSRC_MASK)

#define ELS_ELS_KS6_KS6_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS6_KS6_UHWO_SHIFT               (27U)
/*! KS6_UHWO - Hardware out key */
#define ELS_ELS_KS6_KS6_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UHWO_SHIFT)) & ELS_ELS_KS6_KS6_UHWO_MASK)

#define ELS_ELS_KS6_KS6_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS6_KS6_UWRPOK_SHIFT             (28U)
/*! KS6_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS6_KS6_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UWRPOK_SHIFT)) & ELS_ELS_KS6_KS6_UWRPOK_MASK)

#define ELS_ELS_KS6_KS6_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS6_KS6_UDUK_SHIFT               (29U)
/*! KS6_UDUK - Device Unique Key */
#define ELS_ELS_KS6_KS6_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UDUK_SHIFT)) & ELS_ELS_KS6_KS6_UDUK_MASK)

#define ELS_ELS_KS6_KS6_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS6_KS6_UPPROT_SHIFT             (30U)
/*! KS6_UPPROT - Priviledge level */
#define ELS_ELS_KS6_KS6_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS6_KS6_UPPROT_SHIFT)) & ELS_ELS_KS6_KS6_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS7 - Status register */
/*! @{ */

#define ELS_ELS_KS7_KS7_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS7_KS7_KSIZE_SHIFT              (0U)
/*! KS7_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS7_KS7_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_KSIZE_SHIFT)) & ELS_ELS_KS7_KS7_KSIZE_MASK)

#define ELS_ELS_KS7_KS7_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS7_KS7_RSVD0_SHIFT              (1U)
/*! KS7_RSVD0 - Reserved 0 */
#define ELS_ELS_KS7_KS7_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_RSVD0_SHIFT)) & ELS_ELS_KS7_KS7_RSVD0_MASK)

#define ELS_ELS_KS7_KS7_KACT_MASK                (0x20U)
#define ELS_ELS_KS7_KS7_KACT_SHIFT               (5U)
/*! KS7_KACT - Key is active */
#define ELS_ELS_KS7_KS7_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_KACT_SHIFT)) & ELS_ELS_KS7_KS7_KACT_MASK)

#define ELS_ELS_KS7_KS7_KBASE_MASK               (0x40U)
#define ELS_ELS_KS7_KS7_KBASE_SHIFT              (6U)
/*! KS7_KBASE - First slot in a multislot key */
#define ELS_ELS_KS7_KS7_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_KBASE_SHIFT)) & ELS_ELS_KS7_KS7_KBASE_MASK)

#define ELS_ELS_KS7_KS7_FGP_MASK                 (0x80U)
#define ELS_ELS_KS7_KS7_FGP_SHIFT                (7U)
/*! KS7_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS7_KS7_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_FGP_SHIFT)) & ELS_ELS_KS7_KS7_FGP_MASK)

#define ELS_ELS_KS7_KS7_FRTN_MASK                (0x100U)
#define ELS_ELS_KS7_KS7_FRTN_SHIFT               (8U)
/*! KS7_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS7_KS7_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_FRTN_SHIFT)) & ELS_ELS_KS7_KS7_FRTN_MASK)

#define ELS_ELS_KS7_KS7_FHWO_MASK                (0x200U)
#define ELS_ELS_KS7_KS7_FHWO_SHIFT               (9U)
/*! KS7_FHWO - Hardware Feature Output */
#define ELS_ELS_KS7_KS7_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_FHWO_SHIFT)) & ELS_ELS_KS7_KS7_FHWO_MASK)

#define ELS_ELS_KS7_KS7_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS7_KS7_RSVD1_SHIFT              (10U)
/*! KS7_RSVD1 - Reserved 1 */
#define ELS_ELS_KS7_KS7_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_RSVD1_SHIFT)) & ELS_ELS_KS7_KS7_RSVD1_MASK)

#define ELS_ELS_KS7_KS7_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS7_KS7_UCMAC_SHIFT              (13U)
/*! KS7_UCMAC - CMAC key */
#define ELS_ELS_KS7_KS7_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UCMAC_SHIFT)) & ELS_ELS_KS7_KS7_UCMAC_MASK)

#define ELS_ELS_KS7_KS7_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS7_KS7_UKSK_SHIFT               (14U)
/*! KS7_UKSK - KSK key */
#define ELS_ELS_KS7_KS7_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UKSK_SHIFT)) & ELS_ELS_KS7_KS7_UKSK_MASK)

#define ELS_ELS_KS7_KS7_URTF_MASK                (0x8000U)
#define ELS_ELS_KS7_KS7_URTF_SHIFT               (15U)
/*! KS7_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS7_KS7_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_URTF_SHIFT)) & ELS_ELS_KS7_KS7_URTF_MASK)

#define ELS_ELS_KS7_KS7_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS7_KS7_UCKDF_SHIFT              (16U)
/*! KS7_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS7_KS7_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UCKDF_SHIFT)) & ELS_ELS_KS7_KS7_UCKDF_MASK)

#define ELS_ELS_KS7_KS7_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS7_KS7_UHKDF_SHIFT              (17U)
/*! KS7_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS7_KS7_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UHKDF_SHIFT)) & ELS_ELS_KS7_KS7_UHKDF_MASK)

#define ELS_ELS_KS7_KS7_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS7_KS7_UECSG_SHIFT              (18U)
/*! KS7_UECSG - Ecc signing key */
#define ELS_ELS_KS7_KS7_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UECSG_SHIFT)) & ELS_ELS_KS7_KS7_UECSG_MASK)

#define ELS_ELS_KS7_KS7_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS7_KS7_UECDH_SHIFT              (19U)
/*! KS7_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS7_KS7_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UECDH_SHIFT)) & ELS_ELS_KS7_KS7_UECDH_MASK)

#define ELS_ELS_KS7_KS7_UAES_MASK                (0x100000U)
#define ELS_ELS_KS7_KS7_UAES_SHIFT               (20U)
/*! KS7_UAES - Aes key */
#define ELS_ELS_KS7_KS7_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UAES_SHIFT)) & ELS_ELS_KS7_KS7_UAES_MASK)

#define ELS_ELS_KS7_KS7_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS7_KS7_UHMAC_SHIFT              (21U)
/*! KS7_UHMAC - Hmac key */
#define ELS_ELS_KS7_KS7_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UHMAC_SHIFT)) & ELS_ELS_KS7_KS7_UHMAC_MASK)

#define ELS_ELS_KS7_KS7_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS7_KS7_UKWK_SHIFT               (22U)
/*! KS7_UKWK - Key wrapping key */
#define ELS_ELS_KS7_KS7_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UKWK_SHIFT)) & ELS_ELS_KS7_KS7_UKWK_MASK)

#define ELS_ELS_KS7_KS7_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS7_KS7_UKUOK_SHIFT              (23U)
/*! KS7_UKUOK - Key unwrapping key */
#define ELS_ELS_KS7_KS7_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UKUOK_SHIFT)) & ELS_ELS_KS7_KS7_UKUOK_MASK)

#define ELS_ELS_KS7_KS7_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS7_KS7_UTLSPMS_SHIFT            (24U)
/*! KS7_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS7_KS7_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UTLSPMS_SHIFT)) & ELS_ELS_KS7_KS7_UTLSPMS_MASK)

#define ELS_ELS_KS7_KS7_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS7_KS7_UTLSMS_SHIFT             (25U)
/*! KS7_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS7_KS7_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UTLSMS_SHIFT)) & ELS_ELS_KS7_KS7_UTLSMS_MASK)

#define ELS_ELS_KS7_KS7_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS7_KS7_UKGSRC_SHIFT             (26U)
/*! KS7_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS7_KS7_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UKGSRC_SHIFT)) & ELS_ELS_KS7_KS7_UKGSRC_MASK)

#define ELS_ELS_KS7_KS7_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS7_KS7_UHWO_SHIFT               (27U)
/*! KS7_UHWO - Hardware out key */
#define ELS_ELS_KS7_KS7_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UHWO_SHIFT)) & ELS_ELS_KS7_KS7_UHWO_MASK)

#define ELS_ELS_KS7_KS7_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS7_KS7_UWRPOK_SHIFT             (28U)
/*! KS7_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS7_KS7_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UWRPOK_SHIFT)) & ELS_ELS_KS7_KS7_UWRPOK_MASK)

#define ELS_ELS_KS7_KS7_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS7_KS7_UDUK_SHIFT               (29U)
/*! KS7_UDUK - Device Unique Key */
#define ELS_ELS_KS7_KS7_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UDUK_SHIFT)) & ELS_ELS_KS7_KS7_UDUK_MASK)

#define ELS_ELS_KS7_KS7_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS7_KS7_UPPROT_SHIFT             (30U)
/*! KS7_UPPROT - Priviledge level */
#define ELS_ELS_KS7_KS7_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS7_KS7_UPPROT_SHIFT)) & ELS_ELS_KS7_KS7_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS8 - Status register */
/*! @{ */

#define ELS_ELS_KS8_KS8_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS8_KS8_KSIZE_SHIFT              (0U)
/*! KS8_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS8_KS8_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_KSIZE_SHIFT)) & ELS_ELS_KS8_KS8_KSIZE_MASK)

#define ELS_ELS_KS8_KS8_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS8_KS8_RSVD0_SHIFT              (1U)
/*! KS8_RSVD0 - Reserved 0 */
#define ELS_ELS_KS8_KS8_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_RSVD0_SHIFT)) & ELS_ELS_KS8_KS8_RSVD0_MASK)

#define ELS_ELS_KS8_KS8_KACT_MASK                (0x20U)
#define ELS_ELS_KS8_KS8_KACT_SHIFT               (5U)
/*! KS8_KACT - Key is active */
#define ELS_ELS_KS8_KS8_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_KACT_SHIFT)) & ELS_ELS_KS8_KS8_KACT_MASK)

#define ELS_ELS_KS8_KS8_KBASE_MASK               (0x40U)
#define ELS_ELS_KS8_KS8_KBASE_SHIFT              (6U)
/*! KS8_KBASE - First slot in a multislot key */
#define ELS_ELS_KS8_KS8_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_KBASE_SHIFT)) & ELS_ELS_KS8_KS8_KBASE_MASK)

#define ELS_ELS_KS8_KS8_FGP_MASK                 (0x80U)
#define ELS_ELS_KS8_KS8_FGP_SHIFT                (7U)
/*! KS8_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS8_KS8_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_FGP_SHIFT)) & ELS_ELS_KS8_KS8_FGP_MASK)

#define ELS_ELS_KS8_KS8_FRTN_MASK                (0x100U)
#define ELS_ELS_KS8_KS8_FRTN_SHIFT               (8U)
/*! KS8_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS8_KS8_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_FRTN_SHIFT)) & ELS_ELS_KS8_KS8_FRTN_MASK)

#define ELS_ELS_KS8_KS8_FHWO_MASK                (0x200U)
#define ELS_ELS_KS8_KS8_FHWO_SHIFT               (9U)
/*! KS8_FHWO - Hardware Feature Output */
#define ELS_ELS_KS8_KS8_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_FHWO_SHIFT)) & ELS_ELS_KS8_KS8_FHWO_MASK)

#define ELS_ELS_KS8_KS8_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS8_KS8_RSVD1_SHIFT              (10U)
/*! KS8_RSVD1 - Reserved 1 */
#define ELS_ELS_KS8_KS8_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_RSVD1_SHIFT)) & ELS_ELS_KS8_KS8_RSVD1_MASK)

#define ELS_ELS_KS8_KS8_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS8_KS8_UCMAC_SHIFT              (13U)
/*! KS8_UCMAC - CMAC key */
#define ELS_ELS_KS8_KS8_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UCMAC_SHIFT)) & ELS_ELS_KS8_KS8_UCMAC_MASK)

#define ELS_ELS_KS8_KS8_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS8_KS8_UKSK_SHIFT               (14U)
/*! KS8_UKSK - KSK key */
#define ELS_ELS_KS8_KS8_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UKSK_SHIFT)) & ELS_ELS_KS8_KS8_UKSK_MASK)

#define ELS_ELS_KS8_KS8_URTF_MASK                (0x8000U)
#define ELS_ELS_KS8_KS8_URTF_SHIFT               (15U)
/*! KS8_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS8_KS8_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_URTF_SHIFT)) & ELS_ELS_KS8_KS8_URTF_MASK)

#define ELS_ELS_KS8_KS8_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS8_KS8_UCKDF_SHIFT              (16U)
/*! KS8_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS8_KS8_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UCKDF_SHIFT)) & ELS_ELS_KS8_KS8_UCKDF_MASK)

#define ELS_ELS_KS8_KS8_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS8_KS8_UHKDF_SHIFT              (17U)
/*! KS8_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS8_KS8_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UHKDF_SHIFT)) & ELS_ELS_KS8_KS8_UHKDF_MASK)

#define ELS_ELS_KS8_KS8_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS8_KS8_UECSG_SHIFT              (18U)
/*! KS8_UECSG - Ecc signing key */
#define ELS_ELS_KS8_KS8_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UECSG_SHIFT)) & ELS_ELS_KS8_KS8_UECSG_MASK)

#define ELS_ELS_KS8_KS8_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS8_KS8_UECDH_SHIFT              (19U)
/*! KS8_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS8_KS8_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UECDH_SHIFT)) & ELS_ELS_KS8_KS8_UECDH_MASK)

#define ELS_ELS_KS8_KS8_UAES_MASK                (0x100000U)
#define ELS_ELS_KS8_KS8_UAES_SHIFT               (20U)
/*! KS8_UAES - Aes key */
#define ELS_ELS_KS8_KS8_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UAES_SHIFT)) & ELS_ELS_KS8_KS8_UAES_MASK)

#define ELS_ELS_KS8_KS8_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS8_KS8_UHMAC_SHIFT              (21U)
/*! KS8_UHMAC - Hmac key */
#define ELS_ELS_KS8_KS8_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UHMAC_SHIFT)) & ELS_ELS_KS8_KS8_UHMAC_MASK)

#define ELS_ELS_KS8_KS8_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS8_KS8_UKWK_SHIFT               (22U)
/*! KS8_UKWK - Key wrapping key */
#define ELS_ELS_KS8_KS8_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UKWK_SHIFT)) & ELS_ELS_KS8_KS8_UKWK_MASK)

#define ELS_ELS_KS8_KS8_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS8_KS8_UKUOK_SHIFT              (23U)
/*! KS8_UKUOK - Key unwrapping key */
#define ELS_ELS_KS8_KS8_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UKUOK_SHIFT)) & ELS_ELS_KS8_KS8_UKUOK_MASK)

#define ELS_ELS_KS8_KS8_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS8_KS8_UTLSPMS_SHIFT            (24U)
/*! KS8_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS8_KS8_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UTLSPMS_SHIFT)) & ELS_ELS_KS8_KS8_UTLSPMS_MASK)

#define ELS_ELS_KS8_KS8_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS8_KS8_UTLSMS_SHIFT             (25U)
/*! KS8_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS8_KS8_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UTLSMS_SHIFT)) & ELS_ELS_KS8_KS8_UTLSMS_MASK)

#define ELS_ELS_KS8_KS8_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS8_KS8_UKGSRC_SHIFT             (26U)
/*! KS8_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS8_KS8_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UKGSRC_SHIFT)) & ELS_ELS_KS8_KS8_UKGSRC_MASK)

#define ELS_ELS_KS8_KS8_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS8_KS8_UHWO_SHIFT               (27U)
/*! KS8_UHWO - Hardware out key */
#define ELS_ELS_KS8_KS8_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UHWO_SHIFT)) & ELS_ELS_KS8_KS8_UHWO_MASK)

#define ELS_ELS_KS8_KS8_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS8_KS8_UWRPOK_SHIFT             (28U)
/*! KS8_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS8_KS8_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UWRPOK_SHIFT)) & ELS_ELS_KS8_KS8_UWRPOK_MASK)

#define ELS_ELS_KS8_KS8_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS8_KS8_UDUK_SHIFT               (29U)
/*! KS8_UDUK - Device Unique Key */
#define ELS_ELS_KS8_KS8_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UDUK_SHIFT)) & ELS_ELS_KS8_KS8_UDUK_MASK)

#define ELS_ELS_KS8_KS8_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS8_KS8_UPPROT_SHIFT             (30U)
/*! KS8_UPPROT - Priviledge level */
#define ELS_ELS_KS8_KS8_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS8_KS8_UPPROT_SHIFT)) & ELS_ELS_KS8_KS8_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS9 - Status register */
/*! @{ */

#define ELS_ELS_KS9_KS9_KSIZE_MASK               (0x1U)
#define ELS_ELS_KS9_KS9_KSIZE_SHIFT              (0U)
/*! KS9_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS9_KS9_KSIZE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_KSIZE_SHIFT)) & ELS_ELS_KS9_KS9_KSIZE_MASK)

#define ELS_ELS_KS9_KS9_RSVD0_MASK               (0x1EU)
#define ELS_ELS_KS9_KS9_RSVD0_SHIFT              (1U)
/*! KS9_RSVD0 - Reserved 0 */
#define ELS_ELS_KS9_KS9_RSVD0(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_RSVD0_SHIFT)) & ELS_ELS_KS9_KS9_RSVD0_MASK)

#define ELS_ELS_KS9_KS9_KACT_MASK                (0x20U)
#define ELS_ELS_KS9_KS9_KACT_SHIFT               (5U)
/*! KS9_KACT - Key is active */
#define ELS_ELS_KS9_KS9_KACT(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_KACT_SHIFT)) & ELS_ELS_KS9_KS9_KACT_MASK)

#define ELS_ELS_KS9_KS9_KBASE_MASK               (0x40U)
#define ELS_ELS_KS9_KS9_KBASE_SHIFT              (6U)
/*! KS9_KBASE - First slot in a multislot key */
#define ELS_ELS_KS9_KS9_KBASE(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_KBASE_SHIFT)) & ELS_ELS_KS9_KS9_KBASE_MASK)

#define ELS_ELS_KS9_KS9_FGP_MASK                 (0x80U)
#define ELS_ELS_KS9_KS9_FGP_SHIFT                (7U)
/*! KS9_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS9_KS9_FGP(x)                   (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_FGP_SHIFT)) & ELS_ELS_KS9_KS9_FGP_MASK)

#define ELS_ELS_KS9_KS9_FRTN_MASK                (0x100U)
#define ELS_ELS_KS9_KS9_FRTN_SHIFT               (8U)
/*! KS9_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS9_KS9_FRTN(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_FRTN_SHIFT)) & ELS_ELS_KS9_KS9_FRTN_MASK)

#define ELS_ELS_KS9_KS9_FHWO_MASK                (0x200U)
#define ELS_ELS_KS9_KS9_FHWO_SHIFT               (9U)
/*! KS9_FHWO - Hardware Feature Output */
#define ELS_ELS_KS9_KS9_FHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_FHWO_SHIFT)) & ELS_ELS_KS9_KS9_FHWO_MASK)

#define ELS_ELS_KS9_KS9_RSVD1_MASK               (0x1C00U)
#define ELS_ELS_KS9_KS9_RSVD1_SHIFT              (10U)
/*! KS9_RSVD1 - Reserved 1 */
#define ELS_ELS_KS9_KS9_RSVD1(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_RSVD1_SHIFT)) & ELS_ELS_KS9_KS9_RSVD1_MASK)

#define ELS_ELS_KS9_KS9_UCMAC_MASK               (0x2000U)
#define ELS_ELS_KS9_KS9_UCMAC_SHIFT              (13U)
/*! KS9_UCMAC - CMAC key */
#define ELS_ELS_KS9_KS9_UCMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UCMAC_SHIFT)) & ELS_ELS_KS9_KS9_UCMAC_MASK)

#define ELS_ELS_KS9_KS9_UKSK_MASK                (0x4000U)
#define ELS_ELS_KS9_KS9_UKSK_SHIFT               (14U)
/*! KS9_UKSK - KSK key */
#define ELS_ELS_KS9_KS9_UKSK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UKSK_SHIFT)) & ELS_ELS_KS9_KS9_UKSK_MASK)

#define ELS_ELS_KS9_KS9_URTF_MASK                (0x8000U)
#define ELS_ELS_KS9_KS9_URTF_SHIFT               (15U)
/*! KS9_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS9_KS9_URTF(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_URTF_SHIFT)) & ELS_ELS_KS9_KS9_URTF_MASK)

#define ELS_ELS_KS9_KS9_UCKDF_MASK               (0x10000U)
#define ELS_ELS_KS9_KS9_UCKDF_SHIFT              (16U)
/*! KS9_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS9_KS9_UCKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UCKDF_SHIFT)) & ELS_ELS_KS9_KS9_UCKDF_MASK)

#define ELS_ELS_KS9_KS9_UHKDF_MASK               (0x20000U)
#define ELS_ELS_KS9_KS9_UHKDF_SHIFT              (17U)
/*! KS9_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS9_KS9_UHKDF(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UHKDF_SHIFT)) & ELS_ELS_KS9_KS9_UHKDF_MASK)

#define ELS_ELS_KS9_KS9_UECSG_MASK               (0x40000U)
#define ELS_ELS_KS9_KS9_UECSG_SHIFT              (18U)
/*! KS9_UECSG - Ecc signing key */
#define ELS_ELS_KS9_KS9_UECSG(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UECSG_SHIFT)) & ELS_ELS_KS9_KS9_UECSG_MASK)

#define ELS_ELS_KS9_KS9_UECDH_MASK               (0x80000U)
#define ELS_ELS_KS9_KS9_UECDH_SHIFT              (19U)
/*! KS9_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS9_KS9_UECDH(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UECDH_SHIFT)) & ELS_ELS_KS9_KS9_UECDH_MASK)

#define ELS_ELS_KS9_KS9_UAES_MASK                (0x100000U)
#define ELS_ELS_KS9_KS9_UAES_SHIFT               (20U)
/*! KS9_UAES - Aes key */
#define ELS_ELS_KS9_KS9_UAES(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UAES_SHIFT)) & ELS_ELS_KS9_KS9_UAES_MASK)

#define ELS_ELS_KS9_KS9_UHMAC_MASK               (0x200000U)
#define ELS_ELS_KS9_KS9_UHMAC_SHIFT              (21U)
/*! KS9_UHMAC - Hmac key */
#define ELS_ELS_KS9_KS9_UHMAC(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UHMAC_SHIFT)) & ELS_ELS_KS9_KS9_UHMAC_MASK)

#define ELS_ELS_KS9_KS9_UKWK_MASK                (0x400000U)
#define ELS_ELS_KS9_KS9_UKWK_SHIFT               (22U)
/*! KS9_UKWK - Key wrapping key */
#define ELS_ELS_KS9_KS9_UKWK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UKWK_SHIFT)) & ELS_ELS_KS9_KS9_UKWK_MASK)

#define ELS_ELS_KS9_KS9_UKUOK_MASK               (0x800000U)
#define ELS_ELS_KS9_KS9_UKUOK_SHIFT              (23U)
/*! KS9_UKUOK - Key unwrapping key */
#define ELS_ELS_KS9_KS9_UKUOK(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UKUOK_SHIFT)) & ELS_ELS_KS9_KS9_UKUOK_MASK)

#define ELS_ELS_KS9_KS9_UTLSPMS_MASK             (0x1000000U)
#define ELS_ELS_KS9_KS9_UTLSPMS_SHIFT            (24U)
/*! KS9_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS9_KS9_UTLSPMS(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UTLSPMS_SHIFT)) & ELS_ELS_KS9_KS9_UTLSPMS_MASK)

#define ELS_ELS_KS9_KS9_UTLSMS_MASK              (0x2000000U)
#define ELS_ELS_KS9_KS9_UTLSMS_SHIFT             (25U)
/*! KS9_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS9_KS9_UTLSMS(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UTLSMS_SHIFT)) & ELS_ELS_KS9_KS9_UTLSMS_MASK)

#define ELS_ELS_KS9_KS9_UKGSRC_MASK              (0x4000000U)
#define ELS_ELS_KS9_KS9_UKGSRC_SHIFT             (26U)
/*! KS9_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS9_KS9_UKGSRC(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UKGSRC_SHIFT)) & ELS_ELS_KS9_KS9_UKGSRC_MASK)

#define ELS_ELS_KS9_KS9_UHWO_MASK                (0x8000000U)
#define ELS_ELS_KS9_KS9_UHWO_SHIFT               (27U)
/*! KS9_UHWO - Hardware out key */
#define ELS_ELS_KS9_KS9_UHWO(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UHWO_SHIFT)) & ELS_ELS_KS9_KS9_UHWO_MASK)

#define ELS_ELS_KS9_KS9_UWRPOK_MASK              (0x10000000U)
#define ELS_ELS_KS9_KS9_UWRPOK_SHIFT             (28U)
/*! KS9_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS9_KS9_UWRPOK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UWRPOK_SHIFT)) & ELS_ELS_KS9_KS9_UWRPOK_MASK)

#define ELS_ELS_KS9_KS9_UDUK_MASK                (0x20000000U)
#define ELS_ELS_KS9_KS9_UDUK_SHIFT               (29U)
/*! KS9_UDUK - Device Unique Key */
#define ELS_ELS_KS9_KS9_UDUK(x)                  (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UDUK_SHIFT)) & ELS_ELS_KS9_KS9_UDUK_MASK)

#define ELS_ELS_KS9_KS9_UPPROT_MASK              (0xC0000000U)
#define ELS_ELS_KS9_KS9_UPPROT_SHIFT             (30U)
/*! KS9_UPPROT - Priviledge level */
#define ELS_ELS_KS9_KS9_UPPROT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS9_KS9_UPPROT_SHIFT)) & ELS_ELS_KS9_KS9_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS10 - Status register */
/*! @{ */

#define ELS_ELS_KS10_KS10_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS10_KS10_KSIZE_SHIFT            (0U)
/*! KS10_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS10_KS10_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_KSIZE_SHIFT)) & ELS_ELS_KS10_KS10_KSIZE_MASK)

#define ELS_ELS_KS10_KS10_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS10_KS10_RSVD0_SHIFT            (1U)
/*! KS10_RSVD0 - Reserved 0 */
#define ELS_ELS_KS10_KS10_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_RSVD0_SHIFT)) & ELS_ELS_KS10_KS10_RSVD0_MASK)

#define ELS_ELS_KS10_KS10_KACT_MASK              (0x20U)
#define ELS_ELS_KS10_KS10_KACT_SHIFT             (5U)
/*! KS10_KACT - Key is active */
#define ELS_ELS_KS10_KS10_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_KACT_SHIFT)) & ELS_ELS_KS10_KS10_KACT_MASK)

#define ELS_ELS_KS10_KS10_KBASE_MASK             (0x40U)
#define ELS_ELS_KS10_KS10_KBASE_SHIFT            (6U)
/*! KS10_KBASE - First slot in a multislot key */
#define ELS_ELS_KS10_KS10_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_KBASE_SHIFT)) & ELS_ELS_KS10_KS10_KBASE_MASK)

#define ELS_ELS_KS10_KS10_FGP_MASK               (0x80U)
#define ELS_ELS_KS10_KS10_FGP_SHIFT              (7U)
/*! KS10_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS10_KS10_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_FGP_SHIFT)) & ELS_ELS_KS10_KS10_FGP_MASK)

#define ELS_ELS_KS10_KS10_FRTN_MASK              (0x100U)
#define ELS_ELS_KS10_KS10_FRTN_SHIFT             (8U)
/*! KS10_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS10_KS10_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_FRTN_SHIFT)) & ELS_ELS_KS10_KS10_FRTN_MASK)

#define ELS_ELS_KS10_KS10_FHWO_MASK              (0x200U)
#define ELS_ELS_KS10_KS10_FHWO_SHIFT             (9U)
/*! KS10_FHWO - Hardware Feature Output */
#define ELS_ELS_KS10_KS10_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_FHWO_SHIFT)) & ELS_ELS_KS10_KS10_FHWO_MASK)

#define ELS_ELS_KS10_KS10_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS10_KS10_RSVD1_SHIFT            (10U)
/*! KS10_RSVD1 - Reserved 1 */
#define ELS_ELS_KS10_KS10_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_RSVD1_SHIFT)) & ELS_ELS_KS10_KS10_RSVD1_MASK)

#define ELS_ELS_KS10_KS10_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS10_KS10_UCMAC_SHIFT            (13U)
/*! KS10_UCMAC - CMAC key */
#define ELS_ELS_KS10_KS10_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UCMAC_SHIFT)) & ELS_ELS_KS10_KS10_UCMAC_MASK)

#define ELS_ELS_KS10_KS10_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS10_KS10_UKSK_SHIFT             (14U)
/*! KS10_UKSK - KSK key */
#define ELS_ELS_KS10_KS10_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UKSK_SHIFT)) & ELS_ELS_KS10_KS10_UKSK_MASK)

#define ELS_ELS_KS10_KS10_URTF_MASK              (0x8000U)
#define ELS_ELS_KS10_KS10_URTF_SHIFT             (15U)
/*! KS10_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS10_KS10_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_URTF_SHIFT)) & ELS_ELS_KS10_KS10_URTF_MASK)

#define ELS_ELS_KS10_KS10_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS10_KS10_UCKDF_SHIFT            (16U)
/*! KS10_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS10_KS10_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UCKDF_SHIFT)) & ELS_ELS_KS10_KS10_UCKDF_MASK)

#define ELS_ELS_KS10_KS10_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS10_KS10_UHKDF_SHIFT            (17U)
/*! KS10_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS10_KS10_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UHKDF_SHIFT)) & ELS_ELS_KS10_KS10_UHKDF_MASK)

#define ELS_ELS_KS10_KS10_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS10_KS10_UECSG_SHIFT            (18U)
/*! KS10_UECSG - Ecc signing key */
#define ELS_ELS_KS10_KS10_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UECSG_SHIFT)) & ELS_ELS_KS10_KS10_UECSG_MASK)

#define ELS_ELS_KS10_KS10_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS10_KS10_UECDH_SHIFT            (19U)
/*! KS10_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS10_KS10_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UECDH_SHIFT)) & ELS_ELS_KS10_KS10_UECDH_MASK)

#define ELS_ELS_KS10_KS10_UAES_MASK              (0x100000U)
#define ELS_ELS_KS10_KS10_UAES_SHIFT             (20U)
/*! KS10_UAES - Aes key */
#define ELS_ELS_KS10_KS10_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UAES_SHIFT)) & ELS_ELS_KS10_KS10_UAES_MASK)

#define ELS_ELS_KS10_KS10_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS10_KS10_UHMAC_SHIFT            (21U)
/*! KS10_UHMAC - Hmac key */
#define ELS_ELS_KS10_KS10_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UHMAC_SHIFT)) & ELS_ELS_KS10_KS10_UHMAC_MASK)

#define ELS_ELS_KS10_KS10_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS10_KS10_UKWK_SHIFT             (22U)
/*! KS10_UKWK - Key wrapping key */
#define ELS_ELS_KS10_KS10_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UKWK_SHIFT)) & ELS_ELS_KS10_KS10_UKWK_MASK)

#define ELS_ELS_KS10_KS10_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS10_KS10_UKUOK_SHIFT            (23U)
/*! KS10_UKUOK - Key unwrapping key */
#define ELS_ELS_KS10_KS10_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UKUOK_SHIFT)) & ELS_ELS_KS10_KS10_UKUOK_MASK)

#define ELS_ELS_KS10_KS10_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS10_KS10_UTLSPMS_SHIFT          (24U)
/*! KS10_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS10_KS10_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UTLSPMS_SHIFT)) & ELS_ELS_KS10_KS10_UTLSPMS_MASK)

#define ELS_ELS_KS10_KS10_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS10_KS10_UTLSMS_SHIFT           (25U)
/*! KS10_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS10_KS10_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UTLSMS_SHIFT)) & ELS_ELS_KS10_KS10_UTLSMS_MASK)

#define ELS_ELS_KS10_KS10_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS10_KS10_UKGSRC_SHIFT           (26U)
/*! KS10_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS10_KS10_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UKGSRC_SHIFT)) & ELS_ELS_KS10_KS10_UKGSRC_MASK)

#define ELS_ELS_KS10_KS10_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS10_KS10_UHWO_SHIFT             (27U)
/*! KS10_UHWO - Hardware out key */
#define ELS_ELS_KS10_KS10_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UHWO_SHIFT)) & ELS_ELS_KS10_KS10_UHWO_MASK)

#define ELS_ELS_KS10_KS10_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS10_KS10_UWRPOK_SHIFT           (28U)
/*! KS10_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS10_KS10_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UWRPOK_SHIFT)) & ELS_ELS_KS10_KS10_UWRPOK_MASK)

#define ELS_ELS_KS10_KS10_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS10_KS10_UDUK_SHIFT             (29U)
/*! KS10_UDUK - Device Unique Key */
#define ELS_ELS_KS10_KS10_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UDUK_SHIFT)) & ELS_ELS_KS10_KS10_UDUK_MASK)

#define ELS_ELS_KS10_KS10_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS10_KS10_UPPROT_SHIFT           (30U)
/*! KS10_UPPROT - Priviledge level */
#define ELS_ELS_KS10_KS10_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS10_KS10_UPPROT_SHIFT)) & ELS_ELS_KS10_KS10_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS11 - Status register */
/*! @{ */

#define ELS_ELS_KS11_KS11_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS11_KS11_KSIZE_SHIFT            (0U)
/*! KS11_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS11_KS11_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_KSIZE_SHIFT)) & ELS_ELS_KS11_KS11_KSIZE_MASK)

#define ELS_ELS_KS11_KS11_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS11_KS11_RSVD0_SHIFT            (1U)
/*! KS11_RSVD0 - Reserved 0 */
#define ELS_ELS_KS11_KS11_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_RSVD0_SHIFT)) & ELS_ELS_KS11_KS11_RSVD0_MASK)

#define ELS_ELS_KS11_KS11_KACT_MASK              (0x20U)
#define ELS_ELS_KS11_KS11_KACT_SHIFT             (5U)
/*! KS11_KACT - Key is active */
#define ELS_ELS_KS11_KS11_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_KACT_SHIFT)) & ELS_ELS_KS11_KS11_KACT_MASK)

#define ELS_ELS_KS11_KS11_KBASE_MASK             (0x40U)
#define ELS_ELS_KS11_KS11_KBASE_SHIFT            (6U)
/*! KS11_KBASE - First slot in a multislot key */
#define ELS_ELS_KS11_KS11_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_KBASE_SHIFT)) & ELS_ELS_KS11_KS11_KBASE_MASK)

#define ELS_ELS_KS11_KS11_FGP_MASK               (0x80U)
#define ELS_ELS_KS11_KS11_FGP_SHIFT              (7U)
/*! KS11_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS11_KS11_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_FGP_SHIFT)) & ELS_ELS_KS11_KS11_FGP_MASK)

#define ELS_ELS_KS11_KS11_FRTN_MASK              (0x100U)
#define ELS_ELS_KS11_KS11_FRTN_SHIFT             (8U)
/*! KS11_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS11_KS11_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_FRTN_SHIFT)) & ELS_ELS_KS11_KS11_FRTN_MASK)

#define ELS_ELS_KS11_KS11_FHWO_MASK              (0x200U)
#define ELS_ELS_KS11_KS11_FHWO_SHIFT             (9U)
/*! KS11_FHWO - Hardware Feature Output */
#define ELS_ELS_KS11_KS11_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_FHWO_SHIFT)) & ELS_ELS_KS11_KS11_FHWO_MASK)

#define ELS_ELS_KS11_KS11_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS11_KS11_RSVD1_SHIFT            (10U)
/*! KS11_RSVD1 - Reserved 1 */
#define ELS_ELS_KS11_KS11_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_RSVD1_SHIFT)) & ELS_ELS_KS11_KS11_RSVD1_MASK)

#define ELS_ELS_KS11_KS11_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS11_KS11_UCMAC_SHIFT            (13U)
/*! KS11_UCMAC - CMAC key */
#define ELS_ELS_KS11_KS11_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UCMAC_SHIFT)) & ELS_ELS_KS11_KS11_UCMAC_MASK)

#define ELS_ELS_KS11_KS11_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS11_KS11_UKSK_SHIFT             (14U)
/*! KS11_UKSK - KSK key */
#define ELS_ELS_KS11_KS11_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UKSK_SHIFT)) & ELS_ELS_KS11_KS11_UKSK_MASK)

#define ELS_ELS_KS11_KS11_URTF_MASK              (0x8000U)
#define ELS_ELS_KS11_KS11_URTF_SHIFT             (15U)
/*! KS11_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS11_KS11_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_URTF_SHIFT)) & ELS_ELS_KS11_KS11_URTF_MASK)

#define ELS_ELS_KS11_KS11_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS11_KS11_UCKDF_SHIFT            (16U)
/*! KS11_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS11_KS11_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UCKDF_SHIFT)) & ELS_ELS_KS11_KS11_UCKDF_MASK)

#define ELS_ELS_KS11_KS11_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS11_KS11_UHKDF_SHIFT            (17U)
/*! KS11_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS11_KS11_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UHKDF_SHIFT)) & ELS_ELS_KS11_KS11_UHKDF_MASK)

#define ELS_ELS_KS11_KS11_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS11_KS11_UECSG_SHIFT            (18U)
/*! KS11_UECSG - Ecc signing key */
#define ELS_ELS_KS11_KS11_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UECSG_SHIFT)) & ELS_ELS_KS11_KS11_UECSG_MASK)

#define ELS_ELS_KS11_KS11_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS11_KS11_UECDH_SHIFT            (19U)
/*! KS11_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS11_KS11_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UECDH_SHIFT)) & ELS_ELS_KS11_KS11_UECDH_MASK)

#define ELS_ELS_KS11_KS11_UAES_MASK              (0x100000U)
#define ELS_ELS_KS11_KS11_UAES_SHIFT             (20U)
/*! KS11_UAES - Aes key */
#define ELS_ELS_KS11_KS11_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UAES_SHIFT)) & ELS_ELS_KS11_KS11_UAES_MASK)

#define ELS_ELS_KS11_KS11_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS11_KS11_UHMAC_SHIFT            (21U)
/*! KS11_UHMAC - Hmac key */
#define ELS_ELS_KS11_KS11_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UHMAC_SHIFT)) & ELS_ELS_KS11_KS11_UHMAC_MASK)

#define ELS_ELS_KS11_KS11_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS11_KS11_UKWK_SHIFT             (22U)
/*! KS11_UKWK - Key wrapping key */
#define ELS_ELS_KS11_KS11_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UKWK_SHIFT)) & ELS_ELS_KS11_KS11_UKWK_MASK)

#define ELS_ELS_KS11_KS11_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS11_KS11_UKUOK_SHIFT            (23U)
/*! KS11_UKUOK - Key unwrapping key */
#define ELS_ELS_KS11_KS11_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UKUOK_SHIFT)) & ELS_ELS_KS11_KS11_UKUOK_MASK)

#define ELS_ELS_KS11_KS11_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS11_KS11_UTLSPMS_SHIFT          (24U)
/*! KS11_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS11_KS11_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UTLSPMS_SHIFT)) & ELS_ELS_KS11_KS11_UTLSPMS_MASK)

#define ELS_ELS_KS11_KS11_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS11_KS11_UTLSMS_SHIFT           (25U)
/*! KS11_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS11_KS11_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UTLSMS_SHIFT)) & ELS_ELS_KS11_KS11_UTLSMS_MASK)

#define ELS_ELS_KS11_KS11_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS11_KS11_UKGSRC_SHIFT           (26U)
/*! KS11_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS11_KS11_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UKGSRC_SHIFT)) & ELS_ELS_KS11_KS11_UKGSRC_MASK)

#define ELS_ELS_KS11_KS11_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS11_KS11_UHWO_SHIFT             (27U)
/*! KS11_UHWO - Hardware out key */
#define ELS_ELS_KS11_KS11_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UHWO_SHIFT)) & ELS_ELS_KS11_KS11_UHWO_MASK)

#define ELS_ELS_KS11_KS11_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS11_KS11_UWRPOK_SHIFT           (28U)
/*! KS11_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS11_KS11_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UWRPOK_SHIFT)) & ELS_ELS_KS11_KS11_UWRPOK_MASK)

#define ELS_ELS_KS11_KS11_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS11_KS11_UDUK_SHIFT             (29U)
/*! KS11_UDUK - Device Unique Key */
#define ELS_ELS_KS11_KS11_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UDUK_SHIFT)) & ELS_ELS_KS11_KS11_UDUK_MASK)

#define ELS_ELS_KS11_KS11_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS11_KS11_UPPROT_SHIFT           (30U)
/*! KS11_UPPROT - Priviledge level */
#define ELS_ELS_KS11_KS11_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS11_KS11_UPPROT_SHIFT)) & ELS_ELS_KS11_KS11_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS12 - Status register */
/*! @{ */

#define ELS_ELS_KS12_KS12_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS12_KS12_KSIZE_SHIFT            (0U)
/*! KS12_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS12_KS12_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_KSIZE_SHIFT)) & ELS_ELS_KS12_KS12_KSIZE_MASK)

#define ELS_ELS_KS12_KS12_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS12_KS12_RSVD0_SHIFT            (1U)
/*! KS12_RSVD0 - Reserved 0 */
#define ELS_ELS_KS12_KS12_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_RSVD0_SHIFT)) & ELS_ELS_KS12_KS12_RSVD0_MASK)

#define ELS_ELS_KS12_KS12_KACT_MASK              (0x20U)
#define ELS_ELS_KS12_KS12_KACT_SHIFT             (5U)
/*! KS12_KACT - Key is active */
#define ELS_ELS_KS12_KS12_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_KACT_SHIFT)) & ELS_ELS_KS12_KS12_KACT_MASK)

#define ELS_ELS_KS12_KS12_KBASE_MASK             (0x40U)
#define ELS_ELS_KS12_KS12_KBASE_SHIFT            (6U)
/*! KS12_KBASE - First slot in a multislot key */
#define ELS_ELS_KS12_KS12_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_KBASE_SHIFT)) & ELS_ELS_KS12_KS12_KBASE_MASK)

#define ELS_ELS_KS12_KS12_FGP_MASK               (0x80U)
#define ELS_ELS_KS12_KS12_FGP_SHIFT              (7U)
/*! KS12_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS12_KS12_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_FGP_SHIFT)) & ELS_ELS_KS12_KS12_FGP_MASK)

#define ELS_ELS_KS12_KS12_FRTN_MASK              (0x100U)
#define ELS_ELS_KS12_KS12_FRTN_SHIFT             (8U)
/*! KS12_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS12_KS12_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_FRTN_SHIFT)) & ELS_ELS_KS12_KS12_FRTN_MASK)

#define ELS_ELS_KS12_KS12_FHWO_MASK              (0x200U)
#define ELS_ELS_KS12_KS12_FHWO_SHIFT             (9U)
/*! KS12_FHWO - Hardware Feature Output */
#define ELS_ELS_KS12_KS12_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_FHWO_SHIFT)) & ELS_ELS_KS12_KS12_FHWO_MASK)

#define ELS_ELS_KS12_KS12_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS12_KS12_RSVD1_SHIFT            (10U)
/*! KS12_RSVD1 - Reserved 1 */
#define ELS_ELS_KS12_KS12_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_RSVD1_SHIFT)) & ELS_ELS_KS12_KS12_RSVD1_MASK)

#define ELS_ELS_KS12_KS12_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS12_KS12_UCMAC_SHIFT            (13U)
/*! KS12_UCMAC - CMAC key */
#define ELS_ELS_KS12_KS12_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UCMAC_SHIFT)) & ELS_ELS_KS12_KS12_UCMAC_MASK)

#define ELS_ELS_KS12_KS12_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS12_KS12_UKSK_SHIFT             (14U)
/*! KS12_UKSK - KSK key */
#define ELS_ELS_KS12_KS12_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UKSK_SHIFT)) & ELS_ELS_KS12_KS12_UKSK_MASK)

#define ELS_ELS_KS12_KS12_URTF_MASK              (0x8000U)
#define ELS_ELS_KS12_KS12_URTF_SHIFT             (15U)
/*! KS12_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS12_KS12_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_URTF_SHIFT)) & ELS_ELS_KS12_KS12_URTF_MASK)

#define ELS_ELS_KS12_KS12_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS12_KS12_UCKDF_SHIFT            (16U)
/*! KS12_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS12_KS12_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UCKDF_SHIFT)) & ELS_ELS_KS12_KS12_UCKDF_MASK)

#define ELS_ELS_KS12_KS12_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS12_KS12_UHKDF_SHIFT            (17U)
/*! KS12_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS12_KS12_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UHKDF_SHIFT)) & ELS_ELS_KS12_KS12_UHKDF_MASK)

#define ELS_ELS_KS12_KS12_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS12_KS12_UECSG_SHIFT            (18U)
/*! KS12_UECSG - Ecc signing key */
#define ELS_ELS_KS12_KS12_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UECSG_SHIFT)) & ELS_ELS_KS12_KS12_UECSG_MASK)

#define ELS_ELS_KS12_KS12_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS12_KS12_UECDH_SHIFT            (19U)
/*! KS12_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS12_KS12_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UECDH_SHIFT)) & ELS_ELS_KS12_KS12_UECDH_MASK)

#define ELS_ELS_KS12_KS12_UAES_MASK              (0x100000U)
#define ELS_ELS_KS12_KS12_UAES_SHIFT             (20U)
/*! KS12_UAES - Aes key */
#define ELS_ELS_KS12_KS12_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UAES_SHIFT)) & ELS_ELS_KS12_KS12_UAES_MASK)

#define ELS_ELS_KS12_KS12_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS12_KS12_UHMAC_SHIFT            (21U)
/*! KS12_UHMAC - Hmac key */
#define ELS_ELS_KS12_KS12_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UHMAC_SHIFT)) & ELS_ELS_KS12_KS12_UHMAC_MASK)

#define ELS_ELS_KS12_KS12_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS12_KS12_UKWK_SHIFT             (22U)
/*! KS12_UKWK - Key wrapping key */
#define ELS_ELS_KS12_KS12_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UKWK_SHIFT)) & ELS_ELS_KS12_KS12_UKWK_MASK)

#define ELS_ELS_KS12_KS12_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS12_KS12_UKUOK_SHIFT            (23U)
/*! KS12_UKUOK - Key unwrapping key */
#define ELS_ELS_KS12_KS12_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UKUOK_SHIFT)) & ELS_ELS_KS12_KS12_UKUOK_MASK)

#define ELS_ELS_KS12_KS12_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS12_KS12_UTLSPMS_SHIFT          (24U)
/*! KS12_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS12_KS12_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UTLSPMS_SHIFT)) & ELS_ELS_KS12_KS12_UTLSPMS_MASK)

#define ELS_ELS_KS12_KS12_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS12_KS12_UTLSMS_SHIFT           (25U)
/*! KS12_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS12_KS12_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UTLSMS_SHIFT)) & ELS_ELS_KS12_KS12_UTLSMS_MASK)

#define ELS_ELS_KS12_KS12_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS12_KS12_UKGSRC_SHIFT           (26U)
/*! KS12_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS12_KS12_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UKGSRC_SHIFT)) & ELS_ELS_KS12_KS12_UKGSRC_MASK)

#define ELS_ELS_KS12_KS12_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS12_KS12_UHWO_SHIFT             (27U)
/*! KS12_UHWO - Hardware out key */
#define ELS_ELS_KS12_KS12_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UHWO_SHIFT)) & ELS_ELS_KS12_KS12_UHWO_MASK)

#define ELS_ELS_KS12_KS12_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS12_KS12_UWRPOK_SHIFT           (28U)
/*! KS12_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS12_KS12_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UWRPOK_SHIFT)) & ELS_ELS_KS12_KS12_UWRPOK_MASK)

#define ELS_ELS_KS12_KS12_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS12_KS12_UDUK_SHIFT             (29U)
/*! KS12_UDUK - Device Unique Key */
#define ELS_ELS_KS12_KS12_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UDUK_SHIFT)) & ELS_ELS_KS12_KS12_UDUK_MASK)

#define ELS_ELS_KS12_KS12_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS12_KS12_UPPROT_SHIFT           (30U)
/*! KS12_UPPROT - Priviledge level */
#define ELS_ELS_KS12_KS12_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS12_KS12_UPPROT_SHIFT)) & ELS_ELS_KS12_KS12_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS13 - Status register */
/*! @{ */

#define ELS_ELS_KS13_KS13_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS13_KS13_KSIZE_SHIFT            (0U)
/*! KS13_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS13_KS13_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_KSIZE_SHIFT)) & ELS_ELS_KS13_KS13_KSIZE_MASK)

#define ELS_ELS_KS13_KS13_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS13_KS13_RSVD0_SHIFT            (1U)
/*! KS13_RSVD0 - Reserved 0 */
#define ELS_ELS_KS13_KS13_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_RSVD0_SHIFT)) & ELS_ELS_KS13_KS13_RSVD0_MASK)

#define ELS_ELS_KS13_KS13_KACT_MASK              (0x20U)
#define ELS_ELS_KS13_KS13_KACT_SHIFT             (5U)
/*! KS13_KACT - Key is active */
#define ELS_ELS_KS13_KS13_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_KACT_SHIFT)) & ELS_ELS_KS13_KS13_KACT_MASK)

#define ELS_ELS_KS13_KS13_KBASE_MASK             (0x40U)
#define ELS_ELS_KS13_KS13_KBASE_SHIFT            (6U)
/*! KS13_KBASE - First slot in a multislot key */
#define ELS_ELS_KS13_KS13_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_KBASE_SHIFT)) & ELS_ELS_KS13_KS13_KBASE_MASK)

#define ELS_ELS_KS13_KS13_FGP_MASK               (0x80U)
#define ELS_ELS_KS13_KS13_FGP_SHIFT              (7U)
/*! KS13_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS13_KS13_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_FGP_SHIFT)) & ELS_ELS_KS13_KS13_FGP_MASK)

#define ELS_ELS_KS13_KS13_FRTN_MASK              (0x100U)
#define ELS_ELS_KS13_KS13_FRTN_SHIFT             (8U)
/*! KS13_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS13_KS13_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_FRTN_SHIFT)) & ELS_ELS_KS13_KS13_FRTN_MASK)

#define ELS_ELS_KS13_KS13_FHWO_MASK              (0x200U)
#define ELS_ELS_KS13_KS13_FHWO_SHIFT             (9U)
/*! KS13_FHWO - Hardware Feature Output */
#define ELS_ELS_KS13_KS13_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_FHWO_SHIFT)) & ELS_ELS_KS13_KS13_FHWO_MASK)

#define ELS_ELS_KS13_KS13_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS13_KS13_RSVD1_SHIFT            (10U)
/*! KS13_RSVD1 - Reserved 1 */
#define ELS_ELS_KS13_KS13_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_RSVD1_SHIFT)) & ELS_ELS_KS13_KS13_RSVD1_MASK)

#define ELS_ELS_KS13_KS13_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS13_KS13_UCMAC_SHIFT            (13U)
/*! KS13_UCMAC - CMAC key */
#define ELS_ELS_KS13_KS13_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UCMAC_SHIFT)) & ELS_ELS_KS13_KS13_UCMAC_MASK)

#define ELS_ELS_KS13_KS13_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS13_KS13_UKSK_SHIFT             (14U)
/*! KS13_UKSK - KSK key */
#define ELS_ELS_KS13_KS13_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UKSK_SHIFT)) & ELS_ELS_KS13_KS13_UKSK_MASK)

#define ELS_ELS_KS13_KS13_URTF_MASK              (0x8000U)
#define ELS_ELS_KS13_KS13_URTF_SHIFT             (15U)
/*! KS13_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS13_KS13_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_URTF_SHIFT)) & ELS_ELS_KS13_KS13_URTF_MASK)

#define ELS_ELS_KS13_KS13_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS13_KS13_UCKDF_SHIFT            (16U)
/*! KS13_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS13_KS13_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UCKDF_SHIFT)) & ELS_ELS_KS13_KS13_UCKDF_MASK)

#define ELS_ELS_KS13_KS13_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS13_KS13_UHKDF_SHIFT            (17U)
/*! KS13_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS13_KS13_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UHKDF_SHIFT)) & ELS_ELS_KS13_KS13_UHKDF_MASK)

#define ELS_ELS_KS13_KS13_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS13_KS13_UECSG_SHIFT            (18U)
/*! KS13_UECSG - Ecc signing key */
#define ELS_ELS_KS13_KS13_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UECSG_SHIFT)) & ELS_ELS_KS13_KS13_UECSG_MASK)

#define ELS_ELS_KS13_KS13_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS13_KS13_UECDH_SHIFT            (19U)
/*! KS13_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS13_KS13_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UECDH_SHIFT)) & ELS_ELS_KS13_KS13_UECDH_MASK)

#define ELS_ELS_KS13_KS13_UAES_MASK              (0x100000U)
#define ELS_ELS_KS13_KS13_UAES_SHIFT             (20U)
/*! KS13_UAES - Aes key */
#define ELS_ELS_KS13_KS13_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UAES_SHIFT)) & ELS_ELS_KS13_KS13_UAES_MASK)

#define ELS_ELS_KS13_KS13_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS13_KS13_UHMAC_SHIFT            (21U)
/*! KS13_UHMAC - Hmac key */
#define ELS_ELS_KS13_KS13_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UHMAC_SHIFT)) & ELS_ELS_KS13_KS13_UHMAC_MASK)

#define ELS_ELS_KS13_KS13_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS13_KS13_UKWK_SHIFT             (22U)
/*! KS13_UKWK - Key wrapping key */
#define ELS_ELS_KS13_KS13_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UKWK_SHIFT)) & ELS_ELS_KS13_KS13_UKWK_MASK)

#define ELS_ELS_KS13_KS13_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS13_KS13_UKUOK_SHIFT            (23U)
/*! KS13_UKUOK - Key unwrapping key */
#define ELS_ELS_KS13_KS13_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UKUOK_SHIFT)) & ELS_ELS_KS13_KS13_UKUOK_MASK)

#define ELS_ELS_KS13_KS13_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS13_KS13_UTLSPMS_SHIFT          (24U)
/*! KS13_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS13_KS13_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UTLSPMS_SHIFT)) & ELS_ELS_KS13_KS13_UTLSPMS_MASK)

#define ELS_ELS_KS13_KS13_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS13_KS13_UTLSMS_SHIFT           (25U)
/*! KS13_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS13_KS13_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UTLSMS_SHIFT)) & ELS_ELS_KS13_KS13_UTLSMS_MASK)

#define ELS_ELS_KS13_KS13_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS13_KS13_UKGSRC_SHIFT           (26U)
/*! KS13_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS13_KS13_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UKGSRC_SHIFT)) & ELS_ELS_KS13_KS13_UKGSRC_MASK)

#define ELS_ELS_KS13_KS13_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS13_KS13_UHWO_SHIFT             (27U)
/*! KS13_UHWO - Hardware out key */
#define ELS_ELS_KS13_KS13_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UHWO_SHIFT)) & ELS_ELS_KS13_KS13_UHWO_MASK)

#define ELS_ELS_KS13_KS13_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS13_KS13_UWRPOK_SHIFT           (28U)
/*! KS13_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS13_KS13_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UWRPOK_SHIFT)) & ELS_ELS_KS13_KS13_UWRPOK_MASK)

#define ELS_ELS_KS13_KS13_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS13_KS13_UDUK_SHIFT             (29U)
/*! KS13_UDUK - Device Unique Key */
#define ELS_ELS_KS13_KS13_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UDUK_SHIFT)) & ELS_ELS_KS13_KS13_UDUK_MASK)

#define ELS_ELS_KS13_KS13_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS13_KS13_UPPROT_SHIFT           (30U)
/*! KS13_UPPROT - Priviledge level */
#define ELS_ELS_KS13_KS13_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS13_KS13_UPPROT_SHIFT)) & ELS_ELS_KS13_KS13_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS14 - Status register */
/*! @{ */

#define ELS_ELS_KS14_KS14_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS14_KS14_KSIZE_SHIFT            (0U)
/*! KS14_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS14_KS14_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_KSIZE_SHIFT)) & ELS_ELS_KS14_KS14_KSIZE_MASK)

#define ELS_ELS_KS14_KS14_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS14_KS14_RSVD0_SHIFT            (1U)
/*! KS14_RSVD0 - Reserved 0 */
#define ELS_ELS_KS14_KS14_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_RSVD0_SHIFT)) & ELS_ELS_KS14_KS14_RSVD0_MASK)

#define ELS_ELS_KS14_KS14_KACT_MASK              (0x20U)
#define ELS_ELS_KS14_KS14_KACT_SHIFT             (5U)
/*! KS14_KACT - Key is active */
#define ELS_ELS_KS14_KS14_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_KACT_SHIFT)) & ELS_ELS_KS14_KS14_KACT_MASK)

#define ELS_ELS_KS14_KS14_KBASE_MASK             (0x40U)
#define ELS_ELS_KS14_KS14_KBASE_SHIFT            (6U)
/*! KS14_KBASE - First slot in a multislot key */
#define ELS_ELS_KS14_KS14_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_KBASE_SHIFT)) & ELS_ELS_KS14_KS14_KBASE_MASK)

#define ELS_ELS_KS14_KS14_FGP_MASK               (0x80U)
#define ELS_ELS_KS14_KS14_FGP_SHIFT              (7U)
/*! KS14_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS14_KS14_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_FGP_SHIFT)) & ELS_ELS_KS14_KS14_FGP_MASK)

#define ELS_ELS_KS14_KS14_FRTN_MASK              (0x100U)
#define ELS_ELS_KS14_KS14_FRTN_SHIFT             (8U)
/*! KS14_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS14_KS14_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_FRTN_SHIFT)) & ELS_ELS_KS14_KS14_FRTN_MASK)

#define ELS_ELS_KS14_KS14_FHWO_MASK              (0x200U)
#define ELS_ELS_KS14_KS14_FHWO_SHIFT             (9U)
/*! KS14_FHWO - Hardware Feature Output */
#define ELS_ELS_KS14_KS14_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_FHWO_SHIFT)) & ELS_ELS_KS14_KS14_FHWO_MASK)

#define ELS_ELS_KS14_KS14_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS14_KS14_RSVD1_SHIFT            (10U)
/*! KS14_RSVD1 - Reserved 1 */
#define ELS_ELS_KS14_KS14_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_RSVD1_SHIFT)) & ELS_ELS_KS14_KS14_RSVD1_MASK)

#define ELS_ELS_KS14_KS14_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS14_KS14_UCMAC_SHIFT            (13U)
/*! KS14_UCMAC - CMAC key */
#define ELS_ELS_KS14_KS14_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UCMAC_SHIFT)) & ELS_ELS_KS14_KS14_UCMAC_MASK)

#define ELS_ELS_KS14_KS14_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS14_KS14_UKSK_SHIFT             (14U)
/*! KS14_UKSK - KSK key */
#define ELS_ELS_KS14_KS14_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UKSK_SHIFT)) & ELS_ELS_KS14_KS14_UKSK_MASK)

#define ELS_ELS_KS14_KS14_URTF_MASK              (0x8000U)
#define ELS_ELS_KS14_KS14_URTF_SHIFT             (15U)
/*! KS14_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS14_KS14_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_URTF_SHIFT)) & ELS_ELS_KS14_KS14_URTF_MASK)

#define ELS_ELS_KS14_KS14_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS14_KS14_UCKDF_SHIFT            (16U)
/*! KS14_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS14_KS14_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UCKDF_SHIFT)) & ELS_ELS_KS14_KS14_UCKDF_MASK)

#define ELS_ELS_KS14_KS14_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS14_KS14_UHKDF_SHIFT            (17U)
/*! KS14_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS14_KS14_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UHKDF_SHIFT)) & ELS_ELS_KS14_KS14_UHKDF_MASK)

#define ELS_ELS_KS14_KS14_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS14_KS14_UECSG_SHIFT            (18U)
/*! KS14_UECSG - Ecc signing key */
#define ELS_ELS_KS14_KS14_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UECSG_SHIFT)) & ELS_ELS_KS14_KS14_UECSG_MASK)

#define ELS_ELS_KS14_KS14_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS14_KS14_UECDH_SHIFT            (19U)
/*! KS14_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS14_KS14_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UECDH_SHIFT)) & ELS_ELS_KS14_KS14_UECDH_MASK)

#define ELS_ELS_KS14_KS14_UAES_MASK              (0x100000U)
#define ELS_ELS_KS14_KS14_UAES_SHIFT             (20U)
/*! KS14_UAES - Aes key */
#define ELS_ELS_KS14_KS14_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UAES_SHIFT)) & ELS_ELS_KS14_KS14_UAES_MASK)

#define ELS_ELS_KS14_KS14_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS14_KS14_UHMAC_SHIFT            (21U)
/*! KS14_UHMAC - Hmac key */
#define ELS_ELS_KS14_KS14_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UHMAC_SHIFT)) & ELS_ELS_KS14_KS14_UHMAC_MASK)

#define ELS_ELS_KS14_KS14_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS14_KS14_UKWK_SHIFT             (22U)
/*! KS14_UKWK - Key wrapping key */
#define ELS_ELS_KS14_KS14_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UKWK_SHIFT)) & ELS_ELS_KS14_KS14_UKWK_MASK)

#define ELS_ELS_KS14_KS14_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS14_KS14_UKUOK_SHIFT            (23U)
/*! KS14_UKUOK - Key unwrapping key */
#define ELS_ELS_KS14_KS14_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UKUOK_SHIFT)) & ELS_ELS_KS14_KS14_UKUOK_MASK)

#define ELS_ELS_KS14_KS14_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS14_KS14_UTLSPMS_SHIFT          (24U)
/*! KS14_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS14_KS14_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UTLSPMS_SHIFT)) & ELS_ELS_KS14_KS14_UTLSPMS_MASK)

#define ELS_ELS_KS14_KS14_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS14_KS14_UTLSMS_SHIFT           (25U)
/*! KS14_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS14_KS14_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UTLSMS_SHIFT)) & ELS_ELS_KS14_KS14_UTLSMS_MASK)

#define ELS_ELS_KS14_KS14_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS14_KS14_UKGSRC_SHIFT           (26U)
/*! KS14_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS14_KS14_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UKGSRC_SHIFT)) & ELS_ELS_KS14_KS14_UKGSRC_MASK)

#define ELS_ELS_KS14_KS14_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS14_KS14_UHWO_SHIFT             (27U)
/*! KS14_UHWO - Hardware out key */
#define ELS_ELS_KS14_KS14_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UHWO_SHIFT)) & ELS_ELS_KS14_KS14_UHWO_MASK)

#define ELS_ELS_KS14_KS14_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS14_KS14_UWRPOK_SHIFT           (28U)
/*! KS14_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS14_KS14_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UWRPOK_SHIFT)) & ELS_ELS_KS14_KS14_UWRPOK_MASK)

#define ELS_ELS_KS14_KS14_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS14_KS14_UDUK_SHIFT             (29U)
/*! KS14_UDUK - Device Unique Key */
#define ELS_ELS_KS14_KS14_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UDUK_SHIFT)) & ELS_ELS_KS14_KS14_UDUK_MASK)

#define ELS_ELS_KS14_KS14_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS14_KS14_UPPROT_SHIFT           (30U)
/*! KS14_UPPROT - Priviledge level */
#define ELS_ELS_KS14_KS14_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS14_KS14_UPPROT_SHIFT)) & ELS_ELS_KS14_KS14_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS15 - Status register */
/*! @{ */

#define ELS_ELS_KS15_KS15_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS15_KS15_KSIZE_SHIFT            (0U)
/*! KS15_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS15_KS15_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_KSIZE_SHIFT)) & ELS_ELS_KS15_KS15_KSIZE_MASK)

#define ELS_ELS_KS15_KS15_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS15_KS15_RSVD0_SHIFT            (1U)
/*! KS15_RSVD0 - Reserved 0 */
#define ELS_ELS_KS15_KS15_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_RSVD0_SHIFT)) & ELS_ELS_KS15_KS15_RSVD0_MASK)

#define ELS_ELS_KS15_KS15_KACT_MASK              (0x20U)
#define ELS_ELS_KS15_KS15_KACT_SHIFT             (5U)
/*! KS15_KACT - Key is active */
#define ELS_ELS_KS15_KS15_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_KACT_SHIFT)) & ELS_ELS_KS15_KS15_KACT_MASK)

#define ELS_ELS_KS15_KS15_KBASE_MASK             (0x40U)
#define ELS_ELS_KS15_KS15_KBASE_SHIFT            (6U)
/*! KS15_KBASE - First slot in a multislot key */
#define ELS_ELS_KS15_KS15_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_KBASE_SHIFT)) & ELS_ELS_KS15_KS15_KBASE_MASK)

#define ELS_ELS_KS15_KS15_FGP_MASK               (0x80U)
#define ELS_ELS_KS15_KS15_FGP_SHIFT              (7U)
/*! KS15_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS15_KS15_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_FGP_SHIFT)) & ELS_ELS_KS15_KS15_FGP_MASK)

#define ELS_ELS_KS15_KS15_FRTN_MASK              (0x100U)
#define ELS_ELS_KS15_KS15_FRTN_SHIFT             (8U)
/*! KS15_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS15_KS15_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_FRTN_SHIFT)) & ELS_ELS_KS15_KS15_FRTN_MASK)

#define ELS_ELS_KS15_KS15_FHWO_MASK              (0x200U)
#define ELS_ELS_KS15_KS15_FHWO_SHIFT             (9U)
/*! KS15_FHWO - Hardware Feature Output */
#define ELS_ELS_KS15_KS15_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_FHWO_SHIFT)) & ELS_ELS_KS15_KS15_FHWO_MASK)

#define ELS_ELS_KS15_KS15_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS15_KS15_RSVD1_SHIFT            (10U)
/*! KS15_RSVD1 - Reserved 1 */
#define ELS_ELS_KS15_KS15_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_RSVD1_SHIFT)) & ELS_ELS_KS15_KS15_RSVD1_MASK)

#define ELS_ELS_KS15_KS15_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS15_KS15_UCMAC_SHIFT            (13U)
/*! KS15_UCMAC - CMAC key */
#define ELS_ELS_KS15_KS15_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UCMAC_SHIFT)) & ELS_ELS_KS15_KS15_UCMAC_MASK)

#define ELS_ELS_KS15_KS15_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS15_KS15_UKSK_SHIFT             (14U)
/*! KS15_UKSK - KSK key */
#define ELS_ELS_KS15_KS15_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UKSK_SHIFT)) & ELS_ELS_KS15_KS15_UKSK_MASK)

#define ELS_ELS_KS15_KS15_URTF_MASK              (0x8000U)
#define ELS_ELS_KS15_KS15_URTF_SHIFT             (15U)
/*! KS15_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS15_KS15_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_URTF_SHIFT)) & ELS_ELS_KS15_KS15_URTF_MASK)

#define ELS_ELS_KS15_KS15_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS15_KS15_UCKDF_SHIFT            (16U)
/*! KS15_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS15_KS15_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UCKDF_SHIFT)) & ELS_ELS_KS15_KS15_UCKDF_MASK)

#define ELS_ELS_KS15_KS15_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS15_KS15_UHKDF_SHIFT            (17U)
/*! KS15_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS15_KS15_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UHKDF_SHIFT)) & ELS_ELS_KS15_KS15_UHKDF_MASK)

#define ELS_ELS_KS15_KS15_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS15_KS15_UECSG_SHIFT            (18U)
/*! KS15_UECSG - Ecc signing key */
#define ELS_ELS_KS15_KS15_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UECSG_SHIFT)) & ELS_ELS_KS15_KS15_UECSG_MASK)

#define ELS_ELS_KS15_KS15_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS15_KS15_UECDH_SHIFT            (19U)
/*! KS15_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS15_KS15_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UECDH_SHIFT)) & ELS_ELS_KS15_KS15_UECDH_MASK)

#define ELS_ELS_KS15_KS15_UAES_MASK              (0x100000U)
#define ELS_ELS_KS15_KS15_UAES_SHIFT             (20U)
/*! KS15_UAES - Aes key */
#define ELS_ELS_KS15_KS15_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UAES_SHIFT)) & ELS_ELS_KS15_KS15_UAES_MASK)

#define ELS_ELS_KS15_KS15_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS15_KS15_UHMAC_SHIFT            (21U)
/*! KS15_UHMAC - Hmac key */
#define ELS_ELS_KS15_KS15_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UHMAC_SHIFT)) & ELS_ELS_KS15_KS15_UHMAC_MASK)

#define ELS_ELS_KS15_KS15_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS15_KS15_UKWK_SHIFT             (22U)
/*! KS15_UKWK - Key wrapping key */
#define ELS_ELS_KS15_KS15_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UKWK_SHIFT)) & ELS_ELS_KS15_KS15_UKWK_MASK)

#define ELS_ELS_KS15_KS15_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS15_KS15_UKUOK_SHIFT            (23U)
/*! KS15_UKUOK - Key unwrapping key */
#define ELS_ELS_KS15_KS15_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UKUOK_SHIFT)) & ELS_ELS_KS15_KS15_UKUOK_MASK)

#define ELS_ELS_KS15_KS15_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS15_KS15_UTLSPMS_SHIFT          (24U)
/*! KS15_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS15_KS15_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UTLSPMS_SHIFT)) & ELS_ELS_KS15_KS15_UTLSPMS_MASK)

#define ELS_ELS_KS15_KS15_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS15_KS15_UTLSMS_SHIFT           (25U)
/*! KS15_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS15_KS15_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UTLSMS_SHIFT)) & ELS_ELS_KS15_KS15_UTLSMS_MASK)

#define ELS_ELS_KS15_KS15_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS15_KS15_UKGSRC_SHIFT           (26U)
/*! KS15_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS15_KS15_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UKGSRC_SHIFT)) & ELS_ELS_KS15_KS15_UKGSRC_MASK)

#define ELS_ELS_KS15_KS15_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS15_KS15_UHWO_SHIFT             (27U)
/*! KS15_UHWO - Hardware out key */
#define ELS_ELS_KS15_KS15_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UHWO_SHIFT)) & ELS_ELS_KS15_KS15_UHWO_MASK)

#define ELS_ELS_KS15_KS15_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS15_KS15_UWRPOK_SHIFT           (28U)
/*! KS15_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS15_KS15_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UWRPOK_SHIFT)) & ELS_ELS_KS15_KS15_UWRPOK_MASK)

#define ELS_ELS_KS15_KS15_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS15_KS15_UDUK_SHIFT             (29U)
/*! KS15_UDUK - Device Unique Key */
#define ELS_ELS_KS15_KS15_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UDUK_SHIFT)) & ELS_ELS_KS15_KS15_UDUK_MASK)

#define ELS_ELS_KS15_KS15_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS15_KS15_UPPROT_SHIFT           (30U)
/*! KS15_UPPROT - Priviledge level */
#define ELS_ELS_KS15_KS15_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS15_KS15_UPPROT_SHIFT)) & ELS_ELS_KS15_KS15_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS16 - Status register */
/*! @{ */

#define ELS_ELS_KS16_KS16_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS16_KS16_KSIZE_SHIFT            (0U)
/*! KS16_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS16_KS16_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_KSIZE_SHIFT)) & ELS_ELS_KS16_KS16_KSIZE_MASK)

#define ELS_ELS_KS16_KS16_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS16_KS16_RSVD0_SHIFT            (1U)
/*! KS16_RSVD0 - Reserved 0 */
#define ELS_ELS_KS16_KS16_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_RSVD0_SHIFT)) & ELS_ELS_KS16_KS16_RSVD0_MASK)

#define ELS_ELS_KS16_KS16_KACT_MASK              (0x20U)
#define ELS_ELS_KS16_KS16_KACT_SHIFT             (5U)
/*! KS16_KACT - Key is active */
#define ELS_ELS_KS16_KS16_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_KACT_SHIFT)) & ELS_ELS_KS16_KS16_KACT_MASK)

#define ELS_ELS_KS16_KS16_KBASE_MASK             (0x40U)
#define ELS_ELS_KS16_KS16_KBASE_SHIFT            (6U)
/*! KS16_KBASE - First slot in a multislot key */
#define ELS_ELS_KS16_KS16_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_KBASE_SHIFT)) & ELS_ELS_KS16_KS16_KBASE_MASK)

#define ELS_ELS_KS16_KS16_FGP_MASK               (0x80U)
#define ELS_ELS_KS16_KS16_FGP_SHIFT              (7U)
/*! KS16_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS16_KS16_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_FGP_SHIFT)) & ELS_ELS_KS16_KS16_FGP_MASK)

#define ELS_ELS_KS16_KS16_FRTN_MASK              (0x100U)
#define ELS_ELS_KS16_KS16_FRTN_SHIFT             (8U)
/*! KS16_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS16_KS16_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_FRTN_SHIFT)) & ELS_ELS_KS16_KS16_FRTN_MASK)

#define ELS_ELS_KS16_KS16_FHWO_MASK              (0x200U)
#define ELS_ELS_KS16_KS16_FHWO_SHIFT             (9U)
/*! KS16_FHWO - Hardware Feature Output */
#define ELS_ELS_KS16_KS16_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_FHWO_SHIFT)) & ELS_ELS_KS16_KS16_FHWO_MASK)

#define ELS_ELS_KS16_KS16_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS16_KS16_RSVD1_SHIFT            (10U)
/*! KS16_RSVD1 - Reserved 1 */
#define ELS_ELS_KS16_KS16_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_RSVD1_SHIFT)) & ELS_ELS_KS16_KS16_RSVD1_MASK)

#define ELS_ELS_KS16_KS16_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS16_KS16_UCMAC_SHIFT            (13U)
/*! KS16_UCMAC - CMAC key */
#define ELS_ELS_KS16_KS16_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UCMAC_SHIFT)) & ELS_ELS_KS16_KS16_UCMAC_MASK)

#define ELS_ELS_KS16_KS16_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS16_KS16_UKSK_SHIFT             (14U)
/*! KS16_UKSK - KSK key */
#define ELS_ELS_KS16_KS16_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UKSK_SHIFT)) & ELS_ELS_KS16_KS16_UKSK_MASK)

#define ELS_ELS_KS16_KS16_URTF_MASK              (0x8000U)
#define ELS_ELS_KS16_KS16_URTF_SHIFT             (15U)
/*! KS16_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS16_KS16_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_URTF_SHIFT)) & ELS_ELS_KS16_KS16_URTF_MASK)

#define ELS_ELS_KS16_KS16_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS16_KS16_UCKDF_SHIFT            (16U)
/*! KS16_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS16_KS16_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UCKDF_SHIFT)) & ELS_ELS_KS16_KS16_UCKDF_MASK)

#define ELS_ELS_KS16_KS16_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS16_KS16_UHKDF_SHIFT            (17U)
/*! KS16_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS16_KS16_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UHKDF_SHIFT)) & ELS_ELS_KS16_KS16_UHKDF_MASK)

#define ELS_ELS_KS16_KS16_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS16_KS16_UECSG_SHIFT            (18U)
/*! KS16_UECSG - Ecc signing key */
#define ELS_ELS_KS16_KS16_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UECSG_SHIFT)) & ELS_ELS_KS16_KS16_UECSG_MASK)

#define ELS_ELS_KS16_KS16_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS16_KS16_UECDH_SHIFT            (19U)
/*! KS16_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS16_KS16_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UECDH_SHIFT)) & ELS_ELS_KS16_KS16_UECDH_MASK)

#define ELS_ELS_KS16_KS16_UAES_MASK              (0x100000U)
#define ELS_ELS_KS16_KS16_UAES_SHIFT             (20U)
/*! KS16_UAES - Aes key */
#define ELS_ELS_KS16_KS16_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UAES_SHIFT)) & ELS_ELS_KS16_KS16_UAES_MASK)

#define ELS_ELS_KS16_KS16_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS16_KS16_UHMAC_SHIFT            (21U)
/*! KS16_UHMAC - Hmac key */
#define ELS_ELS_KS16_KS16_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UHMAC_SHIFT)) & ELS_ELS_KS16_KS16_UHMAC_MASK)

#define ELS_ELS_KS16_KS16_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS16_KS16_UKWK_SHIFT             (22U)
/*! KS16_UKWK - Key wrapping key */
#define ELS_ELS_KS16_KS16_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UKWK_SHIFT)) & ELS_ELS_KS16_KS16_UKWK_MASK)

#define ELS_ELS_KS16_KS16_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS16_KS16_UKUOK_SHIFT            (23U)
/*! KS16_UKUOK - Key unwrapping key */
#define ELS_ELS_KS16_KS16_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UKUOK_SHIFT)) & ELS_ELS_KS16_KS16_UKUOK_MASK)

#define ELS_ELS_KS16_KS16_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS16_KS16_UTLSPMS_SHIFT          (24U)
/*! KS16_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS16_KS16_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UTLSPMS_SHIFT)) & ELS_ELS_KS16_KS16_UTLSPMS_MASK)

#define ELS_ELS_KS16_KS16_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS16_KS16_UTLSMS_SHIFT           (25U)
/*! KS16_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS16_KS16_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UTLSMS_SHIFT)) & ELS_ELS_KS16_KS16_UTLSMS_MASK)

#define ELS_ELS_KS16_KS16_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS16_KS16_UKGSRC_SHIFT           (26U)
/*! KS16_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS16_KS16_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UKGSRC_SHIFT)) & ELS_ELS_KS16_KS16_UKGSRC_MASK)

#define ELS_ELS_KS16_KS16_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS16_KS16_UHWO_SHIFT             (27U)
/*! KS16_UHWO - Hardware out key */
#define ELS_ELS_KS16_KS16_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UHWO_SHIFT)) & ELS_ELS_KS16_KS16_UHWO_MASK)

#define ELS_ELS_KS16_KS16_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS16_KS16_UWRPOK_SHIFT           (28U)
/*! KS16_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS16_KS16_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UWRPOK_SHIFT)) & ELS_ELS_KS16_KS16_UWRPOK_MASK)

#define ELS_ELS_KS16_KS16_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS16_KS16_UDUK_SHIFT             (29U)
/*! KS16_UDUK - Device Unique Key */
#define ELS_ELS_KS16_KS16_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UDUK_SHIFT)) & ELS_ELS_KS16_KS16_UDUK_MASK)

#define ELS_ELS_KS16_KS16_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS16_KS16_UPPROT_SHIFT           (30U)
/*! KS16_UPPROT - Priviledge level */
#define ELS_ELS_KS16_KS16_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS16_KS16_UPPROT_SHIFT)) & ELS_ELS_KS16_KS16_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS17 - Status register */
/*! @{ */

#define ELS_ELS_KS17_KS17_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS17_KS17_KSIZE_SHIFT            (0U)
/*! KS17_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS17_KS17_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_KSIZE_SHIFT)) & ELS_ELS_KS17_KS17_KSIZE_MASK)

#define ELS_ELS_KS17_KS17_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS17_KS17_RSVD0_SHIFT            (1U)
/*! KS17_RSVD0 - Reserved 0 */
#define ELS_ELS_KS17_KS17_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_RSVD0_SHIFT)) & ELS_ELS_KS17_KS17_RSVD0_MASK)

#define ELS_ELS_KS17_KS17_KACT_MASK              (0x20U)
#define ELS_ELS_KS17_KS17_KACT_SHIFT             (5U)
/*! KS17_KACT - Key is active */
#define ELS_ELS_KS17_KS17_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_KACT_SHIFT)) & ELS_ELS_KS17_KS17_KACT_MASK)

#define ELS_ELS_KS17_KS17_KBASE_MASK             (0x40U)
#define ELS_ELS_KS17_KS17_KBASE_SHIFT            (6U)
/*! KS17_KBASE - First slot in a multislot key */
#define ELS_ELS_KS17_KS17_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_KBASE_SHIFT)) & ELS_ELS_KS17_KS17_KBASE_MASK)

#define ELS_ELS_KS17_KS17_FGP_MASK               (0x80U)
#define ELS_ELS_KS17_KS17_FGP_SHIFT              (7U)
/*! KS17_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS17_KS17_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_FGP_SHIFT)) & ELS_ELS_KS17_KS17_FGP_MASK)

#define ELS_ELS_KS17_KS17_FRTN_MASK              (0x100U)
#define ELS_ELS_KS17_KS17_FRTN_SHIFT             (8U)
/*! KS17_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS17_KS17_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_FRTN_SHIFT)) & ELS_ELS_KS17_KS17_FRTN_MASK)

#define ELS_ELS_KS17_KS17_FHWO_MASK              (0x200U)
#define ELS_ELS_KS17_KS17_FHWO_SHIFT             (9U)
/*! KS17_FHWO - Hardware Feature Output */
#define ELS_ELS_KS17_KS17_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_FHWO_SHIFT)) & ELS_ELS_KS17_KS17_FHWO_MASK)

#define ELS_ELS_KS17_KS17_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS17_KS17_RSVD1_SHIFT            (10U)
/*! KS17_RSVD1 - Reserved 1 */
#define ELS_ELS_KS17_KS17_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_RSVD1_SHIFT)) & ELS_ELS_KS17_KS17_RSVD1_MASK)

#define ELS_ELS_KS17_KS17_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS17_KS17_UCMAC_SHIFT            (13U)
/*! KS17_UCMAC - CMAC key */
#define ELS_ELS_KS17_KS17_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UCMAC_SHIFT)) & ELS_ELS_KS17_KS17_UCMAC_MASK)

#define ELS_ELS_KS17_KS17_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS17_KS17_UKSK_SHIFT             (14U)
/*! KS17_UKSK - KSK key */
#define ELS_ELS_KS17_KS17_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UKSK_SHIFT)) & ELS_ELS_KS17_KS17_UKSK_MASK)

#define ELS_ELS_KS17_KS17_URTF_MASK              (0x8000U)
#define ELS_ELS_KS17_KS17_URTF_SHIFT             (15U)
/*! KS17_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS17_KS17_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_URTF_SHIFT)) & ELS_ELS_KS17_KS17_URTF_MASK)

#define ELS_ELS_KS17_KS17_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS17_KS17_UCKDF_SHIFT            (16U)
/*! KS17_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS17_KS17_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UCKDF_SHIFT)) & ELS_ELS_KS17_KS17_UCKDF_MASK)

#define ELS_ELS_KS17_KS17_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS17_KS17_UHKDF_SHIFT            (17U)
/*! KS17_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS17_KS17_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UHKDF_SHIFT)) & ELS_ELS_KS17_KS17_UHKDF_MASK)

#define ELS_ELS_KS17_KS17_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS17_KS17_UECSG_SHIFT            (18U)
/*! KS17_UECSG - Ecc signing key */
#define ELS_ELS_KS17_KS17_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UECSG_SHIFT)) & ELS_ELS_KS17_KS17_UECSG_MASK)

#define ELS_ELS_KS17_KS17_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS17_KS17_UECDH_SHIFT            (19U)
/*! KS17_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS17_KS17_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UECDH_SHIFT)) & ELS_ELS_KS17_KS17_UECDH_MASK)

#define ELS_ELS_KS17_KS17_UAES_MASK              (0x100000U)
#define ELS_ELS_KS17_KS17_UAES_SHIFT             (20U)
/*! KS17_UAES - Aes key */
#define ELS_ELS_KS17_KS17_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UAES_SHIFT)) & ELS_ELS_KS17_KS17_UAES_MASK)

#define ELS_ELS_KS17_KS17_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS17_KS17_UHMAC_SHIFT            (21U)
/*! KS17_UHMAC - Hmac key */
#define ELS_ELS_KS17_KS17_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UHMAC_SHIFT)) & ELS_ELS_KS17_KS17_UHMAC_MASK)

#define ELS_ELS_KS17_KS17_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS17_KS17_UKWK_SHIFT             (22U)
/*! KS17_UKWK - Key wrapping key */
#define ELS_ELS_KS17_KS17_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UKWK_SHIFT)) & ELS_ELS_KS17_KS17_UKWK_MASK)

#define ELS_ELS_KS17_KS17_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS17_KS17_UKUOK_SHIFT            (23U)
/*! KS17_UKUOK - Key unwrapping key */
#define ELS_ELS_KS17_KS17_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UKUOK_SHIFT)) & ELS_ELS_KS17_KS17_UKUOK_MASK)

#define ELS_ELS_KS17_KS17_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS17_KS17_UTLSPMS_SHIFT          (24U)
/*! KS17_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS17_KS17_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UTLSPMS_SHIFT)) & ELS_ELS_KS17_KS17_UTLSPMS_MASK)

#define ELS_ELS_KS17_KS17_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS17_KS17_UTLSMS_SHIFT           (25U)
/*! KS17_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS17_KS17_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UTLSMS_SHIFT)) & ELS_ELS_KS17_KS17_UTLSMS_MASK)

#define ELS_ELS_KS17_KS17_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS17_KS17_UKGSRC_SHIFT           (26U)
/*! KS17_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS17_KS17_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UKGSRC_SHIFT)) & ELS_ELS_KS17_KS17_UKGSRC_MASK)

#define ELS_ELS_KS17_KS17_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS17_KS17_UHWO_SHIFT             (27U)
/*! KS17_UHWO - Hardware out key */
#define ELS_ELS_KS17_KS17_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UHWO_SHIFT)) & ELS_ELS_KS17_KS17_UHWO_MASK)

#define ELS_ELS_KS17_KS17_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS17_KS17_UWRPOK_SHIFT           (28U)
/*! KS17_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS17_KS17_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UWRPOK_SHIFT)) & ELS_ELS_KS17_KS17_UWRPOK_MASK)

#define ELS_ELS_KS17_KS17_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS17_KS17_UDUK_SHIFT             (29U)
/*! KS17_UDUK - Device Unique Key */
#define ELS_ELS_KS17_KS17_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UDUK_SHIFT)) & ELS_ELS_KS17_KS17_UDUK_MASK)

#define ELS_ELS_KS17_KS17_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS17_KS17_UPPROT_SHIFT           (30U)
/*! KS17_UPPROT - Priviledge level */
#define ELS_ELS_KS17_KS17_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS17_KS17_UPPROT_SHIFT)) & ELS_ELS_KS17_KS17_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS18 - Status register */
/*! @{ */

#define ELS_ELS_KS18_KS18_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS18_KS18_KSIZE_SHIFT            (0U)
/*! KS18_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS18_KS18_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_KSIZE_SHIFT)) & ELS_ELS_KS18_KS18_KSIZE_MASK)

#define ELS_ELS_KS18_KS18_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS18_KS18_RSVD0_SHIFT            (1U)
/*! KS18_RSVD0 - Reserved 0 */
#define ELS_ELS_KS18_KS18_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_RSVD0_SHIFT)) & ELS_ELS_KS18_KS18_RSVD0_MASK)

#define ELS_ELS_KS18_KS18_KACT_MASK              (0x20U)
#define ELS_ELS_KS18_KS18_KACT_SHIFT             (5U)
/*! KS18_KACT - Key is active */
#define ELS_ELS_KS18_KS18_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_KACT_SHIFT)) & ELS_ELS_KS18_KS18_KACT_MASK)

#define ELS_ELS_KS18_KS18_KBASE_MASK             (0x40U)
#define ELS_ELS_KS18_KS18_KBASE_SHIFT            (6U)
/*! KS18_KBASE - First slot in a multislot key */
#define ELS_ELS_KS18_KS18_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_KBASE_SHIFT)) & ELS_ELS_KS18_KS18_KBASE_MASK)

#define ELS_ELS_KS18_KS18_FGP_MASK               (0x80U)
#define ELS_ELS_KS18_KS18_FGP_SHIFT              (7U)
/*! KS18_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS18_KS18_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_FGP_SHIFT)) & ELS_ELS_KS18_KS18_FGP_MASK)

#define ELS_ELS_KS18_KS18_FRTN_MASK              (0x100U)
#define ELS_ELS_KS18_KS18_FRTN_SHIFT             (8U)
/*! KS18_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS18_KS18_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_FRTN_SHIFT)) & ELS_ELS_KS18_KS18_FRTN_MASK)

#define ELS_ELS_KS18_KS18_FHWO_MASK              (0x200U)
#define ELS_ELS_KS18_KS18_FHWO_SHIFT             (9U)
/*! KS18_FHWO - Hardware Feature Output */
#define ELS_ELS_KS18_KS18_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_FHWO_SHIFT)) & ELS_ELS_KS18_KS18_FHWO_MASK)

#define ELS_ELS_KS18_KS18_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS18_KS18_RSVD1_SHIFT            (10U)
/*! KS18_RSVD1 - Reserved 1 */
#define ELS_ELS_KS18_KS18_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_RSVD1_SHIFT)) & ELS_ELS_KS18_KS18_RSVD1_MASK)

#define ELS_ELS_KS18_KS18_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS18_KS18_UCMAC_SHIFT            (13U)
/*! KS18_UCMAC - CMAC key */
#define ELS_ELS_KS18_KS18_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UCMAC_SHIFT)) & ELS_ELS_KS18_KS18_UCMAC_MASK)

#define ELS_ELS_KS18_KS18_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS18_KS18_UKSK_SHIFT             (14U)
/*! KS18_UKSK - KSK key */
#define ELS_ELS_KS18_KS18_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UKSK_SHIFT)) & ELS_ELS_KS18_KS18_UKSK_MASK)

#define ELS_ELS_KS18_KS18_URTF_MASK              (0x8000U)
#define ELS_ELS_KS18_KS18_URTF_SHIFT             (15U)
/*! KS18_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS18_KS18_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_URTF_SHIFT)) & ELS_ELS_KS18_KS18_URTF_MASK)

#define ELS_ELS_KS18_KS18_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS18_KS18_UCKDF_SHIFT            (16U)
/*! KS18_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS18_KS18_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UCKDF_SHIFT)) & ELS_ELS_KS18_KS18_UCKDF_MASK)

#define ELS_ELS_KS18_KS18_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS18_KS18_UHKDF_SHIFT            (17U)
/*! KS18_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS18_KS18_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UHKDF_SHIFT)) & ELS_ELS_KS18_KS18_UHKDF_MASK)

#define ELS_ELS_KS18_KS18_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS18_KS18_UECSG_SHIFT            (18U)
/*! KS18_UECSG - Ecc signing key */
#define ELS_ELS_KS18_KS18_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UECSG_SHIFT)) & ELS_ELS_KS18_KS18_UECSG_MASK)

#define ELS_ELS_KS18_KS18_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS18_KS18_UECDH_SHIFT            (19U)
/*! KS18_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS18_KS18_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UECDH_SHIFT)) & ELS_ELS_KS18_KS18_UECDH_MASK)

#define ELS_ELS_KS18_KS18_UAES_MASK              (0x100000U)
#define ELS_ELS_KS18_KS18_UAES_SHIFT             (20U)
/*! KS18_UAES - Aes key */
#define ELS_ELS_KS18_KS18_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UAES_SHIFT)) & ELS_ELS_KS18_KS18_UAES_MASK)

#define ELS_ELS_KS18_KS18_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS18_KS18_UHMAC_SHIFT            (21U)
/*! KS18_UHMAC - Hmac key */
#define ELS_ELS_KS18_KS18_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UHMAC_SHIFT)) & ELS_ELS_KS18_KS18_UHMAC_MASK)

#define ELS_ELS_KS18_KS18_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS18_KS18_UKWK_SHIFT             (22U)
/*! KS18_UKWK - Key wrapping key */
#define ELS_ELS_KS18_KS18_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UKWK_SHIFT)) & ELS_ELS_KS18_KS18_UKWK_MASK)

#define ELS_ELS_KS18_KS18_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS18_KS18_UKUOK_SHIFT            (23U)
/*! KS18_UKUOK - Key unwrapping key */
#define ELS_ELS_KS18_KS18_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UKUOK_SHIFT)) & ELS_ELS_KS18_KS18_UKUOK_MASK)

#define ELS_ELS_KS18_KS18_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS18_KS18_UTLSPMS_SHIFT          (24U)
/*! KS18_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS18_KS18_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UTLSPMS_SHIFT)) & ELS_ELS_KS18_KS18_UTLSPMS_MASK)

#define ELS_ELS_KS18_KS18_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS18_KS18_UTLSMS_SHIFT           (25U)
/*! KS18_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS18_KS18_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UTLSMS_SHIFT)) & ELS_ELS_KS18_KS18_UTLSMS_MASK)

#define ELS_ELS_KS18_KS18_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS18_KS18_UKGSRC_SHIFT           (26U)
/*! KS18_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS18_KS18_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UKGSRC_SHIFT)) & ELS_ELS_KS18_KS18_UKGSRC_MASK)

#define ELS_ELS_KS18_KS18_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS18_KS18_UHWO_SHIFT             (27U)
/*! KS18_UHWO - Hardware out key */
#define ELS_ELS_KS18_KS18_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UHWO_SHIFT)) & ELS_ELS_KS18_KS18_UHWO_MASK)

#define ELS_ELS_KS18_KS18_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS18_KS18_UWRPOK_SHIFT           (28U)
/*! KS18_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS18_KS18_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UWRPOK_SHIFT)) & ELS_ELS_KS18_KS18_UWRPOK_MASK)

#define ELS_ELS_KS18_KS18_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS18_KS18_UDUK_SHIFT             (29U)
/*! KS18_UDUK - Device Unique Key */
#define ELS_ELS_KS18_KS18_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UDUK_SHIFT)) & ELS_ELS_KS18_KS18_UDUK_MASK)

#define ELS_ELS_KS18_KS18_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS18_KS18_UPPROT_SHIFT           (30U)
/*! KS18_UPPROT - Priviledge level */
#define ELS_ELS_KS18_KS18_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS18_KS18_UPPROT_SHIFT)) & ELS_ELS_KS18_KS18_UPPROT_MASK)
/*! @} */

/*! @name ELS_KS19 - Status register */
/*! @{ */

#define ELS_ELS_KS19_KS19_KSIZE_MASK             (0x1U)
#define ELS_ELS_KS19_KS19_KSIZE_SHIFT            (0U)
/*! KS19_KSIZE - Key size: 0-128, 1-256 */
#define ELS_ELS_KS19_KS19_KSIZE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_KSIZE_SHIFT)) & ELS_ELS_KS19_KS19_KSIZE_MASK)

#define ELS_ELS_KS19_KS19_RSVD0_MASK             (0x1EU)
#define ELS_ELS_KS19_KS19_RSVD0_SHIFT            (1U)
/*! KS19_RSVD0 - Reserved 0 */
#define ELS_ELS_KS19_KS19_RSVD0(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_RSVD0_SHIFT)) & ELS_ELS_KS19_KS19_RSVD0_MASK)

#define ELS_ELS_KS19_KS19_KACT_MASK              (0x20U)
#define ELS_ELS_KS19_KS19_KACT_SHIFT             (5U)
/*! KS19_KACT - Key is active */
#define ELS_ELS_KS19_KS19_KACT(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_KACT_SHIFT)) & ELS_ELS_KS19_KS19_KACT_MASK)

#define ELS_ELS_KS19_KS19_KBASE_MASK             (0x40U)
#define ELS_ELS_KS19_KS19_KBASE_SHIFT            (6U)
/*! KS19_KBASE - First slot in a multislot key */
#define ELS_ELS_KS19_KS19_KBASE(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_KBASE_SHIFT)) & ELS_ELS_KS19_KS19_KBASE_MASK)

#define ELS_ELS_KS19_KS19_FGP_MASK               (0x80U)
#define ELS_ELS_KS19_KS19_FGP_SHIFT              (7U)
/*! KS19_FGP - Hardware Feature General Purpose */
#define ELS_ELS_KS19_KS19_FGP(x)                 (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_FGP_SHIFT)) & ELS_ELS_KS19_KS19_FGP_MASK)

#define ELS_ELS_KS19_KS19_FRTN_MASK              (0x100U)
#define ELS_ELS_KS19_KS19_FRTN_SHIFT             (8U)
/*! KS19_FRTN - Hardware Feature Retention */
#define ELS_ELS_KS19_KS19_FRTN(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_FRTN_SHIFT)) & ELS_ELS_KS19_KS19_FRTN_MASK)

#define ELS_ELS_KS19_KS19_FHWO_MASK              (0x200U)
#define ELS_ELS_KS19_KS19_FHWO_SHIFT             (9U)
/*! KS19_FHWO - Hardware Feature Output */
#define ELS_ELS_KS19_KS19_FHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_FHWO_SHIFT)) & ELS_ELS_KS19_KS19_FHWO_MASK)

#define ELS_ELS_KS19_KS19_RSVD1_MASK             (0x1C00U)
#define ELS_ELS_KS19_KS19_RSVD1_SHIFT            (10U)
/*! KS19_RSVD1 - Reserved 1 */
#define ELS_ELS_KS19_KS19_RSVD1(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_RSVD1_SHIFT)) & ELS_ELS_KS19_KS19_RSVD1_MASK)

#define ELS_ELS_KS19_KS19_UCMAC_MASK             (0x2000U)
#define ELS_ELS_KS19_KS19_UCMAC_SHIFT            (13U)
/*! KS19_UCMAC - CMAC key */
#define ELS_ELS_KS19_KS19_UCMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UCMAC_SHIFT)) & ELS_ELS_KS19_KS19_UCMAC_MASK)

#define ELS_ELS_KS19_KS19_UKSK_MASK              (0x4000U)
#define ELS_ELS_KS19_KS19_UKSK_SHIFT             (14U)
/*! KS19_UKSK - KSK key */
#define ELS_ELS_KS19_KS19_UKSK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UKSK_SHIFT)) & ELS_ELS_KS19_KS19_UKSK_MASK)

#define ELS_ELS_KS19_KS19_URTF_MASK              (0x8000U)
#define ELS_ELS_KS19_KS19_URTF_SHIFT             (15U)
/*! KS19_URTF - Real Time Fingerprint key */
#define ELS_ELS_KS19_KS19_URTF(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_URTF_SHIFT)) & ELS_ELS_KS19_KS19_URTF_MASK)

#define ELS_ELS_KS19_KS19_UCKDF_MASK             (0x10000U)
#define ELS_ELS_KS19_KS19_UCKDF_SHIFT            (16U)
/*! KS19_UCKDF - Derivation key for CKDF command */
#define ELS_ELS_KS19_KS19_UCKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UCKDF_SHIFT)) & ELS_ELS_KS19_KS19_UCKDF_MASK)

#define ELS_ELS_KS19_KS19_UHKDF_MASK             (0x20000U)
#define ELS_ELS_KS19_KS19_UHKDF_SHIFT            (17U)
/*! KS19_UHKDF - Derivation key for HKDF command */
#define ELS_ELS_KS19_KS19_UHKDF(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UHKDF_SHIFT)) & ELS_ELS_KS19_KS19_UHKDF_MASK)

#define ELS_ELS_KS19_KS19_UECSG_MASK             (0x40000U)
#define ELS_ELS_KS19_KS19_UECSG_SHIFT            (18U)
/*! KS19_UECSG - Ecc signing key */
#define ELS_ELS_KS19_KS19_UECSG(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UECSG_SHIFT)) & ELS_ELS_KS19_KS19_UECSG_MASK)

#define ELS_ELS_KS19_KS19_UECDH_MASK             (0x80000U)
#define ELS_ELS_KS19_KS19_UECDH_SHIFT            (19U)
/*! KS19_UECDH - Ecc diffie hellman key */
#define ELS_ELS_KS19_KS19_UECDH(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UECDH_SHIFT)) & ELS_ELS_KS19_KS19_UECDH_MASK)

#define ELS_ELS_KS19_KS19_UAES_MASK              (0x100000U)
#define ELS_ELS_KS19_KS19_UAES_SHIFT             (20U)
/*! KS19_UAES - Aes key */
#define ELS_ELS_KS19_KS19_UAES(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UAES_SHIFT)) & ELS_ELS_KS19_KS19_UAES_MASK)

#define ELS_ELS_KS19_KS19_UHMAC_MASK             (0x200000U)
#define ELS_ELS_KS19_KS19_UHMAC_SHIFT            (21U)
/*! KS19_UHMAC - Hmac key */
#define ELS_ELS_KS19_KS19_UHMAC(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UHMAC_SHIFT)) & ELS_ELS_KS19_KS19_UHMAC_MASK)

#define ELS_ELS_KS19_KS19_UKWK_MASK              (0x400000U)
#define ELS_ELS_KS19_KS19_UKWK_SHIFT             (22U)
/*! KS19_UKWK - Key wrapping key */
#define ELS_ELS_KS19_KS19_UKWK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UKWK_SHIFT)) & ELS_ELS_KS19_KS19_UKWK_MASK)

#define ELS_ELS_KS19_KS19_UKUOK_MASK             (0x800000U)
#define ELS_ELS_KS19_KS19_UKUOK_SHIFT            (23U)
/*! KS19_UKUOK - Key unwrapping key */
#define ELS_ELS_KS19_KS19_UKUOK(x)               (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UKUOK_SHIFT)) & ELS_ELS_KS19_KS19_UKUOK_MASK)

#define ELS_ELS_KS19_KS19_UTLSPMS_MASK           (0x1000000U)
#define ELS_ELS_KS19_KS19_UTLSPMS_SHIFT          (24U)
/*! KS19_UTLSPMS - TLS Pre Master Secret */
#define ELS_ELS_KS19_KS19_UTLSPMS(x)             (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UTLSPMS_SHIFT)) & ELS_ELS_KS19_KS19_UTLSPMS_MASK)

#define ELS_ELS_KS19_KS19_UTLSMS_MASK            (0x2000000U)
#define ELS_ELS_KS19_KS19_UTLSMS_SHIFT           (25U)
/*! KS19_UTLSMS - TLS Master Secret */
#define ELS_ELS_KS19_KS19_UTLSMS(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UTLSMS_SHIFT)) & ELS_ELS_KS19_KS19_UTLSMS_MASK)

#define ELS_ELS_KS19_KS19_UKGSRC_MASK            (0x4000000U)
#define ELS_ELS_KS19_KS19_UKGSRC_SHIFT           (26U)
/*! KS19_UKGSRC - Supply KEYGEN source */
#define ELS_ELS_KS19_KS19_UKGSRC(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UKGSRC_SHIFT)) & ELS_ELS_KS19_KS19_UKGSRC_MASK)

#define ELS_ELS_KS19_KS19_UHWO_MASK              (0x8000000U)
#define ELS_ELS_KS19_KS19_UHWO_SHIFT             (27U)
/*! KS19_UHWO - Hardware out key */
#define ELS_ELS_KS19_KS19_UHWO(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UHWO_SHIFT)) & ELS_ELS_KS19_KS19_UHWO_MASK)

#define ELS_ELS_KS19_KS19_UWRPOK_MASK            (0x10000000U)
#define ELS_ELS_KS19_KS19_UWRPOK_SHIFT           (28U)
/*! KS19_UWRPOK - Ok to wrap key */
#define ELS_ELS_KS19_KS19_UWRPOK(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UWRPOK_SHIFT)) & ELS_ELS_KS19_KS19_UWRPOK_MASK)

#define ELS_ELS_KS19_KS19_UDUK_MASK              (0x20000000U)
#define ELS_ELS_KS19_KS19_UDUK_SHIFT             (29U)
/*! KS19_UDUK - Device Unique Key */
#define ELS_ELS_KS19_KS19_UDUK(x)                (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UDUK_SHIFT)) & ELS_ELS_KS19_KS19_UDUK_MASK)

#define ELS_ELS_KS19_KS19_UPPROT_MASK            (0xC0000000U)
#define ELS_ELS_KS19_KS19_UPPROT_SHIFT           (30U)
/*! KS19_UPPROT - Priviledge level */
#define ELS_ELS_KS19_KS19_UPPROT(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_KS19_KS19_UPPROT_SHIFT)) & ELS_ELS_KS19_KS19_UPPROT_MASK)
/*! @} */

/*! @name ELS_BOOT_ADDR - SW control for the ELS boot addr */
/*! @{ */

#define ELS_ELS_BOOT_ADDR_BOOT_ADDR_MASK         (0xFFFFFFFFU)
#define ELS_ELS_BOOT_ADDR_BOOT_ADDR_SHIFT        (0U)
/*! BOOT_ADDR - 32-bit wide boot offset */
#define ELS_ELS_BOOT_ADDR_BOOT_ADDR(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_BOOT_ADDR_BOOT_ADDR_SHIFT)) & ELS_ELS_BOOT_ADDR_BOOT_ADDR_MASK)
/*! @} */

/*! @name ELS_DBG_CFG - ELS Debug Config SFR */
/*! @{ */

#define ELS_ELS_DBG_CFG_DBG_CFG0_MASK            (0x7U)
#define ELS_ELS_DBG_CFG_DBG_CFG0_SHIFT           (0U)
/*! DBG_CFG0 - Debug Config 0 */
#define ELS_ELS_DBG_CFG_DBG_CFG0(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DBG_CFG_DBG_CFG0_SHIFT)) & ELS_ELS_DBG_CFG_DBG_CFG0_MASK)

#define ELS_ELS_DBG_CFG_DBG_CFG1_MASK            (0x38U)
#define ELS_ELS_DBG_CFG_DBG_CFG1_SHIFT           (3U)
/*! DBG_CFG1 - Debug Config 1 */
#define ELS_ELS_DBG_CFG_DBG_CFG1(x)              (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DBG_CFG_DBG_CFG1_SHIFT)) & ELS_ELS_DBG_CFG_DBG_CFG1_MASK)

#define ELS_ELS_DBG_CFG_DBG_CFG_RFU_MASK         (0xFFFFFFC0U)
#define ELS_ELS_DBG_CFG_DBG_CFG_RFU_SHIFT        (6U)
/*! DBG_CFG_RFU - reserved */
#define ELS_ELS_DBG_CFG_DBG_CFG_RFU(x)           (((uint32_t)(((uint32_t)(x)) << ELS_ELS_DBG_CFG_DBG_CFG_RFU_SHIFT)) & ELS_ELS_DBG_CFG_DBG_CFG_RFU_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group ELS_Register_Masks */


/*!
 * @}
 */ /* end of group ELS_Peripheral_Access_Layer */


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


#endif  /* ELS_H_ */

