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
**         CMSIS Peripheral Access Layer for CIU2
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
 * @file CIU2.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CIU2
 *
 * CMSIS Peripheral Access Layer for CIU2
 */

#if !defined(CIU2_H_)
#define CIU2_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- CIU2 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CIU2_Peripheral_Access_Layer CIU2 Peripheral Access Layer
 * @{
 */

/** CIU2 - Register Layout Typedef */
typedef struct {
  __IO uint32_t CIU2_CLK_ENABLE;                   /**< Clock enable, offset: 0x0 */
  __IO uint32_t CIU2_ECO_0;                        /**< ECO Register 0, offset: 0x4 */
  __IO uint32_t CIU2_ECO_1;                        /**< ECO Register 1, offset: 0x8 */
  __IO uint32_t CIU2_ECO_2;                        /**< ECO Register 2, offset: 0xC */
  __IO uint32_t CIU2_ECO_3;                        /**< ECO Register 3, offset: 0x10 */
  __IO uint32_t CIU2_ECO_4;                        /**< ECO Register 4, offset: 0x14 */
  __IO uint32_t CIU2_ECO_5;                        /**< ECO Register 5, offset: 0x18 */
  __IO uint32_t CIU2_ECO_6;                        /**< ECO Register 6, offset: 0x1C */
  __IO uint32_t CIU2_ECO_7;                        /**< ECO Register 7, offset: 0x20 */
  __IO uint32_t CIU2_ECO_8;                        /**< ECO Register 8, offset: 0x24 */
  __IO uint32_t CIU2_ECO_9;                        /**< ECO Register 9, offset: 0x28 */
  __IO uint32_t CIU2_ECO_10;                       /**< ECO Register 10, offset: 0x2C */
  __IO uint32_t CIU2_ECO_11;                       /**< ECO Register 11, offset: 0x30 */
  __IO uint32_t CIU2_ECO_12;                       /**< ECO Register 12, offset: 0x34 */
  __IO uint32_t CIU2_ECO_13;                       /**< ECO Register 13, offset: 0x38 */
  __IO uint32_t CIU2_ECO_14;                       /**< ECO Register 14, offset: 0x3C */
  __IO uint32_t CIU2_ECO_15;                       /**< ECO Register 15, offset: 0x40 */
       uint8_t RESERVED_0[188];
  __IO uint32_t CIU2_CLK_ENABLE4;                  /**< Clock Enable 4, offset: 0x100 */
  __IO uint32_t CIU2_CLK_ENABLE5;                  /**< Clock Enable 5, offset: 0x104 */
  __IO uint32_t CIU2_CLK_CPU2CLK_CTRL;             /**< CPU2_AHB2 Clock Control, offset: 0x108 */
  __IO uint32_t CIU2_CLK_UARTCLK_CTRL;             /**< UART Clock Control, offset: 0x10C */
  __IO uint32_t CIU2_CLK_LBU2_BTRTU1_CTRL;         /**< LBU2 BT_RTU1 Clock Control, offset: 0x110 */
       uint8_t RESERVED_1[4];
  __IO uint32_t CIU2_CLK_CP15_DIS3;                /**< Clock Auto Shut-off Enable3, offset: 0x118 */
  __IO uint32_t CIU2_RST_SW3;                      /**< Software Module Reset, offset: 0x11C */
  __IO uint32_t CIU2_MEM_WRTC3;                    /**< Memory WRTC Control 3, offset: 0x120 */
  __IO uint32_t CIU2_MEM_WRTC4;                    /**< Memory WRTC Control 4, offset: 0x124 */
  __IO uint32_t CIU2_MEM_PWDN3;                    /**< Memory Power down Control, offset: 0x128 */
  __I  uint32_t CIU2_SOC_AHB2APB_STATUS;           /**< SOC AHB2APB Status, offset: 0x12C */
       uint8_t RESERVED_2[16];
  __IO uint32_t CIU2_BLE_CTRL;                     /**< BLE Control and Status, offset: 0x140 */
  __I  uint32_t CIU2_AHB2_TO_LAST_ADDR;            /**< AHB2 Timeout Last Address, offset: 0x144 */
  __I  uint32_t CIU2_AHB2_TO_CUR_ADDR;             /**< AHB2 Current Timeout Address, offset: 0x148 */
  __IO uint32_t CIU2_AHB2_TO_CTRL;                 /**< AHB2 ARB Control, offset: 0x14C */
  __IO uint32_t CIU2_AHB2_SMU1_ACCESS_ADDR;        /**< AHB2 to SMU1 Accessible Address, offset: 0x150 */
  __IO uint32_t CIU2_AHB2_SMU1_ACCESS_MASK;        /**< AHB2 to SMU1 Accessible Mask, offset: 0x154 */
       uint8_t RESERVED_3[4];
  __IO uint32_t CIU2_CPU2_ICODE_INV_ADDR_CTRL;     /**< CPU2 Icode invalid address access control, offset: 0x15C */
  __I  uint32_t CIU2_CPU2_ICODE_INV_ADDR;          /**< CPU2 Icode invalid address, offset: 0x160 */
  __IO uint32_t CIU2_CPU2_DCODE_INV_ADDR_CTRL;     /**< CPU2 Dcode invalid address access control, offset: 0x164 */
  __I  uint32_t CIU2_CPU2_DCODE_INV_ADDR;          /**< CPU2 Dcode invalid address, offset: 0x168 */
  __IO uint32_t CIU2_CPU_CPU2_CTRL;                /**< CPU2 control register, offset: 0x16C */
  __IO uint32_t CIU2_BRF_CTRL;                     /**< BRF Control and Status, offset: 0x170 */
  __IO uint32_t CIU2_BRF_EXTRA_PORT;               /**< BRF Extra Port Connection, offset: 0x174 */
       uint8_t RESERVED_4[4];
  __IO uint32_t CIU2_BRF_ECO_CTRL;                 /**< BRF ECO Control, offset: 0x17C */
  __IO uint32_t CIU2_BTU_CTRL;                     /**< BTU Control and Status, offset: 0x180 */
  __IO uint32_t CIU2_BT_PS;                        /**< BT Clock Power Save, offset: 0x184 */
  __IO uint32_t CIU2_BT_PS2;                       /**< BT Clock Power Save 2, offset: 0x188 */
  __IO uint32_t CIU2_BT_REF_CTRL;                  /**< BT Ref Control, offset: 0x18C */
       uint8_t RESERVED_5[4];
  __IO uint32_t CIU2_BT_PS3;                       /**< BT Clock Power Save 3, offset: 0x194 */
  __IO uint32_t CIU2_BTU_ECO_CTRL;                 /**< BTU ECO Control, offset: 0x198 */
       uint8_t RESERVED_6[4];
  __IO uint32_t CIU2_INT_MASK;                     /**< CIU2 Interrupt Mask, offset: 0x1A0 */
  __IO uint32_t CIU2_INT_SELECT;                   /**< CIU2 Interrupt Select, offset: 0x1A4 */
  __IO uint32_t CIU2_INT_EVENT_MASK;               /**< CIU2 Interrupt Event Mask, offset: 0x1A8 */
  __I  uint32_t CIU2_INT_STATUS;                   /**< CIU2 Interrupt Status, offset: 0x1AC */
  __IO uint32_t CPU2_ERR_INT_MASK;                 /**< CPU2 ERR Interrupt Mask, offset: 0x1B0 */
  __IO uint32_t CPU2_ERR_INT_SELECT;               /**< CPU2 ERR Interrupt Clear Select, offset: 0x1B4 */
  __IO uint32_t CPU2_ERR_INT_EVENT_MASK;           /**< CPU2 ERR Interrupt Event Mask, offset: 0x1B8 */
  __I  uint32_t CPU2_ERR_INT_STATUS;               /**< CPU2 ERR Interrupt Status, offset: 0x1BC */
       uint8_t RESERVED_7[64];
  __IO uint32_t CIU2_BCA1_CPU2_INT_MASK;           /**< BCA1 to CPU2 Interrupt Mask, offset: 0x200 */
  __IO uint32_t CIU2_BCA1_CPU2_INT_SELECT;         /**< BCA1 to CPU2 Interrupt Select, offset: 0x204 */
  __IO uint32_t CIU2_BCA1_CPU2_INT_EVENT_MASK;     /**< BCA1 to CPU2 Interrupt Event Mask, offset: 0x208 */
  __I  uint32_t CIU2_BCA1_CPU2_INT_STATUS;         /**< BCA1 to CPU2 Interrupt Status, offset: 0x20C */
  __IO uint32_t CIU2_APU_BYPASS1;                  /**< CIU2 APU Bypass Register 1, offset: 0x210 */
  __IO uint32_t CIU2_CPU2_LMU_STA_BYPASS0;         /**< LMU static bank control byapss0 Register for CPU2 mem, offset: 0x214 */
  __IO uint32_t CIU2_CPU2_LMU_STA_BYPASS1;         /**< LMU static bank control byapss1 Register for CPU2, offset: 0x218 */
  __IO uint32_t CIU2_CPU2_LMU_STA_BYPASS2;         /**< LMU static bank byapss2 Register for CPU2, offset: 0x21C */
  __IO uint32_t CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS;  /**< LMU G2Bist control bypass Register for CPU2, offset: 0x220 */
       uint8_t RESERVED_8[8];
  __IO uint32_t CIU2_APU_PWR_CTRL_BYPASS1;         /**< APU power control Bypass Register 1, offset: 0x22C */
  __IO uint32_t CIU2_AHB2AHB_BRIDGE_CTRL;          /**< AHB2AHB Bridge Control Register, offset: 0x230 */
  __IO uint32_t CIU2_CLK_CP15_DIS1;                /**< Clock Auto Shut-off Enable1, offset: 0x234 */
  __IO uint32_t CIU_CLK_RTU_NCO_CTRL;              /**< RTU NCO Clock Control, offset: 0x238 */
  __IO uint32_t CIU_CLK_SOCCLK_CTRL;               /**< SOC Clock Control, offset: 0x23C */
  __IO uint32_t CIU_CLK_SLEEPCLK_CTRL;             /**< Sleep Clock Control, offset: 0x240 */
  __IO uint32_t CIU_CLK_SLEEPCLK_CTRL2;            /**< Sleep Clock Control 2, offset: 0x244 */
       uint8_t RESERVED_9[8];
  __IO uint32_t CIU2_IOMUX_MODE_CTRL;              /**< Test Bus Select, offset: 0x250 */
  __IO uint32_t CIU2_RST_SW2;                      /**< Software Module Reset, offset: 0x254 */
       uint8_t RESERVED_10[8];
  __IO uint32_t CIU2_AHB2_TO_CLEAR;                /**< AHB2 timeout logic clear register, offset: 0x260 */
  __IO uint32_t CIU2_CPU_DYN_CLK_CTRL;             /**< Dynamic CPU Clock Control, offset: 0x264 */
  __I  uint32_t CPU2_DBG_STAT;                     /**< CPU2 debug register, offset: 0x268 */
  __I  uint32_t BTSS_MBIST_STAT;                   /**< offset: 0x26C */
       uint8_t RESERVED_11[4];
  __IO uint32_t CIU2_TEST_MODE;                    /**< ", offset: 0x274 */
       uint8_t RESERVED_12[4];
  __IO uint32_t CIU2_APU_BYPASS2;                  /**< CIU2 APU Bypass Register 2, offset: 0x27C */
  __I  uint32_t CIU2_TST_G2BIST_STATUS;            /**< WL G2BIST Status, offset: 0x280 */
  __IO uint32_t CIU2_LPO_CLK_GEN_CTRL;             /**< BLE LPO CLK GEN Control, offset: 0x284 */
  __I  uint32_t CIU2_LPO_CLK_GEN_STATUS;           /**< BLE LPO CLK GEN Status, offset: 0x288 */
  __IO uint32_t CIU2_LPO_SLP_CLK_GEN_CTRL;         /**< ", offset: 0x28C */
  __IO uint32_t CPU2_INT_CTRL;                     /**< ", offset: 0x290 */
  __I  uint32_t CIU2_BRF_EXTRA_PORT_STATUS;        /**< ", offset: 0x294 */
  __I  uint32_t CIU2_DEBUG;                        /**< ", offset: 0x298 */
       uint8_t RESERVED_13[100];
  __IO uint32_t CIU2_MCI_EXTRA;                    /**< MCI EXTRA Ports, offset: 0x300 */
  __IO uint32_t CIU2_TSTBUS_SEL;                   /**< Test Bus Select, offset: 0x304 */
  __IO uint32_t FFU_CTRL;                          /**< FFU Specific Control Register, offset: 0x308 */
       uint8_t RESERVED_14[116];
  __IO uint32_t BLE_RAACS_CTRL;                    /**< RAACS control registers, offset: 0x380 */
  __IO uint32_t BLE_RAACS_PERFORMANCE_STATISTICS;  /**< RAACS performance statistics counter., offset: 0x384 */
       uint8_t RESERVED_15[1832056];
  __IO uint32_t CIU2_CPU_CPU2_MSG_CTRL;            /**< CPU2 message register, offset: 0x1BF800 */
  __IO uint32_t CIU2_IMU_CPU3_WR_MSG_TO_CPU2;      /**< CPU3 write message to CPU2, offset: 0x1BF804 */
  __I  uint32_t CIU2_IMU_CPU3_RD_MSG_FROM_CPU2;    /**< CPU3 read message from CPU2, offset: 0x1BF808 */
  __I  uint32_t CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS; /**< CPU3 to CPU2 message FIFO status, offset: 0x1BF80C */
  __IO uint32_t CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL;  /**< CPU3 to CPU2 message FIFO control, offset: 0x1BF810 */
  __I  uint32_t CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG; /**< CPU2 last message read (from cpu3), offset: 0x1BF814 */
  __IO uint32_t CIU2_IMU_CPU2_WR_MSG_TO_CPU3;      /**< CPU2 write message to CPU3, offset: 0x1BF818 */
  __I  uint32_t CIU2_IMU_CPU2_RD_MSG_FROM_CPU3;    /**< CPU2 read message from CPU3, offset: 0x1BF81C */
  __I  uint32_t CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS; /**< CPU2 to CPU3 message FIFO status, offset: 0x1BF820 */
  __IO uint32_t CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL;  /**< CPU2 to CPU3 message FIFO control, offset: 0x1BF824 */
  __I  uint32_t CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG; /**< CPU3 last message read (from cpu2), offset: 0x1BF828 */
  __IO uint32_t CIU2_CPU3_WAKEUP_CTRL;             /**< CIU2 register to wakeup CPU3, offset: 0x1BF82C */
  __IO uint32_t CIU2_CPU2_WAKEUP_DONE;             /**< Wakeup done Control Register to CPU3, offset: 0x1BF830 */
  __IO uint32_t CIU2_CPU3_NS_GP_INT;               /**< Non Secure region GP interrupt to CPU3, offset: 0x1BF834 */
  __IO uint32_t CIU2_IMU_ECO_BITS;                 /**< IMU ECO Control, offset: 0x1BF838 */
} CIU2_Type;

/* ----------------------------------------------------------------------------
   -- CIU2 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CIU2_Register_Masks CIU2 Register Masks
 * @{
 */

/*! @name CIU2_CLK_ENABLE - Clock enable */
/*! @{ */

#define CIU2_CIU2_CLK_ENABLE_CIU2_AHB2APB_CLK_EN_MASK (0x40000U)
#define CIU2_CIU2_CLK_ENABLE_CIU2_AHB2APB_CLK_EN_SHIFT (18U)
/*! CIU2_AHB2APB_CLK_EN - Enable AHB2APB HCLK */
#define CIU2_CIU2_CLK_ENABLE_CIU2_AHB2APB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU2_AHB2APB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU2_AHB2APB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU2_MCI_A2A_CLK_EN_MASK (0x80000U)
#define CIU2_CIU2_CLK_ENABLE_CIU2_MCI_A2A_CLK_EN_SHIFT (19U)
/*! CIU2_MCI_A2A_CLK_EN - Enable AHB2 Clock to A2A in MCI */
#define CIU2_CIU2_CLK_ENABLE_CIU2_MCI_A2A_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU2_MCI_A2A_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU2_MCI_A2A_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU2_FP4_SLP_CLK_EN_MASK (0x100000U)
#define CIU2_CIU2_CLK_ENABLE_CIU2_FP4_SLP_CLK_EN_SHIFT (20U)
/*! CIU2_FP4_SLP_CLK_EN - Enable FFU Sleep Clock */
#define CIU2_CIU2_CLK_ENABLE_CIU2_FP4_SLP_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU2_FP4_SLP_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU2_FP4_SLP_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU2_FFU_OSC_CLK_EN_MASK (0x200000U)
#define CIU2_CIU2_CLK_ENABLE_CIU2_FFU_OSC_CLK_EN_SHIFT (21U)
/*! CIU2_FFU_OSC_CLK_EN - Enable FFU Oscillator Clock */
#define CIU2_CIU2_CLK_ENABLE_CIU2_FFU_OSC_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU2_FFU_OSC_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU2_FFU_OSC_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_SOCCIU_A2A_CLK_EN_MASK (0x400000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_SOCCIU_A2A_CLK_EN_SHIFT (22U)
/*! CIU_SOCCIU_A2A_CLK_EN - Enable AHB2 Clock to A2A in SOCCIU */
#define CIU2_CIU2_CLK_ENABLE_CIU_SOCCIU_A2A_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_SOCCIU_A2A_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_SOCCIU_A2A_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_MSC_A2A_CLK_EN_MASK (0x800000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_MSC_A2A_CLK_EN_SHIFT (23U)
/*! CIU_MSC_A2A_CLK_EN - Enable AHB2 Clock to A2A in MSC */
#define CIU2_CIU2_CLK_ENABLE_CIU_MSC_A2A_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_MSC_A2A_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_MSC_A2A_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_DVFS_CLK_EN_MASK (0x1000000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_DVFS_CLK_EN_SHIFT (24U)
/*! CIU_BTAPU_DVFS_CLK_EN - APU DVFS Clock Enable */
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_DVFS_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_DVFS_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_DVFS_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_AHB_CLK_EN_MASK (0x2000000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_AHB_CLK_EN_SHIFT (25U)
/*! CIU_BTAPU_AHB_CLK_EN - APU AHB Clock Enable */
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_AHB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_AHB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_AHB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_SLPCLK_EN_MASK (0x4000000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_SLPCLK_EN_SHIFT (26U)
/*! CIU_BTAPU_SLPCLK_EN - APU Sleep Clock Enable */
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_SLPCLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_SLPCLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_SLPCLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_CAL_CLK_EN_MASK (0x8000000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_CAL_CLK_EN_SHIFT (27U)
/*! CIU_BTAPU_CAL_CLK_EN - APU Calibration Clock Enable */
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_CAL_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_CAL_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_CAL_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_REF_CLK_EN_MASK (0x10000000U)
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_REF_CLK_EN_SHIFT (28U)
/*! CIU_BTAPU_REF_CLK_EN - APU Reference Clock Enable */
#define CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_REF_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_REF_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CIU_BTAPU_REF_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE_AHB2_CLK_ENABLE_MASK (0x20000000U)
#define CIU2_CIU2_CLK_ENABLE_AHB2_CLK_ENABLE_SHIFT (29U)
/*! AHB2_CLK_ENABLE - Clock ahb2_clk enable signal. Ahb2_clk enable. 1: enable, 0: disable */
#define CIU2_CIU2_CLK_ENABLE_AHB2_CLK_ENABLE(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_AHB2_CLK_ENABLE_SHIFT)) & CIU2_CIU2_CLK_ENABLE_AHB2_CLK_ENABLE_MASK)

#define CIU2_CIU2_CLK_ENABLE_CPU1_DIV_CLK_ENABLE_MASK (0x40000000U)
#define CIU2_CIU2_CLK_ENABLE_CPU1_DIV_CLK_ENABLE_SHIFT (30U)
/*! CPU1_DIV_CLK_ENABLE - Clock cpu1_div_clk enable signal. cpu1_div_clk enable. 1: enable, 0: disable */
#define CIU2_CIU2_CLK_ENABLE_CPU1_DIV_CLK_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_CPU1_DIV_CLK_ENABLE_SHIFT)) & CIU2_CIU2_CLK_ENABLE_CPU1_DIV_CLK_ENABLE_MASK)

#define CIU2_CIU2_CLK_ENABLE_SOC_AHB_CLK_SEL_MASK (0x80000000U)
#define CIU2_CIU2_CLK_ENABLE_SOC_AHB_CLK_SEL_SHIFT (31U)
/*! SOC_AHB_CLK_SEL - Clock selection for soc_ahb_clk. 0: AHB2_CLK, 1: CPU1_CLK_DIV */
#define CIU2_CIU2_CLK_ENABLE_SOC_AHB_CLK_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE_SOC_AHB_CLK_SEL_SHIFT)) & CIU2_CIU2_CLK_ENABLE_SOC_AHB_CLK_SEL_MASK)
/*! @} */

/*! @name CIU2_ECO_0 - ECO Register 0 */
/*! @{ */

#define CIU2_CIU2_ECO_0_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_0_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_0_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_0_SPARE_SHIFT)) & CIU2_CIU2_ECO_0_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_1 - ECO Register 1 */
/*! @{ */

#define CIU2_CIU2_ECO_1_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_1_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_1_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_1_SPARE_SHIFT)) & CIU2_CIU2_ECO_1_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_2 - ECO Register 2 */
/*! @{ */

#define CIU2_CIU2_ECO_2_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_2_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_2_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_2_SPARE_SHIFT)) & CIU2_CIU2_ECO_2_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_3 - ECO Register 3 */
/*! @{ */

#define CIU2_CIU2_ECO_3_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_3_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_3_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_3_SPARE_SHIFT)) & CIU2_CIU2_ECO_3_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_4 - ECO Register 4 */
/*! @{ */

#define CIU2_CIU2_ECO_4_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_4_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_4_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_4_SPARE_SHIFT)) & CIU2_CIU2_ECO_4_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_5 - ECO Register 5 */
/*! @{ */

#define CIU2_CIU2_ECO_5_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_5_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_5_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_5_SPARE_SHIFT)) & CIU2_CIU2_ECO_5_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_6 - ECO Register 6 */
/*! @{ */

#define CIU2_CIU2_ECO_6_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_6_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_6_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_6_SPARE_SHIFT)) & CIU2_CIU2_ECO_6_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_7 - ECO Register 7 */
/*! @{ */

#define CIU2_CIU2_ECO_7_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_7_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_7_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_7_SPARE_SHIFT)) & CIU2_CIU2_ECO_7_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_8 - ECO Register 8 */
/*! @{ */

#define CIU2_CIU2_ECO_8_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_8_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_8_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_8_SPARE_SHIFT)) & CIU2_CIU2_ECO_8_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_9 - ECO Register 9 */
/*! @{ */

#define CIU2_CIU2_ECO_9_SPARE_MASK               (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_9_SPARE_SHIFT              (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_9_SPARE(x)                 (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_9_SPARE_SHIFT)) & CIU2_CIU2_ECO_9_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_10 - ECO Register 10 */
/*! @{ */

#define CIU2_CIU2_ECO_10_SPARE_MASK              (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_10_SPARE_SHIFT             (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_10_SPARE(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_10_SPARE_SHIFT)) & CIU2_CIU2_ECO_10_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_11 - ECO Register 11 */
/*! @{ */

#define CIU2_CIU2_ECO_11_SPARE_MASK              (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_11_SPARE_SHIFT             (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_11_SPARE(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_11_SPARE_SHIFT)) & CIU2_CIU2_ECO_11_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_12 - ECO Register 12 */
/*! @{ */

#define CIU2_CIU2_ECO_12_SPARE_MASK              (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_12_SPARE_SHIFT             (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_12_SPARE(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_12_SPARE_SHIFT)) & CIU2_CIU2_ECO_12_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_13 - ECO Register 13 */
/*! @{ */

#define CIU2_CIU2_ECO_13_SPARE_MASK              (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_13_SPARE_SHIFT             (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_13_SPARE(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_13_SPARE_SHIFT)) & CIU2_CIU2_ECO_13_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_14 - ECO Register 14 */
/*! @{ */

#define CIU2_CIU2_ECO_14_SPARE_MASK              (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_14_SPARE_SHIFT             (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_14_SPARE(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_14_SPARE_SHIFT)) & CIU2_CIU2_ECO_14_SPARE_MASK)
/*! @} */

/*! @name CIU2_ECO_15 - ECO Register 15 */
/*! @{ */

#define CIU2_CIU2_ECO_15_SPARE_MASK              (0xFFFFFFFFU)
#define CIU2_CIU2_ECO_15_SPARE_SHIFT             (0U)
/*! SPARE - Eco Reserve Register */
#define CIU2_CIU2_ECO_15_SPARE(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_ECO_15_SPARE_SHIFT)) & CIU2_CIU2_ECO_15_SPARE_MASK)
/*! @} */

/*! @name CIU2_CLK_ENABLE4 - Clock Enable 4 */
/*! @{ */

#define CIU2_CIU2_CLK_ENABLE4_BIST_AHB2_CLK_GATING_EN_MASK (0x1U)
#define CIU2_CIU2_CLK_ENABLE4_BIST_AHB2_CLK_GATING_EN_SHIFT (0U)
/*! BIST_AHB2_CLK_GATING_EN - CPU2 bist Clock for IMEM/DMEM/SMU2/EBRAM/ROM */
#define CIU2_CIU2_CLK_ENABLE4_BIST_AHB2_CLK_GATING_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BIST_AHB2_CLK_GATING_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BIST_AHB2_CLK_GATING_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_ADDR_MASK_DIS_MASK (0x2U)
#define CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_ADDR_MASK_DIS_SHIFT (1U)
/*! BRU_AHB2_ADDR_MASK_DIS - CPU2 ROM Address Mask Selection */
#define CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_ADDR_MASK_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_ADDR_MASK_DIS_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_ADDR_MASK_DIS_MASK)

#define CIU2_CIU2_CLK_ENABLE4_ITCM_AHB2_DYN_CLK_GATING_DIS_MASK (0x4U)
#define CIU2_CIU2_CLK_ENABLE4_ITCM_AHB2_DYN_CLK_GATING_DIS_SHIFT (2U)
/*! ITCM_AHB2_DYN_CLK_GATING_DIS - CPU2 ITCM Dynamic Clock Gating Feature */
#define CIU2_CIU2_CLK_ENABLE4_ITCM_AHB2_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_ITCM_AHB2_DYN_CLK_GATING_DIS_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_ITCM_AHB2_DYN_CLK_GATING_DIS_MASK)

#define CIU2_CIU2_CLK_ENABLE4_DTCM_AHB2_DYN_CLK_GATING_DIS_MASK (0x8U)
#define CIU2_CIU2_CLK_ENABLE4_DTCM_AHB2_DYN_CLK_GATING_DIS_SHIFT (3U)
/*! DTCM_AHB2_DYN_CLK_GATING_DIS - CPU2 DTCM Dynamic Clock Gating Feature */
#define CIU2_CIU2_CLK_ENABLE4_DTCM_AHB2_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_DTCM_AHB2_DYN_CLK_GATING_DIS_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_DTCM_AHB2_DYN_CLK_GATING_DIS_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_DYN_CLK_GATING_DIS_MASK (0x10U)
#define CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_DYN_CLK_GATING_DIS_SHIFT (4U)
/*! BRU_AHB2_DYN_CLK_GATING_DIS - CPU2 ROM Dynamic Clock Gating Feature */
#define CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_DYN_CLK_GATING_DIS_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BRU_AHB2_DYN_CLK_GATING_DIS_MASK)

#define CIU2_CIU2_CLK_ENABLE4_SMU2_DYN_CLK_GATING_DIS_MASK (0x20U)
#define CIU2_CIU2_CLK_ENABLE4_SMU2_DYN_CLK_GATING_DIS_SHIFT (5U)
/*! SMU2_DYN_CLK_GATING_DIS - SMU2 Dynamic Clock Gating Feature */
#define CIU2_CIU2_CLK_ENABLE4_SMU2_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_SMU2_DYN_CLK_GATING_DIS_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_SMU2_DYN_CLK_GATING_DIS_MASK)

#define CIU2_CIU2_CLK_ENABLE4_EBRAM_BIST_CLK_EN_MASK (0x100U)
#define CIU2_CIU2_CLK_ENABLE4_EBRAM_BIST_CLK_EN_SHIFT (8U)
/*! EBRAM_BIST_CLK_EN - EBRAM BIST Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_EBRAM_BIST_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_EBRAM_BIST_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_EBRAM_BIST_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BT_ECLK_EN_MASK    (0x200U)
#define CIU2_CIU2_CLK_ENABLE4_BT_ECLK_EN_SHIFT   (9U)
/*! BT_ECLK_EN - BTU EBC Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BT_ECLK_EN(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BT_ECLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BT_ECLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BT_4MCLK_EN_MASK   (0x400U)
#define CIU2_CIU2_CLK_ENABLE4_BT_4MCLK_EN_SHIFT  (10U)
/*! BT_4MCLK_EN - BTU 4 MHz Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BT_4MCLK_EN(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BT_4MCLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BT_4MCLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BTU_AHB_CLK_EN_MASK (0x2000U)
#define CIU2_CIU2_CLK_ENABLE4_BTU_AHB_CLK_EN_SHIFT (13U)
/*! BTU_AHB_CLK_EN - BTU AHB Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BTU_AHB_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BTU_AHB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BTU_AHB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_SIU_CLK_EN_MASK    (0x4000U)
#define CIU2_CIU2_CLK_ENABLE4_SIU_CLK_EN_SHIFT   (14U)
/*! SIU_CLK_EN - BT SIU (UART) clock enable */
#define CIU2_CIU2_CLK_ENABLE4_SIU_CLK_EN(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_SIU_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_SIU_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_SMU2_AHB_CLK_EN_MASK (0x10000U)
#define CIU2_CIU2_CLK_ENABLE4_SMU2_AHB_CLK_EN_SHIFT (16U)
/*! SMU2_AHB_CLK_EN - SMU2 AHB Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_SMU2_AHB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_SMU2_AHB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_SMU2_AHB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_HPU2_CIU_CLK_EN_MASK (0x80000U)
#define CIU2_CIU2_CLK_ENABLE4_HPU2_CIU_CLK_EN_SHIFT (19U)
/*! HPU2_CIU_CLK_EN - HPU2 CIU Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_HPU2_CIU_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_HPU2_CIU_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_HPU2_CIU_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BLE_AHB_CLK_EN_MASK (0x100000U)
#define CIU2_CIU2_CLK_ENABLE4_BLE_AHB_CLK_EN_SHIFT (20U)
/*! BLE_AHB_CLK_EN - BLE ARM Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BLE_AHB_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BLE_AHB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BLE_AHB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BLE_SYS_CLK_EN_MASK (0x200000U)
#define CIU2_CIU2_CLK_ENABLE4_BLE_SYS_CLK_EN_SHIFT (21U)
/*! BLE_SYS_CLK_EN - BLE SYS Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BLE_SYS_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BLE_SYS_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BLE_SYS_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BLE_AEU_CLK_EN_MASK (0x400000U)
#define CIU2_CIU2_CLK_ENABLE4_BLE_AEU_CLK_EN_SHIFT (22U)
/*! BLE_AEU_CLK_EN - BT/BLE AEU Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BLE_AEU_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BLE_AEU_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BLE_AEU_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BT_16M_CLK_EN_MASK (0x800000U)
#define CIU2_CIU2_CLK_ENABLE4_BT_16M_CLK_EN_SHIFT (23U)
/*! BT_16M_CLK_EN - BT 16MHz Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_BT_16M_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BT_16M_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BT_16M_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_DBUS_CLK_EN_MASK   (0x1000000U)
#define CIU2_CIU2_CLK_ENABLE4_DBUS_CLK_EN_SHIFT  (24U)
/*! DBUS_CLK_EN - BLE DBUS Clock Enable */
#define CIU2_CIU2_CLK_ENABLE4_DBUS_CLK_EN(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_DBUS_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_DBUS_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_SIU_AHB2_CLK_EN_MASK (0x20000000U)
#define CIU2_CIU2_CLK_ENABLE4_SIU_AHB2_CLK_EN_SHIFT (29U)
/*! SIU_AHB2_CLK_EN - BT SIU (UART) AHB clock enable */
#define CIU2_CIU2_CLK_ENABLE4_SIU_AHB2_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_SIU_AHB2_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_SIU_AHB2_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE4_BTRTU1_CLK_EN_MASK (0x40000000U)
#define CIU2_CIU2_CLK_ENABLE4_BTRTU1_CLK_EN_SHIFT (30U)
/*! BTRTU1_CLK_EN - BT RTU1 clock enable */
#define CIU2_CIU2_CLK_ENABLE4_BTRTU1_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE4_BTRTU1_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE4_BTRTU1_CLK_EN_MASK)
/*! @} */

/*! @name CIU2_CLK_ENABLE5 - Clock Enable 5 */
/*! @{ */

#define CIU2_CIU2_CLK_ENABLE5_ITCM_AHB2_CLK_EN_MASK (0x7U)
#define CIU2_CIU2_CLK_ENABLE5_ITCM_AHB2_CLK_EN_SHIFT (0U)
/*! ITCM_AHB2_CLK_EN - Enable CPU2 ITCM Banks 1-2 */
#define CIU2_CIU2_CLK_ENABLE5_ITCM_AHB2_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_ITCM_AHB2_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_ITCM_AHB2_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_CIU2_FFU_AHB_CLK_EN_MASK (0x40U)
#define CIU2_CIU2_CLK_ENABLE5_CIU2_FFU_AHB_CLK_EN_SHIFT (6U)
/*! CIU2_FFU_AHB_CLK_EN - Enable for AHB Clock to FFU */
#define CIU2_CIU2_CLK_ENABLE5_CIU2_FFU_AHB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_CIU2_FFU_AHB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_CIU2_FFU_AHB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_CIU2_REG_CLK_EN_MASK (0x80U)
#define CIU2_CIU2_CLK_ENABLE5_CIU2_REG_CLK_EN_SHIFT (7U)
/*! CIU2_REG_CLK_EN - CIU2 Reg Clock Enable */
#define CIU2_CIU2_CLK_ENABLE5_CIU2_REG_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_CIU2_REG_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_CIU2_REG_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_BR_AHB2_CLK_EN_MASK (0x7FFF00U)
#define CIU2_CIU2_CLK_ENABLE5_BR_AHB2_CLK_EN_SHIFT (8U)
/*! BR_AHB2_CLK_EN - CPU2 BROM AHB Clock Enable */
#define CIU2_CIU2_CLK_ENABLE5_BR_AHB2_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_BR_AHB2_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_BR_AHB2_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_BTU_MCLK_EN_MASK   (0x800000U)
#define CIU2_CIU2_CLK_ENABLE5_BTU_MCLK_EN_SHIFT  (23U)
/*! BTU_MCLK_EN - BTU MCLK Enable */
#define CIU2_CIU2_CLK_ENABLE5_BTU_MCLK_EN(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_BTU_MCLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_BTU_MCLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_SMU2_BANK_CLK_EN_MASK (0x3000000U)
#define CIU2_CIU2_CLK_ENABLE5_SMU2_BANK_CLK_EN_SHIFT (24U)
/*! SMU2_BANK_CLK_EN - SMU2 bank Clock Enable */
#define CIU2_CIU2_CLK_ENABLE5_SMU2_BANK_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_SMU2_BANK_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_SMU2_BANK_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_BTRTU1_AHB_CLK_EN_MASK (0x4000000U)
#define CIU2_CIU2_CLK_ENABLE5_BTRTU1_AHB_CLK_EN_SHIFT (26U)
/*! BTRTU1_AHB_CLK_EN - SW enable for btrtu ahb clock */
#define CIU2_CIU2_CLK_ENABLE5_BTRTU1_AHB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_BTRTU1_AHB_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_BTRTU1_AHB_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_SIF_CLK_SEL_MASK   (0x8000000U)
#define CIU2_CIU2_CLK_ENABLE5_SIF_CLK_SEL_SHIFT  (27U)
/*! SIF_CLK_SEL - SIF Clock Select */
#define CIU2_CIU2_CLK_ENABLE5_SIF_CLK_SEL(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_SIF_CLK_SEL_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_SIF_CLK_SEL_MASK)

#define CIU2_CIU2_CLK_ENABLE5_CPU2_GATEHCLK_EN_MASK (0x10000000U)
#define CIU2_CIU2_CLK_ENABLE5_CPU2_GATEHCLK_EN_SHIFT (28U)
/*! CPU2_GATEHCLK_EN - CPU2gate HCLK Feature */
#define CIU2_CIU2_CLK_ENABLE5_CPU2_GATEHCLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_CPU2_GATEHCLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_CPU2_GATEHCLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_CPU2_FABRIC_CLK_EN_MASK (0x20000000U)
#define CIU2_CIU2_CLK_ENABLE5_CPU2_FABRIC_CLK_EN_SHIFT (29U)
/*! CPU2_FABRIC_CLK_EN - CPU2 Fabric Clock Control Feature */
#define CIU2_CIU2_CLK_ENABLE5_CPU2_FABRIC_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_CPU2_FABRIC_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_CPU2_FABRIC_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_CPU2_MEM_SLV_CLK_EN_MASK (0x40000000U)
#define CIU2_CIU2_CLK_ENABLE5_CPU2_MEM_SLV_CLK_EN_SHIFT (30U)
/*! CPU2_MEM_SLV_CLK_EN - CPU2 Memory Slave Clock Control Feature */
#define CIU2_CIU2_CLK_ENABLE5_CPU2_MEM_SLV_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_CPU2_MEM_SLV_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_CPU2_MEM_SLV_CLK_EN_MASK)

#define CIU2_CIU2_CLK_ENABLE5_SIF_AHB2_CLK_EN_MASK (0x80000000U)
#define CIU2_CIU2_CLK_ENABLE5_SIF_AHB2_CLK_EN_SHIFT (31U)
/*! SIF_AHB2_CLK_EN - SIF ahb2 Clock Enable */
#define CIU2_CIU2_CLK_ENABLE5_SIF_AHB2_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_ENABLE5_SIF_AHB2_CLK_EN_SHIFT)) & CIU2_CIU2_CLK_ENABLE5_SIF_AHB2_CLK_EN_MASK)
/*! @} */

/*! @name CIU2_CLK_CPU2CLK_CTRL - CPU2_AHB2 Clock Control */
/*! @{ */

#define CIU2_CIU2_CLK_CPU2CLK_CTRL_T1_FREQ_SEL_MASK (0xFU)
#define CIU2_CIU2_CLK_CPU2CLK_CTRL_T1_FREQ_SEL_SHIFT (0U)
/*! T1_FREQ_SEL - AHB2 Clock Frequency Select */
#define CIU2_CIU2_CLK_CPU2CLK_CTRL_T1_FREQ_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CPU2CLK_CTRL_T1_FREQ_SEL_SHIFT)) & CIU2_CIU2_CLK_CPU2CLK_CTRL_T1_FREQ_SEL_MASK)
/*! @} */

/*! @name CIU2_CLK_UARTCLK_CTRL - UART Clock Control */
/*! @{ */

#define CIU2_CIU2_CLK_UARTCLK_CTRL_REFCLK_SEL_MASK (0x1U)
#define CIU2_CIU2_CLK_UARTCLK_CTRL_REFCLK_SEL_SHIFT (0U)
/*! REFCLK_SEL - Reference Clock Select */
#define CIU2_CIU2_CLK_UARTCLK_CTRL_REFCLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_UARTCLK_CTRL_REFCLK_SEL_SHIFT)) & CIU2_CIU2_CLK_UARTCLK_CTRL_REFCLK_SEL_MASK)

#define CIU2_CIU2_CLK_UARTCLK_CTRL_NCO_STEP_SIZE_MASK (0xFFFFFF80U)
#define CIU2_CIU2_CLK_UARTCLK_CTRL_NCO_STEP_SIZE_SHIFT (7U)
/*! NCO_STEP_SIZE - Programmable UART Clock Frequency */
#define CIU2_CIU2_CLK_UARTCLK_CTRL_NCO_STEP_SIZE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_UARTCLK_CTRL_NCO_STEP_SIZE_SHIFT)) & CIU2_CIU2_CLK_UARTCLK_CTRL_NCO_STEP_SIZE_MASK)
/*! @} */

/*! @name CIU2_CLK_LBU2_BTRTU1_CTRL - LBU2 BT_RTU1 Clock Control */
/*! @{ */

#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_LBU2_USE_REFCLK_MASK (0x2U)
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_LBU2_USE_REFCLK_SHIFT (1U)
/*! LBU2_USE_REFCLK - Static bit set by FW based on Reference Clock Frequency. If reference clock
 *    frequency is lower and LBU can not support high baud rate of UART, then FW will set
 *    soc_use_ref_mode = 0. This is an indication for Bluetooth subsystem that there is some IP which need PLL
 *    to function which is LBU in this case.
 */
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_LBU2_USE_REFCLK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_LBU2_USE_REFCLK_SHIFT)) & CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_LBU2_USE_REFCLK_MASK)

#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_TIMER1_USE_SLP_CLK_MASK (0x800U)
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_TIMER1_USE_SLP_CLK_SHIFT (11U)
/*! BTRTU1_TIMER1_USE_SLP_CLK - Timer 1 BT_RTU1 Clock */
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_TIMER1_USE_SLP_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_TIMER1_USE_SLP_CLK_SHIFT)) & CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_TIMER1_USE_SLP_CLK_MASK)

