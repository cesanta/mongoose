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
**         CMSIS Peripheral Access Layer for SOC_OTP_CTRL
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
 * @file SOC_OTP_CTRL.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SOC_OTP_CTRL
 *
 * CMSIS Peripheral Access Layer for SOC_OTP_CTRL
 */

#if !defined(SOC_OTP_CTRL_H_)
#define SOC_OTP_CTRL_H_                          /**< Symbol preventing repeated inclusion */

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
   -- SOC_OTP_CTRL Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SOC_OTP_CTRL_Peripheral_Access_Layer SOC_OTP_CTRL Peripheral Access Layer
 * @{
 */

/** SOC_OTP_CTRL - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[512];
  __IO uint16_t OTP_SPARE0;                        /**< OTP_SPARE0_REG, offset: 0x200 */
       uint8_t RESERVED_1[2];
  __IO uint16_t OTP_SPARE1;                        /**< OTP_SPARE1_REG, offset: 0x204 */
       uint8_t RESERVED_2[2];
  __O  uint16_t OTP_WDATA0;                        /**< OTP_WDATA0_REG, offset: 0x208 */
       uint8_t RESERVED_3[2];
  __IO uint16_t OTP_WDATA1;                        /**< OTP_WDATA1_REG, offset: 0x20C */
       uint8_t RESERVED_4[2];
  __IO uint16_t OTP_WDATA2;                        /**< OTP_WDATA2_REG, offset: 0x210 */
       uint8_t RESERVED_5[2];
  __IO uint16_t OTP_WDATA3;                        /**< OTP_WDATA3_REG, offset: 0x214 */
       uint8_t RESERVED_6[2];
  __I  uint16_t OTP_WDATA4;                        /**< OTP_WDATA4_REG, offset: 0x218 */
       uint8_t RESERVED_7[2];
  __IO uint16_t OTP_ADDR;                          /**< OTP_ADDR_REG, offset: 0x21C */
       uint8_t RESERVED_8[2];
  __IO uint16_t OTP_CTRL0;                         /**< OTP_CTRL0_REG, offset: 0x220 */
       uint8_t RESERVED_9[2];
  __IO uint16_t OTP_CTRL1;                         /**< OTP_CTRL1_REG, offset: 0x224 */
       uint8_t RESERVED_10[2];
  __IO uint16_t OTP_CMD_START;                     /**< OTP_CMD_START_REG, offset: 0x228 */
       uint8_t RESERVED_11[2];
  __IO uint16_t OTP_PARAM0;                        /**< OTP_PARAM0_REG, offset: 0x22C */
       uint8_t RESERVED_12[2];
  __IO uint16_t OTP_PARAM1;                        /**< OTP_PARAM1_REG, offset: 0x230 */
       uint8_t RESERVED_13[2];
  __IO uint16_t OTP_PARAM2;                        /**< OTP_PARAM2_REG, offset: 0x234 */
       uint8_t RESERVED_14[2];
  __IO uint16_t OTP_BYPASS_MODE0;                  /**< OTP_BYPASS_MODE0_REG, offset: 0x238 */
       uint8_t RESERVED_15[2];
  __IO uint16_t OTP_BYPASS_MODE1;                  /**< OTP_BYPASS_MODE1_REG, offset: 0x23C */
       uint8_t RESERVED_16[2];
  __IO uint16_t OTP_TESTBUS_SEL;                   /**< OTP_TESTBUS_SEL_REG, offset: 0x240 */
       uint8_t RESERVED_17[2];
  __I  uint16_t OTP_TESTBUS;                       /**< OTP_TESTBUS_REG, offset: 0x244 */
       uint8_t RESERVED_18[2];
  __IO uint16_t OTP_BYPASS_MODE2;                  /**< OTP_BYPASS_MODE2_REG, offset: 0x248 */
       uint8_t RESERVED_19[2];
  __IO uint16_t OTP_RST_B;                         /**< OTP_RST_B_REG, offset: 0x24C */
       uint8_t RESERVED_20[2];
  __IO uint16_t OTP_POR_B;                         /**< OTP_POR_B_REG, offset: 0x250 */
       uint8_t RESERVED_21[10];
  __IO uint16_t OTP_WRITE_LOCK_REG;                /**< OTP_WRITE_LOCK_REG (Firecrest), offset: 0x25C */
       uint8_t RESERVED_22[2];
  __IO uint16_t OTP_WRITE_DIS_REG_15_0;            /**< OTP_WRITE_DIS_REG[15:0] (Firecrest), offset: 0x260 */
       uint8_t RESERVED_23[2];
  __IO uint16_t OTP_WRITE_DIS_REG_31_16;           /**< OTP_WRITE_DIS_REG[31:16] (Firecrest), offset: 0x264 */
       uint8_t RESERVED_24[2];
  __IO uint16_t OTP_WRITE_DIS_REG_47_32;           /**< OTP_WRITE_DIS_REG[47:32] (Firecrest), offset: 0x268 */
       uint8_t RESERVED_25[2];
  __IO uint16_t OTP_WRITE_DIS_REG_63_48;           /**< OTP_WRITE_DIS_REG[63:48] (Firecrest), offset: 0x26C */
       uint8_t RESERVED_26[2];
  __IO uint16_t OTP_WRITE_DIS_REG_79_64;           /**< OTP_WRITE_DIS_REG[79:64] (Firecrest), offset: 0x270 */
       uint8_t RESERVED_27[2];
  __IO uint16_t OTP_WRITE_DIS_REG_95_80;           /**< OTP_WRITE_DIS_REG[95:80] (Firecrest), offset: 0x274 */
       uint8_t RESERVED_28[2];
  __IO uint16_t OTP_WRITE_DIS_REG_111_96;          /**< OTP_WRITE_DIS_REG[111:96] (Firecrest), offset: 0x278 */
       uint8_t RESERVED_29[2];
  __IO uint16_t OTP_WRITE_DIS_REG_127_112;         /**< OTP_WRITE_DIS_REG[127:112] (Firecrest), offset: 0x27C */
} SOC_OTP_CTRL_Type;

