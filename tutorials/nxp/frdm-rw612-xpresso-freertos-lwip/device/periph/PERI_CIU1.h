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
**         CMSIS Peripheral Access Layer for CIU1
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
 * @file CIU1.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for CIU1
 *
 * CMSIS Peripheral Access Layer for CIU1
 */

#if !defined(CIU1_H_)
#define CIU1_H_                                  /**< Symbol preventing repeated inclusion */

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
   -- CIU1 Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CIU1_Peripheral_Access_Layer CIU1 Peripheral Access Layer
 * @{
 */

/** CIU1 - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[132];
  __I  uint32_t CIU_CHIP_REV;                      /**< CIU Revision, offset: 0x84 */
       uint8_t RESERVED_1[116];
  __IO uint32_t CIU_CHIP_ECO_CTRL;                 /**< Chip ECO Control, offset: 0xFC */
  __IO uint32_t CIU_CLK_ENABLE;                    /**< Clock Enable, offset: 0x100 */
  __IO uint32_t CIU_CLK_ENABLE2;                   /**< Clock enable2, offset: 0x104 */
  __IO uint32_t CIU_CLK_ENABLE3;                   /**< Clock Enable 3, offset: 0x108 */
       uint8_t RESERVED_2[8];
  __IO uint32_t CIU_CLK_CPU1CLK_CTRL;              /**< CPU1_AHB1 Clock Control, offset: 0x114 */
  __IO uint32_t CIU_CLK_SYSCLK_CTRL;               /**< SYS Clock Control, offset: 0x118 */
       uint8_t RESERVED_3[16];
  __IO uint32_t CIU_CLK_RTU_NCO_CTRL;              /**< RTU NCO Clock Control, offset: 0x12C */
  __IO uint32_t CIU_CLK_LBU1_WLRTU_CTRL;           /**< LBU1 WLRTU1 Clock Control, offset: 0x130 */
  __IO uint32_t CIU_CLK_SOCCLK_CTRL;               /**< SOC Clock Control, offset: 0x134 */
  __IO uint32_t CIU_CLK_SLEEPCLK_CTRL;             /**< Sleep Clock Control, offset: 0x138 */
  __IO uint32_t CIU_CLK_CP15_DIS1;                 /**< Clock Auto Shut-off Enable1, offset: 0x13C */
  __IO uint32_t CIU_CLK_CP15_DIS2;                 /**< Clock Auto Shut-off Enable2, offset: 0x140 */
  __IO uint32_t CIU_CLK_CP15_DIS3;                 /**< Clock Auto Shut-off Enable3, offset: 0x144 */
       uint8_t RESERVED_4[4];
  __IO uint32_t CIU_CLK_ENABLE6;                   /**< Clock Enable 6, offset: 0x14C */
  __IO uint32_t CIU_CLK_SLEEPCLK_CTRL2;            /**< Sleep Clock Control 2, offset: 0x150 */
       uint8_t RESERVED_5[36];
  __IO uint32_t CIU_MAC_CLK_CTRL;                  /**< MAC clock CTRL, offset: 0x178 */
  __IO uint32_t CIU_CLK_ECO_CTRL;                  /**< Clock ECO Control, offset: 0x17C */
  __IO uint32_t CIU_RST_SW1;                       /**< Software Module Reset, offset: 0x180 */
  __IO uint32_t CIU_RST_SW2;                       /**< Software Module Reset, offset: 0x184 */
       uint8_t RESERVED_6[4];
  __IO uint32_t CIU_RST_SW4;                       /**< Software Module Reset, offset: 0x18C */
  __IO uint32_t CIU_RST_SW5;                       /**< Software Module Reset, offset: 0x190 */
       uint8_t RESERVED_7[104];
  __IO uint32_t CIU_RST_ECO_CTRL;                  /**< Reset ECO Control, offset: 0x1FC */
       uint8_t RESERVED_8[4];
  __IO uint32_t CIU_MEM_WRTC2;                     /**< Memory WRTC Control2, offset: 0x204 */
  __IO uint32_t CIU_MEM_WRTC3;                     /**< Memory WRTC Control 3, offset: 0x208 */
       uint8_t RESERVED_9[16];
  __IO uint32_t CIU_MEM_CTRL;                      /**< Memory Control, offset: 0x21C */
  __I  uint32_t CIU_SMU1_DBG_STAT0;                /**< SMU1 debug register0, offset: 0x220 */
  __I  uint32_t CIU_SMU1_DBG_STAT1;                /**< SMU1 debug register1, offset: 0x224 */
  __I  uint32_t CIU_SMU1_DBG_STAT2;                /**< SMU1 debug register2, offset: 0x228 */
       uint8_t RESERVED_10[80];
  __IO uint32_t CIU_MEM_ECO_CTRL;                  /**< Memory ECO Control, offset: 0x27C */
  __IO uint32_t CIU1_INT_MASK;                     /**< CIU1 Interrupt Mask, offset: 0x280 */
  __IO uint32_t CIU1_INT_SELECT;                   /**< CIU1 Interrupt Select, offset: 0x284 */
  __IO uint32_t CIU1_INT_EVENT_MASK;               /**< CIU1 Interrupt Event Mask, offset: 0x288 */
  __I  uint32_t CIU1_INT_STATUS;                   /**< CIU1 Interrupt Status, offset: 0x28C */
  __IO uint32_t CIU_INT_HOST_CTRL;                 /**< Host Interrupt Control, offset: 0x290 */
  __IO uint32_t CIU_BCA1_INT_MASK;                 /**< BCA1 to CPU1 Interrupt Mask, offset: 0x294 */
  __IO uint32_t CIU_BCA1_INT_SELECT;               /**< BCA1 to CPU1 Interrupt Select, offset: 0x298 */
  __IO uint32_t CIU_BCA1_INT_EVENT_MASK;           /**< BCA1 to CPU1 Interrupt Event Mask, offset: 0x29C */
  __I  uint32_t CIU_BCA1_INT_STATUS;               /**< BCA1 to CPU1 Interrupt Status, offset: 0x2A0 */
       uint8_t RESERVED_11[16];
  __IO uint32_t CPU1_ERR_INT_MASK;                 /**< CPU1 ERR Interrupt Mask, offset: 0x2B4 */
  __IO uint32_t CPU1_ERR_INT_SELECT;               /**< CPU1 ERR Interrupt Clear Select, offset: 0x2B8 */
  __IO uint32_t CPU1_ERR_INT_EVENT_MASK;           /**< CPU1 ERR Interrupt Event Mask, offset: 0x2BC */
  __I  uint32_t CPU1_ERR_INT_STATUS;               /**< CPU1 ERR Interrupt Status, offset: 0x2C0 */
       uint8_t RESERVED_12[32];
  __IO uint32_t CPU2_INT_CTRL;                     /**< CPU2 INT control, offset: 0x2E4 */
       uint8_t RESERVED_13[16];
  __IO uint32_t CPU3_INT_CTRL;                     /**< CPU3 INT control, offset: 0x2F8 */
  __IO uint32_t CIU_INT_ECO_CTRL;                  /**< Interrupt ECO Control, offset: 0x2FC */
  __IO uint32_t CIU_PTP_CTRL;                      /**< Vsensor and Vreg Pad Control, offset: 0x300 */
       uint8_t RESERVED_14[156];
  __IO uint32_t CIU_RFU_DBC_MUX_SEL;               /**< RFU related DBC mux selection for different mode, offset: 0x3A0 */
  __IO uint32_t CIU_BCA_DBC_MUX_SEL;               /**< BCA related DBC mux selection for different mode, offset: 0x3A4 */
       uint8_t RESERVED_15[88];
  __I  uint32_t CIU_TST_G2BIST_STATUS;             /**< WL G2BIST Status, offset: 0x400 */
  __I  uint32_t CIU_TST_MBIST_READY;               /**< MBIST Status (BIST_READY), offset: 0x404 */
  __I  uint32_t CIU_TST_MBIST_FAIL;                /**< MBIST Status (BIST_FAIL), offset: 0x408 */
       uint8_t RESERVED_16[28];
  __IO uint32_t CIU_TST_TSTBUS_CTRL2;              /**< Testbux Mux Control2, offset: 0x428 */
       uint8_t RESERVED_17[4];
  __IO uint32_t CIU_TST_CTRL;                      /**< Test Control, offset: 0x430 */
       uint8_t RESERVED_18[72];
  __IO uint32_t CIU_TST_ECO_CTRL;                  /**< Test ECO Control, offset: 0x47C */
       uint8_t RESERVED_19[128];
  __IO uint32_t CIU_GPS_GPIO_MASK;                 /**< GPS GPIO MASK, offset: 0x500 */
  __IO uint32_t CIU_GPS_SW_PERIOD;                 /**< GPS SWITCH CHANNEL PERIOD, offset: 0x504 */
  __IO uint32_t CIU_GPS_SW_EARLY;                  /**< GPS SWITCH CHANNEL EARLY, offset: 0x508 */
  __I  uint32_t CIU_GPS_TMR_RD;                    /**< GPS TIMER READ, offset: 0x50C */
       uint8_t RESERVED_20[1132];
  __IO uint32_t CIU_LDO_ECO_CTRL;                  /**< LDO ECO Control, offset: 0x97C */
       uint8_t RESERVED_21[24];
  __IO uint32_t CIU_AXI_CLK_CTRL2;                 /**< AXI clk bypass contrl2, offset: 0x998 */
  __IO uint32_t CIU_FABRIC_TESTBUS_CTRL;           /**< fabric(scm, wlm) testbus select, offset: 0x99C */
  __IO uint32_t CIU_FABRIC_CREQ_DLY_TIMER;         /**< fabric(scm, wlm) delay timer for c_req, offset: 0x9A0 */
       uint8_t RESERVED_22[88];
  __IO uint32_t CIU_ABU_ECO_CTRL;                  /**< ABU ECO Control, offset: 0x9FC */
  __IO uint32_t CIU1_AHB1_TO_CLEAR;                /**< AHB1 timeout logic clear register, offset: 0xA00 */
  __I  uint32_t CIU_ARB_TO_LAST_ADDR;              /**< AHB Timeout Last Address, offset: 0xA04 */
  __I  uint32_t CIU_ARB_TO_CUR_ADDR;               /**< AHB Current Timeout Address, offset: 0xA08 */
  __IO uint32_t CIU_ARB_CTRL;                      /**< AHB ARB Control, offset: 0xA0C */
       uint8_t RESERVED_23[68];
  __IO uint32_t CIU1_CPU1_ICODE_INV_ADDR_CTRL;     /**< CPU1 Icode invalid address access control, offset: 0xA54 */
  __I  uint32_t CIU1_CPU1_ICODE_INV_ADDR;          /**< CPU1 Icode invalid address, offset: 0xA58 */
  __IO uint32_t CIU1_CPU1_DCODE_INV_ADDR_CTRL;     /**< CPU1 Dcode invalid address access control, offset: 0xA5C */
  __I  uint32_t CIU1_CPU1_DCODE_INV_ADDR;          /**< CPU1 Dcode invalid address, offset: 0xA60 */
  __IO uint32_t CIU1_AHB2AHB_BRIDGE_CTRL;          /**< AHB2AHB Bridge Control Register, offset: 0xA64 */
  __IO uint32_t WL_RAACS_CTRL;                     /**< RAACS control registers, offset: 0xA68 */
  __IO uint32_t WL_RAACS_PERFORMANCE_STATISTICS;   /**< RAACS performance statistics counter., offset: 0xA6C */
  __IO uint32_t CIU_AHB1_TSTBUS_SEL;               /**< AHB1 Control Signals testbus select, offset: 0xA70 */
       uint8_t RESERVED_24[8];
  __IO uint32_t CIU_ARB_ECO_CTRL;                  /**< ARB ECO Control, offset: 0xA7C */
       uint8_t RESERVED_25[8];
  __IO uint32_t CIU_CPU_DYN_CLK_CTRL;              /**< Dynamic CPU Clock Control, offset: 0xA88 */
       uint8_t RESERVED_26[12];
  __I  uint32_t CIU_CPU_DYN_CPUCLK_MONITOR;        /**< Dynamic ahb clock Monitor, offset: 0xA98 */
  __I  uint32_t CIU_CPU_DYN_SYSCLK_MONITOR;        /**< Dynamic sysclk Monitor, offset: 0xA9C */
       uint8_t RESERVED_27[16];
  __IO uint32_t CIU_CPU_CPU1_CTRL;                 /**< CPU1 control register, offset: 0xAB0 */
       uint8_t RESERVED_28[12];
  __IO uint32_t CIU_CPU_CPU1_ACCESS_CTRL;          /**< CPU1 access control register, offset: 0xAC0 */
  __IO uint32_t CIU_CPU_CPU2_ACCESS_CTRL;          /**< CPU2 access control register, offset: 0xAC4 */
  __IO uint32_t CIU_CPU_CPU3_ACCESS_CTRL;          /**< CPU3 access control register, offset: 0xAC8 */
  __I  uint32_t CIU_CPU_CPU1_DBG_STAT1;            /**< CPU1 debug register1, offset: 0xACC */
       uint8_t RESERVED_29[8];
  __IO uint32_t CIU_CPU1_CPU2_FW_DWLD_CTRL;        /**< CPUs FW dwld control register, offset: 0xAD8 */
  __IO uint32_t CIU_CPU_COMM0;                     /**< CPU Communication reserved0, offset: 0xADC */
  __IO uint32_t CIU_CPU_COMM1;                     /**< CPU Communication reserved1, offset: 0xAE0 */
       uint8_t RESERVED_30[88];
  __IO uint32_t CIU_CPU_ECO_CTRL;                  /**< CPU ECO control, offset: 0xB3C */
       uint8_t RESERVED_31[32];
  __IO uint32_t CIU_RFU_CTRL;                      /**< RFU Control and Status, offset: 0xB60 */
  __IO uint32_t CIU_RFU_EXTRA_PORT;                /**< RFU Extra Port Connection, offset: 0xB64 */
       uint8_t RESERVED_32[20];
  __IO uint32_t CIU_RFU_ECO_CTRL;                  /**< RFU ECO Control, offset: 0xB7C */
       uint8_t RESERVED_33[128];
  __IO uint32_t CIU_MCI_EXTRA;                     /**< MCI EXTRA Ports, offset: 0xC00 */
       uint8_t RESERVED_34[252];
  __IO uint32_t CIU_BBUD_CTRL;                     /**< BBUD Control Register, offset: 0xD00 */
  __IO uint32_t CIU_BBUD_EXTRA_PORT;               /**< BBUD Extra Port Connection, offset: 0xD04 */
  __IO uint32_t CIU_BBUD_PTP_INTR_CTRL;            /**< PTP Input capture interrupt control, offset: 0xD08 */
       uint8_t RESERVED_35[112];
  __IO uint32_t CIU_BBUD_ECO_CTRL;                 /**< BBUD ECO Control, offset: 0xD7C */
  __IO uint32_t CIU_AIU_CTRL;                      /**< AIU Control Register, offset: 0xD80 */
       uint8_t RESERVED_36[120];
  __IO uint32_t CIU_AIU_ECO_CTRL;                  /**< AIU ECO Control, offset: 0xDFC */
       uint8_t RESERVED_37[252];
  __IO uint32_t CIU_CBU_ECO_CTRL;                  /**< CBU ECO Control, offset: 0xEFC */
       uint8_t RESERVED_38[1829120];
  __IO uint32_t CIU1_IMU_CPU1_WR_MSG_TO_CPU3;      /**< CPU1(WL) write message to CPU3(MCI) (0xAF0-0xAF4 IMU register access by CPU1), offset: 0x1BF800 */
  __I  uint32_t CIU1_IMU_CPU1_RD_MSG_FROM_CPU3;    /**< cpu1 read message from CPU3, offset: 0x1BF804 */
  __I  uint32_t CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS; /**< cpu1 to CPU3 message FIFO status, offset: 0x1BF808 */
  __IO uint32_t CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL;  /**< cpu1 to CPU3 message FIFO control, offset: 0x1BF80C */
  __I  uint32_t CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG; /**< CPU3 last message read (from cpu1), offset: 0x1BF810 */
       uint8_t RESERVED_39[12];
  __IO uint32_t CIU1_IMU_CPU3_WR_MSG_TO_CPU1;      /**< CPU3 write message to cpu1 (0xB04-0xB14 IMU register access by CPU3), offset: 0x1BF820 */
  __I  uint32_t CIU1_IMU_CPU3_RD_MSG_FROM_CPU1;    /**< CPU3 read message from cpu1, offset: 0x1BF824 */
  __I  uint32_t CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS; /**< CPU3 to cpu1 message FIFO status, offset: 0x1BF828 */
  __IO uint32_t CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL;  /**< CPU3 to cpu1 message FIFO control, offset: 0x1BF82C */
  __I  uint32_t CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG; /**< cpu1 last message read (from cpu3), offset: 0x1BF830 */
  __IO uint32_t CIU_CPU1_CPU3_MSG_CTRL;            /**< CPU1_CPU3 message register, offset: 0x1BF834 */
  __IO uint32_t CIU1_CPU3_WAKEUP_CTRL;             /**< CIU1 register to wakeup CPU3, offset: 0x1BF838 */
  __IO uint32_t CIU1_CPU1_WAKEUP_DONE;             /**< Wakeup done Control Register to CPU3, offset: 0x1BF83C */
  __IO uint32_t CIU1_CPU3_NS_GP_INT;               /**< Non Secure region GP interrupt to CPU3, offset: 0x1BF840 */
       uint8_t RESERVED_40[184];
  __IO uint32_t CIU_IMU_ECO_BITS;                  /**< IMU ECO Control, offset: 0x1BF8FC */
} CIU1_Type;

/* ----------------------------------------------------------------------------
   -- CIU1 Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CIU1_Register_Masks CIU1 Register Masks
 * @{
 */

/*! @name CIU_CHIP_REV - CIU Revision */
/*! @{ */

#define CIU1_CIU_CHIP_REV_CIU_IP_REVISION_MASK   (0xFFFFU)
#define CIU1_CIU_CHIP_REV_CIU_IP_REVISION_SHIFT  (0U)
/*! CIU_IP_REVISION - CIU Revision Number */
#define CIU1_CIU_CHIP_REV_CIU_IP_REVISION(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CHIP_REV_CIU_IP_REVISION_SHIFT)) & CIU1_CIU_CHIP_REV_CIU_IP_REVISION_MASK)
/*! @} */

/*! @name CIU_CHIP_ECO_CTRL - Chip ECO Control */
/*! @{ */

#define CIU1_CIU_CHIP_ECO_CTRL_ECO_BITS_MASK     (0xFFFFFFFFU)
#define CIU1_CIU_CHIP_ECO_CTRL_ECO_BITS_SHIFT    (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_CHIP_ECO_CTRL_ECO_BITS(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CHIP_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_CHIP_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_CLK_ENABLE - Clock Enable */
/*! @{ */

#define CIU1_CIU_CLK_ENABLE_BIST_AHB1_CLK_GATING_EN_MASK (0x1U)
#define CIU1_CIU_CLK_ENABLE_BIST_AHB1_CLK_GATING_EN_SHIFT (0U)
/*! BIST_AHB1_CLK_GATING_EN - bist Clock gating for IMEM/DMEM/SMU1/ROM */
#define CIU1_CIU_CLK_ENABLE_BIST_AHB1_CLK_GATING_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_BIST_AHB1_CLK_GATING_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_BIST_AHB1_CLK_GATING_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CPU1_GATEHCLK_EN_MASK (0x8U)
#define CIU1_CIU_CLK_ENABLE_CPU1_GATEHCLK_EN_SHIFT (3U)
/*! CPU1_GATEHCLK_EN - CPU1 Gate HCLK Control Feature */
#define CIU1_CIU_CLK_ENABLE_CPU1_GATEHCLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CPU1_GATEHCLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CPU1_GATEHCLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CPU1_FABRIC_CLK_EN_MASK (0x10U)
#define CIU1_CIU_CLK_ENABLE_CPU1_FABRIC_CLK_EN_SHIFT (4U)
/*! CPU1_FABRIC_CLK_EN - CPU1 Fabric Clock Control Feature */
#define CIU1_CIU_CLK_ENABLE_CPU1_FABRIC_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CPU1_FABRIC_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CPU1_FABRIC_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CPU1_MEM_SLV_CLK_EN_MASK (0x20U)
#define CIU1_CIU_CLK_ENABLE_CPU1_MEM_SLV_CLK_EN_SHIFT (5U)
/*! CPU1_MEM_SLV_CLK_EN - CPU1 Memory Slave Clock Control Feature */
#define CIU1_CIU_CLK_ENABLE_CPU1_MEM_SLV_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CPU1_MEM_SLV_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CPU1_MEM_SLV_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_SMU1_DYN_CLK_GATING_DIS_MASK (0x40U)
#define CIU1_CIU_CLK_ENABLE_SMU1_DYN_CLK_GATING_DIS_SHIFT (6U)
/*! SMU1_DYN_CLK_GATING_DIS - SMU1 Dynamic Clock Gating Feature */
#define CIU1_CIU_CLK_ENABLE_SMU1_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_SMU1_DYN_CLK_GATING_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE_SMU1_DYN_CLK_GATING_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE_WLRTU1_AHB_CLK_EN_MASK (0x100U)
#define CIU1_CIU_CLK_ENABLE_WLRTU1_AHB_CLK_EN_SHIFT (8U)
/*! WLRTU1_AHB_CLK_EN - WLRTU1 AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_WLRTU1_AHB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_WLRTU1_AHB_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_WLRTU1_AHB_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_SMU1_AHB_CLK_EN_MASK (0x200U)
#define CIU1_CIU_CLK_ENABLE_SMU1_AHB_CLK_EN_SHIFT (9U)
/*! SMU1_AHB_CLK_EN - SMU1 AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_SMU1_AHB_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_SMU1_AHB_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_SMU1_AHB_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_IPS_HCLK_EN_MASK     (0x400U)
#define CIU1_CIU_CLK_ENABLE_IPS_HCLK_EN_SHIFT    (10U)
/*! IPS_HCLK_EN - IPS AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_IPS_HCLK_EN(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_IPS_HCLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_IPS_HCLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_LBU1_AHB_CLK_EN_MASK (0x800U)
#define CIU1_CIU_CLK_ENABLE_LBU1_AHB_CLK_EN_SHIFT (11U)
/*! LBU1_AHB_CLK_EN - PBU AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_LBU1_AHB_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_LBU1_AHB_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_LBU1_AHB_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_AHB_CLK_EN_MASK (0x1000U)
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_AHB_CLK_EN_SHIFT (12U)
/*! CIU_WLAPU_AHB_CLK_EN - APU AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_AHB_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CIU_WLAPU_AHB_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CIU_WLAPU_AHB_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_SLPCLK_EN_MASK (0x2000U)
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_SLPCLK_EN_SHIFT (13U)
/*! CIU_WLAPU_SLPCLK_EN - APU Sleep Clock Enable */
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_SLPCLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CIU_WLAPU_SLPCLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CIU_WLAPU_SLPCLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_CAL_CLK_EN_MASK (0x4000U)
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_CAL_CLK_EN_SHIFT (14U)
/*! CIU_WLAPU_CAL_CLK_EN - APU Calibration Clock Enable */
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_CAL_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CIU_WLAPU_CAL_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CIU_WLAPU_CAL_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_REF_CLK_EN_MASK (0x8000U)
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_REF_CLK_EN_SHIFT (15U)
/*! CIU_WLAPU_REF_CLK_EN - APU Reference Clock Enable */
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_REF_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CIU_WLAPU_REF_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CIU_WLAPU_REF_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_IPS_CLK_EN_MASK      (0x10000U)
#define CIU1_CIU_CLK_ENABLE_IPS_CLK_EN_SHIFT     (16U)
/*! IPS_CLK_EN - Enable for ips functional clock */
#define CIU1_CIU_CLK_ENABLE_IPS_CLK_EN(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_IPS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_IPS_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_IPS_PCLK_EN_MASK     (0x20000U)
#define CIU1_CIU_CLK_ENABLE_IPS_PCLK_EN_SHIFT    (17U)
/*! IPS_PCLK_EN - Enable for ips apb clock */
#define CIU1_CIU_CLK_ENABLE_IPS_PCLK_EN(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_IPS_PCLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_IPS_PCLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_MCI_LITE2AHB_HCLK_EN_MASK (0x40000U)
#define CIU1_CIU_CLK_ENABLE_MCI_LITE2AHB_HCLK_EN_SHIFT (18U)
/*! MCI_LITE2AHB_HCLK_EN - Enable for mci lite2ahb module in wl_top */
#define CIU1_CIU_CLK_ENABLE_MCI_LITE2AHB_HCLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_MCI_LITE2AHB_HCLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_MCI_LITE2AHB_HCLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_LBU1_LSBIF_CLK_EN_MASK (0x100000U)
#define CIU1_CIU_CLK_ENABLE_LBU1_LSBIF_CLK_EN_SHIFT (20U)
/*! LBU1_LSBIF_CLK_EN - PBU Device Clock (PCLK) Enable */
#define CIU1_CIU_CLK_ENABLE_LBU1_LSBIF_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_LBU1_LSBIF_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_LBU1_LSBIF_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_EU1_CORE_CLK_EN_MASK (0x400000U)
#define CIU1_CIU_CLK_ENABLE_EU1_CORE_CLK_EN_SHIFT (22U)
/*! EU1_CORE_CLK_EN - EU1 Core Clock Enable */
#define CIU1_CIU_CLK_ENABLE_EU1_CORE_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_EU1_CORE_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_EU1_CORE_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_MCU1_AHB_CLK_EN_MASK (0x800000U)
#define CIU1_CIU_CLK_ENABLE_MCU1_AHB_CLK_EN_SHIFT (23U)
/*! MCU1_AHB_CLK_EN - MCU1 AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_MCU1_AHB_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_MCU1_AHB_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_MCU1_AHB_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_WL_AHB2APB_HCLK_EN_MASK (0x2000000U)
#define CIU1_CIU_CLK_ENABLE_WL_AHB2APB_HCLK_EN_SHIFT (25U)
/*! WL_AHB2APB_HCLK_EN - Wireless AHB to APB Clock Enable */
#define CIU1_CIU_CLK_ENABLE_WL_AHB2APB_HCLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_WL_AHB2APB_HCLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_WL_AHB2APB_HCLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_MCU1_SYS_CLK_EN_MASK (0x4000000U)
#define CIU1_CIU_CLK_ENABLE_MCU1_SYS_CLK_EN_SHIFT (26U)
/*! MCU1_SYS_CLK_EN - MCU1 System Clock Enable */
#define CIU1_CIU_CLK_ENABLE_MCU1_SYS_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_MCU1_SYS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_MCU1_SYS_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_DVFS_CLK_EN_MASK (0x80000000U)
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_DVFS_CLK_EN_SHIFT (31U)
/*! CIU_WLAPU_DVFS_CLK_EN - APU DVFS Clock Enable */
#define CIU1_CIU_CLK_ENABLE_CIU_WLAPU_DVFS_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE_CIU_WLAPU_DVFS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE_CIU_WLAPU_DVFS_CLK_EN_MASK)
/*! @} */