#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_USE_REF_CLK_MASK (0x1000U)
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_USE_REF_CLK_SHIFT (12U)
/*! BTRTU1_USE_REF_CLK - Static bit set by FW. If it is required that timers need not be programmed
 *    with dynamic switching of T1/Reference, the BT_RTU1 source clock is set on reference clock so
 *    that the timer are not disturbed.
 */
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_USE_REF_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_USE_REF_CLK_SHIFT)) & CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_USE_REF_CLK_MASK)

#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_DBG_CLK_CTRL_MASK (0x8000U)
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_DBG_CLK_CTRL_SHIFT (15U)
/*! BTRTU1_DBG_CLK_CTRL - BTRTU1 Debug Clock Control Feature */
#define CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_DBG_CLK_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_DBG_CLK_CTRL_SHIFT)) & CIU2_CIU2_CLK_LBU2_BTRTU1_CTRL_BTRTU1_DBG_CLK_CTRL_MASK)
/*! @} */

/*! @name CIU2_CLK_CP15_DIS3 - Clock Auto Shut-off Enable3 */
/*! @{ */

#define CIU2_CIU2_CLK_CP15_DIS3_BR_AHB2_CLK_MASK (0xFFFFU)
#define CIU2_CIU2_CLK_CP15_DIS3_BR_AHB2_CLK_SHIFT (0U)
/*! BR_AHB2_CLK - BRU_AHB2 Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_BR_AHB2_CLK(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_BR_AHB2_CLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_BR_AHB2_CLK_MASK)

#define CIU2_CIU2_CLK_CP15_DIS3_FFU_AHB2_CLK_MASK (0x100000U)
#define CIU2_CIU2_CLK_CP15_DIS3_FFU_AHB2_CLK_SHIFT (20U)
/*! FFU_AHB2_CLK - FFU_AHB Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_FFU_AHB2_CLK(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_FFU_AHB2_CLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_FFU_AHB2_CLK_MASK)

#define CIU2_CIU2_CLK_CP15_DIS3_IMEM_AHB2_CLK_MASK (0x1E00000U)
#define CIU2_CIU2_CLK_CP15_DIS3_IMEM_AHB2_CLK_SHIFT (21U)
/*! IMEM_AHB2_CLK - IMEM_AHB2 Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_IMEM_AHB2_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_IMEM_AHB2_CLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_IMEM_AHB2_CLK_MASK)

#define CIU2_CIU2_CLK_CP15_DIS3_DMEM_AHB2_CLK_MASK (0x6000000U)
#define CIU2_CIU2_CLK_CP15_DIS3_DMEM_AHB2_CLK_SHIFT (25U)
/*! DMEM_AHB2_CLK - DMEM_AHB2 Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_DMEM_AHB2_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_DMEM_AHB2_CLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_DMEM_AHB2_CLK_MASK)

#define CIU2_CIU2_CLK_CP15_DIS3_BLE_HCLK_MASK    (0x10000000U)
#define CIU2_CIU2_CLK_CP15_DIS3_BLE_HCLK_SHIFT   (28U)
/*! BLE_HCLK - BLE ahb Arbiter/Decoder Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_BLE_HCLK(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_BLE_HCLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_BLE_HCLK_MASK)

#define CIU2_CIU2_CLK_CP15_DIS3_BTU_AHB_CLK_MASK (0x40000000U)
#define CIU2_CIU2_CLK_CP15_DIS3_BTU_AHB_CLK_SHIFT (30U)
/*! BTU_AHB_CLK - BTU Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_BTU_AHB_CLK(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_BTU_AHB_CLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_BTU_AHB_CLK_MASK)

#define CIU2_CIU2_CLK_CP15_DIS3_BLE_AHB_CLK_MASK (0x80000000U)
#define CIU2_CIU2_CLK_CP15_DIS3_BLE_AHB_CLK_SHIFT (31U)
/*! BLE_AHB_CLK - BLE Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS3_BLE_AHB_CLK(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS3_BLE_AHB_CLK_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS3_BLE_AHB_CLK_MASK)
/*! @} */