/* ----------------------------------------------------------------------------
   -- SOC_OTP_CTRL Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SOC_OTP_CTRL_Register_Masks SOC_OTP_CTRL Register Masks
 * @{
 */

/*! @name OTP_SPARE0 - OTP_SPARE0_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_SPARE0_OTP_SPARE0_MASK  (0xFFFFU)
#define SOC_OTP_CTRL_OTP_SPARE0_OTP_SPARE0_SHIFT (0U)
/*! OTP_SPARE0 - SPARE register for future ECO's, h/w default is 0x0000 */
#define SOC_OTP_CTRL_OTP_SPARE0_OTP_SPARE0(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_SPARE0_OTP_SPARE0_SHIFT)) & SOC_OTP_CTRL_OTP_SPARE0_OTP_SPARE0_MASK)
/*! @} */

/*! @name OTP_SPARE1 - OTP_SPARE1_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_SPARE1_OTP_SPARE1_MASK  (0xFFFFU)
#define SOC_OTP_CTRL_OTP_SPARE1_OTP_SPARE1_SHIFT (0U)
/*! OTP_SPARE1 - SPARE register for future ECO's, h/w default is 0xFFFF */
#define SOC_OTP_CTRL_OTP_SPARE1_OTP_SPARE1(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_SPARE1_OTP_SPARE1_SHIFT)) & SOC_OTP_CTRL_OTP_SPARE1_OTP_SPARE1_MASK)
/*! @} */

/*! @name OTP_WDATA0 - OTP_WDATA0_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WDATA0_OTP_WDATA0_MASK  (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WDATA0_OTP_WDATA0_SHIFT (0U)
/*! OTP_WDATA0 - Data[15:0] */
#define SOC_OTP_CTRL_OTP_WDATA0_OTP_WDATA0(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA0_OTP_WDATA0_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA0_OTP_WDATA0_MASK)
/*! @} */