/*! @name CIU_CLK_ENABLE2 - Clock enable2 */
/*! @{ */

#define CIU1_CIU_CLK_ENABLE2_IPS_PROM_ADDR_MASK_DIS_MASK (0x1U)
#define CIU1_CIU_CLK_ENABLE2_IPS_PROM_ADDR_MASK_DIS_SHIFT (0U)
/*! IPS_PROM_ADDR_MASK_DIS - ips prom ROM Address Mask Selection */
#define CIU1_CIU_CLK_ENABLE2_IPS_PROM_ADDR_MASK_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_IPS_PROM_ADDR_MASK_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE2_IPS_PROM_ADDR_MASK_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE2_IPS_PROM_DYN_CLK_DIS_MASK (0x2U)
#define CIU1_CIU_CLK_ENABLE2_IPS_PROM_DYN_CLK_DIS_SHIFT (1U)
/*! IPS_PROM_DYN_CLK_DIS - ips ROM Dynamic Clock Gating Feature */
#define CIU1_CIU_CLK_ENABLE2_IPS_PROM_DYN_CLK_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_IPS_PROM_DYN_CLK_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE2_IPS_PROM_DYN_CLK_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE2_BR_AHB1_CLK_EN_MASK (0x1F0000U)
#define CIU1_CIU_CLK_ENABLE2_BR_AHB1_CLK_EN_SHIFT (16U)
/*! BR_AHB1_CLK_EN - BRU AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE2_BR_AHB1_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_BR_AHB1_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE2_BR_AHB1_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE2_WEU_AHB_CLK_EN_MASK (0x4000000U)
#define CIU1_CIU_CLK_ENABLE2_WEU_AHB_CLK_EN_SHIFT (26U)
/*! WEU_AHB_CLK_EN - WEU AHB Clock Enable */
#define CIU1_CIU_CLK_ENABLE2_WEU_AHB_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_WEU_AHB_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE2_WEU_AHB_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE2_WEU_SYS_CLK_EN_MASK (0x8000000U)
#define CIU1_CIU_CLK_ENABLE2_WEU_SYS_CLK_EN_SHIFT (27U)
/*! WEU_SYS_CLK_EN - WEU SYS Clock Enable */
#define CIU1_CIU_CLK_ENABLE2_WEU_SYS_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_WEU_SYS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE2_WEU_SYS_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE2_HPU1_CIU_CLK_EN_MASK (0x10000000U)
#define CIU1_CIU_CLK_ENABLE2_HPU1_CIU_CLK_EN_SHIFT (28U)
/*! HPU1_CIU_CLK_EN - HPU1 CIU Clock Enable */
#define CIU1_CIU_CLK_ENABLE2_HPU1_CIU_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_HPU1_CIU_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE2_HPU1_CIU_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE2_SMU1_TOP_CLK_EN_MASK (0x20000000U)
#define CIU1_CIU_CLK_ENABLE2_SMU1_TOP_CLK_EN_SHIFT (29U)
/*! SMU1_TOP_CLK_EN - SMU1 Top Clock Enable */
#define CIU1_CIU_CLK_ENABLE2_SMU1_TOP_CLK_EN(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_SMU1_TOP_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE2_SMU1_TOP_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE2_SMU1_PORT0_SYS_CLK_EN_MASK (0x40000000U)
#define CIU1_CIU_CLK_ENABLE2_SMU1_PORT0_SYS_CLK_EN_SHIFT (30U)
/*! SMU1_PORT0_SYS_CLK_EN - SMU1 Port 0 System Clock Enable */
#define CIU1_CIU_CLK_ENABLE2_SMU1_PORT0_SYS_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE2_SMU1_PORT0_SYS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE2_SMU1_PORT0_SYS_CLK_EN_MASK)
/*! @} */

/*! @name CIU_CLK_ENABLE3 - Clock Enable 3 */
/*! @{ */

#define CIU1_CIU_CLK_ENABLE3_WL_MSC_A2A_CLK_EN_MASK (0x1U)
#define CIU1_CIU_CLK_ENABLE3_WL_MSC_A2A_CLK_EN_SHIFT (0U)
/*! WL_MSC_A2A_CLK_EN - Enable Clock for MSC A2A */
#define CIU1_CIU_CLK_ENABLE3_WL_MSC_A2A_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_WL_MSC_A2A_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE3_WL_MSC_A2A_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE3_WL_SOC_A2A_CLK_EN_MASK (0x2U)
#define CIU1_CIU_CLK_ENABLE3_WL_SOC_A2A_CLK_EN_SHIFT (1U)
/*! WL_SOC_A2A_CLK_EN - Enable Clock for SOC A2A */
#define CIU1_CIU_CLK_ENABLE3_WL_SOC_A2A_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_WL_SOC_A2A_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE3_WL_SOC_A2A_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE3_WLRTU1_CLK_EN_MASK  (0x40U)
#define CIU1_CIU_CLK_ENABLE3_WLRTU1_CLK_EN_SHIFT (6U)
/*! WLRTU1_CLK_EN - Enable WL RTU1 timer clock */
#define CIU1_CIU_CLK_ENABLE3_WLRTU1_CLK_EN(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_WLRTU1_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE3_WLRTU1_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE3_BRU_AHB1_ADDR_MASK_DIS_MASK (0x1000U)
#define CIU1_CIU_CLK_ENABLE3_BRU_AHB1_ADDR_MASK_DIS_SHIFT (12U)
/*! BRU_AHB1_ADDR_MASK_DIS - CPU1 ROM Address Mask Selection */
#define CIU1_CIU_CLK_ENABLE3_BRU_AHB1_ADDR_MASK_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_BRU_AHB1_ADDR_MASK_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE3_BRU_AHB1_ADDR_MASK_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE3_BRU_AHB1_DYN_CLK_GATING_DIS_MASK (0x2000U)
#define CIU1_CIU_CLK_ENABLE3_BRU_AHB1_DYN_CLK_GATING_DIS_SHIFT (13U)
/*! BRU_AHB1_DYN_CLK_GATING_DIS - CPU1 ROM Dynamic Clock Gating Feature */
#define CIU1_CIU_CLK_ENABLE3_BRU_AHB1_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_BRU_AHB1_DYN_CLK_GATING_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE3_BRU_AHB1_DYN_CLK_GATING_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE3_ITCM_AHB1_DYN_CLK_GATING_DIS_MASK (0x4000U)
#define CIU1_CIU_CLK_ENABLE3_ITCM_AHB1_DYN_CLK_GATING_DIS_SHIFT (14U)
/*! ITCM_AHB1_DYN_CLK_GATING_DIS - CPU1 ITCM/IMEM Dynamic Clock Gating Feature */
#define CIU1_CIU_CLK_ENABLE3_ITCM_AHB1_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_ITCM_AHB1_DYN_CLK_GATING_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE3_ITCM_AHB1_DYN_CLK_GATING_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE3_DTCM_AHB1_DYN_CLK_GATING_DIS_MASK (0x8000U)
#define CIU1_CIU_CLK_ENABLE3_DTCM_AHB1_DYN_CLK_GATING_DIS_SHIFT (15U)
/*! DTCM_AHB1_DYN_CLK_GATING_DIS - CPU1 DTCM/DMEM Dynamic Clock Gating Feature */
#define CIU1_CIU_CLK_ENABLE3_DTCM_AHB1_DYN_CLK_GATING_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_DTCM_AHB1_DYN_CLK_GATING_DIS_SHIFT)) & CIU1_CIU_CLK_ENABLE3_DTCM_AHB1_DYN_CLK_GATING_DIS_MASK)

#define CIU1_CIU_CLK_ENABLE3_SMU1_BANK_SEG_CLK_EN_MASK (0xFF0000U)
#define CIU1_CIU_CLK_ENABLE3_SMU1_BANK_SEG_CLK_EN_SHIFT (16U)
/*! SMU1_BANK_SEG_CLK_EN - SMU1 bank segment Clock Enable */
#define CIU1_CIU_CLK_ENABLE3_SMU1_BANK_SEG_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_SMU1_BANK_SEG_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE3_SMU1_BANK_SEG_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE3_MCI_A2A_CLK_EN_MASK (0x40000000U)
#define CIU1_CIU_CLK_ENABLE3_MCI_A2A_CLK_EN_SHIFT (30U)
/*! MCI_A2A_CLK_EN - Enable Clock for MCI A2A */
#define CIU1_CIU_CLK_ENABLE3_MCI_A2A_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_MCI_A2A_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE3_MCI_A2A_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE3_SMU1_MEM_SYS_CLK_EN_MASK (0x80000000U)
#define CIU1_CIU_CLK_ENABLE3_SMU1_MEM_SYS_CLK_EN_SHIFT (31U)
/*! SMU1_MEM_SYS_CLK_EN - SMU1 mem banks and peripheral control logic Clock Enable */
#define CIU1_CIU_CLK_ENABLE3_SMU1_MEM_SYS_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE3_SMU1_MEM_SYS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE3_SMU1_MEM_SYS_CLK_EN_MASK)
/*! @} */

/*! @name CIU_CLK_CPU1CLK_CTRL - CPU1_AHB1 Clock Control */
/*! @{ */

#define CIU1_CIU_CLK_CPU1CLK_CTRL_T1_FREQ_SEL_MASK (0xFU)
#define CIU1_CIU_CLK_CPU1CLK_CTRL_T1_FREQ_SEL_SHIFT (0U)
/*! T1_FREQ_SEL - CPU1 Clock Frequency Select */
#define CIU1_CIU_CLK_CPU1CLK_CTRL_T1_FREQ_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CPU1CLK_CTRL_T1_FREQ_SEL_SHIFT)) & CIU1_CIU_CLK_CPU1CLK_CTRL_T1_FREQ_SEL_MASK)
/*! @} */

/*! @name CIU_CLK_SYSCLK_CTRL - SYS Clock Control */
/*! @{ */

#define CIU1_CIU_CLK_SYSCLK_CTRL_T1_FREQ_SEL_MASK (0xFU)
#define CIU1_CIU_CLK_SYSCLK_CTRL_T1_FREQ_SEL_SHIFT (0U)
/*! T1_FREQ_SEL - Sys Clock Frequency Select */
#define CIU1_CIU_CLK_SYSCLK_CTRL_T1_FREQ_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SYSCLK_CTRL_T1_FREQ_SEL_SHIFT)) & CIU1_CIU_CLK_SYSCLK_CTRL_T1_FREQ_SEL_MASK)
/*! @} */

/*! @name CIU_CLK_RTU_NCO_CTRL - RTU NCO Clock Control */
/*! @{ */

#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_REF_CLK_SEL_MASK (0x1U)
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_REF_CLK_SEL_SHIFT (0U)
/*! CIU_WLRTU_REF_CLK_SEL - RTU Reference Clock from UART reference clock tree */
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_REF_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_REF_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_REF_CLK_SEL_MASK)

#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_OUT_SEL_MASK (0x2U)
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_OUT_SEL_SHIFT (1U)
/*! CIU_WLRTU_NCO_OUT_SEL - RTU NCO Mode Select (Reference Clock Based) */
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_OUT_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_OUT_SEL_SHIFT)) & CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_OUT_SEL_MASK)

#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_ENABLE_MASK (0x4U)
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_ENABLE_SHIFT (2U)
/*! CIU_WLRTU_NCO_ENABLE - RTU NCO Enable (Reference Clock Based) */
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_ENABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_ENABLE_SHIFT)) & CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_ENABLE_MASK)

#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_STEP_MASK (0xFFFF0000U)
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_STEP_SHIFT (16U)
/*! CIU_WLRTU_NCO_STEP - Step size for RTU clock NCO (Reference Clock Based) */
#define CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_STEP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_STEP_SHIFT)) & CIU1_CIU_CLK_RTU_NCO_CTRL_CIU_WLRTU_NCO_STEP_MASK)
/*! @} */

