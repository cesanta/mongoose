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
**         CMSIS Peripheral Access Layer for AHB_SECURE_CTRL
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
 * @file AHB_SECURE_CTRL.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for AHB_SECURE_CTRL
 *
 * CMSIS Peripheral Access Layer for AHB_SECURE_CTRL
 */

#if !defined(AHB_SECURE_CTRL_H_)
#define AHB_SECURE_CTRL_H_                       /**< Symbol preventing repeated inclusion */

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
   -- AHB_SECURE_CTRL Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AHB_SECURE_CTRL_Peripheral_Access_Layer AHB_SECURE_CTRL Peripheral Access Layer
 * @{
 */

/** AHB_SECURE_CTRL - Size of Registers Arrays */
#define AHB_SECURE_CTRL_BOOTROM_COUNT             4u
#define AHB_SECURE_CTRL_FLEXSPI_RULES_FLEXSPI_RULES_REGION0_MEM_RULE_COUNT 4u
#define AHB_SECURE_CTRL_FLEXSPI_RULES_FLEXSPI_RULES_REGION5_MEM_RULE_COUNT 4u
#define AHB_SECURE_CTRL_FLEXSPI_RULES_COUNT       3u
#define AHB_SECURE_CTRL_RAM00_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM01_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM02_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM03_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM04_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM05_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM06_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM07_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM08_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM09_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM10_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM11_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM12_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM13_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM14_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM15_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM16_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM17_RULE_COUNT          4u
#define AHB_SECURE_CTRL_RAM18_RULE_COUNT          4u
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_COUNT    4u
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_COUNT     4u
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_COUNT    4u
#define AHB_SECURE_CTRL_SEC_VIO_ADDR_COUNT        20u
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_COUNT   20u

/** AHB_SECURE_CTRL - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[16];
  __IO uint32_t BOOTROM0_MEM_RULE[AHB_SECURE_CTRL_BOOTROM_COUNT]; /**< 0x03000000-0x0303FFFF part0..0x03000000-0x0303FFFF part3, array offset: 0x10, array step: 0x4 */
  struct {                                         /* offset: 0x20, array step: 0xB0 */
         uint8_t RESERVED_0[16];
    __IO uint32_t REGION0_MEM_RULE[AHB_SECURE_CTRL_FLEXSPI_RULES_FLEXSPI_RULES_REGION0_MEM_RULE_COUNT];   /**< 0x08000000--0x080FFFFF..0x48300000--0x483FFFFF, array offset: 0x30, array step: index*0xB0, index2*0x4 */
    __IO uint32_t REGION1_MEM_RULE;                  /**< 0x08400000--0x087FFFFF..0x48400000--0x487FFFFF, array offset: 0x40, array step: 0xB0 */
         uint8_t RESERVED_1[12];
    __IO uint32_t REGION2_MEM_RULE;                  /**< 0x08800000--0x08FFFFFF..0x48800000--0x48FFFFFF, array offset: 0x50, array step: 0xB0 */
         uint8_t RESERVED_2[12];
    __IO uint32_t REGION3_MEM_RULE;                  /**< 0x09000000--0x09FFFFFF..0x49000000--0x49FFFFFF, array offset: 0x60, array step: 0xB0 */
         uint8_t RESERVED_3[12];
    __IO uint32_t REGION4_MEM_RULE;                  /**< 0x0A000000--0x0BFFFFFF..0x4A000000--0x4BFFFFFF, array offset: 0x70, array step: 0xB0 */
         uint8_t RESERVED_4[12];
    __IO uint32_t REGION5_MEM_RULE[AHB_SECURE_CTRL_FLEXSPI_RULES_FLEXSPI_RULES_REGION5_MEM_RULE_COUNT];   /**< 0x0C000000--0x0C0FFFFF..0x4C300000--0x4C3FFFFF, array offset: 0x80, array step: index*0xB0, index2*0x4 */
    __IO uint32_t REGION6_MEM_RULE;                  /**< 0x0C400000--0x0C7FFFFF..0x4C400000--0x4C7FFFFF, array offset: 0x90, array step: 0xB0 */
         uint8_t RESERVED_5[12];
    __IO uint32_t REGION7_MEM_RULE;                  /**< 0x0C800000--0x0CFFFFFF..0x4C800000--0x4CFFFFFF, array offset: 0xA0, array step: 0xB0 */
         uint8_t RESERVED_6[12];
    __IO uint32_t REGION8_MEM_RULE;                  /**< 0x0D000000~0x0DFFFFFFF..0x4D000000~0x4DFFFFFFF, array offset: 0xB0, array step: 0xB0 */
         uint8_t RESERVED_7[12];
    __IO uint32_t REGION9_MEM_RULE;                  /**< 0x0E000000~0x0FFFFFFF..0x4E000000~0x4FFFFFFF, array offset: 0xC0, array step: 0xB0 */
         uint8_t RESERVED_8[12];
  } FLEXSPI_RULES[AHB_SECURE_CTRL_FLEXSPI_RULES_COUNT];
       uint8_t RESERVED_1[16];
  __IO uint32_t RAM00_RULE[AHB_SECURE_CTRL_RAM00_RULE_COUNT]; /**< 0x20000000--0x2000FFFF part0..0x20000000--0x2000FFFF part3, array offset: 0x240, array step: 0x4 */
  __IO uint32_t RAM01_RULE[AHB_SECURE_CTRL_RAM01_RULE_COUNT]; /**< 0x20010000--0x2001FFFF part0..0x20010000--0x2001FFFF part3, array offset: 0x250, array step: 0x4 */
       uint8_t RESERVED_2[16];
  __IO uint32_t RAM02_RULE[AHB_SECURE_CTRL_RAM02_RULE_COUNT]; /**< 0x20020000--0x2002FFFF part0..0x20020000--0x2002FFFF part3, array offset: 0x270, array step: 0x4 */
  __IO uint32_t RAM03_RULE[AHB_SECURE_CTRL_RAM03_RULE_COUNT]; /**< 0x20030000--0x2003FFFF part0..0x20030000--0x2003FFFF part3, array offset: 0x280, array step: 0x4 */
       uint8_t RESERVED_3[16];
  __IO uint32_t RAM04_RULE[AHB_SECURE_CTRL_RAM04_RULE_COUNT]; /**< 0x20040000--0x2004FFFF part0..0x20040000--0x2004FFFF part3, array offset: 0x2A0, array step: 0x4 */
  __IO uint32_t RAM05_RULE[AHB_SECURE_CTRL_RAM05_RULE_COUNT]; /**< 0x20050000--0x2005FFFF part0..0x20050000--0x2005FFFF part3, array offset: 0x2B0, array step: 0x4 */
       uint8_t RESERVED_4[16];
  __IO uint32_t RAM06_RULE[AHB_SECURE_CTRL_RAM06_RULE_COUNT]; /**< 0x20060000--0x2006FFFF part0..0x20060000--0x2006FFFF part3, array offset: 0x2D0, array step: 0x4 */
  __IO uint32_t RAM07_RULE[AHB_SECURE_CTRL_RAM07_RULE_COUNT]; /**< 0x20070000--0x2007FFFF part0..0x20070000--0x2007FFFF part3, array offset: 0x2E0, array step: 0x4 */
       uint8_t RESERVED_5[16];
  __IO uint32_t RAM08_RULE[AHB_SECURE_CTRL_RAM08_RULE_COUNT]; /**< 0x20080000--0x2008FFFF part0..0x20080000--0x2008FFFF part3, array offset: 0x300, array step: 0x4 */
  __IO uint32_t RAM09_RULE[AHB_SECURE_CTRL_RAM09_RULE_COUNT]; /**< 0x20090000--0x2009FFFF part0..0x20090000--0x2009FFFF part3, array offset: 0x310, array step: 0x4 */
  __IO uint32_t RAM10_RULE[AHB_SECURE_CTRL_RAM10_RULE_COUNT]; /**< 0x200A0000--0x200AFFFF part0..0x200A0000--0x200AFFFF part3, array offset: 0x320, array step: 0x4 */
  __IO uint32_t RAM11_RULE[AHB_SECURE_CTRL_RAM11_RULE_COUNT]; /**< 0x200B0000--0x200BFFFF part0..0x200B0000--0x200BFFFF part3, array offset: 0x330, array step: 0x4 */
       uint8_t RESERVED_6[16];
  __IO uint32_t RAM12_RULE[AHB_SECURE_CTRL_RAM12_RULE_COUNT]; /**< 0x200C0000--0x200CFFFF part0..0x200C0000--0x200CFFFF part3, array offset: 0x350, array step: 0x4 */
  __IO uint32_t RAM13_RULE[AHB_SECURE_CTRL_RAM13_RULE_COUNT]; /**< 0x200D0000--0x200DFFFF part0..0x200D0000--0x200DFFFF part3, array offset: 0x360, array step: 0x4 */
  __IO uint32_t RAM14_RULE[AHB_SECURE_CTRL_RAM14_RULE_COUNT]; /**< 0x200E0000--0x200EFFFF part0..0x200E0000--0x200EFFFF part3, array offset: 0x370, array step: 0x4 */
  __IO uint32_t RAM15_RULE[AHB_SECURE_CTRL_RAM15_RULE_COUNT]; /**< 0x200F0000--0x200FFFFF part0..0x200F0000--0x200FFFFF part3, array offset: 0x380, array step: 0x4 */
       uint8_t RESERVED_7[16];
  __IO uint32_t RAM16_RULE[AHB_SECURE_CTRL_RAM16_RULE_COUNT]; /**< 0x20100000--0x2010FFFF part0..0x20100000--0x2010FFFF part3, array offset: 0x3A0, array step: 0x4 */
  __IO uint32_t RAM17_RULE[AHB_SECURE_CTRL_RAM17_RULE_COUNT]; /**< 0x20011000--0x20011FFF part0..0x20011000--0x20011FFF part3, array offset: 0x3B0, array step: 0x4 */
  __IO uint32_t RAM18_RULE[AHB_SECURE_CTRL_RAM18_RULE_COUNT]; /**< 0x20012000--0x20012FFF part0..0x20012000--0x20012FFF part3, array offset: 0x3C0, array step: 0x4 */
       uint8_t RESERVED_8[16];
  __IO uint32_t APB_GRP0_MEM_RULE0;                /**< 0x4000000--0x40007FFF, offset: 0x3E0 */
  __IO uint32_t APB_GRP0_MEM_RULE1;                /**< 0x40008000~0x4000FFFF, offset: 0x3E4 */
  __IO uint32_t APB_GRP0_MEM_RULE2;                /**< 0x40010000--0x40017FFF, offset: 0x3E8 */
  __IO uint32_t APB_GRP0_MEM_RULE3;                /**< 0x40018000--0x4001FFFF, offset: 0x3EC */
  __IO uint32_t APB_GRP1_MEM_RULE0;                /**< 0x40020000--0x40027FFF, offset: 0x3F0 */
  __IO uint32_t APB_GRP1_MEM_RULE1;                /**< 0x40028000--0x4002FFFF, offset: 0x3F4 */
  __IO uint32_t APB_GRP1_MEM_RULE2;                /**< 0x40030000--0x40037FFF, offset: 0x3F8 */
  __IO uint32_t APB_GRP1_MEM_RULE3;                /**< 0x40038000--0x4003FFFF, offset: 0x3FC */
  __IO uint32_t AHB_PERIPH0_SLAVE_RULE;            /**< 0x40100000--0x4011FFFF, offset: 0x400 */
       uint8_t RESERVED_9[12];
  __IO uint32_t AHB_PERIPH1_SLAVE_RULE;            /**< 0x40120000--0x40127FFF, offset: 0x410 */
       uint8_t RESERVED_10[28];
  __IO uint32_t AIPS_BRIDGE_MEM_RULE0;             /**< 0x4013 0000 ~0x4013 7FFF, offset: 0x430 */
  __IO uint32_t AIPS_BRIDGE_MEM_RULE1;             /**< 0x4013 8000 ~0x4013 FFFF, offset: 0x434 */
       uint8_t RESERVED_11[8];
  __IO uint32_t AHB_PERIPH2_SLAVE_RULE;            /**< 0x40140000--0x4014FFFF, offset: 0x440 */
       uint8_t RESERVED_12[12];
  __IO uint32_t SECURITY_CTRL_MEM_RULE;            /**< 0x40148000--0x4014BFFF, offset: 0x450 */
       uint8_t RESERVED_13[12];
  __IO uint32_t AHB_PERIPH3_SLAVE_RULE;            /**< the memory map is 0x40150000--0x40158FFF, offset: 0x460 */
       uint8_t RESERVED_14[12];
  __IO uint32_t AON_MEM_RULE;                      /**< 0x4015C000--0x4015FFFF, offset: 0x470 */
       uint8_t RESERVED_15[12];
  __IO uint32_t WLAN_S0_SLAVE_RULE;                /**< 0x41000000-0x4137FFFF, 3.5MB, offset: 0x480 */
       uint8_t RESERVED_16[12];
  __IO uint32_t WLAN_S1_MEM_RULE[AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_COUNT]; /**< 0x41380000-0x413FFFFF, part0..0x41380000-0x413FFFFF, part3, array offset: 0x490, array step: 0x4 */
  __IO uint32_t BLE_S0_SLAVE_RULE;                 /**< 0x44000000-0x443BFFFF, offset: 0x4A0 */
       uint8_t RESERVED_17[12];
  __IO uint32_t BLE_S1_MEM_RULE[AHB_SECURE_CTRL_BLE_S1_MEM_RULE_COUNT]; /**< 0x443C0000-0x443FFFFF part0..0x443C0000-0x443FFFFF part3, array offset: 0x4B0, array step: 0x4 */
       uint8_t RESERVED_18[16];
  __IO uint32_t SOC_TOP_MEM_RULE[AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_COUNT]; /**< 0x45000000-0x4500FFFF part0..0x45000000-0x4500FFFF part3, array offset: 0x4D0, array step: 0x4 */
       uint8_t RESERVED_19[2336];
  __I  uint32_t SEC_VIO_ADDR[AHB_SECURE_CTRL_SEC_VIO_ADDR_COUNT]; /**< most recent security violation address for AHB layer n, array offset: 0xE00, array step: 0x4 */
       uint8_t RESERVED_20[48];
  __I  uint32_t SEC_VIO_MISC_INFO[AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_COUNT]; /**< most recent security violation miscellaneous information for AHB layer n, array offset: 0xE80, array step: 0x4 */
       uint8_t RESERVED_21[48];
  __IO uint32_t SEC_VIO_INFO_VALID;                /**< security violation address/information registers valid flags, offset: 0xF00 */
       uint8_t RESERVED_22[124];
  __IO uint32_t SEC_GPIO_MASK0;                    /**< Secure GPIO mask for port 0 pins. This register is used to block leakage of Secure interface (GPIOs, I2C, UART configured as secure peripherals) pin states to non-secure world., offset: 0xF80 */
  __IO uint32_t SEC_GPIO_MASK1;                    /**< Secure GPIO mask for port 1 pins., offset: 0xF84 */
       uint8_t RESERVED_23[52];
  __IO uint32_t SEC_MASK_LOCK;                     /**< sec_gp_reg write-lock bits, offset: 0xFBC */
       uint8_t RESERVED_24[16];
  __IO uint32_t MASTER_SEC_LEVEL;                  /**< master secure level register, offset: 0xFD0 */
  __IO uint32_t MASTER_SEC_LEVEL_ANTI_POL;         /**< master secure level anti-pole register, offset: 0xFD4 */
       uint8_t RESERVED_25[20];
  __IO uint32_t CM33_LOCK_REG;                     /**< m33 lock control register, offset: 0xFEC */
       uint8_t RESERVED_26[8];
  __IO uint32_t MISC_CTRL_DP_REG;                  /**< secure control duplicate register, offset: 0xFF8 */
  __IO uint32_t MISC_CTRL_REG;                     /**< secure control register, offset: 0xFFC */
} AHB_SECURE_CTRL_Type;

