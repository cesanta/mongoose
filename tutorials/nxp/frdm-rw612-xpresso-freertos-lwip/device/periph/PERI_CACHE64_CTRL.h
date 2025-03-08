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
**         CMSIS Peripheral Access Layer for CACHE64_CTRL
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
 * @file CACHE64_CTRL.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CACHE64_CTRL
 *
 * CMSIS Peripheral Access Layer for CACHE64_CTRL
 */

#if !defined(CACHE64_CTRL_H_)
#define CACHE64_CTRL_H_                          /**< Symbol preventing repeated inclusion */

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
   -- CACHE64_CTRL Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CACHE64_CTRL_Peripheral_Access_Layer CACHE64_CTRL Peripheral Access Layer
 * @{
 */

/** CACHE64_CTRL - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[2048];
  __IO uint32_t CCR;                               /**< Cache control register, offset: 0x800 */
  __IO uint32_t CLCR;                              /**< Cache line control register, offset: 0x804 */
  __IO uint32_t CSAR;                              /**< Cache search address register, offset: 0x808 */
  __IO uint32_t CCVR;                              /**< Cache read/write value register, offset: 0x80C */
} CACHE64_CTRL_Type;

/* ----------------------------------------------------------------------------
   -- CACHE64_CTRL Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CACHE64_CTRL_Register_Masks CACHE64_CTRL Register Masks
 * @{
 */

/*! @name CCR - Cache control register */
/*! @{ */

#define CACHE64_CTRL_CCR_ENCACHE_MASK            (0x1U)
#define CACHE64_CTRL_CCR_ENCACHE_SHIFT           (0U)
/*! ENCACHE - Cache enable
 *  0b0..Cache disabled
 *  0b1..Cache enabled
 */
#define CACHE64_CTRL_CCR_ENCACHE(x)              (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_ENCACHE_SHIFT)) & CACHE64_CTRL_CCR_ENCACHE_MASK)

#define CACHE64_CTRL_CCR_ENWRBUF_MASK            (0x2U)
#define CACHE64_CTRL_CCR_ENWRBUF_SHIFT           (1U)
/*! ENWRBUF - Enable Write Buffer
 *  0b0..Write buffer disabled
 *  0b1..Write buffer enabled
 */
#define CACHE64_CTRL_CCR_ENWRBUF(x)              (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_ENWRBUF_SHIFT)) & CACHE64_CTRL_CCR_ENWRBUF_MASK)

#define CACHE64_CTRL_CCR_INVW0_MASK              (0x1000000U)
#define CACHE64_CTRL_CCR_INVW0_SHIFT             (24U)
/*! INVW0 - Invalidate Way 0
 *  0b0..No operation
 *  0b1..When setting the GO bit, invalidate all lines in way 0.
 */
#define CACHE64_CTRL_CCR_INVW0(x)                (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_INVW0_SHIFT)) & CACHE64_CTRL_CCR_INVW0_MASK)

#define CACHE64_CTRL_CCR_PUSHW0_MASK             (0x2000000U)
#define CACHE64_CTRL_CCR_PUSHW0_SHIFT            (25U)
/*! PUSHW0 - Push Way 0
 *  0b0..No operation
 *  0b1..When setting the GO bit, push all modified lines in way 0
 */
#define CACHE64_CTRL_CCR_PUSHW0(x)               (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_PUSHW0_SHIFT)) & CACHE64_CTRL_CCR_PUSHW0_MASK)

#define CACHE64_CTRL_CCR_INVW1_MASK              (0x4000000U)
#define CACHE64_CTRL_CCR_INVW1_SHIFT             (26U)
/*! INVW1 - Invalidate Way 1
 *  0b0..No operation
 *  0b1..When setting the GO bit, invalidate all lines in way 1
 */
#define CACHE64_CTRL_CCR_INVW1(x)                (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_INVW1_SHIFT)) & CACHE64_CTRL_CCR_INVW1_MASK)