/*! @name CIU_CLK_LBU1_WLRTU_CTRL - LBU1 WLRTU1 Clock Control */
/*! @{ */

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_USE_REFCLK_MASK (0x2U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_USE_REFCLK_SHIFT (1U)
/*! LBU1_USE_REFCLK - Static bit set by FW based on Reference Clock Frequency. If reference clock
 *    frequency is lower and LBU can not support high baud rate of UART, then FW will set
 *    soc_use_ref_mode = 0. This is an indication for Bluetooth subsystem that there is some IP which need PLL
 *    to function which is LBU in this case.
 */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_USE_REFCLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_USE_REFCLK_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_USE_REFCLK_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_EN_MASK (0x4U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_EN_SHIFT (2U)
/*! LBU1_CLK_SCALE_EN - Enable lbu1 lsb clock div (upto div by 3/4/5/6 only) */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_EN_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_EN_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_FACTOR_MASK (0x38U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_FACTOR_SHIFT (3U)
/*! LBU1_CLK_SCALE_FACTOR - lbu1 lsb clock divider value[2:0] */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_FACTOR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_FACTOR_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_CLK_SCALE_FACTOR_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_HIGH_BAUD_SEL_MASK (0x40U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_HIGH_BAUD_SEL_SHIFT (6U)
/*! LBU1_HIGH_BAUD_SEL - PBU Bus Reference Clock */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_HIGH_BAUD_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_HIGH_BAUD_SEL_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_HIGH_BAUD_SEL_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_DIV_BY_2_SEL_MASK (0x80U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_DIV_BY_2_SEL_SHIFT (7U)
/*! LBU1_DIV_BY_2_SEL - PBU Bus Reference Clock */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_DIV_BY_2_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_DIV_BY_2_SEL_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_LBU1_DIV_BY_2_SEL_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_TIMER1_USE_SLP_CLK_MASK (0x800U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_TIMER1_USE_SLP_CLK_SHIFT (11U)
/*! WLRTU1_TIMER1_USE_SLP_CLK - Timer 1 WL_RTU1 Clock */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_TIMER1_USE_SLP_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_TIMER1_USE_SLP_CLK_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_TIMER1_USE_SLP_CLK_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_USE_REF_CLK_MASK (0x1000U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_USE_REF_CLK_SHIFT (12U)
/*! WLRTU1_USE_REF_CLK - Static bit set by FW. If it is required that timers need not be programmed
 *    with dynamic switching of T1/Reference, the WL_RTU1 source clock is set on reference clock so
 *    that the timer are not disturbed.
 */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_USE_REF_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_USE_REF_CLK_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_USE_REF_CLK_MASK)

#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_DBG_CLK_CTRL_MASK (0x8000U)
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_DBG_CLK_CTRL_SHIFT (15U)
/*! WLRTU1_DBG_CLK_CTRL - WLRTU1 Debug Clock Control Feature */
#define CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_DBG_CLK_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_DBG_CLK_CTRL_SHIFT)) & CIU1_CIU_CLK_LBU1_WLRTU_CTRL_WLRTU1_DBG_CLK_CTRL_MASK)
/*! @} */

/*! @name CIU_CLK_SOCCLK_CTRL - SOC Clock Control */
/*! @{ */

#define CIU1_CIU_CLK_SOCCLK_CTRL_EU1_CORE_CLK_SEL_MASK (0x3U)
#define CIU1_CIU_CLK_SOCCLK_CTRL_EU1_CORE_CLK_SEL_SHIFT (0U)
/*! EU1_CORE_CLK_SEL - EU1 Core Clock Select */
#define CIU1_CIU_CLK_SOCCLK_CTRL_EU1_CORE_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SOCCLK_CTRL_EU1_CORE_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_SOCCLK_CTRL_EU1_CORE_CLK_SEL_MASK)

#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CONST_CAL_CLK_SEL_MASK (0x400U)
#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CONST_CAL_CLK_SEL_SHIFT (10U)
/*! CIU_WLAPU_CONST_CAL_CLK_SEL - PMU Constant Calibration Clock Select */
#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CONST_CAL_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CONST_CAL_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CONST_CAL_CLK_SEL_MASK)

#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CAL_CLK_SEL_MASK (0x800U)
#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CAL_CLK_SEL_SHIFT (11U)
/*! CIU_WLAPU_CAL_CLK_SEL - PMU Calibration Clock */
#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CAL_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CAL_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_SOCCLK_CTRL_CIU_WLAPU_CAL_CLK_SEL_MASK)

#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_MASK (0x4000U)
#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_SHIFT (14U)
/*! CIU_USE_REFCLK - SoC_Clk Clock */
#define CIU1_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_SHIFT)) & CIU1_CIU_CLK_SOCCLK_CTRL_CIU_USE_REFCLK_MASK)

#define CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_PCLK_DIV_SEL_MASK (0xF0000U)
#define CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_PCLK_DIV_SEL_SHIFT (16U)
/*! WL_AHB2APB_PCLK_DIV_SEL - WL AHB2APB PCLK Divider Select */
#define CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_PCLK_DIV_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_PCLK_DIV_SEL_SHIFT)) & CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_PCLK_DIV_SEL_MASK)

#define CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_WAIT_CYCLES_MASK (0xF0000000U)
#define CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_WAIT_CYCLES_SHIFT (28U)
/*! WL_AHB2APB_WAIT_CYCLES - WL AHB2APB Wait Cycles between each APB transaction */
#define CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_WAIT_CYCLES(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_WAIT_CYCLES_SHIFT)) & CIU1_CIU_CLK_SOCCLK_CTRL_WL_AHB2APB_WAIT_CYCLES_MASK)
/*! @} */

/*! @name CIU_CLK_SLEEPCLK_CTRL - Sleep Clock Control */
/*! @{ */

#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_MASK (0xFFFFFFU)
#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_SHIFT (0U)
/*! CIU_SLEEP_CLK_NCO_MVAL - Sleep Clock NCO */
#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_SHIFT)) & CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_SLEEP_CLK_NCO_MVAL_MASK)

#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_MASK (0x2000000U)
#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_SHIFT (25U)
/*! CIU_NCO_SLEEP_CLK_SEL - NCO Sleep Clock Select */
#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_NCO_SLEEP_CLK_SEL_MASK)

#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_MASK (0x10000000U)
#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_SHIFT (28U)
/*! CIU_REFCLK_SLEEP_CLK_SEL - Reference Clock Sleep Clock Select */
#define CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_SLEEPCLK_CTRL_CIU_REFCLK_SLEEP_CLK_SEL_MASK)
/*! @} */

/*! @name CIU_CLK_CP15_DIS1 - Clock Auto Shut-off Enable1 */
/*! @{ */

#define CIU1_CIU_CLK_CP15_DIS1_WLAN_HCLK_MASK    (0x40000U)
#define CIU1_CIU_CLK_CP15_DIS1_WLAN_HCLK_SHIFT   (18U)
/*! WLAN_HCLK - WLAN ahb Arbiter/Decoder Shut Off */
#define CIU1_CIU_CLK_CP15_DIS1_WLAN_HCLK(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CP15_DIS1_WLAN_HCLK_SHIFT)) & CIU1_CIU_CLK_CP15_DIS1_WLAN_HCLK_MASK)

#define CIU1_CIU_CLK_CP15_DIS1_CP15_DIS_WLAPU_AHB_CLK_MASK (0x100000U)
#define CIU1_CIU_CLK_CP15_DIS1_CP15_DIS_WLAPU_AHB_CLK_SHIFT (20U)
/*! CP15_DIS_WLAPU_AHB_CLK - APU Shut Off */
#define CIU1_CIU_CLK_CP15_DIS1_CP15_DIS_WLAPU_AHB_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CP15_DIS1_CP15_DIS_WLAPU_AHB_CLK_SHIFT)) & CIU1_CIU_CLK_CP15_DIS1_CP15_DIS_WLAPU_AHB_CLK_MASK)
/*! @} */

/*! @name CIU_CLK_CP15_DIS2 - Clock Auto Shut-off Enable2 */
/*! @{ */

#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_BROM_CLK_MASK (0x1FU)
#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_BROM_CLK_SHIFT (0U)
/*! CP15_DIS_CPU1_BROM_CLK - APU Shut Off */
#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_BROM_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_BROM_CLK_SHIFT)) & CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_BROM_CLK_MASK)

#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_DMEM_CLK_MASK (0x300U)
#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_DMEM_CLK_SHIFT (8U)
/*! CP15_DIS_CPU1_DMEM_CLK - APU Shut Off */
#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_DMEM_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_DMEM_CLK_SHIFT)) & CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_DMEM_CLK_MASK)

#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_IMEM_CLK_MASK (0x1F0000U)
#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_IMEM_CLK_SHIFT (16U)
/*! CP15_DIS_CPU1_IMEM_CLK - APU Shut Off 0 = disable this auto shut off feature 1 = AHB clock for
 *    the IMEM is automatically shut off while CPU is asleep
 */
#define CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_IMEM_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_IMEM_CLK_SHIFT)) & CIU1_CIU_CLK_CP15_DIS2_CP15_DIS_CPU1_IMEM_CLK_MASK)
/*! @} */

/*! @name CIU_CLK_CP15_DIS3 - Clock Auto Shut-off Enable3 */
/*! @{ */

#define CIU1_CIU_CLK_CP15_DIS3_CP15_DIS_WEU_AHB_CLK_MASK (0x100000U)
#define CIU1_CIU_CLK_CP15_DIS3_CP15_DIS_WEU_AHB_CLK_SHIFT (20U)
/*! CP15_DIS_WEU_AHB_CLK - TKIP/WEP WEU AHB Shut Off */
#define CIU1_CIU_CLK_CP15_DIS3_CP15_DIS_WEU_AHB_CLK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_CP15_DIS3_CP15_DIS_WEU_AHB_CLK_SHIFT)) & CIU1_CIU_CLK_CP15_DIS3_CP15_DIS_WEU_AHB_CLK_MASK)
/*! @} */

/*! @name CIU_CLK_ENABLE6 - Clock Enable 6 */
/*! @{ */

#define CIU1_CIU_CLK_ENABLE6_CAU_SIF_CLK_SEL_MASK (0x100U)
#define CIU1_CIU_CLK_ENABLE6_CAU_SIF_CLK_SEL_SHIFT (8U)
/*! CAU_SIF_CLK_SEL - CAU SIF Clock Select */
#define CIU1_CIU_CLK_ENABLE6_CAU_SIF_CLK_SEL(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE6_CAU_SIF_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_ENABLE6_CAU_SIF_CLK_SEL_MASK)

#define CIU1_CIU_CLK_ENABLE6_CAU_SIF_AHB1_CLK_EN_MASK (0x200U)
#define CIU1_CIU_CLK_ENABLE6_CAU_SIF_AHB1_CLK_EN_SHIFT (9U)
/*! CAU_SIF_AHB1_CLK_EN - CAU AHB2SIF AHB clock enable */
#define CIU1_CIU_CLK_ENABLE6_CAU_SIF_AHB1_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE6_CAU_SIF_AHB1_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE6_CAU_SIF_AHB1_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE6_GPS_PPS_CLK_EN_MASK (0x400U)
#define CIU1_CIU_CLK_ENABLE6_GPS_PPS_CLK_EN_SHIFT (10U)
/*! GPS_PPS_CLK_EN - GPS PPS Clock Enable */
#define CIU1_CIU_CLK_ENABLE6_GPS_PPS_CLK_EN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE6_GPS_PPS_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE6_GPS_PPS_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE6_MAC1_G2BIST_CLK_EN_MASK (0x800U)
#define CIU1_CIU_CLK_ENABLE6_MAC1_G2BIST_CLK_EN_SHIFT (11U)
/*! MAC1_G2BIST_CLK_EN - Clock Enable for MAC1 BIST Controller Clock */
#define CIU1_CIU_CLK_ENABLE6_MAC1_G2BIST_CLK_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE6_MAC1_G2BIST_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE6_MAC1_G2BIST_CLK_EN_MASK)

#define CIU1_CIU_CLK_ENABLE6_BBUD_MAC_CLK_SEL_MASK (0x2000U)
#define CIU1_CIU_CLK_ENABLE6_BBUD_MAC_CLK_SEL_SHIFT (13U)
/*! BBUD_MAC_CLK_SEL - BBUD MAC Clock Select */
#define CIU1_CIU_CLK_ENABLE6_BBUD_MAC_CLK_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE6_BBUD_MAC_CLK_SEL_SHIFT)) & CIU1_CIU_CLK_ENABLE6_BBUD_MAC_CLK_SEL_MASK)

#define CIU1_CIU_CLK_ENABLE6_PTP_CLK_EN_MASK     (0x4000U)
#define CIU1_CIU_CLK_ENABLE6_PTP_CLK_EN_SHIFT    (14U)
/*! PTP_CLK_EN - PTP input capture pulse interrupt clock enable */
#define CIU1_CIU_CLK_ENABLE6_PTP_CLK_EN(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ENABLE6_PTP_CLK_EN_SHIFT)) & CIU1_CIU_CLK_ENABLE6_PTP_CLK_EN_MASK)
/*! @} */

/*! @name CIU_CLK_SLEEPCLK_CTRL2 - Sleep Clock Control 2 */
/*! @{ */

#define CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_MASK (0xFFFFFFU)
#define CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_SHIFT (0U)
/*! CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP - Sleep Clock NCO value for the sleep mode */
#define CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_SHIFT)) & CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_DURING_SLP_MASK)

#define CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_MASK (0x1000000U)
#define CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_SHIFT (24U)
/*! CIU_SLEEP_CLK_NCO_MVAL_BYPASS - Sleep Clock NCO MVAL Bypass Feature */
#define CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_SHIFT)) & CIU1_CIU_CLK_SLEEPCLK_CTRL2_CIU_SLEEP_CLK_NCO_MVAL_BYPASS_MASK)
/*! @} */

/*! @name CIU_MAC_CLK_CTRL - MAC clock CTRL */
/*! @{ */

#define CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_EN_MASK (0x1U)
#define CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_EN_SHIFT (0U)
/*! MAC1_BBUD_BYPASS_EN - Bypass 11J_EN and PUB_SFT from BBUD1 */
#define CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_EN_SHIFT)) & CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_EN_MASK)

#define CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_VAL_MASK (0x6U)
#define CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_VAL_SHIFT (1U)
/*! MAC1_BBUD_BYPASS_VAL - Bypass value */
#define CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_VAL_SHIFT)) & CIU1_CIU_MAC_CLK_CTRL_MAC1_BBUD_BYPASS_VAL_MASK)
/*! @} */

/*! @name CIU_CLK_ECO_CTRL - Clock ECO Control */
/*! @{ */

#define CIU1_CIU_CLK_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_CLK_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_CLK_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CLK_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_CLK_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_RST_SW1 - Software Module Reset */
/*! @{ */

#define CIU1_CIU_RST_SW1_HPU1__MASK              (0x1U)
#define CIU1_CIU_RST_SW1_HPU1__SHIFT             (0U)
/*! HPU1_ - HPU1 Reset */
#define CIU1_CIU_RST_SW1_HPU1_(x)                (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_HPU1__SHIFT)) & CIU1_CIU_RST_SW1_HPU1__MASK)

#define CIU1_CIU_RST_SW1_SMU1_PORT0_SYS_CLK_N_MASK (0x4U)
#define CIU1_CIU_RST_SW1_SMU1_PORT0_SYS_CLK_N_SHIFT (2U)
/*! SMU1_PORT0_SYS_CLK_N - SMU1 port 0 (SYS_Clk) Soft Reset */
#define CIU1_CIU_RST_SW1_SMU1_PORT0_SYS_CLK_N(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_SMU1_PORT0_SYS_CLK_N_SHIFT)) & CIU1_CIU_RST_SW1_SMU1_PORT0_SYS_CLK_N_MASK)

#define CIU1_CIU_RST_SW1_SMU1_MEM_CLK__MASK      (0x8U)
#define CIU1_CIU_RST_SW1_SMU1_MEM_CLK__SHIFT     (3U)
/*! SMU1_MEM_CLK_ - SMU1 bank clock Soft Reset */
#define CIU1_CIU_RST_SW1_SMU1_MEM_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_SMU1_MEM_CLK__SHIFT)) & CIU1_CIU_RST_SW1_SMU1_MEM_CLK__MASK)

#define CIU1_CIU_RST_SW1_SMU1_TM_RST__MASK       (0x80U)
#define CIU1_CIU_RST_SW1_SMU1_TM_RST__SHIFT      (7U)
/*! SMU1_TM_RST_ - SMU1 testmode logic reset */
#define CIU1_CIU_RST_SW1_SMU1_TM_RST_(x)         (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_SMU1_TM_RST__SHIFT)) & CIU1_CIU_RST_SW1_SMU1_TM_RST__MASK)

#define CIU1_CIU_RST_SW1_WEU_SYS_CLK_N_MASK      (0x100U)
#define CIU1_CIU_RST_SW1_WEU_SYS_CLK_N_SHIFT     (8U)
/*! WEU_SYS_CLK_N - WEU sys clock domain soft reset */
#define CIU1_CIU_RST_SW1_WEU_SYS_CLK_N(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WEU_SYS_CLK_N_SHIFT)) & CIU1_CIU_RST_SW1_WEU_SYS_CLK_N_MASK)

#define CIU1_CIU_RST_SW1_WEU_AHB_CLK_N_MASK      (0x200U)
#define CIU1_CIU_RST_SW1_WEU_AHB_CLK_N_SHIFT     (9U)
/*! WEU_AHB_CLK_N - WEU ahb clock domain soft reset */
#define CIU1_CIU_RST_SW1_WEU_AHB_CLK_N(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WEU_AHB_CLK_N_SHIFT)) & CIU1_CIU_RST_SW1_WEU_AHB_CLK_N_MASK)

#define CIU1_CIU_RST_SW1_CIU1_CFG_RST__MASK      (0x400U)
#define CIU1_CIU_RST_SW1_CIU1_CFG_RST__SHIFT     (10U)
/*! CIU1_CFG_RST_ - CIU ahb clock domain Soft Reset */
#define CIU1_CIU_RST_SW1_CIU1_CFG_RST_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_CIU1_CFG_RST__SHIFT)) & CIU1_CIU_RST_SW1_CIU1_CFG_RST__MASK)

#define CIU1_CIU_RST_SW1_CIU1_REGISTER_RST__MASK (0x800U)
#define CIU1_CIU_RST_SW1_CIU1_REGISTER_RST__SHIFT (11U)
/*! CIU1_REGISTER_RST_ - CIU_Reg Module Soft Reset */
#define CIU1_CIU_RST_SW1_CIU1_REGISTER_RST_(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_CIU1_REGISTER_RST__SHIFT)) & CIU1_CIU_RST_SW1_CIU1_REGISTER_RST__MASK)

#define CIU1_CIU_RST_SW1_MCU1_SYS_CLK__MASK      (0x8000U)
#define CIU1_CIU_RST_SW1_MCU1_SYS_CLK__SHIFT     (15U)
/*! MCU1_SYS_CLK_ - MCU1 sysclk domain Soft Reset */
#define CIU1_CIU_RST_SW1_MCU1_SYS_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_MCU1_SYS_CLK__SHIFT)) & CIU1_CIU_RST_SW1_MCU1_SYS_CLK__MASK)

#define CIU1_CIU_RST_SW1_WL_AHB2APB_CLK_N_MASK   (0x10000U)
#define CIU1_CIU_RST_SW1_WL_AHB2APB_CLK_N_SHIFT  (16U)
/*! WL_AHB2APB_CLK_N - WL AHB2APB AHB clock domain reset */
#define CIU1_CIU_RST_SW1_WL_AHB2APB_CLK_N(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WL_AHB2APB_CLK_N_SHIFT)) & CIU1_CIU_RST_SW1_WL_AHB2APB_CLK_N_MASK)

#define CIU1_CIU_RST_SW1_WL_AHB_RST__MASK        (0x80000U)
#define CIU1_CIU_RST_SW1_WL_AHB_RST__SHIFT       (19U)
/*! WL_AHB_RST_ - WL AHB Decoder Mux and Arbiter and CIU AHB intf Soft Reset */
#define CIU1_CIU_RST_SW1_WL_AHB_RST_(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WL_AHB_RST__SHIFT)) & CIU1_CIU_RST_SW1_WL_AHB_RST__MASK)

#define CIU1_CIU_RST_SW1_LBU1__MASK              (0x100000U)
#define CIU1_CIU_RST_SW1_LBU1__SHIFT             (20U)
/*! LBU1_ - LBU1 Soft Reset */
#define CIU1_CIU_RST_SW1_LBU1_(x)                (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_LBU1__SHIFT)) & CIU1_CIU_RST_SW1_LBU1__MASK)

#define CIU1_CIU_RST_SW1_WLAPU_REF_CLK__MASK     (0x200000U)
#define CIU1_CIU_RST_SW1_WLAPU_REF_CLK__SHIFT    (21U)
/*! WLAPU_REF_CLK_ - APU ref Clock Reset */
#define CIU1_CIU_RST_SW1_WLAPU_REF_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WLAPU_REF_CLK__SHIFT)) & CIU1_CIU_RST_SW1_WLAPU_REF_CLK__MASK)

#define CIU1_CIU_RST_SW1_BBU1_DSP__MASK          (0x1000000U)
#define CIU1_CIU_RST_SW1_BBU1_DSP__SHIFT         (24U)
/*! BBU1_DSP_ - BBU1_DSP Reset */
#define CIU1_CIU_RST_SW1_BBU1_DSP_(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_BBU1_DSP__SHIFT)) & CIU1_CIU_RST_SW1_BBU1_DSP__MASK)

#define CIU1_CIU_RST_SW1_RFU1_PR__MASK           (0x2000000U)
#define CIU1_CIU_RST_SW1_RFU1_PR__SHIFT          (25U)
/*! RFU1_PR_ - RFU1_PR Reset */
#define CIU1_CIU_RST_SW1_RFU1_PR_(x)             (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_RFU1_PR__SHIFT)) & CIU1_CIU_RST_SW1_RFU1_PR__MASK)

#define CIU1_CIU_RST_SW1_WLAPU_AHB_CLK__MASK     (0x4000000U)
#define CIU1_CIU_RST_SW1_WLAPU_AHB_CLK__SHIFT    (26U)
/*! WLAPU_AHB_CLK_ - APU ARM Clock Reset */
#define CIU1_CIU_RST_SW1_WLAPU_AHB_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WLAPU_AHB_CLK__SHIFT)) & CIU1_CIU_RST_SW1_WLAPU_AHB_CLK__MASK)

#define CIU1_CIU_RST_SW1_WLAPU_SLP_CLK__MASK     (0x8000000U)
#define CIU1_CIU_RST_SW1_WLAPU_SLP_CLK__SHIFT    (27U)
/*! WLAPU_SLP_CLK_ - APU Sleep Clock domain Reset */
#define CIU1_CIU_RST_SW1_WLAPU_SLP_CLK_(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_WLAPU_SLP_CLK__SHIFT)) & CIU1_CIU_RST_SW1_WLAPU_SLP_CLK__MASK)

#define CIU1_CIU_RST_SW1_MCU1_MCLK__MASK         (0x10000000U)
#define CIU1_CIU_RST_SW1_MCU1_MCLK__SHIFT        (28U)
/*! MCU1_MCLK_ - MCU1 MCLK domain reset */
#define CIU1_CIU_RST_SW1_MCU1_MCLK_(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_MCU1_MCLK__SHIFT)) & CIU1_CIU_RST_SW1_MCU1_MCLK__MASK)

#define CIU1_CIU_RST_SW1_EU1_CORE_CLK__MASK      (0x20000000U)
#define CIU1_CIU_RST_SW1_EU1_CORE_CLK__SHIFT     (29U)
/*! EU1_CORE_CLK_ - EU1 Core Soft Reset */
#define CIU1_CIU_RST_SW1_EU1_CORE_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_EU1_CORE_CLK__SHIFT)) & CIU1_CIU_RST_SW1_EU1_CORE_CLK__MASK)

#define CIU1_CIU_RST_SW1_MCU1_AHB_CLK__MASK      (0x80000000U)
#define CIU1_CIU_RST_SW1_MCU1_AHB_CLK__SHIFT     (31U)
/*! MCU1_AHB_CLK_ - MCU1 AHB Soft Reset */
#define CIU1_CIU_RST_SW1_MCU1_AHB_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW1_MCU1_AHB_CLK__SHIFT)) & CIU1_CIU_RST_SW1_MCU1_AHB_CLK__MASK)
/*! @} */

/*! @name CIU_RST_SW2 - Software Module Reset */
/*! @{ */

#define CIU1_CIU_RST_SW2_W1_CLK__MASK            (0x1U)
#define CIU1_CIU_RST_SW2_W1_CLK__SHIFT           (0U)
/*! W1_CLK_ - W1 Interface (PM chip) Soft Reset */
#define CIU1_CIU_RST_SW2_W1_CLK_(x)              (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_W1_CLK__SHIFT)) & CIU1_CIU_RST_SW2_W1_CLK__MASK)

#define CIU1_CIU_RST_SW2_BRU_AHB1_CLK__MASK      (0x8U)
#define CIU1_CIU_RST_SW2_BRU_AHB1_CLK__SHIFT     (3U)
/*! BRU_AHB1_CLK_ - BRU_AHB1 Soft Reset */
#define CIU1_CIU_RST_SW2_BRU_AHB1_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_BRU_AHB1_CLK__SHIFT)) & CIU1_CIU_RST_SW2_BRU_AHB1_CLK__MASK)

#define CIU1_CIU_RST_SW2_IPS_CLK__MASK           (0x2000U)
#define CIU1_CIU_RST_SW2_IPS_CLK__SHIFT          (13U)
/*! IPS_CLK_ - ips functional clock SW reset */
#define CIU1_CIU_RST_SW2_IPS_CLK_(x)             (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_IPS_CLK__SHIFT)) & CIU1_CIU_RST_SW2_IPS_CLK__MASK)

#define CIU1_CIU_RST_SW2_IPS_PROM_CLK__MASK      (0x4000U)
#define CIU1_CIU_RST_SW2_IPS_PROM_CLK__SHIFT     (14U)
/*! IPS_PROM_CLK_ - SW reset for ips_prom_clk */
#define CIU1_CIU_RST_SW2_IPS_PROM_CLK_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_IPS_PROM_CLK__SHIFT)) & CIU1_CIU_RST_SW2_IPS_PROM_CLK__MASK)

#define CIU1_CIU_RST_SW2_WLAN_SUB_G2BIST_RSTB_MASK (0x8000U)
#define CIU1_CIU_RST_SW2_WLAN_SUB_G2BIST_RSTB_SHIFT (15U)
/*! WLAN_SUB_G2BIST_RSTB - SW reset for wlan sub-system g2bist controller */
#define CIU1_CIU_RST_SW2_WLAN_SUB_G2BIST_RSTB(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_WLAN_SUB_G2BIST_RSTB_SHIFT)) & CIU1_CIU_RST_SW2_WLAN_SUB_G2BIST_RSTB_MASK)

#define CIU1_CIU_RST_SW2_IPS_HCLK__MASK          (0x80000U)
#define CIU1_CIU_RST_SW2_IPS_HCLK__SHIFT         (19U)
/*! IPS_HCLK_ - ips block hresetn */
#define CIU1_CIU_RST_SW2_IPS_HCLK_(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_IPS_HCLK__SHIFT)) & CIU1_CIU_RST_SW2_IPS_HCLK__MASK)

#define CIU1_CIU_RST_SW2_CPU1_TCM__MASK          (0x400000U)
#define CIU1_CIU_RST_SW2_CPU1_TCM__SHIFT         (22U)
/*! CPU1_TCM_ - CPU1 TCM/DMA/Arbiter reset */
#define CIU1_CIU_RST_SW2_CPU1_TCM_(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_CPU1_TCM__SHIFT)) & CIU1_CIU_RST_SW2_CPU1_TCM__MASK)

#define CIU1_CIU_RST_SW2_APB__MASK               (0x800000U)
#define CIU1_CIU_RST_SW2_APB__SHIFT              (23U)
/*! APB_ - APB core clock Soft Reset (TBD_TREEPIE may not need this) */
#define CIU1_CIU_RST_SW2_APB_(x)                 (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_APB__SHIFT)) & CIU1_CIU_RST_SW2_APB__MASK)

#define CIU1_CIU_RST_SW2_WD1_CHIP_RST_DISABLE_MASK (0x10000000U)
#define CIU1_CIU_RST_SW2_WD1_CHIP_RST_DISABLE_SHIFT (28U)
/*! WD1_CHIP_RST_DISABLE - WD1 Chip Reset Disable Feature */
#define CIU1_CIU_RST_SW2_WD1_CHIP_RST_DISABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_WD1_CHIP_RST_DISABLE_SHIFT)) & CIU1_CIU_RST_SW2_WD1_CHIP_RST_DISABLE_MASK)

#define CIU1_CIU_RST_SW2_WD1_CPU1_RST_DISABLE_MASK (0x20000000U)
#define CIU1_CIU_RST_SW2_WD1_CPU1_RST_DISABLE_SHIFT (29U)
/*! WD1_CPU1_RST_DISABLE - CPU1 Reset Disable Feature */
#define CIU1_CIU_RST_SW2_WD1_CPU1_RST_DISABLE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_WD1_CPU1_RST_DISABLE_SHIFT)) & CIU1_CIU_RST_SW2_WD1_CPU1_RST_DISABLE_MASK)

#define CIU1_CIU_RST_SW2_CPU1_CORE__MASK         (0x40000000U)
#define CIU1_CIU_RST_SW2_CPU1_CORE__SHIFT        (30U)
/*! CPU1_CORE_ - CPU1 core reset */
#define CIU1_CIU_RST_SW2_CPU1_CORE_(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW2_CPU1_CORE__SHIFT)) & CIU1_CIU_RST_SW2_CPU1_CORE__MASK)
/*! @} */

/*! @name CIU_RST_SW4 - Software Module Reset */
/*! @{ */

#define CIU1_CIU_RST_SW4_CPU1_DBG__MASK          (0x1U)
#define CIU1_CIU_RST_SW4_CPU1_DBG__SHIFT         (0U)
/*! CPU1_DBG_ - CPU1 debug logic soft reset. */
#define CIU1_CIU_RST_SW4_CPU1_DBG_(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_CPU1_DBG__SHIFT)) & CIU1_CIU_RST_SW4_CPU1_DBG__MASK)

#define CIU1_CIU_RST_SW4_CPU1_WATCHDOG__MASK     (0x2U)
#define CIU1_CIU_RST_SW4_CPU1_WATCHDOG__SHIFT    (1U)
/*! CPU1_WATCHDOG_ - CPU1 watchdog logic soft reset */
#define CIU1_CIU_RST_SW4_CPU1_WATCHDOG_(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_CPU1_WATCHDOG__SHIFT)) & CIU1_CIU_RST_SW4_CPU1_WATCHDOG__MASK)

#define CIU1_CIU_RST_SW4_CPU1_G2BIST__MASK       (0x8U)
#define CIU1_CIU_RST_SW4_CPU1_G2BIST__SHIFT      (3U)
/*! CPU1_G2BIST_ - CPU1 g2bist soft reset */
#define CIU1_CIU_RST_SW4_CPU1_G2BIST_(x)         (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_CPU1_G2BIST__SHIFT)) & CIU1_CIU_RST_SW4_CPU1_G2BIST__MASK)

#define CIU1_CIU_RST_SW4_CAU_SIF__MASK           (0x10U)
#define CIU1_CIU_RST_SW4_CAU_SIF__SHIFT          (4U)
/*! CAU_SIF_ - CAU sif clock Soft Reset */
#define CIU1_CIU_RST_SW4_CAU_SIF_(x)             (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_CAU_SIF__SHIFT)) & CIU1_CIU_RST_SW4_CAU_SIF__MASK)

#define CIU1_CIU_RST_SW4_CAU_SIF_AHB1_CLK__MASK  (0x20U)
#define CIU1_CIU_RST_SW4_CAU_SIF_AHB1_CLK__SHIFT (5U)
/*! CAU_SIF_AHB1_CLK_ - CAU sif ahb1 Clock Soft Reset */
#define CIU1_CIU_RST_SW4_CAU_SIF_AHB1_CLK_(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_CAU_SIF_AHB1_CLK__SHIFT)) & CIU1_CIU_RST_SW4_CAU_SIF_AHB1_CLK__MASK)

#define CIU1_CIU_RST_SW4_WLM_SYS_CLK__MASK       (0x200000U)
#define CIU1_CIU_RST_SW4_WLM_SYS_CLK__SHIFT      (21U)
/*! WLM_SYS_CLK_ - WLM SYS CLK Soft Reset */
#define CIU1_CIU_RST_SW4_WLM_SYS_CLK_(x)         (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_WLM_SYS_CLK__SHIFT)) & CIU1_CIU_RST_SW4_WLM_SYS_CLK__MASK)

#define CIU1_CIU_RST_SW4_WL_SYS_CLK__MASK        (0x400000U)
#define CIU1_CIU_RST_SW4_WL_SYS_CLK__SHIFT       (22U)
/*! WL_SYS_CLK_ - WL SYS CLK Soft Reset (Not used in Rendfinch) */
#define CIU1_CIU_RST_SW4_WL_SYS_CLK_(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_WL_SYS_CLK__SHIFT)) & CIU1_CIU_RST_SW4_WL_SYS_CLK__MASK)

#define CIU1_CIU_RST_SW4_MCU1_AHB_CLK_RETENTION__MASK (0x1000000U)
#define CIU1_CIU_RST_SW4_MCU1_AHB_CLK_RETENTION__SHIFT (24U)
/*! MCU1_AHB_CLK_RETENTION_ - MCU1 AHB Soft Retention Reset */
#define CIU1_CIU_RST_SW4_MCU1_AHB_CLK_RETENTION_(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_MCU1_AHB_CLK_RETENTION__SHIFT)) & CIU1_CIU_RST_SW4_MCU1_AHB_CLK_RETENTION__MASK)

#define CIU1_CIU_RST_SW4_MCU1_MCLK_RETENTION__MASK (0x2000000U)
#define CIU1_CIU_RST_SW4_MCU1_MCLK_RETENTION__SHIFT (25U)
/*! MCU1_MCLK_RETENTION_ - MCU1 MCLK domain retention reset */
#define CIU1_CIU_RST_SW4_MCU1_MCLK_RETENTION_(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_MCU1_MCLK_RETENTION__SHIFT)) & CIU1_CIU_RST_SW4_MCU1_MCLK_RETENTION__MASK)

#define CIU1_CIU_RST_SW4_MCU1_SYS_CLK_RETENTION__MASK (0x4000000U)
#define CIU1_CIU_RST_SW4_MCU1_SYS_CLK_RETENTION__SHIFT (26U)
/*! MCU1_SYS_CLK_RETENTION_ - MCU1 sysclk domain Soft Retention Reset */
#define CIU1_CIU_RST_SW4_MCU1_SYS_CLK_RETENTION_(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW4_MCU1_SYS_CLK_RETENTION__SHIFT)) & CIU1_CIU_RST_SW4_MCU1_SYS_CLK_RETENTION__MASK)
/*! @} */

/*! @name CIU_RST_SW5 - Software Module Reset */
/*! @{ */

#define CIU1_CIU_RST_SW5_WL_MSC_A2A__MASK        (0x8U)
#define CIU1_CIU_RST_SW5_WL_MSC_A2A__SHIFT       (3U)
/*! WL_MSC_A2A_ - MSC a2a soft reset */
#define CIU1_CIU_RST_SW5_WL_MSC_A2A_(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_WL_MSC_A2A__SHIFT)) & CIU1_CIU_RST_SW5_WL_MSC_A2A__MASK)

#define CIU1_CIU_RST_SW5_WL_SOC_A2A__MASK        (0x10U)
#define CIU1_CIU_RST_SW5_WL_SOC_A2A__SHIFT       (4U)
/*! WL_SOC_A2A_ - SOC TOP a2a soft reset */
#define CIU1_CIU_RST_SW5_WL_SOC_A2A_(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_WL_SOC_A2A__SHIFT)) & CIU1_CIU_RST_SW5_WL_SOC_A2A__MASK)

#define CIU1_CIU_RST_SW5_SMU1_AHB_RST__MASK      (0x20U)
#define CIU1_CIU_RST_SW5_SMU1_AHB_RST__SHIFT     (5U)
/*! SMU1_AHB_RST_ - SW reset for smu1 ahb */
#define CIU1_CIU_RST_SW5_SMU1_AHB_RST_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_SMU1_AHB_RST__SHIFT)) & CIU1_CIU_RST_SW5_SMU1_AHB_RST__MASK)

#define CIU1_CIU_RST_SW5_MCI_LITE2AHB_RST__MASK  (0x40U)
#define CIU1_CIU_RST_SW5_MCI_LITE2AHB_RST__SHIFT (6U)
/*! MCI_LITE2AHB_RST_ - SW reset to mci lite2ahb in wl_top */
#define CIU1_CIU_RST_SW5_MCI_LITE2AHB_RST_(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_MCI_LITE2AHB_RST__SHIFT)) & CIU1_CIU_RST_SW5_MCI_LITE2AHB_RST__MASK)

#define CIU1_CIU_RST_SW5_SMU1_CFG_RST__MASK      (0x80U)
#define CIU1_CIU_RST_SW5_SMU1_CFG_RST__SHIFT     (7U)
/*! SMU1_CFG_RST_ - SW reset for smu1 reg */
#define CIU1_CIU_RST_SW5_SMU1_CFG_RST_(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_SMU1_CFG_RST__SHIFT)) & CIU1_CIU_RST_SW5_SMU1_CFG_RST__MASK)

#define CIU1_CIU_RST_SW5_SMU1_UNGATED_CLK_RST__MASK (0x100U)
#define CIU1_CIU_RST_SW5_SMU1_UNGATED_CLK_RST__SHIFT (8U)
/*! SMU1_UNGATED_CLK_RST_ - SW reset for the NIC of smu1 */
#define CIU1_CIU_RST_SW5_SMU1_UNGATED_CLK_RST_(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_SMU1_UNGATED_CLK_RST__SHIFT)) & CIU1_CIU_RST_SW5_SMU1_UNGATED_CLK_RST__MASK)

#define CIU1_CIU_RST_SW5_GPS_PPS_RST__MASK       (0x200U)
#define CIU1_CIU_RST_SW5_GPS_PPS_RST__SHIFT      (9U)
/*! GPS_PPS_RST_ - SW Reset for the gps pss */
#define CIU1_CIU_RST_SW5_GPS_PPS_RST_(x)         (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_GPS_PPS_RST__SHIFT)) & CIU1_CIU_RST_SW5_GPS_PPS_RST__MASK)

#define CIU1_CIU_RST_SW5_MCI_WL_A2A_MHRESETN_MASK (0x400U)
#define CIU1_CIU_RST_SW5_MCI_WL_A2A_MHRESETN_SHIFT (10U)
/*! MCI_WL_A2A_MHRESETN - SW Reset for mci_wl_a2a_mhresetn */
#define CIU1_CIU_RST_SW5_MCI_WL_A2A_MHRESETN(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_MCI_WL_A2A_MHRESETN_SHIFT)) & CIU1_CIU_RST_SW5_MCI_WL_A2A_MHRESETN_MASK)

#define CIU1_CIU_RST_SW5_PTP_RST__MASK           (0x800U)
#define CIU1_CIU_RST_SW5_PTP_RST__SHIFT          (11U)
/*! PTP_RST_ - SW reset for ptp input capture logic */
#define CIU1_CIU_RST_SW5_PTP_RST_(x)             (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_SW5_PTP_RST__SHIFT)) & CIU1_CIU_RST_SW5_PTP_RST__MASK)
/*! @} */

/*! @name CIU_RST_ECO_CTRL - Reset ECO Control */
/*! @{ */

#define CIU1_CIU_RST_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_RST_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - ECO Bits */
#define CIU1_CIU_RST_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RST_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_RST_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_MEM_WRTC2 - Memory WRTC Control2 */
/*! @{ */

#define CIU1_CIU_MEM_WRTC2_SMU1_RTC_MASK         (0x3000U)
#define CIU1_CIU_MEM_WRTC2_SMU1_RTC_SHIFT        (12U)
/*! SMU1_RTC - SMU1 SEG1 RTC */
#define CIU1_CIU_MEM_WRTC2_SMU1_RTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_SMU1_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_SMU1_RTC_MASK)

#define CIU1_CIU_MEM_WRTC2_SMU1_WTC_MASK         (0xC000U)
#define CIU1_CIU_MEM_WRTC2_SMU1_WTC_SHIFT        (14U)
/*! SMU1_WTC - SMU1 SEG1 WTC */
#define CIU1_CIU_MEM_WRTC2_SMU1_WTC(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_SMU1_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_SMU1_WTC_MASK)

#define CIU1_CIU_MEM_WRTC2_R1P_RTC_MASK          (0x30000U)
#define CIU1_CIU_MEM_WRTC2_R1P_RTC_SHIFT         (16U)
/*! R1P_RTC - Small Single Port SRAM RTC */
#define CIU1_CIU_MEM_WRTC2_R1P_RTC(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_R1P_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_R1P_RTC_MASK)

#define CIU1_CIU_MEM_WRTC2_R1P_WTC_MASK          (0xC0000U)
#define CIU1_CIU_MEM_WRTC2_R1P_WTC_SHIFT         (18U)
/*! R1P_WTC - Small Single Port SRAM WTC */
#define CIU1_CIU_MEM_WRTC2_R1P_WTC(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_R1P_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_R1P_WTC_MASK)

#define CIU1_CIU_MEM_WRTC2_R2P_RTC_MASK          (0x300000U)
#define CIU1_CIU_MEM_WRTC2_R2P_RTC_SHIFT         (20U)
/*! R2P_RTC - Small Dual Port SRAM RTC */
#define CIU1_CIU_MEM_WRTC2_R2P_RTC(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_R2P_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_R2P_RTC_MASK)

#define CIU1_CIU_MEM_WRTC2_R2P_WTC_MASK          (0xC00000U)
#define CIU1_CIU_MEM_WRTC2_R2P_WTC_SHIFT         (22U)
/*! R2P_WTC - Small Dual Port SRAM WTC */
#define CIU1_CIU_MEM_WRTC2_R2P_WTC(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_R2P_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_R2P_WTC_MASK)

#define CIU1_CIU_MEM_WRTC2_SMU1_SEG0_RTC_MASK    (0xC000000U)
#define CIU1_CIU_MEM_WRTC2_SMU1_SEG0_RTC_SHIFT   (26U)
/*! SMU1_SEG0_RTC - SMU1 SEG0 RTC */
#define CIU1_CIU_MEM_WRTC2_SMU1_SEG0_RTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_SMU1_SEG0_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_SMU1_SEG0_RTC_MASK)

#define CIU1_CIU_MEM_WRTC2_SMU1_SEG0_WTC_MASK    (0xC0000000U)
#define CIU1_CIU_MEM_WRTC2_SMU1_SEG0_WTC_SHIFT   (30U)
/*! SMU1_SEG0_WTC - SMU1 SEG0 WTC */
#define CIU1_CIU_MEM_WRTC2_SMU1_SEG0_WTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC2_SMU1_SEG0_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC2_SMU1_SEG0_WTC_MASK)
/*! @} */

/*! @name CIU_MEM_WRTC3 - Memory WRTC Control 3 */
/*! @{ */

#define CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_MASK      (0x7U)
#define CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_SHIFT     (0U)
/*! WEU_ROM_RTC - WEU 256x15 ROM RTC */
#define CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_REF_MASK  (0x30U)
#define CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_REF_SHIFT (4U)
/*! WEU_ROM_RTC_REF - WEU 256x16 ROM RTC_REF */
#define CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_REF(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_REF_SHIFT)) & CIU1_CIU_MEM_WRTC3_WEU_ROM_RTC_REF_MASK)

#define CIU1_CIU_MEM_WRTC3_CPU1_ITCM_RTC_MASK    (0xC0U)
#define CIU1_CIU_MEM_WRTC3_CPU1_ITCM_RTC_SHIFT   (6U)
/*! CPU1_ITCM_RTC - CPU1 ITCM RTC */
#define CIU1_CIU_MEM_WRTC3_CPU1_ITCM_RTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_CPU1_ITCM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_CPU1_ITCM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_CPU1_ITCM_WTC_MASK    (0x300U)
#define CIU1_CIU_MEM_WRTC3_CPU1_ITCM_WTC_SHIFT   (8U)
/*! CPU1_ITCM_WTC - CPU1 ITCM WTC */
#define CIU1_CIU_MEM_WRTC3_CPU1_ITCM_WTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_CPU1_ITCM_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_CPU1_ITCM_WTC_MASK)

#define CIU1_CIU_MEM_WRTC3_CPU1_DTCM_RTC_MASK    (0xC00U)
#define CIU1_CIU_MEM_WRTC3_CPU1_DTCM_RTC_SHIFT   (10U)
/*! CPU1_DTCM_RTC - CPU1 DTCM RTC */
#define CIU1_CIU_MEM_WRTC3_CPU1_DTCM_RTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_CPU1_DTCM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_CPU1_DTCM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_CPU1_DTCM_WTC_MASK    (0x3000U)
#define CIU1_CIU_MEM_WRTC3_CPU1_DTCM_WTC_SHIFT   (12U)
/*! CPU1_DTCM_WTC - CPU1 DTCM WTC */
#define CIU1_CIU_MEM_WRTC3_CPU1_DTCM_WTC(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_CPU1_DTCM_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_CPU1_DTCM_WTC_MASK)

#define CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_MASK     (0x1C000U)
#define CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_SHIFT    (14U)
/*! CPU1_ROM_RTC - CPU1 ROM RTC */
#define CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_REF_MASK (0x60000U)
#define CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_REF_SHIFT (17U)
/*! CPU1_ROM_RTC_REF - Cpu1 ROM RTC_REF */
#define CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_REF(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_REF_SHIFT)) & CIU1_CIU_MEM_WRTC3_CPU1_ROM_RTC_REF_MASK)

#define CIU1_CIU_MEM_WRTC3_IPS_IRAM_RTC_MASK     (0x180000U)
#define CIU1_CIU_MEM_WRTC3_IPS_IRAM_RTC_SHIFT    (19U)
/*! IPS_IRAM_RTC - IPS IRAM RTC */
#define CIU1_CIU_MEM_WRTC3_IPS_IRAM_RTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_IPS_IRAM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_IPS_IRAM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_IPS_IRAM_WTC_MASK     (0x600000U)
#define CIU1_CIU_MEM_WRTC3_IPS_IRAM_WTC_SHIFT    (21U)
/*! IPS_IRAM_WTC - IPS IRAM WTC */
#define CIU1_CIU_MEM_WRTC3_IPS_IRAM_WTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_IPS_IRAM_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_IPS_IRAM_WTC_MASK)

#define CIU1_CIU_MEM_WRTC3_IPS_PRAM_RTC_MASK     (0x1800000U)
#define CIU1_CIU_MEM_WRTC3_IPS_PRAM_RTC_SHIFT    (23U)
/*! IPS_PRAM_RTC - IPS PRAM RTC */
#define CIU1_CIU_MEM_WRTC3_IPS_PRAM_RTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_IPS_PRAM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_IPS_PRAM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_IPS_PRAM_WTC_MASK     (0x6000000U)
#define CIU1_CIU_MEM_WRTC3_IPS_PRAM_WTC_SHIFT    (25U)
/*! IPS_PRAM_WTC - IPS PRAM WTC */
#define CIU1_CIU_MEM_WRTC3_IPS_PRAM_WTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_IPS_PRAM_WTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_IPS_PRAM_WTC_MASK)

#define CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_MASK     (0x38000000U)
#define CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_SHIFT    (27U)
/*! IPS_PROM_RTC - IPS PROM RTC */
#define CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_SHIFT)) & CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_MASK)

#define CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_REF_MASK (0xC0000000U)
#define CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_REF_SHIFT (30U)
/*! IPS_PROM_RTC_REF - IPS PROM RTC_REF */
#define CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_REF(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_REF_SHIFT)) & CIU1_CIU_MEM_WRTC3_IPS_PROM_RTC_REF_MASK)
/*! @} */

/*! @name CIU_MEM_CTRL - Memory Control */
/*! @{ */

#define CIU1_CIU_MEM_CTRL_SMU1_DEBUG_ON_MASK     (0x20U)
#define CIU1_CIU_MEM_CTRL_SMU1_DEBUG_ON_SHIFT    (5U)
/*! SMU1_DEBUG_ON - 1= enable smu internal register latch for debug */
#define CIU1_CIU_MEM_CTRL_SMU1_DEBUG_ON(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_CTRL_SMU1_DEBUG_ON_SHIFT)) & CIU1_CIU_MEM_CTRL_SMU1_DEBUG_ON_MASK)
/*! @} */

/*! @name CIU_SMU1_DBG_STAT0 - SMU1 debug register0 */
/*! @{ */

#define CIU1_CIU_SMU1_DBG_STAT0_SMU1_DBG_STAT0_MASK (0xFFFFFFFFU)
#define CIU1_CIU_SMU1_DBG_STAT0_SMU1_DBG_STAT0_SHIFT (0U)
/*! SMU1_DBG_STAT0 - smu1 debug addr output */
#define CIU1_CIU_SMU1_DBG_STAT0_SMU1_DBG_STAT0(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_SMU1_DBG_STAT0_SMU1_DBG_STAT0_SHIFT)) & CIU1_CIU_SMU1_DBG_STAT0_SMU1_DBG_STAT0_MASK)
/*! @} */

/*! @name CIU_SMU1_DBG_STAT1 - SMU1 debug register1 */
/*! @{ */

#define CIU1_CIU_SMU1_DBG_STAT1_SMU1_DBG_STAT1_MASK (0xFFFFFFFFU)
#define CIU1_CIU_SMU1_DBG_STAT1_SMU1_DBG_STAT1_SHIFT (0U)
/*! SMU1_DBG_STAT1 - smu1 debug data output */
#define CIU1_CIU_SMU1_DBG_STAT1_SMU1_DBG_STAT1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_SMU1_DBG_STAT1_SMU1_DBG_STAT1_SHIFT)) & CIU1_CIU_SMU1_DBG_STAT1_SMU1_DBG_STAT1_MASK)
/*! @} */

/*! @name CIU_SMU1_DBG_STAT2 - SMU1 debug register2 */
/*! @{ */

#define CIU1_CIU_SMU1_DBG_STAT2_SMU1_DBG_STAT2_MASK (0xFFFFFFFFU)
#define CIU1_CIU_SMU1_DBG_STAT2_SMU1_DBG_STAT2_SHIFT (0U)
/*! SMU1_DBG_STAT2 - smu1 debug ctrl output */
#define CIU1_CIU_SMU1_DBG_STAT2_SMU1_DBG_STAT2(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_SMU1_DBG_STAT2_SMU1_DBG_STAT2_SHIFT)) & CIU1_CIU_SMU1_DBG_STAT2_SMU1_DBG_STAT2_MASK)
/*! @} */

/*! @name CIU_MEM_ECO_CTRL - Memory ECO Control */
/*! @{ */

#define CIU1_CIU_MEM_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_MEM_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_MEM_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MEM_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_MEM_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU1_INT_MASK - CIU1 Interrupt Mask */
/*! @{ */

#define CIU1_CIU1_INT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_MASK (0x2U)
#define CIU1_CIU1_INT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_SHIFT (1U)
/*! PTP_INPUTCAPTURE_EVENT_INTR - PTP Input Capture Event Interrupt for Time Synchronization. In
 *    case of PTP mode input capture pulse from PAD or HMAC is used to latched PHC Value inside BBUD.
 *    On receiving this interrupt SW will read the PHC timestamp Value from BBUD register
 */
#define CIU1_CIU1_INT_MASK_PTP_INPUTCAPTURE_EVENT_INTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_SHIFT)) & CIU1_CIU1_INT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_MASK)

#define CIU1_CIU1_INT_MASK_MCI_WL_WAKEUP_DONE_INT_MASK (0xCU)
#define CIU1_CIU1_INT_MASK_MCI_WL_WAKEUP_DONE_INT_SHIFT (2U)
/*! MCI_WL_WAKEUP_DONE_INT - Wake up Interrupt done, from MCI (CPU3) to WL (CPU1) */
#define CIU1_CIU1_INT_MASK_MCI_WL_WAKEUP_DONE_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_MCI_WL_WAKEUP_DONE_INT_SHIFT)) & CIU1_CIU1_INT_MASK_MCI_WL_WAKEUP_DONE_INT_MASK)

#define CIU1_CIU1_INT_MASK_CPU1_TO_CPU3_MSG_DONE_MASK (0x10U)
#define CIU1_CIU1_INT_MASK_CPU1_TO_CPU3_MSG_DONE_SHIFT (4U)
/*! CPU1_TO_CPU3_MSG_DONE - CPU1 to CPU3 Message Done Interrupt. After CPU3 completed message
 *    processing requested by CPU1 it sends this interrupt to CPU1 by set 1 to CIU_CPU_CPU3_MSG_CTRL[8]
 */
#define CIU1_CIU1_INT_MASK_CPU1_TO_CPU3_MSG_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_CPU1_TO_CPU3_MSG_DONE_SHIFT)) & CIU1_CIU1_INT_MASK_CPU1_TO_CPU3_MSG_DONE_MASK)

#define CIU1_CIU1_INT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_MASK (0x20U)
#define CIU1_CIU1_INT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT (5U)
/*! IMU13_CPU1_MSG_SPACE_AVAIL_IMU - CPU1 to CPU3 message FIFO space available for CPU1 to write more messages. (Not used) */
#define CIU1_CIU1_INT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT)) & CIU1_CIU1_INT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_MASK)

#define CIU1_CIU1_INT_MASK_CPU1_TO_CPU2_MSG_DONE_MASK (0x40U)
#define CIU1_CIU1_INT_MASK_CPU1_TO_CPU2_MSG_DONE_SHIFT (6U)
/*! CPU1_TO_CPU2_MSG_DONE - CPU1 to CPU2 Message Done Interrupt. After CPU2 completed message
 *    processing requested by CPU1 it sends this interrupt to CPU1 by set 1 to CIU_CPU_CPU2_MSG_CTRL[8]
 */
#define CIU1_CIU1_INT_MASK_CPU1_TO_CPU2_MSG_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_CPU1_TO_CPU2_MSG_DONE_SHIFT)) & CIU1_CIU1_INT_MASK_CPU1_TO_CPU2_MSG_DONE_MASK)

#define CIU1_CIU1_INT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_MASK (0x80U)
#define CIU1_CIU1_INT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT (7U)
/*! IMU12_CPU1_MSG_SPACE_AVAIL_IMU - CPU1 to CPU2 message FIFO space available for CPU1 to write more messages.(Not used) */
#define CIU1_CIU1_INT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT)) & CIU1_CIU1_INT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_MASK)

#define CIU1_CIU1_INT_MASK_RFU_INT_2G_MASK       (0x100U)
#define CIU1_CIU1_INT_MASK_RFU_INT_2G_SHIFT      (8U)
/*! RFU_INT_2G - RFU2G interrupt */
#define CIU1_CIU1_INT_MASK_RFU_INT_2G(x)         (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_RFU_INT_2G_SHIFT)) & CIU1_CIU1_INT_MASK_RFU_INT_2G_MASK)

#define CIU1_CIU1_INT_MASK_RFU_INT_5G_A_MASK     (0x200U)
#define CIU1_CIU1_INT_MASK_RFU_INT_5G_A_SHIFT    (9U)
/*! RFU_INT_5G_A - RFU5G channel A interrupt */
#define CIU1_CIU1_INT_MASK_RFU_INT_5G_A(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_RFU_INT_5G_A_SHIFT)) & CIU1_CIU1_INT_MASK_RFU_INT_5G_A_MASK)

#define CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_EARLY_MASK (0x400U)
#define CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_EARLY_SHIFT (10U)
/*! GPS_INTR_CH_SW_EARLY - GPS PPS timer early interrupt */
#define CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_EARLY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_EARLY_SHIFT)) & CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_EARLY_MASK)

#define CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_MASK   (0x800U)
#define CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_SHIFT  (11U)
/*! GPS_INTR_CH_SW - GPS PPS timer interrupt */
#define CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_SHIFT)) & CIU1_CIU1_INT_MASK_GPS_INTR_CH_SW_MASK)

#define CIU1_CIU1_INT_MASK_APU_INTR_BT_WAKEUP_MASK (0x1000U)
#define CIU1_CIU1_INT_MASK_APU_INTR_BT_WAKEUP_SHIFT (12U)
/*! APU_INTR_BT_WAKEUP - APU BT interrupt. This bit indicates that the BTU has encountered some
 *    event during hardware sleep and would like the firmware to intervene. During BT wake time, this
 *    interrupt path should not be taken. Refer to APU doc for more details on the sleep/wakeup
 *    interrupt split.
 */
#define CIU1_CIU1_INT_MASK_APU_INTR_BT_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_APU_INTR_BT_WAKEUP_SHIFT)) & CIU1_CIU1_INT_MASK_APU_INTR_BT_WAKEUP_MASK)

#define CIU1_CIU1_INT_MASK_APU_INTR_WLAN_WAKEUP1_MASK (0x2000U)
#define CIU1_CIU1_INT_MASK_APU_INTR_WLAN_WAKEUP1_SHIFT (13U)
/*! APU_INTR_WLAN_WAKEUP1 - APU WLAN1 interrupt. This bit indicates that the WLAN hardware needs to
 *    wakeup for an incoming beacon. During WLAN wake time, this interrupt path should not be taken.
 *    Refer to APU doc for more details on the sleep/wakeup interrupt split.
 */
#define CIU1_CIU1_INT_MASK_APU_INTR_WLAN_WAKEUP1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_APU_INTR_WLAN_WAKEUP1_SHIFT)) & CIU1_CIU1_INT_MASK_APU_INTR_WLAN_WAKEUP1_MASK)

#define CIU1_CIU1_INT_MASK_APU_INTR_GEN_TIMER_WAKEUP_MASK (0x4000U)
#define CIU1_CIU1_INT_MASK_APU_INTR_GEN_TIMER_WAKEUP_SHIFT (14U)
/*! APU_INTR_GEN_TIMER_WAKEUP - APU beacon Timer wakeup interrupt */
#define CIU1_CIU1_INT_MASK_APU_INTR_GEN_TIMER_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_APU_INTR_GEN_TIMER_WAKEUP_SHIFT)) & CIU1_CIU1_INT_MASK_APU_INTR_GEN_TIMER_WAKEUP_MASK)

#define CIU1_CIU1_INT_MASK_APU_INTR_WAKEUP_MASK  (0x8000U)
#define CIU1_CIU1_INT_MASK_APU_INTR_WAKEUP_SHIFT (15U)
/*! APU_INTR_WAKEUP - APU default interrupt. The purpose of this interrupt is to catch any erroneous
 *    host wakeup case. In the event of an erroneous host wakeup, it is possible that the host
 *    wakeup interrupt will not assert. In order to ensure that the APU does not get stuck in some
 *    unknown state, this dummy interrupt will be thrown in it's place. Firmware should only care about
 *    this bit if no other interrupts are set in this register.
 */
#define CIU1_CIU1_INT_MASK_APU_INTR_WAKEUP(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_APU_INTR_WAKEUP_SHIFT)) & CIU1_CIU1_INT_MASK_APU_INTR_WAKEUP_MASK)

#define CIU1_CIU1_INT_MASK_APU_INTR_HOST_WAKEUP_MASK (0xFFFF0000U)
#define CIU1_CIU1_INT_MASK_APU_INTR_HOST_WAKEUP_SHIFT (16U)
/*! APU_INTR_HOST_WAKEUP - APU Host wakeup interrupt */
#define CIU1_CIU1_INT_MASK_APU_INTR_HOST_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_MASK_APU_INTR_HOST_WAKEUP_SHIFT)) & CIU1_CIU1_INT_MASK_APU_INTR_HOST_WAKEUP_MASK)
/*! @} */

/*! @name CIU1_INT_SELECT - CIU1 Interrupt Select */
/*! @{ */

#define CIU1_CIU1_INT_SELECT_SEL_MASK            (0xFFFFFFFFU)
#define CIU1_CIU1_INT_SELECT_SEL_SHIFT           (0U)
/*! SEL - Interrupt Read/Write Clear for CIU1 Interrupts */
#define CIU1_CIU1_INT_SELECT_SEL(x)              (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_SELECT_SEL_SHIFT)) & CIU1_CIU1_INT_SELECT_SEL_MASK)
/*! @} */

/*! @name CIU1_INT_EVENT_MASK - CIU1 Interrupt Event Mask */
/*! @{ */

#define CIU1_CIU1_INT_EVENT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_MASK (0x2U)
#define CIU1_CIU1_INT_EVENT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_SHIFT (1U)
/*! PTP_INPUTCAPTURE_EVENT_INTR - PTP Input Capture Event Interrupt for Time Synchronization. In
 *    case of PTP mode input capture pulse from PAD or HMAC is used to latched PHC Value inside BBUD.
 *    On receiving this interrupt SW will read the PHC timestamp Value from BBUD register
 */
#define CIU1_CIU1_INT_EVENT_MASK_PTP_INPUTCAPTURE_EVENT_INTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_PTP_INPUTCAPTURE_EVENT_INTR_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_MCI_WL_WAKEUP_DONE_INT_MASK (0xCU)
#define CIU1_CIU1_INT_EVENT_MASK_MCI_WL_WAKEUP_DONE_INT_SHIFT (2U)
/*! MCI_WL_WAKEUP_DONE_INT - Wake up Interrupt done, from MCI (CPU3) to WL (CPU1) */
#define CIU1_CIU1_INT_EVENT_MASK_MCI_WL_WAKEUP_DONE_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_MCI_WL_WAKEUP_DONE_INT_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_MCI_WL_WAKEUP_DONE_INT_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU3_MSG_DONE_MASK (0x10U)
#define CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU3_MSG_DONE_SHIFT (4U)
/*! CPU1_TO_CPU3_MSG_DONE - CPU1 to CPU3 Message Done Interrupt. After CPU3 completed message
 *    processing requested by CPU1 it sends this interrupt to CPU1 by set 1 to CIU_CPU_CPU3_MSG_CTRL[8]
 */
#define CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU3_MSG_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU3_MSG_DONE_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU3_MSG_DONE_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_MASK (0x20U)
#define CIU1_CIU1_INT_EVENT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT (5U)
/*! IMU13_CPU1_MSG_SPACE_AVAIL_IMU - CPU1 to CPU3 message FIFO space available for CPU1 to write more messages. (Not used) */
#define CIU1_CIU1_INT_EVENT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU2_MSG_DONE_MASK (0x40U)
#define CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU2_MSG_DONE_SHIFT (6U)
/*! CPU1_TO_CPU2_MSG_DONE - CPU1 to CPU2 Message Done Interrupt. After CPU2 completed message
 *    processing requested by CPU1 it sends this interrupt to CPU1 by set 1 to CIU_CPU_CPU2_MSG_CTRL[8]
 */
#define CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU2_MSG_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU2_MSG_DONE_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_CPU1_TO_CPU2_MSG_DONE_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_MASK (0x80U)
#define CIU1_CIU1_INT_EVENT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT (7U)
/*! IMU12_CPU1_MSG_SPACE_AVAIL_IMU - CPU1 to CPU2 message FIFO space available for CPU1 to write more messages.(Not used) */
#define CIU1_CIU1_INT_EVENT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_RFU_INT_2G_MASK (0x100U)
#define CIU1_CIU1_INT_EVENT_MASK_RFU_INT_2G_SHIFT (8U)
/*! RFU_INT_2G - RFU2G interrupt */
#define CIU1_CIU1_INT_EVENT_MASK_RFU_INT_2G(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_RFU_INT_2G_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_RFU_INT_2G_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_RFU_INT_5G_A_MASK (0x200U)
#define CIU1_CIU1_INT_EVENT_MASK_RFU_INT_5G_A_SHIFT (9U)
/*! RFU_INT_5G_A - RFU5G channel A interrupt */
#define CIU1_CIU1_INT_EVENT_MASK_RFU_INT_5G_A(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_RFU_INT_5G_A_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_RFU_INT_5G_A_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_EARLY_MASK (0x400U)
#define CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_EARLY_SHIFT (10U)
/*! GPS_INTR_CH_SW_EARLY - GPS PPS timer early interrupt */
#define CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_EARLY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_EARLY_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_EARLY_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_MASK (0x800U)
#define CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_SHIFT (11U)
/*! GPS_INTR_CH_SW - GPS PPS timer interrupt */
#define CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_GPS_INTR_CH_SW_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_BT_WAKEUP_MASK (0x1000U)
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_BT_WAKEUP_SHIFT (12U)
/*! APU_INTR_BT_WAKEUP - APU BT interrupt. This bit indicates that the BTU has encountered some
 *    event during hardware sleep and would like the firmware to intervene. During BT wake time, this
 *    interrupt path should not be taken. Refer to APU doc for more details on the sleep/wakeup
 *    interrupt split.
 */
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_BT_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_APU_INTR_BT_WAKEUP_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_APU_INTR_BT_WAKEUP_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WLAN_WAKEUP1_MASK (0x2000U)
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WLAN_WAKEUP1_SHIFT (13U)
/*! APU_INTR_WLAN_WAKEUP1 - APU WLAN1 interrupt. This bit indicates that the WLAN hardware needs to
 *    wakeup for an incoming beacon. During WLAN wake time, this interrupt path should not be taken.
 *    Refer to APU doc for more details on the sleep/wakeup interrupt split.
 */
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WLAN_WAKEUP1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WLAN_WAKEUP1_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WLAN_WAKEUP1_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_GEN_TIMER_WAKEUP_MASK (0x4000U)
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_GEN_TIMER_WAKEUP_SHIFT (14U)
/*! APU_INTR_GEN_TIMER_WAKEUP - APU beacon Timer wakeup interrupt */
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_GEN_TIMER_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_APU_INTR_GEN_TIMER_WAKEUP_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_APU_INTR_GEN_TIMER_WAKEUP_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WAKEUP_MASK (0x8000U)
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WAKEUP_SHIFT (15U)
/*! APU_INTR_WAKEUP - APU default interrupt. The purpose of this interrupt is to catch any erroneous
 *    host wakeup case. In the event of an erroneous host wakeup, it is possible that the host
 *    wakeup interrupt will not assert. In order to ensure that the APU does not get stuck in some
 *    unknown state, this dummy interrupt will be thrown in it's place. Firmware should only care about
 *    this bit if no other interrupts are set in this register.
 */
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WAKEUP_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_APU_INTR_WAKEUP_MASK)

#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_HOST_WAKEUP_MASK (0xFFFF0000U)
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_HOST_WAKEUP_SHIFT (16U)
/*! APU_INTR_HOST_WAKEUP - APU Host wakeup interrupt */
#define CIU1_CIU1_INT_EVENT_MASK_APU_INTR_HOST_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_EVENT_MASK_APU_INTR_HOST_WAKEUP_SHIFT)) & CIU1_CIU1_INT_EVENT_MASK_APU_INTR_HOST_WAKEUP_MASK)
/*! @} */

/*! @name CIU1_INT_STATUS - CIU1 Interrupt Status */
/*! @{ */

#define CIU1_CIU1_INT_STATUS_PTP_INPUTCAPTURE_EVENT_INTR_MASK (0x2U)
#define CIU1_CIU1_INT_STATUS_PTP_INPUTCAPTURE_EVENT_INTR_SHIFT (1U)
/*! PTP_INPUTCAPTURE_EVENT_INTR - PTP Input Capture Event Interrupt for Time Synchronization. In
 *    case of PTP mode input capture pulse from PAD or HMAC is used to latched PHC Value inside BBUD.
 *    On receiving this interrupt SW will read the PHC timestamp Value from BBUD register
 */
#define CIU1_CIU1_INT_STATUS_PTP_INPUTCAPTURE_EVENT_INTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_PTP_INPUTCAPTURE_EVENT_INTR_SHIFT)) & CIU1_CIU1_INT_STATUS_PTP_INPUTCAPTURE_EVENT_INTR_MASK)

#define CIU1_CIU1_INT_STATUS_MCI_WL_WAKEUP_DONE_INT_MASK (0xCU)
#define CIU1_CIU1_INT_STATUS_MCI_WL_WAKEUP_DONE_INT_SHIFT (2U)
/*! MCI_WL_WAKEUP_DONE_INT - Wake up Interrupt done, from MCI (CPU3) to WL (CPU1) */
#define CIU1_CIU1_INT_STATUS_MCI_WL_WAKEUP_DONE_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_MCI_WL_WAKEUP_DONE_INT_SHIFT)) & CIU1_CIU1_INT_STATUS_MCI_WL_WAKEUP_DONE_INT_MASK)

#define CIU1_CIU1_INT_STATUS_CPU1_TO_CPU3_MSG_DONE_MASK (0x10U)
#define CIU1_CIU1_INT_STATUS_CPU1_TO_CPU3_MSG_DONE_SHIFT (4U)
/*! CPU1_TO_CPU3_MSG_DONE - CPU1 to CPU3 Message Done Interrupt. After CPU3 completed message
 *    processing requested by CPU1 it sends this interrupt to CPU1 by set 1 to CIU_CPU_CPU3_MSG_CTRL[8]
 */
#define CIU1_CIU1_INT_STATUS_CPU1_TO_CPU3_MSG_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_CPU1_TO_CPU3_MSG_DONE_SHIFT)) & CIU1_CIU1_INT_STATUS_CPU1_TO_CPU3_MSG_DONE_MASK)

#define CIU1_CIU1_INT_STATUS_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_MASK (0x20U)
#define CIU1_CIU1_INT_STATUS_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT (5U)
/*! IMU13_CPU1_MSG_SPACE_AVAIL_IMU - CPU1 to CPU3 message FIFO space available for CPU1 to write more messages. (Not used) */
#define CIU1_CIU1_INT_STATUS_IMU13_CPU1_MSG_SPACE_AVAIL_IMU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT)) & CIU1_CIU1_INT_STATUS_IMU13_CPU1_MSG_SPACE_AVAIL_IMU_MASK)

#define CIU1_CIU1_INT_STATUS_CPU1_TO_CPU2_MSG_DONE_MASK (0x40U)
#define CIU1_CIU1_INT_STATUS_CPU1_TO_CPU2_MSG_DONE_SHIFT (6U)
/*! CPU1_TO_CPU2_MSG_DONE - CPU1 to CPU2 Message Done Interrupt. After CPU2 completed message
 *    processing requested by CPU1 it sends this interrupt to CPU1 by set 1 to CIU_CPU_CPU2_MSG_CTRL[8]
 */
#define CIU1_CIU1_INT_STATUS_CPU1_TO_CPU2_MSG_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_CPU1_TO_CPU2_MSG_DONE_SHIFT)) & CIU1_CIU1_INT_STATUS_CPU1_TO_CPU2_MSG_DONE_MASK)

#define CIU1_CIU1_INT_STATUS_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_MASK (0x80U)
#define CIU1_CIU1_INT_STATUS_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT (7U)
/*! IMU12_CPU1_MSG_SPACE_AVAIL_IMU - CPU1 to CPU2 message FIFO space available for CPU1 to write more messages.(Not used) */
#define CIU1_CIU1_INT_STATUS_IMU12_CPU1_MSG_SPACE_AVAIL_IMU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_SHIFT)) & CIU1_CIU1_INT_STATUS_IMU12_CPU1_MSG_SPACE_AVAIL_IMU_MASK)

#define CIU1_CIU1_INT_STATUS_RFU_INT_2G_MASK     (0x100U)
#define CIU1_CIU1_INT_STATUS_RFU_INT_2G_SHIFT    (8U)
/*! RFU_INT_2G - RFU2G interrupt */
#define CIU1_CIU1_INT_STATUS_RFU_INT_2G(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_RFU_INT_2G_SHIFT)) & CIU1_CIU1_INT_STATUS_RFU_INT_2G_MASK)

#define CIU1_CIU1_INT_STATUS_RFU_INT_5G_A_MASK   (0x200U)
#define CIU1_CIU1_INT_STATUS_RFU_INT_5G_A_SHIFT  (9U)
/*! RFU_INT_5G_A - RFU5G channel A interrupt */
#define CIU1_CIU1_INT_STATUS_RFU_INT_5G_A(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_RFU_INT_5G_A_SHIFT)) & CIU1_CIU1_INT_STATUS_RFU_INT_5G_A_MASK)

#define CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_EARLY_MASK (0x400U)
#define CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_EARLY_SHIFT (10U)
/*! GPS_INTR_CH_SW_EARLY - GPS PPS timer early interrupt */
#define CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_EARLY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_EARLY_SHIFT)) & CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_EARLY_MASK)

#define CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_MASK (0x800U)
#define CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_SHIFT (11U)
/*! GPS_INTR_CH_SW - GPS PPS timer interrupt */
#define CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_SHIFT)) & CIU1_CIU1_INT_STATUS_GPS_INTR_CH_SW_MASK)

#define CIU1_CIU1_INT_STATUS_APU_INTR_BT_WAKEUP_MASK (0x1000U)
#define CIU1_CIU1_INT_STATUS_APU_INTR_BT_WAKEUP_SHIFT (12U)
/*! APU_INTR_BT_WAKEUP - APU BT interrupt. This bit indicates that the BTU has encountered some
 *    event during hardware sleep and would like the firmware to intervene. During BT wake time, this
 *    interrupt path should not be taken. Refer to APU doc for more details on the sleep/wakeup
 *    interrupt split.
 */
#define CIU1_CIU1_INT_STATUS_APU_INTR_BT_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_APU_INTR_BT_WAKEUP_SHIFT)) & CIU1_CIU1_INT_STATUS_APU_INTR_BT_WAKEUP_MASK)

#define CIU1_CIU1_INT_STATUS_APU_INTR_WLAN_WAKEUP1_MASK (0x2000U)
#define CIU1_CIU1_INT_STATUS_APU_INTR_WLAN_WAKEUP1_SHIFT (13U)
/*! APU_INTR_WLAN_WAKEUP1 - APU WLAN1 interrupt. This bit indicates that the WLAN hardware needs to
 *    wakeup for an incoming beacon. During WLAN wake time, this interrupt path should not be taken.
 *    Refer to APU doc for more details on the sleep/wakeup interrupt split.
 */
#define CIU1_CIU1_INT_STATUS_APU_INTR_WLAN_WAKEUP1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_APU_INTR_WLAN_WAKEUP1_SHIFT)) & CIU1_CIU1_INT_STATUS_APU_INTR_WLAN_WAKEUP1_MASK)

#define CIU1_CIU1_INT_STATUS_APU_INTR_GEN_TIMER_WAKEUP_MASK (0x4000U)
#define CIU1_CIU1_INT_STATUS_APU_INTR_GEN_TIMER_WAKEUP_SHIFT (14U)
/*! APU_INTR_GEN_TIMER_WAKEUP - APU beacon Timer wakeup interrupt */
#define CIU1_CIU1_INT_STATUS_APU_INTR_GEN_TIMER_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_APU_INTR_GEN_TIMER_WAKEUP_SHIFT)) & CIU1_CIU1_INT_STATUS_APU_INTR_GEN_TIMER_WAKEUP_MASK)

#define CIU1_CIU1_INT_STATUS_APU_INTR_WAKEUP_MASK (0x8000U)
#define CIU1_CIU1_INT_STATUS_APU_INTR_WAKEUP_SHIFT (15U)
/*! APU_INTR_WAKEUP - APU default interrupt. The purpose of this interrupt is to catch any erroneous
 *    host wakeup case. In the event of an erroneous host wakeup, it is possible that the host
 *    wakeup interrupt will not assert. In order to ensure that the APU does not get stuck in some
 *    unknown state, this dummy interrupt will be thrown in it's place. Firmware should only care about
 *    this bit if no other interrupts are set in this register.
 */
#define CIU1_CIU1_INT_STATUS_APU_INTR_WAKEUP(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_APU_INTR_WAKEUP_SHIFT)) & CIU1_CIU1_INT_STATUS_APU_INTR_WAKEUP_MASK)

#define CIU1_CIU1_INT_STATUS_APU_INTR_HOST_WAKEUP_MASK (0xFFFF0000U)
#define CIU1_CIU1_INT_STATUS_APU_INTR_HOST_WAKEUP_SHIFT (16U)
/*! APU_INTR_HOST_WAKEUP - APU Host wakeup interrupt */
#define CIU1_CIU1_INT_STATUS_APU_INTR_HOST_WAKEUP(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_INT_STATUS_APU_INTR_HOST_WAKEUP_SHIFT)) & CIU1_CIU1_INT_STATUS_APU_INTR_HOST_WAKEUP_MASK)
/*! @} */

/*! @name CIU_INT_HOST_CTRL - Host Interrupt Control */
/*! @{ */

#define CIU1_CIU_INT_HOST_CTRL_UART_RX_IDLE_STATE_DETECT_MODE_MASK (0x1U)
#define CIU1_CIU_INT_HOST_CTRL_UART_RX_IDLE_STATE_DETECT_MODE_SHIFT (0U)
/*! UART_RX_IDLE_STATE_DETECT_MODE - UART Rx IDLE State Detection Mode. */
#define CIU1_CIU_INT_HOST_CTRL_UART_RX_IDLE_STATE_DETECT_MODE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_INT_HOST_CTRL_UART_RX_IDLE_STATE_DETECT_MODE_SHIFT)) & CIU1_CIU_INT_HOST_CTRL_UART_RX_IDLE_STATE_DETECT_MODE_MASK)
/*! @} */

/*! @name CIU_BCA1_INT_MASK - BCA1 to CPU1 Interrupt Mask */
/*! @{ */

#define CIU1_CIU_BCA1_INT_MASK_IMR_MASK          (0xFFFFFFFFU)
#define CIU1_CIU_BCA1_INT_MASK_IMR_SHIFT         (0U)
/*! IMR - Interrupt Mask for BCA1 to CPU1 Interrupts */
#define CIU1_CIU_BCA1_INT_MASK_IMR(x)            (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA1_INT_MASK_IMR_SHIFT)) & CIU1_CIU_BCA1_INT_MASK_IMR_MASK)
/*! @} */

/*! @name CIU_BCA1_INT_SELECT - BCA1 to CPU1 Interrupt Select */
/*! @{ */

#define CIU1_CIU_BCA1_INT_SELECT_RSR_MASK        (0xFFFFFFFFU)
#define CIU1_CIU_BCA1_INT_SELECT_RSR_SHIFT       (0U)
/*! RSR - Interrupt Read/Write Clear for BCA1 to CPU1 Interrupts */
#define CIU1_CIU_BCA1_INT_SELECT_RSR(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA1_INT_SELECT_RSR_SHIFT)) & CIU1_CIU_BCA1_INT_SELECT_RSR_MASK)
/*! @} */

/*! @name CIU_BCA1_INT_EVENT_MASK - BCA1 to CPU1 Interrupt Event Mask */
/*! @{ */

#define CIU1_CIU_BCA1_INT_EVENT_MASK_SMR_MASK    (0xFFFFFFFFU)
#define CIU1_CIU_BCA1_INT_EVENT_MASK_SMR_SHIFT   (0U)
/*! SMR - Interrupt Event Mask for BCA1 to CPU1 Interrupts */
#define CIU1_CIU_BCA1_INT_EVENT_MASK_SMR(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA1_INT_EVENT_MASK_SMR_SHIFT)) & CIU1_CIU_BCA1_INT_EVENT_MASK_SMR_MASK)
/*! @} */

/*! @name CIU_BCA1_INT_STATUS - BCA1 to CPU1 Interrupt Status */
/*! @{ */

#define CIU1_CIU_BCA1_INT_STATUS_ISR_MASK        (0xFFFFFFFFU)
#define CIU1_CIU_BCA1_INT_STATUS_ISR_SHIFT       (0U)
/*! ISR - BCA1 to CPU1 Interrupt Status */
#define CIU1_CIU_BCA1_INT_STATUS_ISR(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA1_INT_STATUS_ISR_SHIFT)) & CIU1_CIU_BCA1_INT_STATUS_ISR_MASK)
/*! @} */

/*! @name CPU1_ERR_INT_MASK - CPU1 ERR Interrupt Mask */
/*! @{ */

#define CIU1_CPU1_ERR_INT_MASK_MASK_MASK         (0xFFFFFFFFU)
#define CIU1_CPU1_ERR_INT_MASK_MASK_SHIFT        (0U)
/*! MASK - Interrupt Mask for CPU1 ERR Interrupts */
#define CIU1_CPU1_ERR_INT_MASK_MASK(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CPU1_ERR_INT_MASK_MASK_SHIFT)) & CIU1_CPU1_ERR_INT_MASK_MASK_MASK)
/*! @} */

/*! @name CPU1_ERR_INT_SELECT - CPU1 ERR Interrupt Clear Select */
/*! @{ */

#define CIU1_CPU1_ERR_INT_SELECT_SEL_MASK        (0xFFFFFFFFU)
#define CIU1_CPU1_ERR_INT_SELECT_SEL_SHIFT       (0U)
/*! SEL - Interrupt Read/Write Clear for CPU1 ERR Interrupts */
#define CIU1_CPU1_ERR_INT_SELECT_SEL(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CPU1_ERR_INT_SELECT_SEL_SHIFT)) & CIU1_CPU1_ERR_INT_SELECT_SEL_MASK)
/*! @} */

/*! @name CPU1_ERR_INT_EVENT_MASK - CPU1 ERR Interrupt Event Mask */
/*! @{ */

#define CIU1_CPU1_ERR_INT_EVENT_MASK_MASK_MASK   (0xFFFFFFFFU)
#define CIU1_CPU1_ERR_INT_EVENT_MASK_MASK_SHIFT  (0U)
/*! MASK - Interrupt Event Mask for CPU1 ERR Interrupts */
#define CIU1_CPU1_ERR_INT_EVENT_MASK_MASK(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CPU1_ERR_INT_EVENT_MASK_MASK_SHIFT)) & CIU1_CPU1_ERR_INT_EVENT_MASK_MASK_MASK)
/*! @} */

/*! @name CPU1_ERR_INT_STATUS - CPU1 ERR Interrupt Status */
/*! @{ */

#define CIU1_CPU1_ERR_INT_STATUS_ERR_ISR_MASK    (0xFFFFFFFFU)
#define CIU1_CPU1_ERR_INT_STATUS_ERR_ISR_SHIFT   (0U)
/*! ERR_ISR - CPU1 ERR Interrupt Status (ISR) */
#define CIU1_CPU1_ERR_INT_STATUS_ERR_ISR(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CPU1_ERR_INT_STATUS_ERR_ISR_SHIFT)) & CIU1_CPU1_ERR_INT_STATUS_ERR_ISR_MASK)
/*! @} */

/*! @name CPU2_INT_CTRL - CPU2 INT control */
/*! @{ */

#define CIU1_CPU2_INT_CTRL_CPU2_SW_INT_MASK      (0x1U)
#define CIU1_CPU2_INT_CTRL_CPU2_SW_INT_SHIFT     (0U)
/*! CPU2_SW_INT - SW programmed interrupt register for cpu2. write 1 to generate interrupt to CPU2. */
#define CIU1_CPU2_INT_CTRL_CPU2_SW_INT(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CPU2_INT_CTRL_CPU2_SW_INT_SHIFT)) & CIU1_CPU2_INT_CTRL_CPU2_SW_INT_MASK)
/*! @} */

/*! @name CPU3_INT_CTRL - CPU3 INT control */
/*! @{ */

#define CIU1_CPU3_INT_CTRL_CPU1_CPU3_GP_INT_MASK (0xFU)
#define CIU1_CPU3_INT_CTRL_CPU1_CPU3_GP_INT_SHIFT (0U)
/*! CPU1_CPU3_GP_INT - SW programmed interrupt register for cpu3. write 1 to generate interrupt to CPU3. */
#define CIU1_CPU3_INT_CTRL_CPU1_CPU3_GP_INT(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CPU3_INT_CTRL_CPU1_CPU3_GP_INT_SHIFT)) & CIU1_CPU3_INT_CTRL_CPU1_CPU3_GP_INT_MASK)
/*! @} */

/*! @name CIU_INT_ECO_CTRL - Interrupt ECO Control */
/*! @{ */

#define CIU1_CIU_INT_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_INT_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_INT_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_INT_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_INT_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_PTP_CTRL - Vsensor and Vreg Pad Control */
/*! @{ */

#define CIU1_CIU_PTP_CTRL_MODE_EN_MASK           (0xFFU)
#define CIU1_CIU_PTP_CTRL_MODE_EN_SHIFT          (0U)
/*! MODE_EN - PTP mode enable in iomux (bitwise) */
#define CIU1_CIU_PTP_CTRL_MODE_EN(x)             (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_PTP_CTRL_MODE_EN_SHIFT)) & CIU1_CIU_PTP_CTRL_MODE_EN_MASK)
/*! @} */

/*! @name CIU_RFU_DBC_MUX_SEL - RFU related DBC mux selection for different mode */
/*! @{ */

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX17_SEL_MASK (0x2U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX17_SEL_SHIFT (1U)
/*! RFU5G_A_MUX17_SEL - RFU 5G path A input selection from two bca */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX17_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX17_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX17_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX18_SEL_MASK (0x4U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX18_SEL_SHIFT (2U)
/*! RFU5G_B_MUX18_SEL - RFU 5G path B input selection from two bca */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX18_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX18_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX18_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX1_SEL_MASK (0x8U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX1_SEL_SHIFT (3U)
/*! RFU5G_A_MUX1_SEL - RFU 5G path A input selection from two bbud */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX1_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX1_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX1_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX2_SEL_MASK (0x10U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX2_SEL_SHIFT (4U)
/*! RFU5G_B_MUX2_SEL - RFU 5G path B input selection from two bbud */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX2_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX2_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX2_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX3_SEL_MASK (0x20U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX3_SEL_SHIFT (5U)
/*! RFU2G_A_MUX3_SEL - RFU 2G path A input selection from two bbud */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX3_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX3_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX3_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX4_SEL_MASK (0x40U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX4_SEL_SHIFT (6U)
/*! RFU2G_B_MUX4_SEL - RFU 2G path B input selection from two bbud */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX4_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX4_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX4_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_A_MUX5_SEL_MASK (0x80U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_A_MUX5_SEL_SHIFT (7U)
/*! BBUD1_A_MUX5_SEL - bbud1 path A input selection from two RFU */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_A_MUX5_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_A_MUX5_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_A_MUX5_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_B_MUX6_SEL_MASK (0x100U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_B_MUX6_SEL_SHIFT (8U)
/*! BBUD1_B_MUX6_SEL - bbud1 path B input selection from two RFU */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_B_MUX6_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_B_MUX6_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_B_MUX6_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_B_MUX7_SEL_MASK (0x200U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_B_MUX7_SEL_SHIFT (9U)
/*! BBUD2_B_MUX7_SEL - bbud2 path B input selection from two RFU */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_B_MUX7_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_B_MUX7_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_B_MUX7_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_A_MUX8_SEL_MASK (0x400U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_A_MUX8_SEL_SHIFT (10U)
/*! BBUD2_A_MUX8_SEL - bbud2 path A input selection from two RFU */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_A_MUX8_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_A_MUX8_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_A_MUX8_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_0_MASK (0x1800U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_0_SHIFT (11U)
/*! RFU5G_A_MUX13_SEL_0 - RFU 5G path A PE1/PE2/PAPE input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_0(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_0_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_0_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_0_MASK (0x6000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_0_SHIFT (13U)
/*! RFU5G_B_MUX14_SEL_0 - RFU 5G path B PE1/PE2/PAPE input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_0(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_0_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_0_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_0_MASK (0x18000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_0_SHIFT (15U)
/*! RFU2G_A_MUX15_SEL_0 - RFU 2G path A PE1/PE2/PAPE input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_0(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_0_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_0_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_0_MASK (0x60000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_0_SHIFT (17U)
/*! RFU2G_B_MUX16_SEL_0 - RFU 2G path B PE1/PE2/PAPE input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_0(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_0_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_0_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_1_MASK (0x80000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_1_SHIFT (19U)
/*! RFU5G_A_MUX13_SEL_1 - RFU 5G path A other input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_1_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_A_MUX13_SEL_1_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_1_MASK (0x100000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_1_SHIFT (20U)
/*! RFU5G_B_MUX14_SEL_1 - RFU 5G path B other input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_1_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU5G_B_MUX14_SEL_1_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_1_MASK (0x200000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_1_SHIFT (21U)
/*! RFU2G_A_MUX15_SEL_1 - RFU 2G path A other input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_1_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_A_MUX15_SEL_1_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_1_MASK (0x400000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_1_SHIFT (22U)
/*! RFU2G_B_MUX16_SEL_1 - RFU 2G path B other input selection from two mac */
#define CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_1_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_RFU2G_B_MUX16_SEL_1_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_A_SEL_MASK (0x1800000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_A_SEL_SHIFT (23U)
/*! BBUD1_TRX_RDY_A_SEL - bbud1_RFU_RDY_A and bbud1_RFU_INC_CAL_A selection */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_A_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_A_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_A_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_TRX_RDY_SEL_MASK (0x6000000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_TRX_RDY_SEL_SHIFT (25U)
/*! BBUD2_TRX_RDY_SEL - bbud2_RFU_RDY and bbud2_RFU_INC_CAL selection */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_TRX_RDY_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_TRX_RDY_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD2_TRX_RDY_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_B_SEL_MASK (0x18000000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_B_SEL_SHIFT (27U)
/*! BBUD1_TRX_RDY_B_SEL - bbud1_RFU_RDY_E and bbud1_RFU_INC_CAL_E selection */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_B_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_B_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BBUD1_TRX_RDY_B_SEL_MASK)

#define CIU1_CIU_RFU_DBC_MUX_SEL_BRF_TXPWR_SEL_MASK (0x80000000U)
#define CIU1_CIU_RFU_DBC_MUX_SEL_BRF_TXPWR_SEL_SHIFT (31U)
/*! BRF_TXPWR_SEL - SOC_BRF_PE1/2_2G and SOC_BRF_TXPWR_2G selection */
#define CIU1_CIU_RFU_DBC_MUX_SEL_BRF_TXPWR_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_DBC_MUX_SEL_BRF_TXPWR_SEL_SHIFT)) & CIU1_CIU_RFU_DBC_MUX_SEL_BRF_TXPWR_SEL_MASK)
/*! @} */

/*! @name CIU_BCA_DBC_MUX_SEL - BCA related DBC mux selection for different mode */
/*! @{ */

#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX11_SEL_MASK (0x1U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX11_SEL_SHIFT (0U)
/*! BCA1_MUX11_SEL - bca1 input selection from two mcu */
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX11_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX11_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX11_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX12_SEL_MASK (0x2U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX12_SEL_SHIFT (1U)
/*! BCA2_MUX12_SEL - bca2 input selection from two mcu */
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX12_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX12_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX12_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_MCU1_MUX9_SEL_MASK (0x4U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_MCU1_MUX9_SEL_SHIFT (2U)
/*! MCU1_MUX9_SEL - mcu1 input selection from two bca */
#define CIU1_CIU_BCA_DBC_MUX_SEL_MCU1_MUX9_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_MCU1_MUX9_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_MCU1_MUX9_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_MCU2_MUX10_SEL_MASK (0x8U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_MCU2_MUX10_SEL_SHIFT (3U)
/*! MCU2_MUX10_SEL - mcu2 input selection from two bca */
#define CIU1_CIU_BCA_DBC_MUX_SEL_MCU2_MUX10_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_MCU2_MUX10_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_MCU2_MUX10_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX19_SEL_MASK (0x30U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX19_SEL_SHIFT (4U)
/*! BCA1_MUX19_SEL - bca1 ros cal input selection from two RFU */
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX19_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX19_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_MUX19_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX20_SEL_MASK (0xC0U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX20_SEL_SHIFT (6U)
/*! BCA2_MUX20_SEL - bca2 ros cal input selection from two RFU */
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX20_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX20_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_MUX20_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_EPA_BYPASS_SEL_MASK (0x100U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_EPA_BYPASS_SEL_SHIFT (8U)
/*! BCA1_EPA_BYPASS_SEL - bca1 epa_bypass signal selction from two RFU */
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_EPA_BYPASS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_EPA_BYPASS_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_BCA1_EPA_BYPASS_SEL_MASK)

#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_EPA_BYPASS_SEL_MASK (0x200U)
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_EPA_BYPASS_SEL_SHIFT (9U)
/*! BCA2_EPA_BYPASS_SEL - bca2 epa_bypass signal selction from two RFU */
#define CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_EPA_BYPASS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_EPA_BYPASS_SEL_SHIFT)) & CIU1_CIU_BCA_DBC_MUX_SEL_BCA2_EPA_BYPASS_SEL_MASK)
/*! @} */

/*! @name CIU_TST_G2BIST_STATUS - WL G2BIST Status */
/*! @{ */

#define CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_STATUS_MASK (0xFU)
#define CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_STATUS_SHIFT (0U)
/*! WL_G2B_STATUS - Redundant Bist Selection */
#define CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_STATUS_SHIFT)) & CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_STATUS_MASK)

#define CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_FINISH_MASK (0x10U)
#define CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_FINISH_SHIFT (4U)
/*! WL_G2B_FINISH - WL Bist Done */
#define CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_FINISH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_FINISH_SHIFT)) & CIU1_CIU_TST_G2BIST_STATUS_WL_G2B_FINISH_MASK)

#define CIU1_CIU_TST_G2BIST_STATUS_MAC1_G2B_FINISH_MASK (0x20U)
#define CIU1_CIU_TST_G2BIST_STATUS_MAC1_G2B_FINISH_SHIFT (5U)
/*! MAC1_G2B_FINISH - MAC1 Bist Done */
#define CIU1_CIU_TST_G2BIST_STATUS_MAC1_G2B_FINISH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_G2BIST_STATUS_MAC1_G2B_FINISH_SHIFT)) & CIU1_CIU_TST_G2BIST_STATUS_MAC1_G2B_FINISH_MASK)
/*! @} */

/*! @name CIU_TST_MBIST_READY - MBIST Status (BIST_READY) */
/*! @{ */

#define CIU1_CIU_TST_MBIST_READY_BIST_READY_MASK (0xFFFFFFFFU)
#define CIU1_CIU_TST_MBIST_READY_BIST_READY_SHIFT (0U)
/*! BIST_READY - Bist ready[31:0] */
#define CIU1_CIU_TST_MBIST_READY_BIST_READY(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_MBIST_READY_BIST_READY_SHIFT)) & CIU1_CIU_TST_MBIST_READY_BIST_READY_MASK)
/*! @} */

/*! @name CIU_TST_MBIST_FAIL - MBIST Status (BIST_FAIL) */
/*! @{ */

#define CIU1_CIU_TST_MBIST_FAIL_BIST_FAIL_MASK   (0xFFFFFFFFU)
#define CIU1_CIU_TST_MBIST_FAIL_BIST_FAIL_SHIFT  (0U)
/*! BIST_FAIL - Bist Fail[31:0] */
#define CIU1_CIU_TST_MBIST_FAIL_BIST_FAIL(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_MBIST_FAIL_BIST_FAIL_SHIFT)) & CIU1_CIU_TST_MBIST_FAIL_BIST_FAIL_MASK)
/*! @} */

/*! @name CIU_TST_TSTBUS_CTRL2 - Testbux Mux Control2 */
/*! @{ */

#define CIU1_CIU_TST_TSTBUS_CTRL2_SMU_DEBUG_SEL_MASK (0xF8U)
#define CIU1_CIU_TST_TSTBUS_CTRL2_SMU_DEBUG_SEL_SHIFT (3U)
/*! SMU_DEBUG_SEL - SMU1/SMU2 Debug Select */
#define CIU1_CIU_TST_TSTBUS_CTRL2_SMU_DEBUG_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_TSTBUS_CTRL2_SMU_DEBUG_SEL_SHIFT)) & CIU1_CIU_TST_TSTBUS_CTRL2_SMU_DEBUG_SEL_MASK)
/*! @} */

/*! @name CIU_TST_CTRL - Test Control */
/*! @{ */

#define CIU1_CIU_TST_CTRL_BBU_TEST_MODE_EN_MASK  (0x200000U)
#define CIU1_CIU_TST_CTRL_BBU_TEST_MODE_EN_SHIFT (21U)
/*! BBU_TEST_MODE_EN - Baseband Test Mode Enable */
#define CIU1_CIU_TST_CTRL_BBU_TEST_MODE_EN(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_CTRL_BBU_TEST_MODE_EN_SHIFT)) & CIU1_CIU_TST_CTRL_BBU_TEST_MODE_EN_MASK)
/*! @} */

/*! @name CIU_TST_ECO_CTRL - Test ECO Control */
/*! @{ */

#define CIU1_CIU_TST_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_TST_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_TST_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_TST_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_TST_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_GPS_GPIO_MASK - GPS GPIO MASK */
/*! @{ */

#define CIU1_CIU_GPS_GPIO_MASK_GPIO2GPS_PPS_MASK_MASK (0xFFFFFFFFU)
#define CIU1_CIU_GPS_GPIO_MASK_GPIO2GPS_PPS_MASK_SHIFT (0U)
/*! GPIO2GPS_PPS_MASK - gpio to gps pps mask. '1' is to enable the gpio bit as PPS. Only 1 bit can be set */
#define CIU1_CIU_GPS_GPIO_MASK_GPIO2GPS_PPS_MASK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_GPS_GPIO_MASK_GPIO2GPS_PPS_MASK_SHIFT)) & CIU1_CIU_GPS_GPIO_MASK_GPIO2GPS_PPS_MASK_MASK)
/*! @} */

/*! @name CIU_GPS_SW_PERIOD - GPS SWITCH CHANNEL PERIOD */
/*! @{ */

#define CIU1_CIU_GPS_SW_PERIOD_CH_SW_PERIOD_MASK (0x1FFFFFU)
#define CIU1_CIU_GPS_SW_PERIOD_CH_SW_PERIOD_SHIFT (0U)
/*! CH_SW_PERIOD - period for channel switch. default to 50 ms */
#define CIU1_CIU_GPS_SW_PERIOD_CH_SW_PERIOD(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_GPS_SW_PERIOD_CH_SW_PERIOD_SHIFT)) & CIU1_CIU_GPS_SW_PERIOD_CH_SW_PERIOD_MASK)
/*! @} */

/*! @name CIU_GPS_SW_EARLY - GPS SWITCH CHANNEL EARLY */
/*! @{ */

#define CIU1_CIU_GPS_SW_EARLY_CH_SW_EARLY_TIME_MASK (0xFFFFU)
#define CIU1_CIU_GPS_SW_EARLY_CH_SW_EARLY_TIME_SHIFT (0U)
/*! CH_SW_EARLY_TIME - time offset from switch channel. default to 1 ms */
#define CIU1_CIU_GPS_SW_EARLY_CH_SW_EARLY_TIME(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_GPS_SW_EARLY_CH_SW_EARLY_TIME_SHIFT)) & CIU1_CIU_GPS_SW_EARLY_CH_SW_EARLY_TIME_MASK)
/*! @} */

/*! @name CIU_GPS_TMR_RD - GPS TIMER READ */
/*! @{ */

#define CIU1_CIU_GPS_TMR_RD_GPS_PPS_TMR_STATUS_MASK (0x3FFFFFU)
#define CIU1_CIU_GPS_TMR_RD_GPS_PPS_TMR_STATUS_SHIFT (0U)
/*! GPS_PPS_TMR_STATUS - gps pps timer status read */
#define CIU1_CIU_GPS_TMR_RD_GPS_PPS_TMR_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_GPS_TMR_RD_GPS_PPS_TMR_STATUS_SHIFT)) & CIU1_CIU_GPS_TMR_RD_GPS_PPS_TMR_STATUS_MASK)
/*! @} */

/*! @name CIU_LDO_ECO_CTRL - LDO ECO Control */
/*! @{ */

#define CIU1_CIU_LDO_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_LDO_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_LDO_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_LDO_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_LDO_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_AXI_CLK_CTRL2 - AXI clk bypass contrl2 */
/*! @{ */

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_VAL_MASK (0x1U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_VAL_SHIFT (0U)
/*! WLM_M_WIP_CG_BYPASS_VAL - AXI Clock gate enable value if bypass is enable. */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_VAL_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_VAL_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_EN_MASK (0x2U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_EN_SHIFT (1U)
/*! WLM_M_WIP_CG_BYPASS_EN - AXI Dynamic Clock gating Bypass for WEU */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_EN_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_M_WIP_CG_BYPASS_EN_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_VAL_MASK (0x4U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_VAL_SHIFT (2U)
/*! WLM_M_TBUS_CG_BYPASS_VAL - AXI Clock gate enable value if bypass is enable. */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_VAL_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_VAL_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_EN_MASK (0x8U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_EN_SHIFT (3U)
/*! WLM_M_TBUS_CG_BYPASS_EN - AXI Dynamic Clock gating Bypass for BBUd SQU Testbus master */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_EN_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_M_TBUS_CG_BYPASS_EN_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_VAL_MASK (0x10U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_VAL_SHIFT (4U)
/*! WLM_MCU1_64B_CG_BYPASS_VAL - AXI Clock gate enable value for HMAC if bypass is enable. */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_VAL_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_VAL_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_EN_MASK (0x20U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_EN_SHIFT (5U)
/*! WLM_MCU1_64B_CG_BYPASS_EN - AXI Dynamic Clock gating Bypass for HMAC */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_EN_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_MCU1_64B_CG_BYPASS_EN_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_VAL_MASK (0x40U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_VAL_SHIFT (6U)
/*! WLM_S_WL_CG_BYPASS_VAL - wlm_soc_wl_cg_bypass_val */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_VAL_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_VAL_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_EN_MASK (0x80U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_EN_SHIFT (7U)
/*! WLM_S_WL_CG_BYPASS_EN - wlm_soc_wl_cg_bypass_en */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_EN_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_S_WL_CG_BYPASS_EN_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_VAL_MASK (0x100U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_VAL_SHIFT (8U)
/*! WLM_GPV_CG_BYPASS_VAL - wlm_gpv_cg_bypass_val */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_VAL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_VAL_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_VAL_MASK)

#define CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_EN_MASK (0x200U)
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_EN_SHIFT (9U)
/*! WLM_GPV_CG_BYPASS_EN - wlm_gpv_cg_bypass_en */
#define CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_EN_SHIFT)) & CIU1_CIU_AXI_CLK_CTRL2_WLM_GPV_CG_BYPASS_EN_MASK)
/*! @} */

/*! @name CIU_FABRIC_TESTBUS_CTRL - fabric(scm, wlm) testbus select */
/*! @{ */

#define CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_HIGH_TESTBUS_SEL_MASK (0x3F0000U)
#define CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_HIGH_TESTBUS_SEL_SHIFT (16U)
/*! WLM_HIGH_TESTBUS_SEL - testbus select for smu1_nic_testbus[15:8] */
#define CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_HIGH_TESTBUS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_HIGH_TESTBUS_SEL_SHIFT)) & CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_HIGH_TESTBUS_SEL_MASK)

#define CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_LOW_TESTBUS_SEL_MASK (0x3F000000U)
#define CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_LOW_TESTBUS_SEL_SHIFT (24U)
/*! WLM_LOW_TESTBUS_SEL - testbus select for smu1_nic_testbus[7:0] */
#define CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_LOW_TESTBUS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_LOW_TESTBUS_SEL_SHIFT)) & CIU1_CIU_FABRIC_TESTBUS_CTRL_WLM_LOW_TESTBUS_SEL_MASK)
/*! @} */

/*! @name CIU_FABRIC_CREQ_DLY_TIMER - fabric(scm, wlm) delay timer for c_req */
/*! @{ */

#define CIU1_CIU_FABRIC_CREQ_DLY_TIMER_WLM_CREQ_DLY_TIMER_MASK (0x3FF00000U)
#define CIU1_CIU_FABRIC_CREQ_DLY_TIMER_WLM_CREQ_DLY_TIMER_SHIFT (20U)
/*! WLM_CREQ_DLY_TIMER - wlm delay timer for c_req */
#define CIU1_CIU_FABRIC_CREQ_DLY_TIMER_WLM_CREQ_DLY_TIMER(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_FABRIC_CREQ_DLY_TIMER_WLM_CREQ_DLY_TIMER_SHIFT)) & CIU1_CIU_FABRIC_CREQ_DLY_TIMER_WLM_CREQ_DLY_TIMER_MASK)
/*! @} */

/*! @name CIU_ABU_ECO_CTRL - ABU ECO Control */
/*! @{ */

#define CIU1_CIU_ABU_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_ABU_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_ABU_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ABU_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_ABU_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU1_AHB1_TO_CLEAR - AHB1 timeout logic clear register */
/*! @{ */

#define CIU1_CIU1_AHB1_TO_CLEAR_AHB1_TIMEOUT_CLEAR_MASK (0x1U)
#define CIU1_CIU1_AHB1_TO_CLEAR_AHB1_TIMEOUT_CLEAR_SHIFT (0U)
/*! AHB1_TIMEOUT_CLEAR - After the timeout happened on AHB1 bus, the cpu will read the ERR ISR and
 *    read the bus state which cause the timeout and then set this bit to 1 to clear the AHB1 timeout
 *    logic to start recroding next transaction. This is self clearing bit
 */
#define CIU1_CIU1_AHB1_TO_CLEAR_AHB1_TIMEOUT_CLEAR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_AHB1_TO_CLEAR_AHB1_TIMEOUT_CLEAR_SHIFT)) & CIU1_CIU1_AHB1_TO_CLEAR_AHB1_TIMEOUT_CLEAR_MASK)

#define CIU1_CIU1_AHB1_TO_CLEAR_CPU1_DCODE_INV_ADDR_CLR_MASK (0x2U)
#define CIU1_CIU1_AHB1_TO_CLEAR_CPU1_DCODE_INV_ADDR_CLR_SHIFT (1U)
/*! CPU1_DCODE_INV_ADDR_CLR - After the invalid address int happened on CPU1 dcode bus, the cpu1
 *    will read the ERR ISR and read the bus state which cause the timeout and then set this bit to 1
 *    to clear the CPU1 Dcode invalid addr logic to start recroding next transaction. This is self
 *    clearing bit
 */
#define CIU1_CIU1_AHB1_TO_CLEAR_CPU1_DCODE_INV_ADDR_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_AHB1_TO_CLEAR_CPU1_DCODE_INV_ADDR_CLR_SHIFT)) & CIU1_CIU1_AHB1_TO_CLEAR_CPU1_DCODE_INV_ADDR_CLR_MASK)

#define CIU1_CIU1_AHB1_TO_CLEAR_CPU1_ICODE_INV_ADDR_CLR_MASK (0x4U)
#define CIU1_CIU1_AHB1_TO_CLEAR_CPU1_ICODE_INV_ADDR_CLR_SHIFT (2U)
/*! CPU1_ICODE_INV_ADDR_CLR - After the invalid address int happened on CPU1 icode bus, the cpu1
 *    will read the ERR ISR and read the bus state which cause the timeout and then set this bit to 1
 *    to clear the CPU1 Icode invalid addr logic to start recroding next transaction. This is self
 *    clearing bit
 */
#define CIU1_CIU1_AHB1_TO_CLEAR_CPU1_ICODE_INV_ADDR_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_AHB1_TO_CLEAR_CPU1_ICODE_INV_ADDR_CLR_SHIFT)) & CIU1_CIU1_AHB1_TO_CLEAR_CPU1_ICODE_INV_ADDR_CLR_MASK)
/*! @} */

/*! @name CIU_ARB_TO_LAST_ADDR - AHB Timeout Last Address */
/*! @{ */

#define CIU1_CIU_ARB_TO_LAST_ADDR_ADDRESS_MASK   (0xFFFFFFFFU)
#define CIU1_CIU_ARB_TO_LAST_ADDR_ADDRESS_SHIFT  (0U)
/*! ADDRESS - Last AHB1 Address Right Before the Current Timeout */
#define CIU1_CIU_ARB_TO_LAST_ADDR_ADDRESS(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_TO_LAST_ADDR_ADDRESS_SHIFT)) & CIU1_CIU_ARB_TO_LAST_ADDR_ADDRESS_MASK)
/*! @} */

/*! @name CIU_ARB_TO_CUR_ADDR - AHB Current Timeout Address */
/*! @{ */

#define CIU1_CIU_ARB_TO_CUR_ADDR_ADDRESS_MASK    (0xFFFFFFFFU)
#define CIU1_CIU_ARB_TO_CUR_ADDR_ADDRESS_SHIFT   (0U)
/*! ADDRESS - Current_TO_Addr */
#define CIU1_CIU_ARB_TO_CUR_ADDR_ADDRESS(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_TO_CUR_ADDR_ADDRESS_SHIFT)) & CIU1_CIU_ARB_TO_CUR_ADDR_ADDRESS_MASK)
/*! @} */

/*! @name CIU_ARB_CTRL - AHB ARB Control */
/*! @{ */

#define CIU1_CIU_ARB_CTRL_CURRENT_TO_SLAVE_ID_MASK (0xFU)
#define CIU1_CIU_ARB_CTRL_CURRENT_TO_SLAVE_ID_SHIFT (0U)
/*! CURRENT_TO_SLAVE_ID - Current_TO_Slave_ID */
#define CIU1_CIU_ARB_CTRL_CURRENT_TO_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_CURRENT_TO_SLAVE_ID_SHIFT)) & CIU1_CIU_ARB_CTRL_CURRENT_TO_SLAVE_ID_MASK)

#define CIU1_CIU_ARB_CTRL_LAST_TO_SLAVE_ID_MASK  (0xF0U)
#define CIU1_CIU_ARB_CTRL_LAST_TO_SLAVE_ID_SHIFT (4U)
/*! LAST_TO_SLAVE_ID - Last_TO_Slave_ID */
#define CIU1_CIU_ARB_CTRL_LAST_TO_SLAVE_ID(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_LAST_TO_SLAVE_ID_SHIFT)) & CIU1_CIU_ARB_CTRL_LAST_TO_SLAVE_ID_MASK)

#define CIU1_CIU_ARB_CTRL_CURRENT_TO_MASTER_ID_MASK (0x700U)
#define CIU1_CIU_ARB_CTRL_CURRENT_TO_MASTER_ID_SHIFT (8U)
/*! CURRENT_TO_MASTER_ID - Current_TO_Master_ID */
#define CIU1_CIU_ARB_CTRL_CURRENT_TO_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_CURRENT_TO_MASTER_ID_SHIFT)) & CIU1_CIU_ARB_CTRL_CURRENT_TO_MASTER_ID_MASK)

#define CIU1_CIU_ARB_CTRL_LAST_TO_MASTER_ID_MASK (0x3800U)
#define CIU1_CIU_ARB_CTRL_LAST_TO_MASTER_ID_SHIFT (11U)
/*! LAST_TO_MASTER_ID - Last_TO_Master_ID */
#define CIU1_CIU_ARB_CTRL_LAST_TO_MASTER_ID(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_LAST_TO_MASTER_ID_SHIFT)) & CIU1_CIU_ARB_CTRL_LAST_TO_MASTER_ID_MASK)

#define CIU1_CIU_ARB_CTRL_AHB1_SMU1_MEM_PROT_DIS_MASK (0x10000U)
#define CIU1_CIU_ARB_CTRL_AHB1_SMU1_MEM_PROT_DIS_SHIFT (16U)
/*! AHB1_SMU1_MEM_PROT_DIS - Disable SMU1 Memory Protection from AHB2 side */
#define CIU1_CIU_ARB_CTRL_AHB1_SMU1_MEM_PROT_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_AHB1_SMU1_MEM_PROT_DIS_SHIFT)) & CIU1_CIU_ARB_CTRL_AHB1_SMU1_MEM_PROT_DIS_MASK)

#define CIU1_CIU_ARB_CTRL_AHB1_CPU1_IMEM_PROT_DIS_MASK (0x20000U)
#define CIU1_CIU_ARB_CTRL_AHB1_CPU1_IMEM_PROT_DIS_SHIFT (17U)
/*! AHB1_CPU1_IMEM_PROT_DIS - 1 = Disable CPU1 Imem Memory Protection from AHB1 side and allow AHB1 to read/write Imem */
#define CIU1_CIU_ARB_CTRL_AHB1_CPU1_IMEM_PROT_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_AHB1_CPU1_IMEM_PROT_DIS_SHIFT)) & CIU1_CIU_ARB_CTRL_AHB1_CPU1_IMEM_PROT_DIS_MASK)

#define CIU1_CIU_ARB_CTRL_AHB1_CPU1_DMEM_PROT_DIS_MASK (0x40000U)
#define CIU1_CIU_ARB_CTRL_AHB1_CPU1_DMEM_PROT_DIS_SHIFT (18U)
/*! AHB1_CPU1_DMEM_PROT_DIS - 1 = Disable CPU1 Dmem Memory Protection from AHB1 side and allow AHB1 to read/write Dmem */
#define CIU1_CIU_ARB_CTRL_AHB1_CPU1_DMEM_PROT_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_AHB1_CPU1_DMEM_PROT_DIS_SHIFT)) & CIU1_CIU_ARB_CTRL_AHB1_CPU1_DMEM_PROT_DIS_MASK)

#define CIU1_CIU_ARB_CTRL_AHB1_A2A_PROT_DIS_MASK (0x80000U)
#define CIU1_CIU_ARB_CTRL_AHB1_A2A_PROT_DIS_SHIFT (19U)
/*! AHB1_A2A_PROT_DIS - 1 = Disable A2A Memory Protection from AHB1 side and allow AHB1 to A2A */
#define CIU1_CIU_ARB_CTRL_AHB1_A2A_PROT_DIS(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_AHB1_A2A_PROT_DIS_SHIFT)) & CIU1_CIU_ARB_CTRL_AHB1_A2A_PROT_DIS_MASK)

#define CIU1_CIU_ARB_CTRL_ARB_TIMEOUT_MODE_MASK  (0xC0000000U)
#define CIU1_CIU_ARB_CTRL_ARB_TIMEOUT_MODE_SHIFT (30U)
/*! ARB_TIMEOUT_MODE - AHB1_TimeoutMode[1:0] */
#define CIU1_CIU_ARB_CTRL_ARB_TIMEOUT_MODE(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_CTRL_ARB_TIMEOUT_MODE_SHIFT)) & CIU1_CIU_ARB_CTRL_ARB_TIMEOUT_MODE_MASK)
/*! @} */

/*! @name CIU1_CPU1_ICODE_INV_ADDR_CTRL - CPU1 Icode invalid address access control */
/*! @{ */

#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK (0xFU)
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT (0U)
/*! LAST2_INV_ADDR_SLAVE_ID - Last2_inv_addr_Slave_ID */
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT)) & CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK)

#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK (0xF0U)
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT (4U)
/*! LAST_INV_ADDR_SLAVE_ID - Last_inv_addr_Slave_ID */
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT)) & CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK)

#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK (0xF00U)
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT (8U)
/*! CUR_INV_ADDR_SLAVE_ID - Cur_inv_addr_Slave_ID */
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT)) & CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK)

#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_MASK (0xC0000000U)
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_SHIFT (30U)
/*! HADDR_ICOD_SEL - There are 3 haddr which can be observed by selecting this: */
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_SHIFT)) & CIU1_CIU1_CPU1_ICODE_INV_ADDR_CTRL_HADDR_ICOD_SEL_MASK)
/*! @} */

/*! @name CIU1_CPU1_ICODE_INV_ADDR - CPU1 Icode invalid address */
/*! @{ */

#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_HADDR_INV_ADDR_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_HADDR_INV_ADDR_SHIFT (0U)
/*! HADDR_INV_ADDR - based on CIU1_CPU1_ICODE_INV_ADDR_CTRL[31:30], the address status is observed in this register */
#define CIU1_CIU1_CPU1_ICODE_INV_ADDR_HADDR_INV_ADDR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_ICODE_INV_ADDR_HADDR_INV_ADDR_SHIFT)) & CIU1_CIU1_CPU1_ICODE_INV_ADDR_HADDR_INV_ADDR_MASK)
/*! @} */

/*! @name CIU1_CPU1_DCODE_INV_ADDR_CTRL - CPU1 Dcode invalid address access control */
/*! @{ */

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK (0xFU)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT (0U)
/*! LAST2_INV_ADDR_SLAVE_ID - Last2_inv_addr_Slave_ID */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_SLAVE_ID_MASK)

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK (0xF0U)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT (4U)
/*! LAST_INV_ADDR_SLAVE_ID - Last_inv_addr_Slave_ID */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_SLAVE_ID_MASK)

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK (0xF00U)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT (8U)
/*! CUR_INV_ADDR_SLAVE_ID - Cur_inv_addr_Slave_ID */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_SLAVE_ID_MASK)

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_MASK (0xF000U)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_SHIFT (12U)
/*! LAST2_INV_ADDR_MASTER_ID - Last2_inv_addr_master_ID */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST2_INV_ADDR_MASTER_ID_MASK)

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_MASK (0xF0000U)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_SHIFT (16U)
/*! LAST_INV_ADDR_MASTER_ID - Last_inv_addr_master_ID */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_LAST_INV_ADDR_MASTER_ID_MASK)

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_MASK (0xF00000U)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_SHIFT (20U)
/*! CUR_INV_ADDR_MASTER_ID - Cur_inv_addr_master_ID */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_CUR_INV_ADDR_MASTER_ID_MASK)

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_HADDR_DCOD_SEL_MASK (0xC0000000U)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_HADDR_DCOD_SEL_SHIFT (30U)
/*! HADDR_DCOD_SEL - There are 3 haddr which can be observed by selecting this: */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_HADDR_DCOD_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_HADDR_DCOD_SEL_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_CTRL_HADDR_DCOD_SEL_MASK)
/*! @} */

/*! @name CIU1_CPU1_DCODE_INV_ADDR - CPU1 Dcode invalid address */
/*! @{ */

#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_HADDR_INV_ADDR_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_HADDR_INV_ADDR_SHIFT (0U)
/*! HADDR_INV_ADDR - based on CIU1_CPU1_DCODE_INV_ADDR_CTRL[31:30], the address status is observed in this register */
#define CIU1_CIU1_CPU1_DCODE_INV_ADDR_HADDR_INV_ADDR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_DCODE_INV_ADDR_HADDR_INV_ADDR_SHIFT)) & CIU1_CIU1_CPU1_DCODE_INV_ADDR_HADDR_INV_ADDR_MASK)
/*! @} */

/*! @name CIU1_AHB2AHB_BRIDGE_CTRL - AHB2AHB Bridge Control Register */
/*! @{ */

#define CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_MASK (0x1U)
#define CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_SHIFT (0U)
/*! PREFETCH_HSEL_EN - ahb2ahb bridge pre-fetch hsel enable */
#define CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_SHIFT)) & CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_PREFETCH_HSEL_EN_MASK)

#define CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_MCI_WL_A2A_PREFETCH_EN_MASK (0x2U)
#define CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_MCI_WL_A2A_PREFETCH_EN_SHIFT (1U)
/*! MCI_WL_A2A_PREFETCH_EN - MCI-WL ahb2ahb bridge pre-fetch hsel enable */
#define CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_MCI_WL_A2A_PREFETCH_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_MCI_WL_A2A_PREFETCH_EN_SHIFT)) & CIU1_CIU1_AHB2AHB_BRIDGE_CTRL_MCI_WL_A2A_PREFETCH_EN_MASK)
/*! @} */

/*! @name WL_RAACS_CTRL - RAACS control registers */
/*! @{ */

#define CIU1_WL_RAACS_CTRL_RAACS_EN_MASK         (0x1U)
#define CIU1_WL_RAACS_CTRL_RAACS_EN_SHIFT        (0U)
/*! RAACS_EN - raacs en . S/W Write 1 to enable raacs block. */
#define CIU1_WL_RAACS_CTRL_RAACS_EN(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_CTRL_RAACS_EN_SHIFT)) & CIU1_WL_RAACS_CTRL_RAACS_EN_MASK)

#define CIU1_WL_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_MASK (0x2U)
#define CIU1_WL_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_SHIFT (1U)
/*! USE_RAACS_CLK_FOR_CPU - SW write 0 to use RAACS clock for CPU. SW write 1 to select clock gating
 *    based alternate implementation of RAACS clocking for CM3 CPU.
 */
#define CIU1_WL_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU(x) (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_SHIFT)) & CIU1_WL_RAACS_CTRL_USE_RAACS_CLK_FOR_CPU_MASK)

#define CIU1_WL_RAACS_CTRL_RAACS_CLK_SEL_MASK    (0x1CU)
#define CIU1_WL_RAACS_CTRL_RAACS_CLK_SEL_SHIFT   (2U)
/*! RAACS_CLK_SEL - defines the lowest clock to which RAACS will go down to during IDLE period (x/2;
 *    x/4; x/8; ... ;x/128) for the given test.
 */
#define CIU1_WL_RAACS_CTRL_RAACS_CLK_SEL(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_CTRL_RAACS_CLK_SEL_SHIFT)) & CIU1_WL_RAACS_CTRL_RAACS_CLK_SEL_MASK)

#define CIU1_WL_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_MASK (0x7FFE0U)
#define CIU1_WL_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_SHIFT (5U)
/*! RAACS_WAIT_COUNTER_VALUE - initial IDLE-time for which RAACS FSM waits before starting to scale down the clock. */
#define CIU1_WL_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_SHIFT)) & CIU1_WL_RAACS_CTRL_RAACS_WAIT_COUNTER_VALUE_MASK)

#define CIU1_WL_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_MASK (0xFFF80000U)
#define CIU1_WL_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_SHIFT (19U)
/*! RAACS_IDLE_COUNTER_VALUE - IDLE time for which RAACS-FSM waits before shifting to next successive scaled clock. */
#define CIU1_WL_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_SHIFT)) & CIU1_WL_RAACS_CTRL_RAACS_IDLE_COUNTER_VALUE_MASK)
/*! @} */

/*! @name WL_RAACS_PERFORMANCE_STATISTICS - RAACS performance statistics counter. */
/*! @{ */

#define CIU1_WL_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_MASK (0x1U)
#define CIU1_WL_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_SHIFT (0U)
/*! PERFORMANCE_STATISTICS_CNT_EN - performance counter en. S/W write 1 to enable performance counter. */
#define CIU1_WL_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN(x) (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_SHIFT)) & CIU1_WL_RAACS_PERFORMANCE_STATISTICS_PERFORMANCE_STATISTICS_CNT_EN_MASK)

#define CIU1_WL_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_MASK (0xFFFFFFEU)
#define CIU1_WL_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_SHIFT (1U)
/*! RAACS_PERFORMANCE_STATISTICS - This counter is maintaining RAACS performance count. This counter
 *    will increment by one after every 1 us(1MHZ) when RAACS is in scaled clk state.
 */
#define CIU1_WL_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_WL_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_SHIFT)) & CIU1_WL_RAACS_PERFORMANCE_STATISTICS_RAACS_PERFORMANCE_STATISTICS_MASK)
/*! @} */

/*! @name CIU_AHB1_TSTBUS_SEL - AHB1 Control Signals testbus select */
/*! @{ */

#define CIU1_CIU_AHB1_TSTBUS_SEL_AHB1_TSTBUS_SEL_MASK (0x1U)
#define CIU1_CIU_AHB1_TSTBUS_SEL_AHB1_TSTBUS_SEL_SHIFT (0U)
/*! AHB1_TSTBUS_SEL - Select AHB1 Arbiter Testbus for testing */
#define CIU1_CIU_AHB1_TSTBUS_SEL_AHB1_TSTBUS_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AHB1_TSTBUS_SEL_AHB1_TSTBUS_SEL_SHIFT)) & CIU1_CIU_AHB1_TSTBUS_SEL_AHB1_TSTBUS_SEL_MASK)
/*! @} */

/*! @name CIU_ARB_ECO_CTRL - ARB ECO Control */
/*! @{ */

#define CIU1_CIU_ARB_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_ARB_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_ARB_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_ARB_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_ARB_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_CPU_DYN_CLK_CTRL - Dynamic CPU Clock Control */
/*! @{ */

#define CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_CPU1_CTRL_DIS_MASK (0x10000000U)
#define CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_CPU1_CTRL_DIS_SHIFT (28U)
/*! DYN_CPU1_CTRL_DIS - Disable Dynamic CPU1 Clock Control Feature */
#define CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_CPU1_CTRL_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_CPU1_CTRL_DIS_SHIFT)) & CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_CPU1_CTRL_DIS_MASK)

#define CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_SYS_CTRL_DIS_MASK (0x20000000U)
#define CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_SYS_CTRL_DIS_SHIFT (29U)
/*! DYN_SYS_CTRL_DIS - Disable Dynamic SYS Clock Control Feature */
#define CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_SYS_CTRL_DIS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_SYS_CTRL_DIS_SHIFT)) & CIU1_CIU_CPU_DYN_CLK_CTRL_DYN_SYS_CTRL_DIS_MASK)
/*! @} */

/*! @name CIU_CPU_DYN_CPUCLK_MONITOR - Dynamic ahb clock Monitor */
/*! @{ */

#define CIU1_CIU_CPU_DYN_CPUCLK_MONITOR_DYN_CPU1_CLK_MON_T1_MASK (0xFU)
#define CIU1_CIU_CPU_DYN_CPUCLK_MONITOR_DYN_CPU1_CLK_MON_T1_SHIFT (0U)
/*! DYN_CPU1_CLK_MON_T1 - Dynamic CPU1 Clock Monitor T1 */
#define CIU1_CIU_CPU_DYN_CPUCLK_MONITOR_DYN_CPU1_CLK_MON_T1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_DYN_CPUCLK_MONITOR_DYN_CPU1_CLK_MON_T1_SHIFT)) & CIU1_CIU_CPU_DYN_CPUCLK_MONITOR_DYN_CPU1_CLK_MON_T1_MASK)
/*! @} */

/*! @name CIU_CPU_DYN_SYSCLK_MONITOR - Dynamic sysclk Monitor */
/*! @{ */

#define CIU1_CIU_CPU_DYN_SYSCLK_MONITOR_DYN_SYSCLK_MON_T1_MASK (0xFU)
#define CIU1_CIU_CPU_DYN_SYSCLK_MONITOR_DYN_SYSCLK_MON_T1_SHIFT (0U)
/*! DYN_SYSCLK_MON_T1 - Dynamic System Clock Monitor T1 */
#define CIU1_CIU_CPU_DYN_SYSCLK_MONITOR_DYN_SYSCLK_MON_T1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_DYN_SYSCLK_MONITOR_DYN_SYSCLK_MON_T1_SHIFT)) & CIU1_CIU_CPU_DYN_SYSCLK_MONITOR_DYN_SYSCLK_MON_T1_MASK)
/*! @} */

/*! @name CIU_CPU_CPU1_CTRL - CPU1 control register */
/*! @{ */

#define CIU1_CIU_CPU_CPU1_CTRL_CPU1_DBG_CTRL_MASK (0xFF00U)
#define CIU1_CIU_CPU_CPU1_CTRL_CPU1_DBG_CTRL_SHIFT (8U)
/*! CPU1_DBG_CTRL - cpu2 debug control */
#define CIU1_CIU_CPU_CPU1_CTRL_CPU1_DBG_CTRL(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU1_CTRL_CPU1_DBG_CTRL_SHIFT)) & CIU1_CIU_CPU_CPU1_CTRL_CPU1_DBG_CTRL_MASK)

#define CIU1_CIU_CPU_CPU1_CTRL_CPU1_CPU3_MSG_SCHEME_MASK (0x40000U)
#define CIU1_CIU_CPU_CPU1_CTRL_CPU1_CPU3_MSG_SCHEME_SHIFT (18U)
/*! CPU1_CPU3_MSG_SCHEME - CPU1 to CPU3 Message Scheme */
#define CIU1_CIU_CPU_CPU1_CTRL_CPU1_CPU3_MSG_SCHEME(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU1_CTRL_CPU1_CPU3_MSG_SCHEME_SHIFT)) & CIU1_CIU_CPU_CPU1_CTRL_CPU1_CPU3_MSG_SCHEME_MASK)

#define CIU1_CIU_CPU_CPU1_CTRL_CPU3_RESET_INT_MASK (0x20000000U)
#define CIU1_CIU_CPU_CPU1_CTRL_CPU3_RESET_INT_SHIFT (29U)
/*! CPU3_RESET_INT - cpu1 fw reset cpu3 */
#define CIU1_CIU_CPU_CPU1_CTRL_CPU3_RESET_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU1_CTRL_CPU3_RESET_INT_SHIFT)) & CIU1_CIU_CPU_CPU1_CTRL_CPU3_RESET_INT_MASK)

#define CIU1_CIU_CPU_CPU1_CTRL_CPU2_RESET_INT_MASK (0x80000000U)
#define CIU1_CIU_CPU_CPU1_CTRL_CPU2_RESET_INT_SHIFT (31U)
/*! CPU2_RESET_INT - cpu1 fw reset cpu2 */
#define CIU1_CIU_CPU_CPU1_CTRL_CPU2_RESET_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU1_CTRL_CPU2_RESET_INT_SHIFT)) & CIU1_CIU_CPU_CPU1_CTRL_CPU2_RESET_INT_MASK)
/*! @} */

/*! @name CIU_CPU_CPU1_ACCESS_CTRL - CPU1 access control register */
/*! @{ */

#define CIU1_CIU_CPU_CPU1_ACCESS_CTRL_CPU1_ACCESS_CTRL_MASK (0xFFFFFFFFU)
#define CIU1_CIU_CPU_CPU1_ACCESS_CTRL_CPU1_ACCESS_CTRL_SHIFT (0U)
/*! CPU1_ACCESS_CTRL - CPU1 will read CIU_CPU_CPU1_ACCESS_CRTL and set CIU_CPU1_ACCESS_CRTL bit.
 *    After writing the bit, the CPU1 will check if the CIU_CPU_CPU1_ACCESS_CRTL bit is set or not. If
 *    set then CPU1 will access the resource (for e.g CSU).
 */
#define CIU1_CIU_CPU_CPU1_ACCESS_CTRL_CPU1_ACCESS_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU1_ACCESS_CTRL_CPU1_ACCESS_CTRL_SHIFT)) & CIU1_CIU_CPU_CPU1_ACCESS_CTRL_CPU1_ACCESS_CTRL_MASK)
/*! @} */

/*! @name CIU_CPU_CPU2_ACCESS_CTRL - CPU2 access control register */
/*! @{ */

#define CIU1_CIU_CPU_CPU2_ACCESS_CTRL_CPU2_ACCESS_CTRL_MASK (0xFFFFFFFFU)
#define CIU1_CIU_CPU_CPU2_ACCESS_CTRL_CPU2_ACCESS_CTRL_SHIFT (0U)
/*! CPU2_ACCESS_CTRL - CPU2 will read CIU_CPU_CPU2_ACCESS_CRTL and set CIU_CPU2_ACCESS_CRTL bit.
 *    After writing the bit, the CPU2 will check if the CIU_CPU_CPU2_ACCESS_CRTL bit is set or not. If
 *    set then CPU2 will access the resource (for e.g CSU).
 */
#define CIU1_CIU_CPU_CPU2_ACCESS_CTRL_CPU2_ACCESS_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU2_ACCESS_CTRL_CPU2_ACCESS_CTRL_SHIFT)) & CIU1_CIU_CPU_CPU2_ACCESS_CTRL_CPU2_ACCESS_CTRL_MASK)
/*! @} */

/*! @name CIU_CPU_CPU3_ACCESS_CTRL - CPU3 access control register */
/*! @{ */

#define CIU1_CIU_CPU_CPU3_ACCESS_CTRL_CPU3_ACCESS_CTRL_MASK (0xFFFFFFFFU)
#define CIU1_CIU_CPU_CPU3_ACCESS_CTRL_CPU3_ACCESS_CTRL_SHIFT (0U)
/*! CPU3_ACCESS_CTRL - CPU3 will read CIU_CPU_CPU3_ACCESS_CRTL and set CIU_CPU3_ACCESS_CRTL bit.
 *    After writing the bit, the CPU3 will check if the CIU_CPU_CPU3_ACCESS_CRTL bit is set or not. If
 *    set then CPU3 will access the resource (for e.g CSU).
 */
#define CIU1_CIU_CPU_CPU3_ACCESS_CTRL_CPU3_ACCESS_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU3_ACCESS_CTRL_CPU3_ACCESS_CTRL_SHIFT)) & CIU1_CIU_CPU_CPU3_ACCESS_CTRL_CPU3_ACCESS_CTRL_MASK)
/*! @} */

/*! @name CIU_CPU_CPU1_DBG_STAT1 - CPU1 debug register1 */
/*! @{ */

#define CIU1_CIU_CPU_CPU1_DBG_STAT1_CPU1_RO_STATUS_MASK (0xFFFFFFFFU)
#define CIU1_CIU_CPU_CPU1_DBG_STAT1_CPU1_RO_STATUS_SHIFT (0U)
/*! CPU1_RO_STATUS - cpu1 debug output */
#define CIU1_CIU_CPU_CPU1_DBG_STAT1_CPU1_RO_STATUS(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_CPU1_DBG_STAT1_CPU1_RO_STATUS_SHIFT)) & CIU1_CIU_CPU_CPU1_DBG_STAT1_CPU1_RO_STATUS_MASK)
/*! @} */

/*! @name CIU_CPU1_CPU2_FW_DWLD_CTRL - CPUs FW dwld control register */
/*! @{ */

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_FW_DWLD_INFO_VALID_MASK (0x1U)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_FW_DWLD_INFO_VALID_SHIFT (0U)
/*! FW_DWLD_INFO_VALID - After writing the bit[8] and bit[16], the CPU1 writes valid bit to indicate
 *    to cpu2 that information is valid and ready to use. CPU2 polls this bit and once set take
 *    appropriate action based on bit[8] and bit[16]
 */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_FW_DWLD_INFO_VALID(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_FW_DWLD_INFO_VALID_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_FW_DWLD_INFO_VALID_MASK)

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SKETCH_MASK (0xFEU)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SKETCH_SHIFT (1U)
/*! PARALLEL_DWLD_SKETCH - SKETCH register for Parallel download */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SKETCH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SKETCH_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SKETCH_MASK)

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_MASK (0x100U)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SHIFT (8U)
/*! PARALLEL_DWLD - For the parallel mode of FW download, CPU1 assert this bit to provide information to CPU2. */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_PARALLEL_DWLD_MASK)

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_SKETCH_MASK (0xFE00U)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_SKETCH_SHIFT (9U)
/*! CPU2_FW_DWLD_SKETCH - Sketch registers for Chip Download */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_SKETCH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_SKETCH_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_SKETCH_MASK)

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_DONE_MASK (0x10000U)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_DONE_SHIFT (16U)
/*! CPU2_FW_DWLD_DONE - For the serial mode of FW download, when CPU1 done with FW download for CPU2
 *    also, then it assert this bit. After this CPU2 can jump to it's ITCM to start execution
 */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_DONE_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CPU2_FW_DWLD_DONE_MASK)

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SKETCH_MASK (0x7FFE0000U)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SKETCH_SHIFT (17U)
/*! CHIP_INIT_DONE_SKETCH - Sketch registers for Chip Init */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SKETCH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SKETCH_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SKETCH_MASK)

#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_MASK (0x80000000U)
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SHIFT (31U)
/*! CHIP_INIT_DONE - After the FW is downloaded (serial or parallel), the CPU1 does the chip init
 *    and set this bit. It is used later by boot code in case the FW crash happen, then it doesn't
 *    need to do chip init again
 */
#define CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_SHIFT)) & CIU1_CIU_CPU1_CPU2_FW_DWLD_CTRL_CHIP_INIT_DONE_MASK)
/*! @} */

/*! @name CIU_CPU_COMM0 - CPU Communication reserved0 */
/*! @{ */

#define CIU1_CIU_CPU_COMM0_CPU_COMM0_MASK        (0xFFFFFFFFU)
#define CIU1_CIU_CPU_COMM0_CPU_COMM0_SHIFT       (0U)
/*! CPU_COMM0 - CPUs extra reserved communication reg to be used by FW */
#define CIU1_CIU_CPU_COMM0_CPU_COMM0(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_COMM0_CPU_COMM0_SHIFT)) & CIU1_CIU_CPU_COMM0_CPU_COMM0_MASK)
/*! @} */

/*! @name CIU_CPU_COMM1 - CPU Communication reserved1 */
/*! @{ */

#define CIU1_CIU_CPU_COMM1_CPU_COMM1_MASK        (0xFFFFFFFFU)
#define CIU1_CIU_CPU_COMM1_CPU_COMM1_SHIFT       (0U)
/*! CPU_COMM1 - CPUs extra reserved communication reg to be used by FW */
#define CIU1_CIU_CPU_COMM1_CPU_COMM1(x)          (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_COMM1_CPU_COMM1_SHIFT)) & CIU1_CIU_CPU_COMM1_CPU_COMM1_MASK)
/*! @} */

/*! @name CIU_CPU_ECO_CTRL - CPU ECO control */
/*! @{ */

#define CIU1_CIU_CPU_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_CPU_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_CPU_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_CPU_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_RFU_CTRL - RFU Control and Status */
/*! @{ */

#define CIU1_CIU_RFU_CTRL_APB_EN_5G_MASK         (0x2U)
#define CIU1_CIU_RFU_CTRL_APB_EN_5G_SHIFT        (1U)
/*! APB_EN_5G - Enable RFU5G APB Interface for Register Programming */
#define CIU1_CIU_RFU_CTRL_APB_EN_5G(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_CTRL_APB_EN_5G_SHIFT)) & CIU1_CIU_RFU_CTRL_APB_EN_5G_MASK)

#define CIU1_CIU_RFU_CTRL_APB_DWORD_SEL_MASK     (0x4U)
#define CIU1_CIU_RFU_CTRL_APB_DWORD_SEL_SHIFT    (2U)
/*! APB_DWORD_SEL - RFU APB DWORD Select */
#define CIU1_CIU_RFU_CTRL_APB_DWORD_SEL(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_CTRL_APB_DWORD_SEL_SHIFT)) & CIU1_CIU_RFU_CTRL_APB_DWORD_SEL_MASK)

#define CIU1_CIU_RFU_CTRL_APB_EN_2G_MASK         (0x8U)
#define CIU1_CIU_RFU_CTRL_APB_EN_2G_SHIFT        (3U)
/*! APB_EN_2G - Enable RFU2G APB Interface for Register Programming */
#define CIU1_CIU_RFU_CTRL_APB_EN_2G(x)           (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_CTRL_APB_EN_2G_SHIFT)) & CIU1_CIU_RFU_CTRL_APB_EN_2G_MASK)

#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_2G_A_MASK  (0x20000000U)
#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_2G_A_SHIFT (29U)
/*! RFU_TRX_RDY_2G_A - RFU2G channel A Rdy Status */
#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_2G_A(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_2G_A_SHIFT)) & CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_2G_A_MASK)

#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_A_MASK  (0x40000000U)
#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_A_SHIFT (30U)
/*! RFU_TRX_RDY_5G_A - RFU5G channel A Rdy Status */
#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_A(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_A_SHIFT)) & CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_A_MASK)

#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_B_MASK  (0x80000000U)
#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_B_SHIFT (31U)
/*! RFU_TRX_RDY_5G_B - RFU5G channel B Rdy Status */
#define CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_B(x)    (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_B_SHIFT)) & CIU1_CIU_RFU_CTRL_RFU_TRX_RDY_5G_B_MASK)
/*! @} */

/*! @name CIU_RFU_EXTRA_PORT - RFU Extra Port Connection */
/*! @{ */

#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_A_MASK (0xFFU)
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_A_SHIFT (0U)
/*! SOC_RFU5G_EXTRA_A - SOC_RFU5G_EXTRA_A[7:0] */
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_A(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_A_SHIFT)) & CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_A_MASK)

#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_B_MASK (0xFF00U)
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_B_SHIFT (8U)
/*! SOC_RFU5G_EXTRA_B - SOC_RFU5G_EXTRA_B[7:0] */
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_B(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_B_SHIFT)) & CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU5G_EXTRA_B_MASK)