/* ----------------------------------------------------------------------------
   -- AHB_SECURE_CTRL Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AHB_SECURE_CTRL_Register_Masks AHB_SECURE_CTRL Register Masks
 * @{
 */

/*! @name BOOTROM0_MEM_RULE - 0x03000000-0x0303FFFF part0..0x03000000-0x0303FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_BOOTROM0_MEM_RULE */
#define AHB_SECURE_CTRL_BOOTROM0_MEM_RULE_COUNT  (4U)

/*! @name FLEXSPI_RULES_REGION0_MEM_RULE - 0x08000000--0x080FFFFF..0x48300000--0x483FFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_COUNT (3U)

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION0_MEM_RULE_COUNT2 (4U)

/*! @name FLEXSPI_RULES_REGION1_MEM_RULE - 0x08400000--0x087FFFFF..0x48400000--0x487FFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION1_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION2_MEM_RULE - 0x08800000--0x08FFFFFF..0x48800000--0x48FFFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION2_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION3_MEM_RULE - 0x09000000--0x09FFFFFF..0x49000000--0x49FFFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION3_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION4_MEM_RULE - 0x0A000000--0x0BFFFFFF..0x4A000000--0x4BFFFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION4_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION5_MEM_RULE - 0x0C000000--0x0C0FFFFF..0x4C300000--0x4C3FFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_COUNT (3U)

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION5_MEM_RULE_COUNT2 (4U)

/*! @name FLEXSPI_RULES_REGION6_MEM_RULE - 0x0C400000--0x0C7FFFFF..0x4C400000--0x4C7FFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION6_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION7_MEM_RULE - 0x0C800000--0x0CFFFFFF..0x4C800000--0x4CFFFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION7_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION8_MEM_RULE - 0x0D000000~0x0DFFFFFFF..0x4D000000~0x4DFFFFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION8_MEM_RULE_COUNT (3U)

/*! @name FLEXSPI_RULES_REGION9_MEM_RULE - 0x0E000000~0x0FFFFFFF..0x4E000000~0x4FFFFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_RULE7_MASK)
/*! @} */

