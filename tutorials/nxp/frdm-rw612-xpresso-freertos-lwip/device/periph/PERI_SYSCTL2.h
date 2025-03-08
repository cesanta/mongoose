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
**         CMSIS Peripheral Access Layer for SYSCTL2
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
 * @file SYSCTL2.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SYSCTL2
 *
 * CMSIS Peripheral Access Layer for SYSCTL2
 */

#if !defined(SYSCTL2_H_)
#define SYSCTL2_H_                               /**< Symbol preventing repeated inclusion */

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
   -- SYSCTL2 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSCTL2_Peripheral_Access_Layer SYSCTL2 Peripheral Access Layer
 * @{
 */

/** SYSCTL2 - Register Layout Typedef */
typedef struct {
  __IO uint32_t RAM_CTRL0;                         /**< RAM Memory Control Register 0, offset: 0x0 */
  __IO uint32_t USB_CTRL;                          /**< USB Control Register, offset: 0x4 */
  __IO uint32_t ANA_GRP_CTRL;                      /**< ANA GRP control register, offset: 0x8 */
  __IO uint32_t AVPLL_CTRL0;                       /**< Audio PLL Control register0, offset: 0xC */
  __IO uint32_t AVPLL_CTRL1;                       /**< Audio PLL Control register1, offset: 0x10 */
  __IO uint32_t AVPLL_CTRL2;                       /**< Audio PLL Control register2, offset: 0x14 */
  __IO uint32_t AVPLL_CTRL3;                       /**< Audio PLL Control register3, offset: 0x18 */
  __IO uint32_t AVPLL_CTRL4;                       /**< Audio PLL Control register4, offset: 0x1C */
  __IO uint32_t AVPLL_CTRL5;                       /**< Audio PLL Control register5, offset: 0x20 */
  __IO uint32_t AVPLL_CTRL6;                       /**< Audio PLL Control register6, offset: 0x24 */
  __I  uint32_t AVPLL_CTRL7;                       /**< Audio PLL Control register7, offset: 0x28 */
  __IO uint32_t AVPLL_CTRL8;                       /**< Audio PLL Control register8, offset: 0x2C */
  __IO uint32_t AVPLL_CTRL9;                       /**< Audio PLL Control register9, offset: 0x30 */
  __IO uint32_t AVPLL_CTRL10;                      /**< Audio PLL Control register10, offset: 0x34 */
  __IO uint32_t AVPLL_CTRL11;                      /**< Audio PLL Control register11, offset: 0x38 */
  __IO uint32_t AVPLL_CTRL12;                      /**< Audio PLL Control register12, offset: 0x3C */
  __IO uint32_t GAU_CTRL;                          /**< GAU Control register, offset: 0x40 */
  __IO uint32_t CTIMER_CTRL;                       /**< CTIMER Control register, offset: 0x44 */
  __IO uint32_t EXT_H2H_CTRL;                      /**< AHB async bridge Control register, offset: 0x48 */
  __IO uint32_t RAM_CTRL1;                         /**< RAM Memory Control Register 1, offset: 0x4C */
  __IO uint32_t ROM_CTRL;                          /**< ROM Control Register, offset: 0x50 */
  __IO uint32_t MEM_PD_CTRL;                       /**< MEM PD Control enable register when PM2 mode, offset: 0x54 */
  __IO uint32_t MEM_PD_CFG;                        /**< MEM PD Configure register when PM2 mode, offset: 0x58 */
  __IO uint32_t ENET_IN_SEL_TIMER;                 /**< Select input source for enet pad0, offset: 0x5C */
  __IO uint32_t ENET_IPG_STOP;                     /**< Configure ipg_stop, used by enet wakeup sequence, offset: 0x60 */
  __I  uint32_t ENET_IPG_STOP_ACK;                 /**< Store ipg_stop_ack, used by enet wakeup sequence, offset: 0x64 */
  __IO uint32_t ROM_BRU_ADDR_MASK_DIS;             /**< Disable dynamic address masking feature, offset: 0x68 */
  __IO uint32_t ROM_BRU_DYN_CLK_DIS;               /**< Disable dynamic clock gating feature, offset: 0x6C */
       uint8_t RESERVED_0[12];
  __I  uint32_t OTP_EARLY_FUSE_VALID;              /**< Early fuse valid from OTP, offset: 0x7C */
  __I  uint32_t OTP_MEDIUM_FUSE_VALID;             /**< Medium fuse valid from OTP, offset: 0x80 */
  __I  uint32_t OTP_ALL_FUSE_VALID;                /**< All fuse valid from OTP, offset: 0x84 */
  __IO uint32_t PLL_CTRL;                          /**< PLL control register, offset: 0x88 */
  __IO uint32_t ANA_PDWN_PM2;                      /**< ana_pdwn control signal when PM2 mode, offset: 0x8C */
  __IO uint32_t SOURCE_CLK_GATE;                   /**< source clock gate control, offset: 0x90 */
  __IO uint32_t TRNG_PWR_MODE;                     /**< TRNG_PWR_MODE, offset: 0x94 */
  __IO uint32_t TRNG_PIN_CTRL;                     /**< TRNG_PIN_CTRL, offset: 0x98 */
  __IO uint32_t CAU_CTRL;                          /**< CAU control register, offset: 0x9C */
  __IO uint32_t SOC_CIU_RDY_MASK;                  /**< SOC_CIU_RDY_MASK, offset: 0xA0 */
  __IO uint32_t LE_AUDIO_TIMER_ENABLE;             /**< Enable bit for le audio timer, offset: 0xA4 */
  __IO uint32_t LE_AUDIO_TIMER_CNT_CLR;            /**< Clear bit of internal counter, offset: 0xA8 */
  __I  uint32_t LE_AUDIO_TIMER_CNT0;               /**< Counter value captured by trigger0, offset: 0xAC */
  __I  uint32_t LE_AUDIO_TIMER_CNT1;               /**< Counter value captured by trigger1, offset: 0xB0 */
  __I  uint32_t LE_AUDIO_TIMER_CNT2;               /**< Counter value captured by trigger2, offset: 0xB4 */
       uint8_t RESERVED_1[8];
  __IO uint32_t AVPLL_CTRL13;                      /**< Audio PLL Control register13, offset: 0xC0 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_STATUS;       /**< CPU Code Bus Access Sram Checker Violation Status, offset: 0xC4 */
  __IO uint32_t MEM_ACC_CHK_CODE_VIO_CLR;          /**< CPU Code Bus Access Sram Checker Violation Status Clear, offset: 0xC8 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_STATUS;        /**< CPU SYS Bus Access Sram Checker Violation Status, offset: 0xCC */
  __IO uint32_t MEM_ACC_CHK_SYS_VIO_CLR;           /**< CPU SYS Bus Access Sram Checker Violation Status Clear, offset: 0xD0 */
       uint8_t RESERVED_2[4];
  __I  uint32_t SOC_MCI_EXTRA;                     /**< Reserved register, offset: 0xD8 */
  __IO uint32_t MCI_SOC_EXTRA;                     /**< Reserved register, offset: 0xDC */
  __IO uint32_t ROM_DYN_CLK_EN;                    /**< To control dynamic clock gating of the rom clock, offset: 0xE0 */
  __IO uint32_t RESERVED_REG1;                     /**< Reserved register, offset: 0xE4 */
  __IO uint32_t RESERVED_REG2;                     /**< Reserved register, offset: 0xE8 */
  __IO uint32_t RESERVED_REG3;                     /**< Reserved register, offset: 0xEC */
  __IO uint32_t RESERVED_REG4;                     /**< Reserved register, offset: 0xF0 */
  __IO uint32_t RESERVED_REG5;                     /**< Reserved register, offset: 0xF4 */
  __IO uint32_t RESERVED_REG6;                     /**< Reserved register, offset: 0xF8 */
  __IO uint32_t RESERVED_REG7;                     /**< Reserved register, offset: 0xFC */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR0;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x100 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC0;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x104 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR1;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x108 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC1;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x10C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR2;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x110 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC2;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x114 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR3;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x118 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC3;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x11C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR4;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x120 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC4;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x124 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR5;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x128 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC5;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x12C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR6;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x130 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC6;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x134 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR7;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x138 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC7;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x13C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR8;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x140 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC8;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x144 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR9;        /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x148 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC9;        /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x14C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR10;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x150 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC10;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x154 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR11;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x158 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC11;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x15C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR12;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x160 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC12;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x164 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR13;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x168 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC13;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x16C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR14;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x170 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC14;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x174 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR15;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x178 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC15;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x17C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR16;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x180 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC16;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x184 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR17;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x188 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC17;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x18C */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_ADDR18;       /**< CPU CODE Bus Access Sram Checker: Address of Violated Transfer, offset: 0x190 */
  __I  uint32_t MEM_ACC_CHK_CODE_VIO_MISC18;       /**< CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer, offset: 0x194 */
       uint8_t RESERVED_3[104];
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR0;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x200 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC0;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x204 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR1;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x208 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC1;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x20C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR2;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x210 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC2;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x214 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR3;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x218 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC3;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x21C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR4;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x220 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC4;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x224 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR5;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x228 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC5;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x22C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR6;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x230 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC6;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x234 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR7;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x238 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC7;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x23C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR8;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x240 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC8;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x244 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR9;         /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x248 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC9;         /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x24C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR10;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x250 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC10;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x254 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR11;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x258 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC11;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x25C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR12;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x260 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC12;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x264 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR13;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x268 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC13;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x26C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR14;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x270 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC14;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x274 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR15;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x278 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC15;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x27C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR16;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x280 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC16;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x284 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR17;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x288 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC17;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x28C */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_ADDR18;        /**< CPU SYS Bus Access Sram Checker: Address of Violated Transfer, offset: 0x290 */
  __I  uint32_t MEM_ACC_CHK_SYS_VIO_MISC18;        /**< CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer, offset: 0x294 */
} SYSCTL2_Type;

/* ----------------------------------------------------------------------------
   -- SYSCTL2 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSCTL2_Register_Masks SYSCTL2 Register Masks
 * @{
 */

/*! @name RAM_CTRL0 - RAM Memory Control Register 0 */
/*! @{ */

#define SYSCTL2_RAM_CTRL0_AON_MEM_RTC1_MASK      (0x3U)
#define SYSCTL2_RAM_CTRL0_AON_MEM_RTC1_SHIFT     (0U)
/*! AON_MEM_RTC1 - aon_mem_rtc1 */
#define SYSCTL2_RAM_CTRL0_AON_MEM_RTC1(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_AON_MEM_RTC1_SHIFT)) & SYSCTL2_RAM_CTRL0_AON_MEM_RTC1_MASK)

#define SYSCTL2_RAM_CTRL0_AON_MEM_WTC1_MASK      (0xCU)
#define SYSCTL2_RAM_CTRL0_AON_MEM_WTC1_SHIFT     (2U)
/*! AON_MEM_WTC1 - aon_mem_wtc1 */
#define SYSCTL2_RAM_CTRL0_AON_MEM_WTC1(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_AON_MEM_WTC1_SHIFT)) & SYSCTL2_RAM_CTRL0_AON_MEM_WTC1_MASK)

#define SYSCTL2_RAM_CTRL0_AON_MEM_RTC0_MASK      (0x30U)
#define SYSCTL2_RAM_CTRL0_AON_MEM_RTC0_SHIFT     (4U)
/*! AON_MEM_RTC0 - aon_mem_rtc0 */
#define SYSCTL2_RAM_CTRL0_AON_MEM_RTC0(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_AON_MEM_RTC0_SHIFT)) & SYSCTL2_RAM_CTRL0_AON_MEM_RTC0_MASK)

#define SYSCTL2_RAM_CTRL0_AON_MEM_WTC0_MASK      (0xC0U)
#define SYSCTL2_RAM_CTRL0_AON_MEM_WTC0_SHIFT     (6U)
/*! AON_MEM_WTC0 - aon_mem_wtc0 */
#define SYSCTL2_RAM_CTRL0_AON_MEM_WTC0(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_AON_MEM_WTC0_SHIFT)) & SYSCTL2_RAM_CTRL0_AON_MEM_WTC0_MASK)

#define SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_RTC_MASK   (0x300U)
#define SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_RTC_SHIFT  (8U)
/*! FLEXSPI_MEM_RTC - flexspi_mem_rtc */
#define SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_RTC(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_RTC_SHIFT)) & SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_RTC_MASK)

#define SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_WTC_MASK   (0xC00U)
#define SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_WTC_SHIFT  (10U)
/*! FLEXSPI_MEM_WTC - flexspi_mem_wtc */
#define SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_WTC(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_WTC_SHIFT)) & SYSCTL2_RAM_CTRL0_FLEXSPI_MEM_WTC_MASK)

#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_RTC_MASK (0x3000U)
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_RTC_SHIFT (12U)
/*! FLEXSPI_CACHE_RTC - flexspi_cache_rtc */
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_RTC(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_RTC_SHIFT)) & SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_RTC_MASK)

#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_WTC_MASK (0xC000U)
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_WTC_SHIFT (14U)
/*! FLEXSPI_CACHE_WTC - flexspi_cache_wtc */
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_WTC(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_WTC_SHIFT)) & SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_WTC_MASK)

#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_RTC_MASK (0x30000U)
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_RTC_SHIFT (16U)
/*! FLEXSPI_CACHE_TAG_RTC - flexspi_cache_tag_rtc */
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_RTC(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_RTC_SHIFT)) & SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_RTC_MASK)

#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_WTC_MASK (0xC0000U)
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_WTC_SHIFT (18U)
/*! FLEXSPI_CACHE_TAG_WTC - flexspi_cache_tag_wtc */
#define SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_WTC(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_WTC_SHIFT)) & SYSCTL2_RAM_CTRL0_FLEXSPI_CACHE_TAG_WTC_MASK)
/*! @} */

/*! @name USB_CTRL - USB Control Register */
/*! @{ */

#define SYSCTL2_USB_CTRL_USB_PU_PLL_MASK         (0x1U)
#define SYSCTL2_USB_CTRL_USB_PU_PLL_SHIFT        (0U)
/*! USB_PU_PLL - USB PU PLL */
#define SYSCTL2_USB_CTRL_USB_PU_PLL(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_USB_PU_PLL_SHIFT)) & SYSCTL2_USB_CTRL_USB_PU_PLL_MASK)

#define SYSCTL2_USB_CTRL_USB_PU_OTG_MASK         (0x2U)
#define SYSCTL2_USB_CTRL_USB_PU_OTG_SHIFT        (1U)
/*! USB_PU_OTG - USB PU OTG */
#define SYSCTL2_USB_CTRL_USB_PU_OTG(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_USB_PU_OTG_SHIFT)) & SYSCTL2_USB_CTRL_USB_PU_OTG_MASK)

#define SYSCTL2_USB_CTRL_USB_PU_MASK             (0x4U)
#define SYSCTL2_USB_CTRL_USB_PU_SHIFT            (2U)
/*! USB_PU - USB PU */
#define SYSCTL2_USB_CTRL_USB_PU(x)               (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_USB_PU_SHIFT)) & SYSCTL2_USB_CTRL_USB_PU_MASK)

#define SYSCTL2_USB_CTRL_REG_RX_PDFVSSM_MASK     (0x8U)
#define SYSCTL2_USB_CTRL_REG_RX_PDFVSSM_SHIFT    (3U)
/*! REG_RX_PDFVSSM - reg_rx_pdfvssm */
#define SYSCTL2_USB_CTRL_REG_RX_PDFVSSM(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_RX_PDFVSSM_SHIFT)) & SYSCTL2_USB_CTRL_REG_RX_PDFVSSM_MASK)

#define SYSCTL2_USB_CTRL_REG_RX_PDLVMC_MASK      (0x10U)
#define SYSCTL2_USB_CTRL_REG_RX_PDLVMC_SHIFT     (4U)
/*! REG_RX_PDLVMC - reg_rx_pdlvmc */
#define SYSCTL2_USB_CTRL_REG_RX_PDLVMC(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_RX_PDLVMC_SHIFT)) & SYSCTL2_USB_CTRL_REG_RX_PDLVMC_MASK)

#define SYSCTL2_USB_CTRL_REG_TX_PDFVSSM_MASK     (0x20U)
#define SYSCTL2_USB_CTRL_REG_TX_PDFVSSM_SHIFT    (5U)
/*! REG_TX_PDFVSSM - reg_tx_pdfvssm */
#define SYSCTL2_USB_CTRL_REG_TX_PDFVSSM(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_TX_PDFVSSM_SHIFT)) & SYSCTL2_USB_CTRL_REG_TX_PDFVSSM_MASK)