#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_A_MASK (0xFF0000U)
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_A_SHIFT (16U)
/*! SOC_RFU2G_EXTRA_A - SOC_RFU2G_EXTRA_A[7:0] */
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_A(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_A_SHIFT)) & CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_A_MASK)

#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_B_MASK (0xF000000U)
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_B_SHIFT (24U)
/*! SOC_RFU2G_EXTRA_B - SOC_RFU2G_EXTRA_B[3:0] */
#define CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_B(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_B_SHIFT)) & CIU1_CIU_RFU_EXTRA_PORT_SOC_RFU2G_EXTRA_B_MASK)
/*! @} */

/*! @name CIU_RFU_ECO_CTRL - RFU ECO Control */
/*! @{ */

#define CIU1_CIU_RFU_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_RFU_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_RFU_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_RFU_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_RFU_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_MCI_EXTRA - MCI EXTRA Ports */
/*! @{ */

#define CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_OUT_MASK (0xFU)
#define CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_OUT_SHIFT (0U)
/*! CIU_MCI_EXTRA_OUT - Extra Ports to MCI */
#define CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_OUT(x)  (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_OUT_SHIFT)) & CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_OUT_MASK)

#define CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_IN_MASK (0xF0U)
#define CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_IN_SHIFT (4U)
/*! CIU_MCI_EXTRA_IN - Extra Ports from MCI */
#define CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_IN(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_IN_SHIFT)) & CIU1_CIU_MCI_EXTRA_CIU_MCI_EXTRA_IN_MASK)
/*! @} */