/* The count of AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE */
#define AHB_SECURE_CTRL_FLEXSPI_RULES_REGION9_MEM_RULE_COUNT (3U)

/*! @name RAM00_RULE - 0x20000000--0x2000FFFF part0..0x20000000--0x2000FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM00_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM00_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM00_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM00_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM00_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM00_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM01_RULE - 0x20010000--0x2001FFFF part0..0x20010000--0x2001FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM01_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM01_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM01_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM01_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM01_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM01_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM02_RULE - 0x20020000--0x2002FFFF part0..0x20020000--0x2002FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM02_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM02_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM02_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM02_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM02_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM02_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM03_RULE - 0x20030000--0x2003FFFF part0..0x20030000--0x2003FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM03_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM03_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM03_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM03_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM03_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM03_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM04_RULE - 0x20040000--0x2004FFFF part0..0x20040000--0x2004FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM04_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM04_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM04_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM04_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM04_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM04_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM05_RULE - 0x20050000--0x2005FFFF part0..0x20050000--0x2005FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM05_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM05_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM05_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM05_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM05_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM05_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM06_RULE - 0x20060000--0x2006FFFF part0..0x20060000--0x2006FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM06_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM06_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM06_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM06_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM06_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM06_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM07_RULE - 0x20070000--0x2007FFFF part0..0x20070000--0x2007FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM07_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM07_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM07_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM07_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM07_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM07_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM08_RULE - 0x20080000--0x2008FFFF part0..0x20080000--0x2008FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM08_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM08_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM08_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM08_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM08_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM08_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM09_RULE - 0x20090000--0x2009FFFF part0..0x20090000--0x2009FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM09_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM09_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM09_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM09_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM09_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM09_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM10_RULE - 0x200A0000--0x200AFFFF part0..0x200A0000--0x200AFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM10_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM10_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM10_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM10_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM10_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM10_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM11_RULE - 0x200B0000--0x200BFFFF part0..0x200B0000--0x200BFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM11_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM11_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM11_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM11_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM11_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM11_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM12_RULE - 0x200C0000--0x200CFFFF part0..0x200C0000--0x200CFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM12_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM12_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM12_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM12_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM12_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM12_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM13_RULE - 0x200D0000--0x200DFFFF part0..0x200D0000--0x200DFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM13_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM13_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM13_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM13_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM13_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM13_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM14_RULE - 0x200E0000--0x200EFFFF part0..0x200E0000--0x200EFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM14_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM14_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM14_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM14_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM14_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM14_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM15_RULE - 0x200F0000--0x200FFFFF part0..0x200F0000--0x200FFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM15_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM15_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM15_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM15_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM15_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM15_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM16_RULE - 0x20100000--0x2010FFFF part0..0x20100000--0x2010FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM16_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM16_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM16_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM16_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM16_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM16_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM17_RULE - 0x20011000--0x20011FFF part0..0x20011000--0x20011FFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM17_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM17_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM17_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM17_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM17_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM17_RULE_RULE7_MASK)
/*! @} */

/*! @name RAM18_RULE - 0x20012000--0x20012FFF part0..0x20012000--0x20012FFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_RAM18_RULE_RULE0_MASK    (0x3U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE0_SHIFT   (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE0(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE1_MASK    (0x30U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE1_SHIFT   (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE1(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE2_MASK    (0x300U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE2_SHIFT   (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE2(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE3_MASK    (0x3000U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE3_SHIFT   (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE3(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE4_MASK    (0x30000U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE4_SHIFT   (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE4(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE5_MASK    (0x300000U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE5_SHIFT   (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE5(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE6_MASK    (0x3000000U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE6_SHIFT   (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE6(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_RAM18_RULE_RULE7_MASK    (0x30000000U)
#define AHB_SECURE_CTRL_RAM18_RULE_RULE7_SHIFT   (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_RAM18_RULE_RULE7(x)      (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_RAM18_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_RAM18_RULE_RULE7_MASK)
/*! @} */

/*! @name APB_GRP0_MEM_RULE0 - 0x4000000--0x40007FFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_RSTCTL0_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_RSTCTL0_RULE0_SHIFT (0U)
/*! RSTCTL0_RULE0 - 0x4000 0000--0x4000 0FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_RSTCTL0_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_RSTCTL0_RULE0_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_RSTCTL0_RULE0_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_CLKCTL0_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_CLKCTL0_RULE1_SHIFT (4U)
/*! CLKCTL0_RULE1 - 0x4000 1000--0x4000 1FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_CLKCTL0_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_CLKCTL0_RULE1_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_CLKCTL0_RULE1_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL0_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL0_RULE2_SHIFT (8U)
/*! SYSCTL0_RULE2 - 0x4000 2000--0x4000 2FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL0_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL0_RULE2_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL0_RULE2_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL2_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL2_RULE3_SHIFT (12U)
/*! SYSCTL2_RULE3 - 0x4000 3000--0x4000 3FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL2_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL2_RULE3_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_SYSCTL2_RULE3_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_IOCON_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_IOCON_RULE4_SHIFT (16U)
/*! IOCON_RULE4 - 0x4000 4000--0x4000 4FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_IOCON_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_IOCON_RULE4_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_IOCON_RULE4_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_PUFCTRL_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_PUFCTRL_RULE6_SHIFT (24U)
/*! PUFCTRL_RULE6 - 0x4000 6000--0x4000 6FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_PUFCTRL_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_PUFCTRL_RULE6_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_PUFCTRL_RULE6_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_ELS_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_ELS_RULE7_SHIFT (28U)
/*! ELS_RULE7 - 0x4000 7000--0x4000 7FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_ELS_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_ELS_RULE7_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE0_ELS_RULE7_MASK)
/*! @} */

/*! @name APB_GRP0_MEM_RULE1 - 0x40008000~0x4000FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_USIM_RULE8_MASK (0x3U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_USIM_RULE8_SHIFT (0U)
/*! USIM_RULE8 - 0x4000 8000--0x4000 8FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_USIM_RULE8(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_USIM_RULE8_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_USIM_RULE8_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_PKC_RULE9_MASK (0x30U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_PKC_RULE9_SHIFT (4U)
/*! PKC_RULE9 - 0x4000 9000--0x4000 9FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_PKC_RULE9(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_PKC_RULE9_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_PKC_RULE9_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_RULE10_MASK (0x300U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_RULE10_SHIFT (8U)
/*! OCOTP_RULE10 - 0x4000 A000--0x4000 AFFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_RULE10(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_RULE10_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_RULE10_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_ADAP_RULE11_MASK (0x3000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_ADAP_RULE11_SHIFT (12U)
/*! OCOTP_ADAP_RULE11 - 0x4000 A000--0x4000 AFFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_ADAP_RULE11(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_ADAP_RULE11_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_OCOTP_ADAP_RULE11_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_WWDT0_RULE14_MASK (0x3000000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_WWDT0_RULE14_SHIFT (24U)
/*! WWDT0_RULE14 - 0x4000 E000--0x4000 EFFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_WWDT0_RULE14(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_WWDT0_RULE14_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_WWDT0_RULE14_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_UTICK_RULE15_MASK (0x30000000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_UTICK_RULE15_SHIFT (28U)
/*! UTICK_RULE15 - 0x4000 F000--0x4000 FFFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_UTICK_RULE15(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_UTICK_RULE15_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE1_UTICK_RULE15_MASK)
/*! @} */