#define SYSCTL2_USB_CTRL_REG_TX_PDLVMC_MASK      (0x40U)
#define SYSCTL2_USB_CTRL_REG_TX_PDLVMC_SHIFT     (6U)
/*! REG_TX_PDLVMC - reg_tx_pdlvmc */
#define SYSCTL2_USB_CTRL_REG_TX_PDLVMC(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_TX_PDLVMC_SHIFT)) & SYSCTL2_USB_CTRL_REG_TX_PDLVMC_MASK)

#define SYSCTL2_USB_CTRL_REG_RX_BUF_RTC_MASK     (0x180U)
#define SYSCTL2_USB_CTRL_REG_RX_BUF_RTC_SHIFT    (7U)
/*! REG_RX_BUF_RTC - reg_rx_buf_rtc */
#define SYSCTL2_USB_CTRL_REG_RX_BUF_RTC(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_RX_BUF_RTC_SHIFT)) & SYSCTL2_USB_CTRL_REG_RX_BUF_RTC_MASK)

#define SYSCTL2_USB_CTRL_REG_RX_BUF_WTC_MASK     (0x600U)
#define SYSCTL2_USB_CTRL_REG_RX_BUF_WTC_SHIFT    (9U)
/*! REG_RX_BUF_WTC - reg_rx_buf_wtc */
#define SYSCTL2_USB_CTRL_REG_RX_BUF_WTC(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_RX_BUF_WTC_SHIFT)) & SYSCTL2_USB_CTRL_REG_RX_BUF_WTC_MASK)

#define SYSCTL2_USB_CTRL_REG_TX_BUF_RTC_MASK     (0x1800U)
#define SYSCTL2_USB_CTRL_REG_TX_BUF_RTC_SHIFT    (11U)
/*! REG_TX_BUF_RTC - reg_tx_buf_rtc */
#define SYSCTL2_USB_CTRL_REG_TX_BUF_RTC(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_TX_BUF_RTC_SHIFT)) & SYSCTL2_USB_CTRL_REG_TX_BUF_RTC_MASK)

#define SYSCTL2_USB_CTRL_REG_TX_BUF_WTC_MASK     (0x6000U)
#define SYSCTL2_USB_CTRL_REG_TX_BUF_WTC_SHIFT    (13U)
/*! REG_TX_BUF_WTC - reg_tx_buf_wtc */
#define SYSCTL2_USB_CTRL_REG_TX_BUF_WTC(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_REG_TX_BUF_WTC_SHIFT)) & SYSCTL2_USB_CTRL_REG_TX_BUF_WTC_MASK)

#define SYSCTL2_USB_CTRL_USB_RESUME_MASK         (0x8000U)
#define SYSCTL2_USB_CTRL_USB_RESUME_SHIFT        (15U)
/*! USB_RESUME - USB Resume */
#define SYSCTL2_USB_CTRL_USB_RESUME(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_USB_RESUME_SHIFT)) & SYSCTL2_USB_CTRL_USB_RESUME_MASK)

#define SYSCTL2_USB_CTRL_IDDQ_TEST_MASK          (0x10000U)
#define SYSCTL2_USB_CTRL_IDDQ_TEST_SHIFT         (16U)
/*! IDDQ_TEST - iddq Test */
#define SYSCTL2_USB_CTRL_IDDQ_TEST(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_IDDQ_TEST_SHIFT)) & SYSCTL2_USB_CTRL_IDDQ_TEST_MASK)

#define SYSCTL2_USB_CTRL_SOFT_PHY_RESET_MASK     (0x20000U)
#define SYSCTL2_USB_CTRL_SOFT_PHY_RESET_SHIFT    (17U)
/*! SOFT_PHY_RESET - Soft PHY Reset */
#define SYSCTL2_USB_CTRL_SOFT_PHY_RESET(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_SOFT_PHY_RESET_SHIFT)) & SYSCTL2_USB_CTRL_SOFT_PHY_RESET_MASK)

#define SYSCTL2_USB_CTRL_PHY_RESET_SEL_MASK      (0x40000U)
#define SYSCTL2_USB_CTRL_PHY_RESET_SEL_SHIFT     (18U)
/*! PHY_RESET_SEL - PHY Reset Select */
#define SYSCTL2_USB_CTRL_PHY_RESET_SEL(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_PHY_RESET_SEL_SHIFT)) & SYSCTL2_USB_CTRL_PHY_RESET_SEL_MASK)

#define SYSCTL2_USB_CTRL_SOFT_UTMI_SESSEND_MASK  (0x80000U)
#define SYSCTL2_USB_CTRL_SOFT_UTMI_SESSEND_SHIFT (19U)
/*! SOFT_UTMI_SESSEND - Soft UTMI sessend */
#define SYSCTL2_USB_CTRL_SOFT_UTMI_SESSEND(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_SOFT_UTMI_SESSEND_SHIFT)) & SYSCTL2_USB_CTRL_SOFT_UTMI_SESSEND_MASK)

#define SYSCTL2_USB_CTRL_SOFT_UTMI_XVALID_MASK   (0x100000U)
#define SYSCTL2_USB_CTRL_SOFT_UTMI_XVALID_SHIFT  (20U)
/*! SOFT_UTMI_XVALID - Soft UTMI xvalid */
#define SYSCTL2_USB_CTRL_SOFT_UTMI_XVALID(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_SOFT_UTMI_XVALID_SHIFT)) & SYSCTL2_USB_CTRL_SOFT_UTMI_XVALID_MASK)

#define SYSCTL2_USB_CTRL_SOFT_UTMI_IDDIG_MASK    (0x200000U)
#define SYSCTL2_USB_CTRL_SOFT_UTMI_IDDIG_SHIFT   (21U)
/*! SOFT_UTMI_IDDIG - Soft UTMI iddig */
#define SYSCTL2_USB_CTRL_SOFT_UTMI_IDDIG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_SOFT_UTMI_IDDIG_SHIFT)) & SYSCTL2_USB_CTRL_SOFT_UTMI_IDDIG_MASK)

#define SYSCTL2_USB_CTRL_MAC_CTRL_SEL_MASK       (0x400000U)
#define SYSCTL2_USB_CTRL_MAC_CTRL_SEL_SHIFT      (22U)
/*! MAC_CTRL_SEL - MAC Control Select */
#define SYSCTL2_USB_CTRL_MAC_CTRL_SEL(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_MAC_CTRL_SEL_SHIFT)) & SYSCTL2_USB_CTRL_MAC_CTRL_SEL_MASK)

#define SYSCTL2_USB_CTRL_USB_TX_BITSTUFF_EN_MASK (0x800000U)
#define SYSCTL2_USB_CTRL_USB_TX_BITSTUFF_EN_SHIFT (23U)
/*! USB_TX_BITSTUFF_EN - USM TX BITSTUFF EN */
#define SYSCTL2_USB_CTRL_USB_TX_BITSTUFF_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_USB_CTRL_USB_TX_BITSTUFF_EN_SHIFT)) & SYSCTL2_USB_CTRL_USB_TX_BITSTUFF_EN_MASK)
/*! @} */

/*! @name ANA_GRP_CTRL - ANA GRP control register */
/*! @{ */

#define SYSCTL2_ANA_GRP_CTRL_ICC_ADJ_MASK        (0x3U)
#define SYSCTL2_ANA_GRP_CTRL_ICC_ADJ_SHIFT       (0U)
/*! ICC_ADJ - ICC_ADJ Current Select */
#define SYSCTL2_ANA_GRP_CTRL_ICC_ADJ(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_GRP_CTRL_ICC_ADJ_SHIFT)) & SYSCTL2_ANA_GRP_CTRL_ICC_ADJ_MASK)

#define SYSCTL2_ANA_GRP_CTRL_IPP_ADJ_MASK        (0xCU)
#define SYSCTL2_ANA_GRP_CTRL_IPP_ADJ_SHIFT       (2U)
/*! IPP_ADJ - IPP_ADJ Current Select */
#define SYSCTL2_ANA_GRP_CTRL_IPP_ADJ(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_GRP_CTRL_IPP_ADJ_SHIFT)) & SYSCTL2_ANA_GRP_CTRL_IPP_ADJ_MASK)

#define SYSCTL2_ANA_GRP_CTRL_AVDD1815_SEL_MASK   (0x20U)
#define SYSCTL2_ANA_GRP_CTRL_AVDD1815_SEL_SHIFT  (5U)
/*! AVDD1815_SEL - AVDD Select */
#define SYSCTL2_ANA_GRP_CTRL_AVDD1815_SEL(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_GRP_CTRL_AVDD1815_SEL_SHIFT)) & SYSCTL2_ANA_GRP_CTRL_AVDD1815_SEL_MASK)

#define SYSCTL2_ANA_GRP_CTRL_TEST_ANA_MASK       (0x3C0U)
#define SYSCTL2_ANA_GRP_CTRL_TEST_ANA_SHIFT      (6U)
/*! TEST_ANA - TEST ANA */
#define SYSCTL2_ANA_GRP_CTRL_TEST_ANA(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_GRP_CTRL_TEST_ANA_SHIFT)) & SYSCTL2_ANA_GRP_CTRL_TEST_ANA_MASK)

#define SYSCTL2_ANA_GRP_CTRL_PU_XTL_MASK         (0x400U)
#define SYSCTL2_ANA_GRP_CTRL_PU_XTL_SHIFT        (10U)
/*! PU_XTL - PU_XTL power up . Provide the reference voltage , USB need it */
#define SYSCTL2_ANA_GRP_CTRL_PU_XTL(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_GRP_CTRL_PU_XTL_SHIFT)) & SYSCTL2_ANA_GRP_CTRL_PU_XTL_MASK)

#define SYSCTL2_ANA_GRP_CTRL_PU_AG_MASK          (0x1000U)
#define SYSCTL2_ANA_GRP_CTRL_PU_AG_SHIFT         (12U)
/*! PU_AG - Analog Group Power Up.Provide the reference current , AVPLL and USB need it */
#define SYSCTL2_ANA_GRP_CTRL_PU_AG(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_GRP_CTRL_PU_AG_SHIFT)) & SYSCTL2_ANA_GRP_CTRL_PU_AG_MASK)
/*! @} */

/*! @name AVPLL_CTRL0 - Audio PLL Control register0 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL0_EXT_SPEED_EN_MASK    (0x1U)
#define SYSCTL2_AVPLL_CTRL0_EXT_SPEED_EN_SHIFT   (0U)
/*! EXT_SPEED_EN - EXT_SPEED Enable */
#define SYSCTL2_AVPLL_CTRL0_EXT_SPEED_EN(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EXT_SPEED_EN_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EXT_SPEED_EN_MASK)

#define SYSCTL2_AVPLL_CTRL0_EXT_SPEED_MASK       (0x1EU)
#define SYSCTL2_AVPLL_CTRL0_EXT_SPEED_SHIFT      (1U)
/*! EXT_SPEED - External VCO Speed Control for Different VCO Frequencies. */
#define SYSCTL2_AVPLL_CTRL0_EXT_SPEED(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EXT_SPEED_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EXT_SPEED_MASK)

#define SYSCTL2_AVPLL_CTRL0_EXT_SP_FBRES_EN_MASK (0x20U)
#define SYSCTL2_AVPLL_CTRL0_EXT_SP_FBRES_EN_SHIFT (5U)
/*! EXT_SP_FBRES_EN - External Speed Enable Pin. */
#define SYSCTL2_AVPLL_CTRL0_EXT_SP_FBRES_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EXT_SP_FBRES_EN_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EXT_SP_FBRES_EN_MASK)

#define SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_EN_MASK (0x40U)
#define SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_EN_SHIFT (6U)
/*! EXT_SLLP_DAC_EN - EXT_SLLP_DAC Enable */
#define SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_EN_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_EN_MASK)

#define SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_MASK    (0x3F80U)
#define SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_SHIFT   (7U)
/*! EXT_SLLP_DAC - VCON Reference Value Set */
#define SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EXT_SLLP_DAC_MASK)

#define SYSCTL2_AVPLL_CTRL0_EN_LP_C1_MASK        (0xC000U)
#define SYSCTL2_AVPLL_CTRL0_EN_LP_C1_SHIFT       (14U)
/*! EN_LP_C1 - Channel CX LP Enable. */
#define SYSCTL2_AVPLL_CTRL0_EN_LP_C1(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EN_LP_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EN_LP_C1_MASK)

#define SYSCTL2_AVPLL_CTRL0_EN_DPLL_C1_MASK      (0x10000U)
#define SYSCTL2_AVPLL_CTRL0_EN_DPLL_C1_SHIFT     (16U)
/*! EN_DPLL_C1 - Enable/ Disable Channel CX¡¯s DPLL */
#define SYSCTL2_AVPLL_CTRL0_EN_DPLL_C1(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_EN_DPLL_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL0_EN_DPLL_C1_MASK)

#define SYSCTL2_AVPLL_CTRL0_DPHER_DLY_SEL_MASK   (0x60000U)
#define SYSCTL2_AVPLL_CTRL0_DPHER_DLY_SEL_SHIFT  (17U)
/*! DPHER_DLY_SEL - DPHERCK Delay Tune */
#define SYSCTL2_AVPLL_CTRL0_DPHER_DLY_SEL(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_DPHER_DLY_SEL_SHIFT)) & SYSCTL2_AVPLL_CTRL0_DPHER_DLY_SEL_MASK)

#define SYSCTL2_AVPLL_CTRL0_CLKOUT_TST_EN_MASK   (0x80000U)
#define SYSCTL2_AVPLL_CTRL0_CLKOUT_TST_EN_SHIFT  (19U)
/*! CLKOUT_TST_EN - Clock Out Test Output Enable */
#define SYSCTL2_AVPLL_CTRL0_CLKOUT_TST_EN(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_CLKOUT_TST_EN_SHIFT)) & SYSCTL2_AVPLL_CTRL0_CLKOUT_TST_EN_MASK)

#define SYSCTL2_AVPLL_CTRL0_CLK_DET_EN_MASK      (0x100000U)
#define SYSCTL2_AVPLL_CTRL0_CLK_DET_EN_SHIFT     (20U)
/*! CLK_DET_EN - PI Output Clock Enable for Internal Reset Circuit */
#define SYSCTL2_AVPLL_CTRL0_CLK_DET_EN(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_CLK_DET_EN_SHIFT)) & SYSCTL2_AVPLL_CTRL0_CLK_DET_EN_MASK)

#define SYSCTL2_AVPLL_CTRL0_CAL_FBDIV_MASK       (0x3FE00000U)
#define SYSCTL2_AVPLL_CTRL0_CAL_FBDIV_SHIFT      (21U)
/*! CAL_FBDIV - FBDIV Calibration */
#define SYSCTL2_AVPLL_CTRL0_CAL_FBDIV(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL0_CAL_FBDIV_SHIFT)) & SYSCTL2_AVPLL_CTRL0_CAL_FBDIV_MASK)
/*! @} */

/*! @name AVPLL_CTRL1 - Audio PLL Control register1 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1_MASK (0x1U)
#define SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1_SHIFT (0U)
/*! FREQ_OFFSET_READY_C1 - Indicate Frequency Offset Value Readiness. */
#define SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_READY_C1_MASK)

#define SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1_MASK  (0xFFFFEU)
#define SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1_SHIFT (1U)
/*! FREQ_OFFSET_C1 - FREQ_OFFSET_CX[18:0] Set */
#define SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL1_FREQ_OFFSET_C1_MASK)

#define SYSCTL2_AVPLL_CTRL1_FBDIV_MASK           (0x1FF00000U)
#define SYSCTL2_AVPLL_CTRL1_FBDIV_SHIFT          (20U)
/*! FBDIV - Feedback Clock Divider Select */
#define SYSCTL2_AVPLL_CTRL1_FBDIV(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL1_FBDIV_SHIFT)) & SYSCTL2_AVPLL_CTRL1_FBDIV_MASK)
/*! @} */

/*! @name AVPLL_CTRL2 - Audio PLL Control register2 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL2_PW_SLLP_MASK         (0x7U)
#define SYSCTL2_AVPLL_CTRL2_PW_SLLP_SHIFT        (0U)
/*! PW_SLLP - PLL in Slow Loop */
#define SYSCTL2_AVPLL_CTRL2_PW_SLLP(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PW_SLLP_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PW_SLLP_MASK)

#define SYSCTL2_AVPLL_CTRL2_PU_OFST_CTRL_C1_MASK (0x8U)
#define SYSCTL2_AVPLL_CTRL2_PU_OFST_CTRL_C1_SHIFT (3U)
/*! PU_OFST_CTRL_C1 - Power Up/Down FREQ_OFFSET Integrator of CX */
#define SYSCTL2_AVPLL_CTRL2_PU_OFST_CTRL_C1(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PU_OFST_CTRL_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PU_OFST_CTRL_C1_MASK)