/*! @name CIU2_RST_SW3 - Software Module Reset */
/*! @{ */

#define CIU2_CIU2_RST_SW3_BTU_AHB_CLK__MASK      (0x1U)
#define CIU2_CIU2_RST_SW3_BTU_AHB_CLK__SHIFT     (0U)
/*! BTU_AHB_CLK_ - BTU (ARM_Clk) Soft Reset */
#define CIU2_CIU2_RST_SW3_BTU_AHB_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BTU_AHB_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_BTU_AHB_CLK__MASK)

#define CIU2_CIU2_RST_SW3_BLE_SOC__MASK          (0x2U)
#define CIU2_CIU2_RST_SW3_BLE_SOC__SHIFT         (1U)
/*! BLE_SOC_ - BLE SoC Soft Reset */
#define CIU2_CIU2_RST_SW3_BLE_SOC_(x)            (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BLE_SOC__SHIFT)) & CIU2_CIU2_RST_SW3_BLE_SOC__MASK)

#define CIU2_CIU2_RST_SW3_BT_COMMON__MASK        (0x4U)
#define CIU2_CIU2_RST_SW3_BT_COMMON__SHIFT       (2U)
/*! BT_COMMON_ - BT Common Soft Rest */
#define CIU2_CIU2_RST_SW3_BT_COMMON_(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BT_COMMON__SHIFT)) & CIU2_CIU2_RST_SW3_BT_COMMON__MASK)

#define CIU2_CIU2_RST_SW3_CPU2_CORE__MASK        (0x10U)
#define CIU2_CIU2_RST_SW3_CPU2_CORE__SHIFT       (4U)
/*! CPU2_CORE_ - CPU2 core reset */
#define CIU2_CIU2_RST_SW3_CPU2_CORE_(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_CPU2_CORE__SHIFT)) & CIU2_CIU2_RST_SW3_CPU2_CORE__MASK)

#define CIU2_CIU2_RST_SW3_CPU2_TCM__MASK         (0x20U)
#define CIU2_CIU2_RST_SW3_CPU2_TCM__SHIFT        (5U)
/*! CPU2_TCM_ - CPU2 TCM/DMA/Arbiter reset */
#define CIU2_CIU2_RST_SW3_CPU2_TCM_(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_CPU2_TCM__SHIFT)) & CIU2_CIU2_RST_SW3_CPU2_TCM__MASK)

#define CIU2_CIU2_RST_SW3_ARB_AHB2_CLK__MASK     (0x80U)
#define CIU2_CIU2_RST_SW3_ARB_AHB2_CLK__SHIFT    (7U)
/*! ARB_AHB2_CLK_ - AHB2 Arbiter Soft Reset */
#define CIU2_CIU2_RST_SW3_ARB_AHB2_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_ARB_AHB2_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_ARB_AHB2_CLK__MASK)

#define CIU2_CIU2_RST_SW3_DEC_AHB2_CLK__MASK     (0x100U)
#define CIU2_CIU2_RST_SW3_DEC_AHB2_CLK__SHIFT    (8U)
/*! DEC_AHB2_CLK_ - AHB2 Decoder Mux Soft Reset */
#define CIU2_CIU2_RST_SW3_DEC_AHB2_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_DEC_AHB2_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_DEC_AHB2_CLK__MASK)

#define CIU2_CIU2_RST_SW3_BRU_AHB2_CLK__MASK     (0x200U)
#define CIU2_CIU2_RST_SW3_BRU_AHB2_CLK__SHIFT    (9U)
/*! BRU_AHB2_CLK_ - BRU_AHB2 Soft Reset */
#define CIU2_CIU2_RST_SW3_BRU_AHB2_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BRU_AHB2_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_BRU_AHB2_CLK__MASK)

#define CIU2_CIU2_RST_SW3_BT_UART_N_MASK         (0x400U)
#define CIU2_CIU2_RST_SW3_BT_UART_N_SHIFT        (10U)
/*! BT_UART_N - BT UART soft reset */
#define CIU2_CIU2_RST_SW3_BT_UART_N(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BT_UART_N_SHIFT)) & CIU2_CIU2_RST_SW3_BT_UART_N_MASK)

#define CIU2_CIU2_RST_SW3_SIU_AHB2_CLK_N_MASK    (0x800U)
#define CIU2_CIU2_RST_SW3_SIU_AHB2_CLK_N_SHIFT   (11U)
/*! SIU_AHB2_CLK_N - BT SIU (UART) AHB soft reset */
#define CIU2_CIU2_RST_SW3_SIU_AHB2_CLK_N(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_SIU_AHB2_CLK_N_SHIFT)) & CIU2_CIU2_RST_SW3_SIU_AHB2_CLK_N_MASK)

#define CIU2_CIU2_RST_SW3_SMU2_AHB_CLK__MASK     (0x10000U)
#define CIU2_CIU2_RST_SW3_SMU2_AHB_CLK__SHIFT    (16U)
/*! SMU2_AHB_CLK_ - SMU2 (AHB_Clk) Soft Reset */
#define CIU2_CIU2_RST_SW3_SMU2_AHB_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_SMU2_AHB_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_SMU2_AHB_CLK__MASK)

#define CIU2_CIU2_RST_SW3_SIF__MASK              (0x40000U)
#define CIU2_CIU2_RST_SW3_SIF__SHIFT             (18U)
/*! SIF_ - sif clock Soft Reset */
#define CIU2_CIU2_RST_SW3_SIF_(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_SIF__SHIFT)) & CIU2_CIU2_RST_SW3_SIF__MASK)

#define CIU2_CIU2_RST_SW3_SIF_AHB2_CLK__MASK     (0x80000U)
#define CIU2_CIU2_RST_SW3_SIF_AHB2_CLK__SHIFT    (19U)
/*! SIF_AHB2_CLK_ - sif ahb2 Clock Soft Reset */
#define CIU2_CIU2_RST_SW3_SIF_AHB2_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_SIF_AHB2_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_SIF_AHB2_CLK__MASK)

#define CIU2_CIU2_RST_SW3_HPU2__MASK             (0x100000U)
#define CIU2_CIU2_RST_SW3_HPU2__SHIFT            (20U)
/*! HPU2_ - HPU2 Reset */
#define CIU2_CIU2_RST_SW3_HPU2_(x)               (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_HPU2__SHIFT)) & CIU2_CIU2_RST_SW3_HPU2__MASK)

#define CIU2_CIU2_RST_SW3_CIU2_AHB_CLK__MASK     (0x400000U)
#define CIU2_CIU2_RST_SW3_CIU2_AHB_CLK__SHIFT    (22U)
/*! CIU2_AHB_CLK_ - CIU2 AHB Soft Reset */
#define CIU2_CIU2_RST_SW3_CIU2_AHB_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_CIU2_AHB_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_CIU2_AHB_CLK__MASK)

#define CIU2_CIU2_RST_SW3_BRF_PR__MASK           (0x4000000U)
#define CIU2_CIU2_RST_SW3_BRF_PR__SHIFT          (26U)
/*! BRF_PR_ - BRF_PR Reset */
#define CIU2_CIU2_RST_SW3_BRF_PR_(x)             (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BRF_PR__SHIFT)) & CIU2_CIU2_RST_SW3_BRF_PR__MASK)

#define CIU2_CIU2_RST_SW3_WD2_CHIP_RST_DISABLE_MASK (0x10000000U)
#define CIU2_CIU2_RST_SW3_WD2_CHIP_RST_DISABLE_SHIFT (28U)
/*! WD2_CHIP_RST_DISABLE - WD2 Chip Reset Control */
#define CIU2_CIU2_RST_SW3_WD2_CHIP_RST_DISABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_WD2_CHIP_RST_DISABLE_SHIFT)) & CIU2_CIU2_RST_SW3_WD2_CHIP_RST_DISABLE_MASK)

#define CIU2_CIU2_RST_SW3_WD2_CPU2_RST_DISABLE_MASK (0x20000000U)
#define CIU2_CIU2_RST_SW3_WD2_CPU2_RST_DISABLE_SHIFT (29U)
/*! WD2_CPU2_RST_DISABLE - WD2 CPU2 Reset Control */
#define CIU2_CIU2_RST_SW3_WD2_CPU2_RST_DISABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_WD2_CPU2_RST_DISABLE_SHIFT)) & CIU2_CIU2_RST_SW3_WD2_CPU2_RST_DISABLE_MASK)

#define CIU2_CIU2_RST_SW3_BT_16M_CLK__MASK       (0x40000000U)
#define CIU2_CIU2_RST_SW3_BT_16M_CLK__SHIFT      (30U)
/*! BT_16M_CLK_ - Bt 16M clock reset */
#define CIU2_CIU2_RST_SW3_BT_16M_CLK_(x)         (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW3_BT_16M_CLK__SHIFT)) & CIU2_CIU2_RST_SW3_BT_16M_CLK__MASK)
/*! @} */

/*! @name CIU2_MEM_WRTC3 - Memory WRTC Control 3 */
/*! @{ */

#define CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_MASK     (0x700U)
#define CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_SHIFT    (8U)
/*! BLE_ROM_RTC - BLE ROM RTC */
#define CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_REF_MASK (0x3000U)
#define CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_REF_SHIFT (12U)
/*! BLE_ROM_RTC_REF - BLE ROM RTC_REF */
#define CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_REF(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_REF_SHIFT)) & CIU2_CIU2_MEM_WRTC3_BLE_ROM_RTC_REF_MASK)

#define CIU2_CIU2_MEM_WRTC3_R1P_RTC_MASK         (0xC0000U)
#define CIU2_CIU2_MEM_WRTC3_R1P_RTC_SHIFT        (18U)
/*! R1P_RTC - RTC for small memory for UART in AHB2_TOP */
#define CIU2_CIU2_MEM_WRTC3_R1P_RTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC3_R1P_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC3_R1P_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC3_R1P_WTC_MASK         (0x300000U)
#define CIU2_CIU2_MEM_WRTC3_R1P_WTC_SHIFT        (20U)
/*! R1P_WTC - WTC for small memory for UART in AHB2_TOP */
#define CIU2_CIU2_MEM_WRTC3_R1P_WTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC3_R1P_WTC_SHIFT)) & CIU2_CIU2_MEM_WRTC3_R1P_WTC_MASK)
/*! @} */

/*! @name CIU2_MEM_WRTC4 - Memory WRTC Control 4 */
/*! @{ */

#define CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_RTC_MASK   (0x3U)
#define CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_RTC_SHIFT  (0U)
/*! CPU2_ITCM_RTC - CPU2 ITCM RTC */
#define CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_RTC(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_WTC_MASK   (0xCU)
#define CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_WTC_SHIFT  (2U)
/*! CPU2_ITCM_WTC - CPU2 ITCM WTC */
#define CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_WTC(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_WTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_CPU2_ITCM_WTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_RTC_MASK   (0x30U)
#define CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_RTC_SHIFT  (4U)
/*! CPU2_DTCM_RTC - CPU2 DTCM RTC */
#define CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_RTC(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_WTC_MASK   (0xC0U)
#define CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_WTC_SHIFT  (6U)
/*! CPU2_DTCM_WTC - CPU2 DTCM WTC */
#define CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_WTC(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_WTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_CPU2_DTCM_WTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_SMU2_RTC_MASK        (0x300U)
#define CIU2_CIU2_MEM_WRTC4_SMU2_RTC_SHIFT       (8U)
/*! SMU2_RTC - SMU2 RTC */
#define CIU2_CIU2_MEM_WRTC4_SMU2_RTC(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_SMU2_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_SMU2_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_SMU2_WTC_MASK        (0xC00U)
#define CIU2_CIU2_MEM_WRTC4_SMU2_WTC_SHIFT       (10U)
/*! SMU2_WTC - SMU2 WTC */
#define CIU2_CIU2_MEM_WRTC4_SMU2_WTC(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_SMU2_WTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_SMU2_WTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_MASK    (0x7000U)
#define CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_SHIFT   (12U)
/*! CPU2_BRU_RTC - CPU2 BROM RTC */
#define CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_REF_MASK (0x30000U)
#define CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_REF_SHIFT (16U)
/*! CPU2_BRU_RTC_REF - CPU2 BROM RTC_REF */
#define CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_REF(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_REF_SHIFT)) & CIU2_CIU2_MEM_WRTC4_CPU2_BRU_RTC_REF_MASK)

#define CIU2_CIU2_MEM_WRTC4_BTU_RTC_MASK         (0xC0000U)
#define CIU2_CIU2_MEM_WRTC4_BTU_RTC_SHIFT        (18U)
/*! BTU_RTC - BTU EBRAM RTC */
#define CIU2_CIU2_MEM_WRTC4_BTU_RTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_BTU_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_BTU_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_BTU_WTC_MASK         (0x300000U)
#define CIU2_CIU2_MEM_WRTC4_BTU_WTC_SHIFT        (20U)
/*! BTU_WTC - BTU EBRAM WTC */
#define CIU2_CIU2_MEM_WRTC4_BTU_WTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_BTU_WTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_BTU_WTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_BLE_RTC_MASK         (0xC000000U)
#define CIU2_CIU2_MEM_WRTC4_BLE_RTC_SHIFT        (26U)
/*! BLE_RTC - ble RTC */
#define CIU2_CIU2_MEM_WRTC4_BLE_RTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_BLE_RTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_BLE_RTC_MASK)

#define CIU2_CIU2_MEM_WRTC4_BLE_WTC_MASK         (0x30000000U)
#define CIU2_CIU2_MEM_WRTC4_BLE_WTC_SHIFT        (28U)
/*! BLE_WTC - ble WTC */
#define CIU2_CIU2_MEM_WRTC4_BLE_WTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_WRTC4_BLE_WTC_SHIFT)) & CIU2_CIU2_MEM_WRTC4_BLE_WTC_MASK)
/*! @} */

/*! @name CIU2_MEM_PWDN3 - Memory Power down Control */
/*! @{ */

#define CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_VAL_MASK (0x1U)
#define CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_VAL_SHIFT (0U)
/*! CPU2_BRU_BYPASS_VAL - Firmware Bypass value for CPU2 Boot ROM Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_VAL_MASK (0x2U)
#define CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_VAL_SHIFT (1U)
/*! CPU2_DTCM_BYPASS_VAL - Firmware Bypass value for CPU2 DTCM Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_VAL_MASK (0x4U)
#define CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_VAL_SHIFT (2U)
/*! CPU2_ITCM_BYPASS_VAL - Firmware Bypass value for CPU2 ITCM Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_VAL_MASK (0x10U)
#define CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_VAL_SHIFT (4U)
/*! SMU2_BYPASS_VAL - Firmware Bypass value for SMU2 Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_VAL(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_VAL_MASK  (0x20U)
#define CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_VAL_SHIFT (5U)
/*! SIU_BYPASS_VAL - Firmware Bypass value for UART Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_VAL(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_VAL_MASK  (0x40U)
#define CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_VAL_SHIFT (6U)
/*! BTU_BYPASS_VAL - Firmware Bypass value for BTU Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_VAL(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_VAL_MASK (0x200U)
#define CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_VAL_SHIFT (9U)
/*! BT_ADMA_BYPASS_VAL - Firmware Bypass value for BT ADMA Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_VAL_SHIFT)) & CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_VAL_MASK)

#define CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_EN_MASK (0x10000U)
#define CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_EN_SHIFT (16U)
/*! CPU2_BRU_BYPASS_EN - Firmware Bypass Enable for CPU2 Boot ROM Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_CPU2_BRU_BYPASS_EN_MASK)

#define CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_EN_MASK (0x20000U)
#define CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_EN_SHIFT (17U)
/*! CPU2_DTCM_BYPASS_EN - Firmware Bypass Enable for CPU2 DTCM Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_CPU2_DTCM_BYPASS_EN_MASK)

#define CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_EN_MASK (0x40000U)
#define CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_EN_SHIFT (18U)
/*! CPU2_ITCM_BYPASS_EN - Firmware Bypass Enable for CPU2 ITCM Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_CPU2_ITCM_BYPASS_EN_MASK)

#define CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_EN_MASK  (0x100000U)
#define CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_EN_SHIFT (20U)
/*! SMU2_BYPASS_EN - Firmware Bypass Enable for SMU2 Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_EN(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_SMU2_BYPASS_EN_MASK)

#define CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_EN_MASK   (0x200000U)
#define CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_EN_SHIFT  (21U)
/*! SIU_BYPASS_EN - Firmware Bypass Enable for UART Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_EN(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_SIU_BYPASS_EN_MASK)

#define CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_EN_MASK   (0x400000U)
#define CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_EN_SHIFT  (22U)
/*! BTU_BYPASS_EN - Firmware Bypass Enable for BTU Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_EN(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_BTU_BYPASS_EN_MASK)

#define CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_EN_MASK (0x2000000U)
#define CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_EN_SHIFT (25U)
/*! BT_ADMA_BYPASS_EN - Firmware Bypass Enable for BT ADMA Memories Power Down */
#define CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_EN_SHIFT)) & CIU2_CIU2_MEM_PWDN3_BT_ADMA_BYPASS_EN_MASK)
/*! @} */

/*! @name CIU2_SOC_AHB2APB_STATUS - SOC AHB2APB Status */
/*! @{ */

