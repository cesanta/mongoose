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
**         CMSIS Peripheral Access Layer for POWERQUAD
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
 * @file POWERQUAD.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for POWERQUAD
 *
 * CMSIS Peripheral Access Layer for POWERQUAD
 */

#if !defined(POWERQUAD_H_)
#define POWERQUAD_H_                             /**< Symbol preventing repeated inclusion */

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
   -- POWERQUAD Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup POWERQUAD_Peripheral_Access_Layer POWERQUAD Peripheral Access Layer
 * @{
 */

/** POWERQUAD - Size of Registers Arrays */
#define POWERQUAD_GPREG_COUNT                     16u
#define POWERQUAD_COMPREGS_COUNT                  8u

/** POWERQUAD - Register Layout Typedef */
typedef struct {
  __IO uint32_t OUTBASE;                           /**< Base address register for output region, offset: 0x0 */
  __IO uint32_t OUTFORMAT;                         /**< Output format, offset: 0x4 */
  __IO uint32_t TMPBASE;                           /**< Base address register for temp region, offset: 0x8 */
  __IO uint32_t TMPFORMAT;                         /**< Temp format, offset: 0xC */
  __IO uint32_t INABASE;                           /**< Base address register for input A region, offset: 0x10 */
  __IO uint32_t INAFORMAT;                         /**< Input A format, offset: 0x14 */
  __IO uint32_t INBBASE;                           /**< Base address register for input B region, offset: 0x18 */
  __IO uint32_t INBFORMAT;                         /**< Input B format, offset: 0x1C */
       uint8_t RESERVED_0[224];
  __IO uint32_t CONTROL;                           /**< PowerQuad Control register, offset: 0x100 */
  __IO uint32_t LENGTH;                            /**< Length register, offset: 0x104 */
  __IO uint32_t CPPRE;                             /**< Pre-scale register, offset: 0x108 */
  __IO uint32_t MISC;                              /**< Misc register, offset: 0x10C */
  __IO uint32_t CURSORY;                           /**< Cursory register, offset: 0x110 */
       uint8_t RESERVED_1[108];
  __IO uint32_t CORDIC_X;                          /**< Cordic input X register, offset: 0x180 */
  __IO uint32_t CORDIC_Y;                          /**< Cordic input Y register, offset: 0x184 */
  __IO uint32_t CORDIC_Z;                          /**< Cordic input Z register, offset: 0x188 */
  __IO uint32_t ERRSTAT;                           /**< Read/Write register where error statuses are captured (sticky), offset: 0x18C */
  __IO uint32_t INTREN;                            /**< INTERRUPT enable register, offset: 0x190 */
  __IO uint32_t EVENTEN;                           /**< Event Enable register, offset: 0x194 */
  __IO uint32_t INTRSTAT;                          /**< INTERRUPT STATUS register, offset: 0x198 */
       uint8_t RESERVED_2[100];
  __IO uint32_t GPREG[POWERQUAD_GPREG_COUNT];      /**< General purpose register bank N., array offset: 0x200, array step: 0x4 */
  __IO uint32_t COMPREG[POWERQUAD_COMPREGS_COUNT]; /**< Compute register bank, array offset: 0x240, array step: 0x4 */
} POWERQUAD_Type;

/* ----------------------------------------------------------------------------
   -- POWERQUAD Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup POWERQUAD_Register_Masks POWERQUAD Register Masks
 * @{
 */

/*! @name OUTBASE - Base address register for output region */
/*! @{ */

#define POWERQUAD_OUTBASE_OUTBASE_MASK           (0xFFFFFFFFU)
#define POWERQUAD_OUTBASE_OUTBASE_SHIFT          (0U)
/*! OUTBASE - Base address register for the output region */
#define POWERQUAD_OUTBASE_OUTBASE(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_OUTBASE_OUTBASE_SHIFT)) & POWERQUAD_OUTBASE_OUTBASE_MASK)
/*! @} */

/*! @name OUTFORMAT - Output format */
/*! @{ */

#define POWERQUAD_OUTFORMAT_OUT_FORMATINT_MASK   (0x3U)
#define POWERQUAD_OUTFORMAT_OUT_FORMATINT_SHIFT  (0U)
/*! OUT_FORMATINT - Output Internal format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_OUTFORMAT_OUT_FORMATINT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_OUTFORMAT_OUT_FORMATINT_SHIFT)) & POWERQUAD_OUTFORMAT_OUT_FORMATINT_MASK)

#define POWERQUAD_OUTFORMAT_OUT_FORMATEXT_MASK   (0x30U)
#define POWERQUAD_OUTFORMAT_OUT_FORMATEXT_SHIFT  (4U)
/*! OUT_FORMATEXT - Output External format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_OUTFORMAT_OUT_FORMATEXT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_OUTFORMAT_OUT_FORMATEXT_SHIFT)) & POWERQUAD_OUTFORMAT_OUT_FORMATEXT_MASK)

#define POWERQUAD_OUTFORMAT_OUT_SCALER_MASK      (0xFF00U)
#define POWERQUAD_OUTFORMAT_OUT_SCALER_SHIFT     (8U)
/*! OUT_SCALER - Output Scaler value (for scaled 'q31' formats) */
#define POWERQUAD_OUTFORMAT_OUT_SCALER(x)        (((uint32_t)(((uint32_t)(x)) << POWERQUAD_OUTFORMAT_OUT_SCALER_SHIFT)) & POWERQUAD_OUTFORMAT_OUT_SCALER_MASK)
/*! @} */

/*! @name TMPBASE - Base address register for temp region */
/*! @{ */

#define POWERQUAD_TMPBASE_TMPBASE_MASK           (0xFFFFFFFFU)
#define POWERQUAD_TMPBASE_TMPBASE_SHIFT          (0U)
/*! TMPBASE - Base address register for the temporary region */
#define POWERQUAD_TMPBASE_TMPBASE(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_TMPBASE_TMPBASE_SHIFT)) & POWERQUAD_TMPBASE_TMPBASE_MASK)
/*! @} */

/*! @name TMPFORMAT - Temp format */
/*! @{ */

#define POWERQUAD_TMPFORMAT_TMP_FORMATINT_MASK   (0x3U)
#define POWERQUAD_TMPFORMAT_TMP_FORMATINT_SHIFT  (0U)
/*! TMP_FORMATINT - Temp Internal format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_TMPFORMAT_TMP_FORMATINT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_TMPFORMAT_TMP_FORMATINT_SHIFT)) & POWERQUAD_TMPFORMAT_TMP_FORMATINT_MASK)

#define POWERQUAD_TMPFORMAT_TMP_FORMATEXT_MASK   (0x30U)
#define POWERQUAD_TMPFORMAT_TMP_FORMATEXT_SHIFT  (4U)
/*! TMP_FORMATEXT - Temp External format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_TMPFORMAT_TMP_FORMATEXT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_TMPFORMAT_TMP_FORMATEXT_SHIFT)) & POWERQUAD_TMPFORMAT_TMP_FORMATEXT_MASK)

#define POWERQUAD_TMPFORMAT_TMP_SCALER_MASK      (0xFF00U)
#define POWERQUAD_TMPFORMAT_TMP_SCALER_SHIFT     (8U)
/*! TMP_SCALER - Temp Scaler value (for scaled 'q31' formats) */
#define POWERQUAD_TMPFORMAT_TMP_SCALER(x)        (((uint32_t)(((uint32_t)(x)) << POWERQUAD_TMPFORMAT_TMP_SCALER_SHIFT)) & POWERQUAD_TMPFORMAT_TMP_SCALER_MASK)
/*! @} */

/*! @name INABASE - Base address register for input A region */
/*! @{ */

#define POWERQUAD_INABASE_INABASE_MASK           (0xFFFFFFFFU)
#define POWERQUAD_INABASE_INABASE_SHIFT          (0U)
/*! INABASE - Base address register for the input A region */
#define POWERQUAD_INABASE_INABASE(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INABASE_INABASE_SHIFT)) & POWERQUAD_INABASE_INABASE_MASK)
/*! @} */

/*! @name INAFORMAT - Input A format */
/*! @{ */

#define POWERQUAD_INAFORMAT_INA_FORMATINT_MASK   (0x3U)
#define POWERQUAD_INAFORMAT_INA_FORMATINT_SHIFT  (0U)
/*! INA_FORMATINT - Input A Internal format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_INAFORMAT_INA_FORMATINT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INAFORMAT_INA_FORMATINT_SHIFT)) & POWERQUAD_INAFORMAT_INA_FORMATINT_MASK)

#define POWERQUAD_INAFORMAT_INA_FORMATEXT_MASK   (0x30U)
#define POWERQUAD_INAFORMAT_INA_FORMATEXT_SHIFT  (4U)
/*! INA_FORMATEXT - Input A External format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_INAFORMAT_INA_FORMATEXT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INAFORMAT_INA_FORMATEXT_SHIFT)) & POWERQUAD_INAFORMAT_INA_FORMATEXT_MASK)

#define POWERQUAD_INAFORMAT_INA_SCALER_MASK      (0xFF00U)
#define POWERQUAD_INAFORMAT_INA_SCALER_SHIFT     (8U)
/*! INA_SCALER - Input A Scaler value (for scaled 'q31' formats) */
#define POWERQUAD_INAFORMAT_INA_SCALER(x)        (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INAFORMAT_INA_SCALER_SHIFT)) & POWERQUAD_INAFORMAT_INA_SCALER_MASK)
/*! @} */

/*! @name INBBASE - Base address register for input B region */
/*! @{ */

#define POWERQUAD_INBBASE_INBBASE_MASK           (0xFFFFFFFFU)
#define POWERQUAD_INBBASE_INBBASE_SHIFT          (0U)
/*! INBBASE - Base address register for the input B region */
#define POWERQUAD_INBBASE_INBBASE(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INBBASE_INBBASE_SHIFT)) & POWERQUAD_INBBASE_INBBASE_MASK)
/*! @} */

/*! @name INBFORMAT - Input B format */
/*! @{ */

#define POWERQUAD_INBFORMAT_INB_FORMATINT_MASK   (0x3U)
#define POWERQUAD_INBFORMAT_INB_FORMATINT_SHIFT  (0U)
/*! INB_FORMATINT - Input B Internal format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_INBFORMAT_INB_FORMATINT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INBFORMAT_INB_FORMATINT_SHIFT)) & POWERQUAD_INBFORMAT_INB_FORMATINT_MASK)

#define POWERQUAD_INBFORMAT_INB_FORMATEXT_MASK   (0x30U)
#define POWERQUAD_INBFORMAT_INB_FORMATEXT_SHIFT  (4U)
/*! INB_FORMATEXT - Input B External format (00: q15; 01:q31; 10:float) */
#define POWERQUAD_INBFORMAT_INB_FORMATEXT(x)     (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INBFORMAT_INB_FORMATEXT_SHIFT)) & POWERQUAD_INBFORMAT_INB_FORMATEXT_MASK)

#define POWERQUAD_INBFORMAT_INB_SCALER_MASK      (0xFF00U)
#define POWERQUAD_INBFORMAT_INB_SCALER_SHIFT     (8U)
/*! INB_SCALER - Input B Scaler value (for scaled 'q31' formats) */
#define POWERQUAD_INBFORMAT_INB_SCALER(x)        (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INBFORMAT_INB_SCALER_SHIFT)) & POWERQUAD_INBFORMAT_INB_SCALER_MASK)
/*! @} */

/*! @name CONTROL - PowerQuad Control register */
/*! @{ */

#define POWERQUAD_CONTROL_DECODE_OPCODE_MASK     (0xFU)
#define POWERQUAD_CONTROL_DECODE_OPCODE_SHIFT    (0U)
/*! DECODE_OPCODE - opcode specific to decode_machine */
#define POWERQUAD_CONTROL_DECODE_OPCODE(x)       (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CONTROL_DECODE_OPCODE_SHIFT)) & POWERQUAD_CONTROL_DECODE_OPCODE_MASK)

#define POWERQUAD_CONTROL_DECODE_MACHINE_MASK    (0xF0U)
#define POWERQUAD_CONTROL_DECODE_MACHINE_SHIFT   (4U)
/*! DECODE_MACHINE - 0 : Coprocessor , 1 : matrix , 2 : fft , 3 : fir , 4 : stat , 5 : cordic , 6 -15 : NA */
#define POWERQUAD_CONTROL_DECODE_MACHINE(x)      (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CONTROL_DECODE_MACHINE_SHIFT)) & POWERQUAD_CONTROL_DECODE_MACHINE_MASK)

#define POWERQUAD_CONTROL_INST_BUSY_MASK         (0x80000000U)
#define POWERQUAD_CONTROL_INST_BUSY_SHIFT        (31U)
/*! INST_BUSY - Instruction busy signal when high indicates processing is on */
#define POWERQUAD_CONTROL_INST_BUSY(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CONTROL_INST_BUSY_SHIFT)) & POWERQUAD_CONTROL_INST_BUSY_MASK)
/*! @} */

/*! @name LENGTH - Length register */
/*! @{ */

#define POWERQUAD_LENGTH_INST_LENGTH_MASK        (0xFFFFFFFFU)
#define POWERQUAD_LENGTH_INST_LENGTH_SHIFT       (0U)
/*! INST_LENGTH - Length register. When FIR : fir_xlength = inst_length[15:0] , fir_tlength =
 *    inst_len[31:16]. When MTX : rows_a = inst_length[4:0] , cols_a = inst_length[12:8] , cols_b =
 *    inst_length[20:16]
 */
#define POWERQUAD_LENGTH_INST_LENGTH(x)          (((uint32_t)(((uint32_t)(x)) << POWERQUAD_LENGTH_INST_LENGTH_SHIFT)) & POWERQUAD_LENGTH_INST_LENGTH_MASK)
/*! @} */

/*! @name CPPRE - Pre-scale register */
/*! @{ */

#define POWERQUAD_CPPRE_CPPRE_IN_MASK            (0xFFU)
#define POWERQUAD_CPPRE_CPPRE_IN_SHIFT           (0U)
/*! CPPRE_IN - co-processor scaling of input */
#define POWERQUAD_CPPRE_CPPRE_IN(x)              (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CPPRE_CPPRE_IN_SHIFT)) & POWERQUAD_CPPRE_CPPRE_IN_MASK)

#define POWERQUAD_CPPRE_CPPRE_OUT_MASK           (0xFF00U)
#define POWERQUAD_CPPRE_CPPRE_OUT_SHIFT          (8U)
/*! CPPRE_OUT - co-processor fixed point output */
#define POWERQUAD_CPPRE_CPPRE_OUT(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CPPRE_CPPRE_OUT_SHIFT)) & POWERQUAD_CPPRE_CPPRE_OUT_MASK)

#define POWERQUAD_CPPRE_CPPRE_SAT_MASK           (0x10000U)
#define POWERQUAD_CPPRE_CPPRE_SAT_SHIFT          (16U)
/*! CPPRE_SAT - 1 : forces sub-32 bit saturation */
#define POWERQUAD_CPPRE_CPPRE_SAT(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CPPRE_CPPRE_SAT_SHIFT)) & POWERQUAD_CPPRE_CPPRE_SAT_MASK)

#define POWERQUAD_CPPRE_CPPRE_SAT8_MASK          (0x20000U)
#define POWERQUAD_CPPRE_CPPRE_SAT8_SHIFT         (17U)
/*! CPPRE_SAT8 - 0 = 8bits, 1 = 16bits */
#define POWERQUAD_CPPRE_CPPRE_SAT8(x)            (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CPPRE_CPPRE_SAT8_SHIFT)) & POWERQUAD_CPPRE_CPPRE_SAT8_MASK)
/*! @} */

/*! @name MISC - Misc register */
/*! @{ */

#define POWERQUAD_MISC_INST_MISC_MASK            (0xFFFFFFFFU)
#define POWERQUAD_MISC_INST_MISC_SHIFT           (0U)
/*! INST_MISC - Misc register. For Matrix : Used for scale factor */
#define POWERQUAD_MISC_INST_MISC(x)              (((uint32_t)(((uint32_t)(x)) << POWERQUAD_MISC_INST_MISC_SHIFT)) & POWERQUAD_MISC_INST_MISC_MASK)
/*! @} */

/*! @name CURSORY - Cursory register */
/*! @{ */

#define POWERQUAD_CURSORY_CURSORY_MASK           (0x1U)
#define POWERQUAD_CURSORY_CURSORY_SHIFT          (0U)
/*! CURSORY - 1 : Enable cursory mode */
#define POWERQUAD_CURSORY_CURSORY(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CURSORY_CURSORY_SHIFT)) & POWERQUAD_CURSORY_CURSORY_MASK)
/*! @} */

/*! @name CORDIC_X - Cordic input X register */
/*! @{ */

#define POWERQUAD_CORDIC_X_CORDIC_X_MASK         (0xFFFFFFFFU)
#define POWERQUAD_CORDIC_X_CORDIC_X_SHIFT        (0U)
/*! CORDIC_X - Cordic input x */
#define POWERQUAD_CORDIC_X_CORDIC_X(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CORDIC_X_CORDIC_X_SHIFT)) & POWERQUAD_CORDIC_X_CORDIC_X_MASK)
/*! @} */

/*! @name CORDIC_Y - Cordic input Y register */
/*! @{ */

#define POWERQUAD_CORDIC_Y_CORDIC_Y_MASK         (0xFFFFFFFFU)
#define POWERQUAD_CORDIC_Y_CORDIC_Y_SHIFT        (0U)
/*! CORDIC_Y - Cordic input y */
#define POWERQUAD_CORDIC_Y_CORDIC_Y(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CORDIC_Y_CORDIC_Y_SHIFT)) & POWERQUAD_CORDIC_Y_CORDIC_Y_MASK)
/*! @} */

/*! @name CORDIC_Z - Cordic input Z register */
/*! @{ */

#define POWERQUAD_CORDIC_Z_CORDIC_Z_MASK         (0xFFFFFFFFU)
#define POWERQUAD_CORDIC_Z_CORDIC_Z_SHIFT        (0U)
/*! CORDIC_Z - Cordic input z */
#define POWERQUAD_CORDIC_Z_CORDIC_Z(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_CORDIC_Z_CORDIC_Z_SHIFT)) & POWERQUAD_CORDIC_Z_CORDIC_Z_MASK)
/*! @} */

/*! @name ERRSTAT - Read/Write register where error statuses are captured (sticky) */
/*! @{ */

#define POWERQUAD_ERRSTAT_OVERFLOW_MASK          (0x1U)
#define POWERQUAD_ERRSTAT_OVERFLOW_SHIFT         (0U)
/*! OVERFLOW - overflow */
#define POWERQUAD_ERRSTAT_OVERFLOW(x)            (((uint32_t)(((uint32_t)(x)) << POWERQUAD_ERRSTAT_OVERFLOW_SHIFT)) & POWERQUAD_ERRSTAT_OVERFLOW_MASK)

#define POWERQUAD_ERRSTAT_NAN_MASK               (0x2U)
#define POWERQUAD_ERRSTAT_NAN_SHIFT              (1U)
/*! NAN - nan */
#define POWERQUAD_ERRSTAT_NAN(x)                 (((uint32_t)(((uint32_t)(x)) << POWERQUAD_ERRSTAT_NAN_SHIFT)) & POWERQUAD_ERRSTAT_NAN_MASK)

#define POWERQUAD_ERRSTAT_FIXEDOVERFLOW_MASK     (0x4U)
#define POWERQUAD_ERRSTAT_FIXEDOVERFLOW_SHIFT    (2U)
/*! FIXEDOVERFLOW - fixed_pt_overflow */
#define POWERQUAD_ERRSTAT_FIXEDOVERFLOW(x)       (((uint32_t)(((uint32_t)(x)) << POWERQUAD_ERRSTAT_FIXEDOVERFLOW_SHIFT)) & POWERQUAD_ERRSTAT_FIXEDOVERFLOW_MASK)

#define POWERQUAD_ERRSTAT_UNDERFLOW_MASK         (0x8U)
#define POWERQUAD_ERRSTAT_UNDERFLOW_SHIFT        (3U)
/*! UNDERFLOW - underflow */
#define POWERQUAD_ERRSTAT_UNDERFLOW(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_ERRSTAT_UNDERFLOW_SHIFT)) & POWERQUAD_ERRSTAT_UNDERFLOW_MASK)

#define POWERQUAD_ERRSTAT_BUSERROR_MASK          (0x10U)
#define POWERQUAD_ERRSTAT_BUSERROR_SHIFT         (4U)
/*! BUSERROR - bus_error */
#define POWERQUAD_ERRSTAT_BUSERROR(x)            (((uint32_t)(((uint32_t)(x)) << POWERQUAD_ERRSTAT_BUSERROR_SHIFT)) & POWERQUAD_ERRSTAT_BUSERROR_MASK)
/*! @} */

/*! @name INTREN - INTERRUPT enable register */
/*! @{ */

#define POWERQUAD_INTREN_INTR_OFLOW_MASK         (0x1U)
#define POWERQUAD_INTREN_INTR_OFLOW_SHIFT        (0U)
/*! INTR_OFLOW - 1 : Enable interrupt on Floating point overflow */
#define POWERQUAD_INTREN_INTR_OFLOW(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTREN_INTR_OFLOW_SHIFT)) & POWERQUAD_INTREN_INTR_OFLOW_MASK)

#define POWERQUAD_INTREN_INTR_NAN_MASK           (0x2U)
#define POWERQUAD_INTREN_INTR_NAN_SHIFT          (1U)
/*! INTR_NAN - 1 : Enable interrupt on Floating point NaN */
#define POWERQUAD_INTREN_INTR_NAN(x)             (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTREN_INTR_NAN_SHIFT)) & POWERQUAD_INTREN_INTR_NAN_MASK)

#define POWERQUAD_INTREN_INTR_FIXED_MASK         (0x4U)
#define POWERQUAD_INTREN_INTR_FIXED_SHIFT        (2U)
/*! INTR_FIXED - 1: Enable interrupt on Fixed point Overflow */
#define POWERQUAD_INTREN_INTR_FIXED(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTREN_INTR_FIXED_SHIFT)) & POWERQUAD_INTREN_INTR_FIXED_MASK)

#define POWERQUAD_INTREN_INTR_UFLOW_MASK         (0x8U)
#define POWERQUAD_INTREN_INTR_UFLOW_SHIFT        (3U)
/*! INTR_UFLOW - 1 : Enable interrupt on Subnormal truncation */
#define POWERQUAD_INTREN_INTR_UFLOW(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTREN_INTR_UFLOW_SHIFT)) & POWERQUAD_INTREN_INTR_UFLOW_MASK)

#define POWERQUAD_INTREN_INTR_BERR_MASK          (0x10U)
#define POWERQUAD_INTREN_INTR_BERR_SHIFT         (4U)
/*! INTR_BERR - 1: Enable interrupt on AHBM Buss Error */
#define POWERQUAD_INTREN_INTR_BERR(x)            (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTREN_INTR_BERR_SHIFT)) & POWERQUAD_INTREN_INTR_BERR_MASK)

#define POWERQUAD_INTREN_INTR_COMP_MASK          (0x80U)
#define POWERQUAD_INTREN_INTR_COMP_SHIFT         (7U)
/*! INTR_COMP - 1: Enable interrupt on instruction completion */
#define POWERQUAD_INTREN_INTR_COMP(x)            (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTREN_INTR_COMP_SHIFT)) & POWERQUAD_INTREN_INTR_COMP_MASK)
/*! @} */

/*! @name EVENTEN - Event Enable register */
/*! @{ */

#define POWERQUAD_EVENTEN_EVENT_OFLOW_MASK       (0x1U)
#define POWERQUAD_EVENTEN_EVENT_OFLOW_SHIFT      (0U)
/*! EVENT_OFLOW - 1 : Enable event trigger on Floating point overflow */
#define POWERQUAD_EVENTEN_EVENT_OFLOW(x)         (((uint32_t)(((uint32_t)(x)) << POWERQUAD_EVENTEN_EVENT_OFLOW_SHIFT)) & POWERQUAD_EVENTEN_EVENT_OFLOW_MASK)

#define POWERQUAD_EVENTEN_EVENT_NAN_MASK         (0x2U)
#define POWERQUAD_EVENTEN_EVENT_NAN_SHIFT        (1U)
/*! EVENT_NAN - 1 : Enable event trigger on Floating point NaN */
#define POWERQUAD_EVENTEN_EVENT_NAN(x)           (((uint32_t)(((uint32_t)(x)) << POWERQUAD_EVENTEN_EVENT_NAN_SHIFT)) & POWERQUAD_EVENTEN_EVENT_NAN_MASK)

#define POWERQUAD_EVENTEN_EVENT_FIXED_MASK       (0x4U)
#define POWERQUAD_EVENTEN_EVENT_FIXED_SHIFT      (2U)
/*! EVENT_FIXED - 1: Enable event trigger on Fixed point Overflow */
#define POWERQUAD_EVENTEN_EVENT_FIXED(x)         (((uint32_t)(((uint32_t)(x)) << POWERQUAD_EVENTEN_EVENT_FIXED_SHIFT)) & POWERQUAD_EVENTEN_EVENT_FIXED_MASK)

#define POWERQUAD_EVENTEN_EVENT_UFLOW_MASK       (0x8U)
#define POWERQUAD_EVENTEN_EVENT_UFLOW_SHIFT      (3U)
/*! EVENT_UFLOW - 1 : Enable event trigger on Subnormal truncation */
#define POWERQUAD_EVENTEN_EVENT_UFLOW(x)         (((uint32_t)(((uint32_t)(x)) << POWERQUAD_EVENTEN_EVENT_UFLOW_SHIFT)) & POWERQUAD_EVENTEN_EVENT_UFLOW_MASK)

#define POWERQUAD_EVENTEN_EVENT_BERR_MASK        (0x10U)
#define POWERQUAD_EVENTEN_EVENT_BERR_SHIFT       (4U)
/*! EVENT_BERR - 1: Enable event trigger on AHBM Buss Error */
#define POWERQUAD_EVENTEN_EVENT_BERR(x)          (((uint32_t)(((uint32_t)(x)) << POWERQUAD_EVENTEN_EVENT_BERR_SHIFT)) & POWERQUAD_EVENTEN_EVENT_BERR_MASK)

#define POWERQUAD_EVENTEN_EVENT_COMP_MASK        (0x80U)
#define POWERQUAD_EVENTEN_EVENT_COMP_SHIFT       (7U)
/*! EVENT_COMP - 1: Enable event trigger on instruction completion */
#define POWERQUAD_EVENTEN_EVENT_COMP(x)          (((uint32_t)(((uint32_t)(x)) << POWERQUAD_EVENTEN_EVENT_COMP_SHIFT)) & POWERQUAD_EVENTEN_EVENT_COMP_MASK)
/*! @} */

/*! @name INTRSTAT - INTERRUPT STATUS register */
/*! @{ */

#define POWERQUAD_INTRSTAT_INTR_STAT_MASK        (0x1U)
#define POWERQUAD_INTRSTAT_INTR_STAT_SHIFT       (0U)
/*! INTR_STAT - Intr status ( 1 bit to indicate interrupt captured, 0 means no new interrupt), write any value will clear this bit */
#define POWERQUAD_INTRSTAT_INTR_STAT(x)          (((uint32_t)(((uint32_t)(x)) << POWERQUAD_INTRSTAT_INTR_STAT_SHIFT)) & POWERQUAD_INTRSTAT_INTR_STAT_MASK)
/*! @} */

/*! @name GPREG - General purpose register bank N. */
/*! @{ */

#define POWERQUAD_GPREG_GPREG_MASK               (0xFFFFFFFFU)
#define POWERQUAD_GPREG_GPREG_SHIFT              (0U)
/*! GPREG - General purpose register bank */
#define POWERQUAD_GPREG_GPREG(x)                 (((uint32_t)(((uint32_t)(x)) << POWERQUAD_GPREG_GPREG_SHIFT)) & POWERQUAD_GPREG_GPREG_MASK)
/*! @} */

/*! @name COMPREGS_COMPREG - Compute register bank */
/*! @{ */

#define POWERQUAD_COMPREGS_COMPREG_COMPREG_MASK  (0xFFFFFFFFU)
#define POWERQUAD_COMPREGS_COMPREG_COMPREG_SHIFT (0U)
/*! COMPREG - Compute register bank */
#define POWERQUAD_COMPREGS_COMPREG_COMPREG(x)    (((uint32_t)(((uint32_t)(x)) << POWERQUAD_COMPREGS_COMPREG_COMPREG_SHIFT)) & POWERQUAD_COMPREGS_COMPREG_COMPREG_MASK)
/*! @} */

/* The count of POWERQUAD_COMPREGS_COMPREG */
#define POWERQUAD_COMPREGS_COMPREG_COUNT         (8U)


/*!
 * @}
 */ /* end of group POWERQUAD_Register_Masks */


/*!
 * @}
 */ /* end of group POWERQUAD_Peripheral_Access_Layer */


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


#endif  /* POWERQUAD_H_ */