/*! @name APB_GRP0_MEM_RULE2 - 0x40010000--0x40017FFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE2_TRNG_RULE20_MASK (0x30000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE2_TRNG_RULE20_SHIFT (16U)
/*! TRNG_RULE20 - 0x4001 4000--0x4001 4FFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE2_TRNG_RULE20(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE2_TRNG_RULE20_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE2_TRNG_RULE20_MASK)
/*! @} */

/*! @name APB_GRP0_MEM_RULE3 - 0x40018000--0x4001FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0_DOMAIN_TESTCON_RULE27_MASK (0x3000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0_DOMAIN_TESTCON_RULE27_SHIFT (12U)
/*! C0_DOMAIN_TESTCON_RULE27 - 0x4001 B000--0x4001 BFFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0_DOMAIN_TESTCON_RULE27(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0_DOMAIN_TESTCON_RULE27_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0_DOMAIN_TESTCON_RULE27_MASK)

#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0AON_DOMAIN_TESTCON_RULE28_MASK (0x30000U)
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0AON_DOMAIN_TESTCON_RULE28_SHIFT (16U)
/*! C0AON_DOMAIN_TESTCON_RULE28 - 0x4001 C000--0x4001 CFFF */
#define AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0AON_DOMAIN_TESTCON_RULE28(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0AON_DOMAIN_TESTCON_RULE28_SHIFT)) & AHB_SECURE_CTRL_APB_GRP0_MEM_RULE3_C0AON_DOMAIN_TESTCON_RULE28_MASK)
/*! @} */

/*! @name APB_GRP1_MEM_RULE0 - 0x40020000--0x40027FFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_RSTCTL1_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_RSTCTL1_RULE0_SHIFT (0U)
/*! RSTCTL1_RULE0 - 0x4002 0000--0x4002 0FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_RSTCTL1_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_RSTCTL1_RULE0_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_RSTCTL1_RULE0_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_CLKCTL1_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_CLKCTL1_RULE1_SHIFT (4U)
/*! CLKCTL1_RULE1 - 0x4002 1000--0x4002 1FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_CLKCTL1_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_CLKCTL1_RULE1_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_CLKCTL1_RULE1_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_SYSCTL1_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_SYSCTL1_RULE2_SHIFT (8U)
/*! SYSCTL1_RULE2 - 0x4002 2000--0x4002 2FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_SYSCTL1_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_SYSCTL1_RULE2_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_SYSCTL1_RULE2_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_ITRC_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_ITRC_RULE4_SHIFT (16U)
/*! ITRC_RULE4 - 0x4002 4000--0x4002 4FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_ITRC_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_ITRC_RULE4_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_ITRC_RULE4_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_GPIO_INTR_CTRL_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_GPIO_INTR_CTRL_RULE5_SHIFT (20U)
/*! GPIO_INTR_CTRL_RULE5 - 0x4002 5000--0x4002 5FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_GPIO_INTR_CTRL_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_GPIO_INTR_CTRL_RULE5_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_GPIO_INTR_CTRL_RULE5_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_PERIPH_INPUT_MUX_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_PERIPH_INPUT_MUX_RULE6_SHIFT (24U)
/*! PERIPH_INPUT_MUX_RULE6 - 0x4002 6000--0x4002 6FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_PERIPH_INPUT_MUX_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_PERIPH_INPUT_MUX_RULE6_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE0_PERIPH_INPUT_MUX_RULE6_MASK)
/*! @} */

/*! @name APB_GRP1_MEM_RULE1 - 0x40028000--0x4002FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT0_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT0_RULE0_SHIFT (0U)
/*! CT32BIT0_RULE0 - 0x4002 8000--0x4002 8FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT0_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT0_RULE0_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT0_RULE0_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT1_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT1_RULE1_SHIFT (4U)
/*! CT32BIT1_RULE1 - 0x4002 9000--0x4002 9FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT1_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT1_RULE1_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT1_RULE1_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT2_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT2_RULE2_SHIFT (8U)
/*! CT32BIT2_RULE2 - 0x4002 A000--0x4002 AFFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT2_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT2_RULE2_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT2_RULE2_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT3_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT3_RULE3_SHIFT (12U)
/*! CT32BIT3_RULE3 - 0x4002 B000--0x4002 BFFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT3_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT3_RULE3_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_CT32BIT3_RULE3_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_MRT_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_MRT_RULE5_SHIFT (20U)
/*! MRT_RULE5 - 0x4002 D000--0x4002 DFFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_MRT_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_MRT_RULE5_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_MRT_RULE5_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_FREQME_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_FREQME_RULE7_SHIFT (28U)
/*! FREQME_RULE7 - 0x4002 F000--0x4002 FFFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_FREQME_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_FREQME_RULE7_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE1_FREQME_RULE7_MASK)
/*! @} */

/*! @name APB_GRP1_MEM_RULE2 - 0x40030000--0x40037FFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_RTC_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_RTC_RULE0_SHIFT (0U)
/*! RTC_RULE0 - 0x4003 0000--0x4003 0FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_RTC_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_RTC_RULE0_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_RTC_RULE0_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_PMU_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_PMU_RULE1_SHIFT (4U)
/*! PMU_RULE1 - 0x4003 1000--0x4003 1FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_PMU_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_PMU_RULE1_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_PMU_RULE1_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE0_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE0_RULE3_SHIFT (12U)
/*! FLASH_CACHE0_RULE3 - 0x4003 3000--0x4003 3FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE0_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE0_RULE3_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE0_RULE3_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE1_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE1_RULE4_SHIFT (16U)
/*! FLASH_CACHE1_RULE4 - 0x4003 4000--0x4003 4FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE1_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE1_RULE4_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE2_FLASH_CACHE1_RULE4_MASK)
/*! @} */

/*! @name APB_GRP1_MEM_RULE3 - 0x40038000--0x4003FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_GAU_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_GAU_RULE0_SHIFT (0U)
/*! GAU_RULE0 - 0x4003 8000--0x4003 8FFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_GAU_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_GAU_RULE0_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_GAU_RULE0_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_RF_SYSCON_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_RF_SYSCON_RULE3_SHIFT (12U)
/*! RF_SYSCON_RULE3 - 0x4003 B000--0x4003 BFFF */
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_RF_SYSCON_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_RF_SYSCON_RULE3_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_RF_SYSCON_RULE3_MASK)

#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_FREEMRT_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_FREEMRT_RULE7_SHIFT (28U)
#define AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_FREEMRT_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_FREEMRT_RULE7_SHIFT)) & AHB_SECURE_CTRL_APB_GRP1_MEM_RULE3_FREEMRT_RULE7_MASK)
/*! @} */

/*! @name AHB_PERIPH0_SLAVE_RULE - 0x40100000--0x4011FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_HSGPIO_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_HSGPIO_RULE0_SHIFT (0U)
/*! HSGPIO_RULE0 - 0x40100000--0x40103FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_HSGPIO_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_HSGPIO_RULE0_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_HSGPIO_RULE0_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA0_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA0_RULE1_SHIFT (4U)
/*! DMA0_RULE1 - 0x40104000--0x40104FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA0_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA0_RULE1_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA0_RULE1_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA1_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA1_RULE2_SHIFT (8U)
/*! DMA1_RULE2 - 0x40105000--0x40105FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA1_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA1_RULE2_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DMA1_RULE2_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM0_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM0_RULE3_SHIFT (12U)
/*! FLEXCOMM0_RULE3 - 0x40106000--0x40106FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM0_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM0_RULE3_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM0_RULE3_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM1_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM1_RULE4_SHIFT (16U)
/*! FLEXCOMM1_RULE4 - 0x40107000--0x40107FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM1_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM1_RULE4_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM1_RULE4_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM2_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM2_RULE5_SHIFT (20U)
/*! FLEXCOMM2_RULE5 - 0x40108000--0x40108FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM2_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM2_RULE5_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM2_RULE5_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM3_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM3_RULE6_SHIFT (24U)
/*! FLEXCOMM3_RULE6 - 0x40109000--0x40109FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM3_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM3_RULE6_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_FLEXCOMM3_RULE6_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DEBUG_MAILBOX_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DEBUG_MAILBOX_RULE7_SHIFT (28U)
/*! DEBUG_MAILBOX_RULE7 - 0x4010F000--0x4010FFFF */
#define AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DEBUG_MAILBOX_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DEBUG_MAILBOX_RULE7_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH0_SLAVE_RULE_DEBUG_MAILBOX_RULE7_MASK)
/*! @} */

