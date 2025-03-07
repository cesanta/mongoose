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
**         CMSIS Peripheral Access Layer for PUF
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
 * @file PUF.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for PUF
 *
 * CMSIS Peripheral Access Layer for PUF
 */

#if !defined(PUF_H_)
#define PUF_H_                                   /**< Symbol preventing repeated inclusion */

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
   -- PUF Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PUF_Peripheral_Access_Layer PUF Peripheral Access Layer
 * @{
 */

/** PUF - Register Layout Typedef */
typedef struct {
  __IO uint32_t MODE;                              /**< Mode register, offset: 0x0 */
  __IO uint32_t CTRL;                              /**< Control register, offset: 0x4 */
  __I  uint32_t STATUS;                            /**< Status register, offset: 0x8 */
  __I  uint32_t VERSION;                           /**< Version register, offset: 0xC */
  __IO uint32_t CKSUM;                             /**< Checksum register, offset: 0x10 */
       uint8_t RESERVED_0[12];
  __IO uint32_t PARITY_0;                          /**< Parity register, offset: 0x20 */
  __IO uint32_t PARITY_1;                          /**< Parity register, offset: 0x24 */
  __IO uint32_t PARITY_2;                          /**< Parity register, offset: 0x28 */
       uint8_t RESERVED_1[20];
  __IO uint32_t IGNORE;                            /**< Ignore register, offset: 0x40 */
       uint8_t RESERVED_2[12];
  __IO uint32_t RNG;                               /**< Random Number register, offset: 0x50 */
       uint8_t RESERVED_3[12];
  __I  uint32_t KEY_0;                             /**< Key register, offset: 0x60 */
  __I  uint32_t KEY_1;                             /**< Key register, offset: 0x64 */
       uint8_t RESERVED_4[8];
  __IO uint32_t LOCK;                              /**< Lock register, offset: 0x70 */
  __I  uint32_t RO_FREQ;                           /**< RO Frequency register, offset: 0x74 */
  __I  uint32_t SLW_RO;                            /**< Slow RO register, offset: 0x78 */
       uint8_t RESERVED_5[4];
  __IO uint32_t EVAL_SEL;                          /**< Evaluation Select register, offset: 0x80 */
  __I  uint32_t EVAL_VAL;                          /**< Evaluation Value register, offset: 0x84 */
  __I  uint32_t EVAL_RAW;                          /**< Evaluation Raw register, offset: 0x88 */
  __I  uint32_t EVAL_BCH;                          /**< Evaluation BCH register, offset: 0x8C */
  __I  uint32_t EVAL_ERR_LOC_0_1;                  /**< Error Location register, offset: 0x90 */
  __I  uint32_t EVAL_ERR_LOC_2_3;                  /**< Error Location register, offset: 0x94 */
  __I  uint32_t EVAL_ERR_LOC_4_5;                  /**< Error Location register, offset: 0x98 */
  __I  uint32_t EVAL_ERR_LOC_6_7;                  /**< Error Location register, offset: 0x9C */
  __I  uint32_t EVAL_ERR_LOC_8_9;                  /**< Error Location register, offset: 0xA0 */
       uint8_t RESERVED_6[76];
  __I  uint32_t EVAL_RND_KEY_0;                    /**< Evaluation Round Key register, offset: 0xF0 */
  __I  uint32_t EVAL_RND_KEY_1;                    /**< Evaluation Round Key register, offset: 0xF4 */
       uint8_t RESERVED_7[4];
  __IO uint32_t EVAL_CTRL;                         /**< Evaluation Control register, offset: 0xFC */
       uint8_t RESERVED_8[3776];
  __I  uint32_t ACCESS_ERR;                        /**< Access Error register, offset: 0xFC0 */
  __IO uint32_t ACCESS_ERR_CLR;                    /**< Access Error Clear register, offset: 0xFC4 */
       uint8_t RESERVED_9[24];
  __I  uint32_t INT_STATUS;                        /**< Interrupt Status register, offset: 0xFE0 */
  __IO uint32_t INT_ENABLE;                        /**< Interrupt Enable register, offset: 0xFE4 */
  __IO uint32_t INT_STATUS_CLR;                    /**< Interrupt Status Clear register, offset: 0xFE8 */
  __IO uint32_t INT_STATUS_SET;                    /**< Interrupt Status Set register, offset: 0xFEC */
       uint8_t RESERVED_10[12];
  __I  uint32_t MODULE_ID;                         /**< Module ID register, offset: 0xFFC */
} PUF_Type;

/* ----------------------------------------------------------------------------
   -- PUF Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PUF_Register_Masks PUF Register Masks
 * @{
 */

/*! @name MODE - Mode register */
/*! @{ */

#define PUF_MODE_START_MASK                      (0x1U)
#define PUF_MODE_START_SHIFT                     (0U)
/*! START - Start */
#define PUF_MODE_START(x)                        (((uint32_t)(((uint32_t)(x)) << PUF_MODE_START_SHIFT)) & PUF_MODE_START_MASK)

#define PUF_MODE_ENROLL_MASK                     (0x2U)
#define PUF_MODE_ENROLL_SHIFT                    (1U)
/*! ENROLL - Enrollment; 1'b0 - Reconstruction; 1'b1 - Enrollment */
#define PUF_MODE_ENROLL(x)                       (((uint32_t)(((uint32_t)(x)) << PUF_MODE_ENROLL_SHIFT)) & PUF_MODE_ENROLL_MASK)

#define PUF_MODE_MODE_RSVD_3_MASK                (0xCU)
#define PUF_MODE_MODE_RSVD_3_SHIFT               (2U)
/*! MODE_RSVD_3 - Reserved */
#define PUF_MODE_MODE_RSVD_3(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_MODE_MODE_RSVD_3_SHIFT)) & PUF_MODE_MODE_RSVD_3_MASK)

#define PUF_MODE_SLW_LMT_MASK                    (0xF0U)
#define PUF_MODE_SLW_LMT_SHIFT                   (4U)
/*! SLW_LMT - Power of 2 count limit for slow limit */
#define PUF_MODE_SLW_LMT(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_MODE_SLW_LMT_SHIFT)) & PUF_MODE_SLW_LMT_MASK)

#define PUF_MODE_MODE_RSVD_2_MASK                (0xF00U)
#define PUF_MODE_MODE_RSVD_2_SHIFT               (8U)
/*! MODE_RSVD_2 - Reserved */
#define PUF_MODE_MODE_RSVD_2(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_MODE_MODE_RSVD_2_SHIFT)) & PUF_MODE_MODE_RSVD_2_MASK)

#define PUF_MODE_WRM_LMT_MASK                    (0xF000U)
#define PUF_MODE_WRM_LMT_SHIFT                   (12U)
/*! WRM_LMT - Power of 2 count limit for warmup */
#define PUF_MODE_WRM_LMT(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_MODE_WRM_LMT_SHIFT)) & PUF_MODE_WRM_LMT_MASK)

#define PUF_MODE_MODE_RSVD_1_MASK                (0xF0000U)
#define PUF_MODE_MODE_RSVD_1_SHIFT               (16U)
/*! MODE_RSVD_1 - Reserved */
#define PUF_MODE_MODE_RSVD_1(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_MODE_MODE_RSVD_1_SHIFT)) & PUF_MODE_MODE_RSVD_1_MASK)

#define PUF_MODE_REC_LMT_MASK                    (0xF00000U)
#define PUF_MODE_REC_LMT_SHIFT                   (20U)
/*! REC_LMT - Power of 2 count limit for reconstruction */
#define PUF_MODE_REC_LMT(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_MODE_REC_LMT_SHIFT)) & PUF_MODE_REC_LMT_MASK)

#define PUF_MODE_MODE_RSVD_0_MASK                (0xF000000U)
#define PUF_MODE_MODE_RSVD_0_SHIFT               (24U)
/*! MODE_RSVD_0 - Reserved */
#define PUF_MODE_MODE_RSVD_0(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_MODE_MODE_RSVD_0_SHIFT)) & PUF_MODE_MODE_RSVD_0_MASK)

#define PUF_MODE_ENR_LMT_MASK                    (0xF0000000U)
#define PUF_MODE_ENR_LMT_SHIFT                   (28U)
/*! ENR_LMT - Power of 2 count limit for enrollment */
#define PUF_MODE_ENR_LMT(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_MODE_ENR_LMT_SHIFT)) & PUF_MODE_ENR_LMT_MASK)
/*! @} */

/*! @name CTRL - Control register */
/*! @{ */

#define PUF_CTRL_GEN_KEY_MASK                    (0x1U)
#define PUF_CTRL_GEN_KEY_SHIFT                   (0U)
/*! GEN_KEY - Generate Next Key; Request another key of a different KEY_ID */
#define PUF_CTRL_GEN_KEY(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_CTRL_GEN_KEY_SHIFT)) & PUF_CTRL_GEN_KEY_MASK)

#define PUF_CTRL_NEXT_CHUNK_MASK                 (0x2U)
#define PUF_CTRL_NEXT_CHUNK_SHIFT                (1U)
/*! NEXT_CHUNK - Next Key Chunk; Request next chunk of key with current key ID */
#define PUF_CTRL_NEXT_CHUNK(x)                   (((uint32_t)(((uint32_t)(x)) << PUF_CTRL_NEXT_CHUNK_SHIFT)) & PUF_CTRL_NEXT_CHUNK_MASK)

#define PUF_CTRL_CTRL_RSVD_1_MASK                (0xCU)
#define PUF_CTRL_CTRL_RSVD_1_SHIFT               (2U)
/*! CTRL_RSVD_1 - Reserved */
#define PUF_CTRL_CTRL_RSVD_1(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_CTRL_CTRL_RSVD_1_SHIFT)) & PUF_CTRL_CTRL_RSVD_1_MASK)

#define PUF_CTRL_KEY_ID_MASK                     (0xF0U)
#define PUF_CTRL_KEY_ID_SHIFT                    (4U)
/*! KEY_ID - Key ID; Upper nibble of the key generation seed. */
#define PUF_CTRL_KEY_ID(x)                       (((uint32_t)(((uint32_t)(x)) << PUF_CTRL_KEY_ID_SHIFT)) & PUF_CTRL_KEY_ID_MASK)

#define PUF_CTRL_CTRL_RSVD_0_MASK                (0x7FFFFF00U)
#define PUF_CTRL_CTRL_RSVD_0_SHIFT               (8U)
/*! CTRL_RSVD_0 - Reserved */
#define PUF_CTRL_CTRL_RSVD_0(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_CTRL_CTRL_RSVD_0_SHIFT)) & PUF_CTRL_CTRL_RSVD_0_MASK)

#define PUF_CTRL_PUF_RST_MASK                    (0x80000000U)
#define PUF_CTRL_PUF_RST_SHIFT                   (31U)
/*! PUF_RST - Synchronous Reset */
#define PUF_CTRL_PUF_RST(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_CTRL_PUF_RST_SHIFT)) & PUF_CTRL_PUF_RST_MASK)
/*! @} */

/*! @name STATUS - Status register */
/*! @{ */

#define PUF_STATUS_BUSY_MASK                     (0x1U)
#define PUF_STATUS_BUSY_SHIFT                    (0U)
/*! BUSY - PUF is busy */
#define PUF_STATUS_BUSY(x)                       (((uint32_t)(((uint32_t)(x)) << PUF_STATUS_BUSY_SHIFT)) & PUF_STATUS_BUSY_MASK)

#define PUF_STATUS_STATUS_RSVD_1_MASK            (0xFFFFFEU)
#define PUF_STATUS_STATUS_RSVD_1_SHIFT           (1U)
/*! STATUS_RSVD_1 - Reserved */
#define PUF_STATUS_STATUS_RSVD_1(x)              (((uint32_t)(((uint32_t)(x)) << PUF_STATUS_STATUS_RSVD_1_SHIFT)) & PUF_STATUS_STATUS_RSVD_1_MASK)

#define PUF_STATUS_ERROR_MASK                    (0xF000000U)
#define PUF_STATUS_ERROR_SHIFT                   (24U)
/*! ERROR - Error Code; 4'b0001 - Checksum mismatch; 4'b0010 - Chunk overflow, next chunk invalid;
 *    4'b0011 - Key ID requested is locked; 4'b0100 - Invalid counter limit; 4'b0101 -
 *    Enroll/reconstruction is locked; 4'b0110 - Next chunk requested before key requested; 4'b0111 - Invalid lock
 *    code
 */
#define PUF_STATUS_ERROR(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_STATUS_ERROR_SHIFT)) & PUF_STATUS_ERROR_MASK)

#define PUF_STATUS_STATUS_RSVD_0_MASK            (0xF0000000U)
#define PUF_STATUS_STATUS_RSVD_0_SHIFT           (28U)
/*! STATUS_RSVD_0 - Reserved */
#define PUF_STATUS_STATUS_RSVD_0(x)              (((uint32_t)(((uint32_t)(x)) << PUF_STATUS_STATUS_RSVD_0_SHIFT)) & PUF_STATUS_STATUS_RSVD_0_MASK)
/*! @} */

/*! @name VERSION - Version register */
/*! @{ */

#define PUF_VERSION_ENTROPY_MASK                 (0xFFU)
#define PUF_VERSION_ENTROPY_SHIFT                (0U)
/*! ENTROPY - Entropy Divided by Four */
#define PUF_VERSION_ENTROPY(x)                   (((uint32_t)(((uint32_t)(x)) << PUF_VERSION_ENTROPY_SHIFT)) & PUF_VERSION_ENTROPY_MASK)

#define PUF_VERSION_NUM_RO_GRP_MASK              (0xFF00U)
#define PUF_VERSION_NUM_RO_GRP_SHIFT             (8U)
/*! NUM_RO_GRP - Number of RO Groups */
#define PUF_VERSION_NUM_RO_GRP(x)                (((uint32_t)(((uint32_t)(x)) << PUF_VERSION_NUM_RO_GRP_SHIFT)) & PUF_VERSION_NUM_RO_GRP_MASK)

#define PUF_VERSION_MIN_VER_MASK                 (0xFF0000U)
#define PUF_VERSION_MIN_VER_SHIFT                (16U)
/*! MIN_VER - Minor Version */
#define PUF_VERSION_MIN_VER(x)                   (((uint32_t)(((uint32_t)(x)) << PUF_VERSION_MIN_VER_SHIFT)) & PUF_VERSION_MIN_VER_MASK)

#define PUF_VERSION_MAJ_VER_MASK                 (0xFF000000U)
#define PUF_VERSION_MAJ_VER_SHIFT                (24U)
/*! MAJ_VER - Major Version */
#define PUF_VERSION_MAJ_VER(x)                   (((uint32_t)(((uint32_t)(x)) << PUF_VERSION_MAJ_VER_SHIFT)) & PUF_VERSION_MAJ_VER_MASK)
/*! @} */

/*! @name CKSUM - Checksum register */
/*! @{ */

#define PUF_CKSUM_CKSUM_MASK                     (0xFFFFFFFFU)
#define PUF_CKSUM_CKSUM_SHIFT                    (0U)
/*! CKSUM - Checksum; Enrollment: read after operation to store computed checksum; Reconstruction:
 *    rxpected checksum to compare written before operation
 */
#define PUF_CKSUM_CKSUM(x)                       (((uint32_t)(((uint32_t)(x)) << PUF_CKSUM_CKSUM_SHIFT)) & PUF_CKSUM_CKSUM_MASK)
/*! @} */

/*! @name PARITY_0 - Parity register */
/*! @{ */

#define PUF_PARITY_0_PARITY_0_MASK               (0xFFFFFFFFU)
#define PUF_PARITY_0_PARITY_0_SHIFT              (0U)
/*! PARITY_0 - Bits [31:0] of the helper parity data */
#define PUF_PARITY_0_PARITY_0(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_PARITY_0_PARITY_0_SHIFT)) & PUF_PARITY_0_PARITY_0_MASK)
/*! @} */

/*! @name PARITY_1 - Parity register */
/*! @{ */

#define PUF_PARITY_1_PARITY_1_MASK               (0xFFFFFFFFU)
#define PUF_PARITY_1_PARITY_1_SHIFT              (0U)
/*! PARITY_1 - Bits [63:32] of the helper parity data */
#define PUF_PARITY_1_PARITY_1(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_PARITY_1_PARITY_1_SHIFT)) & PUF_PARITY_1_PARITY_1_MASK)
/*! @} */

/*! @name PARITY_2 - Parity register */
/*! @{ */

#define PUF_PARITY_2_PARITY_2_MASK               (0xFFFU)
#define PUF_PARITY_2_PARITY_2_SHIFT              (0U)
/*! PARITY_2 - Bits [75:64] of the helper parity data */
#define PUF_PARITY_2_PARITY_2(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_PARITY_2_PARITY_2_SHIFT)) & PUF_PARITY_2_PARITY_2_MASK)

#define PUF_PARITY_2_PAR_RSVD_MASK               (0xFFFFF000U)
#define PUF_PARITY_2_PAR_RSVD_SHIFT              (12U)
/*! PAR_RSVD - Reserved */
#define PUF_PARITY_2_PAR_RSVD(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_PARITY_2_PAR_RSVD_SHIFT)) & PUF_PARITY_2_PAR_RSVD_MASK)
/*! @} */

/*! @name IGNORE - Ignore register */
/*! @{ */

#define PUF_IGNORE_IGNORE_MASK                   (0xFFFFFFFFU)
#define PUF_IGNORE_IGNORE_SHIFT                  (0U)
/*! IGNORE - Ignore data; Enrollment: Read after operation to store computed ignore tags;
 *    Reconstruction: Written before operation to load ignore tags
 */
#define PUF_IGNORE_IGNORE(x)                     (((uint32_t)(((uint32_t)(x)) << PUF_IGNORE_IGNORE_SHIFT)) & PUF_IGNORE_IGNORE_MASK)
/*! @} */

/*! @name RNG - Random Number register */
/*! @{ */

#define PUF_RNG_RNG_MASK                         (0xFFFFFFFFU)
#define PUF_RNG_RNG_SHIFT                        (0U)
/*! RNG - Random bits used for masking during reconstruction */
#define PUF_RNG_RNG(x)                           (((uint32_t)(((uint32_t)(x)) << PUF_RNG_RNG_SHIFT)) & PUF_RNG_RNG_MASK)
/*! @} */

/*! @name KEY_0 - Key register */
/*! @{ */

#define PUF_KEY_0_KEY_0_MASK                     (0xFFFFFFFFU)
#define PUF_KEY_0_KEY_0_SHIFT                    (0U)
/*! KEY_0 - Bits [31:0] of generated key chunk */
#define PUF_KEY_0_KEY_0(x)                       (((uint32_t)(((uint32_t)(x)) << PUF_KEY_0_KEY_0_SHIFT)) & PUF_KEY_0_KEY_0_MASK)
/*! @} */

/*! @name KEY_1 - Key register */
/*! @{ */

#define PUF_KEY_1_KEY_1_MASK                     (0xFFFFFFFFU)
#define PUF_KEY_1_KEY_1_SHIFT                    (0U)
/*! KEY_1 - Bits [63:32] of generated key chunk */
#define PUF_KEY_1_KEY_1(x)                       (((uint32_t)(((uint32_t)(x)) << PUF_KEY_1_KEY_1_SHIFT)) & PUF_KEY_1_KEY_1_MASK)
/*! @} */

/*! @name LOCK - Lock register */
/*! @{ */

#define PUF_LOCK_KEY_ID_LCK_MASK                 (0xFFFFU)
#define PUF_LOCK_KEY_ID_LCK_SHIFT                (0U)
/*! KEY_ID_LCK - Key Lock; Each bit position represents the Key ID of a key; 1'b0 - Unlocked; 1'b1 - Locked */
#define PUF_LOCK_KEY_ID_LCK(x)                   (((uint32_t)(((uint32_t)(x)) << PUF_LOCK_KEY_ID_LCK_SHIFT)) & PUF_LOCK_KEY_ID_LCK_MASK)

#define PUF_LOCK_LOCK_RSVD_MASK                  (0xFF0000U)
#define PUF_LOCK_LOCK_RSVD_SHIFT                 (16U)
/*! LOCK_RSVD - Reserved */
#define PUF_LOCK_LOCK_RSVD(x)                    (((uint32_t)(((uint32_t)(x)) << PUF_LOCK_LOCK_RSVD_SHIFT)) & PUF_LOCK_LOCK_RSVD_MASK)

#define PUF_LOCK_REC_LCK_MASK                    (0xF000000U)
#define PUF_LOCK_REC_LCK_SHIFT                   (24U)
/*! REC_LCK - Reconstruction lock; 4'h5 - Locked; 4'hA - Unlocked */
#define PUF_LOCK_REC_LCK(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_LOCK_REC_LCK_SHIFT)) & PUF_LOCK_REC_LCK_MASK)

#define PUF_LOCK_ENR_LCK_MASK                    (0xF0000000U)
#define PUF_LOCK_ENR_LCK_SHIFT                   (28U)
/*! ENR_LCK - Enrollment lock; 4'h5 - Locked; 4'hA - Unlocked */
#define PUF_LOCK_ENR_LCK(x)                      (((uint32_t)(((uint32_t)(x)) << PUF_LOCK_ENR_LCK_SHIFT)) & PUF_LOCK_ENR_LCK_MASK)
/*! @} */

/*! @name RO_FREQ - RO Frequency register */
/*! @{ */

#define PUF_RO_FREQ_RO_FREQ_MASK                 (0xFFFFFFFFU)
#define PUF_RO_FREQ_RO_FREQ_SHIFT                (0U)
/*! RO_FREQ - System clock count it takes for the fastest RO of each group to reach the limit */
#define PUF_RO_FREQ_RO_FREQ(x)                   (((uint32_t)(((uint32_t)(x)) << PUF_RO_FREQ_RO_FREQ_SHIFT)) & PUF_RO_FREQ_RO_FREQ_MASK)
/*! @} */

/*! @name SLW_RO - Slow RO register */
/*! @{ */

#define PUF_SLW_RO_SLW_RO_MASK                   (0x1FU)
#define PUF_SLW_RO_SLW_RO_SHIFT                  (0U)
/*! SLW_RO - Number of slow ROs in the current group. */
#define PUF_SLW_RO_SLW_RO(x)                     (((uint32_t)(((uint32_t)(x)) << PUF_SLW_RO_SLW_RO_SHIFT)) & PUF_SLW_RO_SLW_RO_MASK)

#define PUF_SLW_RO_SLW_MAX_PER_GRP_MASK          (0x1F00U)
#define PUF_SLW_RO_SLW_MAX_PER_GRP_SHIFT         (8U)
/*! SLW_MAX_PER_GRP - Largest number of slow ROs detected in a single group */
#define PUF_SLW_RO_SLW_MAX_PER_GRP(x)            (((uint32_t)(((uint32_t)(x)) << PUF_SLW_RO_SLW_MAX_PER_GRP_SHIFT)) & PUF_SLW_RO_SLW_MAX_PER_GRP_MASK)

#define PUF_SLW_RO_SLW_TOTAL_MASK                (0x1FF0000U)
#define PUF_SLW_RO_SLW_TOTAL_SHIFT               (16U)
/*! SLW_TOTAL - Running total number of slow ROs */
#define PUF_SLW_RO_SLW_TOTAL(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_SLW_RO_SLW_TOTAL_SHIFT)) & PUF_SLW_RO_SLW_TOTAL_MASK)
/*! @} */

/*! @name EVAL_SEL - Evaluation Select register */
/*! @{ */

#define PUF_EVAL_SEL_E_RANK_SEL_MASK             (0x1FU)
#define PUF_EVAL_SEL_E_RANK_SEL_SHIFT            (0U)
/*! E_RANK_SEL - Select RO based its post-ranking position */
#define PUF_EVAL_SEL_E_RANK_SEL(x)               (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_SEL_E_RANK_SEL_SHIFT)) & PUF_EVAL_SEL_E_RANK_SEL_MASK)

#define PUF_EVAL_SEL_E_RSVD_2_MASK               (0xE0U)
#define PUF_EVAL_SEL_E_RSVD_2_SHIFT              (5U)
/*! E_RSVD_2 - Reserved */
#define PUF_EVAL_SEL_E_RSVD_2(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_SEL_E_RSVD_2_SHIFT)) & PUF_EVAL_SEL_E_RSVD_2_MASK)

#define PUF_EVAL_SEL_E_FREE_GRP_MASK             (0x300U)
#define PUF_EVAL_SEL_E_FREE_GRP_SHIFT            (8U)
/*! E_FREE_GRP - Free run mode group select */
#define PUF_EVAL_SEL_E_FREE_GRP(x)               (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_SEL_E_FREE_GRP_SHIFT)) & PUF_EVAL_SEL_E_FREE_GRP_MASK)

#define PUF_EVAL_SEL_E_RSVD_1_MASK               (0xC00U)
#define PUF_EVAL_SEL_E_RSVD_1_SHIFT              (10U)
/*! E_RSVD_1 - Reserved */
#define PUF_EVAL_SEL_E_RSVD_1(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_SEL_E_RSVD_1_SHIFT)) & PUF_EVAL_SEL_E_RSVD_1_MASK)

#define PUF_EVAL_SEL_E_FREE_SEC_MASK             (0x7000U)
#define PUF_EVAL_SEL_E_FREE_SEC_SHIFT            (12U)
/*! E_FREE_SEC - Free run mode section select */
#define PUF_EVAL_SEL_E_FREE_SEC(x)               (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_SEL_E_FREE_SEC_SHIFT)) & PUF_EVAL_SEL_E_FREE_SEC_MASK)

#define PUF_EVAL_SEL_E_RSVD_0_MASK               (0xFFFF8000U)
#define PUF_EVAL_SEL_E_RSVD_0_SHIFT              (15U)
/*! E_RSVD_0 - Reserved */
#define PUF_EVAL_SEL_E_RSVD_0(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_SEL_E_RSVD_0_SHIFT)) & PUF_EVAL_SEL_E_RSVD_0_MASK)
/*! @} */

/*! @name EVAL_VAL - Evaluation Value register */
/*! @{ */

#define PUF_EVAL_VAL_E_CNT_MASK                  (0xFFFFU)
#define PUF_EVAL_VAL_E_CNT_SHIFT                 (0U)
/*! E_CNT - Counter value of the selected RO in PUF_EVAL_SEL */
#define PUF_EVAL_VAL_E_CNT(x)                    (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_VAL_E_CNT_SHIFT)) & PUF_EVAL_VAL_E_CNT_MASK)

#define PUF_EVAL_VAL_E_RSVD_4_MASK               (0xFF0000U)
#define PUF_EVAL_VAL_E_RSVD_4_SHIFT              (16U)
/*! E_RSVD_4 - Reserved */
#define PUF_EVAL_VAL_E_RSVD_4(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_VAL_E_RSVD_4_SHIFT)) & PUF_EVAL_VAL_E_RSVD_4_MASK)

#define PUF_EVAL_VAL_E_RO_NUM_MASK               (0x1F000000U)
#define PUF_EVAL_VAL_E_RO_NUM_SHIFT              (24U)
/*! E_RO_NUM - Original position of the selected RO in PUF_EVAL_SEL */
#define PUF_EVAL_VAL_E_RO_NUM(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_VAL_E_RO_NUM_SHIFT)) & PUF_EVAL_VAL_E_RO_NUM_MASK)

#define PUF_EVAL_VAL_E_RSVD_3_MASK               (0xE0000000U)
#define PUF_EVAL_VAL_E_RSVD_3_SHIFT              (29U)
/*! E_RSVD_3 - Reserved */
#define PUF_EVAL_VAL_E_RSVD_3(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_VAL_E_RSVD_3_SHIFT)) & PUF_EVAL_VAL_E_RSVD_3_MASK)
/*! @} */

/*! @name EVAL_RAW - Evaluation Raw register */
/*! @{ */

#define PUF_EVAL_RAW_E_RAW_MASK                  (0xFFFU)
#define PUF_EVAL_RAW_E_RAW_SHIFT                 (0U)
/*! E_RAW - Raw value generated by the current group of ROs */
#define PUF_EVAL_RAW_E_RAW(x)                    (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_RAW_E_RAW_SHIFT)) & PUF_EVAL_RAW_E_RAW_MASK)

#define PUF_EVAL_RAW_E_RSVD_5_MASK               (0xFFFFF000U)
#define PUF_EVAL_RAW_E_RSVD_5_SHIFT              (12U)
/*! E_RSVD_5 - Reserved */
#define PUF_EVAL_RAW_E_RSVD_5(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_RAW_E_RSVD_5_SHIFT)) & PUF_EVAL_RAW_E_RSVD_5_MASK)
/*! @} */

/*! @name EVAL_BCH - Evaluation BCH register */
/*! @{ */

#define PUF_EVAL_BCH_E_BCH_ERR_MASK              (0xFU)
#define PUF_EVAL_BCH_E_BCH_ERR_SHIFT             (0U)
/*! E_BCH_ERR - Number of errors PUF detected */
#define PUF_EVAL_BCH_E_BCH_ERR(x)                (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_BCH_E_BCH_ERR_SHIFT)) & PUF_EVAL_BCH_E_BCH_ERR_MASK)

#define PUF_EVAL_BCH_E_RSVD_7_MASK               (0xF0U)
#define PUF_EVAL_BCH_E_RSVD_7_SHIFT              (4U)
/*! E_RSVD_7 - Reserved */
#define PUF_EVAL_BCH_E_RSVD_7(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_BCH_E_RSVD_7_SHIFT)) & PUF_EVAL_BCH_E_RSVD_7_MASK)

#define PUF_EVAL_BCH_E_ERR_LMT_MASK              (0xF00U)
#define PUF_EVAL_BCH_E_ERR_LMT_SHIFT             (8U)
/*! E_ERR_LMT - Number of errors PUF can detect */
#define PUF_EVAL_BCH_E_ERR_LMT(x)                (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_BCH_E_ERR_LMT_SHIFT)) & PUF_EVAL_BCH_E_ERR_LMT_MASK)

#define PUF_EVAL_BCH_E_RSVD_6_MASK               (0xFFFFF000U)
#define PUF_EVAL_BCH_E_RSVD_6_SHIFT              (12U)
/*! E_RSVD_6 - Reserved */
#define PUF_EVAL_BCH_E_RSVD_6(x)                 (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_BCH_E_RSVD_6_SHIFT)) & PUF_EVAL_BCH_E_RSVD_6_MASK)
/*! @} */

/*! @name EVAL_ERR_LOC_0_1 - Error Location register */
/*! @{ */

#define PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_0_MASK    (0xFFU)
#define PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_0_SHIFT   (0U)
/*! E_ERR_LOC_0 - Error location 0 */
#define PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_0(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_0_SHIFT)) & PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_0_MASK)

#define PUF_EVAL_ERR_LOC_0_1_E_RSVD_9_MASK       (0xFF00U)
#define PUF_EVAL_ERR_LOC_0_1_E_RSVD_9_SHIFT      (8U)
/*! E_RSVD_9 - Reserved */
#define PUF_EVAL_ERR_LOC_0_1_E_RSVD_9(x)         (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_0_1_E_RSVD_9_SHIFT)) & PUF_EVAL_ERR_LOC_0_1_E_RSVD_9_MASK)

#define PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_1_MASK    (0xFF0000U)
#define PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_1_SHIFT   (16U)
/*! E_ERR_LOC_1 - Error location 1 */
#define PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_1(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_1_SHIFT)) & PUF_EVAL_ERR_LOC_0_1_E_ERR_LOC_1_MASK)

#define PUF_EVAL_ERR_LOC_0_1_E_RSVD_8_MASK       (0xFF000000U)
#define PUF_EVAL_ERR_LOC_0_1_E_RSVD_8_SHIFT      (24U)
/*! E_RSVD_8 - Reserved */
#define PUF_EVAL_ERR_LOC_0_1_E_RSVD_8(x)         (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_0_1_E_RSVD_8_SHIFT)) & PUF_EVAL_ERR_LOC_0_1_E_RSVD_8_MASK)
/*! @} */

/*! @name EVAL_ERR_LOC_2_3 - Error Location register */
/*! @{ */

#define PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_2_MASK    (0xFFU)
#define PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_2_SHIFT   (0U)
/*! E_ERR_LOC_2 - Error location 2 */
#define PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_2(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_2_SHIFT)) & PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_2_MASK)

#define PUF_EVAL_ERR_LOC_2_3_E_RSVD_11_MASK      (0xFF00U)
#define PUF_EVAL_ERR_LOC_2_3_E_RSVD_11_SHIFT     (8U)
/*! E_RSVD_11 - Reserved */
#define PUF_EVAL_ERR_LOC_2_3_E_RSVD_11(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_2_3_E_RSVD_11_SHIFT)) & PUF_EVAL_ERR_LOC_2_3_E_RSVD_11_MASK)

#define PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_3_MASK    (0xFF0000U)
#define PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_3_SHIFT   (16U)
/*! E_ERR_LOC_3 - Error location 3 */
#define PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_3(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_3_SHIFT)) & PUF_EVAL_ERR_LOC_2_3_E_ERR_LOC_3_MASK)

#define PUF_EVAL_ERR_LOC_2_3_E_RSVD_10_MASK      (0xFF000000U)
#define PUF_EVAL_ERR_LOC_2_3_E_RSVD_10_SHIFT     (24U)
/*! E_RSVD_10 - Reserved */
#define PUF_EVAL_ERR_LOC_2_3_E_RSVD_10(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_2_3_E_RSVD_10_SHIFT)) & PUF_EVAL_ERR_LOC_2_3_E_RSVD_10_MASK)
/*! @} */

/*! @name EVAL_ERR_LOC_4_5 - Error Location register */
/*! @{ */

#define PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_4_MASK    (0xFFU)
#define PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_4_SHIFT   (0U)
/*! E_ERR_LOC_4 - Error location 4 */
#define PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_4(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_4_SHIFT)) & PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_4_MASK)

#define PUF_EVAL_ERR_LOC_4_5_E_RSVD_13_MASK      (0xFF00U)
#define PUF_EVAL_ERR_LOC_4_5_E_RSVD_13_SHIFT     (8U)
/*! E_RSVD_13 - Reserved */
#define PUF_EVAL_ERR_LOC_4_5_E_RSVD_13(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_4_5_E_RSVD_13_SHIFT)) & PUF_EVAL_ERR_LOC_4_5_E_RSVD_13_MASK)

#define PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_5_MASK    (0xFF0000U)
#define PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_5_SHIFT   (16U)
/*! E_ERR_LOC_5 - Error location 5 */
#define PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_5(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_5_SHIFT)) & PUF_EVAL_ERR_LOC_4_5_E_ERR_LOC_5_MASK)

#define PUF_EVAL_ERR_LOC_4_5_E_RSVD_12_MASK      (0xFF000000U)
#define PUF_EVAL_ERR_LOC_4_5_E_RSVD_12_SHIFT     (24U)
/*! E_RSVD_12 - Reserved */
#define PUF_EVAL_ERR_LOC_4_5_E_RSVD_12(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_4_5_E_RSVD_12_SHIFT)) & PUF_EVAL_ERR_LOC_4_5_E_RSVD_12_MASK)
/*! @} */

/*! @name EVAL_ERR_LOC_6_7 - Error Location register */
/*! @{ */

#define PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_6_MASK    (0xFFU)
#define PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_6_SHIFT   (0U)
/*! E_ERR_LOC_6 - Error location 6 */
#define PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_6(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_6_SHIFT)) & PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_6_MASK)

#define PUF_EVAL_ERR_LOC_6_7_E_RSVD_15_MASK      (0xFF00U)
#define PUF_EVAL_ERR_LOC_6_7_E_RSVD_15_SHIFT     (8U)
/*! E_RSVD_15 - Reserved */
#define PUF_EVAL_ERR_LOC_6_7_E_RSVD_15(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_6_7_E_RSVD_15_SHIFT)) & PUF_EVAL_ERR_LOC_6_7_E_RSVD_15_MASK)

#define PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_7_MASK    (0xFF0000U)
#define PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_7_SHIFT   (16U)
/*! E_ERR_LOC_7 - Error location 7 */
#define PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_7(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_7_SHIFT)) & PUF_EVAL_ERR_LOC_6_7_E_ERR_LOC_7_MASK)

#define PUF_EVAL_ERR_LOC_6_7_E_RSVD_14_MASK      (0xFF000000U)
#define PUF_EVAL_ERR_LOC_6_7_E_RSVD_14_SHIFT     (24U)
/*! E_RSVD_14 - Reserved */
#define PUF_EVAL_ERR_LOC_6_7_E_RSVD_14(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_6_7_E_RSVD_14_SHIFT)) & PUF_EVAL_ERR_LOC_6_7_E_RSVD_14_MASK)
/*! @} */

/*! @name EVAL_ERR_LOC_8_9 - Error Location register */
/*! @{ */

#define PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_8_MASK    (0xFFU)
#define PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_8_SHIFT   (0U)
/*! E_ERR_LOC_8 - Error location 8 */
#define PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_8(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_8_SHIFT)) & PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_8_MASK)

#define PUF_EVAL_ERR_LOC_8_9_E_RSVD_17_MASK      (0xFF00U)
#define PUF_EVAL_ERR_LOC_8_9_E_RSVD_17_SHIFT     (8U)
/*! E_RSVD_17 - Reserved */
#define PUF_EVAL_ERR_LOC_8_9_E_RSVD_17(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_8_9_E_RSVD_17_SHIFT)) & PUF_EVAL_ERR_LOC_8_9_E_RSVD_17_MASK)

#define PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_9_MASK    (0xFF0000U)
#define PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_9_SHIFT   (16U)
/*! E_ERR_LOC_9 - Error location 9 */
#define PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_9(x)      (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_9_SHIFT)) & PUF_EVAL_ERR_LOC_8_9_E_ERR_LOC_9_MASK)

#define PUF_EVAL_ERR_LOC_8_9_E_RSVD_16_MASK      (0xFF000000U)
#define PUF_EVAL_ERR_LOC_8_9_E_RSVD_16_SHIFT     (24U)
/*! E_RSVD_16 - Reserved */
#define PUF_EVAL_ERR_LOC_8_9_E_RSVD_16(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_ERR_LOC_8_9_E_RSVD_16_SHIFT)) & PUF_EVAL_ERR_LOC_8_9_E_RSVD_16_MASK)
/*! @} */

/*! @name EVAL_RND_KEY_0 - Evaluation Round Key register */
/*! @{ */

#define PUF_EVAL_RND_KEY_0_E_RND_KEY_0_MASK      (0xFFFFFFFFU)
#define PUF_EVAL_RND_KEY_0_E_RND_KEY_0_SHIFT     (0U)
/*! E_RND_KEY_0 - Bits [31:0] of current round key used in PRASH */
#define PUF_EVAL_RND_KEY_0_E_RND_KEY_0(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_RND_KEY_0_E_RND_KEY_0_SHIFT)) & PUF_EVAL_RND_KEY_0_E_RND_KEY_0_MASK)
/*! @} */

/*! @name EVAL_RND_KEY_1 - Evaluation Round Key register */
/*! @{ */

#define PUF_EVAL_RND_KEY_1_E_RND_KEY_1_MASK      (0xFFFFFFFFU)
#define PUF_EVAL_RND_KEY_1_E_RND_KEY_1_SHIFT     (0U)
/*! E_RND_KEY_1 - Bits [63:32] of current round key used in PRASH */
#define PUF_EVAL_RND_KEY_1_E_RND_KEY_1(x)        (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_RND_KEY_1_E_RND_KEY_1_SHIFT)) & PUF_EVAL_RND_KEY_1_E_RND_KEY_1_MASK)
/*! @} */

/*! @name EVAL_CTRL - Evaluation Control register */
/*! @{ */

#define PUF_EVAL_CTRL_E_EVAL_EN_MASK             (0xFU)
#define PUF_EVAL_CTRL_E_EVAL_EN_SHIFT            (0U)
/*! E_EVAL_EN - Evaluation mode enable; 4'h5 - Disabled; 4'hA - Enabled */
#define PUF_EVAL_CTRL_E_EVAL_EN(x)               (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_CTRL_E_EVAL_EN_SHIFT)) & PUF_EVAL_CTRL_E_EVAL_EN_MASK)

#define PUF_EVAL_CTRL_E_EVAL_LCK_MASK            (0xF0U)
#define PUF_EVAL_CTRL_E_EVAL_LCK_SHIFT           (4U)
/*! E_EVAL_LCK - Evaluation mode lock; 4'h5 - Locked; 4'hA - Unlocked */
#define PUF_EVAL_CTRL_E_EVAL_LCK(x)              (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_CTRL_E_EVAL_LCK_SHIFT)) & PUF_EVAL_CTRL_E_EVAL_LCK_MASK)

#define PUF_EVAL_CTRL_E_FREE_MASK                (0x100U)
#define PUF_EVAL_CTRL_E_FREE_SHIFT               (8U)
/*! E_FREE - Free running mode; 1'b0 - Off; 1'b1 - On */
#define PUF_EVAL_CTRL_E_FREE(x)                  (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_CTRL_E_FREE_SHIFT)) & PUF_EVAL_CTRL_E_FREE_MASK)

#define PUF_EVAL_CTRL_E_RSVD_18_MASK             (0xFFFFFE00U)
#define PUF_EVAL_CTRL_E_RSVD_18_SHIFT            (9U)
/*! E_RSVD_18 - Reserved */
#define PUF_EVAL_CTRL_E_RSVD_18(x)               (((uint32_t)(((uint32_t)(x)) << PUF_EVAL_CTRL_E_RSVD_18_SHIFT)) & PUF_EVAL_CTRL_E_RSVD_18_MASK)
/*! @} */

/*! @name ACCESS_ERR - Access Error register */
/*! @{ */

#define PUF_ACCESS_ERR_APB_NOTAV_MASK            (0x1U)
#define PUF_ACCESS_ERR_APB_NOTAV_SHIFT           (0U)
/*! APB_NOTAV - APB error: address not available */
#define PUF_ACCESS_ERR_APB_NOTAV(x)              (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_APB_NOTAV_SHIFT)) & PUF_ACCESS_ERR_APB_NOTAV_MASK)

#define PUF_ACCESS_ERR_APB_WRGMD_MASK            (0x2U)
#define PUF_ACCESS_ERR_APB_WRGMD_SHIFT           (1U)
/*! APB_WRGMD - APB error: wrong access mode */
#define PUF_ACCESS_ERR_APB_WRGMD(x)              (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_APB_WRGMD_SHIFT)) & PUF_ACCESS_ERR_APB_WRGMD_MASK)

#define PUF_ACCESS_ERR_ACCESS_ERR_RSVD_1_MASK    (0xCU)
#define PUF_ACCESS_ERR_ACCESS_ERR_RSVD_1_SHIFT   (2U)
/*! ACCESS_ERR_RSVD_1 - Reserved */
#define PUF_ACCESS_ERR_ACCESS_ERR_RSVD_1(x)      (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_ACCESS_ERR_RSVD_1_SHIFT)) & PUF_ACCESS_ERR_ACCESS_ERR_RSVD_1_MASK)

#define PUF_ACCESS_ERR_APB_MASTER_MASK           (0xF0U)
#define PUF_ACCESS_ERR_APB_MASTER_SHIFT          (4U)
/*! APB_MASTER - APB Master that triggered the APB error */
#define PUF_ACCESS_ERR_APB_MASTER(x)             (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_APB_MASTER_SHIFT)) & PUF_ACCESS_ERR_APB_MASTER_MASK)

#define PUF_ACCESS_ERR_ACCESS_ERR_RSVD_0_MASK    (0xFFFFFF00U)
#define PUF_ACCESS_ERR_ACCESS_ERR_RSVD_0_SHIFT   (8U)
/*! ACCESS_ERR_RSVD_0 - Reserved */
#define PUF_ACCESS_ERR_ACCESS_ERR_RSVD_0(x)      (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_ACCESS_ERR_RSVD_0_SHIFT)) & PUF_ACCESS_ERR_ACCESS_ERR_RSVD_0_MASK)
/*! @} */

/*! @name ACCESS_ERR_CLR - Access Error Clear register */
/*! @{ */

#define PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_MASK   (0x1U)
#define PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_SHIFT  (0U)
/*! ACCESS_ERR_CLR - Clear access errors */
#define PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR(x)     (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_SHIFT)) & PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_MASK)

#define PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_RSVD_MASK (0xFFFFFFFEU)
#define PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_RSVD_SHIFT (1U)
/*! ACCESS_ERR_CLR_RSVD - Reserved */
#define PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_RSVD(x) (((uint32_t)(((uint32_t)(x)) << PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_RSVD_SHIFT)) & PUF_ACCESS_ERR_CLR_ACCESS_ERR_CLR_RSVD_MASK)
/*! @} */

/*! @name INT_STATUS - Interrupt Status register */
/*! @{ */

#define PUF_INT_STATUS_INT_ERROR_MASK            (0x1U)
#define PUF_INT_STATUS_INT_ERROR_SHIFT           (0U)
/*! INT_ERROR - Error has occured */
#define PUF_INT_STATUS_INT_ERROR(x)              (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_INT_ERROR_SHIFT)) & PUF_INT_STATUS_INT_ERROR_MASK)

#define PUF_INT_STATUS_RNG_RDY_MASK              (0x2U)
#define PUF_INT_STATUS_RNG_RDY_SHIFT             (1U)
/*! RNG_RDY - Reconstruction only: more random data is required */
#define PUF_INT_STATUS_RNG_RDY(x)                (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_RNG_RDY_SHIFT)) & PUF_INT_STATUS_RNG_RDY_MASK)

#define PUF_INT_STATUS_RANK_DONE_MASK            (0x4U)
#define PUF_INT_STATUS_RANK_DONE_SHIFT           (2U)
/*! RANK_DONE - Enrollment only: Ranking is complete and ignore data is ready to be read */
#define PUF_INT_STATUS_RANK_DONE(x)              (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_RANK_DONE_SHIFT)) & PUF_INT_STATUS_RANK_DONE_MASK)

#define PUF_INT_STATUS_PAR_RDY_MASK              (0x8U)
#define PUF_INT_STATUS_PAR_RDY_SHIFT             (3U)
/*! PAR_RDY - Parity data has been calculated and ready to be read */
#define PUF_INT_STATUS_PAR_RDY(x)                (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_PAR_RDY_SHIFT)) & PUF_INT_STATUS_PAR_RDY_MASK)

#define PUF_INT_STATUS_CKS_RDY_MASK              (0x10U)
#define PUF_INT_STATUS_CKS_RDY_SHIFT             (4U)
/*! CKS_RDY - Checksum has been calculated and ready to be read */
#define PUF_INT_STATUS_CKS_RDY(x)                (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CKS_RDY_SHIFT)) & PUF_INT_STATUS_CKS_RDY_MASK)

#define PUF_INT_STATUS_IGN_LOAD_MASK             (0x20U)
#define PUF_INT_STATUS_IGN_LOAD_SHIFT            (5U)
/*! IGN_LOAD - Reconstruction only: ignore data is required to be loaded */
#define PUF_INT_STATUS_IGN_LOAD(x)               (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_IGN_LOAD_SHIFT)) & PUF_INT_STATUS_IGN_LOAD_MASK)

#define PUF_INT_STATUS_KEY_RDY_MASK              (0x40U)
#define PUF_INT_STATUS_KEY_RDY_SHIFT             (6U)
/*! KEY_RDY - Key chunk has been generated and ready to be read */
#define PUF_INT_STATUS_KEY_RDY(x)                (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_KEY_RDY_SHIFT)) & PUF_INT_STATUS_KEY_RDY_MASK)

#define PUF_INT_STATUS_INT_RSVD_MASK             (0xFFFFFF80U)
#define PUF_INT_STATUS_INT_RSVD_SHIFT            (7U)
/*! INT_RSVD - Reserved */
#define PUF_INT_STATUS_INT_RSVD(x)               (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_INT_RSVD_SHIFT)) & PUF_INT_STATUS_INT_RSVD_MASK)
/*! @} */

/*! @name INT_ENABLE - Interrupt Enable register */
/*! @{ */

#define PUF_INT_ENABLE_INT_EN_ERROR_MASK         (0x1U)
#define PUF_INT_ENABLE_INT_EN_ERROR_SHIFT        (0U)
/*! INT_EN_ERROR - Interrupt enable for error interrupt */
#define PUF_INT_ENABLE_INT_EN_ERROR(x)           (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_ERROR_SHIFT)) & PUF_INT_ENABLE_INT_EN_ERROR_MASK)

#define PUF_INT_ENABLE_INT_EN_RNG_RDY_MASK       (0x2U)
#define PUF_INT_ENABLE_INT_EN_RNG_RDY_SHIFT      (1U)
/*! INT_EN_RNG_RDY - Interrupt enable for RNG_RDY */
#define PUF_INT_ENABLE_INT_EN_RNG_RDY(x)         (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_RNG_RDY_SHIFT)) & PUF_INT_ENABLE_INT_EN_RNG_RDY_MASK)

#define PUF_INT_ENABLE_INT_EN_RANK_DONE_MASK     (0x4U)
#define PUF_INT_ENABLE_INT_EN_RANK_DONE_SHIFT    (2U)
/*! INT_EN_RANK_DONE - Interrupt enable for RANK_DONE */
#define PUF_INT_ENABLE_INT_EN_RANK_DONE(x)       (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_RANK_DONE_SHIFT)) & PUF_INT_ENABLE_INT_EN_RANK_DONE_MASK)

#define PUF_INT_ENABLE_INT_EN_PAR_RDY_MASK       (0x8U)
#define PUF_INT_ENABLE_INT_EN_PAR_RDY_SHIFT      (3U)
/*! INT_EN_PAR_RDY - Interrupt enable for PAR_RDY */
#define PUF_INT_ENABLE_INT_EN_PAR_RDY(x)         (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_PAR_RDY_SHIFT)) & PUF_INT_ENABLE_INT_EN_PAR_RDY_MASK)

#define PUF_INT_ENABLE_INT_EN_CKS_RDY_MASK       (0x10U)
#define PUF_INT_ENABLE_INT_EN_CKS_RDY_SHIFT      (4U)
/*! INT_EN_CKS_RDY - Interrupt enable for CKS_RDY */
#define PUF_INT_ENABLE_INT_EN_CKS_RDY(x)         (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_CKS_RDY_SHIFT)) & PUF_INT_ENABLE_INT_EN_CKS_RDY_MASK)

#define PUF_INT_ENABLE_INT_EN_IGN_LOAD_MASK      (0x20U)
#define PUF_INT_ENABLE_INT_EN_IGN_LOAD_SHIFT     (5U)
/*! INT_EN_IGN_LOAD - Interrupt enable for IGN_LOAD */
#define PUF_INT_ENABLE_INT_EN_IGN_LOAD(x)        (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_IGN_LOAD_SHIFT)) & PUF_INT_ENABLE_INT_EN_IGN_LOAD_MASK)

#define PUF_INT_ENABLE_INT_EN_KEY_RDY_MASK       (0x40U)
#define PUF_INT_ENABLE_INT_EN_KEY_RDY_SHIFT      (6U)
/*! INT_EN_KEY_RDY - Interrupt enable for KEY_RDY */
#define PUF_INT_ENABLE_INT_EN_KEY_RDY(x)         (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_KEY_RDY_SHIFT)) & PUF_INT_ENABLE_INT_EN_KEY_RDY_MASK)

#define PUF_INT_ENABLE_INT_EN_RSVD_MASK          (0xFFFFFF80U)
#define PUF_INT_ENABLE_INT_EN_RSVD_SHIFT         (7U)
/*! INT_EN_RSVD - Reserved */
#define PUF_INT_ENABLE_INT_EN_RSVD(x)            (((uint32_t)(((uint32_t)(x)) << PUF_INT_ENABLE_INT_EN_RSVD_SHIFT)) & PUF_INT_ENABLE_INT_EN_RSVD_MASK)
/*! @} */

/*! @name INT_STATUS_CLR - Interrupt Status Clear register */
/*! @{ */

#define PUF_INT_STATUS_CLR_INT_CLR_ERROR_MASK    (0x1U)
#define PUF_INT_STATUS_CLR_INT_CLR_ERROR_SHIFT   (0U)
/*! INT_CLR_ERROR - Interrupt clear for error interrupt */
#define PUF_INT_STATUS_CLR_INT_CLR_ERROR(x)      (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_ERROR_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_ERROR_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_RNG_RDY_MASK  (0x2U)
#define PUF_INT_STATUS_CLR_INT_CLR_RNG_RDY_SHIFT (1U)
/*! INT_CLR_RNG_RDY - Interrupt clear for RNG_RDY */
#define PUF_INT_STATUS_CLR_INT_CLR_RNG_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_RNG_RDY_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_RNG_RDY_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_RANK_DONE_MASK (0x4U)
#define PUF_INT_STATUS_CLR_INT_CLR_RANK_DONE_SHIFT (2U)
/*! INT_CLR_RANK_DONE - Interrupt clear for RANK_DONE */
#define PUF_INT_STATUS_CLR_INT_CLR_RANK_DONE(x)  (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_RANK_DONE_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_RANK_DONE_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_PAR_RDY_MASK  (0x8U)
#define PUF_INT_STATUS_CLR_INT_CLR_PAR_RDY_SHIFT (3U)
/*! INT_CLR_PAR_RDY - Interrupt clear for PAR_RDY */
#define PUF_INT_STATUS_CLR_INT_CLR_PAR_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_PAR_RDY_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_PAR_RDY_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_CKS_RDY_MASK  (0x10U)
#define PUF_INT_STATUS_CLR_INT_CLR_CKS_RDY_SHIFT (4U)
/*! INT_CLR_CKS_RDY - Interrupt clear for CKS_RDY */
#define PUF_INT_STATUS_CLR_INT_CLR_CKS_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_CKS_RDY_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_CKS_RDY_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_IGN_LOAD_MASK (0x20U)
#define PUF_INT_STATUS_CLR_INT_CLR_IGN_LOAD_SHIFT (5U)
/*! INT_CLR_IGN_LOAD - Interrupt clear for IGN_LOAD */
#define PUF_INT_STATUS_CLR_INT_CLR_IGN_LOAD(x)   (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_IGN_LOAD_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_IGN_LOAD_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_KEY_RDY_MASK  (0x40U)
#define PUF_INT_STATUS_CLR_INT_CLR_KEY_RDY_SHIFT (6U)
/*! INT_CLR_KEY_RDY - Interrupt clear for KEY_RDY */
#define PUF_INT_STATUS_CLR_INT_CLR_KEY_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_KEY_RDY_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_KEY_RDY_MASK)

#define PUF_INT_STATUS_CLR_INT_CLR_RSVD_MASK     (0xFFFFFF80U)
#define PUF_INT_STATUS_CLR_INT_CLR_RSVD_SHIFT    (7U)
/*! INT_CLR_RSVD - Reserved */
#define PUF_INT_STATUS_CLR_INT_CLR_RSVD(x)       (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_CLR_INT_CLR_RSVD_SHIFT)) & PUF_INT_STATUS_CLR_INT_CLR_RSVD_MASK)
/*! @} */

/*! @name INT_STATUS_SET - Interrupt Status Set register */
/*! @{ */

#define PUF_INT_STATUS_SET_INT_SET_ERROR_MASK    (0x1U)
#define PUF_INT_STATUS_SET_INT_SET_ERROR_SHIFT   (0U)
/*! INT_SET_ERROR - Interrupt set for error interrupt */
#define PUF_INT_STATUS_SET_INT_SET_ERROR(x)      (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_ERROR_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_ERROR_MASK)

#define PUF_INT_STATUS_SET_INT_SET_RNG_RDY_MASK  (0x2U)
#define PUF_INT_STATUS_SET_INT_SET_RNG_RDY_SHIFT (1U)
/*! INT_SET_RNG_RDY - Interrupt set for RNG_RDY */
#define PUF_INT_STATUS_SET_INT_SET_RNG_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_RNG_RDY_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_RNG_RDY_MASK)

#define PUF_INT_STATUS_SET_INT_SET_RANK_DONE_MASK (0x4U)
#define PUF_INT_STATUS_SET_INT_SET_RANK_DONE_SHIFT (2U)
/*! INT_SET_RANK_DONE - Interrupt set for RANK_DONE */
#define PUF_INT_STATUS_SET_INT_SET_RANK_DONE(x)  (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_RANK_DONE_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_RANK_DONE_MASK)

#define PUF_INT_STATUS_SET_INT_SET_PAR_RDY_MASK  (0x8U)
#define PUF_INT_STATUS_SET_INT_SET_PAR_RDY_SHIFT (3U)
/*! INT_SET_PAR_RDY - Interrupt set for PAR_RDY */
#define PUF_INT_STATUS_SET_INT_SET_PAR_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_PAR_RDY_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_PAR_RDY_MASK)

#define PUF_INT_STATUS_SET_INT_SET_CKS_RDY_MASK  (0x10U)
#define PUF_INT_STATUS_SET_INT_SET_CKS_RDY_SHIFT (4U)
/*! INT_SET_CKS_RDY - Interrupt set for CKS_RDY */
#define PUF_INT_STATUS_SET_INT_SET_CKS_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_CKS_RDY_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_CKS_RDY_MASK)

#define PUF_INT_STATUS_SET_INT_SET_IGN_LOAD_MASK (0x20U)
#define PUF_INT_STATUS_SET_INT_SET_IGN_LOAD_SHIFT (5U)
/*! INT_SET_IGN_LOAD - Interrupt set for IGN_LOAD */
#define PUF_INT_STATUS_SET_INT_SET_IGN_LOAD(x)   (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_IGN_LOAD_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_IGN_LOAD_MASK)

#define PUF_INT_STATUS_SET_INT_SET_KEY_RDY_MASK  (0x40U)
#define PUF_INT_STATUS_SET_INT_SET_KEY_RDY_SHIFT (6U)
/*! INT_SET_KEY_RDY - Interrupt set for KEY_RDY */
#define PUF_INT_STATUS_SET_INT_SET_KEY_RDY(x)    (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_KEY_RDY_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_KEY_RDY_MASK)

#define PUF_INT_STATUS_SET_INT_SET_RSVD_MASK     (0xFFFFFF80U)
#define PUF_INT_STATUS_SET_INT_SET_RSVD_SHIFT    (7U)
/*! INT_SET_RSVD - Reserved */
#define PUF_INT_STATUS_SET_INT_SET_RSVD(x)       (((uint32_t)(((uint32_t)(x)) << PUF_INT_STATUS_SET_INT_SET_RSVD_SHIFT)) & PUF_INT_STATUS_SET_INT_SET_RSVD_MASK)
/*! @} */

/*! @name MODULE_ID - Module ID register */
/*! @{ */

#define PUF_MODULE_ID_PLACEHOLDER_MASK           (0xFFFFFFFFU)
#define PUF_MODULE_ID_PLACEHOLDER_SHIFT          (0U)
/*! PLACEHOLDER - Module ID */
#define PUF_MODULE_ID_PLACEHOLDER(x)             (((uint32_t)(((uint32_t)(x)) << PUF_MODULE_ID_PLACEHOLDER_SHIFT)) & PUF_MODULE_ID_PLACEHOLDER_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group PUF_Register_Masks */


/*!
 * @}
 */ /* end of group PUF_Peripheral_Access_Layer */


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


#endif  /* PUF_H_ */