#define CACHE64_CTRL_CCR_PUSHW1_MASK             (0x8000000U)
#define CACHE64_CTRL_CCR_PUSHW1_SHIFT            (27U)
/*! PUSHW1 - Push Way 1
 *  0b0..No operation
 *  0b1..When setting the GO bit, push all modified lines in way 1
 */
#define CACHE64_CTRL_CCR_PUSHW1(x)               (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_PUSHW1_SHIFT)) & CACHE64_CTRL_CCR_PUSHW1_MASK)

#define CACHE64_CTRL_CCR_GO_MASK                 (0x80000000U)
#define CACHE64_CTRL_CCR_GO_SHIFT                (31U)
/*! GO - Initiate Cache Command
 *  0b0..Write: no effect. Read: no cache command active.
 *  0b1..Write: initiate command indicated by bits 27-24. Read: cache command active.
 */
#define CACHE64_CTRL_CCR_GO(x)                   (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCR_GO_SHIFT)) & CACHE64_CTRL_CCR_GO_MASK)
/*! @} */

/*! @name CLCR - Cache line control register */
/*! @{ */

#define CACHE64_CTRL_CLCR_LGO_MASK               (0x1U)
#define CACHE64_CTRL_CLCR_LGO_SHIFT              (0U)
/*! LGO - Initiate Cache Line Command
 *  0b0..Write: no effect. Read: no line command active.
 *  0b1..Write: initiate line command indicated by bits 27-24. Read: line command active.
 */
#define CACHE64_CTRL_CLCR_LGO(x)                 (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LGO_SHIFT)) & CACHE64_CTRL_CLCR_LGO_MASK)

#define CACHE64_CTRL_CLCR_CACHEADDR_MASK         (0x3FFCU)
#define CACHE64_CTRL_CLCR_CACHEADDR_SHIFT        (2U)
/*! CACHEADDR - Cache address */
#define CACHE64_CTRL_CLCR_CACHEADDR(x)           (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_CACHEADDR_SHIFT)) & CACHE64_CTRL_CLCR_CACHEADDR_MASK)

#define CACHE64_CTRL_CLCR_WSEL_MASK              (0x4000U)
#define CACHE64_CTRL_CLCR_WSEL_SHIFT             (14U)
/*! WSEL - Way select
 *  0b0..Way 0
 *  0b1..Way 1
 */
#define CACHE64_CTRL_CLCR_WSEL(x)                (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_WSEL_SHIFT)) & CACHE64_CTRL_CLCR_WSEL_MASK)

#define CACHE64_CTRL_CLCR_TDSEL_MASK             (0x10000U)
#define CACHE64_CTRL_CLCR_TDSEL_SHIFT            (16U)
/*! TDSEL - Tag/Data Select
 *  0b0..Data
 *  0b1..Tag
 */
#define CACHE64_CTRL_CLCR_TDSEL(x)               (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_TDSEL_SHIFT)) & CACHE64_CTRL_CLCR_TDSEL_MASK)

#define CACHE64_CTRL_CLCR_LCIVB_MASK             (0x100000U)
#define CACHE64_CTRL_CLCR_LCIVB_SHIFT            (20U)
/*! LCIVB - Line Command Initial Valid Bit */
#define CACHE64_CTRL_CLCR_LCIVB(x)               (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LCIVB_SHIFT)) & CACHE64_CTRL_CLCR_LCIVB_MASK)

#define CACHE64_CTRL_CLCR_LCIMB_MASK             (0x200000U)
#define CACHE64_CTRL_CLCR_LCIMB_SHIFT            (21U)
/*! LCIMB - Line Command Initial Modified Bit */
#define CACHE64_CTRL_CLCR_LCIMB(x)               (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LCIMB_SHIFT)) & CACHE64_CTRL_CLCR_LCIMB_MASK)

#define CACHE64_CTRL_CLCR_LCWAY_MASK             (0x400000U)
#define CACHE64_CTRL_CLCR_LCWAY_SHIFT            (22U)
/*! LCWAY - Line Command Way */
#define CACHE64_CTRL_CLCR_LCWAY(x)               (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LCWAY_SHIFT)) & CACHE64_CTRL_CLCR_LCWAY_MASK)