/*! @name AHB_PERIPH1_SLAVE_RULE - 0x40120000--0x40127FFF */
/*! @{ */

#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_CRC_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_CRC_RULE0_SHIFT (0U)
/*! CRC_RULE0 - 0x40120000--0x40120FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_CRC_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_CRC_RULE0_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_CRC_RULE0_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_DMIC_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_DMIC_RULE1_SHIFT (4U)
/*! DMIC_RULE1 - 0x40121000--0x40121FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_DMIC_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_DMIC_RULE1_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_DMIC_RULE1_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM4_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM4_RULE2_SHIFT (8U)
/*! FLEXCOMM4_RULE2 - 0x40122000--0x40122FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM4_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM4_RULE2_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM4_RULE2_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM14_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM14_RULE6_SHIFT (24U)
/*! FLEXCOMM14_RULE6 - 0x40126000--0x40126FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM14_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM14_RULE6_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH1_SLAVE_RULE_FLEXCOMM14_RULE6_MASK)
/*! @} */

/*! @name AIPS_BRIDGE_MEM_RULE0 - 0x4013 0000 ~0x4013 7FFF */
/*! @{ */

#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE0_OSPI_AND_OTFAD_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE0_OSPI_AND_OTFAD_RULE4_SHIFT (16U)
/*! OSPI_AND_OTFAD_RULE4 - 0x4013 4000--0x4013 4FFF */
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE0_OSPI_AND_OTFAD_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE0_OSPI_AND_OTFAD_RULE4_SHIFT)) & AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE0_OSPI_AND_OTFAD_RULE4_MASK)
/*! @} */

/*! @name AIPS_BRIDGE_MEM_RULE1 - 0x4013 8000 ~0x4013 FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ENET_MASK (0x3U)
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ENET_SHIFT (0U)
/*! ENET - 0x4013 8000--0x4013 8FFF */
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ENET(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ENET_SHIFT)) & AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ENET_MASK)

#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_OSTIMER_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_OSTIMER_RULE3_SHIFT (12U)
/*! OSTIMER_RULE3 - 0x4013 B000--0x4013 BFFF */
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_OSTIMER_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_OSTIMER_RULE3_SHIFT)) & AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_OSTIMER_RULE3_MASK)

#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ROM_CTRL_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ROM_CTRL_RULE4_SHIFT (16U)
/*! ROM_CTRL_RULE4 - 0x4013 C000--0x4013 CFFF */
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ROM_CTRL_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ROM_CTRL_RULE4_SHIFT)) & AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ROM_CTRL_RULE4_MASK)

#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_MTR_TEST_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_MTR_TEST_RULE6_SHIFT (24U)
/*! MTR_TEST_RULE6 - 0x4013 E000--0x4013 EFFF */
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_MTR_TEST_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_MTR_TEST_RULE6_SHIFT)) & AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_MTR_TEST_RULE6_MASK)

#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ATX_TEST_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ATX_TEST_RULE7_SHIFT (28U)
/*! ATX_TEST_RULE7 - 0x4013 F000--0x4013 FFFF */
#define AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ATX_TEST_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ATX_TEST_RULE7_SHIFT)) & AHB_SECURE_CTRL_AIPS_BRIDGE_MEM_RULE1_ATX_TEST_RULE7_MASK)
/*! @} */