/*! @name CIU_BBUD_CTRL - BBUD Control Register */
/*! @{ */

#define CIU1_CIU_BBUD_CTRL_APB_EN_BBU1_MASK      (0x1U)
#define CIU1_CIU_BBUD_CTRL_APB_EN_BBU1_SHIFT     (0U)
/*! APB_EN_BBU1 - Enable BBUD1 APB Interface for Register Programming */
#define CIU1_CIU_BBUD_CTRL_APB_EN_BBU1(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_CTRL_APB_EN_BBU1_SHIFT)) & CIU1_CIU_BBUD_CTRL_APB_EN_BBU1_MASK)

#define CIU1_CIU_BBUD_CTRL_APB_DWORD_SEL_MASK    (0x2U)
#define CIU1_CIU_BBUD_CTRL_APB_DWORD_SEL_SHIFT   (1U)
/*! APB_DWORD_SEL - BBUD APB DWORD Select */
#define CIU1_CIU_BBUD_CTRL_APB_DWORD_SEL(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_CTRL_APB_DWORD_SEL_SHIFT)) & CIU1_CIU_BBUD_CTRL_APB_DWORD_SEL_MASK)

#define CIU1_CIU_BBUD_CTRL_BBUD_SPEC_SEL_MASK    (0x4U)
#define CIU1_CIU_BBUD_CTRL_BBUD_SPEC_SEL_SHIFT   (2U)
/*! BBUD_SPEC_SEL - BBUD SPEC CLK select */
#define CIU1_CIU_BBUD_CTRL_BBUD_SPEC_SEL(x)      (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_CTRL_BBUD_SPEC_SEL_SHIFT)) & CIU1_CIU_BBUD_CTRL_BBUD_SPEC_SEL_MASK)