/*! @name OTP_WDATA1 - OTP_WDATA1_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WDATA1_OTP_WDATA1_MASK  (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WDATA1_OTP_WDATA1_SHIFT (0U)
/*! OTP_WDATA1 - Data[31:16] */
#define SOC_OTP_CTRL_OTP_WDATA1_OTP_WDATA1(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA1_OTP_WDATA1_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA1_OTP_WDATA1_MASK)
/*! @} */

/*! @name OTP_WDATA2 - OTP_WDATA2_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WDATA2_OTP_WDATA2_MASK  (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WDATA2_OTP_WDATA2_SHIFT (0U)
/*! OTP_WDATA2 - Data[47:32] */
#define SOC_OTP_CTRL_OTP_WDATA2_OTP_WDATA2(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA2_OTP_WDATA2_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA2_OTP_WDATA2_MASK)
/*! @} */

/*! @name OTP_WDATA3 - OTP_WDATA3_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WDATA3_OTP_WDATA3_MASK  (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WDATA3_OTP_WDATA3_SHIFT (0U)
/*! OTP_WDATA3 - Data[63:48] */
#define SOC_OTP_CTRL_OTP_WDATA3_OTP_WDATA3(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA3_OTP_WDATA3_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA3_OTP_WDATA3_MASK)
/*! @} */

/*! @name OTP_WDATA4 - OTP_WDATA4_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WDATA4_ATE_TEST_BIT_MASK (0x1U)
#define SOC_OTP_CTRL_OTP_WDATA4_ATE_TEST_BIT_SHIFT (0U)
/*! ATE_TEST_BIT - ATE test bit (READ ONLY) */
#define SOC_OTP_CTRL_OTP_WDATA4_ATE_TEST_BIT(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA4_ATE_TEST_BIT_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA4_ATE_TEST_BIT_MASK)

#define SOC_OTP_CTRL_OTP_WDATA4_DATA_LINE_VALID_BIT_MASK (0x2U)
#define SOC_OTP_CTRL_OTP_WDATA4_DATA_LINE_VALID_BIT_SHIFT (1U)
/*! DATA_LINE_VALID_BIT - Data line valid bit (READ ONLY) */
#define SOC_OTP_CTRL_OTP_WDATA4_DATA_LINE_VALID_BIT(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA4_DATA_LINE_VALID_BIT_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA4_DATA_LINE_VALID_BIT_MASK)

#define SOC_OTP_CTRL_OTP_WDATA4_SECURITY_BIT_MASK (0x4U)
#define SOC_OTP_CTRL_OTP_WDATA4_SECURITY_BIT_SHIFT (2U)
/*! SECURITY_BIT - Security Fuse bit (READ ONLY) */
#define SOC_OTP_CTRL_OTP_WDATA4_SECURITY_BIT(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA4_SECURITY_BIT_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA4_SECURITY_BIT_MASK)

#define SOC_OTP_CTRL_OTP_WDATA4_OTP_ALL_BITS_EQ_ZERO_MASK (0x8U)
#define SOC_OTP_CTRL_OTP_WDATA4_OTP_ALL_BITS_EQ_ZERO_SHIFT (3U)
/*! OTP_ALL_BITS_EQ_ZERO - All bits equal to zero (READ ONLY) */
#define SOC_OTP_CTRL_OTP_WDATA4_OTP_ALL_BITS_EQ_ZERO(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WDATA4_OTP_ALL_BITS_EQ_ZERO_SHIFT)) & SOC_OTP_CTRL_OTP_WDATA4_OTP_ALL_BITS_EQ_ZERO_MASK)
/*! @} */

/*! @name OTP_ADDR - OTP_ADDR_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_ADDR_OTP_ADDR_MASK      (0x3FFU)
#define SOC_OTP_CTRL_OTP_ADDR_OTP_ADDR_SHIFT     (0U)
/*! OTP_ADDR - The otp_addr is composed of Bank select[16:13] and row address[12:7], the address
 *    will select a row or a data line of a bank that contains 64 bits of s/w usable data plus the ATE
 *    test bit and data line valid bit and the security bit.
 */
