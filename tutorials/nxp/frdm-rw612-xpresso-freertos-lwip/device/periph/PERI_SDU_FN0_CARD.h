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
**         CMSIS Peripheral Access Layer for SDU_FN0_CARD
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
 * @file SDU_FN0_CARD.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SDU_FN0_CARD
 *
 * CMSIS Peripheral Access Layer for SDU_FN0_CARD
 */

#if !defined(SDU_FN0_CARD_H_)
#define SDU_FN0_CARD_H_                          /**< Symbol preventing repeated inclusion */

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
   -- SDU_FN0_CARD Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDU_FN0_CARD_Peripheral_Access_Layer SDU_FN0_CARD Peripheral Access Layer
 * @{
 */

/** SDU_FN0_CARD - Register Layout Typedef */
typedef struct {
  __IO uint8_t CCCR;                               /**< CCCR/SDIO Revision, offset: 0x0 */
  __IO uint8_t VER;                                /**< SD Specification Revision, offset: 0x1 */
  __I  uint8_t IO_ENABLE;                          /**< I/O Enable, offset: 0x2 */
  __I  uint8_t IO_READY;                           /**< I/O Function Ready, offset: 0x3 */
  __I  uint8_t INT_ENABLE;                         /**< Interrupt Enable, offset: 0x4 */
  __I  uint8_t INT_PENDING;                        /**< Interrupt Pending, offset: 0x5 */
  __I  uint8_t IO_ABORT;                           /**< I/O Abort, offset: 0x6 */
  __IO uint8_t BUS_INTF_CTRL;                      /**< Bus Interface Control, offset: 0x7 */
  __I  uint8_t CAPAB;                              /**< Card Capability, offset: 0x8 */
  __I  uint8_t FUNC0_CIS_0;                        /**< Function 0 CIS Pointer 0, offset: 0x9 */
  __I  uint8_t FUNC0_CIS_1;                        /**< Function 0 CIS Pointer 1, offset: 0xA */
  __I  uint8_t FUNC0_CIS_2;                        /**< Function 0 CIS Pointer 2, offset: 0xB */
  __I  uint8_t BUS_SUSP;                           /**< Bus Suspend, offset: 0xC */
  __I  uint8_t BUS_SEL;                            /**< Function Select, offset: 0xD */
  __I  uint8_t EXEC;                               /**< Execute Flags, offset: 0xE */
  __I  uint8_t READY;                              /**< Ready Flags, offset: 0xF */
  __I  uint8_t FN0_BLOCK_SIZE_0;                   /**< Function 0 Block Size 0, offset: 0x10 */
  __I  uint8_t FN0_BLOCK_SIZE_1;                   /**< Function 0 Block Size 1, offset: 0x11 */
  __IO uint8_t POWER_CONTROL;                      /**< Power Control, offset: 0x12 */
  __IO uint8_t BUS_SPEED_SELECT;                   /**< Bus Speed Select, offset: 0x13 */
  __IO uint8_t UHS_SUPPORT;                        /**< UHS-I Support, offset: 0x14 */
  __IO uint8_t DRIVER_STR;                         /**< Driver Strength, offset: 0x15 */
  __IO uint8_t INTERRUPT_EXT;                      /**< Interrupt Extension, offset: 0x16 */
       uint8_t RESERVED_0[117];
  __IO uint8_t CARD_CTRL1;                         /**< Card Control 1, offset: 0x8C */
  __IO uint8_t CARD_CTRL2;                         /**< Card Control 2, offset: 0x8D */
  __IO uint8_t CMD19_CTRL1;                        /**< CMD19 Control1, offset: 0x8E */
  __IO uint8_t CMD19_CTRL2;                        /**< CMD19 Control2, offset: 0x8F */
  __I  uint8_t FUNC_CARD_INT;                      /**< Function Card Interrupt, offset: 0x90 */
  __IO uint8_t FUNC0_CARD_INTMASK;                 /**< Function Card Interrupt Mask, offset: 0x91 */
  __IO uint8_t DEV_SLEEP;                          /**< Device Sleep, offset: 0x92 */
  __IO uint8_t CARD_CTRL3;                         /**< Card Control 3, offset: 0x93 */
  __IO uint8_t FN0_CARD_INTMASK0;                  /**< Function 0 Card Interrupt Mask 0, offset: 0x94 */
  __IO uint8_t FN0_CARD_INTMASK1;                  /**< Function 0 Card Interrupt Mask 1, offset: 0x95 */
  __IO uint8_t FN0_CARD_INTRSR0;                   /**< Function 0 Card Interrupt Reset Select 0, offset: 0x96 */
  __IO uint8_t FN0_CARD_INTRSR1;                   /**< Function 0 Card Interrupt Reset Select 1, offset: 0x97 */
  __I  uint8_t FN0_CARD_INTSTATUS0;                /**< Function 0 Card Interrupt Status 0, offset: 0x98 */
  __I  uint8_t FN0_CARD_INTSTATUS1;                /**< Function 0 Card Interrupt Status 1, offset: 0x99 */
  __I  uint8_t FN0_CARD_ACTVINTMASK0;              /**< Function 0 Card Active Interrupt Mask 0, offset: 0x9A */
  __I  uint8_t FN0_CARD_ACTVINTMASK1;              /**< Function 0 Card Active Interrupt Mask 1, offset: 0x9B */
  __IO uint8_t CARD_CTRL4;                         /**< Card Control 4, offset: 0x9C */
  __IO uint8_t CARD_CTRL5;                         /**< Card Control 5, offset: 0x9D */
  __IO uint8_t CARD_CTRL6;                         /**< Card Control 6, offset: 0x9E */
  __IO uint8_t CARD_CTRL7;                         /**< Card Control 7, offset: 0x9F */
} SDU_FN0_CARD_Type;

/* ----------------------------------------------------------------------------
   -- SDU_FN0_CARD Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDU_FN0_CARD_Register_Masks SDU_FN0_CARD Register Masks
 * @{
 */

/*! @name CCCR - CCCR/SDIO Revision */
/*! @{ */

#define SDU_FN0_CARD_CCCR_CCCR_REV_MASK          (0xFU)
#define SDU_FN0_CARD_CCCR_CCCR_REV_SHIFT         (0U)
/*! CCCR_REV - CCCR Format Version Number Version of the CCCR format that this card supports. */
#define SDU_FN0_CARD_CCCR_CCCR_REV(x)            (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CCCR_CCCR_REV_SHIFT)) & SDU_FN0_CARD_CCCR_CCCR_REV_MASK)

#define SDU_FN0_CARD_CCCR_SDIO_REV_MASK          (0xF0U)
#define SDU_FN0_CARD_CCCR_SDIO_REV_SHIFT         (4U)
/*! SDIO_REV - SDIO Specification Revision Number Version of the SDIO specification that this card supports. */
#define SDU_FN0_CARD_CCCR_SDIO_REV(x)            (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CCCR_SDIO_REV_SHIFT)) & SDU_FN0_CARD_CCCR_SDIO_REV_MASK)
/*! @} */

/*! @name VER - SD Specification Revision */
/*! @{ */

#define SDU_FN0_CARD_VER_SD_PHY_REV_MASK         (0xFU)
#define SDU_FN0_CARD_VER_SD_PHY_REV_SHIFT        (0U)
/*! SD_PHY_REV - SD Format Version Number Version of the SD Physical specification that this card supports. */
#define SDU_FN0_CARD_VER_SD_PHY_REV(x)           (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_VER_SD_PHY_REV_SHIFT)) & SDU_FN0_CARD_VER_SD_PHY_REV_MASK)
/*! @} */

/*! @name IO_ENABLE - I/O Enable */
/*! @{ */

#define SDU_FN0_CARD_IO_ENABLE_IOE_MASK          (0xFEU)
#define SDU_FN0_CARD_IO_ENABLE_IOE_SHIFT         (1U)
/*! IOE - Function I/O enable 0 = function disabled 1 = function enabled */
#define SDU_FN0_CARD_IO_ENABLE_IOE(x)            (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_IO_ENABLE_IOE_SHIFT)) & SDU_FN0_CARD_IO_ENABLE_IOE_MASK)
/*! @} */

/*! @name IO_READY - I/O Function Ready */
/*! @{ */

#define SDU_FN0_CARD_IO_READY_FN_IO_RDY_MASK     (0xFEU)
#define SDU_FN0_CARD_IO_READY_FN_IO_RDY_SHIFT    (1U)
/*! FN_IO_RDY - Function I/O ready 0 = function not ready to operate 1 = function ready to operate */
#define SDU_FN0_CARD_IO_READY_FN_IO_RDY(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_IO_READY_FN_IO_RDY_SHIFT)) & SDU_FN0_CARD_IO_READY_FN_IO_RDY_MASK)
/*! @} */

/*! @name INT_ENABLE - Interrupt Enable */
/*! @{ */

#define SDU_FN0_CARD_INT_ENABLE_HOST_IEN_MASK    (0x1U)
#define SDU_FN0_CARD_INT_ENABLE_HOST_IEN_SHIFT   (0U)
/*! HOST_IEN - Host Interrupt Enable 0 = no interrupts from this card sent to host 1 = any function's interrupt sent to host */
#define SDU_FN0_CARD_INT_ENABLE_HOST_IEN(x)      (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_INT_ENABLE_HOST_IEN_SHIFT)) & SDU_FN0_CARD_INT_ENABLE_HOST_IEN_MASK)

#define SDU_FN0_CARD_INT_ENABLE_IEN_MASK         (0xFEU)
#define SDU_FN0_CARD_INT_ENABLE_IEN_SHIFT        (1U)
/*! IEN - Function Interrupt Enable 0 = interrupt from this function not sent to host 1 = interrupt
 *    from this function sent to host (host_ien must also be set)
 */
#define SDU_FN0_CARD_INT_ENABLE_IEN(x)           (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_INT_ENABLE_IEN_SHIFT)) & SDU_FN0_CARD_INT_ENABLE_IEN_MASK)
/*! @} */

/*! @name INT_PENDING - Interrupt Pending */
/*! @{ */

#define SDU_FN0_CARD_INT_PENDING_FN_INT_HOST_MASK (0xFEU)
#define SDU_FN0_CARD_INT_PENDING_FN_INT_HOST_SHIFT (1U)
/*! FN_INT_HOST - Function Interrupt Pending 0 = no interrupts pending from this function 1 =
 *    interrupt pending If the ien[x] or host_ien bits are not set in INT_ENABLE, the host will not
 *    receive the pending interrupt.
 */
#define SDU_FN0_CARD_INT_PENDING_FN_INT_HOST(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_INT_PENDING_FN_INT_HOST_SHIFT)) & SDU_FN0_CARD_INT_PENDING_FN_INT_HOST_MASK)
/*! @} */

/*! @name IO_ABORT - I/O Abort */
/*! @{ */

#define SDU_FN0_CARD_IO_ABORT_ABORT_SEL_MASK     (0x7U)
#define SDU_FN0_CARD_IO_ABORT_ABORT_SEL_SHIFT    (0U)
/*! ABORT_SEL - Abort Select To abort an I/O read or write to free the SD bus, the function that is
 *    currently transferring data must be addressed.
 */
#define SDU_FN0_CARD_IO_ABORT_ABORT_SEL(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_IO_ABORT_ABORT_SEL_SHIFT)) & SDU_FN0_CARD_IO_ABORT_ABORT_SEL_MASK)

#define SDU_FN0_CARD_IO_ABORT_IO_CARD_RST_MASK   (0x8U)
#define SDU_FN0_CARD_IO_ABORT_IO_CARD_RST_SHIFT  (3U)
/*! IO_CARD_RST - I/O Card Reset If host sets this bit, an interrupt is generated to the card. */
#define SDU_FN0_CARD_IO_ABORT_IO_CARD_RST(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_IO_ABORT_IO_CARD_RST_SHIFT)) & SDU_FN0_CARD_IO_ABORT_IO_CARD_RST_MASK)
/*! @} */

/*! @name BUS_INTF_CTRL - Bus Interface Control */
/*! @{ */

#define SDU_FN0_CARD_BUS_INTF_CTRL_BUS_WIDTH_MASK (0x3U)
#define SDU_FN0_CARD_BUS_INTF_CTRL_BUS_WIDTH_SHIFT (0U)
/*! BUS_WIDTH - Bus Width Data bus width used for data transfer 00 = 1 bit 10 = 4 bits All
 *    full-speed SDIO cards support both 1-bit and 4-bit bus.
 */
#define SDU_FN0_CARD_BUS_INTF_CTRL_BUS_WIDTH(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_INTF_CTRL_BUS_WIDTH_SHIFT)) & SDU_FN0_CARD_BUS_INTF_CTRL_BUS_WIDTH_MASK)

#define SDU_FN0_CARD_BUS_INTF_CTRL_ECSI_MASK     (0x20U)
#define SDU_FN0_CARD_BUS_INTF_CTRL_ECSI_SHIFT    (5U)
/*! ECSI - ecsi */
#define SDU_FN0_CARD_BUS_INTF_CTRL_ECSI(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_INTF_CTRL_ECSI_SHIFT)) & SDU_FN0_CARD_BUS_INTF_CTRL_ECSI_MASK)

#define SDU_FN0_CARD_BUS_INTF_CTRL_SCSI_MASK     (0x40U)
#define SDU_FN0_CARD_BUS_INTF_CTRL_SCSI_SHIFT    (6U)
/*! SCSI - Support continuous SPI Interrupt writable from internal bus only 0 = SPI supports
 *    interrupt assertion only when CS (SD)DAT[3] pin) is asserted 1 = SPI supports interrupt assertion
 *    independent of CS
 */
#define SDU_FN0_CARD_BUS_INTF_CTRL_SCSI(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_INTF_CTRL_SCSI_SHIFT)) & SDU_FN0_CARD_BUS_INTF_CTRL_SCSI_MASK)

#define SDU_FN0_CARD_BUS_INTF_CTRL_CD_DISABLE_MASK (0x80U)
#define SDU_FN0_CARD_BUS_INTF_CTRL_CD_DISABLE_SHIFT (7U)
/*! CD_DISABLE - Connect/Disconnet Disable Connect[0]/disconnect[1] the 10K-90 kO pull-up resistor on pin 1 of card. */
#define SDU_FN0_CARD_BUS_INTF_CTRL_CD_DISABLE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_INTF_CTRL_CD_DISABLE_SHIFT)) & SDU_FN0_CARD_BUS_INTF_CTRL_CD_DISABLE_MASK)
/*! @} */

/*! @name CAPAB - Card Capability */
/*! @{ */

#define SDU_FN0_CARD_CAPAB_SDC_MASK              (0x1U)
#define SDU_FN0_CARD_CAPAB_SDC_SHIFT             (0U)
/*! SDC - Support Direct Commands Card supports direct commands during multi-byte transfer 0 = all
 *    I/O functions do not accept and execute 1 = all I/O functions accept and execute
 */
#define SDU_FN0_CARD_CAPAB_SDC(x)                (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CAPAB_SDC_SHIFT)) & SDU_FN0_CARD_CAPAB_SDC_MASK)

#define SDU_FN0_CARD_CAPAB_SMB_MASK              (0x2U)
#define SDU_FN0_CARD_CAPAB_SMB_SHIFT             (1U)
/*! SMB - Support Multi-Block Card supports multi block indicator 0 = all I/O functions do not
 *    accept and execute 1 = all I/O functions accept and execute
 */
#define SDU_FN0_CARD_CAPAB_SMB(x)                (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CAPAB_SMB_SHIFT)) & SDU_FN0_CARD_CAPAB_SMB_MASK)

#define SDU_FN0_CARD_CAPAB_S4MI_MASK             (0x10U)
#define SDU_FN0_CARD_CAPAB_S4MI_SHIFT            (4U)
/*! S4MI - Support 4-bit Mode Interrupt Supports interrupt between blocks of data in 4-bit mode 0 =
 *    card not able to signal an interrupt during a 4-bit multi-block data transfer 1 = card is able
 *    to signal an interrupt between blocks during data transfer
 */
#define SDU_FN0_CARD_CAPAB_S4MI(x)               (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CAPAB_S4MI_SHIFT)) & SDU_FN0_CARD_CAPAB_S4MI_MASK)

#define SDU_FN0_CARD_CAPAB_E4MI_MASK             (0x20U)
#define SDU_FN0_CARD_CAPAB_E4MI_SHIFT            (5U)
/*! E4MI - Enable 4-bit Mode Interrupt Enable interrupt between blocks of data in 4-bit mode. */
#define SDU_FN0_CARD_CAPAB_E4MI(x)               (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CAPAB_E4MI_SHIFT)) & SDU_FN0_CARD_CAPAB_E4MI_MASK)
/*! @} */

/*! @name FUNC0_CIS_0 - Function 0 CIS Pointer 0 */
/*! @{ */

#define SDU_FN0_CARD_FUNC0_CIS_0_F0_CIS_PTR0_MASK (0xFFU)
#define SDU_FN0_CARD_FUNC0_CIS_0_F0_CIS_PTR0_SHIFT (0U)
/*! F0_CIS_PTR0 - CIS Pointer Bits[7:0] */
#define SDU_FN0_CARD_FUNC0_CIS_0_F0_CIS_PTR0(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC0_CIS_0_F0_CIS_PTR0_SHIFT)) & SDU_FN0_CARD_FUNC0_CIS_0_F0_CIS_PTR0_MASK)
/*! @} */

/*! @name FUNC0_CIS_1 - Function 0 CIS Pointer 1 */
/*! @{ */

#define SDU_FN0_CARD_FUNC0_CIS_1_F0_CIS_PTR1_MASK (0xFFU)
#define SDU_FN0_CARD_FUNC0_CIS_1_F0_CIS_PTR1_SHIFT (0U)
/*! F0_CIS_PTR1 - CIS Pointer Bits[15:8] */
#define SDU_FN0_CARD_FUNC0_CIS_1_F0_CIS_PTR1(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC0_CIS_1_F0_CIS_PTR1_SHIFT)) & SDU_FN0_CARD_FUNC0_CIS_1_F0_CIS_PTR1_MASK)
/*! @} */

/*! @name FUNC0_CIS_2 - Function 0 CIS Pointer 2 */
/*! @{ */

#define SDU_FN0_CARD_FUNC0_CIS_2_F0_CIS_PTR2_MASK (0xFFU)
#define SDU_FN0_CARD_FUNC0_CIS_2_F0_CIS_PTR2_SHIFT (0U)
/*! F0_CIS_PTR2 - CIS Pointer Bits[23:16] */
#define SDU_FN0_CARD_FUNC0_CIS_2_F0_CIS_PTR2(x)  (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC0_CIS_2_F0_CIS_PTR2_SHIFT)) & SDU_FN0_CARD_FUNC0_CIS_2_F0_CIS_PTR2_MASK)
/*! @} */

/*! @name BUS_SUSP - Bus Suspend */
/*! @{ */

#define SDU_FN0_CARD_BUS_SUSP_BUS_STATUS_MASK    (0x1U)
#define SDU_FN0_CARD_BUS_SUSP_BUS_STATUS_SHIFT   (0U)
/*! BUS_STATUS - Bus Status Bus status indicator 0 = N/A 1 = currently addressed function is currently executing a command */
#define SDU_FN0_CARD_BUS_SUSP_BUS_STATUS(x)      (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_SUSP_BUS_STATUS_SHIFT)) & SDU_FN0_CARD_BUS_SUSP_BUS_STATUS_MASK)

#define SDU_FN0_CARD_BUS_SUSP_BUS_RELEASE_MASK   (0x2U)
#define SDU_FN0_CARD_BUS_SUSP_BUS_RELEASE_SHIFT  (1U)
/*! BUS_RELEASE - Bus Release Bus release request/status indicator 0 = N/A 1 = suspend request still in progress */
#define SDU_FN0_CARD_BUS_SUSP_BUS_RELEASE(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_SUSP_BUS_RELEASE_SHIFT)) & SDU_FN0_CARD_BUS_SUSP_BUS_RELEASE_MASK)
/*! @} */

/*! @name BUS_SEL - Function Select */
/*! @{ */

#define SDU_FN0_CARD_BUS_SEL_FN_SELECT_MASK      (0xFU)
#define SDU_FN0_CARD_BUS_SEL_FN_SELECT_SHIFT     (0U)
/*! FN_SELECT - Select Function (Not used) */
#define SDU_FN0_CARD_BUS_SEL_FN_SELECT(x)        (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_SEL_FN_SELECT_SHIFT)) & SDU_FN0_CARD_BUS_SEL_FN_SELECT_MASK)

#define SDU_FN0_CARD_BUS_SEL_DATA_FLAG_MASK      (0x80U)
#define SDU_FN0_CARD_BUS_SEL_DATA_FLAG_SHIFT     (7U)
/*! DATA_FLAG - Data Flag (Not used) */
#define SDU_FN0_CARD_BUS_SEL_DATA_FLAG(x)        (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_SEL_DATA_FLAG_SHIFT)) & SDU_FN0_CARD_BUS_SEL_DATA_FLAG_MASK)
/*! @} */

/*! @name EXEC - Execute Flags */
/*! @{ */

#define SDU_FN0_CARD_EXEC_MEM_EXEC_MASK          (0x1U)
#define SDU_FN0_CARD_EXEC_MEM_EXEC_SHIFT         (0U)
/*! MEM_EXEC - Execution Flag for memory (Not used) */
#define SDU_FN0_CARD_EXEC_MEM_EXEC(x)            (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_EXEC_MEM_EXEC_SHIFT)) & SDU_FN0_CARD_EXEC_MEM_EXEC_MASK)

#define SDU_FN0_CARD_EXEC_FN_EXEC_MASK           (0xFEU)
#define SDU_FN0_CARD_EXEC_FN_EXEC_SHIFT          (1U)
/*! FN_EXEC - Execution Flag for functions (Not used) */
#define SDU_FN0_CARD_EXEC_FN_EXEC(x)             (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_EXEC_FN_EXEC_SHIFT)) & SDU_FN0_CARD_EXEC_FN_EXEC_MASK)
/*! @} */

/*! @name READY - Ready Flags */
/*! @{ */

#define SDU_FN0_CARD_READY_MEM_RDY_FLAG_MASK     (0x1U)
#define SDU_FN0_CARD_READY_MEM_RDY_FLAG_SHIFT    (0U)
/*! MEM_RDY_FLAG - Ready Flag for memory (Not used) */
#define SDU_FN0_CARD_READY_MEM_RDY_FLAG(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_READY_MEM_RDY_FLAG_SHIFT)) & SDU_FN0_CARD_READY_MEM_RDY_FLAG_MASK)

#define SDU_FN0_CARD_READY_FN_RDY_FLAG_MASK      (0xFEU)
#define SDU_FN0_CARD_READY_FN_RDY_FLAG_SHIFT     (1U)
/*! FN_RDY_FLAG - Ready Flag for functions (Not used) */
#define SDU_FN0_CARD_READY_FN_RDY_FLAG(x)        (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_READY_FN_RDY_FLAG_SHIFT)) & SDU_FN0_CARD_READY_FN_RDY_FLAG_MASK)
/*! @} */

/*! @name FN0_BLOCK_SIZE_0 - Function 0 Block Size 0 */
/*! @{ */

#define SDU_FN0_CARD_FN0_BLOCK_SIZE_0_FN0_BLK_SIZE0_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_BLOCK_SIZE_0_FN0_BLK_SIZE0_SHIFT (0U)
/*! FN0_BLK_SIZE0 - Block size [7:0] for Function 0 I/O block operations */
#define SDU_FN0_CARD_FN0_BLOCK_SIZE_0_FN0_BLK_SIZE0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_BLOCK_SIZE_0_FN0_BLK_SIZE0_SHIFT)) & SDU_FN0_CARD_FN0_BLOCK_SIZE_0_FN0_BLK_SIZE0_MASK)
/*! @} */

/*! @name FN0_BLOCK_SIZE_1 - Function 0 Block Size 1 */
/*! @{ */

#define SDU_FN0_CARD_FN0_BLOCK_SIZE_1_FN0_BLK_SIZE1_MASK (0x1U)
#define SDU_FN0_CARD_FN0_BLOCK_SIZE_1_FN0_BLK_SIZE1_SHIFT (0U)
/*! FN0_BLK_SIZE1 - Block size [8] for Function 0 I/O block operations */
#define SDU_FN0_CARD_FN0_BLOCK_SIZE_1_FN0_BLK_SIZE1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_BLOCK_SIZE_1_FN0_BLK_SIZE1_SHIFT)) & SDU_FN0_CARD_FN0_BLOCK_SIZE_1_FN0_BLK_SIZE1_MASK)
/*! @} */

/*! @name POWER_CONTROL - Power Control */
/*! @{ */

#define SDU_FN0_CARD_POWER_CONTROL_SMPC_MASK     (0x1U)
#define SDU_FN0_CARD_POWER_CONTROL_SMPC_SHIFT    (0U)
/*! SMPC - Support Master Power Control 0 = do not support 1 = support */
#define SDU_FN0_CARD_POWER_CONTROL_SMPC(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_POWER_CONTROL_SMPC_SHIFT)) & SDU_FN0_CARD_POWER_CONTROL_SMPC_MASK)

#define SDU_FN0_CARD_POWER_CONTROL_EMPC_MASK     (0x2U)
#define SDU_FN0_CARD_POWER_CONTROL_EMPC_SHIFT    (1U)
/*! EMPC - Enable Master Power Control 0 = disable 1 = enable */
#define SDU_FN0_CARD_POWER_CONTROL_EMPC(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_POWER_CONTROL_EMPC_SHIFT)) & SDU_FN0_CARD_POWER_CONTROL_EMPC_MASK)
/*! @} */

/*! @name BUS_SPEED_SELECT - Bus Speed Select */
/*! @{ */

#define SDU_FN0_CARD_BUS_SPEED_SELECT_SHS_MASK   (0x1U)
#define SDU_FN0_CARD_BUS_SPEED_SELECT_SHS_SHIFT  (0U)
/*! SHS - Support High Speed Mode 0 = do not support 1 = support */
#define SDU_FN0_CARD_BUS_SPEED_SELECT_SHS(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_SPEED_SELECT_SHS_SHIFT)) & SDU_FN0_CARD_BUS_SPEED_SELECT_SHS_MASK)

#define SDU_FN0_CARD_BUS_SPEED_SELECT_BSS_MASK   (0xEU)
#define SDU_FN0_CARD_BUS_SPEED_SELECT_BSS_SHIFT  (1U)
/*! BSS - Select Ultra High Speed Mode BSS Bus speed(1. */
#define SDU_FN0_CARD_BUS_SPEED_SELECT_BSS(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_BUS_SPEED_SELECT_BSS_SHIFT)) & SDU_FN0_CARD_BUS_SPEED_SELECT_BSS_MASK)
/*! @} */

/*! @name UHS_SUPPORT - UHS-I Support */
/*! @{ */

#define SDU_FN0_CARD_UHS_SUPPORT_SSDR50_MASK     (0x1U)
#define SDU_FN0_CARD_UHS_SUPPORT_SSDR50_SHIFT    (0U)
/*! SSDR50 - This bit indicates support of sdr50. */
#define SDU_FN0_CARD_UHS_SUPPORT_SSDR50(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_UHS_SUPPORT_SSDR50_SHIFT)) & SDU_FN0_CARD_UHS_SUPPORT_SSDR50_MASK)

#define SDU_FN0_CARD_UHS_SUPPORT_SSDR104_MASK    (0x2U)
#define SDU_FN0_CARD_UHS_SUPPORT_SSDR104_SHIFT   (1U)
/*! SSDR104 - This bit indicates support of sdr104. */
#define SDU_FN0_CARD_UHS_SUPPORT_SSDR104(x)      (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_UHS_SUPPORT_SSDR104_SHIFT)) & SDU_FN0_CARD_UHS_SUPPORT_SSDR104_MASK)

#define SDU_FN0_CARD_UHS_SUPPORT_SDDR50_MASK     (0x4U)
#define SDU_FN0_CARD_UHS_SUPPORT_SDDR50_SHIFT    (2U)
/*! SDDR50 - This bit indicates support of ddr50. */
#define SDU_FN0_CARD_UHS_SUPPORT_SDDR50(x)       (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_UHS_SUPPORT_SDDR50_SHIFT)) & SDU_FN0_CARD_UHS_SUPPORT_SDDR50_MASK)
/*! @} */

/*! @name DRIVER_STR - Driver Strength */
/*! @{ */

#define SDU_FN0_CARD_DRIVER_STR_SDTA_MASK        (0x1U)
#define SDU_FN0_CARD_DRIVER_STR_SDTA_SHIFT       (0U)
/*! SDTA - Support Driver Type A This bit indicates support of Driver Type A */
#define SDU_FN0_CARD_DRIVER_STR_SDTA(x)          (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_DRIVER_STR_SDTA_SHIFT)) & SDU_FN0_CARD_DRIVER_STR_SDTA_MASK)

#define SDU_FN0_CARD_DRIVER_STR_SDTC_MASK        (0x2U)
#define SDU_FN0_CARD_DRIVER_STR_SDTC_SHIFT       (1U)
/*! SDTC - Support Driver Type C This bit indicates support of Driver Type C */
#define SDU_FN0_CARD_DRIVER_STR_SDTC(x)          (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_DRIVER_STR_SDTC_SHIFT)) & SDU_FN0_CARD_DRIVER_STR_SDTC_MASK)

#define SDU_FN0_CARD_DRIVER_STR_SDTD_MASK        (0x4U)
#define SDU_FN0_CARD_DRIVER_STR_SDTD_SHIFT       (2U)
/*! SDTD - Support Driver Type D This bit indicates support of Driver Type D */
#define SDU_FN0_CARD_DRIVER_STR_SDTD(x)          (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_DRIVER_STR_SDTD_SHIFT)) & SDU_FN0_CARD_DRIVER_STR_SDTD_MASK)

#define SDU_FN0_CARD_DRIVER_STR_DTS_MASK         (0x30U)
#define SDU_FN0_CARD_DRIVER_STR_DTS_SHIFT        (4U)
/*! DTS - Driver Type Select */
#define SDU_FN0_CARD_DRIVER_STR_DTS(x)           (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_DRIVER_STR_DTS_SHIFT)) & SDU_FN0_CARD_DRIVER_STR_DTS_MASK)
/*! @} */

/*! @name INTERRUPT_EXT - Interrupt Extension */
/*! @{ */

#define SDU_FN0_CARD_INTERRUPT_EXT_SAI_MASK      (0x1U)
#define SDU_FN0_CARD_INTERRUPT_EXT_SAI_SHIFT     (0U)
/*! SAI - Support Asynchronous Interrupt Support bit of asynchronous interrupt. */
#define SDU_FN0_CARD_INTERRUPT_EXT_SAI(x)        (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_INTERRUPT_EXT_SAI_SHIFT)) & SDU_FN0_CARD_INTERRUPT_EXT_SAI_MASK)

#define SDU_FN0_CARD_INTERRUPT_EXT_EAI_MASK      (0x2U)
#define SDU_FN0_CARD_INTERRUPT_EXT_EAI_SHIFT     (1U)
/*! EAI - Enable Asynchronous Interrupt Enable bit of asynchronous interrupt. */
#define SDU_FN0_CARD_INTERRUPT_EXT_EAI(x)        (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_INTERRUPT_EXT_EAI_SHIFT)) & SDU_FN0_CARD_INTERRUPT_EXT_EAI_MASK)
/*! @} */

/*! @name CARD_CTRL1 - Card Control 1 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL1_CMD52_WR_ERR_WKUP_EN_MASK (0x1U)
#define SDU_FN0_CARD_CARD_CTRL1_CMD52_WR_ERR_WKUP_EN_SHIFT (0U)
/*! CMD52_WR_ERR_WKUP_EN - CMD52 Write Error Wakeup Enable If host issues CMD52 write access to any
 *    off-domain register during sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN0_CARD_CARD_CTRL1_CMD52_WR_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL1_CMD52_WR_ERR_WKUP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL1_CMD52_WR_ERR_WKUP_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL1_CMD52_RD_ERR_WKUP_EN_MASK (0x2U)
#define SDU_FN0_CARD_CARD_CTRL1_CMD52_RD_ERR_WKUP_EN_SHIFT (1U)
/*! CMD52_RD_ERR_WKUP_EN - CMD52 Read Error Wakeup Enable If host issues CMD52 read access to any
 *    off-domain register during sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN0_CARD_CARD_CTRL1_CMD52_RD_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL1_CMD52_RD_ERR_WKUP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL1_CMD52_RD_ERR_WKUP_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL1_CMD53_WR_ERR_WKUP_EN_MASK (0x4U)
#define SDU_FN0_CARD_CARD_CTRL1_CMD53_WR_ERR_WKUP_EN_SHIFT (2U)
/*! CMD53_WR_ERR_WKUP_EN - CMD53 Write Error Wakeup Enable If host issues CMD53 write access during
 *    sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN0_CARD_CARD_CTRL1_CMD53_WR_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL1_CMD53_WR_ERR_WKUP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL1_CMD53_WR_ERR_WKUP_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL1_CMD53_RD_ERR_WKUP_EN_MASK (0x8U)
#define SDU_FN0_CARD_CARD_CTRL1_CMD53_RD_ERR_WKUP_EN_SHIFT (3U)
/*! CMD53_RD_ERR_WKUP_EN - CMD53 Read Error Wakeup Enable If host issues CMD53 read access during
 *    sleep mode, setting this bit will trigger wakeup event to the APU.
 */
#define SDU_FN0_CARD_CARD_CTRL1_CMD53_RD_ERR_WKUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL1_CMD53_RD_ERR_WKUP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL1_CMD53_RD_ERR_WKUP_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL1_SD_HOST_INT_ACT_LVL_MASK (0x10U)
#define SDU_FN0_CARD_CARD_CTRL1_SD_HOST_INT_ACT_LVL_SHIFT (4U)
/*! SD_HOST_INT_ACT_LVL - SD Host Interrupt Active Level 0 = Active Low 1 = Active High */
#define SDU_FN0_CARD_CARD_CTRL1_SD_HOST_INT_ACT_LVL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL1_SD_HOST_INT_ACT_LVL_SHIFT)) & SDU_FN0_CARD_CARD_CTRL1_SD_HOST_INT_ACT_LVL_MASK)

#define SDU_FN0_CARD_CARD_CTRL1_CMD53_RFIFO_TH_MASK (0xC0U)
#define SDU_FN0_CARD_CARD_CTRL1_CMD53_RFIFO_TH_SHIFT (6U)
/*! CMD53_RFIFO_TH - CMD53 read fifo threshold 00 = 1 block size 01 = .5 block size 10 = ¼ block size 11 = ¾ block size */
#define SDU_FN0_CARD_CARD_CTRL1_CMD53_RFIFO_TH(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL1_CMD53_RFIFO_TH_SHIFT)) & SDU_FN0_CARD_CARD_CTRL1_CMD53_RFIFO_TH_MASK)
/*! @} */

/*! @name CARD_CTRL2 - Card Control 2 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL2_ASYNC_INT_MASK_EN_MASK (0x1U)
#define SDU_FN0_CARD_CARD_CTRL2_ASYNC_INT_MASK_EN_SHIFT (0U)
/*! ASYNC_INT_MASK_EN - Enable asynchronous interrupt mask */
#define SDU_FN0_CARD_CARD_CTRL2_ASYNC_INT_MASK_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL2_ASYNC_INT_MASK_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL2_ASYNC_INT_MASK_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL2_NGFF_SD_RST_EN_MASK (0x2U)
#define SDU_FN0_CARD_CARD_CTRL2_NGFF_SD_RST_EN_SHIFT (1U)
/*! NGFF_SD_RST_EN - Allow NGFF SDIO RESET# to reset OCR value. */
#define SDU_FN0_CARD_CARD_CTRL2_NGFF_SD_RST_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL2_NGFF_SD_RST_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL2_NGFF_SD_RST_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL2_CMD52_WR_RESP_EN_MASK (0x4U)
#define SDU_FN0_CARD_CARD_CTRL2_CMD52_WR_RESP_EN_SHIFT (2U)
/*! CMD52_WR_RESP_EN - Enable delay of CMD52 write response (Not used) */
#define SDU_FN0_CARD_CARD_CTRL2_CMD52_WR_RESP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL2_CMD52_WR_RESP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL2_CMD52_WR_RESP_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL2_CMD52_DLY_RES_RDATA_EN_MASK (0x8U)
#define SDU_FN0_CARD_CARD_CTRL2_CMD52_DLY_RES_RDATA_EN_SHIFT (3U)
/*! CMD52_DLY_RES_RDATA_EN - Delay latching of CMD52 read data until it is serially shifted out */
#define SDU_FN0_CARD_CARD_CTRL2_CMD52_DLY_RES_RDATA_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL2_CMD52_DLY_RES_RDATA_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL2_CMD52_DLY_RES_RDATA_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL2_CMD52_PWUP_EN_MASK (0x10U)
#define SDU_FN0_CARD_CARD_CTRL2_CMD52_PWUP_EN_SHIFT (4U)
/*! CMD52_PWUP_EN - If this is set to 1, host can issue any CMD52 to wake up the chip. */
#define SDU_FN0_CARD_CARD_CTRL2_CMD52_PWUP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL2_CMD52_PWUP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL2_CMD52_PWUP_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL2_APU_DEV_SLEEP_EN_MASK (0x20U)
#define SDU_FN0_CARD_CARD_CTRL2_APU_DEV_SLEEP_EN_SHIFT (5U)
/*! APU_DEV_SLEEP_EN - If this is set to 1, the IO_READY status and Card Ready status depend on APU sleep/wake state. */
#define SDU_FN0_CARD_CARD_CTRL2_APU_DEV_SLEEP_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL2_APU_DEV_SLEEP_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL2_APU_DEV_SLEEP_EN_MASK)
/*! @} */

/*! @name CMD19_CTRL1 - CMD19 Control1 */
/*! @{ */

#define SDU_FN0_CARD_CMD19_CTRL1_CMD19_TP_ADDR_MASK (0xFFU)
#define SDU_FN0_CARD_CMD19_CTRL1_CMD19_TP_ADDR_SHIFT (0U)
/*! CMD19_TP_ADDR - CMD19 tuning pattern address Set address value for CMD19 tuning pattern array */
#define SDU_FN0_CARD_CMD19_CTRL1_CMD19_TP_ADDR(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CMD19_CTRL1_CMD19_TP_ADDR_SHIFT)) & SDU_FN0_CARD_CMD19_CTRL1_CMD19_TP_ADDR_MASK)
/*! @} */

/*! @name CMD19_CTRL2 - CMD19 Control2 */
/*! @{ */

#define SDU_FN0_CARD_CMD19_CTRL2_CMD19_TP_DATA_MASK (0xFFU)
#define SDU_FN0_CARD_CMD19_CTRL2_CMD19_TP_DATA_SHIFT (0U)
/*! CMD19_TP_DATA - CMD19 tuning pattern data CMD19 tuning pattern data at cmd19_tp_addr */
#define SDU_FN0_CARD_CMD19_CTRL2_CMD19_TP_DATA(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CMD19_CTRL2_CMD19_TP_DATA_SHIFT)) & SDU_FN0_CARD_CMD19_CTRL2_CMD19_TP_DATA_MASK)
/*! @} */

/*! @name FUNC_CARD_INT - Function Card Interrupt */
/*! @{ */

#define SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT1_MASK (0xFU)
#define SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT1_SHIFT (0U)
/*! FN_CARD_INT1 - Pending card interrupt for each function to cpu1 */
#define SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT1_SHIFT)) & SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT1_MASK)

#define SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT2_MASK (0xF0U)
#define SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT2_SHIFT (4U)
/*! FN_CARD_INT2 - Pending card interrupt for each function to cpu2 */
#define SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT2(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT2_SHIFT)) & SDU_FN0_CARD_FUNC_CARD_INT_FN_CARD_INT2_MASK)
/*! @} */

/*! @name FUNC0_CARD_INTMASK - Function Card Interrupt Mask */
/*! @{ */

#define SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN0_CARD_INTMASK_MASK (0x7U)
#define SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN0_CARD_INTMASK_SHIFT (0U)
/*! FN0_CARD_INTMASK - Card interrupt mask for function 0. */
#define SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN0_CARD_INTMASK(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN0_CARD_INTMASK_SHIFT)) & SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN0_CARD_INTMASK_MASK)

#define SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN_CARD_INT3_MASK (0xF0U)
#define SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN_CARD_INT3_SHIFT (4U)
/*! FN_CARD_INT3 - Pending card interrupt for each function to cpu3 */
#define SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN_CARD_INT3(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN_CARD_INT3_SHIFT)) & SDU_FN0_CARD_FUNC0_CARD_INTMASK_FN_CARD_INT3_MASK)
/*! @} */

/*! @name DEV_SLEEP - Device Sleep */
/*! @{ */

#define SDU_FN0_CARD_DEV_SLEEP_DEV_SLEEP_MASK    (0x1U)
#define SDU_FN0_CARD_DEV_SLEEP_DEV_SLEEP_SHIFT   (0U)
/*! DEV_SLEEP - Device Sleep If this is set to 1, IO_READY and CARD_READY status will be 0. */
#define SDU_FN0_CARD_DEV_SLEEP_DEV_SLEEP(x)      (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_DEV_SLEEP_DEV_SLEEP_SHIFT)) & SDU_FN0_CARD_DEV_SLEEP_DEV_SLEEP_MASK)
/*! @} */

/*! @name CARD_CTRL3 - Card Control 3 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL3_CMD53_WR_BUSY_HW_EN_MASK (0x1U)
#define SDU_FN0_CARD_CARD_CTRL3_CMD53_WR_BUSY_HW_EN_SHIFT (0U)
/*! CMD53_WR_BUSY_HW_EN - Main enable bit for cmd53_wr_busy_hw_ctrl feature. */
#define SDU_FN0_CARD_CARD_CTRL3_CMD53_WR_BUSY_HW_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL3_CMD53_WR_BUSY_HW_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL3_CMD53_WR_BUSY_HW_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL3_CMD52_PWUP_MASK  (0x40U)
#define SDU_FN0_CARD_CARD_CTRL3_CMD52_PWUP_SHIFT (6U)
/*! CMD52_PWUP - Wakeup signal to APU. */
#define SDU_FN0_CARD_CARD_CTRL3_CMD52_PWUP(x)    (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL3_CMD52_PWUP_SHIFT)) & SDU_FN0_CARD_CARD_CTRL3_CMD52_PWUP_MASK)
/*! @} */

/*! @name FN0_CARD_INTMASK0 - Function 0 Card Interrupt Mask 0 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_INTMASK0_FN0_CARD_INTMASK0_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_INTMASK0_FN0_CARD_INTMASK0_SHIFT (0U)
/*! FN0_CARD_INTMASK0 - Function 0 card interrupt mask [7:0] */
#define SDU_FN0_CARD_FN0_CARD_INTMASK0_FN0_CARD_INTMASK0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_INTMASK0_FN0_CARD_INTMASK0_SHIFT)) & SDU_FN0_CARD_FN0_CARD_INTMASK0_FN0_CARD_INTMASK0_MASK)
/*! @} */

/*! @name FN0_CARD_INTMASK1 - Function 0 Card Interrupt Mask 1 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_INTMASK1_FN0_CARD_INTMASK1_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_INTMASK1_FN0_CARD_INTMASK1_SHIFT (0U)
/*! FN0_CARD_INTMASK1 - Function 0 card interrupt mask [15:8] */
#define SDU_FN0_CARD_FN0_CARD_INTMASK1_FN0_CARD_INTMASK1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_INTMASK1_FN0_CARD_INTMASK1_SHIFT)) & SDU_FN0_CARD_FN0_CARD_INTMASK1_FN0_CARD_INTMASK1_MASK)
/*! @} */

/*! @name FN0_CARD_INTRSR0 - Function 0 Card Interrupt Reset Select 0 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_INTRSR0_FN0_CARD_INTRSR0_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_INTRSR0_FN0_CARD_INTRSR0_SHIFT (0U)
/*! FN0_CARD_INTRSR0 - Function 0 card interrupt reset select [7:0] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN0_CARD_FN0_CARD_INTRSR0_FN0_CARD_INTRSR0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_INTRSR0_FN0_CARD_INTRSR0_SHIFT)) & SDU_FN0_CARD_FN0_CARD_INTRSR0_FN0_CARD_INTRSR0_MASK)
/*! @} */

/*! @name FN0_CARD_INTRSR1 - Function 0 Card Interrupt Reset Select 1 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_INTRSR1_FN0_CARD_INTRSR1_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_INTRSR1_FN0_CARD_INTRSR1_SHIFT (0U)
/*! FN0_CARD_INTRSR1 - Function 0 card interrupt reset select [15:8] 0 = ISR bit clears when 0 is written to it 1 = ISR bit clears when read */
#define SDU_FN0_CARD_FN0_CARD_INTRSR1_FN0_CARD_INTRSR1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_INTRSR1_FN0_CARD_INTRSR1_SHIFT)) & SDU_FN0_CARD_FN0_CARD_INTRSR1_FN0_CARD_INTRSR1_MASK)
/*! @} */

/*! @name FN0_CARD_INTSTATUS0 - Function 0 Card Interrupt Status 0 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_INTSTATUS0_FN0_CARD_INTSTAT0_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_INTSTATUS0_FN0_CARD_INTSTAT0_SHIFT (0U)
/*! FN0_CARD_INTSTAT0 - Function 0 card interrupt status [7:0] of the following events: [7:1] = fn_ps_event [0] = drv_snth_event */
#define SDU_FN0_CARD_FN0_CARD_INTSTATUS0_FN0_CARD_INTSTAT0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_INTSTATUS0_FN0_CARD_INTSTAT0_SHIFT)) & SDU_FN0_CARD_FN0_CARD_INTSTATUS0_FN0_CARD_INTSTAT0_MASK)
/*! @} */

/*! @name FN0_CARD_INTSTATUS1 - Function 0 Card Interrupt Status 1 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_INTSTATUS1_FN0_CARD_INTSTAT1_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_INTSTATUS1_FN0_CARD_INTSTAT1_SHIFT (0U)
/*! FN0_CARD_INTSTAT1 - Function 0 card interrupt status [15:8] of the following events: [7] = FN0
 *    CMD53 read access during sleep mode [6] = FN0 CMD53 write access during sleep mode [5] = FN0
 *    CMD52 read access to off-domain sdu register during sleep mode [4] = FN0 CMD52 write access to
 *    off-domain sdu register during sleep mode [3] = CMD5 is received [2] = CMD11 start [1] = CMD11
 *    switch start [0] = CMD11 succeeded
 */
#define SDU_FN0_CARD_FN0_CARD_INTSTATUS1_FN0_CARD_INTSTAT1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_INTSTATUS1_FN0_CARD_INTSTAT1_SHIFT)) & SDU_FN0_CARD_FN0_CARD_INTSTATUS1_FN0_CARD_INTSTAT1_MASK)
/*! @} */

/*! @name FN0_CARD_ACTVINTMASK0 - Function 0 Card Active Interrupt Mask 0 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_ACTVINTMASK0_FN0_CARD_ACTVINTMASK0_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_ACTVINTMASK0_FN0_CARD_ACTVINTMASK0_SHIFT (0U)
/*! FN0_CARD_ACTVINTMASK0 - Function 0 card active interrupt mask[7:0] */
#define SDU_FN0_CARD_FN0_CARD_ACTVINTMASK0_FN0_CARD_ACTVINTMASK0(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_ACTVINTMASK0_FN0_CARD_ACTVINTMASK0_SHIFT)) & SDU_FN0_CARD_FN0_CARD_ACTVINTMASK0_FN0_CARD_ACTVINTMASK0_MASK)
/*! @} */

/*! @name FN0_CARD_ACTVINTMASK1 - Function 0 Card Active Interrupt Mask 1 */
/*! @{ */

#define SDU_FN0_CARD_FN0_CARD_ACTVINTMASK1_FN0_CARD_ACTVINTMASK1_MASK (0xFFU)
#define SDU_FN0_CARD_FN0_CARD_ACTVINTMASK1_FN0_CARD_ACTVINTMASK1_SHIFT (0U)
/*! FN0_CARD_ACTVINTMASK1 - Function 0 card active interrupt mask[15:8] */
#define SDU_FN0_CARD_FN0_CARD_ACTVINTMASK1_FN0_CARD_ACTVINTMASK1(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_FN0_CARD_ACTVINTMASK1_FN0_CARD_ACTVINTMASK1_SHIFT)) & SDU_FN0_CARD_FN0_CARD_ACTVINTMASK1_FN0_CARD_ACTVINTMASK1_MASK)
/*! @} */

/*! @name CARD_CTRL4 - Card Control 4 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_VAL_MASK (0x1U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_VAL_SHIFT (0U)
/*! CMD11_V18_BYPASS_VAL - FW bypass value that overrides the V18 signal from the SD pad voltage sensor. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_VAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_VAL_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_VAL_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_EN_MASK (0x2U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_EN_SHIFT (1U)
/*! CMD11_V18_BYPASS_EN - If this is set, the V18 signal from the SD pad voltage sensor is bypassed. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD11_V18_BYPASS_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_CMD11_VIO_CHK_BYPASS_MASK (0x4U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_VIO_CHK_BYPASS_SHIFT (2U)
/*! CMD11_VIO_CHK_BYPASS - If this is set, the cmd11 state machine will bypass the VIO pad sensor check. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_VIO_CHK_BYPASS(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD11_VIO_CHK_BYPASS_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD11_VIO_CHK_BYPASS_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_CMD11_SD_CLK_STOP_BYPASS_MASK (0x8U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_SD_CLK_STOP_BYPASS_SHIFT (3U)
/*! CMD11_SD_CLK_STOP_BYPASS - If this is set, the cmd11 state machine will bypass the SD clk idle checking logic. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD11_SD_CLK_STOP_BYPASS(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD11_SD_CLK_STOP_BYPASS_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD11_SD_CLK_STOP_BYPASS_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_VAL_MASK (0x10U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_VAL_SHIFT (4U)
/*! CMD5_R4_S18A_BYPASS_VAL - FW bypass value that overrides the S18A field in cmd5 R4 response. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_VAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_VAL_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_VAL_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_EN_MASK (0x20U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_EN_SHIFT (5U)
/*! CMD5_R4_S18A_BYPASS_EN - If this is set, the S18A field in cmd5 R4 response is bypassed. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD5_R4_S18A_BYPASS_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_SET_CMD11_ILLEGAL_MASK (0x40U)
#define SDU_FN0_CARD_CARD_CTRL4_SET_CMD11_ILLEGAL_SHIFT (6U)
/*! SET_CMD11_ILLEGAL - If this is set and SD VIO pad sensor V18=1 (1. */
#define SDU_FN0_CARD_CARD_CTRL4_SET_CMD11_ILLEGAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_SET_CMD11_ILLEGAL_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_SET_CMD11_ILLEGAL_MASK)

#define SDU_FN0_CARD_CARD_CTRL4_CMD5_S18R_CHK_MASK (0x80U)
#define SDU_FN0_CARD_CARD_CTRL4_CMD5_S18R_CHK_SHIFT (7U)
/*! CMD5_S18R_CHK - If this is set, S18A in R4 depends on CMD5 S18R bit. */
#define SDU_FN0_CARD_CARD_CTRL4_CMD5_S18R_CHK(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL4_CMD5_S18R_CHK_SHIFT)) & SDU_FN0_CARD_CARD_CTRL4_CMD5_S18R_CHK_MASK)
/*! @} */

/*! @name CARD_CTRL5 - Card Control 5 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_VAL_MASK (0x7U)
#define SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_VAL_SHIFT (0U)
/*! CMD5_R4_FN_BYPASS_VAL - FW bypass value that overrides the function number derived from chip strap settings. */
#define SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_VAL(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_VAL_SHIFT)) & SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_VAL_MASK)

#define SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_EN_MASK (0x8U)
#define SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_EN_SHIFT (3U)
/*! CMD5_R4_FN_BYPASS_EN - If this is set, the function number field of R4 is controlled by
 *    cmd5_r4_fn_bypass_val rather than from chip strap settings.
 */
#define SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_EN(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_EN_SHIFT)) & SDU_FN0_CARD_CARD_CTRL5_CMD5_R4_FN_BYPASS_EN_MASK)

#define SDU_FN0_CARD_CARD_CTRL5_REPEAT_SDU_INIT_MASK (0x10U)
#define SDU_FN0_CARD_CARD_CTRL5_REPEAT_SDU_INIT_SHIFT (4U)
/*! REPEAT_SDU_INIT - If this is set, sdu can handle enumeration sequence initiated by host multiple times */
#define SDU_FN0_CARD_CARD_CTRL5_REPEAT_SDU_INIT(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL5_REPEAT_SDU_INIT_SHIFT)) & SDU_FN0_CARD_CARD_CTRL5_REPEAT_SDU_INIT_MASK)

#define SDU_FN0_CARD_CARD_CTRL5_RESET_RCA_MASK   (0x20U)
#define SDU_FN0_CARD_CARD_CTRL5_RESET_RCA_SHIFT  (5U)
/*! RESET_RCA - Reset RCA. */
#define SDU_FN0_CARD_CARD_CTRL5_RESET_RCA(x)     (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL5_RESET_RCA_SHIFT)) & SDU_FN0_CARD_CARD_CTRL5_RESET_RCA_MASK)

#define SDU_FN0_CARD_CARD_CTRL5_SD_RESET_AFTER_CMD52_R5_MASK (0x40U)
#define SDU_FN0_CARD_CARD_CTRL5_SD_RESET_AFTER_CMD52_R5_SHIFT (6U)
/*! SD_RESET_AFTER_CMD52_R5 - Generate sd_reset after cmd52 R5 response. */
#define SDU_FN0_CARD_CARD_CTRL5_SD_RESET_AFTER_CMD52_R5(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL5_SD_RESET_AFTER_CMD52_R5_SHIFT)) & SDU_FN0_CARD_CARD_CTRL5_SD_RESET_AFTER_CMD52_R5_MASK)

#define SDU_FN0_CARD_CARD_CTRL5_CMD52_RES_VALID_MODE_MASK (0x80U)
#define SDU_FN0_CARD_CARD_CTRL5_CMD52_RES_VALID_MODE_SHIFT (7U)
/*! CMD52_RES_VALID_MODE - CMD52 response valid mode 0 = CMD52 response valid is generated only
 *    after write operation is completed in the ahb_clk domain.
 */
#define SDU_FN0_CARD_CARD_CTRL5_CMD52_RES_VALID_MODE(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL5_CMD52_RES_VALID_MODE_SHIFT)) & SDU_FN0_CARD_CARD_CTRL5_CMD52_RES_VALID_MODE_MASK)
/*! @} */

/*! @name CARD_CTRL6 - Card Control 6 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL6_TESTBUS_SEL_LO_MASK (0xFFU)
#define SDU_FN0_CARD_CARD_CTRL6_TESTBUS_SEL_LO_SHIFT (0U)
/*! TESTBUS_SEL_LO - SDU testbus select [7:0] */
#define SDU_FN0_CARD_CARD_CTRL6_TESTBUS_SEL_LO(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL6_TESTBUS_SEL_LO_SHIFT)) & SDU_FN0_CARD_CARD_CTRL6_TESTBUS_SEL_LO_MASK)
/*! @} */

/*! @name CARD_CTRL7 - Card Control 7 */
/*! @{ */

#define SDU_FN0_CARD_CARD_CTRL7_TESTBUS_SEL_HI_MASK (0xFFU)
#define SDU_FN0_CARD_CARD_CTRL7_TESTBUS_SEL_HI_SHIFT (0U)
/*! TESTBUS_SEL_HI - SDU testbus select [15:8] */
#define SDU_FN0_CARD_CARD_CTRL7_TESTBUS_SEL_HI(x) (((uint8_t)(((uint8_t)(x)) << SDU_FN0_CARD_CARD_CTRL7_TESTBUS_SEL_HI_SHIFT)) & SDU_FN0_CARD_CARD_CTRL7_TESTBUS_SEL_HI_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SDU_FN0_CARD_Register_Masks */


/*!
 * @}
 */ /* end of group SDU_FN0_CARD_Peripheral_Access_Layer */


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


#endif  /* SDU_FN0_CARD_H_ */