#define SYSCTL2_AVPLL_CTRL2_PU_C1_MASK           (0x10U)
#define SYSCTL2_AVPLL_CTRL2_PU_C1_SHIFT          (4U)
/*! PU_C1 - Power Up/Down Channel CX */
#define SYSCTL2_AVPLL_CTRL2_PU_C1(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PU_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PU_C1_MASK)

#define SYSCTL2_AVPLL_CTRL2_PU_MASK              (0x20U)
#define SYSCTL2_AVPLL_CTRL2_PU_SHIFT             (5U)
/*! PU - PLL Power-up Signal */
#define SYSCTL2_AVPLL_CTRL2_PU(x)                (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PU_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PU_MASK)

#define SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1_MASK  (0x40U)
#define SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1_SHIFT (6U)
/*! POSTDIV_0P5_C1 - Audio Clock Divider Program Set */
#define SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL2_POSTDIV_0P5_C1_MASK)

#define SYSCTL2_AVPLL_CTRL2_PLL_VDDRA_SEL_MASK   (0x380U)
#define SYSCTL2_AVPLL_CTRL2_PLL_VDDRA_SEL_SHIFT  (7U)
/*! PLL_VDDRA_SEL - Gate Voltage Select for VDDBUF */
#define SYSCTL2_AVPLL_CTRL2_PLL_VDDRA_SEL(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PLL_VDDRA_SEL_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PLL_VDDRA_SEL_MASK)

#define SYSCTL2_AVPLL_CTRL2_PLL_LPFC2_LESS_MASK  (0x400U)
#define SYSCTL2_AVPLL_CTRL2_PLL_LPFC2_LESS_SHIFT (10U)
/*! PLL_LPFC2_LESS - LPF C2 Capacitor Value Select */
#define SYSCTL2_AVPLL_CTRL2_PLL_LPFC2_LESS(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PLL_LPFC2_LESS_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PLL_LPFC2_LESS_MASK)

#define SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV_MASK  (0xF800U)
#define SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV_SHIFT (11U)
/*! PLL_CALCLK_DIV - Divider Settings to Generate Calibration Clock. */
#define SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PLL_CALCLK_DIV_MASK)

#define SYSCTL2_AVPLL_CTRL2_PLL_CAL_START_MASK   (0x10000U)
#define SYSCTL2_AVPLL_CTRL2_PLL_CAL_START_SHIFT  (16U)
/*! PLL_CAL_START - PLL Calibration Start. */
#define SYSCTL2_AVPLL_CTRL2_PLL_CAL_START(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_PLL_CAL_START_SHIFT)) & SYSCTL2_AVPLL_CTRL2_PLL_CAL_START_MASK)

#define SYSCTL2_AVPLL_CTRL2_MODE_MASK            (0x60000U)
#define SYSCTL2_AVPLL_CTRL2_MODE_SHIFT           (17U)
/*! MODE - Feedback Clock for PLL Select */
#define SYSCTL2_AVPLL_CTRL2_MODE(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_MODE_SHIFT)) & SYSCTL2_AVPLL_CTRL2_MODE_MASK)

#define SYSCTL2_AVPLL_CTRL2_MASTER_SLAVEB_MASK   (0x80000U)
#define SYSCTL2_AVPLL_CTRL2_MASTER_SLAVEB_SHIFT  (19U)
/*! MASTER_SLAVEB - This signal synchronizes frequencies of the two PLL¡¯s that are being used */
#define SYSCTL2_AVPLL_CTRL2_MASTER_SLAVEB(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_MASTER_SLAVEB_SHIFT)) & SYSCTL2_AVPLL_CTRL2_MASTER_SLAVEB_MASK)

#define SYSCTL2_AVPLL_CTRL2_INTPR_MASK           (0x700000U)
#define SYSCTL2_AVPLL_CTRL2_INTPR_SHIFT          (20U)
/*! INTPR - PI Rload Resistor Select */
#define SYSCTL2_AVPLL_CTRL2_INTPR(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_INTPR_SHIFT)) & SYSCTL2_AVPLL_CTRL2_INTPR_MASK)

#define SYSCTL2_AVPLL_CTRL2_INTPI_MASK           (0x7800000U)
#define SYSCTL2_AVPLL_CTRL2_INTPI_SHIFT          (23U)
/*! INTPI - PI Bias Current Select */
#define SYSCTL2_AVPLL_CTRL2_INTPI(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_INTPI_SHIFT)) & SYSCTL2_AVPLL_CTRL2_INTPI_MASK)

#define SYSCTL2_AVPLL_CTRL2_ICP_MASK             (0x78000000U)
#define SYSCTL2_AVPLL_CTRL2_ICP_SHIFT            (27U)
/*! ICP - Charge Pump Current Control Bits */
#define SYSCTL2_AVPLL_CTRL2_ICP(x)               (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL2_ICP_SHIFT)) & SYSCTL2_AVPLL_CTRL2_ICP_MASK)
/*! @} */

/*! @name AVPLL_CTRL3 - Audio PLL Control register3 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL3_REG_SETTLE_LIMIT_MASK (0xFU)
#define SYSCTL2_AVPLL_CTRL3_REG_SETTLE_LIMIT_SHIFT (0U)
/*! REG_SETTLE_LIMIT - Waiting Time Select Before Calibration Start.. */
#define SYSCTL2_AVPLL_CTRL3_REG_SETTLE_LIMIT(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL3_REG_SETTLE_LIMIT_SHIFT)) & SYSCTL2_AVPLL_CTRL3_REG_SETTLE_LIMIT_MASK)

#define SYSCTL2_AVPLL_CTRL3_REG_RING_EXTRA_I_EN_MASK (0x10U)
#define SYSCTL2_AVPLL_CTRL3_REG_RING_EXTRA_I_EN_SHIFT (4U)
/*! REG_RING_EXTRA_I_EN - Extra Current Turn On Select. */
#define SYSCTL2_AVPLL_CTRL3_REG_RING_EXTRA_I_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL3_REG_RING_EXTRA_I_EN_SHIFT)) & SYSCTL2_AVPLL_CTRL3_REG_RING_EXTRA_I_EN_MASK)

#define SYSCTL2_AVPLL_CTRL3_REFDIV_MASK          (0xFE0U)
#define SYSCTL2_AVPLL_CTRL3_REFDIV_SHIFT         (5U)
/*! REFDIV - Reference Clock Divider Select */
#define SYSCTL2_AVPLL_CTRL3_REFDIV(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL3_REFDIV_SHIFT)) & SYSCTL2_AVPLL_CTRL3_REFDIV_MASK)

#define SYSCTL2_AVPLL_CTRL3_P_SYNC1_C1_MASK      (0xFFFFF000U)
#define SYSCTL2_AVPLL_CTRL3_P_SYNC1_C1_SHIFT     (12U)
/*! P_SYNC1_C1 - Set DPLL¡¯s Reference Divider */
#define SYSCTL2_AVPLL_CTRL3_P_SYNC1_C1(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL3_P_SYNC1_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL3_P_SYNC1_C1_MASK)
/*! @} */

/*! @name AVPLL_CTRL4 - Audio PLL Control register4 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL4_SLLP_EN_DIS_MASK     (0x1U)
#define SYSCTL2_AVPLL_CTRL4_SLLP_EN_DIS_SHIFT    (0U)
/*! SLLP_EN_DIS - Slow Loop Select. */
#define SYSCTL2_AVPLL_CTRL4_SLLP_EN_DIS(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL4_SLLP_EN_DIS_SHIFT)) & SYSCTL2_AVPLL_CTRL4_SLLP_EN_DIS_MASK)

#define SYSCTL2_AVPLL_CTRL4_SLLP_CLK_DIV5EN_MASK (0x2U)
#define SYSCTL2_AVPLL_CTRL4_SLLP_CLK_DIV5EN_SHIFT (1U)
/*! SLLP_CLK_DIV5EN - Slow Loop Clock Enable */
#define SYSCTL2_AVPLL_CTRL4_SLLP_CLK_DIV5EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL4_SLLP_CLK_DIV5EN_SHIFT)) & SYSCTL2_AVPLL_CTRL4_SLLP_CLK_DIV5EN_MASK)

#define SYSCTL2_AVPLL_CTRL4_SEL_VTHVCOCONT_MASK  (0x4U)
#define SYSCTL2_AVPLL_CTRL4_SEL_VTHVCOCONT_SHIFT (2U)
/*! SEL_VTHVCOCONT - Select Threshold Source for Calibrated VDDVCO Voltage */
#define SYSCTL2_AVPLL_CTRL4_SEL_VTHVCOCONT(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL4_SEL_VTHVCOCONT_SHIFT)) & SYSCTL2_AVPLL_CTRL4_SEL_VTHVCOCONT_MASK)

#define SYSCTL2_AVPLL_CTRL4_RESERVE_PLL_IN_MASK  (0x1F8U)
#define SYSCTL2_AVPLL_CTRL4_RESERVE_PLL_IN_SHIFT (3U)
/*! RESERVE_PLL_IN - Reserved pins */
#define SYSCTL2_AVPLL_CTRL4_RESERVE_PLL_IN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL4_RESERVE_PLL_IN_SHIFT)) & SYSCTL2_AVPLL_CTRL4_RESERVE_PLL_IN_MASK)

#define SYSCTL2_AVPLL_CTRL4_RESERVE_IN_C1_MASK   (0x600U)
#define SYSCTL2_AVPLL_CTRL4_RESERVE_IN_C1_SHIFT  (9U)
/*! RESERVE_IN_C1 - Reserved pins */
#define SYSCTL2_AVPLL_CTRL4_RESERVE_IN_C1(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL4_RESERVE_IN_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL4_RESERVE_IN_C1_MASK)

#define SYSCTL2_AVPLL_CTRL4_P_SYNC2_C1_MASK      (0x7FFFF800U)
#define SYSCTL2_AVPLL_CTRL4_P_SYNC2_C1_SHIFT     (11U)
/*! P_SYNC2_C1 - Set DPLL¡¯s Feedback Divider */
#define SYSCTL2_AVPLL_CTRL4_P_SYNC2_C1(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL4_P_SYNC2_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL4_P_SYNC2_C1_MASK)
/*! @} */

/*! @name AVPLL_CTRL5 - Audio PLL Control register5 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL5_TEST_MON_MASK        (0x3FU)
#define SYSCTL2_AVPLL_CTRL5_TEST_MON_SHIFT       (0U)
/*! TEST_MON - DC Test Point Register. For Internal Use Only. */
#define SYSCTL2_AVPLL_CTRL5_TEST_MON(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL5_TEST_MON_SHIFT)) & SYSCTL2_AVPLL_CTRL5_TEST_MON_MASK)

#define SYSCTL2_AVPLL_CTRL5_SPEED_THRESH_MASK    (0xFC0U)
#define SYSCTL2_AVPLL_CTRL5_SPEED_THRESH_SHIFT   (6U)
/*! SPEED_THRESH - Threshold for VCO Speed Setting Calibration. */
#define SYSCTL2_AVPLL_CTRL5_SPEED_THRESH(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL5_SPEED_THRESH_SHIFT)) & SYSCTL2_AVPLL_CTRL5_SPEED_THRESH_MASK)

#define SYSCTL2_AVPLL_CTRL5_SPEED_FBRES_MASK     (0xF000U)
#define SYSCTL2_AVPLL_CTRL5_SPEED_FBRES_SHIFT    (12U)
/*! SPEED_FBRES - External feedback resistor (VCO ring) set up bits */
#define SYSCTL2_AVPLL_CTRL5_SPEED_FBRES(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL5_SPEED_FBRES_SHIFT)) & SYSCTL2_AVPLL_CTRL5_SPEED_FBRES_MASK)

#define SYSCTL2_AVPLL_CTRL5_SLLP_PSF_LEVEL_MASK  (0x70000U)
#define SYSCTL2_AVPLL_CTRL5_SLLP_PSF_LEVEL_SHIFT (16U)
/*! SLLP_PSF_LEVEL - Slow Loop Current Generate. */
#define SYSCTL2_AVPLL_CTRL5_SLLP_PSF_LEVEL(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL5_SLLP_PSF_LEVEL_SHIFT)) & SYSCTL2_AVPLL_CTRL5_SLLP_PSF_LEVEL_MASK)

#define SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_MASK      (0xFFF80000U)
#define SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_SHIFT     (19U)
/*! POSTDIV_C1 - Audio Clock Divider Program Set */
#define SYSCTL2_AVPLL_CTRL5_POSTDIV_C1(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL5_POSTDIV_C1_MASK)
/*! @} */

/*! @name AVPLL_CTRL6 - Audio PLL Control register6 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL6_VTH_VCO_PTAT_MASK    (0x3U)
#define SYSCTL2_AVPLL_CTRL6_VTH_VCO_PTAT_SHIFT   (0U)
/*! VTH_VCO_PTAT - IPTAT Current to Generate VDDVCO Voltage Select */
#define SYSCTL2_AVPLL_CTRL6_VTH_VCO_PTAT(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VTH_VCO_PTAT_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VTH_VCO_PTAT_MASK)

#define SYSCTL2_AVPLL_CTRL6_VTH_VCO_CAL_MASK     (0xCU)
#define SYSCTL2_AVPLL_CTRL6_VTH_VCO_CAL_SHIFT    (2U)
/*! VTH_VCO_CAL - VDDVCO Voltage Threshold Select */
#define SYSCTL2_AVPLL_CTRL6_VTH_VCO_CAL(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VTH_VCO_CAL_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VTH_VCO_CAL_MASK)

#define SYSCTL2_AVPLL_CTRL6_VDDL_MASK            (0xF0U)
#define SYSCTL2_AVPLL_CTRL6_VDDL_SHIFT           (4U)
/*! VDDL - Internal Regulated VDD Supply Voltage Control */
#define SYSCTL2_AVPLL_CTRL6_VDDL(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VDDL_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VDDL_MASK)

#define SYSCTL2_AVPLL_CTRL6_VDDBUF_ADJ_MASK      (0x700U)
#define SYSCTL2_AVPLL_CTRL6_VDDBUF_ADJ_SHIFT     (8U)
/*! VDDBUF_ADJ - VDDVDOFBUF Voltage Level Adjust */
#define SYSCTL2_AVPLL_CTRL6_VDDBUF_ADJ(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VDDBUF_ADJ_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VDDBUF_ADJ_MASK)

#define SYSCTL2_AVPLL_CTRL6_VDDA23_PUMP_SEL_MASK (0x1800U)
#define SYSCTL2_AVPLL_CTRL6_VDDA23_PUMP_SEL_SHIFT (11U)
/*! VDDA23_PUMP_SEL - VREF0P96_VDDA23 PUMP Select */
#define SYSCTL2_AVPLL_CTRL6_VDDA23_PUMP_SEL(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VDDA23_PUMP_SEL_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VDDA23_PUMP_SEL_MASK)

#define SYSCTL2_AVPLL_CTRL6_VCON_SEL_MASK        (0x6000U)
#define SYSCTL2_AVPLL_CTRL6_VCON_SEL_SHIFT       (13U)
/*! VCON_SEL - VCON Value Set. */
#define SYSCTL2_AVPLL_CTRL6_VCON_SEL(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VCON_SEL_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VCON_SEL_MASK)

#define SYSCTL2_AVPLL_CTRL6_VCO_REF1P45_SEL_MASK (0x18000U)
#define SYSCTL2_AVPLL_CTRL6_VCO_REF1P45_SEL_SHIFT (15U)
/*! VCO_REF1P45_SEL - VREF1P0V_VCO1P45 Value Select. */
#define SYSCTL2_AVPLL_CTRL6_VCO_REF1P45_SEL(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_VCO_REF1P45_SEL_SHIFT)) & SYSCTL2_AVPLL_CTRL6_VCO_REF1P45_SEL_MASK)

#define SYSCTL2_AVPLL_CTRL6_UPDATE_SEL_MASK      (0x20000U)
#define SYSCTL2_AVPLL_CTRL6_UPDATE_SEL_SHIFT     (17U)
/*! UPDATE_SEL - PLL Update Rate Select */
#define SYSCTL2_AVPLL_CTRL6_UPDATE_SEL(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL6_UPDATE_SEL_SHIFT)) & SYSCTL2_AVPLL_CTRL6_UPDATE_SEL_MASK)
/*! @} */