#define SOC_OTP_CTRL_OTP_ADDR_OTP_ADDR(x)        (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_ADDR_OTP_ADDR_SHIFT)) & SOC_OTP_CTRL_OTP_ADDR_OTP_ADDR_MASK)
/*! @} */

/*! @name OTP_CTRL0 - OTP_CTRL0_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN25_33B_MASK (0x2U)
#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN25_33B_SHIFT (1U)
/*! REGULATOR_EN25_33B - Stepdown regulator input voltage select */
#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN25_33B(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN25_33B_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN25_33B_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_SV_MASK (0xCU)
#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_SV_SHIFT (2U)
/*! REGULATOR_SV - Stepdown regulator Rate Control, default = 2'b10 (2'b10==2.6V for 40nm) */
#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_SV(x)   (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_SV_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_SV_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_REG_SETULATOR_EN_BYPASS_MASK (0x10U)
#define SOC_OTP_CTRL_OTP_CTRL0_REG_SETULATOR_EN_BYPASS_SHIFT (4U)
/*! REG_SETULATOR_EN_BYPASS - Enable/ disable value: */
#define SOC_OTP_CTRL_OTP_CTRL0_REG_SETULATOR_EN_BYPASS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_REG_SETULATOR_EN_BYPASS_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_REG_SETULATOR_EN_BYPASS_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN_MASK (0x20U)
#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN_SHIFT (5U)
/*! REGULATOR_EN - Stepdown regulator output enable, only valid if regulator_EN_bypass==1. */
#define SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN(x)   (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_REGULATOR_EN_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_MACRO_RD_DONE_MASK (0x80U)
#define SOC_OTP_CTRL_OTP_CTRL0_MACRO_RD_DONE_SHIFT (7U)
/*! MACRO_RD_DONE - RD_DONE is from the OTP bank (selected by otp_addr_reg[9:6]). */
#define SOC_OTP_CTRL_OTP_CTRL0_MACRO_RD_DONE(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_MACRO_RD_DONE_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_MACRO_RD_DONE_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_MACRO_TEST_MASK   (0xF00U)
#define SOC_OTP_CTRL_OTP_CTRL0_MACRO_TEST_SHIFT  (8U)
/*! MACRO_TEST - S/W control bits to TEST[3:0] inputs. */
#define SOC_OTP_CTRL_OTP_CTRL0_MACRO_TEST(x)     (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_MACRO_TEST_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_MACRO_TEST_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_REG_CSTATE_RESET_MASK (0x4000U)
#define SOC_OTP_CTRL_OTP_CTRL0_REG_CSTATE_RESET_SHIFT (14U)
/*! REG_CSTATE_RESET - Reset OTP ctrl sm (for debugging purpose). */
#define SOC_OTP_CTRL_OTP_CTRL0_REG_CSTATE_RESET(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_REG_CSTATE_RESET_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_REG_CSTATE_RESET_MASK)

#define SOC_OTP_CTRL_OTP_CTRL0_CTRL_CMD_DONE_MASK (0x8000U)
#define SOC_OTP_CTRL_OTP_CTRL0_CTRL_CMD_DONE_SHIFT (15U)
/*! CTRL_CMD_DONE - otp access command done. */
#define SOC_OTP_CTRL_OTP_CTRL0_CTRL_CMD_DONE(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL0_CTRL_CMD_DONE_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL0_CTRL_CMD_DONE_MASK)
/*! @} */