#define CIU2_CIU2_SOC_AHB2APB_STATUS_CMD_FIFO_AFULL_MASK (0x1U)
#define CIU2_CIU2_SOC_AHB2APB_STATUS_CMD_FIFO_AFULL_SHIFT (0U)
/*! CMD_FIFO_AFULL - soc_io_top ahb2apb command fifo almost full status */
#define CIU2_CIU2_SOC_AHB2APB_STATUS_CMD_FIFO_AFULL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_SOC_AHB2APB_STATUS_CMD_FIFO_AFULL_SHIFT)) & CIU2_CIU2_SOC_AHB2APB_STATUS_CMD_FIFO_AFULL_MASK)
/*! @} */

/*! @name CIU2_BLE_CTRL - BLE Control and Status */
/*! @{ */

#define CIU2_CIU2_BLE_CTRL_BT_AES_CLK_FREQ_SEL_MASK (0x100U)
#define CIU2_CIU2_BLE_CTRL_BT_AES_CLK_FREQ_SEL_SHIFT (8U)
/*! BT_AES_CLK_FREQ_SEL - btu_aes_clk Frequency Select */
#define CIU2_CIU2_BLE_CTRL_BT_AES_CLK_FREQ_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BLE_CTRL_BT_AES_CLK_FREQ_SEL_SHIFT)) & CIU2_CIU2_BLE_CTRL_BT_AES_CLK_FREQ_SEL_MASK)
/*! @} */

/*! @name CIU2_AHB2_TO_LAST_ADDR - AHB2 Timeout Last Address */
/*! @{ */

#define CIU2_CIU2_AHB2_TO_LAST_ADDR_ADDRESS_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_AHB2_TO_LAST_ADDR_ADDRESS_SHIFT (0U)
/*! ADDRESS - Last AHB2 Address Right Before the Current Timeout */
#define CIU2_CIU2_AHB2_TO_LAST_ADDR_ADDRESS(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_LAST_ADDR_ADDRESS_SHIFT)) & CIU2_CIU2_AHB2_TO_LAST_ADDR_ADDRESS_MASK)
/*! @} */

/*! @name CIU2_AHB2_TO_CUR_ADDR - AHB2 Current Timeout Address */
/*! @{ */

#define CIU2_CIU2_AHB2_TO_CUR_ADDR_ADDRESS_MASK  (0xFFFFFFFFU)
#define CIU2_CIU2_AHB2_TO_CUR_ADDR_ADDRESS_SHIFT (0U)
/*! ADDRESS - Current_TO_Addr */
#define CIU2_CIU2_AHB2_TO_CUR_ADDR_ADDRESS(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CUR_ADDR_ADDRESS_SHIFT)) & CIU2_CIU2_AHB2_TO_CUR_ADDR_ADDRESS_MASK)
/*! @} */

/*! @name CIU2_AHB2_TO_CTRL - AHB2 ARB Control */
/*! @{ */

#define CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_SLAVE_ID_MASK (0xFU)
#define CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_SLAVE_ID_SHIFT (0U)
/*! CURRENT_TO_SLAVE_ID - Current_TO_Slave_ID */
#define CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_SLAVE_ID_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_SLAVE_ID_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_SLAVE_ID_MASK (0xF0U)
#define CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_SLAVE_ID_SHIFT (4U)
/*! LAST_TO_SLAVE_ID - Last_TO_Slave_ID */
#define CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_SLAVE_ID_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_SLAVE_ID_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_MASTER_ID_MASK (0xF00U)
#define CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_MASTER_ID_SHIFT (8U)
/*! CURRENT_TO_MASTER_ID - AHB2 Current_TO_Master_ID */
#define CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_MASTER_ID_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_CURRENT_TO_MASTER_ID_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_MASTER_ID_MASK (0xF000U)
#define CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_MASTER_ID_SHIFT (12U)
/*! LAST_TO_MASTER_ID - AHB2 Last_TO_Master_ID */
#define CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_MASTER_ID_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_LAST_TO_MASTER_ID_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_SMU1_MEM_PROT_DIS_MASK (0x10000U)
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_SMU1_MEM_PROT_DIS_SHIFT (16U)
/*! AHB2_SMU1_MEM_PROT_DIS - Disable SMU1 Memory Protection from AHB2 side */
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_SMU1_MEM_PROT_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_AHB2_SMU1_MEM_PROT_DIS_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_AHB2_SMU1_MEM_PROT_DIS_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_IMEM_PROT_DIS_MASK (0x20000U)
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_IMEM_PROT_DIS_SHIFT (17U)
/*! AHB2_CPU2_IMEM_PROT_DIS - 1 = Disable CPU2 Imem Memory Protection from AHB2 side and allow AHB2 to read/write Imem */
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_IMEM_PROT_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_IMEM_PROT_DIS_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_IMEM_PROT_DIS_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_DMEM_PROT_DIS_MASK (0x40000U)
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_DMEM_PROT_DIS_SHIFT (18U)
/*! AHB2_CPU2_DMEM_PROT_DIS - 1 = Disable CPU2 Dmem Memory Protection from AHB2 side and allow AHB2 to read/write Dmem */
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_DMEM_PROT_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_DMEM_PROT_DIS_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_AHB2_CPU2_DMEM_PROT_DIS_MASK)

#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_TIMEOUT_MODE_MASK (0xC0000000U)
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_TIMEOUT_MODE_SHIFT (30U)
/*! AHB2_TIMEOUT_MODE - AHB2_TimeoutMode[1:0] */
#define CIU2_CIU2_AHB2_TO_CTRL_AHB2_TIMEOUT_MODE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CTRL_AHB2_TIMEOUT_MODE_SHIFT)) & CIU2_CIU2_AHB2_TO_CTRL_AHB2_TIMEOUT_MODE_MASK)
/*! @} */

/*! @name CIU2_AHB2_SMU1_ACCESS_ADDR - AHB2 to SMU1 Accessible Address */
/*! @{ */

#define CIU2_CIU2_AHB2_SMU1_ACCESS_ADDR_AHB2_SMU1_ACCESS_ADDR_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_AHB2_SMU1_ACCESS_ADDR_AHB2_SMU1_ACCESS_ADDR_SHIFT (0U)
/*! AHB2_SMU1_ACCESS_ADDR - SMU1 Accessible Memory Address from AHB2 side */
#define CIU2_CIU2_AHB2_SMU1_ACCESS_ADDR_AHB2_SMU1_ACCESS_ADDR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_SMU1_ACCESS_ADDR_AHB2_SMU1_ACCESS_ADDR_SHIFT)) & CIU2_CIU2_AHB2_SMU1_ACCESS_ADDR_AHB2_SMU1_ACCESS_ADDR_MASK)
/*! @} */

/*! @name CIU2_AHB2_SMU1_ACCESS_MASK - AHB2 to SMU1 Accessible Mask */
/*! @{ */

#define CIU2_CIU2_AHB2_SMU1_ACCESS_MASK_AHB2_SMU1_ACCESS_MASK_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_AHB2_SMU1_ACCESS_MASK_AHB2_SMU1_ACCESS_MASK_SHIFT (0U)
/*! AHB2_SMU1_ACCESS_MASK - SMU1 Accessible Memory Mask from AHB2 side */
#define CIU2_CIU2_AHB2_SMU1_ACCESS_MASK_AHB2_SMU1_ACCESS_MASK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_SMU1_ACCESS_MASK_AHB2_SMU1_ACCESS_MASK_SHIFT)) & CIU2_CIU2_AHB2_SMU1_ACCESS_MASK_AHB2_SMU1_ACCESS_MASK_MASK)
/*! @} */

/*! @name CIU2_CPU2_ICODE_INV_ADDR_CTRL - CPU2 Icode invalid address access control */
/*! @{ */

#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK (0xFU)
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT (0U)
/*! LAST2_INV_ADDR_SLAVE_ID - Last2_inv_addr_Slave_ID */
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT)) & CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK)

#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK (0xF0U)
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT (4U)
/*! LAST_INV_ADDR_SLAVE_ID - Last_inv_addr_Slave_ID */
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT)) & CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK)

#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK (0xF00U)
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT (8U)
/*! CUR_INV_ADDR_SLAVE_ID - Cur_inv_addr_Slave_ID */
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT)) & CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK)

#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_MASK (0xC0000000U)
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_SHIFT (30U)
/*! HADDR_ICOD_SEL - There are 3 haddr which can be observed by selecting this: */
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_SHIFT)) & CIU2_CIU2_CPU2_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_MASK)
/*! @} */

/*! @name CIU2_CPU2_ICODE_INV_ADDR - CPU2 Icode invalid address */
/*! @{ */

#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_HADDR_INV_ADDR_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_HADDR_INV_ADDR_SHIFT (0U)
/*! HADDR_INV_ADDR - based on CIU_CPU2_ICODE_INV_ADDR_CTRL[31:30], the address status is observed in this register */
#define CIU2_CIU2_CPU2_ICODE_INV_ADDR_HADDR_INV_ADDR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_ICODE_INV_ADDR_HADDR_INV_ADDR_SHIFT)) & CIU2_CIU2_CPU2_ICODE_INV_ADDR_HADDR_INV_ADDR_MASK)
/*! @} */

/*! @name CIU2_CPU2_DCODE_INV_ADDR_CTRL - CPU2 Dcode invalid address access control */
/*! @{ */

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK (0xFU)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT (0U)
/*! LAST2_INV_ADDR_SLAVE_ID - Last2_inv_addr_Slave_ID */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK)

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK (0xF0U)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT (4U)
/*! LAST_INV_ADDR_SLAVE_ID - Last_inv_addr_Slave_ID */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK)

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK (0xF00U)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT (8U)
/*! CUR_INV_ADDR_SLAVE_ID - Cur_inv_addr_Slave_ID */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK)

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_MASK (0xF000U)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_SHIFT (12U)
/*! LAST2_INV_ADDR_MASTER_ID - Last2_inv_addr_master_ID */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_MASK)

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_MASK (0xF0000U)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_SHIFT (16U)
/*! LAST_INV_ADDR_MASTER_ID - Last_inv_addr_master_ID */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_MASK)

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_MASK (0xF00000U)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_SHIFT (20U)
/*! CUR_INV_ADDR_MASTER_ID - Cur_inv_addr_master_ID */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_MASK)

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_MASK (0xC0000000U)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_SHIFT (30U)
/*! HADDR_ICOD_SEL - There are 3 haddr which can be observed by selecting this: */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_HADDR_ICOD_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_MASK)
/*! @} */

/*! @name CIU2_CPU2_DCODE_INV_ADDR - CPU2 Dcode invalid address */
/*! @{ */

#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_HADDR_INV_ADDR_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_HADDR_INV_ADDR_SHIFT (0U)
/*! HADDR_INV_ADDR - based on CIU_CPU2_DCODE_INV_ADDR_CTRL[31:30], the address status is observed in this register */
#define CIU2_CIU2_CPU2_DCODE_INV_ADDR_HADDR_INV_ADDR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_DCODE_INV_ADDR_HADDR_INV_ADDR_SHIFT)) & CIU2_CIU2_CPU2_DCODE_INV_ADDR_HADDR_INV_ADDR_MASK)
/*! @} */

/*! @name CIU2_CPU_CPU2_CTRL - CPU2 control register */
/*! @{ */

#define CIU2_CIU2_CPU_CPU2_CTRL_VINITHI_MASK     (0x1U)
#define CIU2_CIU2_CPU_CPU2_CTRL_VINITHI_SHIFT    (0U)
/*! VINITHI - Boot Address Control */
#define CIU2_CIU2_CPU_CPU2_CTRL_VINITHI(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_VINITHI_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_VINITHI_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_JTAG_CHAIN_BYPASS_MASK (0x4U)
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_JTAG_CHAIN_BYPASS_SHIFT (2U)
/*! CPU2_JTAG_CHAIN_BYPASS - CPU2 JTAG Chain Bypass Control */
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_JTAG_CHAIN_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_CPU2_JTAG_CHAIN_BYPASS_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_CPU2_JTAG_CHAIN_BYPASS_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_IMEM_MUX_EN_MASK (0x10U)
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_IMEM_MUX_EN_SHIFT (4U)
/*! CPU2_BOOT_IMEM_MUX_EN - CPU2 Boot IMEM mux Enable */
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_IMEM_MUX_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_IMEM_MUX_EN_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_IMEM_MUX_EN_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_DMEM_MUX_EN_MASK (0x20U)
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_DMEM_MUX_EN_SHIFT (5U)
/*! CPU2_BOOT_DMEM_MUX_EN - CPU2 DMEM mux Enable */
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_DMEM_MUX_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_DMEM_MUX_EN_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_CPU2_BOOT_DMEM_MUX_EN_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_DBG_CTRL_MASK (0xFFF0000U)
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_DBG_CTRL_SHIFT (16U)
/*! CPU2_DBG_CTRL - cpu2 debug control */
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU2_DBG_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_CPU2_DBG_CTRL_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_CPU2_DBG_CTRL_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_MCI_AHB2_MSG_SCHEME_MASK (0x10000000U)
#define CIU2_CIU2_CPU_CPU2_CTRL_MCI_AHB2_MSG_SCHEME_SHIFT (28U)
/*! MCI_AHB2_MSG_SCHEME - IMU Scheme Select for Communication between AHB2 & MCI */
#define CIU2_CIU2_CPU_CPU2_CTRL_MCI_AHB2_MSG_SCHEME(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_MCI_AHB2_MSG_SCHEME_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_MCI_AHB2_MSG_SCHEME_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_CPU3_RESET_INT_MASK (0x20000000U)
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU3_RESET_INT_SHIFT (29U)
/*! CPU3_RESET_INT - CPU2 fw resets cpu3(or cpu3 fw resets CPU2 if this register is used by cpu3) */
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU3_RESET_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_CPU3_RESET_INT_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_CPU3_RESET_INT_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_DSR_WKUP_IN_USE_MASK (0x40000000U)
#define CIU2_CIU2_CPU_CPU2_CTRL_DSR_WKUP_IN_USE_SHIFT (30U)
/*! DSR_WKUP_IN_USE - dsr wkup when dsr_wkup_in_use = 1'b1 */
#define CIU2_CIU2_CPU_CPU2_CTRL_DSR_WKUP_IN_USE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_DSR_WKUP_IN_USE_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_DSR_WKUP_IN_USE_MASK)

#define CIU2_CIU2_CPU_CPU2_CTRL_CPU1_RESET_INT_MASK (0x80000000U)
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU1_RESET_INT_SHIFT (31U)
/*! CPU1_RESET_INT - CPU2 fw resets cpu1( or cpu3 fw resets cpu1 if this register is used by cpu3) */
#define CIU2_CIU2_CPU_CPU2_CTRL_CPU1_RESET_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_CTRL_CPU1_RESET_INT_SHIFT)) & CIU2_CIU2_CPU_CPU2_CTRL_CPU1_RESET_INT_MASK)
/*! @} */

/*! @name CIU2_BRF_CTRL - BRF Control and Status */
/*! @{ */

#define CIU2_CIU2_BRF_CTRL_AHB_SLV_BRF_SER_EN_MASK (0x1U)
#define CIU2_CIU2_BRF_CTRL_AHB_SLV_BRF_SER_EN_SHIFT (0U)
/*! AHB_SLV_BRF_SER_EN - When set to 1, BRF serial interface will be accessed thru AHB slave memory mapped from 0xA800A000 to 0xA8011FFF */
#define CIU2_CIU2_BRF_CTRL_AHB_SLV_BRF_SER_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_CTRL_AHB_SLV_BRF_SER_EN_SHIFT)) & CIU2_CIU2_BRF_CTRL_AHB_SLV_BRF_SER_EN_MASK)

#define CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_EN_MASK (0x100U)
#define CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_EN_SHIFT (8U)
/*! CIU_BRF_REF1X_CLK_CTRL_BYPASS_EN - BRF REF1X Clock Control Bypass Enable */
#define CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_EN_SHIFT)) & CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_EN_MASK)

#define CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_VAL_MASK (0x200U)
#define CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_VAL_SHIFT (9U)
/*! CIU_BRF_REF1X_CLK_CTRL_BYPASS_VAL - 1. brf ref clk 1x is enabled */
#define CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_VAL_SHIFT)) & CIU2_CIU2_BRF_CTRL_CIU_BRF_REF1X_CLK_CTRL_BYPASS_VAL_MASK)

#define CIU2_CIU2_BRF_CTRL_BRF_SQU_DUMP_EN_MASK  (0x400U)
#define CIU2_CIU2_BRF_CTRL_BRF_SQU_DUMP_EN_SHIFT (10U)
/*! BRF_SQU_DUMP_EN - Enable SQU data dump from BRF */
#define CIU2_CIU2_BRF_CTRL_BRF_SQU_DUMP_EN(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_CTRL_BRF_SQU_DUMP_EN_SHIFT)) & CIU2_CIU2_BRF_CTRL_BRF_SQU_DUMP_EN_MASK)

#define CIU2_CIU2_BRF_CTRL_FFU_USE_BRF_RX_PATH_MASK (0x800U)
#define CIU2_CIU2_BRF_CTRL_FFU_USE_BRF_RX_PATH_SHIFT (11U)
/*! FFU_USE_BRF_RX_PATH - This bit is connected to FRF_15P4_USE_BRF_RX_PATH input of BRF as recommended by Sridhar. */
#define CIU2_CIU2_BRF_CTRL_FFU_USE_BRF_RX_PATH(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_CTRL_FFU_USE_BRF_RX_PATH_SHIFT)) & CIU2_CIU2_BRF_CTRL_FFU_USE_BRF_RX_PATH_MASK)

#define CIU2_CIU2_BRF_CTRL_BRF_CHIP_RDY_MASK     (0x80000000U)
#define CIU2_CIU2_BRF_CTRL_BRF_CHIP_RDY_SHIFT    (31U)
/*! BRF_CHIP_RDY - BRF Chip_Rdy Status */
#define CIU2_CIU2_BRF_CTRL_BRF_CHIP_RDY(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_CTRL_BRF_CHIP_RDY_SHIFT)) & CIU2_CIU2_BRF_CTRL_BRF_CHIP_RDY_MASK)
/*! @} */

/*! @name CIU2_BRF_EXTRA_PORT - BRF Extra Port Connection */
/*! @{ */

#define CIU2_CIU2_BRF_EXTRA_PORT_SOC_BRF_EXTRA_MASK (0xFU)
#define CIU2_CIU2_BRF_EXTRA_PORT_SOC_BRF_EXTRA_SHIFT (0U)
/*! SOC_BRF_EXTRA - SOC_BRF_EXTRA[3:0] */
#define CIU2_CIU2_BRF_EXTRA_PORT_SOC_BRF_EXTRA(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_EXTRA_PORT_SOC_BRF_EXTRA_SHIFT)) & CIU2_CIU2_BRF_EXTRA_PORT_SOC_BRF_EXTRA_MASK)
/*! @} */

/*! @name CIU2_BRF_ECO_CTRL - BRF ECO Control */
/*! @{ */

#define CIU2_CIU2_BRF_ECO_CTRL_ECO_BITS_MASK     (0xFFFFFFFFU)
#define CIU2_CIU2_BRF_ECO_CTRL_ECO_BITS_SHIFT    (0U)
/*! ECO_BITS - Reserved */
#define CIU2_CIU2_BRF_ECO_CTRL_ECO_BITS(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_ECO_CTRL_ECO_BITS_SHIFT)) & CIU2_CIU2_BRF_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU2_BTU_CTRL - BTU Control and Status */
/*! @{ */

#define CIU2_CIU2_BTU_CTRL_BTU_CIPHER_EN_MASK    (0x1U)
#define CIU2_CIU2_BTU_CTRL_BTU_CIPHER_EN_SHIFT   (0U)
/*! BTU_CIPHER_EN - Bluetooth Cipher Logic */
#define CIU2_CIU2_BTU_CTRL_BTU_CIPHER_EN(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BTU_CTRL_BTU_CIPHER_EN_SHIFT)) & CIU2_CIU2_BTU_CTRL_BTU_CIPHER_EN_MASK)

#define CIU2_CIU2_BTU_CTRL_DBUS_HIGH_SPEED_SEL_MASK (0x2U)
#define CIU2_CIU2_BTU_CTRL_DBUS_HIGH_SPEED_SEL_SHIFT (1U)
/*! DBUS_HIGH_SPEED_SEL - Dbus High Speed Select Signal for Greater than 4 MHz */
#define CIU2_CIU2_BTU_CTRL_DBUS_HIGH_SPEED_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BTU_CTRL_DBUS_HIGH_SPEED_SEL_SHIFT)) & CIU2_CIU2_BTU_CTRL_DBUS_HIGH_SPEED_SEL_MASK)