/*! @name AVPLL_CTRL7 - Audio PLL Control register7 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL7_PLL_LOCK_MASK        (0x1000000U)
#define SYSCTL2_AVPLL_CTRL7_PLL_LOCK_SHIFT       (24U)
/*! PLL_LOCK - Lock Detect Output */
#define SYSCTL2_AVPLL_CTRL7_PLL_LOCK(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL7_PLL_LOCK_SHIFT)) & SYSCTL2_AVPLL_CTRL7_PLL_LOCK_MASK)

#define SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE_MASK    (0x2000000U)
#define SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE_SHIFT   (25U)
/*! PLL_CAL_DONE - Rising edge to indicate the end of PLL calibration */
#define SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE_SHIFT)) & SYSCTL2_AVPLL_CTRL7_PLL_CAL_DONE_MASK)
/*! @} */

/*! @name AVPLL_CTRL8 - Audio PLL Control register8 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8_MASK  (0x1U)
#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8_SHIFT (0U)
/*! AVPLL_RESET_C8 - C8 SW Reset . Active high */
#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C8_MASK)

#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1_MASK  (0x2U)
#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1_SHIFT (1U)
/*! AVPLL_RESET_C1 - C1 SW Reset . Active high */
#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1_SHIFT)) & SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_C1_MASK)

#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_MASK     (0x4U)
#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_SHIFT    (2U)
/*! AVPLL_RESET - SW Reset . Active high */
#define SYSCTL2_AVPLL_CTRL8_AVPLL_RESET(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_SHIFT)) & SYSCTL2_AVPLL_CTRL8_AVPLL_RESET_MASK)

#define SYSCTL2_AVPLL_CTRL8_RESERVE_IN_C8_MASK   (0x18U)
#define SYSCTL2_AVPLL_CTRL8_RESERVE_IN_C8_SHIFT  (3U)
/*! RESERVE_IN_C8 - Reserved pins */
#define SYSCTL2_AVPLL_CTRL8_RESERVE_IN_C8(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_RESERVE_IN_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_RESERVE_IN_C8_MASK)

#define SYSCTL2_AVPLL_CTRL8_PU_OFST_CTRL_C8_MASK (0x20U)
#define SYSCTL2_AVPLL_CTRL8_PU_OFST_CTRL_C8_SHIFT (5U)
/*! PU_OFST_CTRL_C8 - Power Up/Down FREQ_OFFSET Integrator of CX */
#define SYSCTL2_AVPLL_CTRL8_PU_OFST_CTRL_C8(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_PU_OFST_CTRL_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_PU_OFST_CTRL_C8_MASK)

#define SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8_MASK (0x40U)
#define SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8_SHIFT (6U)
/*! FREQ_OFFSET_READY_C8 - Indicate Frequency Offset Value Readiness. */
#define SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_READY_C8_MASK)

#define SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8_MASK  (0x3FFFF80U)
#define SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8_SHIFT (7U)
/*! FREQ_OFFSET_C8 - FREQ_OFFSET_CX[18:0] Set */
#define SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_FREQ_OFFSET_C8_MASK)

#define SYSCTL2_AVPLL_CTRL8_EN_LP_C8_MASK        (0xC000000U)
#define SYSCTL2_AVPLL_CTRL8_EN_LP_C8_SHIFT       (26U)
/*! EN_LP_C8 - Channel CX LP Enable. */
#define SYSCTL2_AVPLL_CTRL8_EN_LP_C8(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_EN_LP_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_EN_LP_C8_MASK)

#define SYSCTL2_AVPLL_CTRL8_EN_DPLL_C8_MASK      (0x10000000U)
#define SYSCTL2_AVPLL_CTRL8_EN_DPLL_C8_SHIFT     (28U)
/*! EN_DPLL_C8 - Enable/ Disable Channel CX¡¯s DPLL */
#define SYSCTL2_AVPLL_CTRL8_EN_DPLL_C8(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL8_EN_DPLL_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL8_EN_DPLL_C8_MASK)
/*! @} */

/*! @name AVPLL_CTRL9 - Audio PLL Control register9 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL9_P_SYNC1_C8_MASK      (0xFFFFFU)
#define SYSCTL2_AVPLL_CTRL9_P_SYNC1_C8_SHIFT     (0U)
/*! P_SYNC1_C8 - Set DPLL¡¯s Reference Divider */
#define SYSCTL2_AVPLL_CTRL9_P_SYNC1_C8(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL9_P_SYNC1_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL9_P_SYNC1_C8_MASK)
/*! @} */

/*! @name AVPLL_CTRL10 - Audio PLL Control register10 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL10_P_SYNC2_C8_MASK     (0xFFFFFU)
#define SYSCTL2_AVPLL_CTRL10_P_SYNC2_C8_SHIFT    (0U)
/*! P_SYNC2_C8 - Set DPLL¡¯s Feedback Divider */
#define SYSCTL2_AVPLL_CTRL10_P_SYNC2_C8(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL10_P_SYNC2_C8_SHIFT)) & SYSCTL2_AVPLL_CTRL10_P_SYNC2_C8_MASK)
/*! @} */

/*! @name AVPLL_CTRL11 - Audio PLL Control register11 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL11_FREQ_OFFSET_C2_MASK (0x7FFFFU)
#define SYSCTL2_AVPLL_CTRL11_FREQ_OFFSET_C2_SHIFT (0U)
/*! FREQ_OFFSET_C2 - FREQ_OFFSET_CX[18:0] Set */
#define SYSCTL2_AVPLL_CTRL11_FREQ_OFFSET_C2(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL11_FREQ_OFFSET_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL11_FREQ_OFFSET_C2_MASK)

#define SYSCTL2_AVPLL_CTRL11_POSTDIV_C2_MASK     (0xFFF80000U)
#define SYSCTL2_AVPLL_CTRL11_POSTDIV_C2_SHIFT    (19U)
/*! POSTDIV_C2 - Audio Clock Divider Program Set */
#define SYSCTL2_AVPLL_CTRL11_POSTDIV_C2(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL11_POSTDIV_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL11_POSTDIV_C2_MASK)
/*! @} */

/*! @name AVPLL_CTRL12 - Audio PLL Control register12 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL12_P_SYNC1_C2_MASK     (0xFFFFFU)
#define SYSCTL2_AVPLL_CTRL12_P_SYNC1_C2_SHIFT    (0U)
/*! P_SYNC1_C2 - Set DPLL¡¯s Reference Divider */
#define SYSCTL2_AVPLL_CTRL12_P_SYNC1_C2(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_P_SYNC1_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_P_SYNC1_C2_MASK)

#define SYSCTL2_AVPLL_CTRL12_EN_LP_C2_MASK       (0x300000U)
#define SYSCTL2_AVPLL_CTRL12_EN_LP_C2_SHIFT      (20U)
/*! EN_LP_C2 - Channel CX LP Enable. */
#define SYSCTL2_AVPLL_CTRL12_EN_LP_C2(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_EN_LP_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_EN_LP_C2_MASK)

#define SYSCTL2_AVPLL_CTRL12_EN_DPLL_C2_MASK     (0x400000U)
#define SYSCTL2_AVPLL_CTRL12_EN_DPLL_C2_SHIFT    (22U)
/*! EN_DPLL_C2 - Enable/ Disable Channel CX¡¯s DPLL */
#define SYSCTL2_AVPLL_CTRL12_EN_DPLL_C2(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_EN_DPLL_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_EN_DPLL_C2_MASK)

#define SYSCTL2_AVPLL_CTRL12_PU_OFST_CTRL_C2_MASK (0x800000U)
#define SYSCTL2_AVPLL_CTRL12_PU_OFST_CTRL_C2_SHIFT (23U)
/*! PU_OFST_CTRL_C2 - Power Up/Down FREQ_OFFSET Integrator of CX */
#define SYSCTL2_AVPLL_CTRL12_PU_OFST_CTRL_C2(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_PU_OFST_CTRL_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_PU_OFST_CTRL_C2_MASK)

#define SYSCTL2_AVPLL_CTRL12_PU_C2_MASK          (0x1000000U)
#define SYSCTL2_AVPLL_CTRL12_PU_C2_SHIFT         (24U)
/*! PU_C2 - Power Up/Down Channel CX */
#define SYSCTL2_AVPLL_CTRL12_PU_C2(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_PU_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_PU_C2_MASK)

#define SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2_MASK (0x2000000U)
#define SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2_SHIFT (25U)
/*! POSTDIV_0P5_C2 - Audio Clock Divider Program Set */
#define SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_POSTDIV_0P5_C2_MASK)

#define SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2_MASK (0x4000000U)
#define SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2_SHIFT (26U)
/*! FREQ_OFFSET_READY_C2 - Indicate Frequency Offset Value Readiness. */
#define SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL12_FREQ_OFFSET_READY_C2_MASK)
/*! @} */

/*! @name GAU_CTRL - GAU Control register */
/*! @{ */

#define SYSCTL2_GAU_CTRL_GAU_GPDAC_MCLK_EN_MASK  (0x1U)
#define SYSCTL2_GAU_CTRL_GAU_GPDAC_MCLK_EN_SHIFT (0U)
/*! GAU_GPDAC_MCLK_EN - gau gpdac mclk enable */
#define SYSCTL2_GAU_CTRL_GAU_GPDAC_MCLK_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_GAU_CTRL_GAU_GPDAC_MCLK_EN_SHIFT)) & SYSCTL2_GAU_CTRL_GAU_GPDAC_MCLK_EN_MASK)

#define SYSCTL2_GAU_CTRL_GAU_BG_MCLK_EN_MASK     (0x2U)
#define SYSCTL2_GAU_CTRL_GAU_BG_MCLK_EN_SHIFT    (1U)
/*! GAU_BG_MCLK_EN - gau bg mclk enable */
#define SYSCTL2_GAU_CTRL_GAU_BG_MCLK_EN(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_GAU_CTRL_GAU_BG_MCLK_EN_SHIFT)) & SYSCTL2_GAU_CTRL_GAU_BG_MCLK_EN_MASK)

#define SYSCTL2_GAU_CTRL_GAU_GPADC1_MCLK_EN_MASK (0x4U)
#define SYSCTL2_GAU_CTRL_GAU_GPADC1_MCLK_EN_SHIFT (2U)
/*! GAU_GPADC1_MCLK_EN - gau gpadc1 mclk enable */
#define SYSCTL2_GAU_CTRL_GAU_GPADC1_MCLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_GAU_CTRL_GAU_GPADC1_MCLK_EN_SHIFT)) & SYSCTL2_GAU_CTRL_GAU_GPADC1_MCLK_EN_MASK)

#define SYSCTL2_GAU_CTRL_GAU_GPADC0_MCLK_EN_MASK (0x8U)
#define SYSCTL2_GAU_CTRL_GAU_GPADC0_MCLK_EN_SHIFT (3U)
/*! GAU_GPADC0_MCLK_EN - gau gpadc0 mclk enable */
#define SYSCTL2_GAU_CTRL_GAU_GPADC0_MCLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_GAU_CTRL_GAU_GPADC0_MCLK_EN_SHIFT)) & SYSCTL2_GAU_CTRL_GAU_GPADC0_MCLK_EN_MASK)

#define SYSCTL2_GAU_CTRL_GAU_ACOMP_MCLK_EN_MASK  (0x10U)
#define SYSCTL2_GAU_CTRL_GAU_ACOMP_MCLK_EN_SHIFT (4U)
/*! GAU_ACOMP_MCLK_EN - gau acomp mclk enable */
#define SYSCTL2_GAU_CTRL_GAU_ACOMP_MCLK_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_GAU_CTRL_GAU_ACOMP_MCLK_EN_SHIFT)) & SYSCTL2_GAU_CTRL_GAU_ACOMP_MCLK_EN_MASK)
/*! @} */

/*! @name CTIMER_CTRL - CTIMER Control register */
/*! @{ */

#define SYSCTL2_CTIMER_CTRL_CT0_GLOBAL_ENABLE_MASK (0x1U)
#define SYSCTL2_CTIMER_CTRL_CT0_GLOBAL_ENABLE_SHIFT (0U)
/*! CT0_GLOBAL_ENABLE - ctimer0 global_enable */
#define SYSCTL2_CTIMER_CTRL_CT0_GLOBAL_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT0_GLOBAL_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT0_GLOBAL_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT0_TRIGGER_ENABLE_MASK (0x2U)
#define SYSCTL2_CTIMER_CTRL_CT0_TRIGGER_ENABLE_SHIFT (1U)
/*! CT0_TRIGGER_ENABLE - ctimer0 trigger_enable */
#define SYSCTL2_CTIMER_CTRL_CT0_TRIGGER_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT0_TRIGGER_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT0_TRIGGER_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT1_GLOBAL_ENABLE_MASK (0x4U)
#define SYSCTL2_CTIMER_CTRL_CT1_GLOBAL_ENABLE_SHIFT (2U)
/*! CT1_GLOBAL_ENABLE - ctimer1 global_enable */
#define SYSCTL2_CTIMER_CTRL_CT1_GLOBAL_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT1_GLOBAL_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT1_GLOBAL_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT1_TRIGGER_ENABLE_MASK (0x8U)
#define SYSCTL2_CTIMER_CTRL_CT1_TRIGGER_ENABLE_SHIFT (3U)
/*! CT1_TRIGGER_ENABLE - ctimer1 trigger_enable */
#define SYSCTL2_CTIMER_CTRL_CT1_TRIGGER_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT1_TRIGGER_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT1_TRIGGER_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT2_GLOBAL_ENABLE_MASK (0x10U)
#define SYSCTL2_CTIMER_CTRL_CT2_GLOBAL_ENABLE_SHIFT (4U)
/*! CT2_GLOBAL_ENABLE - ctimer2 global_enable */
#define SYSCTL2_CTIMER_CTRL_CT2_GLOBAL_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT2_GLOBAL_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT2_GLOBAL_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT2_TRIGGER_ENABLE_MASK (0x20U)
#define SYSCTL2_CTIMER_CTRL_CT2_TRIGGER_ENABLE_SHIFT (5U)
/*! CT2_TRIGGER_ENABLE - ctimer2 trigger_enable */
#define SYSCTL2_CTIMER_CTRL_CT2_TRIGGER_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT2_TRIGGER_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT2_TRIGGER_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT3_GLOBAL_ENABLE_MASK (0x40U)
#define SYSCTL2_CTIMER_CTRL_CT3_GLOBAL_ENABLE_SHIFT (6U)
/*! CT3_GLOBAL_ENABLE - ctimer3 global_enable */
#define SYSCTL2_CTIMER_CTRL_CT3_GLOBAL_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT3_GLOBAL_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT3_GLOBAL_ENABLE_MASK)

#define SYSCTL2_CTIMER_CTRL_CT3_TRIGGER_ENABLE_MASK (0x80U)
#define SYSCTL2_CTIMER_CTRL_CT3_TRIGGER_ENABLE_SHIFT (7U)
/*! CT3_TRIGGER_ENABLE - ctimer3 trigger_enable */
#define SYSCTL2_CTIMER_CTRL_CT3_TRIGGER_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CTIMER_CTRL_CT3_TRIGGER_ENABLE_SHIFT)) & SYSCTL2_CTIMER_CTRL_CT3_TRIGGER_ENABLE_MASK)
/*! @} */

/*! @name EXT_H2H_CTRL - AHB async bridge Control register */
/*! @{ */

#define SYSCTL2_EXT_H2H_CTRL_WLAN_H2H_PREFETCH_EN_MASK (0x1U)
#define SYSCTL2_EXT_H2H_CTRL_WLAN_H2H_PREFETCH_EN_SHIFT (0U)
/*! WLAN_H2H_PREFETCH_EN - wlan_h2h_prefetch_en */
#define SYSCTL2_EXT_H2H_CTRL_WLAN_H2H_PREFETCH_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_EXT_H2H_CTRL_WLAN_H2H_PREFETCH_EN_SHIFT)) & SYSCTL2_EXT_H2H_CTRL_WLAN_H2H_PREFETCH_EN_MASK)

#define SYSCTL2_EXT_H2H_CTRL_BLE_H2H_PREFETCH_EN_MASK (0x2U)
#define SYSCTL2_EXT_H2H_CTRL_BLE_H2H_PREFETCH_EN_SHIFT (1U)
/*! BLE_H2H_PREFETCH_EN - ble_h2h_prefetch_en */
#define SYSCTL2_EXT_H2H_CTRL_BLE_H2H_PREFETCH_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_EXT_H2H_CTRL_BLE_H2H_PREFETCH_EN_SHIFT)) & SYSCTL2_EXT_H2H_CTRL_BLE_H2H_PREFETCH_EN_MASK)
/*! @} */

/*! @name RAM_CTRL1 - RAM Memory Control Register 1 */
/*! @{ */