#define CIU1_CIU_BBUD_CTRL_APB_EN_BBU2_MASK      (0x10U)
#define CIU1_CIU_BBUD_CTRL_APB_EN_BBU2_SHIFT     (4U)
/*! APB_EN_BBU2 - Enable BBUD2 APB Interface for Register Programming */
#define CIU1_CIU_BBUD_CTRL_APB_EN_BBU2(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_CTRL_APB_EN_BBU2_SHIFT)) & CIU1_CIU_BBUD_CTRL_APB_EN_BBU2_MASK)

#define CIU1_CIU_BBUD_CTRL_PPS_IN_SEL_MASK       (0x300U)
#define CIU1_CIU_BBUD_CTRL_PPS_IN_SEL_SHIFT      (8U)
/*! PPS_IN_SEL - BBUD PPS input select */
#define CIU1_CIU_BBUD_CTRL_PPS_IN_SEL(x)         (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_CTRL_PPS_IN_SEL_SHIFT)) & CIU1_CIU_BBUD_CTRL_PPS_IN_SEL_MASK)

#define CIU1_CIU_BBUD_CTRL_PTP_SYNC_PULSE_SEL_MASK (0x10000U)
#define CIU1_CIU_BBUD_CTRL_PTP_SYNC_PULSE_SEL_SHIFT (16U)
/*! PTP_SYNC_PULSE_SEL - Mux select control to select between */
#define CIU1_CIU_BBUD_CTRL_PTP_SYNC_PULSE_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_CTRL_PTP_SYNC_PULSE_SEL_SHIFT)) & CIU1_CIU_BBUD_CTRL_PTP_SYNC_PULSE_SEL_MASK)
/*! @} */