/*! @name AHB_PERIPH2_SLAVE_RULE - 0x40140000--0x4014FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_USBOTG_MASK (0x3U)
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_USBOTG_SHIFT (0U)
/*! USBOTG - 0x40145000--0x40145FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_USBOTG(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_USBOTG_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_USBOTG_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_SCT_MASK (0x30U)
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_SCT_SHIFT (4U)
/*! SCT - 0x40146000--0x40146FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_SCT(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_SCT_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_SCT_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_GDMA_MASK (0x300U)
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_GDMA_SHIFT (8U)
/*! GDMA - 0x4014E000--0x4014EFFF */
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_GDMA(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_GDMA_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_GDMA_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_CDOG_MASK (0x30000U)
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_CDOG_SHIFT (16U)
/*! CDOG - 0x4014C000--0x4014C7FF */
#define AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_CDOG(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_CDOG_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH2_SLAVE_RULE_CDOG_MASK)
/*! @} */

/*! @name SECURITY_CTRL_MEM_RULE - 0x40148000--0x4014BFFF */
/*! @{ */

#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_SECURITY_CTRL_MEM_RULE_RULE3_MASK)
/*! @} */

/*! @name AHB_PERIPH3_SLAVE_RULE - the memory map is 0x40150000--0x40158FFF */
/*! @{ */

#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PQ_COPRO_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PQ_COPRO_RULE0_SHIFT (0U)
/*! PQ_COPRO_RULE0 - 0x40150000--0x40150FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PQ_COPRO_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PQ_COPRO_RULE0_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PQ_COPRO_RULE0_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SECURE_GPIO_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SECURE_GPIO_RULE1_SHIFT (4U)
/*! SECURE_GPIO_RULE1 - 0x40154000--0x40157FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SECURE_GPIO_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SECURE_GPIO_RULE1_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SECURE_GPIO_RULE1_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SDIO_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SDIO_RULE2_SHIFT (8U)
/*! SDIO_RULE2 - 0x40158000--0x40158FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SDIO_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SDIO_RULE2_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_SDIO_RULE2_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_HPU_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_HPU_RULE3_SHIFT (12U)
/*! HPU_RULE3 - 0x40154000--0x40157FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_HPU_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_HPU_RULE3_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_HPU_RULE3_MASK)

#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PKC_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PKC_RULE4_SHIFT (16U)
/*! PKC_RULE4 - 0x40158000--0x40158FFF */
#define AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PKC_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PKC_RULE4_SHIFT)) & AHB_SECURE_CTRL_AHB_PERIPH3_SLAVE_RULE_PKC_RULE4_MASK)
/*! @} */

/*! @name AON_MEM_RULE - 0x4015C000--0x4015FFFF */
/*! @{ */

#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE0_MASK  (0x3U)
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE0(x)    (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AON_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_AON_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE1_MASK  (0x30U)
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE1(x)    (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AON_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_AON_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE2_MASK  (0x300U)
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE2(x)    (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AON_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_AON_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE3_MASK  (0x3000U)
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_AON_MEM_RULE_RULE3(x)    (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_AON_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_AON_MEM_RULE_RULE3_MASK)
/*! @} */

/*! @name WLAN_S0_SLAVE_RULE - 0x41000000-0x4137FFFF, 3.5MB */
/*! @{ */

#define AHB_SECURE_CTRL_WLAN_S0_SLAVE_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_WLAN_S0_SLAVE_RULE_RULE0_SHIFT (0U)
/*! RULE0 - 0x41000000-0x4137FFFF, 3.5MB */
#define AHB_SECURE_CTRL_WLAN_S0_SLAVE_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S0_SLAVE_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_WLAN_S0_SLAVE_RULE_RULE0_MASK)
/*! @} */

/*! @name WLAN_S1_MEM_RULE - 0x41380000-0x413FFFFF, part0..0x41380000-0x413FFFFF, part3 */
/*! @{ */

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_WLAN_S1_MEM_RULE_RULE7_MASK)
/*! @} */

/*! @name BLE_S0_SLAVE_RULE - 0x44000000-0x443BFFFF */
/*! @{ */

#define AHB_SECURE_CTRL_BLE_S0_SLAVE_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_BLE_S0_SLAVE_RULE_RULE0_SHIFT (0U)
/*! RULE0 - 0x44000000-0x443BFFFF */
#define AHB_SECURE_CTRL_BLE_S0_SLAVE_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S0_SLAVE_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_BLE_S0_SLAVE_RULE_RULE0_MASK)
/*! @} */

/*! @name BLE_S1_MEM_RULE - 0x443C0000-0x443FFFFF part0..0x443C0000-0x443FFFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_BLE_S1_MEM_RULE_RULE7_MASK)
/*! @} */

/*! @name SOC_TOP_MEM_RULE - 0x45000000-0x4500FFFF part0..0x45000000-0x4500FFFF part3 */
/*! @{ */

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE0_MASK (0x3U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE0_SHIFT (0U)
/*! RULE0 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE0_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE0_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE1_MASK (0x30U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE1_SHIFT (4U)
/*! RULE1 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE1_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE1_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE2_MASK (0x300U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE2_SHIFT (8U)
/*! RULE2 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE2_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE2_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE3_MASK (0x3000U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE3_SHIFT (12U)
/*! RULE3 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE3_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE3_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE4_MASK (0x30000U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE4_SHIFT (16U)
/*! RULE4 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE4_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE4_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE5_MASK (0x300000U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE5_SHIFT (20U)
/*! RULE5 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE5_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE5_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE6_MASK (0x3000000U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE6_SHIFT (24U)
/*! RULE6 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE6_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE6_MASK)

#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE7_MASK (0x30000000U)
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE7_SHIFT (28U)
/*! RULE7 - secure control rule0. it can be set when check_reg's write_lock is '0' */
#define AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE7_SHIFT)) & AHB_SECURE_CTRL_SOC_TOP_MEM_RULE_RULE7_MASK)
/*! @} */

/*! @name SEC_VIO_ADDR - most recent security violation address for AHB layer n */
/*! @{ */

#define AHB_SECURE_CTRL_SEC_VIO_ADDR_SEC_VIO_ADDR_MASK (0xFFFFFFFFU)
#define AHB_SECURE_CTRL_SEC_VIO_ADDR_SEC_VIO_ADDR_SHIFT (0U)
/*! SEC_VIO_ADDR - security violation address for AHB layer */
#define AHB_SECURE_CTRL_SEC_VIO_ADDR_SEC_VIO_ADDR(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_ADDR_SEC_VIO_ADDR_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_ADDR_SEC_VIO_ADDR_MASK)
/*! @} */

/*! @name SEC_VIO_MISC_INFO - most recent security violation miscellaneous information for AHB layer n */
/*! @{ */

#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_WRITE_MASK (0x1U)
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_WRITE_SHIFT (0U)
/*! SEC_VIO_INFO_WRITE - security violation access read/write indicator, 0: read, 1: write */
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_WRITE(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_WRITE_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_WRITE_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_DATA_ACCESS_MASK (0x2U)
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_DATA_ACCESS_SHIFT (1U)
/*! SEC_VIO_INFO_DATA_ACCESS - security violation access data/code indicator, 0: code, 1 */
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_DATA_ACCESS(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_DATA_ACCESS_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_DATA_ACCESS_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SEC_LEVEL_MASK (0xF0U)
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SEC_LEVEL_SHIFT (4U)
/*! SEC_VIO_INFO_MASTER_SEC_LEVEL - bit [5:4]: master sec level and privilege level bit [7:6]: anti-pol value for master sec level and privilege level */
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SEC_LEVEL(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SEC_LEVEL_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SEC_LEVEL_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_MASK (0xF00U)
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SHIFT (8U)
/*! SEC_VIO_INFO_MASTER - security violation master number */
#define AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_MISC_INFO_SEC_VIO_INFO_MASTER_MASK)
/*! @} */

/*! @name SEC_VIO_INFO_VALID - security violation address/information registers valid flags */
/*! @{ */

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID0_MASK (0x1U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID0_SHIFT (0U)
/*! VIO_INFO_VALID0 - violation information valid flag for AHB layer 0. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID0_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID0_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID1_MASK (0x2U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID1_SHIFT (1U)
/*! VIO_INFO_VALID1 - violation information valid flag for AHB layer 1. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID1_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID1_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID2_MASK (0x4U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID2_SHIFT (2U)
/*! VIO_INFO_VALID2 - violation information valid flag for AHB layer 2. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID2(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID2_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID2_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID3_MASK (0x8U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID3_SHIFT (3U)
/*! VIO_INFO_VALID3 - violation information valid flag for AHB layer 3. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID3(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID3_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID3_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID4_MASK (0x10U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID4_SHIFT (4U)
/*! VIO_INFO_VALID4 - violation information valid flag for AHB layer 4. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID4(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID4_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID4_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID5_MASK (0x20U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID5_SHIFT (5U)
/*! VIO_INFO_VALID5 - violation information valid flag for AHB layer 5. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID5(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID5_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID5_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID6_MASK (0x40U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID6_SHIFT (6U)
/*! VIO_INFO_VALID6 - violation information valid flag for AHB layer 6. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID6(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID6_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID6_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID7_MASK (0x80U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID7_SHIFT (7U)
/*! VIO_INFO_VALID7 - violation information valid flag for AHB layer 7. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID7(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID7_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID7_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID8_MASK (0x100U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID8_SHIFT (8U)
/*! VIO_INFO_VALID8 - violation information valid flag for AHB layer 8. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID8(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID8_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID8_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID9_MASK (0x200U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID9_SHIFT (9U)
/*! VIO_INFO_VALID9 - violation information valid flag for AHB layer 9. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID9(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID9_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID9_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID10_MASK (0x400U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID10_SHIFT (10U)
/*! VIO_INFO_VALID10 - violation information valid flag for AHB layer 10. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID10(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID10_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID10_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID11_MASK (0x800U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID11_SHIFT (11U)
/*! VIO_INFO_VALID11 - violation information valid flag for AHB layer 11. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID11(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID11_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID11_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID12_MASK (0x1000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID12_SHIFT (12U)
/*! VIO_INFO_VALID12 - violation information valid flag for AHB layer 12. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID12(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID12_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID12_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID13_MASK (0x2000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID13_SHIFT (13U)
/*! VIO_INFO_VALID13 - violation information valid flag for AHB layer 13. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID13(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID13_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID13_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID14_MASK (0x4000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID14_SHIFT (14U)
/*! VIO_INFO_VALID14 - violation information valid flag for AHB layer 14. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID14(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID14_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID14_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID15_MASK (0x8000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID15_SHIFT (15U)
/*! VIO_INFO_VALID15 - violation information valid flag for AHB layer 15. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID15(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID15_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID15_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID16_MASK (0x10000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID16_SHIFT (16U)
/*! VIO_INFO_VALID16 - violation information valid flag for AHB layer 16. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID16(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID16_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID16_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID17_MASK (0x20000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID17_SHIFT (17U)
/*! VIO_INFO_VALID17 - violation information valid flag for AHB layer 17. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID17(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID17_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID17_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID18_MASK (0x40000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID18_SHIFT (18U)
/*! VIO_INFO_VALID18 - violation information valid flag for AHB layer 18. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID18(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID18_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID18_MASK)

#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID19_MASK (0x80000U)
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID19_SHIFT (19U)
/*! VIO_INFO_VALID19 - violation information valid flag for AHB layer 19. 0: not valid. 1: valid (violation occurred). Write 1 to clear. */
#define AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID19(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID19_SHIFT)) & AHB_SECURE_CTRL_SEC_VIO_INFO_VALID_VIO_INFO_VALID19_MASK)
/*! @} */

/*! @name SEC_GPIO_MASK0 - Secure GPIO mask for port 0 pins. This register is used to block leakage of Secure interface (GPIOs, I2C, UART configured as secure peripherals) pin states to non-secure world. */
/*! @{ */

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN0_SEC_MASK_MASK (0x1U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN0_SEC_MASK_SHIFT (0U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN0_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN0_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN0_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN1_SEC_MASK_MASK (0x2U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN1_SEC_MASK_SHIFT (1U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN1_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN1_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN1_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN2_SEC_MASK_MASK (0x4U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN2_SEC_MASK_SHIFT (2U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN2_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN2_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN2_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN3_SEC_MASK_MASK (0x8U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN3_SEC_MASK_SHIFT (3U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN3_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN3_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN3_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN4_SEC_MASK_MASK (0x10U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN4_SEC_MASK_SHIFT (4U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN4_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN4_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN4_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN5_SEC_MASK_MASK (0x20U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN5_SEC_MASK_SHIFT (5U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN5_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN5_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN5_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN6_SEC_MASK_MASK (0x40U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN6_SEC_MASK_SHIFT (6U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN6_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN6_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN6_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN7_SEC_MASK_MASK (0x80U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN7_SEC_MASK_SHIFT (7U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN7_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN7_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN7_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN8_SEC_MASK_MASK (0x100U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN8_SEC_MASK_SHIFT (8U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN8_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN8_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN8_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN9_SEC_MASK_MASK (0x200U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN9_SEC_MASK_SHIFT (9U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN9_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN9_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN9_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN10_SEC_MASK_MASK (0x400U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN10_SEC_MASK_SHIFT (10U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN10_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN10_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN10_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN11_SEC_MASK_MASK (0x800U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN11_SEC_MASK_SHIFT (11U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN11_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN11_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN11_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN12_SEC_MASK_MASK (0x1000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN12_SEC_MASK_SHIFT (12U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN12_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN12_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN12_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN13_SEC_MASK_MASK (0x2000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN13_SEC_MASK_SHIFT (13U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN13_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN13_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN13_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN14_SEC_MASK_MASK (0x4000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN14_SEC_MASK_SHIFT (14U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN14_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN14_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN14_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN15_SEC_MASK_MASK (0x8000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN15_SEC_MASK_SHIFT (15U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN15_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN15_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN15_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN16_SEC_MASK_MASK (0x10000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN16_SEC_MASK_SHIFT (16U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN16_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN16_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN16_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN17_SEC_MASK_MASK (0x20000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN17_SEC_MASK_SHIFT (17U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN17_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN17_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN17_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN18_SEC_MASK_MASK (0x40000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN18_SEC_MASK_SHIFT (18U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN18_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN18_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN18_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN19_SEC_MASK_MASK (0x80000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN19_SEC_MASK_SHIFT (19U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN19_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN19_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN19_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN20_SEC_MASK_MASK (0x100000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN20_SEC_MASK_SHIFT (20U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN20_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN20_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN20_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN21_SEC_MASK_MASK (0x200000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN21_SEC_MASK_SHIFT (21U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN21_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN21_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN21_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN22_SEC_MASK_MASK (0x400000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN22_SEC_MASK_SHIFT (22U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN22_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN22_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN22_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN23_SEC_MASK_MASK (0x800000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN23_SEC_MASK_SHIFT (23U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN23_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN23_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN23_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN24_SEC_MASK_MASK (0x1000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN24_SEC_MASK_SHIFT (24U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN24_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN24_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN24_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN25_SEC_MASK_MASK (0x2000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN25_SEC_MASK_SHIFT (25U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN25_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN25_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN25_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN26_SEC_MASK_MASK (0x4000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN26_SEC_MASK_SHIFT (26U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN26_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN26_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN26_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN27_SEC_MASK_MASK (0x8000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN27_SEC_MASK_SHIFT (27U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN27_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN27_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN27_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN28_SEC_MASK_MASK (0x10000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN28_SEC_MASK_SHIFT (28U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN28_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN28_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN28_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN29_SEC_MASK_MASK (0x20000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN29_SEC_MASK_SHIFT (29U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN29_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN29_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN29_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN30_SEC_MASK_MASK (0x40000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN30_SEC_MASK_SHIFT (30U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN30_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN30_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN30_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN31_SEC_MASK_MASK (0x80000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN31_SEC_MASK_SHIFT (31U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN31_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN31_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK0_PIO0_PIN31_SEC_MASK_MASK)
/*! @} */

/*! @name SEC_GPIO_MASK1 - Secure GPIO mask for port 1 pins. */
/*! @{ */

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN0_SEC_MASK_MASK (0x1U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN0_SEC_MASK_SHIFT (0U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN0_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN0_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN0_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN1_SEC_MASK_MASK (0x2U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN1_SEC_MASK_SHIFT (1U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN1_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN1_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN1_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN2_SEC_MASK_MASK (0x4U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN2_SEC_MASK_SHIFT (2U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN2_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN2_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN2_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN3_SEC_MASK_MASK (0x8U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN3_SEC_MASK_SHIFT (3U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN3_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN3_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN3_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN4_SEC_MASK_MASK (0x10U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN4_SEC_MASK_SHIFT (4U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN4_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN4_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN4_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN5_SEC_MASK_MASK (0x20U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN5_SEC_MASK_SHIFT (5U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN5_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN5_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN5_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN6_SEC_MASK_MASK (0x40U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN6_SEC_MASK_SHIFT (6U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN6_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN6_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN6_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN7_SEC_MASK_MASK (0x80U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN7_SEC_MASK_SHIFT (7U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN7_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN7_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN7_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN8_SEC_MASK_MASK (0x100U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN8_SEC_MASK_SHIFT (8U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN8_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN8_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN8_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN9_SEC_MASK_MASK (0x200U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN9_SEC_MASK_SHIFT (9U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN9_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN9_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN9_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN10_SEC_MASK_MASK (0x400U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN10_SEC_MASK_SHIFT (10U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN10_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN10_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN10_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN11_SEC_MASK_MASK (0x800U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN11_SEC_MASK_SHIFT (11U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN11_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN11_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN11_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN12_SEC_MASK_MASK (0x1000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN12_SEC_MASK_SHIFT (12U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN12_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN12_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN12_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN13_SEC_MASK_MASK (0x2000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN13_SEC_MASK_SHIFT (13U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN13_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN13_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN13_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN14_SEC_MASK_MASK (0x4000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN14_SEC_MASK_SHIFT (14U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN14_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN14_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN14_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN15_SEC_MASK_MASK (0x8000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN15_SEC_MASK_SHIFT (15U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN15_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN15_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN15_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN16_SEC_MASK_MASK (0x10000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN16_SEC_MASK_SHIFT (16U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN16_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN16_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN16_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN17_SEC_MASK_MASK (0x20000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN17_SEC_MASK_SHIFT (17U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN17_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN17_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN17_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN18_SEC_MASK_MASK (0x40000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN18_SEC_MASK_SHIFT (18U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN18_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN18_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN18_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN19_SEC_MASK_MASK (0x80000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN19_SEC_MASK_SHIFT (19U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN19_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN19_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN19_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN20_SEC_MASK_MASK (0x100000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN20_SEC_MASK_SHIFT (20U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN20_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN20_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN20_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN21_SEC_MASK_MASK (0x200000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN21_SEC_MASK_SHIFT (21U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN21_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN21_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN21_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN22_SEC_MASK_MASK (0x400000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN22_SEC_MASK_SHIFT (22U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN22_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN22_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN22_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN23_SEC_MASK_MASK (0x800000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN23_SEC_MASK_SHIFT (23U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN23_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN23_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN23_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN24_SEC_MASK_MASK (0x1000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN24_SEC_MASK_SHIFT (24U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN24_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN24_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN24_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN25_SEC_MASK_MASK (0x2000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN25_SEC_MASK_SHIFT (25U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN25_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN25_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN25_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN26_SEC_MASK_MASK (0x4000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN26_SEC_MASK_SHIFT (26U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN26_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN26_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN26_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN27_SEC_MASK_MASK (0x8000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN27_SEC_MASK_SHIFT (27U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN27_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN27_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN27_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN28_SEC_MASK_MASK (0x10000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN28_SEC_MASK_SHIFT (28U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN28_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN28_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN28_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN29_SEC_MASK_MASK (0x20000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN29_SEC_MASK_SHIFT (29U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN29_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN29_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN29_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN30_SEC_MASK_MASK (0x40000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN30_SEC_MASK_SHIFT (30U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN30_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN30_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN30_SEC_MASK_MASK)

#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN31_SEC_MASK_MASK (0x80000000U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN31_SEC_MASK_SHIFT (31U)
#define AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN31_SEC_MASK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN31_SEC_MASK_SHIFT)) & AHB_SECURE_CTRL_SEC_GPIO_MASK1_PIO1_PIN31_SEC_MASK_MASK)
/*! @} */

/*! @name SEC_MASK_LOCK - sec_gp_reg write-lock bits */
/*! @{ */

#define AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG0_LOCK_MASK (0x3U)
#define AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG0_LOCK_SHIFT (0U)
/*! SEC_GP_REG0_LOCK - 2'b10: sec_reg_reg0 can be written. All other values: sec_reg_reg0 can't be written. */
#define AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG0_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG0_LOCK_SHIFT)) & AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG0_LOCK_MASK)

#define AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG1_LOCK_MASK (0xCU)
#define AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG1_LOCK_SHIFT (2U)
/*! SEC_GP_REG1_LOCK - 2'b10: sec_reg_reg1 can be written. All other values: sec_reg_reg1 can't be written. */
#define AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG1_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG1_LOCK_SHIFT)) & AHB_SECURE_CTRL_SEC_MASK_LOCK_SEC_GP_REG1_LOCK_MASK)
/*! @} */

/*! @name MASTER_SEC_LEVEL - master secure level register */
/*! @{ */

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_POWERQUAD_MASK (0x30U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_POWERQUAD_SHIFT (4U)
/*! POWERQUAD - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_POWERQUAD(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_POWERQUAD_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_POWERQUAD_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA0_MASK (0xC0U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA0_SHIFT (6U)
/*! DMA0 - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA0_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA0_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA1_MASK (0x300U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA1_SHIFT (8U)
/*! DMA1 - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA1_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_DMA1_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ELS_MASK (0xC00U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ELS_SHIFT (10U)
/*! ELS - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ELS(x)  (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ELS_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ELS_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_USB_MASK (0x3000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_USB_SHIFT (12U)
/*! USB - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_USB(x)  (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_USB_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_USB_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_PKC_MASK (0xC000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_PKC_SHIFT (14U)
/*! PKC - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_PKC(x)  (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_PKC_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_PKC_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDIO_MASK (0x30000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDIO_SHIFT (16U)
/*! SDIO - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDIO(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDIO_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_SDIO_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ENET_MASK (0xC0000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ENET_SHIFT (18U)
/*! ENET - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ENET(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ENET_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ENET_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_GDMA_MASK (0x300000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_GDMA_SHIFT (20U)
/*! GDMA - master secure level control. */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_GDMA(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_GDMA_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_GDMA_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK_MASK (0xC0000000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK_SHIFT (30U)
/*! MASTER_SEC_LEVEL_LOCK - master_sec_reg write-lock. When 2'b10, this register can be written.
 *    With any other value, this register can't be written.
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_MASTER_SEC_LEVEL_LOCK_MASK)
/*! @} */

/*! @name MASTER_SEC_LEVEL_ANTI_POL - master secure level anti-pole register */
/*! @{ */

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_POWERQUAD_MASK (0x30U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_POWERQUAD_SHIFT (4U)
/*! POWERQUAD - master secure level control anti-pole value (i.e It must be written with the
 *    inverted value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_POWERQUAD(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_POWERQUAD_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_POWERQUAD_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA0_MASK (0xC0U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA0_SHIFT (6U)
/*! DMA0 - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA0(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA0_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA0_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA1_MASK (0x300U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA1_SHIFT (8U)
/*! DMA1 - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA1(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA1_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_DMA1_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ELS_MASK (0xC00U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ELS_SHIFT (10U)
/*! ELS - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ELS(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ELS_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ELS_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_USB_MASK (0x3000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_USB_SHIFT (12U)
/*! USB - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_USB(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_USB_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_USB_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_PKC_MASK (0xC000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_PKC_SHIFT (14U)
/*! PKC - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_PKC(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_PKC_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_PKC_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_SDIO_MASK (0x30000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_SDIO_SHIFT (16U)
/*! SDIO - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_SDIO(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_SDIO_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_SDIO_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ENET_MASK (0xC0000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ENET_SHIFT (18U)
/*! ENET - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ENET(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ENET_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_ENET_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_GDMA_MASK (0x300000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_GDMA_SHIFT (20U)
/*! GDMA - master secure level control anti-pole value (i.e It must be written with the inverted
 *    value of the corresponding field in master_sec_reg).
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_GDMA(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_GDMA_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_GDMA_MASK)

#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_MASTER_SEC_LEVEL_ANTIPOL_LOCK_MASK (0xC0000000U)
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_MASTER_SEC_LEVEL_ANTIPOL_LOCK_SHIFT (30U)
/*! MASTER_SEC_LEVEL_ANTIPOL_LOCK - master_sec_antipol_reg register write-lock. When 2'b10, this
 *    register can be written. With any other value, this register can't be written.
 */
#define AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_MASTER_SEC_LEVEL_ANTIPOL_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_MASTER_SEC_LEVEL_ANTIPOL_LOCK_SHIFT)) & AHB_SECURE_CTRL_MASTER_SEC_LEVEL_ANTI_POL_MASTER_SEC_LEVEL_ANTIPOL_LOCK_MASK)
/*! @} */

/*! @name CM33_LOCK_REG - m33 lock control register */
/*! @{ */

#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_VTOR_MASK (0x3U)
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_VTOR_SHIFT (0U)
/*! LOCK_NS_VTOR - 2'b10: m33 LOCKNSVTOR is 0. All other values: m33 LOCKNSVTOR is 1 */
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_VTOR(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_VTOR_SHIFT)) & AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_VTOR_MASK)

#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_MPU_MASK (0xCU)
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_MPU_SHIFT (2U)
/*! LOCK_NS_MPU - 2'b10:m33 LOCKNSMPU is 0. All other values: m33 LOCKNSMPU is 1 */
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_MPU(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_MPU_SHIFT)) & AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_NS_MPU_MASK)

#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_VTAIRCR_MASK (0x30U)
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_VTAIRCR_SHIFT (4U)
/*! LOCK_S_VTAIRCR - 2'b10:m33 LOCKSVTAURCR is 0. All other values: m33 LOCKSVTAURCR is 1 */
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_VTAIRCR(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_VTAIRCR_SHIFT)) & AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_VTAIRCR_MASK)

#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_MPU_MASK (0xC0U)
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_MPU_SHIFT (6U)
/*! LOCK_S_MPU - 2'b10:m33 LOCKSMPU is 0. All other values: m33 LOCKSMPU is 1 */
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_MPU(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_MPU_SHIFT)) & AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_S_MPU_MASK)

#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_SAU_MASK (0x300U)
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_SAU_SHIFT (8U)
/*! LOCK_SAU - 2'b10:m33 LOCKSAU is 0. All other values: m33 LOCKSAU is 1 */
#define AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_SAU(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_SAU_SHIFT)) & AHB_SECURE_CTRL_CM33_LOCK_REG_LOCK_SAU_MASK)

#define AHB_SECURE_CTRL_CM33_LOCK_REG_M33_LOCK_REG_LOCK_MASK (0xC0000000U)
#define AHB_SECURE_CTRL_CM33_LOCK_REG_M33_LOCK_REG_LOCK_SHIFT (30U)
/*! M33_LOCK_REG_LOCK - 2'b10: this register can be written. All other values: this register can't be written */
#define AHB_SECURE_CTRL_CM33_LOCK_REG_M33_LOCK_REG_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_CM33_LOCK_REG_M33_LOCK_REG_LOCK_SHIFT)) & AHB_SECURE_CTRL_CM33_LOCK_REG_M33_LOCK_REG_LOCK_MASK)
/*! @} */

/*! @name MISC_CTRL_DP_REG - secure control duplicate register */
/*! @{ */

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK_MASK (0x3U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK_SHIFT (0U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_WRITE_LOCK_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_MASK (0xCU)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_SHIFT (2U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_SECURE_CHECKING_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK_MASK (0x30U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK_SHIFT (4U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_S_PRIV_CHECK_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK_MASK (0xC0U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK_SHIFT (6U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_ENABLE_NS_PRIV_CHECK_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_MASK (0x300U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_SHIFT (8U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_VIOLATION_ABORT_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE_MASK (0xC00U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE_SHIFT (10U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_DISABLE_STRICT_MODE_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS_MASK (0xC000U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS_SHIFT (14U)
#define AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_DP_REG_IDAU_ALL_NS_MASK)
/*! @} */

/*! @name MISC_CTRL_REG - secure control register */
/*! @{ */

#define AHB_SECURE_CTRL_MISC_CTRL_REG_WRITE_LOCK_MASK (0x3U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_WRITE_LOCK_SHIFT (0U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_WRITE_LOCK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_WRITE_LOCK_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_WRITE_LOCK_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK (0xCU)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_SHIFT (2U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_SECURE_CHECKING_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK_MASK (0x30U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK_SHIFT (4U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_S_PRIV_CHECK_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK_MASK (0xC0U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK_SHIFT (6U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_ENABLE_NS_PRIV_CHECK_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_MASK (0x300U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_SHIFT (8U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_VIOLATION_ABORT_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_STRICT_MODE_MASK (0xC00U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_STRICT_MODE_SHIFT (10U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_STRICT_MODE(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_STRICT_MODE_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_DISABLE_STRICT_MODE_MASK)

#define AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_MASK (0xC000U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_SHIFT (14U)
#define AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS(x) (((uint32_t)(((uint32_t)(x)) << AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_SHIFT)) & AHB_SECURE_CTRL_MISC_CTRL_REG_IDAU_ALL_NS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group AHB_SECURE_CTRL_Register_Masks */


/*!
 * @}
 */ /* end of group AHB_SECURE_CTRL_Peripheral_Access_Layer */


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


#endif  /* AHB_SECURE_CTRL_H_ */