#define SYSCTL2_RAM_CTRL1_PKC_RTC_MASK           (0x3U)
#define SYSCTL2_RAM_CTRL1_PKC_RTC_SHIFT          (0U)
/*! PKC_RTC - pkc_rtc */
#define SYSCTL2_RAM_CTRL1_PKC_RTC(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_PKC_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_PKC_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_PKC_WTC_MASK           (0xCU)
#define SYSCTL2_RAM_CTRL1_PKC_WTC_SHIFT          (2U)
/*! PKC_WTC - pkc_wtc */
#define SYSCTL2_RAM_CTRL1_PKC_WTC(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_PKC_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_PKC_WTC_MASK)

#define SYSCTL2_RAM_CTRL1_PQ_RTC_MASK            (0x30U)
#define SYSCTL2_RAM_CTRL1_PQ_RTC_SHIFT           (4U)
/*! PQ_RTC - pq_rtc */
#define SYSCTL2_RAM_CTRL1_PQ_RTC(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_PQ_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_PQ_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_PQ_WTC_MASK            (0xC0U)
#define SYSCTL2_RAM_CTRL1_PQ_WTC_SHIFT           (6U)
/*! PQ_WTC - pq_wtc */
#define SYSCTL2_RAM_CTRL1_PQ_WTC(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_PQ_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_PQ_WTC_MASK)

#define SYSCTL2_RAM_CTRL1_S0_SRAM_RTC_MASK       (0x300U)
#define SYSCTL2_RAM_CTRL1_S0_SRAM_RTC_SHIFT      (8U)
/*! S0_SRAM_RTC - s0_sram_rtc */
#define SYSCTL2_RAM_CTRL1_S0_SRAM_RTC(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_S0_SRAM_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_S0_SRAM_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_S0_SRAM_WTC_MASK       (0xC00U)
#define SYSCTL2_RAM_CTRL1_S0_SRAM_WTC_SHIFT      (10U)
/*! S0_SRAM_WTC - s0_sram_wtc */
#define SYSCTL2_RAM_CTRL1_S0_SRAM_WTC(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_S0_SRAM_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_S0_SRAM_WTC_MASK)

#define SYSCTL2_RAM_CTRL1_SDU_CIS_RTC_MASK       (0x3000U)
#define SYSCTL2_RAM_CTRL1_SDU_CIS_RTC_SHIFT      (12U)
/*! SDU_CIS_RTC - sdu_cis_rtc */
#define SYSCTL2_RAM_CTRL1_SDU_CIS_RTC(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_SDU_CIS_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_SDU_CIS_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_SDU_CIS_WTC_MASK       (0xC000U)
#define SYSCTL2_RAM_CTRL1_SDU_CIS_WTC_SHIFT      (14U)
/*! SDU_CIS_WTC - sdu_cis_wtc */
#define SYSCTL2_RAM_CTRL1_SDU_CIS_WTC(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_SDU_CIS_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_SDU_CIS_WTC_MASK)

#define SYSCTL2_RAM_CTRL1_SDU_RX_RTC_MASK        (0x30000U)
#define SYSCTL2_RAM_CTRL1_SDU_RX_RTC_SHIFT       (16U)
/*! SDU_RX_RTC - sdu_rx_rtc */
#define SYSCTL2_RAM_CTRL1_SDU_RX_RTC(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_SDU_RX_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_SDU_RX_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_SDU_RX_WTC_MASK        (0xC0000U)
#define SYSCTL2_RAM_CTRL1_SDU_RX_WTC_SHIFT       (18U)
/*! SDU_RX_WTC - sdu_rx_wtc */
#define SYSCTL2_RAM_CTRL1_SDU_RX_WTC(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_SDU_RX_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_SDU_RX_WTC_MASK)

#define SYSCTL2_RAM_CTRL1_SDU_TX_RTC_MASK        (0x300000U)
#define SYSCTL2_RAM_CTRL1_SDU_TX_RTC_SHIFT       (20U)
/*! SDU_TX_RTC - sdu_tx_rtc */
#define SYSCTL2_RAM_CTRL1_SDU_TX_RTC(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_SDU_TX_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_SDU_TX_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_SDU_TX_WTC_MASK        (0xC00000U)
#define SYSCTL2_RAM_CTRL1_SDU_TX_WTC_SHIFT       (22U)
/*! SDU_TX_WTC - sdu_tx_wtc */
#define SYSCTL2_RAM_CTRL1_SDU_TX_WTC(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_SDU_TX_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_SDU_TX_WTC_MASK)

#define SYSCTL2_RAM_CTRL1_ENET_RTC_MASK          (0x3000000U)
#define SYSCTL2_RAM_CTRL1_ENET_RTC_SHIFT         (24U)
/*! ENET_RTC - enet_rtc */
#define SYSCTL2_RAM_CTRL1_ENET_RTC(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_ENET_RTC_SHIFT)) & SYSCTL2_RAM_CTRL1_ENET_RTC_MASK)

#define SYSCTL2_RAM_CTRL1_ENET_WTC_MASK          (0xC000000U)
#define SYSCTL2_RAM_CTRL1_ENET_WTC_SHIFT         (26U)
/*! ENET_WTC - enet_wtc */
#define SYSCTL2_RAM_CTRL1_ENET_WTC(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RAM_CTRL1_ENET_WTC_SHIFT)) & SYSCTL2_RAM_CTRL1_ENET_WTC_MASK)
/*! @} */

/*! @name ROM_CTRL - ROM Control Register */
/*! @{ */

#define SYSCTL2_ROM_CTRL_ELS_RTC_MASK            (0x7U)
#define SYSCTL2_ROM_CTRL_ELS_RTC_SHIFT           (0U)
/*! ELS_RTC - els_rtc */
#define SYSCTL2_ROM_CTRL_ELS_RTC(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_CTRL_ELS_RTC_SHIFT)) & SYSCTL2_ROM_CTRL_ELS_RTC_MASK)

#define SYSCTL2_ROM_CTRL_ELS_RTC_REF_MASK        (0x18U)
#define SYSCTL2_ROM_CTRL_ELS_RTC_REF_SHIFT       (3U)
/*! ELS_RTC_REF - els_rtc_ref */
#define SYSCTL2_ROM_CTRL_ELS_RTC_REF(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_CTRL_ELS_RTC_REF_SHIFT)) & SYSCTL2_ROM_CTRL_ELS_RTC_REF_MASK)

#define SYSCTL2_ROM_CTRL_S0_ROM_RTC_MASK         (0xE0U)
#define SYSCTL2_ROM_CTRL_S0_ROM_RTC_SHIFT        (5U)
/*! S0_ROM_RTC - s0_rom_rtc */
#define SYSCTL2_ROM_CTRL_S0_ROM_RTC(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_CTRL_S0_ROM_RTC_SHIFT)) & SYSCTL2_ROM_CTRL_S0_ROM_RTC_MASK)

#define SYSCTL2_ROM_CTRL_S0_ROM_RTC_REF_MASK     (0x300U)
#define SYSCTL2_ROM_CTRL_S0_ROM_RTC_REF_SHIFT    (8U)
/*! S0_ROM_RTC_REF - s0_rom_rtc_ref */
#define SYSCTL2_ROM_CTRL_S0_ROM_RTC_REF(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_CTRL_S0_ROM_RTC_REF_SHIFT)) & SYSCTL2_ROM_CTRL_S0_ROM_RTC_REF_MASK)
/*! @} */

/*! @name MEM_PD_CTRL - MEM PD Control enable register when PM2 mode */
/*! @{ */

#define SYSCTL2_MEM_PD_CTRL_SRAM0_SW_CTRL_EN_MASK (0x1U)
#define SYSCTL2_MEM_PD_CTRL_SRAM0_SW_CTRL_EN_SHIFT (0U)
/*! SRAM0_SW_CTRL_EN - SW control sram0 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM0_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM0_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM0_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM1_SW_CTRL_EN_MASK (0x2U)
#define SYSCTL2_MEM_PD_CTRL_SRAM1_SW_CTRL_EN_SHIFT (1U)
/*! SRAM1_SW_CTRL_EN - SW control sram1 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM1_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM1_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM1_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM2_SW_CTRL_EN_MASK (0x4U)
#define SYSCTL2_MEM_PD_CTRL_SRAM2_SW_CTRL_EN_SHIFT (2U)
/*! SRAM2_SW_CTRL_EN - SW control sram2 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM2_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM2_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM2_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM3_SW_CTRL_EN_MASK (0x8U)
#define SYSCTL2_MEM_PD_CTRL_SRAM3_SW_CTRL_EN_SHIFT (3U)
/*! SRAM3_SW_CTRL_EN - SW control sram3 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM3_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM3_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM3_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM4_SW_CTRL_EN_MASK (0x10U)
#define SYSCTL2_MEM_PD_CTRL_SRAM4_SW_CTRL_EN_SHIFT (4U)
/*! SRAM4_SW_CTRL_EN - SW control sram4 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM4_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM4_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM4_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM5_SW_CTRL_EN_MASK (0x20U)
#define SYSCTL2_MEM_PD_CTRL_SRAM5_SW_CTRL_EN_SHIFT (5U)
/*! SRAM5_SW_CTRL_EN - SW control sram5 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM5_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM5_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM5_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM6_SW_CTRL_EN_MASK (0x40U)
#define SYSCTL2_MEM_PD_CTRL_SRAM6_SW_CTRL_EN_SHIFT (6U)
/*! SRAM6_SW_CTRL_EN - SW control sram6 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM6_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM6_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM6_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM7_SW_CTRL_EN_MASK (0x80U)
#define SYSCTL2_MEM_PD_CTRL_SRAM7_SW_CTRL_EN_SHIFT (7U)
/*! SRAM7_SW_CTRL_EN - SW control sram7 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM7_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM7_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM7_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM8_SW_CTRL_EN_MASK (0x100U)
#define SYSCTL2_MEM_PD_CTRL_SRAM8_SW_CTRL_EN_SHIFT (8U)
/*! SRAM8_SW_CTRL_EN - SW control sram8 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM8_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM8_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM8_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM9_SW_CTRL_EN_MASK (0x200U)
#define SYSCTL2_MEM_PD_CTRL_SRAM9_SW_CTRL_EN_SHIFT (9U)
/*! SRAM9_SW_CTRL_EN - SW control sram9 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM9_SW_CTRL_EN(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM9_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM9_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM10_SW_CTRL_EN_MASK (0x400U)
#define SYSCTL2_MEM_PD_CTRL_SRAM10_SW_CTRL_EN_SHIFT (10U)
/*! SRAM10_SW_CTRL_EN - SW control sram10 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM10_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM10_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM10_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM11_SW_CTRL_EN_MASK (0x800U)
#define SYSCTL2_MEM_PD_CTRL_SRAM11_SW_CTRL_EN_SHIFT (11U)
/*! SRAM11_SW_CTRL_EN - SW control sram11 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM11_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM11_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM11_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM12_SW_CTRL_EN_MASK (0x1000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM12_SW_CTRL_EN_SHIFT (12U)
/*! SRAM12_SW_CTRL_EN - SW control sram12 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM12_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM12_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM12_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM13_SW_CTRL_EN_MASK (0x2000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM13_SW_CTRL_EN_SHIFT (13U)
/*! SRAM13_SW_CTRL_EN - SW control sram13 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM13_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM13_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM13_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM14_SW_CTRL_EN_MASK (0x4000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM14_SW_CTRL_EN_SHIFT (14U)
/*! SRAM14_SW_CTRL_EN - SW control sram14 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM14_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM14_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM14_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM15_SW_CTRL_EN_MASK (0x8000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM15_SW_CTRL_EN_SHIFT (15U)
/*! SRAM15_SW_CTRL_EN - SW control sram15 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM15_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM15_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM15_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM16_SW_CTRL_EN_MASK (0x10000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM16_SW_CTRL_EN_SHIFT (16U)
/*! SRAM16_SW_CTRL_EN - SW control sram16 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM16_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM16_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM16_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM17_SW_CTRL_EN_MASK (0x20000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM17_SW_CTRL_EN_SHIFT (17U)
/*! SRAM17_SW_CTRL_EN - SW control sram17 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM17_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM17_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM17_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SRAM18_SW_CTRL_EN_MASK (0x40000U)
#define SYSCTL2_MEM_PD_CTRL_SRAM18_SW_CTRL_EN_SHIFT (18U)
/*! SRAM18_SW_CTRL_EN - SW control sram18 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SRAM18_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SRAM18_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SRAM18_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_AON0_SW_CTRL_EN_MASK (0x80000U)
#define SYSCTL2_MEM_PD_CTRL_AON0_SW_CTRL_EN_SHIFT (19U)
/*! AON0_SW_CTRL_EN - SW control aon0 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_AON0_SW_CTRL_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_AON0_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_AON0_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_AON1_SW_CTRL_EN_MASK (0x100000U)
#define SYSCTL2_MEM_PD_CTRL_AON1_SW_CTRL_EN_SHIFT (20U)
/*! AON1_SW_CTRL_EN - SW control aon1 mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_AON1_SW_CTRL_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_AON1_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_AON1_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_ELS_SW_CTRL_EN_MASK  (0x200000U)
#define SYSCTL2_MEM_PD_CTRL_ELS_SW_CTRL_EN_SHIFT (21U)
/*! ELS_SW_CTRL_EN - SW control els mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_ELS_SW_CTRL_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_ELS_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_ELS_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_PKC_SW_CTRL_EN_MASK  (0x400000U)
#define SYSCTL2_MEM_PD_CTRL_PKC_SW_CTRL_EN_SHIFT (22U)
/*! PKC_SW_CTRL_EN - SW control pkc mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_PKC_SW_CTRL_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_PKC_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_PKC_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_PQ_SW_CTRL_EN_MASK   (0x800000U)
#define SYSCTL2_MEM_PD_CTRL_PQ_SW_CTRL_EN_SHIFT  (23U)
/*! PQ_SW_CTRL_EN - SW control pq mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_PQ_SW_CTRL_EN(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_PQ_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_PQ_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_FLEXSPI_SW_CTRL_EN_MASK (0x1000000U)
#define SYSCTL2_MEM_PD_CTRL_FLEXSPI_SW_CTRL_EN_SHIFT (24U)
/*! FLEXSPI_SW_CTRL_EN - SW control flexspi mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_FLEXSPI_SW_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_FLEXSPI_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_FLEXSPI_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_ROM_SW_CTRL_EN_MASK  (0x2000000U)
#define SYSCTL2_MEM_PD_CTRL_ROM_SW_CTRL_EN_SHIFT (25U)
/*! ROM_SW_CTRL_EN - SW control rom mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_ROM_SW_CTRL_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_ROM_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_ROM_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_OTP_SW_CTRL_EN_MASK  (0x4000000U)
#define SYSCTL2_MEM_PD_CTRL_OTP_SW_CTRL_EN_SHIFT (26U)
/*! OTP_SW_CTRL_EN - SW control otp mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_OTP_SW_CTRL_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_OTP_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_OTP_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_SDIO_SW_CTRL_EN_MASK (0x8000000U)
#define SYSCTL2_MEM_PD_CTRL_SDIO_SW_CTRL_EN_SHIFT (27U)
/*! SDIO_SW_CTRL_EN - SW control sdio mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_SDIO_SW_CTRL_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_SDIO_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_SDIO_SW_CTRL_EN_MASK)

#define SYSCTL2_MEM_PD_CTRL_ENET_SW_CTRL_EN_MASK (0x10000000U)
#define SYSCTL2_MEM_PD_CTRL_ENET_SW_CTRL_EN_SHIFT (28U)
/*! ENET_SW_CTRL_EN - SW control enet mem_pd signal enable when PM2 mode */
#define SYSCTL2_MEM_PD_CTRL_ENET_SW_CTRL_EN(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CTRL_ENET_SW_CTRL_EN_SHIFT)) & SYSCTL2_MEM_PD_CTRL_ENET_SW_CTRL_EN_MASK)
/*! @} */

/*! @name MEM_PD_CFG - MEM PD Configure register when PM2 mode */
/*! @{ */