/*! @name CIU_BBUD_EXTRA_PORT - BBUD Extra Port Connection */
/*! @{ */

#define CIU1_CIU_BBUD_EXTRA_PORT_SOC_BBUD_EXTRA_MASK (0xFFU)
#define CIU1_CIU_BBUD_EXTRA_PORT_SOC_BBUD_EXTRA_SHIFT (0U)
/*! SOC_BBUD_EXTRA - SOC_BBUD_EXTRA[7:0] */
#define CIU1_CIU_BBUD_EXTRA_PORT_SOC_BBUD_EXTRA(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_EXTRA_PORT_SOC_BBUD_EXTRA_SHIFT)) & CIU1_CIU_BBUD_EXTRA_PORT_SOC_BBUD_EXTRA_MASK)
/*! @} */

/*! @name CIU_BBUD_PTP_INTR_CTRL - PTP Input capture interrupt control */
/*! @{ */

#define CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_INTR_MASK_MASK (0x1U)
#define CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_INTR_MASK_SHIFT (0U)
/*! PTP_INPUTCAPTURE_INTR_MASK - Mask control for ptp_inputcaptue interrupt generation */
#define CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_INTR_MASK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_INTR_MASK_SHIFT)) & CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_INTR_MASK_MASK)

#define CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_EDGE_SEL_MASK (0x80U)
#define CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_EDGE_SEL_SHIFT (7U)
/*! PTP_INPUTCAPTURE_EDGE_SEL - Select edge for generation of PTP interrupt */
#define CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_EDGE_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_EDGE_SEL_SHIFT)) & CIU1_CIU_BBUD_PTP_INTR_CTRL_PTP_INPUTCAPTURE_EDGE_SEL_MASK)
/*! @} */