#define CIU2_CIU2_BTU_CTRL_BT_CLK_SEL_MASK       (0xCU)
#define CIU2_CIU2_BTU_CTRL_BT_CLK_SEL_SHIFT      (2U)
/*! BT_CLK_SEL - Bluetooth sys Clock Select */
#define CIU2_CIU2_BTU_CTRL_BT_CLK_SEL(x)         (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BTU_CTRL_BT_CLK_SEL_SHIFT)) & CIU2_CIU2_BTU_CTRL_BT_CLK_SEL_MASK)

#define CIU2_CIU2_BTU_CTRL_BT_IP_SER_SEL_MASK    (0x700U)
#define CIU2_CIU2_BTU_CTRL_BT_IP_SER_SEL_SHIFT   (8U)
/*! BT_IP_SER_SEL - bt_ip_ser_sel */
#define CIU2_CIU2_BTU_CTRL_BT_IP_SER_SEL(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BTU_CTRL_BT_IP_SER_SEL_SHIFT)) & CIU2_CIU2_BTU_CTRL_BT_IP_SER_SEL_MASK)

#define CIU2_CIU2_BTU_CTRL_BTU_MC_WAKEUP_MASK    (0x80000000U)
#define CIU2_CIU2_BTU_CTRL_BTU_MC_WAKEUP_SHIFT   (31U)
/*! BTU_MC_WAKEUP - BTU MC_Wakeup Status */
#define CIU2_CIU2_BTU_CTRL_BTU_MC_WAKEUP(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BTU_CTRL_BTU_MC_WAKEUP_SHIFT)) & CIU2_CIU2_BTU_CTRL_BTU_MC_WAKEUP_MASK)
/*! @} */

/*! @name CIU2_BT_PS - BT Clock Power Save */
/*! @{ */

#define CIU2_CIU2_BT_PS_BT_MCLK_NCO_MVAL_MASK    (0x3FFFFFFU)
#define CIU2_CIU2_BT_PS_BT_MCLK_NCO_MVAL_SHIFT   (0U)
/*! BT_MCLK_NCO_MVAL - BT_MCLK NCO Module Step Control (default 0x0) */
#define CIU2_CIU2_BT_PS_BT_MCLK_NCO_MVAL(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS_BT_MCLK_NCO_MVAL_SHIFT)) & CIU2_CIU2_BT_PS_BT_MCLK_NCO_MVAL_MASK)

#define CIU2_CIU2_BT_PS_BT_MCLK_NCO_EN_MASK      (0x4000000U)
#define CIU2_CIU2_BT_PS_BT_MCLK_NCO_EN_SHIFT     (26U)
/*! BT_MCLK_NCO_EN - BT_MCLK_NCO logic to count */
#define CIU2_CIU2_BT_PS_BT_MCLK_NCO_EN(x)        (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS_BT_MCLK_NCO_EN_SHIFT)) & CIU2_CIU2_BT_PS_BT_MCLK_NCO_EN_MASK)

#define CIU2_CIU2_BT_PS_BT_MCLK_TBG_NCO_SEL_MASK (0x8000000U)
#define CIU2_CIU2_BT_PS_BT_MCLK_TBG_NCO_SEL_SHIFT (27U)
/*! BT_MCLK_TBG_NCO_SEL - BT_4M_PCM_CLK */
#define CIU2_CIU2_BT_PS_BT_MCLK_TBG_NCO_SEL(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS_BT_MCLK_TBG_NCO_SEL_SHIFT)) & CIU2_CIU2_BT_PS_BT_MCLK_TBG_NCO_SEL_MASK)

#define CIU2_CIU2_BT_PS_BT_MCLK_FROM_SOC_SEL_MASK (0x10000000U)
#define CIU2_CIU2_BT_PS_BT_MCLK_FROM_SOC_SEL_SHIFT (28U)
/*! BT_MCLK_FROM_SOC_SEL - BT_MCLK */
#define CIU2_CIU2_BT_PS_BT_MCLK_FROM_SOC_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS_BT_MCLK_FROM_SOC_SEL_SHIFT)) & CIU2_CIU2_BT_PS_BT_MCLK_FROM_SOC_SEL_MASK)
/*! @} */

/*! @name CIU2_BT_PS2 - BT Clock Power Save 2 */
/*! @{ */

#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_MVAL_MASK (0x3FFFFFFU)
#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_MVAL_SHIFT (0U)
/*! BT_PCM_CLK_NCO_MVAL - BT_PCM_CLK NCO Module Step Control (default 0x0) */
#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_MVAL(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_MVAL_SHIFT)) & CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_MVAL_MASK)

#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_EN_MASK  (0x4000000U)
#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_EN_SHIFT (26U)
/*! BT_PCM_CLK_NCO_EN - BT_PCM_CLK_NCO logic to count */
#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_EN(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_EN_SHIFT)) & CIU2_CIU2_BT_PS2_BT_PCM_CLK_NCO_EN_MASK)

#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_TBG_NCO_SEL_MASK (0x8000000U)
#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_TBG_NCO_SEL_SHIFT (27U)
/*! BT_PCM_CLK_TBG_NCO_SEL - BT_4M_PCM_CLK */
#define CIU2_CIU2_BT_PS2_BT_PCM_CLK_TBG_NCO_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS2_BT_PCM_CLK_TBG_NCO_SEL_SHIFT)) & CIU2_CIU2_BT_PS2_BT_PCM_CLK_TBG_NCO_SEL_MASK)
/*! @} */

/*! @name CIU2_BT_REF_CTRL - BT Ref Control */
/*! @{ */

#define CIU2_CIU2_BT_REF_CTRL_NCO_EN_MASK        (0x1U)
#define CIU2_CIU2_BT_REF_CTRL_NCO_EN_SHIFT       (0U)
/*! NCO_EN - Bluetooth Reference Clock NCO Enable information to APU. */
#define CIU2_CIU2_BT_REF_CTRL_NCO_EN(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_REF_CTRL_NCO_EN_SHIFT)) & CIU2_CIU2_BT_REF_CTRL_NCO_EN_MASK)

#define CIU2_CIU2_BT_REF_CTRL_NCO_SEL_MASK       (0x2U)
#define CIU2_CIU2_BT_REF_CTRL_NCO_SEL_SHIFT      (1U)
/*! NCO_SEL - Bluetooth Reference Clock NCO Select Value */
#define CIU2_CIU2_BT_REF_CTRL_NCO_SEL(x)         (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_REF_CTRL_NCO_SEL_SHIFT)) & CIU2_CIU2_BT_REF_CTRL_NCO_SEL_MASK)

#define CIU2_CIU2_BT_REF_CTRL_NCO_GEN_MASK       (0x3FFFCU)
#define CIU2_CIU2_BT_REF_CTRL_NCO_GEN_SHIFT      (2U)
/*! NCO_GEN - Bluetooth Reference Clock NCO Gen Value */
#define CIU2_CIU2_BT_REF_CTRL_NCO_GEN(x)         (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_REF_CTRL_NCO_GEN_SHIFT)) & CIU2_CIU2_BT_REF_CTRL_NCO_GEN_MASK)

#define CIU2_CIU2_BT_REF_CTRL_BT_CLK_NCO_REFCLK_SEL_MASK (0x100000U)
#define CIU2_CIU2_BT_REF_CTRL_BT_CLK_NCO_REFCLK_SEL_SHIFT (20U)
/*! BT_CLK_NCO_REFCLK_SEL - BT clk (bt sys clk) selection */
#define CIU2_CIU2_BT_REF_CTRL_BT_CLK_NCO_REFCLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_REF_CTRL_BT_CLK_NCO_REFCLK_SEL_SHIFT)) & CIU2_CIU2_BT_REF_CTRL_BT_CLK_NCO_REFCLK_SEL_MASK)
/*! @} */

/*! @name CIU2_BT_PS3 - BT Clock Power Save 3 */
/*! @{ */

#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_STEP_CTRL_MASK (0x3FFFFFFU)
#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_STEP_CTRL_SHIFT (0U)
/*! BTU_16M_CLK_NCO_STEP_CTRL - BT_16M_CLK NCO Module Step Control */
#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_STEP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_STEP_CTRL_SHIFT)) & CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_STEP_CTRL_MASK)

#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_EN_MASK (0x4000000U)
#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_EN_SHIFT (26U)
/*! BTU_16M_CLK_NCO_EN - BTU 16M Clock NCO Enable */
#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_EN_SHIFT)) & CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_EN_MASK)

#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_SEL_MASK (0x8000000U)
#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_SEL_SHIFT (27U)
/*! BTU_16M_CLK_NCO_SEL - BTU 16M clock NCO Select Value */
#define CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_SEL_SHIFT)) & CIU2_CIU2_BT_PS3_BTU_16M_CLK_NCO_SEL_MASK)

#define CIU2_CIU2_BT_PS3_BTU_CLK_NCO_MODE_MASK   (0x20000000U)
#define CIU2_CIU2_BT_PS3_BTU_CLK_NCO_MODE_SHIFT  (29U)
/*! BTU_CLK_NCO_MODE - BTU Clock source from ref clock (nco mode) */
#define CIU2_CIU2_BT_PS3_BTU_CLK_NCO_MODE(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BT_PS3_BTU_CLK_NCO_MODE_SHIFT)) & CIU2_CIU2_BT_PS3_BTU_CLK_NCO_MODE_MASK)
/*! @} */

/*! @name CIU2_BTU_ECO_CTRL - BTU ECO Control */
/*! @{ */

#define CIU2_CIU2_BTU_ECO_CTRL_ECO_BITS_MASK     (0xFFFFFFFFU)
#define CIU2_CIU2_BTU_ECO_CTRL_ECO_BITS_SHIFT    (0U)
/*! ECO_BITS - Reserved */
#define CIU2_CIU2_BTU_ECO_CTRL_ECO_BITS(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BTU_ECO_CTRL_ECO_BITS_SHIFT)) & CIU2_CIU2_BTU_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU2_INT_MASK - CIU2 Interrupt Mask */
/*! @{ */

#define CIU2_CIU2_INT_MASK_MASK_MASK             (0xFFFFFFFFU)
#define CIU2_CIU2_INT_MASK_MASK_SHIFT            (0U)
/*! MASK - Interrupt Mask for CIU2 Interrupts */
#define CIU2_CIU2_INT_MASK_MASK(x)               (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_INT_MASK_MASK_SHIFT)) & CIU2_CIU2_INT_MASK_MASK_MASK)
/*! @} */

/*! @name CIU2_INT_SELECT - CIU2 Interrupt Select */
/*! @{ */

#define CIU2_CIU2_INT_SELECT_SEL_MASK            (0xFFFFFFFFU)
#define CIU2_CIU2_INT_SELECT_SEL_SHIFT           (0U)
/*! SEL - Interrupt Read/Write Clear for CIU2 Interrupts */
#define CIU2_CIU2_INT_SELECT_SEL(x)              (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_INT_SELECT_SEL_SHIFT)) & CIU2_CIU2_INT_SELECT_SEL_MASK)
/*! @} */

/*! @name CIU2_INT_EVENT_MASK - CIU2 Interrupt Event Mask */
/*! @{ */

#define CIU2_CIU2_INT_EVENT_MASK_MASK_MASK       (0xFFFFFFFFU)
#define CIU2_CIU2_INT_EVENT_MASK_MASK_SHIFT      (0U)
/*! MASK - Interrupt Event Mask for CIU2 Interrupts */
#define CIU2_CIU2_INT_EVENT_MASK_MASK(x)         (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_INT_EVENT_MASK_MASK_SHIFT)) & CIU2_CIU2_INT_EVENT_MASK_MASK_MASK)
/*! @} */

/*! @name CIU2_INT_STATUS - CIU2 Interrupt Status */
/*! @{ */

#define CIU2_CIU2_INT_STATUS_CIU_ISR_MASK        (0xFFFFFFFFU)
#define CIU2_CIU2_INT_STATUS_CIU_ISR_SHIFT       (0U)
/*! CIU_ISR - CIU2 Interrupt Status (ISR) */
#define CIU2_CIU2_INT_STATUS_CIU_ISR(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_INT_STATUS_CIU_ISR_SHIFT)) & CIU2_CIU2_INT_STATUS_CIU_ISR_MASK)
/*! @} */

/*! @name CPU2_ERR_INT_MASK - CPU2 ERR Interrupt Mask */
/*! @{ */

#define CIU2_CPU2_ERR_INT_MASK_MASK_MASK         (0xFFFFFFFFU)
#define CIU2_CPU2_ERR_INT_MASK_MASK_SHIFT        (0U)
/*! MASK - Interrupt Mask for CPU2 ERR Interrupts */
#define CIU2_CPU2_ERR_INT_MASK_MASK(x)           (((uint32_t)(((uint32_t)(x)) << CIU2_CPU2_ERR_INT_MASK_MASK_SHIFT)) & CIU2_CPU2_ERR_INT_MASK_MASK_MASK)
/*! @} */

/*! @name CPU2_ERR_INT_SELECT - CPU2 ERR Interrupt Clear Select */
/*! @{ */

#define CIU2_CPU2_ERR_INT_SELECT_SEL_MASK        (0xFFFFFFFFU)
#define CIU2_CPU2_ERR_INT_SELECT_SEL_SHIFT       (0U)
/*! SEL - Interrupt Read/Write Clear for CPU2 ERR Interrupts */
#define CIU2_CPU2_ERR_INT_SELECT_SEL(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CPU2_ERR_INT_SELECT_SEL_SHIFT)) & CIU2_CPU2_ERR_INT_SELECT_SEL_MASK)
/*! @} */

/*! @name CPU2_ERR_INT_EVENT_MASK - CPU2 ERR Interrupt Event Mask */
/*! @{ */

#define CIU2_CPU2_ERR_INT_EVENT_MASK_MASK_MASK   (0xFFFFFFFFU)
#define CIU2_CPU2_ERR_INT_EVENT_MASK_MASK_SHIFT  (0U)
/*! MASK - Interrupt Event Mask for CPU2 ERR Interrupts */
#define CIU2_CPU2_ERR_INT_EVENT_MASK_MASK(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CPU2_ERR_INT_EVENT_MASK_MASK_SHIFT)) & CIU2_CPU2_ERR_INT_EVENT_MASK_MASK_MASK)
/*! @} */

/*! @name CPU2_ERR_INT_STATUS - CPU2 ERR Interrupt Status */
/*! @{ */

#define CIU2_CPU2_ERR_INT_STATUS_ERR_ISR_MASK    (0xFFFFFFFFU)
#define CIU2_CPU2_ERR_INT_STATUS_ERR_ISR_SHIFT   (0U)
/*! ERR_ISR - CPU2 ERR Interrupt Status (ISR) */
#define CIU2_CPU2_ERR_INT_STATUS_ERR_ISR(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CPU2_ERR_INT_STATUS_ERR_ISR_SHIFT)) & CIU2_CPU2_ERR_INT_STATUS_ERR_ISR_MASK)
/*! @} */

/*! @name CIU2_BCA1_CPU2_INT_MASK - BCA1 to CPU2 Interrupt Mask */
/*! @{ */

#define CIU2_CIU2_BCA1_CPU2_INT_MASK_IMR_MASK    (0xFFFFFFFFU)
#define CIU2_CIU2_BCA1_CPU2_INT_MASK_IMR_SHIFT   (0U)
/*! IMR - Interrupt Mask for BCA1 to CPU2 Interrupts */
#define CIU2_CIU2_BCA1_CPU2_INT_MASK_IMR(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BCA1_CPU2_INT_MASK_IMR_SHIFT)) & CIU2_CIU2_BCA1_CPU2_INT_MASK_IMR_MASK)
/*! @} */

/*! @name CIU2_BCA1_CPU2_INT_SELECT - BCA1 to CPU2 Interrupt Select */
/*! @{ */

#define CIU2_CIU2_BCA1_CPU2_INT_SELECT_RSR_MASK  (0xFFFFFFFFU)
#define CIU2_CIU2_BCA1_CPU2_INT_SELECT_RSR_SHIFT (0U)
/*! RSR - Interrupt Read/Write Clear for BCA1 to CPU2 Interrupts */
#define CIU2_CIU2_BCA1_CPU2_INT_SELECT_RSR(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BCA1_CPU2_INT_SELECT_RSR_SHIFT)) & CIU2_CIU2_BCA1_CPU2_INT_SELECT_RSR_MASK)
/*! @} */

/*! @name CIU2_BCA1_CPU2_INT_EVENT_MASK - BCA1 to CPU2 Interrupt Event Mask */
/*! @{ */

#define CIU2_CIU2_BCA1_CPU2_INT_EVENT_MASK_SMR_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_BCA1_CPU2_INT_EVENT_MASK_SMR_SHIFT (0U)
/*! SMR - Interrupt Event Mask for BCA1 to CPU2 Interrupts */
#define CIU2_CIU2_BCA1_CPU2_INT_EVENT_MASK_SMR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BCA1_CPU2_INT_EVENT_MASK_SMR_SHIFT)) & CIU2_CIU2_BCA1_CPU2_INT_EVENT_MASK_SMR_MASK)
/*! @} */

/*! @name CIU2_BCA1_CPU2_INT_STATUS - BCA1 to CPU2 Interrupt Status */
/*! @{ */

#define CIU2_CIU2_BCA1_CPU2_INT_STATUS_ISR_MASK  (0xFFFFFFFFU)
#define CIU2_CIU2_BCA1_CPU2_INT_STATUS_ISR_SHIFT (0U)
/*! ISR - BCA1 to CPU2 Interrupt Status */
#define CIU2_CIU2_BCA1_CPU2_INT_STATUS_ISR(x)    (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BCA1_CPU2_INT_STATUS_ISR_SHIFT)) & CIU2_CIU2_BCA1_CPU2_INT_STATUS_ISR_MASK)
/*! @} */

/*! @name CIU2_APU_BYPASS1 - CIU2 APU Bypass Register 1 */
/*! @{ */

#define CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_EN_MASK (0x1U)
#define CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_EN_SHIFT (0U)
/*! BRF_CLK_EN_BYPASS_EN - Firmware Bypass BRF_Clk_En */
#define CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_VAL_MASK (0x2U)
#define CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_VAL_SHIFT (1U)
/*! BRF_CLK_EN_BYPASS_VAL - Firmware Bypass Value for BRF_Clk_En (active high signal) */
#define CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_BYPASS1_BRF_CLK_EN_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_EN_MASK (0x4U)
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_EN_SHIFT (2U)
/*! BT_AES_CLK_EN_BYPASS_EN - Firmware Bypass for Btu_Aes_Clk */
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_VAL_MASK (0x8U)
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_VAL_SHIFT (3U)
/*! BT_AES_CLK_EN_BYPASS_VAL - Firmware Bypass Value for Btu_Aes_Clk */
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_EN_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_EN_MASK (0x10U)
#define CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_EN_SHIFT (4U)
/*! SOC_CLK_EN2_T1_BYPASS_EN - Firmware Bypass for SoC_Clk_En2 */
#define CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_VAL_MASK (0x20U)
#define CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_VAL_SHIFT (5U)
/*! SOC_CLK_EN2_T1_BYPASS_VAL - Firmware Bypass Value for SoC_Clk_En2(active high signal) */
#define CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_BYPASS1_SOC_CLK_EN2_T1_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_SEL_MASK (0xC0U)
#define CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_SEL_SHIFT (6U)
/*! TBG_BTU_CLK_EN_BYPASS_SEL - TBG512_320_176_BTU_Clk_En_Sel to TBG512_320_176 of CAU */
#define CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_SEL_SHIFT)) & CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_SEL_MASK)

#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_EN_MASK (0x100U)
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_EN_SHIFT (8U)
/*! BT_AES_CLK_SEL_BYPASS_EN - Firmware Bypass for Btu_Aes_Clk_Sel */
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_VAL_MASK (0x200U)
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_VAL_SHIFT (9U)
/*! BT_AES_CLK_SEL_BYPASS_VAL - Firmware Bypass Value for Btu_Aes_Clk_Sel */
#define CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_BYPASS1_BT_AES_CLK_SEL_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_VAL_MASK (0x400U)
#define CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_VAL_SHIFT (10U)
/*! TBG_BTU_CLK_EN_BYPASS_VAL - TBG512_320_176_BTU_Clk_En Bypass Value */
#define CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_BYPASS1_TBG_BTU_CLK_EN_BYPASS_VAL_MASK)
/*! @} */