#define CACHE64_CTRL_CLCR_LCMD_MASK              (0x3000000U)
#define CACHE64_CTRL_CLCR_LCMD_SHIFT             (24U)
/*! LCMD - Line Command
 *  0b00..Search and read or write
 *  0b01..Invalidate
 *  0b10..Push
 *  0b11..Clear
 */
#define CACHE64_CTRL_CLCR_LCMD(x)                (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LCMD_SHIFT)) & CACHE64_CTRL_CLCR_LCMD_MASK)

#define CACHE64_CTRL_CLCR_LADSEL_MASK            (0x4000000U)
#define CACHE64_CTRL_CLCR_LADSEL_SHIFT           (26U)
/*! LADSEL - Line Address Select
 *  0b0..Cache address
 *  0b1..Physical address
 */
#define CACHE64_CTRL_CLCR_LADSEL(x)              (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LADSEL_SHIFT)) & CACHE64_CTRL_CLCR_LADSEL_MASK)

#define CACHE64_CTRL_CLCR_LACC_MASK              (0x8000000U)
#define CACHE64_CTRL_CLCR_LACC_SHIFT             (27U)
/*! LACC - Line access type
 *  0b0..Read
 *  0b1..Write
 */
#define CACHE64_CTRL_CLCR_LACC(x)                (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CLCR_LACC_SHIFT)) & CACHE64_CTRL_CLCR_LACC_MASK)
/*! @} */

/*! @name CSAR - Cache search address register */
/*! @{ */

#define CACHE64_CTRL_CSAR_LGO_MASK               (0x1U)
#define CACHE64_CTRL_CSAR_LGO_SHIFT              (0U)
/*! LGO - Initiate Cache Line Command
 *  0b0..Write: no effect. Read: no line command active.
 *  0b1..Write: initiate line command indicated by bits CLCR[27:24]. Read: line command active.
 */
#define CACHE64_CTRL_CSAR_LGO(x)                 (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CSAR_LGO_SHIFT)) & CACHE64_CTRL_CSAR_LGO_MASK)

#define CACHE64_CTRL_CSAR_PHYADDR27_1_MASK       (0xFFFFFFEU)
#define CACHE64_CTRL_CSAR_PHYADDR27_1_SHIFT      (1U)
/*! PHYADDR27_1 - Physical Address */
#define CACHE64_CTRL_CSAR_PHYADDR27_1(x)         (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CSAR_PHYADDR27_1_SHIFT)) & CACHE64_CTRL_CSAR_PHYADDR27_1_MASK)

#define CACHE64_CTRL_CSAR_PHYADDR31_29_MASK      (0xE0000000U)
#define CACHE64_CTRL_CSAR_PHYADDR31_29_SHIFT     (29U)
/*! PHYADDR31_29 - Physical Address */
#define CACHE64_CTRL_CSAR_PHYADDR31_29(x)        (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CSAR_PHYADDR31_29_SHIFT)) & CACHE64_CTRL_CSAR_PHYADDR31_29_MASK)
/*! @} */

/*! @name CCVR - Cache read/write value register */
/*! @{ */

#define CACHE64_CTRL_CCVR_DATA_MASK              (0xFFFFFFFFU)
#define CACHE64_CTRL_CCVR_DATA_SHIFT             (0U)
/*! DATA - Cache read/write Data */
#define CACHE64_CTRL_CCVR_DATA(x)                (((uint32_t)(((uint32_t)(x)) << CACHE64_CTRL_CCVR_DATA_SHIFT)) & CACHE64_CTRL_CCVR_DATA_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CACHE64_CTRL_Register_Masks */


/*!
 * @}
 */ /* end of group CACHE64_CTRL_Peripheral_Access_Layer */


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


#endif  /* CACHE64_CTRL_H_ */