#define SYSCTL2_MEM_PD_CFG_SRAM0_SW_CFG_MASK     (0x1U)
#define SYSCTL2_MEM_PD_CFG_SRAM0_SW_CFG_SHIFT    (0U)
/*! SRAM0_SW_CFG - sw cfg sram0 mem_pdwn signal when PM2 mode; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM0_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM0_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM0_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM1_SW_CFG_MASK     (0x2U)
#define SYSCTL2_MEM_PD_CFG_SRAM1_SW_CFG_SHIFT    (1U)
/*! SRAM1_SW_CFG - sw cfg sram1 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM1_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM1_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM1_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM2_SW_CFG_MASK     (0x4U)
#define SYSCTL2_MEM_PD_CFG_SRAM2_SW_CFG_SHIFT    (2U)
/*! SRAM2_SW_CFG - sw cfg sram2 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM2_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM2_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM2_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM3_SW_CFG_MASK     (0x8U)
#define SYSCTL2_MEM_PD_CFG_SRAM3_SW_CFG_SHIFT    (3U)
/*! SRAM3_SW_CFG - sw cfg sram3 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM3_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM3_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM3_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM4_SW_CFG_MASK     (0x10U)
#define SYSCTL2_MEM_PD_CFG_SRAM4_SW_CFG_SHIFT    (4U)
/*! SRAM4_SW_CFG - sw cfg sram4 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM4_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM4_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM4_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM5_SW_CFG_MASK     (0x20U)
#define SYSCTL2_MEM_PD_CFG_SRAM5_SW_CFG_SHIFT    (5U)
/*! SRAM5_SW_CFG - sw cfg sram5 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM5_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM5_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM5_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM6_SW_CFG_MASK     (0x40U)
#define SYSCTL2_MEM_PD_CFG_SRAM6_SW_CFG_SHIFT    (6U)
/*! SRAM6_SW_CFG - sw cfg sram6 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM6_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM6_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM6_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM7_SW_CFG_MASK     (0x80U)
#define SYSCTL2_MEM_PD_CFG_SRAM7_SW_CFG_SHIFT    (7U)
/*! SRAM7_SW_CFG - sw cfg sram7 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM7_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM7_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM7_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM8_SW_CFG_MASK     (0x100U)
#define SYSCTL2_MEM_PD_CFG_SRAM8_SW_CFG_SHIFT    (8U)
/*! SRAM8_SW_CFG - sw cfg sram8 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM8_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM8_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM8_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM9_SW_CFG_MASK     (0x200U)
#define SYSCTL2_MEM_PD_CFG_SRAM9_SW_CFG_SHIFT    (9U)
/*! SRAM9_SW_CFG - sw cfg sram9 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM9_SW_CFG(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM9_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM9_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM10_SW_CFG_MASK    (0x400U)
#define SYSCTL2_MEM_PD_CFG_SRAM10_SW_CFG_SHIFT   (10U)
/*! SRAM10_SW_CFG - sw cfg sram mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM10_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM10_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM10_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM11_SW_CFG_MASK    (0x800U)
#define SYSCTL2_MEM_PD_CFG_SRAM11_SW_CFG_SHIFT   (11U)
/*! SRAM11_SW_CFG - sw cfg sram11 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM11_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM11_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM11_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM12_SW_CFG_MASK    (0x1000U)
#define SYSCTL2_MEM_PD_CFG_SRAM12_SW_CFG_SHIFT   (12U)
/*! SRAM12_SW_CFG - sw cfg sram12 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM12_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM12_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM12_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM13_SW_CFG_MASK    (0x2000U)
#define SYSCTL2_MEM_PD_CFG_SRAM13_SW_CFG_SHIFT   (13U)
/*! SRAM13_SW_CFG - sw cfg sram13 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM13_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM13_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM13_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM14_SW_CFG_MASK    (0x4000U)
#define SYSCTL2_MEM_PD_CFG_SRAM14_SW_CFG_SHIFT   (14U)
/*! SRAM14_SW_CFG - sw cfg sram14 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM14_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM14_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM14_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM15_SW_CFG_MASK    (0x8000U)
#define SYSCTL2_MEM_PD_CFG_SRAM15_SW_CFG_SHIFT   (15U)
/*! SRAM15_SW_CFG - sw cfg sram15 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM15_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM15_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM15_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM16_SW_CFG_MASK    (0x10000U)
#define SYSCTL2_MEM_PD_CFG_SRAM16_SW_CFG_SHIFT   (16U)
/*! SRAM16_SW_CFG - sw cfg sram16 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM16_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM16_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM16_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM17_SW_CFG_MASK    (0x20000U)
#define SYSCTL2_MEM_PD_CFG_SRAM17_SW_CFG_SHIFT   (17U)
/*! SRAM17_SW_CFG - sw cfg sram17 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM17_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM17_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM17_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SRAM18_SW_CFG_MASK    (0x40000U)
#define SYSCTL2_MEM_PD_CFG_SRAM18_SW_CFG_SHIFT   (18U)
/*! SRAM18_SW_CFG - sw cfg sram18 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SRAM18_SW_CFG(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SRAM18_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SRAM18_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_AON0_SW_CFG_MASK      (0x80000U)
#define SYSCTL2_MEM_PD_CFG_AON0_SW_CFG_SHIFT     (19U)
/*! AON0_SW_CFG - sw cfg aon0 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_AON0_SW_CFG(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_AON0_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_AON0_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_AON1_SW_CFG_MASK      (0x100000U)
#define SYSCTL2_MEM_PD_CFG_AON1_SW_CFG_SHIFT     (20U)
/*! AON1_SW_CFG - sw cfg aon1 mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_AON1_SW_CFG(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_AON1_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_AON1_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_ELS_SW_CFG_MASK       (0x200000U)
#define SYSCTL2_MEM_PD_CFG_ELS_SW_CFG_SHIFT      (21U)
/*! ELS_SW_CFG - sw cfg els mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_ELS_SW_CFG(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_ELS_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_ELS_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_PKC_SW_CFG_MASK       (0x400000U)
#define SYSCTL2_MEM_PD_CFG_PKC_SW_CFG_SHIFT      (22U)
/*! PKC_SW_CFG - sw cfg pkc mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_PKC_SW_CFG(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_PKC_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_PKC_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_PQ_SW_CFG_MASK        (0x800000U)
#define SYSCTL2_MEM_PD_CFG_PQ_SW_CFG_SHIFT       (23U)
/*! PQ_SW_CFG - sw cfg pq mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_PQ_SW_CFG(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_PQ_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_PQ_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_FLEXSPI_SW_CFG_MASK   (0x1000000U)
#define SYSCTL2_MEM_PD_CFG_FLEXSPI_SW_CFG_SHIFT  (24U)
/*! FLEXSPI_SW_CFG - sw cfg flexspi mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_FLEXSPI_SW_CFG(x)     (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_FLEXSPI_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_FLEXSPI_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_ROM_SW_CFG_MASK       (0x2000000U)
#define SYSCTL2_MEM_PD_CFG_ROM_SW_CFG_SHIFT      (25U)
/*! ROM_SW_CFG - sw cfg rom mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_ROM_SW_CFG(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_ROM_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_ROM_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_OTP_SW_CFG_MASK       (0x4000000U)
#define SYSCTL2_MEM_PD_CFG_OTP_SW_CFG_SHIFT      (26U)
/*! OTP_SW_CFG - sw cfg otp mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_OTP_SW_CFG(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_OTP_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_OTP_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_SDIO_SW_CFG_MASK      (0x8000000U)
#define SYSCTL2_MEM_PD_CFG_SDIO_SW_CFG_SHIFT     (27U)
/*! SDIO_SW_CFG - sw cfg sdio mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_SDIO_SW_CFG(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_SDIO_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_SDIO_SW_CFG_MASK)

#define SYSCTL2_MEM_PD_CFG_ENET_SW_CFG_MASK      (0x10000000U)
#define SYSCTL2_MEM_PD_CFG_ENET_SW_CFG_SHIFT     (28U)
/*! ENET_SW_CFG - sw cfg enet mem_pdwn signal when PM2 mode ; 0: active; 1: power down */
#define SYSCTL2_MEM_PD_CFG_ENET_SW_CFG(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_PD_CFG_ENET_SW_CFG_SHIFT)) & SYSCTL2_MEM_PD_CFG_ENET_SW_CFG_MASK)
/*! @} */

/*! @name ENET_IN_SEL_TIMER - Select input source for enet pad0 */
/*! @{ */

#define SYSCTL2_ENET_IN_SEL_TIMER_ENET_IN_SEL_TIMER_MASK (0x1U)
#define SYSCTL2_ENET_IN_SEL_TIMER_ENET_IN_SEL_TIMER_SHIFT (0U)
/*! ENET_IN_SEL_TIMER - select input source as enet input */
#define SYSCTL2_ENET_IN_SEL_TIMER_ENET_IN_SEL_TIMER(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ENET_IN_SEL_TIMER_ENET_IN_SEL_TIMER_SHIFT)) & SYSCTL2_ENET_IN_SEL_TIMER_ENET_IN_SEL_TIMER_MASK)
/*! @} */

/*! @name ENET_IPG_STOP - Configure ipg_stop, used by enet wakeup sequence */
/*! @{ */

#define SYSCTL2_ENET_IPG_STOP_ENET_IPG_STOP_MASK (0x1U)
#define SYSCTL2_ENET_IPG_STOP_ENET_IPG_STOP_SHIFT (0U)
/*! ENET_IPG_STOP - Configure ipg_stop, used by enet wakeup sequence; 0: Drive enet input IPG_STOP low; 1: Drive enet input IPG_STOP high. */
#define SYSCTL2_ENET_IPG_STOP_ENET_IPG_STOP(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ENET_IPG_STOP_ENET_IPG_STOP_SHIFT)) & SYSCTL2_ENET_IPG_STOP_ENET_IPG_STOP_MASK)
/*! @} */

/*! @name ENET_IPG_STOP_ACK - Store ipg_stop_ack, used by enet wakeup sequence */
/*! @{ */

#define SYSCTL2_ENET_IPG_STOP_ACK_ENET_IPG_STOP_ACK_MASK (0x1U)
#define SYSCTL2_ENET_IPG_STOP_ACK_ENET_IPG_STOP_ACK_SHIFT (0U)
/*! ENET_IPG_STOP_ACK - Store ipg_stop_ack, used by enet wakeup sequence */
#define SYSCTL2_ENET_IPG_STOP_ACK_ENET_IPG_STOP_ACK(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ENET_IPG_STOP_ACK_ENET_IPG_STOP_ACK_SHIFT)) & SYSCTL2_ENET_IPG_STOP_ACK_ENET_IPG_STOP_ACK_MASK)
/*! @} */

/*! @name ROM_BRU_ADDR_MASK_DIS - Disable dynamic address masking feature */
/*! @{ */

#define SYSCTL2_ROM_BRU_ADDR_MASK_DIS_ROM_BRU_ADDR_MASK_DIS_MASK (0x1U)
#define SYSCTL2_ROM_BRU_ADDR_MASK_DIS_ROM_BRU_ADDR_MASK_DIS_SHIFT (0U)
/*! ROM_BRU_ADDR_MASK_DIS - Disable dynamic address masking feature */
#define SYSCTL2_ROM_BRU_ADDR_MASK_DIS_ROM_BRU_ADDR_MASK_DIS(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_BRU_ADDR_MASK_DIS_ROM_BRU_ADDR_MASK_DIS_SHIFT)) & SYSCTL2_ROM_BRU_ADDR_MASK_DIS_ROM_BRU_ADDR_MASK_DIS_MASK)
/*! @} */

/*! @name ROM_BRU_DYN_CLK_DIS - Disable dynamic clock gating feature */
/*! @{ */

#define SYSCTL2_ROM_BRU_DYN_CLK_DIS_ROM_BRU_DYN_CLK_DIS_MASK (0x1U)
#define SYSCTL2_ROM_BRU_DYN_CLK_DIS_ROM_BRU_DYN_CLK_DIS_SHIFT (0U)
/*! ROM_BRU_DYN_CLK_DIS - Disable dynamic clock gating feature */
#define SYSCTL2_ROM_BRU_DYN_CLK_DIS_ROM_BRU_DYN_CLK_DIS(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_BRU_DYN_CLK_DIS_ROM_BRU_DYN_CLK_DIS_SHIFT)) & SYSCTL2_ROM_BRU_DYN_CLK_DIS_ROM_BRU_DYN_CLK_DIS_MASK)
/*! @} */

/*! @name OTP_EARLY_FUSE_VALID - Early fuse valid from OTP */
/*! @{ */

#define SYSCTL2_OTP_EARLY_FUSE_VALID_OTP_EARLY_FUSE_VALID_MASK (0x1U)
#define SYSCTL2_OTP_EARLY_FUSE_VALID_OTP_EARLY_FUSE_VALID_SHIFT (0U)
/*! OTP_EARLY_FUSE_VALID - Early fuse valid from OTP */
#define SYSCTL2_OTP_EARLY_FUSE_VALID_OTP_EARLY_FUSE_VALID(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_OTP_EARLY_FUSE_VALID_OTP_EARLY_FUSE_VALID_SHIFT)) & SYSCTL2_OTP_EARLY_FUSE_VALID_OTP_EARLY_FUSE_VALID_MASK)
/*! @} */

/*! @name OTP_MEDIUM_FUSE_VALID - Medium fuse valid from OTP */
/*! @{ */

#define SYSCTL2_OTP_MEDIUM_FUSE_VALID_OTP_MEDIUM_FUSE_VALID_MASK (0x1U)
#define SYSCTL2_OTP_MEDIUM_FUSE_VALID_OTP_MEDIUM_FUSE_VALID_SHIFT (0U)
/*! OTP_MEDIUM_FUSE_VALID - Medium fuse valid from OTP */
#define SYSCTL2_OTP_MEDIUM_FUSE_VALID_OTP_MEDIUM_FUSE_VALID(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_OTP_MEDIUM_FUSE_VALID_OTP_MEDIUM_FUSE_VALID_SHIFT)) & SYSCTL2_OTP_MEDIUM_FUSE_VALID_OTP_MEDIUM_FUSE_VALID_MASK)
/*! @} */

/*! @name OTP_ALL_FUSE_VALID - All fuse valid from OTP */
/*! @{ */

#define SYSCTL2_OTP_ALL_FUSE_VALID_OTP_ALL_FUSE_VALID_MASK (0x1U)
#define SYSCTL2_OTP_ALL_FUSE_VALID_OTP_ALL_FUSE_VALID_SHIFT (0U)
/*! OTP_ALL_FUSE_VALID - All fuse valid from OTP */
#define SYSCTL2_OTP_ALL_FUSE_VALID_OTP_ALL_FUSE_VALID(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_OTP_ALL_FUSE_VALID_OTP_ALL_FUSE_VALID_SHIFT)) & SYSCTL2_OTP_ALL_FUSE_VALID_OTP_ALL_FUSE_VALID_MASK)
/*! @} */

/*! @name PLL_CTRL - PLL control register */
/*! @{ */

#define SYSCTL2_PLL_CTRL_T3_PDB_MASK             (0x1U)
#define SYSCTL2_PLL_CTRL_T3_PDB_SHIFT            (0U)
/*! T3_PDB - T3 PLL enable signal from SOC */
#define SYSCTL2_PLL_CTRL_T3_PDB(x)               (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_T3_PDB_SHIFT)) & SYSCTL2_PLL_CTRL_T3_PDB_MASK)

#define SYSCTL2_PLL_CTRL_T3_LOCK_MASK            (0x2U)
#define SYSCTL2_PLL_CTRL_T3_LOCK_SHIFT           (1U)
/*! T3_LOCK - T3 output clock lock signal */
#define SYSCTL2_PLL_CTRL_T3_LOCK(x)              (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_T3_LOCK_SHIFT)) & SYSCTL2_PLL_CTRL_T3_LOCK_MASK)

#define SYSCTL2_PLL_CTRL_TDDR_PDB_MASK           (0x8U)
#define SYSCTL2_PLL_CTRL_TDDR_PDB_SHIFT          (3U)
/*! TDDR_PDB - TDDR PLL enable signal from SOC */
#define SYSCTL2_PLL_CTRL_TDDR_PDB(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TDDR_PDB_SHIFT)) & SYSCTL2_PLL_CTRL_TDDR_PDB_MASK)

#define SYSCTL2_PLL_CTRL_TDDR_LOCK_MASK          (0x10U)
#define SYSCTL2_PLL_CTRL_TDDR_LOCK_SHIFT         (4U)
/*! TDDR_LOCK - TDDR output clock lock signal */
#define SYSCTL2_PLL_CTRL_TDDR_LOCK(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TDDR_LOCK_SHIFT)) & SYSCTL2_PLL_CTRL_TDDR_LOCK_MASK)

#define SYSCTL2_PLL_CTRL_TCPU_PDB_MASK           (0x20U)
#define SYSCTL2_PLL_CTRL_TCPU_PDB_SHIFT          (5U)
/*! TCPU_PDB - TCPU PLL enable signal from SOC */
#define SYSCTL2_PLL_CTRL_TCPU_PDB(x)             (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TCPU_PDB_SHIFT)) & SYSCTL2_PLL_CTRL_TCPU_PDB_MASK)