/*! @name CIU_BBUD_ECO_CTRL - BBUD ECO Control */
/*! @{ */

#define CIU1_CIU_BBUD_ECO_CTRL_ECO_BITS_MASK     (0xFFFFFFFFU)
#define CIU1_CIU_BBUD_ECO_CTRL_ECO_BITS_SHIFT    (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_BBUD_ECO_CTRL_ECO_BITS(x)       (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_BBUD_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_BBUD_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_AIU_CTRL - AIU Control Register */
/*! @{ */

#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_EN_MASK   (0x4U)
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_EN_SHIFT  (2U)
/*! AIU_MCLK_NCO_EN - AIU_MCLK NCO */
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_EN(x)     (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_EN_SHIFT)) & CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_EN_MASK)

#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_TYPE_MASK (0x8U)
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_TYPE_SHIFT (3U)
/*! AIU_MCLK_NCO_TYPE - NCO */
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_TYPE(x)   (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_TYPE_SHIFT)) & CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_TYPE_MASK)

#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_INPUT_SEL_MASK (0x10U)
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_INPUT_SEL_SHIFT (4U)
/*! AIU_MCLK_NCO_INPUT_SEL - AIU NCO Input */
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_INPUT_SEL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_INPUT_SEL_SHIFT)) & CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_INPUT_SEL_MASK)

#define CIU1_CIU_AIU_CTRL_AIU_MCLK_SEL_MASK      (0x20U)
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_SEL_SHIFT     (5U)
/*! AIU_MCLK_SEL - AIU MCLK */
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_SEL(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AIU_CTRL_AIU_MCLK_SEL_SHIFT)) & CIU1_CIU_AIU_CTRL_AIU_MCLK_SEL_MASK)

#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_STEP_SIZE_MASK (0xFFFFFF00U)
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_STEP_SIZE_SHIFT (8U)
/*! AIU_MCLK_NCO_STEP_SIZE - AIU NCO Step Size */
#define CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_STEP_SIZE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_STEP_SIZE_SHIFT)) & CIU1_CIU_AIU_CTRL_AIU_MCLK_NCO_STEP_SIZE_MASK)
/*! @} */

/*! @name CIU_AIU_ECO_CTRL - AIU ECO Control */
/*! @{ */

#define CIU1_CIU_AIU_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_AIU_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_AIU_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_AIU_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_AIU_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU_CBU_ECO_CTRL - CBU ECO Control */
/*! @{ */

#define CIU1_CIU_CBU_ECO_CTRL_ECO_BITS_MASK      (0xFFFFFFFFU)
#define CIU1_CIU_CBU_ECO_CTRL_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_CBU_ECO_CTRL_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CBU_ECO_CTRL_ECO_BITS_SHIFT)) & CIU1_CIU_CBU_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU1_WR_MSG_TO_CPU3 - CPU1(WL) write message to CPU3(MCI) (0xAF0-0xAF4 IMU register access by CPU1) */
/*! @{ */

#define CIU1_CIU1_IMU_CPU1_WR_MSG_TO_CPU3_CPU1_WR_MSG_CPU3_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_IMU_CPU1_WR_MSG_TO_CPU3_CPU1_WR_MSG_CPU3_SHIFT (0U)
/*! CPU1_WR_MSG_CPU3 - Write cpu1 message data to CPU3 (push to FIFO) */
#define CIU1_CIU1_IMU_CPU1_WR_MSG_TO_CPU3_CPU1_WR_MSG_CPU3(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_WR_MSG_TO_CPU3_CPU1_WR_MSG_CPU3_SHIFT)) & CIU1_CIU1_IMU_CPU1_WR_MSG_TO_CPU3_CPU1_WR_MSG_CPU3_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU1_RD_MSG_FROM_CPU3 - cpu1 read message from CPU3 */
/*! @{ */

#define CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_CPU1_RD_MSG_CPU3_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_CPU1_RD_MSG_CPU3_SHIFT (0U)
/*! CPU1_RD_MSG_CPU3 - cpu1 read message data from CPU3 (pop from FIFO) */
#define CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_CPU1_RD_MSG_CPU3(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_CPU1_RD_MSG_CPU3_SHIFT)) & CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_CPU1_RD_MSG_CPU3_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS - cpu1 to CPU3 message FIFO status */
/*! @{ */

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_LOCKED_MASK (0x1U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_LOCKED_SHIFT (0U)
/*! CPU1_TO_CPU3_MSG_FIFO_LOCKED - cpu1_to_cpu3_msg_fifo_locked */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_LOCKED(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_LOCKED_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_LOCKED_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_ALMOST_FULL_MASK (0x2U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_ALMOST_FULL_SHIFT (1U)
/*! CPU1_TO_CPU3_MSG_FIFO_ALMOST_FULL - cpu1_to_cpu3_msg_fifo_almost_full (based upon FIFO watermark) */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_ALMOST_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_ALMOST_FULL_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_ALMOST_FULL_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_FULL_MASK (0x4U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_FULL_SHIFT (2U)
/*! CPU1_TO_CPU3_MSG_FIFO_FULL - cpu1_to_cpu3_msg_fifo_full (based upon FIFO depth) */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_FULL_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_FULL_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_EMPTY_MASK (0x8U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_EMPTY_SHIFT (3U)
/*! CPU1_TO_CPU3_MSG_FIFO_EMPTY - cpu1_to_cpu3_msg_fifo_empty */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_EMPTY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_EMPTY_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_EMPTY_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_COUNT_MASK (0x1F0U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_COUNT_SHIFT (4U)
/*! CPU1_TO_CPU3_MSG_COUNT - cpu1_to_cpu3_msg_count */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_COUNT_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_COUNT_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_WR_PTR_MASK (0xF0000U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_WR_PTR_SHIFT (16U)
/*! CPU1_TO_CPU3_MSG_FIFO_WR_PTR - cpu1 to cpu3 msg fifo write pointer for debug */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_WR_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_WR_PTR_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_WR_PTR_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_RD_PTR_MASK (0xF00000U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_RD_PTR_SHIFT (20U)
/*! CPU1_TO_CPU3_MSG_FIFO_RD_PTR - cpu1 to cpu3 msg fifo read pointer for debug */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_RD_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_RD_PTR_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_STATUS_CPU1_TO_CPU3_MSG_FIFO_RD_PTR_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL - cpu1 to CPU3 message FIFO control */
/*! @{ */

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_RDY_INT_CLR_MASK (0x1U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_RDY_INT_CLR_SHIFT (0U)
/*! CPU1_MSG_RDY_INT_CLR - Writing 1 to this bit will clear message ready interrupt to CPU1 (self clear bit) */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_RDY_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_RDY_INT_CLR_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_RDY_INT_CLR_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_SP_AV_INT_CLR_MASK (0x100U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_SP_AV_INT_CLR_SHIFT (8U)
/*! CPU1_MSG_SP_AV_INT_CLR - Writing 1 to this bit will clear message space available interrupt to CPU1 (self clear bit) */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_SP_AV_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_SP_AV_INT_CLR_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_MSG_SP_AV_INT_CLR_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FLUSH_MASK (0x10000U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FLUSH_SHIFT (16U)
/*! CPU1_TO_CPU3_MSG_FIFO_FLUSH - Writing 1 to this bit will flush cpu1_to_cpu3 message fifo */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FLUSH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FLUSH_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FLUSH_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_WAIT_FOR_ACK_MASK (0x20000U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_WAIT_FOR_ACK_SHIFT (17U)
/*! CPU1_WAIT_FOR_ACK - CPU1 Wait for Acknowledgment */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_WAIT_FOR_ACK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_WAIT_FOR_ACK_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_WAIT_FOR_ACK_MASK)

#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FULL_WATERMARK_MASK (0xF00000U)
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FULL_WATERMARK_SHIFT (20U)
/*! CPU1_TO_CPU3_MSG_FIFO_FULL_WATERMARK - cpu1_to_cpu3 message fifo full watermark (space avail intr based upon it) */
#define CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FULL_WATERMARK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FULL_WATERMARK_SHIFT)) & CIU1_CIU1_IMU_CPU1_CPU3_MSG_FIFO_CNTL_CPU1_TO_CPU3_MSG_FIFO_FULL_WATERMARK_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG - CPU3 last message read (from cpu1) */
/*! @{ */

#define CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG_CPU3_RD_MSG_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG_CPU3_RD_MSG_SHIFT (0U)
/*! CPU3_RD_MSG - CPU3 last message read (from cpu1) */
#define CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG_CPU3_RD_MSG(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG_CPU3_RD_MSG_SHIFT)) & CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_VAL_DBG_CPU3_RD_MSG_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU3_WR_MSG_TO_CPU1 - CPU3 write message to cpu1 (0xB04-0xB14 IMU register access by CPU3) */
/*! @{ */

#define CIU1_CIU1_IMU_CPU3_WR_MSG_TO_CPU1_CPU3_WR_MSG_CPU1_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_IMU_CPU3_WR_MSG_TO_CPU1_CPU3_WR_MSG_CPU1_SHIFT (0U)
/*! CPU3_WR_MSG_CPU1 - Write CPU3 message data to cpu1 (push to FIFO) */
#define CIU1_CIU1_IMU_CPU3_WR_MSG_TO_CPU1_CPU3_WR_MSG_CPU1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_WR_MSG_TO_CPU1_CPU3_WR_MSG_CPU1_SHIFT)) & CIU1_CIU1_IMU_CPU3_WR_MSG_TO_CPU1_CPU3_WR_MSG_CPU1_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU3_RD_MSG_FROM_CPU1 - CPU3 read message from cpu1 */
/*! @{ */

#define CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_CPU3_RD_MSG_CPU1_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_CPU3_RD_MSG_CPU1_SHIFT (0U)
/*! CPU3_RD_MSG_CPU1 - CPU3 read message data from cpu1 (pop from FIFO) */
#define CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_CPU3_RD_MSG_CPU1(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_CPU3_RD_MSG_CPU1_SHIFT)) & CIU1_CIU1_IMU_CPU3_RD_MSG_FROM_CPU1_CPU3_RD_MSG_CPU1_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS - CPU3 to cpu1 message FIFO status */
/*! @{ */

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_LOCKED_MASK (0x1U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_LOCKED_SHIFT (0U)
/*! CPU3_TO_CPU1_MSG_FIFO_LOCKED - cpu3_to_cpu1_msg_fifo_locked */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_LOCKED(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_LOCKED_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_LOCKED_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_ALMOST_FULL_MASK (0x2U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_ALMOST_FULL_SHIFT (1U)
/*! CPU3_TO_CPU1_MSG_FIFO_ALMOST_FULL - cpu3_to_cpu1_msg_fifo_almost_full (based upon FIFO watermark) */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_ALMOST_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_ALMOST_FULL_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_ALMOST_FULL_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_FULL_MASK (0x4U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_FULL_SHIFT (2U)
/*! CPU3_TO_CPU1_MSG_FIFO_FULL - cpu3_to_cpu1_msg_fifo_full (based upon FIFO depth) */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_FULL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_FULL_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_FULL_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_EMPTY_MASK (0x8U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_EMPTY_SHIFT (3U)
/*! CPU3_TO_CPU1_MSG_FIFO_EMPTY - cpu3_to_cpu1_msg_fifo_empty */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_EMPTY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_EMPTY_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_EMPTY_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_COUNT_MASK (0x1F0U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_COUNT_SHIFT (4U)
/*! CPU3_TO_CPU1_MSG_COUNT - cpu3_to_cpu1_msg_count */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_COUNT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_COUNT_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_COUNT_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_WR_PTR_MASK (0xF0000U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_WR_PTR_SHIFT (16U)
/*! CPU3_TO_CPU1_MSG_FIFO_WR_PTR - cpu3 to cpu1 msg fifo write pointer for debug */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_WR_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_WR_PTR_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_WR_PTR_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_RD_PTR_MASK (0xF00000U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_RD_PTR_SHIFT (20U)
/*! CPU3_TO_CPU1_MSG_FIFO_RD_PTR - cpu3 to cpu1 msg fifo read pointer for debug */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_RD_PTR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_RD_PTR_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_STATUS_CPU3_TO_CPU1_MSG_FIFO_RD_PTR_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL - CPU3 to cpu1 message FIFO control */
/*! @{ */

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_MASK (0x1U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_SHIFT (0U)
/*! CPU3_MSG_RDY_INT_CLR - Writing 1 to this bit will clear message ready interrupt to cpu3 (self clear bit) */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_RDY_INT_CLR_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_MASK (0x100U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_SHIFT (8U)
/*! CPU3_MSG_SP_AV_INT_CLR - Writing 1 to this bit will clear message space available interrupt to CPU3 (self clear bit) */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_MSG_SP_AV_INT_CLR_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FLUSH_MASK (0x10000U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FLUSH_SHIFT (16U)
/*! CPU3_TO_CPU1_MSG_FIFO_FLUSH - Writing 1 to this bit will flush cpu3_to_cpu1 message fifo */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FLUSH(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FLUSH_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FLUSH_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_MASK (0x20000U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_SHIFT (17U)
/*! CPU3_WAIT_FOR_ACK - CPU3 Wait for Acknowledgment */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_WAIT_FOR_ACK_MASK)

#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FULL_WATERMARK_MASK (0xF00000U)
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FULL_WATERMARK_SHIFT (20U)
/*! CPU3_TO_CPU1_MSG_FIFO_FULL_WATERMARK - cpu3_to_cpu1 message fifo full watermark (space avail intr based upon it) */
#define CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FULL_WATERMARK(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FULL_WATERMARK_SHIFT)) & CIU1_CIU1_IMU_CPU3_CPU1_MSG_FIFO_CNTL_CPU3_TO_CPU1_MSG_FIFO_FULL_WATERMARK_MASK)
/*! @} */

/*! @name CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG - cpu1 last message read (from cpu3) */
/*! @{ */

#define CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG_CPU1_RD_MSG_MASK (0xFFFFFFFFU)
#define CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG_CPU1_RD_MSG_SHIFT (0U)
/*! CPU1_RD_MSG - cpu1 last message read (from cpu3) */
#define CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG_CPU1_RD_MSG(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG_CPU1_RD_MSG_SHIFT)) & CIU1_CIU1_IMU_CPU1_RD_MSG_FROM_CPU3_VAL_DBG_CPU1_RD_MSG_MASK)
/*! @} */

/*! @name CIU_CPU1_CPU3_MSG_CTRL - CPU1_CPU3 message register */
/*! @{ */

#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_RDY_MASK (0x1U)
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_RDY_SHIFT (0U)
/*! CPU1_TO_CPU3_MSG_RDY - CPU1 Message for CPU3 is ready. This is self clearing bit. The CPU1
 *    writes 1 to indicate that message for CPU3 is ready. This generates an Interrupt to CPU3 via APU.
 *    This is old schema and we should use IMU based scheme.
 */
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_RDY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_RDY_SHIFT)) & CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_RDY_MASK)

#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_RDY_MASK (0x2U)
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_RDY_SHIFT (1U)
/*! CPU3_TO_CPU1_MSG_RDY - CPU3 Message for CPU1 is ready. This is self clearing bit. The CPU3
 *    writes 1 to indicate that message for CPU1 is ready. This generates an Interrupt to CPU1 via APU.
 *    This is old schema and we should use IMU based scheme.
 */
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_RDY(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_RDY_SHIFT)) & CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_RDY_MASK)

#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_PROCESS_DONE_MASK (0x100U)
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_PROCESS_DONE_SHIFT (8U)
/*! CPU1_TO_CPU3_MSG_PROCESS_DONE - CPU1 Message for CPU3 has been read by CPU3 and executed. This
 *    is self clearing bit. The CPU3 writes 1 to indicate that message sent by CPU1 is executed. This
 *    generates an Interrupt to CPU1 via CIU1.
 */
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_PROCESS_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_PROCESS_DONE_SHIFT)) & CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU1_TO_CPU3_MSG_PROCESS_DONE_MASK)

#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_PROCESS_DONE_MASK (0x200U)
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_PROCESS_DONE_SHIFT (9U)
/*! CPU3_TO_CPU1_MSG_PROCESS_DONE - CPU3 Message for CPU1 has been read by CPU1 and executed. This
 *    is self clearing bit. The CPU1 writes 1 to indicate that message send by CPU3 is executed. This
 *    generates an Interrupt to CPU3 via CIU1.
 */
#define CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_PROCESS_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_PROCESS_DONE_SHIFT)) & CIU1_CIU_CPU1_CPU3_MSG_CTRL_CPU3_TO_CPU1_MSG_PROCESS_DONE_MASK)
/*! @} */

/*! @name CIU1_CPU3_WAKEUP_CTRL - CIU1 register to wakeup CPU3 */
/*! @{ */

#define CIU1_CIU1_CPU3_WAKEUP_CTRL_CPU3_WAKEUP_CTRL_MASK (0x1U)
#define CIU1_CIU1_CPU3_WAKEUP_CTRL_CPU3_WAKEUP_CTRL_SHIFT (0U)
/*! CPU3_WAKEUP_CTRL - CPU3 Wakeup Control Register. S/W Write 1 to generate a wake up interrupt to
 *    CPU3.Clear by S/W once mci_wl_wakeup_done[1] interrupt is received from CPU3.
 */
#define CIU1_CIU1_CPU3_WAKEUP_CTRL_CPU3_WAKEUP_CTRL(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU3_WAKEUP_CTRL_CPU3_WAKEUP_CTRL_SHIFT)) & CIU1_CIU1_CPU3_WAKEUP_CTRL_CPU3_WAKEUP_CTRL_MASK)
/*! @} */

/*! @name CIU1_CPU1_WAKEUP_DONE - Wakeup done Control Register to CPU3 */
/*! @{ */

#define CIU1_CIU1_CPU1_WAKEUP_DONE_CPU1_WAKEUP_DONE_MASK (0xFFU)
#define CIU1_CIU1_CPU1_WAKEUP_DONE_CPU1_WAKEUP_DONE_SHIFT (0U)
/*! CPU1_WAKEUP_DONE - CPU1 Wakeup is done . This bit is set to 1 by S/W when CPU3 wakesup CPU1.This
 *    is self clearing bit. This generates an interrupt to CPU3 via wl_mci_wakeup_done[7:0] signal.
 */
#define CIU1_CIU1_CPU1_WAKEUP_DONE_CPU1_WAKEUP_DONE(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU1_WAKEUP_DONE_CPU1_WAKEUP_DONE_SHIFT)) & CIU1_CIU1_CPU1_WAKEUP_DONE_CPU1_WAKEUP_DONE_MASK)
/*! @} */

/*! @name CIU1_CPU3_NS_GP_INT - Non Secure region GP interrupt to CPU3 */
/*! @{ */

#define CIU1_CIU1_CPU3_NS_GP_INT_CPU1_CPU3_GP_NS_INT_MASK (0x3U)
#define CIU1_CIU1_CPU3_NS_GP_INT_CPU1_CPU3_GP_NS_INT_SHIFT (0U)
/*! CPU1_CPU3_GP_NS_INT - General Purpose interrupt to CPU3 from non secure registers */
#define CIU1_CIU1_CPU3_NS_GP_INT_CPU1_CPU3_GP_NS_INT(x) (((uint32_t)(((uint32_t)(x)) << CIU1_CIU1_CPU3_NS_GP_INT_CPU1_CPU3_GP_NS_INT_SHIFT)) & CIU1_CIU1_CPU3_NS_GP_INT_CPU1_CPU3_GP_NS_INT_MASK)
/*! @} */

/*! @name CIU_IMU_ECO_BITS - IMU ECO Control */
/*! @{ */

#define CIU1_CIU_IMU_ECO_BITS_ECO_BITS_MASK      (0xFFFFU)
#define CIU1_CIU_IMU_ECO_BITS_ECO_BITS_SHIFT     (0U)
/*! ECO_BITS - Reserved for ECOs */
#define CIU1_CIU_IMU_ECO_BITS_ECO_BITS(x)        (((uint32_t)(((uint32_t)(x)) << CIU1_CIU_IMU_ECO_BITS_ECO_BITS_SHIFT)) & CIU1_CIU_IMU_ECO_BITS_ECO_BITS_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group CIU1_Register_Masks */


/*!
 * @}
 */ /* end of group CIU1_Peripheral_Access_Layer */


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


#endif  /* CIU1_H_ */