/*! @name CIU2_CPU2_LMU_STA_BYPASS0 - LMU static bank control byapss0 Register for CPU2 mem */
/*! @{ */

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_EN_MASK (0xFFU)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_EN_SHIFT (0U)
/*! LMU_STA_BANKS_ISO_EN_BP_EN - Firmware Bypass enable for lmu static banks iso_en */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_EN_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_EN_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_VAL_MASK (0xFF00U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_VAL_SHIFT (8U)
/*! LMU_STA_BANKS_ISO_EN_BP_VAL - Firmware Bypass value for lmu static banks iso_en */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_VAL_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_ISO_EN_BP_VAL_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_EN_MASK (0xFF0000U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_EN_SHIFT (16U)
/*! LMU_STA_BANKS_PSW_EN_BP_EN - Firmware Bypass enable for lmu static banks psw_en */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_EN_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_EN_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_VAL_MASK (0xFF000000U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_VAL_SHIFT (24U)
/*! LMU_STA_BANKS_PSW_EN_BP_VAL - Firmware Bypass value for lmu static banks psw_en */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_VAL_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS0_LMU_STA_BANKS_PSW_EN_BP_VAL_MASK)
/*! @} */

/*! @name CIU2_CPU2_LMU_STA_BYPASS1 - LMU static bank control byapss1 Register for CPU2 */
/*! @{ */

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_EN_MASK (0xFFU)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_EN_SHIFT (0U)
/*! LMU_STA_BANKS_SRAM_PD_BP_EN - Firmware Bypass enable for lmu static banks sram_pd */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_EN_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_EN_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_VAL_MASK (0xFF00U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_VAL_SHIFT (8U)
/*! LMU_STA_BANKS_SRAM_PD_BP_VAL - Firmware Bypass value for lmu static banks sram_pd */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_VAL_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_SRAM_PD_BP_VAL_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_EN_MASK (0xFF0000U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_EN_SHIFT (16U)
/*! LMU_STA_BANKS_FNRST_BP_EN - Firmware Bypass enable for lmu static banks fnrst */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_EN_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_EN_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_VAL_MASK (0xFF000000U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_VAL_SHIFT (24U)
/*! LMU_STA_BANKS_FNRST_BP_VAL - Firmware Bypass value for lmu static banks fnrst */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_VAL_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS1_LMU_STA_BANKS_FNRST_BP_VAL_MASK)
/*! @} */

/*! @name CIU2_CPU2_LMU_STA_BYPASS2 - LMU static bank byapss2 Register for CPU2 */
/*! @{ */

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_EN_MASK (0xFFU)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_EN_SHIFT (0U)
/*! LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_EN - Firmware Bypass enable for lmu static banks vddmc_sw_pd_ctrl */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_EN_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_EN_MASK)

#define CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_VAL_MASK (0xFF00U)
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_VAL_SHIFT (8U)
/*! LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_VAL - Firmware Bypass value for lmu static banks vddmc_sw_pd_ctrl */
#define CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_VAL_SHIFT)) & CIU2_CIU2_CPU2_LMU_STA_BYPASS2_LMU_STA_BANKS_VDDMC_SW_PD_CTRL_BP_VAL_MASK)
/*! @} */

/*! @name CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS - LMU G2Bist control bypass Register for CPU2 */
/*! @{ */

#define CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_EN_MASK (0x1U)
#define CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_EN_SHIFT (0U)
/*! LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_EN - Firmware Bypass enable for CPU2 static banks lmu power domain repair request */
#define CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_EN_SHIFT)) & CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_EN_MASK)

#define CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_VAL_MASK (0xFEU)
#define CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_VAL_SHIFT (1U)
/*! LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_VAL - Firmware Bypass value for CPU2 static banks lmu power domain repair request */
#define CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_VAL_SHIFT)) & CIU2_CIU2_CPU2_LMU_G2BIST_CTRL_BYPASS_LMU_CPU2_STA_PWRDMN_RPR_REQ_BP_VAL_MASK)
/*! @} */

/*! @name CIU2_APU_PWR_CTRL_BYPASS1 - APU power control Bypass Register 1 */
/*! @{ */

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_VAL_MASK (0x1U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_VAL_SHIFT (0U)
/*! BRF_PSW_BYPASS_VAL - brf Power Switch Control */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_EN_MASK (0x2U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_EN_SHIFT (1U)
/*! BRF_PSW_BYPASS_EN - brf Power Switch Control Enable */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_PSW_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_VAL_MASK (0x4U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_VAL_SHIFT (2U)
/*! BRF_FWBAR_BYPASS_VAL - brf Firewallbar Control */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_EN_MASK (0x8U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_EN_SHIFT (3U)
/*! BRF_FWBAR_BYPASS_EN - brf Firewallbar Control Enable */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_FWBAR_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_VAL_MASK (0x10U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_VAL_SHIFT (4U)
/*! BRF_ISO_EN_BYPASS_VAL - brf Isolation Cell Control */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_EN_MASK (0x20U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_EN_SHIFT (5U)
/*! BRF_ISO_EN_BYPASS_EN - brf Isolation Cell Control Enable */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_ISO_EN_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_VAL_MASK (0x40U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_VAL_SHIFT (6U)
/*! BRF_CLK_DIV_RSTB_BYPASS_VAL - Firmware Bypass Value for brf Clk_Div_Rstb (active low signal) */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_EN_MASK (0x80U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_EN_SHIFT (7U)
/*! BRF_CLK_DIV_RSTB_BYPASS_EN - Firmware Bypass brf Clk_Div_Rstb from APU */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_CLK_DIV_RSTB_BYPASS_EN_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_VAL_MASK (0x100U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_VAL_SHIFT (8U)
/*! BRF_SRAM_PD_BYPASS_VAL - Firmware Bypass Value for SRAM_PD (active high signal) */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_VAL_MASK)

#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_EN_MASK (0x200U)
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_EN_SHIFT (9U)
/*! BRF_SRAM_PD_BYPASS_EN - Firmware Bypass SRAM_PD from APU */
#define CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_EN_SHIFT)) & CIU2_CIU2_APU_PWR_CTRL_BYPASS1_BRF_SRAM_PD_BYPASS_EN_MASK)
/*! @} */

/*! @name CIU2_AHB2AHB_BRIDGE_CTRL - AHB2AHB Bridge Control Register */
/*! @{ */

#define CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_MASK (0x1U)
#define CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_SHIFT (0U)
/*! PREFETCH_HSEL_EN - ahb2ahb bridge pre-fetch hsel enable */
#define CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_SHIFT)) & CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_MASK)

#define CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_MCI_AHB2_A2A_PREFETCH_EN_MASK (0x2U)
#define CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_MCI_AHB2_A2A_PREFETCH_EN_SHIFT (1U)
/*! MCI_AHB2_A2A_PREFETCH_EN - MCI-AHB2 ahb2ahb bridge pre-fetch hsel enable */
#define CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_MCI_AHB2_A2A_PREFETCH_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_MCI_AHB2_A2A_PREFETCH_EN_SHIFT)) & CIU2_CIU2_AHB2AHB_BRIDGE_CTRL_MCI_AHB2_A2A_PREFETCH_EN_MASK)
/*! @} */

/*! @name CIU2_CLK_CP15_DIS1 - Clock Auto Shut-off Enable1 */
/*! @{ */

#define CIU2_CIU2_CLK_CP15_DIS1_CIU_BTAPU_AHB_CLK_DIS_ON_SLP_MASK (0x100000U)
#define CIU2_CIU2_CLK_CP15_DIS1_CIU_BTAPU_AHB_CLK_DIS_ON_SLP_SHIFT (20U)
/*! CIU_BTAPU_AHB_CLK_DIS_ON_SLP - APU Shut Off */
#define CIU2_CIU2_CLK_CP15_DIS1_CIU_BTAPU_AHB_CLK_DIS_ON_SLP(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CLK_CP15_DIS1_CIU_BTAPU_AHB_CLK_DIS_ON_SLP_SHIFT)) & CIU2_CIU2_CLK_CP15_DIS1_CIU_BTAPU_AHB_CLK_DIS_ON_SLP_MASK)
/*! @} */

/*! @name CIU_CLK_RTU_NCO_CTRL - RTU NCO Clock Control */
/*! @{ */

#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_REF_CLK_SEL_MASK (0x1U)
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_REF_CLK_SEL_SHIFT (0U)
/*! CIU_BTRTU_REF_CLK_SEL - RTU Reference Clock from UART reference clock tree */
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_REF_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_REF_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_REF_CLK_SEL_MASK)

#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_OUT_SEL_MASK (0x2U)
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_OUT_SEL_SHIFT (1U)
/*! CIU_BTRTU_NCO_OUT_SEL - RTU NCO Mode Select (Reference Clock Based) */
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_OUT_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_OUT_SEL_SHIFT)) & CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_OUT_SEL_MASK)

#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_ENABLE_MASK (0x4U)
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_ENABLE_SHIFT (2U)
/*! CIU_BTRTU_NCO_ENABLE - RTU NCO Enable (Reference Clock Based) */
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_ENABLE_SHIFT)) & CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_ENABLE_MASK)

#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_STEP_MASK (0xFFFF0000U)
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_STEP_SHIFT (16U)
/*! CIU_BTRTU_NCO_STEP - Step size for RTU clock NCO (Reference Clock Based) */
#define CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_STEP(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_STEP_SHIFT)) & CIU2_CIU_CLK_RTU_NCO_CTRL_CIU_BTRTU_NCO_STEP_MASK)
/*! @} */

/*! @name CIU_CLK_SOCCLK_CTRL - SOC Clock Control */
/*! @{ */

#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_EN_MASK (0x40U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_EN_SHIFT (6U)
/*! BTU_PCM_CLK_T3_256_DIV125_EN - T3_256 DIV125 Enable */
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_EN_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_EN_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_CLK_SEL_MASK (0x80U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_CLK_SEL_SHIFT (7U)
/*! BTU_PCM_CLK_T3_256_DIV125_CLK_SEL - T3_256 DIV125 Clock Select */
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_BTU_PCM_CLK_T3_256_DIV125_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_EN_MASK (0x100U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_EN_SHIFT (8U)
/*! BTU_MCLK_T3_512_DIV125_EN - T3_512 DIV125 Enable */
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_EN_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_EN_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_CLK_SEL_MASK (0x200U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_CLK_SEL_SHIFT (9U)
/*! BTU_MCLK_T3_512_DIV125_CLK_SEL - T3_512 DIV125 Clock Select */
#define CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_BTU_MCLK_T3_512_DIV125_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CONST_CAL_CLK_SEL_MASK (0x400U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CONST_CAL_CLK_SEL_SHIFT (10U)
/*! CIU_BTAPU_CONST_CAL_CLK_SEL - PMU Constant Calibration Clock Select */
#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CONST_CAL_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CONST_CAL_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CONST_CAL_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CAL_CLK_SEL_MASK (0x800U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CAL_CLK_SEL_SHIFT (11U)
/*! CIU_BTAPU_CAL_CLK_SEL - PMU Calibration Clock */
#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CAL_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CAL_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_CIU_BTAPU_CAL_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_SEL_MASK (0x1000U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_SEL_SHIFT (12U)
/*! PM_CLK_SEL - PM Clock source select */
#define CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_SEL(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_TBG_SEL_MASK (0x2000U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_TBG_SEL_SHIFT (13U)
/*! PM_CLK_TBG_SEL - PM Clock */
#define CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_TBG_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_TBG_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_PM_CLK_TBG_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_MASK (0x4000U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_SHIFT (14U)
/*! CIU_USE_REFCLK - SoC_Clk Clock */
#define CIU2_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_BT_AES_CLK_32_64_SEL_MASK (0x8000U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_BT_AES_CLK_32_64_SEL_SHIFT (15U)
/*! BT_AES_CLK_32_64_SEL - BT AES Clock Select 32 MHz or 64 MHz select */
#define CIU2_CIU_CLK_SOCCLK_CTRL_BT_AES_CLK_32_64_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_BT_AES_CLK_32_64_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_BT_AES_CLK_32_64_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_PCLK_DIV_SEL_MASK (0xF0000U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_PCLK_DIV_SEL_SHIFT (16U)
/*! AHB2_AHB2APB_PCLK_DIV_SEL - AHB2 AHB2APB PCLK Divider Select */
#define CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_PCLK_DIV_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_PCLK_DIV_SEL_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_PCLK_DIV_SEL_MASK)

#define CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_WAIT_CYCLES_MASK (0xF0000000U)
#define CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_WAIT_CYCLES_SHIFT (28U)
/*! AHB2_AHB2APB_WAIT_CYCLES - AH2 AHB2APB Wait Cycles between each APB transaction */
#define CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_WAIT_CYCLES(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_WAIT_CYCLES_SHIFT)) & CIU2_CIU_CLK_SOCCLK_CTRL_AHB2_AHB2APB_WAIT_CYCLES_MASK)
/*! @} */

/*! @name CIU_CLK_SLEEPCLK_CTRL - Sleep Clock Control */
/*! @{ */

#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_MASK (0xFFFFFFU)
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_SHIFT (0U)
/*! CIU_SLEEP_CLK_NCO_MVAL - Sleep Clock NCO */
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_SHIFT)) & CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_MASK)

#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_MASK (0x2000000U)
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_SHIFT (25U)
/*! CIU_NCO_SLEEP_CLK_SEL - NCO Sleep Clock Select */
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_MASK (0x10000000U)
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_SHIFT (28U)
/*! CIU_REFCLK_SLEEP_CLK_SEL - Reference Clock Sleep Clock Select */
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_MASK)

#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_PCIE_SLP_CLK_SEL_MASK (0x40000000U)
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_PCIE_SLP_CLK_SEL_SHIFT (30U)
/*! CIU_PCIE_SLP_CLK_SEL - PCIE Sleep Clock Select */
#define CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_PCIE_SLP_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_PCIE_SLP_CLK_SEL_SHIFT)) & CIU2_CIU_CLK_SLEEPCLK_CTRL_CIU_PCIE_SLP_CLK_SEL_MASK)
/*! @} */

/*! @name CIU_CLK_SLEEPCLK_CTRL2 - Sleep Clock Control 2 */
/*! @{ */

#define CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_MASK (0xFFFFFFU)
#define CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_SHIFT (0U)
/*! CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP - Sleep Clock NCO value for the sleep mode */
#define CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_SHIFT)) & CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_MASK)

#define CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_MASK (0x1000000U)
#define CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_SHIFT (24U)
/*! CIU_SLEEP_CLK_NCO_MVAL_BYPASS - Sleep Clock NCO mval Bypass */
#define CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_SHIFT)) & CIU2_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_MASK)
/*! @} */

/*! @name CIU2_IOMUX_MODE_CTRL - Test Bus Select */
/*! @{ */

#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_BRF_BIDI_MODE_MASK (0x1U)
#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_BRF_BIDI_MODE_SHIFT (0U)
/*! CIU2_EXT_BRF_BIDI_MODE - " */
#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_BRF_BIDI_MODE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_BRF_BIDI_MODE_SHIFT)) & CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_BRF_BIDI_MODE_MASK)

#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_BRF_STANDALONE_DATA_MODE_MASK (0x2U)
#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_BRF_STANDALONE_DATA_MODE_SHIFT (1U)
/*! CIU2_BRF_STANDALONE_DATA_MODE - " */
#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_BRF_STANDALONE_DATA_MODE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_BRF_STANDALONE_DATA_MODE_SHIFT)) & CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_BRF_STANDALONE_DATA_MODE_MASK)

#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_FRF_MODE_MASK (0x4U)
#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_FRF_MODE_SHIFT (2U)
/*! CIU2_EXT_FRF_MODE - 0: On Chip BRF is used for 15. */
#define CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_FRF_MODE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_FRF_MODE_SHIFT)) & CIU2_CIU2_IOMUX_MODE_CTRL_CIU2_EXT_FRF_MODE_MASK)
/*! @} */

/*! @name CIU2_RST_SW2 - Software Module Reset */
/*! @{ */

#define CIU2_CIU2_RST_SW2_DRO__MASK              (0x2U)
#define CIU2_CIU2_RST_SW2_DRO__SHIFT             (1U)
/*! DRO_ - DRO Clock Reset */
#define CIU2_CIU2_RST_SW2_DRO_(x)                (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_DRO__SHIFT)) & CIU2_CIU2_RST_SW2_DRO__MASK)

#define CIU2_CIU2_RST_SW2_BTAPU_AHB_CLK__MASK    (0x4U)
#define CIU2_CIU2_RST_SW2_BTAPU_AHB_CLK__SHIFT   (2U)
/*! BTAPU_AHB_CLK_ - AHB Clock Logic reset for BTAPU */
#define CIU2_CIU2_RST_SW2_BTAPU_AHB_CLK_(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_BTAPU_AHB_CLK__SHIFT)) & CIU2_CIU2_RST_SW2_BTAPU_AHB_CLK__MASK)

#define CIU2_CIU2_RST_SW2_BTAPU_REF_CLK__MASK    (0x8U)
#define CIU2_CIU2_RST_SW2_BTAPU_REF_CLK__SHIFT   (3U)
/*! BTAPU_REF_CLK_ - Ref Clock Logic reset for BTAPU */
#define CIU2_CIU2_RST_SW2_BTAPU_REF_CLK_(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_BTAPU_REF_CLK__SHIFT)) & CIU2_CIU2_RST_SW2_BTAPU_REF_CLK__MASK)

#define CIU2_CIU2_RST_SW2_BTAPU_SLP_CLK__MASK    (0x10U)
#define CIU2_CIU2_RST_SW2_BTAPU_SLP_CLK__SHIFT   (4U)
/*! BTAPU_SLP_CLK_ - Sleep Clock Logic reset for BTAPU */
#define CIU2_CIU2_RST_SW2_BTAPU_SLP_CLK_(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_BTAPU_SLP_CLK__SHIFT)) & CIU2_CIU2_RST_SW2_BTAPU_SLP_CLK__MASK)

#define CIU2_CIU2_RST_SW2_CIU2_REGISTER_RST__MASK (0x20U)
#define CIU2_CIU2_RST_SW2_CIU2_REGISTER_RST__SHIFT (5U)
/*! CIU2_REGISTER_RST_ - CIU_Reg Module Soft Reset */
#define CIU2_CIU2_RST_SW2_CIU2_REGISTER_RST_(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_REGISTER_RST__SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_REGISTER_RST__MASK)

#define CIU2_CIU2_RST_SW2_SMU2_TM__MASK          (0x40U)
#define CIU2_CIU2_RST_SW2_SMU2_TM__SHIFT         (6U)
/*! SMU2_TM_ - Test Mode Reset for SMU2 */
#define CIU2_CIU2_RST_SW2_SMU2_TM_(x)            (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_SMU2_TM__SHIFT)) & CIU2_CIU2_RST_SW2_SMU2_TM__MASK)

#define CIU2_CIU2_RST_SW2_MSC_A2A__MASK          (0x80U)
#define CIU2_CIU2_RST_SW2_MSC_A2A__SHIFT         (7U)
/*! MSC_A2A_ - Soft Reset to A2A in MSC */
#define CIU2_CIU2_RST_SW2_MSC_A2A_(x)            (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_MSC_A2A__SHIFT)) & CIU2_CIU2_RST_SW2_MSC_A2A__MASK)

#define CIU2_CIU2_RST_SW2_SOCCIU_A2A_RST__MASK   (0x100U)
#define CIU2_CIU2_RST_SW2_SOCCIU_A2A_RST__SHIFT  (8U)
/*! SOCCIU_A2A_RST_ - Soft Reset to A2A in SOCCIU */
#define CIU2_CIU2_RST_SW2_SOCCIU_A2A_RST_(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_SOCCIU_A2A_RST__SHIFT)) & CIU2_CIU2_RST_SW2_SOCCIU_A2A_RST__MASK)

#define CIU2_CIU2_RST_SW2_CIU2_FFU_RST__MASK     (0x200U)
#define CIU2_CIU2_RST_SW2_CIU2_FFU_RST__SHIFT    (9U)
/*! CIU2_FFU_RST_ - Soft Reset to FFU */
#define CIU2_CIU2_RST_SW2_CIU2_FFU_RST_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_FFU_RST__SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_FFU_RST__MASK)

#define CIU2_CIU2_RST_SW2_CIU2_AHB2APB_SW_RESETN_MASK (0x400U)
#define CIU2_CIU2_RST_SW2_CIU2_AHB2APB_SW_RESETN_SHIFT (10U)
/*! CIU2_AHB2APB_SW_RESETN - SW reset to the ahb2apb hresetn */
#define CIU2_CIU2_RST_SW2_CIU2_AHB2APB_SW_RESETN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_AHB2APB_SW_RESETN_SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_AHB2APB_SW_RESETN_MASK)

#define CIU2_CIU2_RST_SW2_CIU2_FFU_AHB_RST__MASK (0x10000U)
#define CIU2_CIU2_RST_SW2_CIU2_FFU_AHB_RST__SHIFT (16U)
/*! CIU2_FFU_AHB_RST_ - Soft Reset to FFU AHB I/F logic */
#define CIU2_CIU2_RST_SW2_CIU2_FFU_AHB_RST_(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_FFU_AHB_RST__SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_FFU_AHB_RST__MASK)

#define CIU2_CIU2_RST_SW2_CIU2_FFU_SLP_RST__MASK (0x20000U)
#define CIU2_CIU2_RST_SW2_CIU2_FFU_SLP_RST__SHIFT (17U)
/*! CIU2_FFU_SLP_RST_ - Soft Reset to FFU Sleep Clock Domain Logic */
#define CIU2_CIU2_RST_SW2_CIU2_FFU_SLP_RST_(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_FFU_SLP_RST__SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_FFU_SLP_RST__MASK)

#define CIU2_CIU2_RST_SW2_ABH2_SUB_G2BIST_RSTB_MASK (0x40000U)
#define CIU2_CIU2_RST_SW2_ABH2_SUB_G2BIST_RSTB_SHIFT (18U)
/*! ABH2_SUB_G2BIST_RSTB - SW reset for ble abh2-system g2bist controller */
#define CIU2_CIU2_RST_SW2_ABH2_SUB_G2BIST_RSTB(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_ABH2_SUB_G2BIST_RSTB_SHIFT)) & CIU2_CIU2_RST_SW2_ABH2_SUB_G2BIST_RSTB_MASK)

#define CIU2_CIU2_RST_SW2_BLE_AHB_RST__MASK      (0x80000U)
#define CIU2_CIU2_RST_SW2_BLE_AHB_RST__SHIFT     (19U)
/*! BLE_AHB_RST_ - SW reset for ble ahb arb/dec/ciu */
#define CIU2_CIU2_RST_SW2_BLE_AHB_RST_(x)        (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_BLE_AHB_RST__SHIFT)) & CIU2_CIU2_RST_SW2_BLE_AHB_RST__MASK)

#define CIU2_CIU2_RST_SW2_CIU2_BTU_SLP_RST__MASK (0x100000U)
#define CIU2_CIU2_RST_SW2_CIU2_BTU_SLP_RST__SHIFT (20U)
/*! CIU2_BTU_SLP_RST_ - Soft Reset to BTU Sleep Clock Domain Logic */
#define CIU2_CIU2_RST_SW2_CIU2_BTU_SLP_RST_(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_BTU_SLP_RST__SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_BTU_SLP_RST__MASK)

#define CIU2_CIU2_RST_SW2_MCI_BLE_A2A_MHRESETN_MASK (0x200000U)
#define CIU2_CIU2_RST_SW2_MCI_BLE_A2A_MHRESETN_SHIFT (21U)
/*! MCI_BLE_A2A_MHRESETN - SW Reset for mci_wl_a2a_mhresetn */
#define CIU2_CIU2_RST_SW2_MCI_BLE_A2A_MHRESETN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_MCI_BLE_A2A_MHRESETN_SHIFT)) & CIU2_CIU2_RST_SW2_MCI_BLE_A2A_MHRESETN_MASK)

#define CIU2_CIU2_RST_SW2_CIU2_CFG_RST__MASK     (0x400000U)
#define CIU2_CIU2_RST_SW2_CIU2_CFG_RST__SHIFT    (22U)
/*! CIU2_CFG_RST_ - CIU config reset for IMU, RTU, CIU debug sync */
#define CIU2_CIU2_RST_SW2_CIU2_CFG_RST_(x)       (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_RST_SW2_CIU2_CFG_RST__SHIFT)) & CIU2_CIU2_RST_SW2_CIU2_CFG_RST__MASK)
/*! @} */

/*! @name CIU2_AHB2_TO_CLEAR - AHB2 timeout logic clear register */
/*! @{ */

#define CIU2_CIU2_AHB2_TO_CLEAR_AHB2_TIMEOUT_CLEAR_MASK (0x100U)
#define CIU2_CIU2_AHB2_TO_CLEAR_AHB2_TIMEOUT_CLEAR_SHIFT (8U)
/*! AHB2_TIMEOUT_CLEAR - After the timeout happened on AHB2 bus, the cpu will read the ERR ISR and
 *    read the bus state which cause the timeout and then set this bit to 1 to clear the AHB2 timeout
 *    logic to start recording next transaction. This is self clearing bit
 */
#define CIU2_CIU2_AHB2_TO_CLEAR_AHB2_TIMEOUT_CLEAR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CLEAR_AHB2_TIMEOUT_CLEAR_SHIFT)) & CIU2_CIU2_AHB2_TO_CLEAR_AHB2_TIMEOUT_CLEAR_MASK)

#define CIU2_CIU2_AHB2_TO_CLEAR_CPU2_DCODE_INV_ADDR_CLR_MASK (0x200U)
#define CIU2_CIU2_AHB2_TO_CLEAR_CPU2_DCODE_INV_ADDR_CLR_SHIFT (9U)
/*! CPU2_DCODE_INV_ADDR_CLR - After the invalid address int happened on CPU2 dcode bus, the cpu2
 *    will read the ERR ISR and read the bus state which cause the timeout and then set this bit to 1
 *    to clear the CPU2 Dcode invalid addr logic to start recording next transaction. This is self
 *    clearing bit
 */
#define CIU2_CIU2_AHB2_TO_CLEAR_CPU2_DCODE_INV_ADDR_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CLEAR_CPU2_DCODE_INV_ADDR_CLR_SHIFT)) & CIU2_CIU2_AHB2_TO_CLEAR_CPU2_DCODE_INV_ADDR_CLR_MASK)

#define CIU2_CIU2_AHB2_TO_CLEAR_CPU2_ICODE_INV_ADDR_CLR_MASK (0x400U)
#define CIU2_CIU2_AHB2_TO_CLEAR_CPU2_ICODE_INV_ADDR_CLR_SHIFT (10U)
/*! CPU2_ICODE_INV_ADDR_CLR - After the invalid address int happened on CPU2 icode bus, the cpu2
 *    will read the ERR ISR and read the bus state which cause the timeout and then set this bit to 1
 *    to clear the CPU2 Icode invalid addr logic to start recording next transaction. This is self
 *    clearing bit
 */
#define CIU2_CIU2_AHB2_TO_CLEAR_CPU2_ICODE_INV_ADDR_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_AHB2_TO_CLEAR_CPU2_ICODE_INV_ADDR_CLR_SHIFT)) & CIU2_CIU2_AHB2_TO_CLEAR_CPU2_ICODE_INV_ADDR_CLR_MASK)
/*! @} */

/*! @name CIU2_CPU_DYN_CLK_CTRL - Dynamic CPU Clock Control */
/*! @{ */

#define CIU2_CIU2_CPU_DYN_CLK_CTRL_DYN_CPU2_CTRL_DIS_MASK (0x40000000U)
#define CIU2_CIU2_CPU_DYN_CLK_CTRL_DYN_CPU2_CTRL_DIS_SHIFT (30U)
/*! DYN_CPU2_CTRL_DIS - Disable Dynamic CPU2 Clock Control Feature */
#define CIU2_CIU2_CPU_DYN_CLK_CTRL_DYN_CPU2_CTRL_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_DYN_CLK_CTRL_DYN_CPU2_CTRL_DIS_SHIFT)) & CIU2_CIU2_CPU_DYN_CLK_CTRL_DYN_CPU2_CTRL_DIS_MASK)
/*! @} */

/*! @name CPU2_DBG_STAT - CPU2 debug register */
/*! @{ */

#define CIU2_CPU2_DBG_STAT_CPU2_STATUS_MASK      (0xFFFFFFFFU)
#define CIU2_CPU2_DBG_STAT_CPU2_STATUS_SHIFT     (0U)
/*! CPU2_STATUS - cpu2 debug output */
#define CIU2_CPU2_DBG_STAT_CPU2_STATUS(x)        (((uint32_t)(((uint32_t)(x)) << CIU2_CPU2_DBG_STAT_CPU2_STATUS_SHIFT)) & CIU2_CPU2_DBG_STAT_CPU2_STATUS_MASK)
/*! @} */

/*! @name BTSS_MBIST_STAT -  */
/*! @{ */

#define CIU2_BTSS_MBIST_STAT_BLESS_MBIST_FAIL_MASK (0x1FFU)
#define CIU2_BTSS_MBIST_STAT_BLESS_MBIST_FAIL_SHIFT (0U)
/*! BLESS_MBIST_FAIL - BIST Fail Indication from memories in BT Sub System */
#define CIU2_BTSS_MBIST_STAT_BLESS_MBIST_FAIL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BTSS_MBIST_STAT_BLESS_MBIST_FAIL_SHIFT)) & CIU2_BTSS_MBIST_STAT_BLESS_MBIST_FAIL_MASK)

#define CIU2_BTSS_MBIST_STAT_BLESS_MBIST_READY_MASK (0x1FF0000U)
#define CIU2_BTSS_MBIST_STAT_BLESS_MBIST_READY_SHIFT (16U)
/*! BLESS_MBIST_READY - BIST Ready from memories in BT Sub System */
#define CIU2_BTSS_MBIST_STAT_BLESS_MBIST_READY(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BTSS_MBIST_STAT_BLESS_MBIST_READY_SHIFT)) & CIU2_BTSS_MBIST_STAT_BLESS_MBIST_READY_MASK)
/*! @} */

/*! @name CIU2_TEST_MODE - " */
/*! @{ */

#define CIU2_CIU2_TEST_MODE_BT_UART_MODE_MASK    (0x1U)
#define CIU2_CIU2_TEST_MODE_BT_UART_MODE_SHIFT   (0U)
/*! BT_UART_MODE - Indicates UART Mode for I/O muxing */
#define CIU2_CIU2_TEST_MODE_BT_UART_MODE(x)      (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_TEST_MODE_BT_UART_MODE_SHIFT)) & CIU2_CIU2_TEST_MODE_BT_UART_MODE_MASK)
/*! @} */

/*! @name CIU2_APU_BYPASS2 - CIU2 APU Bypass Register 2 */
/*! @{ */

#define CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_SEL_MASK (0xC0000U)
#define CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_SEL_SHIFT (18U)
/*! TBG_FFU_CLK_EN_BYPASS_SEL - TBG FFU clock enable bypass select */
#define CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_SEL_SHIFT)) & CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_SEL_MASK)

#define CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_VAL_MASK (0x100000U)
#define CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_VAL_SHIFT (20U)
/*! TBG_FFU_CLK_EN_BYPASS_VAL - TBG FFU clock enable bypass value */
#define CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_VAL_SHIFT)) & CIU2_CIU2_APU_BYPASS2_TBG_FFU_CLK_EN_BYPASS_VAL_MASK)
/*! @} */

/*! @name CIU2_TST_G2BIST_STATUS - WL G2BIST Status */
/*! @{ */

#define CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_STATUS_MASK (0xFU)
#define CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_STATUS_SHIFT (0U)
/*! AHB2_G2B_STATUS - Redundant Bist Selection */
#define CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_STATUS_SHIFT)) & CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_STATUS_MASK)

#define CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_FINIH_MASK (0x100U)
#define CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_FINIH_SHIFT (8U)
/*! AHB2_G2B_FINIH - AHB2 Bist Done */
#define CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_FINIH(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_FINIH_SHIFT)) & CIU2_CIU2_TST_G2BIST_STATUS_AHB2_G2B_FINIH_MASK)
/*! @} */

/*! @name CIU2_LPO_CLK_GEN_CTRL - BLE LPO CLK GEN Control */
/*! @{ */

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_AUTO_DEJIT_MASK (0x1U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_AUTO_DEJIT_SHIFT (0U)
/*! AUTO_DEJIT - Enable Auto Dejitter */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_AUTO_DEJIT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_AUTO_DEJIT_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_AUTO_DEJIT_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_DEJIT_EN_MASK (0x2U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_DEJIT_EN_SHIFT (1U)
/*! DEJIT_EN - Enable De-jitter block */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_DEJIT_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_DEJIT_EN_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_DEJIT_EN_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_MAN_SEL_NCO_MASK (0x4U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_MAN_SEL_NCO_SHIFT (2U)
/*! MAN_SEL_NCO - Manually Switch back to NCO Version */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_MAN_SEL_NCO(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_MAN_SEL_NCO_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_MAN_SEL_NCO_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_NCO_EN_MASK (0x8U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_NCO_EN_SHIFT (3U)
/*! SLP_CLK_NCO_EN - Enable NCO Counter in ble_lpoClk_gen */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_NCO_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_NCO_EN_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_NCO_EN_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_LBC_DEBUG_CTRL_MASK (0x30U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_LBC_DEBUG_CTRL_SHIFT (4U)
/*! LBC_DEBUG_CTRL - LBC Debug Control Signal */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_LBC_DEBUG_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_LBC_DEBUG_CTRL_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_LBC_DEBUG_CTRL_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_RST__MASK (0x40U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_RST__SHIFT (6U)
/*! SLP_CLK_RST_ - Soft Reset to LPO Clock Generator for NCO Sleep Clock Domain Logic */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_RST_(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_RST__SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_SLP_CLK_RST__MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_REF_4M_RST__MASK (0x80U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_REF_4M_RST__SHIFT (7U)
/*! REF_4M_RST_ - Soft Reset to LPO Clock Generator for 4M Clock Domain Logic */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_REF_4M_RST_(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_REF_4M_RST__SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_REF_4M_RST__MASK)

#define CIU2_CIU2_LPO_CLK_GEN_CTRL_LPO_CLK_SEL_MASK (0x300U)
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_LPO_CLK_SEL_SHIFT (8U)
/*! LPO_CLK_SEL - Selects the source of 4 MHz clock input to BLE LPO Clock Generator */
#define CIU2_CIU2_LPO_CLK_GEN_CTRL_LPO_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_CTRL_LPO_CLK_SEL_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_CTRL_LPO_CLK_SEL_MASK)
/*! @} */

/*! @name CIU2_LPO_CLK_GEN_STATUS - BLE LPO CLK GEN Status */
/*! @{ */

#define CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_3K2_CNT_MASK (0x1FU)
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_3K2_CNT_SHIFT (0U)
/*! LPO_CLK_3K2_CNT - " */
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_3K2_CNT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_3K2_CNT_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_3K2_CNT_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_SEL_FSM_MASK (0x100U)
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_SEL_FSM_SHIFT (8U)
/*! LPO_CLK_SEL_FSM - " */
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_SEL_FSM(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_SEL_FSM_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_STATUS_LPO_CLK_SEL_FSM_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_RAMP_DN_MASK (0x200U)
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_RAMP_DN_SHIFT (9U)
/*! REF_LPO_RAMP_DN - " */
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_RAMP_DN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_RAMP_DN_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_RAMP_DN_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_CLK_GOOD_MASK (0x400U)
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_CLK_GOOD_SHIFT (10U)
/*! REF_LPO_CLK_GOOD - " */
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_CLK_GOOD(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_CLK_GOOD_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_STATUS_REF_LPO_CLK_GOOD_MASK)

#define CIU2_CIU2_LPO_CLK_GEN_STATUS_NCO_LPO_RAMP_DN_LV_MASK (0x800U)
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_NCO_LPO_RAMP_DN_LV_SHIFT (11U)
/*! NCO_LPO_RAMP_DN_LV - " */
#define CIU2_CIU2_LPO_CLK_GEN_STATUS_NCO_LPO_RAMP_DN_LV(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_CLK_GEN_STATUS_NCO_LPO_RAMP_DN_LV_SHIFT)) & CIU2_CIU2_LPO_CLK_GEN_STATUS_NCO_LPO_RAMP_DN_LV_MASK)
/*! @} */

/*! @name CIU2_LPO_SLP_CLK_GEN_CTRL - " */
/*! @{ */

#define CIU2_CIU2_LPO_SLP_CLK_GEN_CTRL_SLP_CLK_NCO_STEP_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_LPO_SLP_CLK_GEN_CTRL_SLP_CLK_NCO_STEP_SHIFT (0U)
/*! SLP_CLK_NCO_STEP - " */
#define CIU2_CIU2_LPO_SLP_CLK_GEN_CTRL_SLP_CLK_NCO_STEP(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_LPO_SLP_CLK_GEN_CTRL_SLP_CLK_NCO_STEP_SHIFT)) & CIU2_CIU2_LPO_SLP_CLK_GEN_CTRL_SLP_CLK_NCO_STEP_MASK)
/*! @} */

/*! @name CPU2_INT_CTRL - " */
/*! @{ */

#define CIU2_CPU2_INT_CTRL_CPU2_CPU3_GP_INT_MASK (0xFU)
#define CIU2_CPU2_INT_CTRL_CPU2_CPU3_GP_INT_SHIFT (0U)
/*! CPU2_CPU3_GP_INT - General Purpose Interrupt in Secure Region */
#define CIU2_CPU2_INT_CTRL_CPU2_CPU3_GP_INT(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CPU2_INT_CTRL_CPU2_CPU3_GP_INT_SHIFT)) & CIU2_CPU2_INT_CTRL_CPU2_CPU3_GP_INT_MASK)
/*! @} */

/*! @name CIU2_BRF_EXTRA_PORT_STATUS - " */
/*! @{ */

#define CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_CIU2_EXTRA_STATUS_MASK (0x3U)
#define CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_CIU2_EXTRA_STATUS_SHIFT (0U)
/*! BRF_CIU2_EXTRA_STATUS - [1:0] of BRF_SOC_EXTRA output of BRF are made available as status register bits. */
#define CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_CIU2_EXTRA_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_CIU2_EXTRA_STATUS_SHIFT)) & CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_CIU2_EXTRA_STATUS_MASK)

#define CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_ERR_FLAG_MASK (0x100U)
#define CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_ERR_FLAG_SHIFT (8U)
/*! BRF_ERR_FLAG - Error Flag Output from BRF is reported here. */
#define CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_ERR_FLAG(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_ERR_FLAG_SHIFT)) & CIU2_CIU2_BRF_EXTRA_PORT_STATUS_BRF_ERR_FLAG_MASK)
/*! @} */

/*! @name CIU2_DEBUG - " */
/*! @{ */

#define CIU2_CIU2_DEBUG_P2C_UART_SIN_MASK        (0x1U)
#define CIU2_CIU2_DEBUG_P2C_UART_SIN_SHIFT       (0U)
/*! P2C_UART_SIN - Status of UART SIN (p2c_uart_sin) net is captured here. */
#define CIU2_CIU2_DEBUG_P2C_UART_SIN(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_DEBUG_P2C_UART_SIN_SHIFT)) & CIU2_CIU2_DEBUG_P2C_UART_SIN_MASK)
/*! @} */

/*! @name CIU2_MCI_EXTRA - MCI EXTRA Ports */
/*! @{ */

#define CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_OUT_MASK (0xFU)
#define CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_OUT_SHIFT (0U)
/*! CIU2_MCI_EXTRA_OUT - Extra Ports to MCI */
#define CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_OUT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_OUT_SHIFT)) & CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_OUT_MASK)

#define CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_IN_MASK (0xF0U)
#define CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_IN_SHIFT (4U)
/*! CIU2_MCI_EXTRA_IN - Extra Ports from MCI */
#define CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_IN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_IN_SHIFT)) & CIU2_CIU2_MCI_EXTRA_CIU2_MCI_EXTRA_IN_MASK)
/*! @} */

/*! @name CIU2_TSTBUS_SEL - Test Bus Select */
/*! @{ */

#define CIU2_CIU2_TSTBUS_SEL_AHB2_TSTBUS_SEL_MASK (0x1U)
#define CIU2_CIU2_TSTBUS_SEL_AHB2_TSTBUS_SEL_SHIFT (0U)
/*! AHB2_TSTBUS_SEL - Select between 2 groups of signals; The output of the mux is driven to BLE
 *    Test Mux Logic so that it can be observed on GPIOs.
 */
#define CIU2_CIU2_TSTBUS_SEL_AHB2_TSTBUS_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_TSTBUS_SEL_AHB2_TSTBUS_SEL_SHIFT)) & CIU2_CIU2_TSTBUS_SEL_AHB2_TSTBUS_SEL_MASK)

#define CIU2_CIU2_TSTBUS_SEL_SMU2_TSTBUS_SEL_MASK (0x10U)
#define CIU2_CIU2_TSTBUS_SEL_SMU2_TSTBUS_SEL_SHIFT (4U)
/*! SMU2_TSTBUS_SEL - Selects between following 2 Test Buses in SMU2 */
#define CIU2_CIU2_TSTBUS_SEL_SMU2_TSTBUS_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_TSTBUS_SEL_SMU2_TSTBUS_SEL_SHIFT)) & CIU2_CIU2_TSTBUS_SEL_SMU2_TSTBUS_SEL_MASK)
/*! @} */

/*! @name FFU_CTRL - FFU Specific Control Register */
/*! @{ */

#define CIU2_FFU_CTRL_USE_TX_EN_EXTEND_MASK      (0x1U)
#define CIU2_FFU_CTRL_USE_TX_EN_EXTEND_SHIFT     (0U)
/*! USE_TX_EN_EXTEND - Selects tx_en_extend port of FFU & drives it on c2p_soc_zigbee_tx_en. */
#define CIU2_FFU_CTRL_USE_TX_EN_EXTEND(x)        (((uint32_t)(((uint32_t)(x)) << CIU2_FFU_CTRL_USE_TX_EN_EXTEND_SHIFT)) & CIU2_FFU_CTRL_USE_TX_EN_EXTEND_MASK)
/*! @} */

/*! @name BLE_RAACS_CTRL - RAACS control registers */
/*! @{ */

#define CIU2_BLE_RAACS_CTRL_RAACS_EN_MASK        (0x1U)
#define CIU2_BLE_RAACS_CTRL_RAACS_EN_SHIFT       (0U)
/*! RAACS_EN - raacs en . S/W Write 1 to enable raacs block. */
#define CIU2_BLE_RAACS_CTRL_RAACS_EN(x)          (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_CTRL_RAACS_EN_SHIFT)) & CIU2_BLE_RAACS_CTRL_RAACS_EN_MASK)

#define CIU2_BLE_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_MASK (0x2U)
#define CIU2_BLE_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_SHIFT (1U)
/*! USE_RAACS_CLK_FOR_CPU - SW write 0 to use RAACS clock for CPU. SW write 1 to select clock gating
 *    based alternate implementation of RAACS clocking for CM3 CPU.
 */
#define CIU2_BLE_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_SHIFT)) & CIU2_BLE_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_MASK)

#define CIU2_BLE_RAACS_CTRL_RAACS_CLK_SEL_MASK   (0x1CU)
#define CIU2_BLE_RAACS_CTRL_RAACS_CLK_SEL_SHIFT  (2U)
/*! RAACS_CLK_SEL - defines the lowest clock to which RAACS will go down to during IDLE period (x/2;
 *    x/4; x/8; ... ;x/128) for the given test.
 */
#define CIU2_BLE_RAACS_CTRL_RAACS_CLK_SEL(x)     (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_CTRL_RAACS_CLK_SEL_SHIFT)) & CIU2_BLE_RAACS_CTRL_RAACS_CLK_SEL_MASK)

#define CIU2_BLE_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_MASK (0x7FFE0U)
#define CIU2_BLE_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_SHIFT (5U)
/*! RAACS_WAIT_COUNTER_VALUE - initial IDLE-time for which RAACS FSM waits before starting to scale down the clock. */
#define CIU2_BLE_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_SHIFT)) & CIU2_BLE_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_MASK)

#define CIU2_BLE_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_MASK (0xFFF80000U)
#define CIU2_BLE_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_SHIFT (19U)
/*! RAACS_IDLE_COUNTER_VALUE - IDLE time for which RAACS-FSM waits before shifting to next successive scaled clock. */
#define CIU2_BLE_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_SHIFT)) & CIU2_BLE_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_MASK)
/*! @} */

/*! @name BLE_RAACS_PERFORMANCE_STATISTICS - RAACS performance statistics counter. */
/*! @{ */

#define CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_MASK (0x1U)
#define CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_SHIFT (0U)
/*! PERFORMANCE_STATISTICS_CNT_EN - performance counter en. S/W write 1 to enable performance counter. */
#define CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_SHIFT)) & CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_MASK)

#define CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_MASK (0xFFFFFFEU)
#define CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_SHIFT (1U)
/*! RAACS_PERFORMANCE_STATISTICS - This counter is maintaining RAACS performance count. This counter
 *    will increment by one after every 1 us(1MHZ) when RAACS is in scaled clk state.
 */
#define CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS(x) (((uint32_t)(((uint32_t)(x)) << CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_SHIFT)) & CIU2_BLE_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_MASK)
/*! @} */

/*! @name CIU2_CPU_CPU2_MSG_CTRL - CPU2 message register */
/*! @{ */

#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_RDY_MASK (0x1U)
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_RDY_SHIFT (0U)
/*! CPU2_TO_CPU3_MSG_RDY - CPU2 Message for CPU3 is ready. This is self clearing bit. The CPU2
 *    writes 1 to indicate that message for CPU3is ready. This generates an Interrupt to CPU3 via APU.
 *    This is old scheme and we should use IMU based scheme.
 */
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_RDY(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_RDY_SHIFT)) & CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_RDY_MASK)

#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_RDY_MASK (0x2U)
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_RDY_SHIFT (1U)
/*! CPU3_TO_CPU2_MSG_RDY - CPU3 Message for CPU2 is ready. This is self clearing bit. The CPU3
 *    writes 1 to indicate that message for CPU2 is ready. This generates an Interrupt to CPU2 via APU.
 *    This is old scheme and we should use IMU based scheme.
 */
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_RDY(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_RDY_SHIFT)) & CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_RDY_MASK)

#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_PROCESS_DONE_MASK (0x100U)
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_PROCESS_DONE_SHIFT (8U)
/*! CPU2_TO_CPU3_MSG_PROCESS_DONE - CPU2 Message for CPU3 has been read by CPU3 and executed. This
 *    is self clearing bit. The CPU3 writes 1 to indicate that message send by CPU2 is executed. This
 *    generates an Interrupt to CPU2 via CIU1.
 */
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_PROCESS_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_PROCESS_DONE_SHIFT)) & CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU2_TO_CPU3_MSG_PROCESS_DONE_MASK)

#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_PROCESS_DONE_MASK (0x200U)
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_PROCESS_DONE_SHIFT (9U)
/*! CPU3_TO_CPU2_MSG_PROCESS_DONE - CPU3 Message for CPU2 has been read by CPU2 and executed. This
 *    is self clearing bit. The CPU2 writes 1 to indicate that message send by CPU3 is executed. This
 *    generates an Interrupt to CPU3 via CIU1.
 */
#define CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_PROCESS_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_PROCESS_DONE_SHIFT)) & CIU2_CIU2_CPU_CPU2_MSG_CTRL_CPU3_TO_CPU2_MSG_PROCESS_DONE_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU3_WR_MSG_TO_CPU2 - CPU3 write message to CPU2 */
/*! @{ */

#define CIU2_CIU2_IMU_CPU3_WR_MSG_TO_CPU2_CPU3_WR_MSG_CPU2_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_IMU_CPU3_WR_MSG_TO_CPU2_CPU3_WR_MSG_CPU2_SHIFT (0U)
/*! CPU3_WR_MSG_CPU2 - Write CPU3 message data to CPU2 (push to FIFO) */
#define CIU2_CIU2_IMU_CPU3_WR_MSG_TO_CPU2_CPU3_WR_MSG_CPU2(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_WR_MSG_TO_CPU2_CPU3_WR_MSG_CPU2_SHIFT)) & CIU2_CIU2_IMU_CPU3_WR_MSG_TO_CPU2_CPU3_WR_MSG_CPU2_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU3_RD_MSG_FROM_CPU2 - CPU3 read message from CPU2 */
/*! @{ */

#define CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_CPU3_RD_MSG_CPU2_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_CPU3_RD_MSG_CPU2_SHIFT (0U)
/*! CPU3_RD_MSG_CPU2 - CPU3 read message data from CPU2 (pop from FIFO) */
#define CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_CPU3_RD_MSG_CPU2(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_CPU3_RD_MSG_CPU2_SHIFT)) & CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_CPU3_RD_MSG_CPU2_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS - CPU3 to CPU2 message FIFO status */
/*! @{ */

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_LOCKED_MASK (0x1U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_LOCKED_SHIFT (0U)
/*! CPU3_TO_CPU2_MSG_FIFO_LOCKED - cpu3_to_cpu2_msg_fifo_locked */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_LOCKED(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_LOCKED_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_LOCKED_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_ALMOST_FULL_MASK (0x2U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_ALMOST_FULL_SHIFT (1U)
/*! CPU3_TO_CPU2_MSG_FIFO_ALMOST_FULL - cpu3_to_cpu2_msg_fifo_almost_full (based upon FIFO watermark) */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_ALMOST_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_ALMOST_FULL_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_ALMOST_FULL_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_FULL_MASK (0x4U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_FULL_SHIFT (2U)
/*! CPU3_TO_CPU2_MSG_FIFO_FULL - cpu3_to_cpu2_msg_fifo_full (based upon FIFO depth) */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_FULL_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_FULL_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_EMPTY_MASK (0x8U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_EMPTY_SHIFT (3U)
/*! CPU3_TO_CPU2_MSG_FIFO_EMPTY - cpu3_to_cpu2_msg_fifo_empty */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_EMPTY(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_EMPTY_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_EMPTY_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_COUNT_MASK (0x1F0U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_COUNT_SHIFT (4U)
/*! CPU3_TO_CPU2_MSG_COUNT - cpu3_to_cpu2_msg_count */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_COUNT_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_COUNT_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_WR_PTR_MASK (0xF0000U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_WR_PTR_SHIFT (16U)
/*! CPU3_TO_CPU2_MSG_FIFO_WR_PTR - cpu3 to cpu2 msg fifo write pointer for debug */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_WR_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_WR_PTR_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_WR_PTR_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_RD_PTR_MASK (0xF00000U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_RD_PTR_SHIFT (20U)
/*! CPU3_TO_CPU2_MSG_FIFO_RD_PTR - cpu3 to cpu2 msg fifo read pointer for debug */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_RD_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_RD_PTR_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_STATUS_CPU3_TO_CPU2_MSG_FIFO_RD_PTR_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL - CPU3 to CPU2 message FIFO control */
/*! @{ */

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_MASK (0x1U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_SHIFT (0U)
/*! CPU3_MSG_RDY_INT_CLR - Writing 1 to this bit will clear message ready interrupt to CPU3 (self clear bit) */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_MASK (0x100U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_SHIFT (8U)
/*! CPU3_MSG_SP_AV_INT_CLR - Writing 1 to this bit will clear message space available interrupt to CPU3 (self clear bit) */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_TO_CPU2_MSG_FIFO_FLUSH_MASK (0x10000U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_TO_CPU2_MSG_FIFO_FLUSH_SHIFT (16U)
/*! CPU3_TO_CPU2_MSG_FIFO_FLUSH - Writing 1 to this bit will flush cpu3_to_cpu2 message fifo */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_TO_CPU2_MSG_FIFO_FLUSH(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_TO_CPU2_MSG_FIFO_FLUSH_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_TO_CPU2_MSG_FIFO_FLUSH_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_MASK (0x20000U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_SHIFT (17U)
/*! CPU3_WAIT_FOR_ACK - CPU3 wait for Acknowledgment */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_MASK)

#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_CPU2_MSG_FIFO_FULL_WATERMARK_MASK (0xF00000U)
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_CPU2_MSG_FIFO_FULL_WATERMARK_SHIFT (20U)
/*! CPU3_CPU2_MSG_FIFO_FULL_WATERMARK - cpu3_to_cpu2 message fifo full watermark (space avail intr based upon it) */
#define CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_CPU2_MSG_FIFO_FULL_WATERMARK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_CPU2_MSG_FIFO_FULL_WATERMARK_SHIFT)) & CIU2_CIU2_IMU_CPU3_CPU2_MSG_FIFO_CNTL_CPU3_CPU2_MSG_FIFO_FULL_WATERMARK_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG - CPU2 last message read (from cpu3) */
/*! @{ */

#define CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG_CPU2_RD_MSG_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG_CPU2_RD_MSG_SHIFT (0U)
/*! CPU2_RD_MSG - CPU2 last message read (from cpu3) */
#define CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG_CPU2_RD_MSG(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG_CPU2_RD_MSG_SHIFT)) & CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_VAL_DBG_CPU2_RD_MSG_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU2_WR_MSG_TO_CPU3 - CPU2 write message to CPU3 */
/*! @{ */

#define CIU2_CIU2_IMU_CPU2_WR_MSG_TO_CPU3_CPU2_WR_MSG_CPU3_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_IMU_CPU2_WR_MSG_TO_CPU3_CPU2_WR_MSG_CPU3_SHIFT (0U)
/*! CPU2_WR_MSG_CPU3 - Write CPU2 message data to CPU3 (push to FIFO) */
#define CIU2_CIU2_IMU_CPU2_WR_MSG_TO_CPU3_CPU2_WR_MSG_CPU3(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_WR_MSG_TO_CPU3_CPU2_WR_MSG_CPU3_SHIFT)) & CIU2_CIU2_IMU_CPU2_WR_MSG_TO_CPU3_CPU2_WR_MSG_CPU3_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU2_RD_MSG_FROM_CPU3 - CPU2 read message from CPU3 */
/*! @{ */

#define CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_CPU2_RD_MSG_CPU3_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_CPU2_RD_MSG_CPU3_SHIFT (0U)
/*! CPU2_RD_MSG_CPU3 - CPU2 read message data from CPU3 (pop from FIFO) */
#define CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_CPU2_RD_MSG_CPU3(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_CPU2_RD_MSG_CPU3_SHIFT)) & CIU2_CIU2_IMU_CPU2_RD_MSG_FROM_CPU3_CPU2_RD_MSG_CPU3_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS - CPU2 to CPU3 message FIFO status */
/*! @{ */

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_LOCKED_MASK (0x1U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_LOCKED_SHIFT (0U)
/*! CPU2_TO_CPU3_MSG_FIFO_LOCKED - cpu2_to_cpu3_msg_fifo_locked */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_LOCKED(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_LOCKED_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_LOCKED_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_ALMOST_FULL_MASK (0x2U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_ALMOST_FULL_SHIFT (1U)
/*! CPU2_TO_CPU3_MSG_FIFO_ALMOST_FULL - cpu2_to_cpu3_msg_fifo_almost_full (based upon FIFO watermark) */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_ALMOST_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_ALMOST_FULL_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_ALMOST_FULL_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_FULL_MASK (0x4U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_FULL_SHIFT (2U)
/*! CPU2_TO_CPU3_MSG_FIFO_FULL - cpu2_to_cpu3_msg_fifo_full (based upon FIFO depth) */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_FULL_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_FULL_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_EMPTY_MASK (0x8U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_EMPTY_SHIFT (3U)
/*! CPU2_TO_CPU3_MSG_FIFO_EMPTY - cpu2_to_cpu3_msg_fifo_empty */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_EMPTY(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_EMPTY_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_EMPTY_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_COUNT_MASK (0x1F0U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_COUNT_SHIFT (4U)
/*! CPU2_TO_CPU3_MSG_COUNT - cpu2_to_cpu3_msg_count */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_COUNT_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_COUNT_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_WR_PTR_MASK (0xF0000U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_WR_PTR_SHIFT (16U)
/*! CPU2_TO_CPU3_MSG_FIFO_WR_PTR - cpu3 to cpu2 msg fifo write pointer for debug */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_WR_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_WR_PTR_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_WR_PTR_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_RD_PTR_MASK (0xF00000U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_RD_PTR_SHIFT (20U)
/*! CPU2_TO_CPU3_MSG_FIFO_RD_PTR - cpu3 to cpu2 msg fifo read pointer for debug */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_RD_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_RD_PTR_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_STATUS_CPU2_TO_CPU3_MSG_FIFO_RD_PTR_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL - CPU2 to CPU3 message FIFO control */
/*! @{ */

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_RDY_INT_CLR_MASK (0x1U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_RDY_INT_CLR_SHIFT (0U)
/*! CPU2_MSG_RDY_INT_CLR - Writing 1 to this bit will clear message ready interrupt to CPU2 (self clear bit) */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_RDY_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_RDY_INT_CLR_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_RDY_INT_CLR_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_SP_AV_INT_CLR_MASK (0x100U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_SP_AV_INT_CLR_SHIFT (8U)
/*! CPU2_MSG_SP_AV_INT_CLR - Writing 1 to this bit will clear message space available interrupt to CPU2 (self clear bit) */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_SP_AV_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_SP_AV_INT_CLR_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_MSG_SP_AV_INT_CLR_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_TO_CPU3_MSG_FIFO_FLUSH_MASK (0x10000U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_TO_CPU3_MSG_FIFO_FLUSH_SHIFT (16U)
/*! CPU2_TO_CPU3_MSG_FIFO_FLUSH - Writing 1 to this bit will flush cpu2_to_cpu3 message fifo */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_TO_CPU3_MSG_FIFO_FLUSH(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_TO_CPU3_MSG_FIFO_FLUSH_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_TO_CPU3_MSG_FIFO_FLUSH_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_WAIT_FOR_ACK_MASK (0x20000U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_WAIT_FOR_ACK_SHIFT (17U)
/*! CPU2_WAIT_FOR_ACK - CPU2 wait for Acknowledgment */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_WAIT_FOR_ACK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_WAIT_FOR_ACK_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_WAIT_FOR_ACK_MASK)

#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_CPU3_MSG_FIFO_FULL_WATERMARK_MASK (0xF00000U)
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_CPU3_MSG_FIFO_FULL_WATERMARK_SHIFT (20U)
/*! CPU2_CPU3_MSG_FIFO_FULL_WATERMARK - cpu2_to_cpu3 message fifo full watermark (space avail intr based upon it) */
#define CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_CPU3_MSG_FIFO_FULL_WATERMARK(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_CPU3_MSG_FIFO_FULL_WATERMARK_SHIFT)) & CIU2_CIU2_IMU_CPU2_CPU3_MSG_FIFO_CNTL_CPU2_CPU3_MSG_FIFO_FULL_WATERMARK_MASK)
/*! @} */

/*! @name CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG - CPU3 last message read (from cpu2) */
/*! @{ */

#define CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG_CPU3_RD_MSG_MASK (0xFFFFFFFFU)
#define CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG_CPU3_RD_MSG_SHIFT (0U)
/*! CPU3_RD_MSG - CPU3 last message read (from cpu2) */
#define CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG_CPU3_RD_MSG(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG_CPU3_RD_MSG_SHIFT)) & CIU2_CIU2_IMU_CPU3_RD_MSG_FROM_CPU2_VAL_DBG_CPU3_RD_MSG_MASK)
/*! @} */

/*! @name CIU2_CPU3_WAKEUP_CTRL - CIU2 register to wakeup CPU3 */
/*! @{ */

#define CIU2_CIU2_CPU3_WAKEUP_CTRL_CPU3_WAKE_UP_MASK (0x1U)
#define CIU2_CIU2_CPU3_WAKEUP_CTRL_CPU3_WAKE_UP_SHIFT (0U)
/*! CPU3_WAKE_UP - CPU3 Wakeup Control Register. S/W Write 1 to generate a wake up interrupt to
 *    CPU3.Clear by S/W once mci_wl_wakeup_done[1] interrupt is received from CPU3.
 */
#define CIU2_CIU2_CPU3_WAKEUP_CTRL_CPU3_WAKE_UP(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU3_WAKEUP_CTRL_CPU3_WAKE_UP_SHIFT)) & CIU2_CIU2_CPU3_WAKEUP_CTRL_CPU3_WAKE_UP_MASK)
/*! @} */

/*! @name CIU2_CPU2_WAKEUP_DONE - Wakeup done Control Register to CPU3 */
/*! @{ */

#define CIU2_CIU2_CPU2_WAKEUP_DONE_CPU2_WAKEUP_DONE_MASK (0xFFU)
#define CIU2_CIU2_CPU2_WAKEUP_DONE_CPU2_WAKEUP_DONE_SHIFT (0U)
/*! CPU2_WAKEUP_DONE - CPU2 Wakeup is done . This bit is set to 1 by S/W when CPU3 wakesup CPU2.This
 *    is self clearing bit. This generates an interrupt to CPU3 via wl_mci_wakeup_done[7:0] signal.
 */
#define CIU2_CIU2_CPU2_WAKEUP_DONE_CPU2_WAKEUP_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU2_WAKEUP_DONE_CPU2_WAKEUP_DONE_SHIFT)) & CIU2_CIU2_CPU2_WAKEUP_DONE_CPU2_WAKEUP_DONE_MASK)
/*! @} */

/*! @name CIU2_CPU3_NS_GP_INT - Non Secure region GP interrupt to CPU3 */
/*! @{ */

#define CIU2_CIU2_CPU3_NS_GP_INT_CPU2_CPU3_GP_NS_INT_MASK (0x3U)
#define CIU2_CIU2_CPU3_NS_GP_INT_CPU2_CPU3_GP_NS_INT_SHIFT (0U)
/*! CPU2_CPU3_GP_NS_INT - General Purpose interrupt to CPU3 from non secure registers */
#define CIU2_CIU2_CPU3_NS_GP_INT_CPU2_CPU3_GP_NS_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_CPU3_NS_GP_INT_CPU2_CPU3_GP_NS_INT_SHIFT)) & CIU2_CIU2_CPU3_NS_GP_INT_CPU2_CPU3_GP_NS_INT_MASK)
/*! @} */

/*! @name CIU2_IMU_ECO_BITS - IMU ECO Control */
/*! @{ */

#define CIU2_CIU2_IMU_ECO_BITS_IMU_ECO_BITS_MASK (0xFFFFU)
#define CIU2_CIU2_IMU_ECO_BITS_IMU_ECO_BITS_SHIFT (0U)
/*! IMU_ECO_BITS - Reserved for ECOs */
#define CIU2_CIU2_IMU_ECO_BITS_IMU_ECO_BITS(x)   (((uint32_t)(((uint32_t)(x)) << CIU2_CIU2_IMU_ECO_BITS_IMU_ECO_BITS_SHIFT)) & CIU2_CIU2_IMU_ECO_BITS_IMU_ECO_BITS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CIU2_Register_Masks */


/*!
 * @}
 */ /* end of group CIU2_Peripheral_Access_Layer */


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


#endif  /* CIU2_H_ */