#define SYSCTL2_PLL_CTRL_TCPU_LOCK_MASK          (0x40U)
#define SYSCTL2_PLL_CTRL_TCPU_LOCK_SHIFT         (6U)
/*! TCPU_LOCK - TCPU output clock lock signal */
#define SYSCTL2_PLL_CTRL_TCPU_LOCK(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TCPU_LOCK_SHIFT)) & SYSCTL2_PLL_CTRL_TCPU_LOCK_MASK)

#define SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_MASK (0x180U)
#define SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_SHIFT (7U)
/*! TCPU_FLEXSPI_CLK_SEL - TCPU_PLL DIV selection(00:3120/12; 01:3120/11; 10: 3120/10; 11:3120/9) */
#define SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_SHIFT)) & SYSCTL2_PLL_CTRL_TCPU_FLEXSPI_CLK_SEL_MASK)

#define SYSCTL2_PLL_CTRL_TCPU_FBDIV_MASK         (0x1FE00U)
#define SYSCTL2_PLL_CTRL_TCPU_FBDIV_SHIFT        (9U)
/*! TCPU_FBDIV - TCPU_PLL Feedback Divider Value ( Fxtal 40MHZ: 75 to 96 (decimal); 38.4MHz 78 to 100 (decimal)) */
#define SYSCTL2_PLL_CTRL_TCPU_FBDIV(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TCPU_FBDIV_SHIFT)) & SYSCTL2_PLL_CTRL_TCPU_FBDIV_MASK)

#define SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_MASK (0x60000U)
#define SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_SHIFT (17U)
/*! TDDR_FLEXSPI_CLK_SEL - TDDR_PLL Clock selection from SOC for DDR CLOCK (00:3.2GHZ/11; 01: 3.2GHZ/10; 10: 3.2GHz/9;11: 3.2GHz/8) */
#define SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_SHIFT)) & SYSCTL2_PLL_CTRL_TDDR_FLEXSPI_CLK_SEL_MASK)

#define SYSCTL2_PLL_CTRL_T3_ITRC_EN_MASK         (0x80000U)
#define SYSCTL2_PLL_CTRL_T3_ITRC_EN_SHIFT        (19U)
/*! T3_ITRC_EN - 0-disable the unlock event to ITRC; 1-enable the unlock event to ITRC */
#define SYSCTL2_PLL_CTRL_T3_ITRC_EN(x)           (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_T3_ITRC_EN_SHIFT)) & SYSCTL2_PLL_CTRL_T3_ITRC_EN_MASK)

#define SYSCTL2_PLL_CTRL_TCPU_ITRC_EN_MASK       (0x100000U)
#define SYSCTL2_PLL_CTRL_TCPU_ITRC_EN_SHIFT      (20U)
/*! TCPU_ITRC_EN - 0-disable the unlock event to ITRC; 1-enable the unlock event to ITRC */
#define SYSCTL2_PLL_CTRL_TCPU_ITRC_EN(x)         (((uint32_t)(((uint32_t)(x)) << SYSCTL2_PLL_CTRL_TCPU_ITRC_EN_SHIFT)) & SYSCTL2_PLL_CTRL_TCPU_ITRC_EN_MASK)
/*! @} */

/*! @name ANA_PDWN_PM2 - ana_pdwn control signal when PM2 mode */
/*! @{ */

#define SYSCTL2_ANA_PDWN_PM2_AVPLL_ANA_PDWN_PM2_MASK (0x1U)
#define SYSCTL2_ANA_PDWN_PM2_AVPLL_ANA_PDWN_PM2_SHIFT (0U)
/*! AVPLL_ANA_PDWN_PM2 - AVPLL ana_pdwn control signal when PM2 mode, 1: AVPLL is low power mode when PM2 0: AVPLL is normal mode when PM2 */
#define SYSCTL2_ANA_PDWN_PM2_AVPLL_ANA_PDWN_PM2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_AVPLL_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_AVPLL_ANA_PDWN_PM2_MASK)

#define SYSCTL2_ANA_PDWN_PM2_USB_ANA_PDWN_PM2_MASK (0x2U)
#define SYSCTL2_ANA_PDWN_PM2_USB_ANA_PDWN_PM2_SHIFT (1U)
/*! USB_ANA_PDWN_PM2 - USB ana_pdwn control signal when PM2 mode, 1: USB is low power mode when PM2 0: USB is normal mode when PM2 */
#define SYSCTL2_ANA_PDWN_PM2_USB_ANA_PDWN_PM2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_USB_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_USB_ANA_PDWN_PM2_MASK)

#define SYSCTL2_ANA_PDWN_PM2_GAU_ANA_PDWN_PM2_MASK (0x4U)
#define SYSCTL2_ANA_PDWN_PM2_GAU_ANA_PDWN_PM2_SHIFT (2U)
/*! GAU_ANA_PDWN_PM2 - GAU ana_pdwn control signal when PM2 mode, 1: GAU is low power mode when PM2 0: GAU is normal mode when PM2 */
#define SYSCTL2_ANA_PDWN_PM2_GAU_ANA_PDWN_PM2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_GAU_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_GAU_ANA_PDWN_PM2_MASK)

#define SYSCTL2_ANA_PDWN_PM2_ANA_TOP_ANA_PDWN_PM2_MASK (0x8U)
#define SYSCTL2_ANA_PDWN_PM2_ANA_TOP_ANA_PDWN_PM2_SHIFT (3U)
/*! ANA_TOP_ANA_PDWN_PM2 - ANA_TOP ana_pdwn control signal when PM2 mode, 1: ANA_TOP is low power mode when PM2 0: ANA_TOP is normal mode when PM2 */
#define SYSCTL2_ANA_PDWN_PM2_ANA_TOP_ANA_PDWN_PM2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_ANA_TOP_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_ANA_TOP_ANA_PDWN_PM2_MASK)

#define SYSCTL2_ANA_PDWN_PM2_TDDR_TOP_ANA_PDWN_PM2_MASK (0x10U)
#define SYSCTL2_ANA_PDWN_PM2_TDDR_TOP_ANA_PDWN_PM2_SHIFT (4U)
/*! TDDR_TOP_ANA_PDWN_PM2 - TDDR_TOP ana_pdwn control signal when PM2 mode, 1: TDDR_TOP is low power
 *    mode when PM2 0: TDDR_TOP is normal mode when PM2
 */
#define SYSCTL2_ANA_PDWN_PM2_TDDR_TOP_ANA_PDWN_PM2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_TDDR_TOP_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_TDDR_TOP_ANA_PDWN_PM2_MASK)

#define SYSCTL2_ANA_PDWN_PM2_TCPU_TOP_ANA_PDWN_PM2_MASK (0x20U)
#define SYSCTL2_ANA_PDWN_PM2_TCPU_TOP_ANA_PDWN_PM2_SHIFT (5U)
/*! TCPU_TOP_ANA_PDWN_PM2 - TCPU_TOP ana_pdwn control signal when PM2 mode, 1: TCPU_TOP is low power
 *    mode when PM2 0: TCPU_TOP is normal mode when PM2
 */
#define SYSCTL2_ANA_PDWN_PM2_TCPU_TOP_ANA_PDWN_PM2(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_TCPU_TOP_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_TCPU_TOP_ANA_PDWN_PM2_MASK)

#define SYSCTL2_ANA_PDWN_PM2_T3_ANA_PDWN_PM2_MASK (0x40U)
#define SYSCTL2_ANA_PDWN_PM2_T3_ANA_PDWN_PM2_SHIFT (6U)
/*! T3_ANA_PDWN_PM2 - T3 ana_pdwn control signal when PM2 mode, 1: T3 is low power mode when PM2 0: T3 is normal mode when PM2 */
#define SYSCTL2_ANA_PDWN_PM2_T3_ANA_PDWN_PM2(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ANA_PDWN_PM2_T3_ANA_PDWN_PM2_SHIFT)) & SYSCTL2_ANA_PDWN_PM2_T3_ANA_PDWN_PM2_MASK)
/*! @} */

/*! @name SOURCE_CLK_GATE - source clock gate control */
/*! @{ */

#define SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_CLK_CG_MASK (0x1U)
#define SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_CLK_CG_SHIFT (0U)
/*! TCPU_MCI_CLK_CG - gate tcpu_mci_clk */
#define SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_CLK_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_CLK_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_CLK_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_FLEXSPI_CLK_CG_MASK (0x2U)
#define SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_FLEXSPI_CLK_CG_SHIFT (1U)
/*! TCPU_MCI_FLEXSPI_CLK_CG - gate tcpu_mci_flexspi_clk */
#define SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_FLEXSPI_CLK_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_FLEXSPI_CLK_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_TCPU_MCI_FLEXSPI_CLK_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG_MASK (0x4U)
#define SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG_SHIFT (2U)
/*! TDDR_MCI_ENET_CLK_CG - gate tddr_mci_enet_clk */
#define SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_ENET_CLK_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_FLEXSPI_CLK_CG_MASK (0x8U)
#define SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_FLEXSPI_CLK_CG_SHIFT (3U)
/*! TDDR_MCI_FLEXSPI_CLK_CG - gate tddr_mci_flexspi_clk */
#define SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_FLEXSPI_CLK_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_FLEXSPI_CLK_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_TDDR_MCI_FLEXSPI_CLK_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_48_60M_IRC_CG_MASK (0x10U)
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_48_60M_IRC_CG_SHIFT (4U)
/*! T3PLL_MCI_48_60M_IRC_CG - gate t3pll_mci_48_60m_irc */
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_48_60M_IRC_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_48_60M_IRC_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_48_60M_IRC_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG_MASK (0x20U)
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG_SHIFT (5U)
/*! T3PLL_MCI_256M_CG - gate t3pll_mci_256m */
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_256M_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_213P3M_CG_MASK (0x40U)
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_213P3M_CG_SHIFT (6U)
/*! T3PLL_MCI_213P3M_CG - gate t3pll_mci_213p3m */
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_213P3M_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_213P3M_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_213P3M_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_FLEXSPI_CLK_CG_MASK (0x80U)
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_FLEXSPI_CLK_CG_SHIFT (7U)
/*! T3PLL_MCI_FLEXSPI_CLK_CG - gate t3pll_mci_flexspi_clk */
#define SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_FLEXSPI_CLK_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_FLEXSPI_CLK_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_T3PLL_MCI_FLEXSPI_CLK_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_SYS_CG_MASK (0x200U)
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_SYS_CG_SHIFT (9U)
/*! REFCLK_SYS_CG - gate refclk_sys */
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_SYS_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_REFCLK_SYS_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_REFCLK_SYS_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_CPU_CLK_EN_MASK  (0x10000U)
#define SYSCTL2_SOURCE_CLK_GATE_CPU_CLK_EN_SHIFT (16U)
/*! CPU_CLK_EN - enable cpu clk(c0_fr_hclk) bypass PMU gate_cpu_clk control */
#define SYSCTL2_SOURCE_CLK_GATE_CPU_CLK_EN(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_CPU_CLK_EN_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_CPU_CLK_EN_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_TCPU_CG_MASK (0x10000000U)
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_TCPU_CG_SHIFT (28U)
/*! REFCLK_TCPU_CG - gate CAU_PHY_RECLK_CPR_TCPU request when PM0; 1:request;0:no request */
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_TCPU_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_REFCLK_TCPU_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_REFCLK_TCPU_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_TDDR_CG_MASK (0x20000000U)
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_TDDR_CG_SHIFT (29U)
/*! REFCLK_TDDR_CG - gate CAU_PHY_RECLK_CPR_TDDR request when PM0; MCI request CAU gate reference
 *    clock for PHY in PM2 mode (when USB/AUD/TCPU/TDDR PLL are all power down); 1:request;0:no request
 */
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_TDDR_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_REFCLK_TDDR_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_REFCLK_TDDR_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_AUD_CG_MASK (0x40000000U)
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_AUD_CG_SHIFT (30U)
/*! REFCLK_AUD_CG - gate CAU_PHY_RECLK_CPR_AUD request when PM0; 1:request;0:no request */
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_AUD_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_REFCLK_AUD_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_REFCLK_AUD_CG_MASK)

#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_USB_CG_MASK (0x80000000U)
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_USB_CG_SHIFT (31U)
/*! REFCLK_USB_CG - gate CAU_PHY_RECLK_CPR_USB request when PM0;1:request;0:no request */
#define SYSCTL2_SOURCE_CLK_GATE_REFCLK_USB_CG(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOURCE_CLK_GATE_REFCLK_USB_CG_SHIFT)) & SYSCTL2_SOURCE_CLK_GATE_REFCLK_USB_CG_MASK)
/*! @} */

/*! @name TRNG_PWR_MODE - TRNG_PWR_MODE */
/*! @{ */

#define SYSCTL2_TRNG_PWR_MODE_STOP_REQ_MASK      (0x1U)
#define SYSCTL2_TRNG_PWR_MODE_STOP_REQ_SHIFT     (0U)
/*! STOP_REQ - The bit is used as the SoC low-power request. It is generated to tell TRNG that the
 *    clock to TRNG is going to stop running. This signal will be asserted no less than two clocks
 *    before TRNG clock is stopped, to allow TRNG to prepare for stop mode.0: No request; 1:Request
 */
#define SYSCTL2_TRNG_PWR_MODE_STOP_REQ(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_TRNG_PWR_MODE_STOP_REQ_SHIFT)) & SYSCTL2_TRNG_PWR_MODE_STOP_REQ_MASK)

#define SYSCTL2_TRNG_PWR_MODE_STOP_ACK_MASK      (0x2U)
#define SYSCTL2_TRNG_PWR_MODE_STOP_ACK_SHIFT     (1U)
/*! STOP_ACK - The bit is used as the SoC low-power acknowledge. TRNG will generate this single
 *    clock cycle pulse one clock cycle after STOP_REQ has been asserted.0:No Acknowledge; 1:Acknowledge
 */
#define SYSCTL2_TRNG_PWR_MODE_STOP_ACK(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_TRNG_PWR_MODE_STOP_ACK_SHIFT)) & SYSCTL2_TRNG_PWR_MODE_STOP_ACK_MASK)
/*! @} */

/*! @name TRNG_PIN_CTRL - TRNG_PIN_CTRL */
/*! @{ */

#define SYSCTL2_TRNG_PIN_CTRL_ENABLE_MASK        (0x1U)
#define SYSCTL2_TRNG_PIN_CTRL_ENABLE_SHIFT       (0U)
/*! ENABLE - The bit is used to control the interface of TRNG. 0:Disable; 1: Enable */
#define SYSCTL2_TRNG_PIN_CTRL_ENABLE(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_TRNG_PIN_CTRL_ENABLE_SHIFT)) & SYSCTL2_TRNG_PIN_CTRL_ENABLE_MASK)

#define SYSCTL2_TRNG_PIN_CTRL_BUSY_MASK          (0x2U)
#define SYSCTL2_TRNG_PIN_CTRL_BUSY_SHIFT         (1U)
/*! BUSY - The bit indicate that the TRNG is busy. This happens when the TRNG is busy sampling the
 *    entropy bits and has not issued an entropy valid signal.0: Free; 1: Busy; this bit should be
 *    zero after ENABLE(bit0 in this register) is set
 */
#define SYSCTL2_TRNG_PIN_CTRL_BUSY(x)            (((uint32_t)(((uint32_t)(x)) << SYSCTL2_TRNG_PIN_CTRL_BUSY_SHIFT)) & SYSCTL2_TRNG_PIN_CTRL_BUSY_MASK)

#define SYSCTL2_TRNG_PIN_CTRL_HW_ERROR_MASK      (0x4U)
#define SYSCTL2_TRNG_PIN_CTRL_HW_ERROR_SHIFT     (2U)
/*! HW_ERROR - The bit indicate that the TRNG has generated a hardware error. This could be due to a
 *    Frequency Count Fail, a run time or power on reset parameter misconfiguration, and /or a
 *    failure of one or more of the built in internal entropy quality tests. The cause of the error can
 *    be decoded by checking the bits of the STATUS register of TRNG.0:No Error; 1: Error
 */
#define SYSCTL2_TRNG_PIN_CTRL_HW_ERROR(x)        (((uint32_t)(((uint32_t)(x)) << SYSCTL2_TRNG_PIN_CTRL_HW_ERROR_SHIFT)) & SYSCTL2_TRNG_PIN_CTRL_HW_ERROR_MASK)
/*! @} */

/*! @name CAU_CTRL - CAU control register */
/*! @{ */