/*! @name OTP_CTRL1 - OTP_CTRL1_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_OTPMEM_MASK (0x1U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_OTPMEM_SHIFT (0U)
/*! OTP_PROG_SEQ_CODE_CLK_OTPMEM - Clock input, used to program the match code into all the OTP memories */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_OTPMEM(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_OTPMEM_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_OTPMEM_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_OTPMEM_MASK (0x2U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_OTPMEM_SHIFT (1U)
/*! OTP_PROG_SEQ_CODE_OTPMEM - Data input, used to program the match code into all the OTP memories. */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_OTPMEM(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_OTPMEM_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_OTPMEM_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_VTR_MASK (0x4U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_VTR_SHIFT (2U)
/*! OTP_PROG_SEQ_CODE_CLK_VTR - Clock input, used to program the match code into all the VTR (Step Down regulator) */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_VTR(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_VTR_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_CLK_VTR_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_VTR_MASK (0x8U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_VTR_SHIFT (3U)
/*! OTP_PROG_SEQ_CODE_VTR - Data input, used to program the match code into all the VTR (Step Down regulator) */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_VTR(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_VTR_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_VTR_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_MUX_SEL_MASK (0x10U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_MUX_SEL_SHIFT (4U)
/*! OTP_PROG_SEQ_CODE_MUX_SEL - SELECT OTP register for the match Code programming. */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_MUX_SEL(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_MUX_SEL_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_PROG_SEQ_CODE_MUX_SEL_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_SCLK_MEM_PROG_MASK (0x20U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_SCLK_MEM_PROG_SHIFT (5U)
/*! OTP_FORCE_SCLK_MEM_PROG - Only used for OTP memory programming: Set this bit to allow the match
 *    code programming, clear this bit after the match code programming before start otp memory
 *    programming.
 */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_SCLK_MEM_PROG(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_SCLK_MEM_PROG_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_SCLK_MEM_PROG_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_STEPDOWN_PROG_MASK (0x40U)
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_STEPDOWN_PROG_SHIFT (6U)
/*! OTP_FORCE_STEPDOWN_PROG - For OTP memory programming: This bit need to be set before the match
 *    code programming and clear this bit after the otp memory contents programming is done.
 */
#define SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_STEPDOWN_PROG(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_STEPDOWN_PROG_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_OTP_FORCE_STEPDOWN_PROG_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_STEPDOWN_MATCH_MASK (0x80U)
#define SOC_OTP_CTRL_OTP_CTRL1_STEPDOWN_MATCH_SHIFT (7U)
/*! STEPDOWN_MATCH - Match value: */
#define SOC_OTP_CTRL_OTP_CTRL1_STEPDOWN_MATCH(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_STEPDOWN_MATCH_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_STEPDOWN_MATCH_MASK)

#define SOC_OTP_CTRL_OTP_CTRL1_MACRO_MATCH_MASK  (0xFF00U)
#define SOC_OTP_CTRL_OTP_CTRL1_MACRO_MATCH_SHIFT (8U)
/*! MACRO_MATCH - 0: match code does not match. */
#define SOC_OTP_CTRL_OTP_CTRL1_MACRO_MATCH(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CTRL1_MACRO_MATCH_SHIFT)) & SOC_OTP_CTRL_OTP_CTRL1_MACRO_MATCH_MASK)
/*! @} */

/*! @name OTP_CMD_START - OTP_CMD_START_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_START_MASK (0x1U)
#define SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_START_SHIFT (0U)
/*! OTP_CMD_START - Write 1 to start otp access command execution and it clears the ctrl_cmd_done bit. */
#define SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_START(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_START_SHIFT)) & SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_START_MASK)

#define SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_MASK  (0xF0U)
#define SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_SHIFT (4U)
/*! OTP_CMD - OTP access commands (the command execution finishes when the ctrl_cmd_done bit is set): */
#define SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_SHIFT)) & SOC_OTP_CTRL_OTP_CMD_START_OTP_CMD_MASK)
/*! @} */

/*! @name OTP_PARAM0 - OTP_PARAM0_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_PARAM0_TPGM_2US_MASK    (0x3FFU)
#define SOC_OTP_CTRL_OTP_PARAM0_TPGM_2US_SHIFT   (0U)
/*! TPGM_2US - Specify the width of the programming pulse (in number of pclk cycle), the number
 *    depends on the pclk frequency and the pulse width requirement (OTP macro specifies between 2us up
 *    to 10us).
 */
#define SOC_OTP_CTRL_OTP_PARAM0_TPGM_2US(x)      (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM0_TPGM_2US_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM0_TPGM_2US_MASK)

#define SOC_OTP_CTRL_OTP_PARAM0_TSQ_MINUS_TRD_MASK (0x3C00U)
#define SOC_OTP_CTRL_OTP_PARAM0_TSQ_MINUS_TRD_SHIFT (10U)
/*! TSQ_MINUS_TRD - Read data access time, the value to enter is tSQ(70ns)-tRD. The default value is based on 32MHz LSB clock. */
#define SOC_OTP_CTRL_OTP_PARAM0_TSQ_MINUS_TRD(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM0_TSQ_MINUS_TRD_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM0_TSQ_MINUS_TRD_MASK)

#define SOC_OTP_CTRL_OTP_PARAM0_TRD_20NS_LOW_MASK (0xC000U)
#define SOC_OTP_CTRL_OTP_PARAM0_TRD_20NS_LOW_SHIFT (14U)
/*! TRD_20NS_LOW - For 28nm. Read pulse width lower two bits(higher two bits are in
 *    OTP_PARAM2_REG[12:11]). The default value is based on 32MHz LSB clock.
 */
#define SOC_OTP_CTRL_OTP_PARAM0_TRD_20NS_LOW(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM0_TRD_20NS_LOW_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM0_TRD_20NS_LOW_MASK)
/*! @} */

/*! @name OTP_PARAM1 - OTP_PARAM1_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_PARAM1_TVHV_OFF_10US_MASK (0x7FFU)
#define SOC_OTP_CTRL_OTP_PARAM1_TVHV_OFF_10US_SHIFT (0U)
/*! TVHV_OFF_10US - Specify the VHV turn OFF time before the OTP read access. The default value is based on 32MHz */
#define SOC_OTP_CTRL_OTP_PARAM1_TVHV_OFF_10US(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM1_TVHV_OFF_10US_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM1_TVHV_OFF_10US_MASK)

#define SOC_OTP_CTRL_OTP_PARAM1_TSU_300NS_MASK   (0xF800U)
#define SOC_OTP_CTRL_OTP_PARAM1_TSU_300NS_SHIFT  (11U)
/*! TSU_300NS - Specify setup time for few of the OTP control signals. The default value is based on 32MHz LSB clock. */
#define SOC_OTP_CTRL_OTP_PARAM1_TSU_300NS(x)     (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM1_TSU_300NS_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM1_TSU_300NS_MASK)
/*! @} */

/*! @name OTP_PARAM2 - OTP_PARAM2_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_PARAM2_TVHV_ON_10US_MASK (0x7FFU)
#define SOC_OTP_CTRL_OTP_PARAM2_TVHV_ON_10US_SHIFT (0U)
/*! TVHV_ON_10US - Specify the VHV turn ON time before the OTP programming access. The default value is based on 32MHz LSB clock. */
#define SOC_OTP_CTRL_OTP_PARAM2_TVHV_ON_10US(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM2_TVHV_ON_10US_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM2_TVHV_ON_10US_MASK)

#define SOC_OTP_CTRL_OTP_PARAM2_TRD_20NS_HIGH_MASK (0x1800U)
#define SOC_OTP_CTRL_OTP_PARAM2_TRD_20NS_HIGH_SHIFT (11U)
/*! TRD_20NS_HIGH - For 28nm, Read pulse width higher two bits (lower two bits are in
 *    OTP_PARAM0_REG[15:14]). The default value is based on 32MHz LSB clock.
 */
#define SOC_OTP_CTRL_OTP_PARAM2_TRD_20NS_HIGH(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_PARAM2_TRD_20NS_HIGH_SHIFT)) & SOC_OTP_CTRL_OTP_PARAM2_TRD_20NS_HIGH_MASK)
/*! @} */

/*! @name OTP_BYPASS_MODE0 - OTP_BYPASS_MODE0_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_SET_OTP_BYPASS_MASK (0x1U)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_SET_OTP_BYPASS_SHIFT (0U)
/*! SET_OTP_BYPASS - SET OTP bypass mode, all the OTP interface control signals are under register control by s/w. */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_SET_OTP_BYPASS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE0_SET_OTP_BYPASS_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE0_SET_OTP_BYPASS_MASK)

#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_SCLK_MASK  (0x40U)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_SCLK_SHIFT (6U)
/*! SCLK - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_SCLK(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE0_SCLK_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE0_SCLK_MASK)

#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_PGM_B_MASK (0x80U)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_PGM_B_SHIFT (7U)
/*! PGM_B - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_PGM_B(x)   (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE0_PGM_B_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE0_PGM_B_MASK)

#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_LOAD_MASK  (0x100U)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_LOAD_SHIFT (8U)
/*! LOAD - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_LOAD(x)    (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE0_LOAD_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE0_LOAD_MASK)

#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_DOUT_MASK (0x4000U)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_DOUT_SHIFT (14U)
/*! MUX_DOUT - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_DOUT(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_DOUT_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_DOUT_MASK)

#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_RD_DONE_MASK (0x8000U)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_RD_DONE_SHIFT (15U)
/*! MUX_RD_DONE - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_RD_DONE(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_RD_DONE_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE0_MUX_RD_DONE_MASK)
/*! @} */

/*! @name OTP_BYPASS_MODE1 - OTP_BYPASS_MODE1_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_BYPASS_MODE1_BIT_ADDRESS_MASK (0x7FU)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE1_BIT_ADDRESS_SHIFT (0U)
/*! BIT_ADDRESS - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE1_BIT_ADDRESS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE1_BIT_ADDRESS_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE1_BIT_ADDRESS_MASK)
/*! @} */

/*! @name OTP_TESTBUS_SEL - OTP_TESTBUS_SEL_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_TESTBUS_SEL_TESTBUS_MASK (0xFU)
#define SOC_OTP_CTRL_OTP_TESTBUS_SEL_TESTBUS_SHIFT (0U)
/*! TESTBUS - OTP Test Bus */
#define SOC_OTP_CTRL_OTP_TESTBUS_SEL_TESTBUS(x)  (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_TESTBUS_SEL_TESTBUS_SHIFT)) & SOC_OTP_CTRL_OTP_TESTBUS_SEL_TESTBUS_MASK)
/*! @} */

/*! @name OTP_TESTBUS - OTP_TESTBUS_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_TESTBUS_TESTBUS_MASK    (0xFFFFU)
#define SOC_OTP_CTRL_OTP_TESTBUS_TESTBUS_SHIFT   (0U)
/*! TESTBUS - OTP Test Bus */
#define SOC_OTP_CTRL_OTP_TESTBUS_TESTBUS(x)      (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_TESTBUS_TESTBUS_SHIFT)) & SOC_OTP_CTRL_OTP_TESTBUS_TESTBUS_MASK)
/*! @} */

/*! @name OTP_BYPASS_MODE2 - OTP_BYPASS_MODE2_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_BYPASS_MODE2_CSB_MASK   (0xFFFFU)
#define SOC_OTP_CTRL_OTP_BYPASS_MODE2_CSB_SHIFT  (0U)
/*! CSB - Note: Only valid in the OTP BYPASS mode (OTP_BYPASS_MODE_REG[0]=1). */
#define SOC_OTP_CTRL_OTP_BYPASS_MODE2_CSB(x)     (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_BYPASS_MODE2_CSB_SHIFT)) & SOC_OTP_CTRL_OTP_BYPASS_MODE2_CSB_MASK)
/*! @} */

/*! @name OTP_RST_B - OTP_RST_B_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_RST_B_OTP_RST_B_MASK    (0xFFFFU)
#define SOC_OTP_CTRL_OTP_RST_B_OTP_RST_B_SHIFT   (0U)
/*! OTP_RST_B - S/W reset bit to RST_B input of each OTP macro */
#define SOC_OTP_CTRL_OTP_RST_B_OTP_RST_B(x)      (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_RST_B_OTP_RST_B_SHIFT)) & SOC_OTP_CTRL_OTP_RST_B_OTP_RST_B_MASK)
/*! @} */

/*! @name OTP_POR_B - OTP_POR_B_REG */
/*! @{ */

#define SOC_OTP_CTRL_OTP_POR_B_OTP_POR_B_MASK    (0xFFFFU)
#define SOC_OTP_CTRL_OTP_POR_B_OTP_POR_B_SHIFT   (0U)
/*! OTP_POR_B - S/W por bit to POR_B input of each OTP macro. */
#define SOC_OTP_CTRL_OTP_POR_B_OTP_POR_B(x)      (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_POR_B_OTP_POR_B_SHIFT)) & SOC_OTP_CTRL_OTP_POR_B_OTP_POR_B_MASK)
/*! @} */

/*! @name OTP_WRITE_LOCK_REG - OTP_WRITE_LOCK_REG (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_LOCK_REG_OTP_WRITE_LOCK_MASK (0x1U)
#define SOC_OTP_CTRL_OTP_WRITE_LOCK_REG_OTP_WRITE_LOCK_SHIFT (0U)
/*! OTP_WRITE_LOCK - "1 sticky bit, once it is set it will stay at set state until POR_ONLY reset to 0. */
#define SOC_OTP_CTRL_OTP_WRITE_LOCK_REG_OTP_WRITE_LOCK(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_LOCK_REG_OTP_WRITE_LOCK_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_LOCK_REG_OTP_WRITE_LOCK_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_15_0 - OTP_WRITE_DIS_REG[15:0] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_15_0_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_15_0_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[15:0]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_15_0_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_15_0_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_15_0_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_31_16 - OTP_WRITE_DIS_REG[31:16] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_31_16_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_31_16_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[31:16]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_31_16_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_31_16_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_31_16_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_47_32 - OTP_WRITE_DIS_REG[47:32] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_47_32_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_47_32_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[47:32]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_47_32_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_47_32_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_47_32_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_63_48 - OTP_WRITE_DIS_REG[63:48] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_63_48_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_63_48_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[63:48]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_63_48_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_63_48_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_63_48_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_79_64 - OTP_WRITE_DIS_REG[79:64] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_79_64_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_79_64_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[79:64]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_79_64_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_79_64_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_79_64_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_95_80 - OTP_WRITE_DIS_REG[95:80] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_95_80_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_95_80_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[95:80]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_95_80_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_95_80_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_95_80_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_111_96 - OTP_WRITE_DIS_REG[111:96] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_111_96_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_111_96_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[111:96]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_111_96_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_111_96_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_111_96_OTP_WRITE_DIS_MASK)
/*! @} */

/*! @name OTP_WRITE_DIS_REG_127_112 - OTP_WRITE_DIS_REG[127:112] (Firecrest) */
/*! @{ */

#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_127_112_OTP_WRITE_DIS_MASK (0xFFFFU)
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_127_112_OTP_WRITE_DIS_SHIFT (0U)
/*! OTP_WRITE_DIS - OTP_Write_Disable bitmap for OTP Line[127:112]. */
#define SOC_OTP_CTRL_OTP_WRITE_DIS_REG_127_112_OTP_WRITE_DIS(x) (((uint16_t)(((uint16_t)(x)) << SOC_OTP_CTRL_OTP_WRITE_DIS_REG_127_112_OTP_WRITE_DIS_SHIFT)) & SOC_OTP_CTRL_OTP_WRITE_DIS_REG_127_112_OTP_WRITE_DIS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SOC_OTP_CTRL_Register_Masks */


/*!
 * @}
 */ /* end of group SOC_OTP_CTRL_Peripheral_Access_Layer */


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


#endif  /* SOC_OTP_CTRL_H_ */