#define SYSCTL2_CAU_CTRL_SOC_REFCLK_RDY_MASK     (0x1U)
#define SYSCTL2_CAU_CTRL_SOC_REFCLK_RDY_SHIFT    (0U)
/*! SOC_REFCLK_RDY - REFCLK ready signal, if 1, can turn on PLL */
#define SYSCTL2_CAU_CTRL_SOC_REFCLK_RDY(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CAU_CTRL_SOC_REFCLK_RDY_SHIFT)) & SYSCTL2_CAU_CTRL_SOC_REFCLK_RDY_MASK)

#define SYSCTL2_CAU_CTRL_MCI_XOSC_EN_MASK        (0x80000000U)
#define SYSCTL2_CAU_CTRL_MCI_XOSC_EN_SHIFT       (31U)
/*! MCI_XOSC_EN - mci request control different modes for CAU XTAL (1 for normal mode, 0 for sleep/full PD) */
#define SYSCTL2_CAU_CTRL_MCI_XOSC_EN(x)          (((uint32_t)(((uint32_t)(x)) << SYSCTL2_CAU_CTRL_MCI_XOSC_EN_SHIFT)) & SYSCTL2_CAU_CTRL_MCI_XOSC_EN_MASK)
/*! @} */

/*! @name SOC_CIU_RDY_MASK - SOC_CIU_RDY_MASK */
/*! @{ */

#define SYSCTL2_SOC_CIU_RDY_MASK_SOC_CIU_RDY_MASK_MASK (0x1U)
#define SYSCTL2_SOC_CIU_RDY_MASK_SOC_CIU_RDY_MASK_SHIFT (0U)
/*! SOC_CIU_RDY_MASK - 1'b1:mask SOC_CIU_RDY, think its value is 1; 1'b0:unmask SOC_CIU_RDY, see its real value */
#define SYSCTL2_SOC_CIU_RDY_MASK_SOC_CIU_RDY_MASK(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOC_CIU_RDY_MASK_SOC_CIU_RDY_MASK_SHIFT)) & SYSCTL2_SOC_CIU_RDY_MASK_SOC_CIU_RDY_MASK_MASK)
/*! @} */

/*! @name LE_AUDIO_TIMER_ENABLE - Enable bit for le audio timer */
/*! @{ */

#define SYSCTL2_LE_AUDIO_TIMER_ENABLE_ENABLE_MASK (0x1U)
#define SYSCTL2_LE_AUDIO_TIMER_ENABLE_ENABLE_SHIFT (0U)
/*! ENABLE - 1'b1:Timer is enabled; 1'b0:Timer is disabled */
#define SYSCTL2_LE_AUDIO_TIMER_ENABLE_ENABLE(x)  (((uint32_t)(((uint32_t)(x)) << SYSCTL2_LE_AUDIO_TIMER_ENABLE_ENABLE_SHIFT)) & SYSCTL2_LE_AUDIO_TIMER_ENABLE_ENABLE_MASK)
/*! @} */

/*! @name LE_AUDIO_TIMER_CNT_CLR - Clear bit of internal counter */
/*! @{ */

#define SYSCTL2_LE_AUDIO_TIMER_CNT_CLR_CLR_MASK  (0x1U)
#define SYSCTL2_LE_AUDIO_TIMER_CNT_CLR_CLR_SHIFT (0U)
/*! CLR - When timer is enabled, write 1 to clear internal counter, write 0 has no effect */
#define SYSCTL2_LE_AUDIO_TIMER_CNT_CLR_CLR(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_LE_AUDIO_TIMER_CNT_CLR_CLR_SHIFT)) & SYSCTL2_LE_AUDIO_TIMER_CNT_CLR_CLR_MASK)
/*! @} */

/*! @name LE_AUDIO_TIMER_CNT0 - Counter value captured by trigger0 */
/*! @{ */

#define SYSCTL2_LE_AUDIO_TIMER_CNT0_CNT0_MASK    (0xFFFFFFFFU)
#define SYSCTL2_LE_AUDIO_TIMER_CNT0_CNT0_SHIFT   (0U)
/*! CNT0 - Counter value captured by trigger0 */
#define SYSCTL2_LE_AUDIO_TIMER_CNT0_CNT0(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_LE_AUDIO_TIMER_CNT0_CNT0_SHIFT)) & SYSCTL2_LE_AUDIO_TIMER_CNT0_CNT0_MASK)
/*! @} */

/*! @name LE_AUDIO_TIMER_CNT1 - Counter value captured by trigger1 */
/*! @{ */

#define SYSCTL2_LE_AUDIO_TIMER_CNT1_CNT1_MASK    (0xFFFFFFFFU)
#define SYSCTL2_LE_AUDIO_TIMER_CNT1_CNT1_SHIFT   (0U)
/*! CNT1 - Counter value captured by trigger1 */
#define SYSCTL2_LE_AUDIO_TIMER_CNT1_CNT1(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_LE_AUDIO_TIMER_CNT1_CNT1_SHIFT)) & SYSCTL2_LE_AUDIO_TIMER_CNT1_CNT1_MASK)
/*! @} */

/*! @name LE_AUDIO_TIMER_CNT2 - Counter value captured by trigger2 */
/*! @{ */

#define SYSCTL2_LE_AUDIO_TIMER_CNT2_CNT2_MASK    (0xFFFFFFFFU)
#define SYSCTL2_LE_AUDIO_TIMER_CNT2_CNT2_SHIFT   (0U)
/*! CNT2 - Counter value captured by trigger2 */
#define SYSCTL2_LE_AUDIO_TIMER_CNT2_CNT2(x)      (((uint32_t)(((uint32_t)(x)) << SYSCTL2_LE_AUDIO_TIMER_CNT2_CNT2_SHIFT)) & SYSCTL2_LE_AUDIO_TIMER_CNT2_CNT2_MASK)
/*! @} */

/*! @name AVPLL_CTRL13 - Audio PLL Control register13 */
/*! @{ */

#define SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2_MASK (0x1U)
#define SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2_SHIFT (0U)
/*! AVPLL_RESET_C2 - C2 SW Reset . Active high */
#define SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL13_AVPLL_RESET_C2_MASK)

#define SYSCTL2_AVPLL_CTRL13_RESERVE_IN_C2_MASK  (0x600U)
#define SYSCTL2_AVPLL_CTRL13_RESERVE_IN_C2_SHIFT (9U)
/*! RESERVE_IN_C2 - Reserved pins */
#define SYSCTL2_AVPLL_CTRL13_RESERVE_IN_C2(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL13_RESERVE_IN_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL13_RESERVE_IN_C2_MASK)

#define SYSCTL2_AVPLL_CTRL13_P_SYNC2_C2_MASK     (0x7FFFF800U)
#define SYSCTL2_AVPLL_CTRL13_P_SYNC2_C2_SHIFT    (11U)
/*! P_SYNC2_C2 - Set DPLL¡¯s Feedback Divider */
#define SYSCTL2_AVPLL_CTRL13_P_SYNC2_C2(x)       (((uint32_t)(((uint32_t)(x)) << SYSCTL2_AVPLL_CTRL13_P_SYNC2_C2_SHIFT)) & SYSCTL2_AVPLL_CTRL13_P_SYNC2_C2_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_STATUS - CPU Code Bus Access Sram Checker Violation Status */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_STATUS_STATUS_MASK (0x7FFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_STATUS_STATUS_SHIFT (0U)
/*! STATUS - for each of these 19 bits: 0- No violation; 1-violation happened */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_STATUS_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_STATUS_STATUS_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_STATUS_STATUS_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_CLR - CPU Code Bus Access Sram Checker Violation Status Clear */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_CLR_STATUS_CLR_MASK (0x7FFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_CLR_STATUS_CLR_SHIFT (0U)
/*! STATUS_CLR - Write '1' to clear Violation Status bit; These bits are self-clear */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_CLR_STATUS_CLR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_CLR_STATUS_CLR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_CLR_STATUS_CLR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_STATUS - CPU SYS Bus Access Sram Checker Violation Status */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_STATUS_STATUS_MASK (0x7FFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_STATUS_STATUS_SHIFT (0U)
/*! STATUS - for each of these 19 bits: 0- No violation; 1-violation happened */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_STATUS_STATUS(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_STATUS_STATUS_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_STATUS_STATUS_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_CLR - CPU SYS Bus Access Sram Checker Violation Status Clear */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_CLR_STATUS_CLR_MASK (0x7FFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_CLR_STATUS_CLR_SHIFT (0U)
/*! STATUS_CLR - Write '1' to clear Violation Status bit; These bits are self-clear */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_CLR_STATUS_CLR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_CLR_STATUS_CLR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_CLR_STATUS_CLR_MASK)
/*! @} */

/*! @name SOC_MCI_EXTRA - Reserved register */
/*! @{ */

#define SYSCTL2_SOC_MCI_EXTRA_SOC_MCI_EXTRA_MASK (0xFFFFFFFFU)
#define SYSCTL2_SOC_MCI_EXTRA_SOC_MCI_EXTRA_SHIFT (0U)
/*! SOC_MCI_EXTRA - Reserved register */
#define SYSCTL2_SOC_MCI_EXTRA_SOC_MCI_EXTRA(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_SOC_MCI_EXTRA_SOC_MCI_EXTRA_SHIFT)) & SYSCTL2_SOC_MCI_EXTRA_SOC_MCI_EXTRA_MASK)
/*! @} */

/*! @name MCI_SOC_EXTRA - Reserved register */
/*! @{ */

#define SYSCTL2_MCI_SOC_EXTRA_MCI_SOC_EXTRA_MASK (0xFFFFFFFFU)
#define SYSCTL2_MCI_SOC_EXTRA_MCI_SOC_EXTRA_SHIFT (0U)
/*! MCI_SOC_EXTRA - Reserved register */
#define SYSCTL2_MCI_SOC_EXTRA_MCI_SOC_EXTRA(x)   (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MCI_SOC_EXTRA_MCI_SOC_EXTRA_SHIFT)) & SYSCTL2_MCI_SOC_EXTRA_MCI_SOC_EXTRA_MASK)
/*! @} */

/*! @name ROM_DYN_CLK_EN - To control dynamic clock gating of the rom clock */
/*! @{ */

#define SYSCTL2_ROM_DYN_CLK_EN_ROM_DYN_CLK_EN_MASK (0x2U)
#define SYSCTL2_ROM_DYN_CLK_EN_ROM_DYN_CLK_EN_SHIFT (1U)
/*! ROM_DYN_CLK_EN - To control dynamic clock gating of the rom clock
 *  0b0..Disable the clock gating
 *  0b1..Enable the clock gating
 */
#define SYSCTL2_ROM_DYN_CLK_EN_ROM_DYN_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_ROM_DYN_CLK_EN_ROM_DYN_CLK_EN_SHIFT)) & SYSCTL2_ROM_DYN_CLK_EN_ROM_DYN_CLK_EN_MASK)
/*! @} */

/*! @name RESERVED_REG1 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG1_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG1_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG1_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG1_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG1_RESERVED_REG_MASK)
/*! @} */

/*! @name RESERVED_REG2 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG2_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG2_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG2_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG2_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG2_RESERVED_REG_MASK)
/*! @} */

/*! @name RESERVED_REG3 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG3_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG3_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG3_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG3_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG3_RESERVED_REG_MASK)
/*! @} */

/*! @name RESERVED_REG4 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG4_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG4_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG4_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG4_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG4_RESERVED_REG_MASK)
/*! @} */

/*! @name RESERVED_REG5 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG5_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG5_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG5_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG5_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG5_RESERVED_REG_MASK)
/*! @} */

/*! @name RESERVED_REG6 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG6_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG6_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG6_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG6_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG6_RESERVED_REG_MASK)
/*! @} */

/*! @name RESERVED_REG7 - Reserved register */
/*! @{ */

#define SYSCTL2_RESERVED_REG7_RESERVED_REG_MASK  (0xFFFFFFFFU)
#define SYSCTL2_RESERVED_REG7_RESERVED_REG_SHIFT (0U)
/*! RESERVED_REG - Reserved register */
#define SYSCTL2_RESERVED_REG7_RESERVED_REG(x)    (((uint32_t)(((uint32_t)(x)) << SYSCTL2_RESERVED_REG7_RESERVED_REG_SHIFT)) & SYSCTL2_RESERVED_REG7_RESERVED_REG_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR0 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR0_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR0_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR0_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR0_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR0_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC0 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC0_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR1 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR1_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR1_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR1_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR1_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR1_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC1 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC1_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR2 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR2_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR2_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR2_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR2_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR2_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC2 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC2_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR3 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR3_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR3_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR3_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR3_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR3_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC3 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC3_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR4 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR4_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR4_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR4_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR4_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR4_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC4 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC4_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR5 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR5_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR5_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR5_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR5_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR5_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC5 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC5_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR6 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR6_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR6_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR6_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR6_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR6_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC6 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC6_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR7 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR7_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR7_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR7_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR7_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR7_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC7 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC7_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR8 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR8_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR8_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR8_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR8_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR8_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC8 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC8_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR9 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR9_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR9_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR9_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR9_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR9_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC9 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC9_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR10 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR10_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR10_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR10_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR10_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR10_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC10 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC10_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR11 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR11_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR11_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR11_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR11_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR11_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC11 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC11_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR12 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR12_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR12_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR12_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR12_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR12_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC12 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC12_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR13 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR13_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR13_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR13_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR13_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR13_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC13 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC13_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR14 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR14_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR14_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR14_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR14_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR14_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC14 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC14_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR15 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR15_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR15_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR15_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR15_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR15_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC15 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC15_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR16 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR16_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR16_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR16_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR16_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR16_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC16 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC16_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR17 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR17_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR17_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR17_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR17_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR17_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC17 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC17_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_ADDR18 - CPU CODE Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR18_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR18_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR18_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR18_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_ADDR18_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_CODE_VIO_MISC18 - CPU CODE Bus Access Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_CODE_VIO_MISC18_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR0 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR0_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR0_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR0_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR0_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR0_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC0 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC0_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR1 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR1_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR1_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR1_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR1_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR1_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC1 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC1_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR2 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR2_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR2_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR2_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR2_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR2_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC2 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC2_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR3 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR3_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR3_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR3_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR3_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR3_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC3 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC3_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR4 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR4_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR4_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR4_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR4_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR4_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC4 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC4_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR5 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR5_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR5_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR5_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR5_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR5_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC5 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC5_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR6 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR6_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR6_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR6_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR6_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR6_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC6 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC6_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR7 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR7_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR7_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR7_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR7_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR7_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC7 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC7_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR8 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR8_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR8_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR8_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR8_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR8_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC8 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC8_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR9 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR9_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR9_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR9_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR9_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR9_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC9 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC9_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR10 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR10_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR10_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR10_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR10_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR10_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC10 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC10_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR11 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR11_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR11_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR11_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR11_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR11_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC11 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC11_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR12 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR12_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR12_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR12_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR12_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR12_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC12 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC12_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR13 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR13_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR13_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR13_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR13_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR13_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC13 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC13_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR14 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR14_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR14_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR14_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR14_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR14_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC14 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC14_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR15 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR15_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR15_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR15_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR15_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR15_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC15 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC15_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR16 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR16_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR16_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR16_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR16_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR16_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC16 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC16_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR17 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR17_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR17_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR17_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR17_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR17_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC17 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC17_HSEC_LEVEL_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_ADDR18 - CPU SYS Bus Access Sram Checker: Address of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR18_ADDR_MASK (0xFFFFFFFFU)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR18_ADDR_SHIFT (0U)
/*! ADDR - Address of Violated Transfer */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR18_ADDR(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR18_ADDR_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_ADDR18_ADDR_MASK)
/*! @} */

/*! @name MEM_ACC_CHK_SYS_VIO_MISC18 - CPU SYS BusAccess Sram Checker: Misc information of Violated Transfer */
/*! @{ */

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_WRITE_MASK (0x1U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_WRITE_SHIFT (0U)
/*! WRITE - HWRITE of Violated Transfer. 1-Write; 0-Read */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_WRITE(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_WRITE_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_WRITE_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HPROT0_MASK (0x2U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HPROT0_SHIFT (1U)
/*! HPROT0 - HPROT0 of Violated Transfer. 1-Data Access; 0-OPCODE */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HPROT0(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HPROT0_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HPROT0_MASK)

#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HSEC_LEVEL_MASK (0xF0U)
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HSEC_LEVEL_SHIFT (4U)
/*! HSEC_LEVEL - HSEC_LEVEL of Violated Transfer. */
#define SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HSEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HSEC_LEVEL_SHIFT)) & SYSCTL2_MEM_ACC_CHK_SYS_VIO_MISC18_HSEC_LEVEL_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SYSCTL2_Register_Masks */


/*!
 * @}
 */ /* end of group SYSCTL2_Peripheral_Access_Layer */


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


#endif  /* SYSCTL2_H_ */

