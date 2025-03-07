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
**         CMSIS Peripheral Access Layer for SOCCIU
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
 * @file SOCCIU.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for SOCCIU
 *
 * CMSIS Peripheral Access Layer for SOCCIU
 */

#if !defined(SOCCIU_H_)
#define SOCCIU_H_                                /**< Symbol preventing repeated inclusion */

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
   -- SOCCIU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SOCCIU_Peripheral_Access_Layer SOCCIU Peripheral Access Layer
 * @{
 */

/** SOCCIU - Register Layout Typedef */
typedef struct {
  __IO uint32_t PAD_CONFIG0;                       /**< ", offset: 0x0 */
       uint8_t RESERVED_0[4];
  __IO uint32_t PAD_PWRDOWN_CTRL0;                 /**< Pad Power-down Control 0, offset: 0x8 */
  __IO uint32_t PAD_PWRDOWN_CTRL1;                 /**< Pad Power-down Control 1, offset: 0xC */
  __IO uint32_t PAD_PWRDOWN_CTRL2;                 /**< Pad Power-down Control 2, offset: 0x10 */
  __IO uint32_t PAD_PWRDOWN_CTRL3;                 /**< Pad Power-down Control 3, offset: 0x14 */
  __IO uint32_t PAD_PWRDOWN_CTRL4;                 /**< Pad Power-down Control 4, offset: 0x18 */
       uint8_t RESERVED_1[16];
  __IO uint32_t PAD_PWRDOWN_CTRL5;                 /**< Pad Power-down Control 5, offset: 0x2C */
  __IO uint32_t PAD_RF_SW_SLP_CONFIG;              /**< RF Switch Pad Sleep Mode Configuration, offset: 0x30 */
  __IO uint32_t PAD_ATEST_SW_SLP_CONFIG;           /**< ATEST Pad Sleep Mode Configuration, offset: 0x34 */
       uint8_t RESERVED_2[20];
  __IO uint32_t SR_CONFIG0;                        /**< GPIO Slew Rate control, offset: 0x4C */
  __IO uint32_t SR_CONFIG1;                        /**< GPIO Slew Rate control, offset: 0x50 */
  __IO uint32_t SR_CONFIG2;                        /**< GPIO Slew Rate control, offset: 0x54 */
  __IO uint32_t SR_CONFIG3;                        /**< GPIO Slew Rate control, offset: 0x58 */
       uint8_t RESERVED_3[8];
  __IO uint32_t SR_CONFIG4;                        /**< GPIO Slew Rate control, offset: 0x64 */
  __IO uint32_t PAD_WKUP0;                         /**< Pad Wakeup Mode Enable, offset: 0x68 */
       uint8_t RESERVED_4[4];
  __IO uint32_t PAD_PU_PD_EN0;                     /**< Pad Pull-up Pull-down Enable1, offset: 0x70 */
  __IO uint32_t PAD_PU_PD_EN1;                     /**< Pad Pull-up Pull-down Enable2, offset: 0x74 */
  __IO uint32_t PAD_PU_PD_EN2;                     /**< Pad Pull-up Pull-down Enable2, offset: 0x78 */
  __IO uint32_t PAD_PU_PD_EN3;                     /**< Pad Pull-up Pull-down Enable2, offset: 0x7C */
       uint8_t RESERVED_5[8];
  __IO uint32_t PAD_PU_PD_EN4;                     /**< Pad Pull-up Pull-down Enable2, offset: 0x88 */
  __IO uint32_t PAD_SLP_EN0;                       /**< Pad Sleep Mode Enable, offset: 0x8C */
  __IO uint32_t PAD_SLP_EN1;                       /**< Pad Sleep Mode Enable, offset: 0x90 */
       uint8_t RESERVED_6[4];
  __IO uint32_t PAD_SLP_VAL0;                      /**< Pad Sleep Mode Value, offset: 0x98 */
  __IO uint32_t PAD_SLP_VAL1;                      /**< Pad Sleep Mode Value, offset: 0x9C */
       uint8_t RESERVED_7[96];
  __I  uint32_t PSW_VD2_RDY0;                      /**< Power Switch VD2_RDY Status, offset: 0x100 */
  __IO uint32_t PSW_ECO_CTRL;                      /**< Power Switch ECO Control, offset: 0x104 */
  __IO uint32_t CLK_SW;                            /**< Clock Controls for SOC_CLK_TOP, offset: 0x108 */
  __IO uint32_t RST_SW;                            /**< Reset Controls for SOC_RESET_GEN, offset: 0x10C */
       uint8_t RESERVED_8[4];
  __I  uint32_t CHIP_INFO;                         /**< Chip Information, offset: 0x114 */
       uint8_t RESERVED_9[28];
  __IO uint32_t MCI_POWER_MODE_STATUS;             /**< MCI POWER MODE Status, offset: 0x134 */
  __I  uint32_t PSW_VD2_RDY1;                      /**< Power Switch VD2_RDY Status, offset: 0x138 */
       uint8_t RESERVED_10[4];
  __IO uint32_t WLAN_POWER_STATUS;                 /**< WLAN POWER Status, offset: 0x140 */
  __IO uint32_t BLE_POWER_STATUS;                  /**< BLE POWER Status, offset: 0x144 */
       uint8_t RESERVED_11[184];
  __IO uint32_t PAD_VREG_VSENSOR_CTRL;             /**< Vsensor and Vreg Pad Control, offset: 0x200 */
       uint8_t RESERVED_12[4];
  __IO uint32_t PAD_RF_VREG_VSENSOR_CTRL;          /**< RF Vsensor and Vreg Pad Control, offset: 0x208 */
       uint8_t RESERVED_13[16];
  __IO uint32_t PAD_SD_VREG_VSENSOR_CTRL;          /**< SD Vsensor and Vreg Pad Control, offset: 0x21C */
       uint8_t RESERVED_14[348];
  __IO uint32_t PAD_ECO_CTRL;                      /**< Pad ECO Control, offset: 0x37C */
       uint8_t RESERVED_15[164];
  __IO uint32_t TST_TSTBUS_CTRL1;                  /**< Testbus Mux Control1, offset: 0x424 */
  __IO uint32_t TST_TSTBUS_CTRL2;                  /**< Testbus Mux Control2, offset: 0x428 */
       uint8_t RESERVED_16[4];
  __I  uint32_t TST_CTRL;                          /**< Test Control, offset: 0x430 */
       uint8_t RESERVED_17[72];
  __IO uint32_t TST_ECO_CTRL;                      /**< Test ECO Control, offset: 0x47C */
       uint8_t RESERVED_18[8];
  __IO uint32_t DRO_CTRL;                          /**< DRO Control, offset: 0x488 */
  __I  uint32_t DRO_1_2_CNT;                       /**< DRO1 and DRO2 Counter Read back, offset: 0x48C */
  __I  uint32_t DRO_3_CNT;                         /**< DRO3 Counter Read back, offset: 0x490 */
       uint8_t RESERVED_19[4];
  __IO uint32_t DRO_PAR_SEL;                       /**< DRO Parallel Counter Selection, offset: 0x498 */
  __IO uint32_t CLK_SOCCLK_CTRL;                   /**< SOC Clock Control, offset: 0x49C */
       uint8_t RESERVED_20[96];
  __IO uint32_t PAD_SLP_PU_PD_DIS0;                /**< Pad Sleep Pullup and Pulldown Disable1, offset: 0x500 */
  __IO uint32_t PAD_SLP_PU_PD_DIS1;                /**< Pad Sleep Pullup and Pulldown Disable2, offset: 0x504 */
       uint8_t RESERVED_21[8];
  __IO uint32_t PAD_SLP_PU_PD_DIS2;                /**< Pad Sleep Pullup and Pulldown Disable4, offset: 0x510 */
  __IO uint32_t PAD_GPIO;                          /**< GPIO Enable, offset: 0x514 */
       uint8_t RESERVED_22[32];
  __IO uint32_t MCI_IOMUX_EN0;                     /**< mci_iomux_enable control for GPIO[31:28] and GPIO[21:0], offset: 0x538 */
  __IO uint32_t MCI_IOMUX_EN1;                     /**< mci_iomux_enable control for GPIO[63:32], offset: 0x53C */
       uint8_t RESERVED_23[840];
  __IO uint32_t CAU_CTRL;                          /**< CAU Control, offset: 0x888 */
       uint8_t RESERVED_24[24];
  __I  uint32_t SYSPLL_CTRL;                       /**< SYSPLL Control, offset: 0x8A4 */
} SOCCIU_Type;

/* ----------------------------------------------------------------------------
   -- SOCCIU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SOCCIU_Register_Masks SOCCIU Register Masks
 * @{
 */

/*! @name PAD_CONFIG0 - " */
/*! @{ */

#define SOCCIU_PAD_CONFIG0_ALL_PADS_TRISTATE_EN_MASK (0x1U)
#define SOCCIU_PAD_CONFIG0_ALL_PADS_TRISTATE_EN_SHIFT (0U)
/*! ALL_PADS_TRISTATE_EN - Enable/disable Control for Pad Tristate */
#define SOCCIU_PAD_CONFIG0_ALL_PADS_TRISTATE_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_ALL_PADS_TRISTATE_EN_SHIFT)) & SOCCIU_PAD_CONFIG0_ALL_PADS_TRISTATE_EN_MASK)

#define SOCCIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_MASK (0x2U)
#define SOCCIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_SHIFT (1U)
/*! PAD_PWRDOWN_LATCH - Enables the pd_sel latching */
#define SOCCIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_SHIFT)) & SOCCIU_PAD_CONFIG0_PAD_PWRDOWN_LATCH_MASK)

#define SOCCIU_PAD_CONFIG0_RF_CNTL0_ATEST_EN_MASK (0x8U)
#define SOCCIU_PAD_CONFIG0_RF_CNTL0_ATEST_EN_SHIFT (3U)
/*! RF_CNTL0_ATEST_EN - RF_CNTL0 pad ATEST mode Enable */
#define SOCCIU_PAD_CONFIG0_RF_CNTL0_ATEST_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_RF_CNTL0_ATEST_EN_SHIFT)) & SOCCIU_PAD_CONFIG0_RF_CNTL0_ATEST_EN_MASK)

#define SOCCIU_PAD_CONFIG0_RF_CNTL1_ATEST_EN_MASK (0x10U)
#define SOCCIU_PAD_CONFIG0_RF_CNTL1_ATEST_EN_SHIFT (4U)
/*! RF_CNTL1_ATEST_EN - RF_CNTL1 pad ATEST mode Enable */
#define SOCCIU_PAD_CONFIG0_RF_CNTL1_ATEST_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_RF_CNTL1_ATEST_EN_SHIFT)) & SOCCIU_PAD_CONFIG0_RF_CNTL1_ATEST_EN_MASK)

#define SOCCIU_PAD_CONFIG0_RF_CNTL2_ATEST_EN_MASK (0x20U)
#define SOCCIU_PAD_CONFIG0_RF_CNTL2_ATEST_EN_SHIFT (5U)
/*! RF_CNTL2_ATEST_EN - RF_CNTL2 pad ATEST mode Enable */
#define SOCCIU_PAD_CONFIG0_RF_CNTL2_ATEST_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_RF_CNTL2_ATEST_EN_SHIFT)) & SOCCIU_PAD_CONFIG0_RF_CNTL2_ATEST_EN_MASK)

#define SOCCIU_PAD_CONFIG0_RF_CNTL3_ATEST_EN_MASK (0x40U)
#define SOCCIU_PAD_CONFIG0_RF_CNTL3_ATEST_EN_SHIFT (6U)
/*! RF_CNTL3_ATEST_EN - RF_CNTL3 pad ATEST mode Enable */
#define SOCCIU_PAD_CONFIG0_RF_CNTL3_ATEST_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_RF_CNTL3_ATEST_EN_SHIFT)) & SOCCIU_PAD_CONFIG0_RF_CNTL3_ATEST_EN_MASK)

#define SOCCIU_PAD_CONFIG0_ATEST_EN_3_0_MASK     (0x780U)
#define SOCCIU_PAD_CONFIG0_ATEST_EN_3_0_SHIFT    (7U)
/*! ATEST_EN_3_0 - ATEST Pin Force Bits */
#define SOCCIU_PAD_CONFIG0_ATEST_EN_3_0(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_ATEST_EN_3_0_SHIFT)) & SOCCIU_PAD_CONFIG0_ATEST_EN_3_0_MASK)

#define SOCCIU_PAD_CONFIG0_PAD_XOSC_EN_SEL_MASK  (0x800U)
#define SOCCIU_PAD_CONFIG0_PAD_XOSC_EN_SEL_SHIFT (11U)
/*! PAD_XOSC_EN_SEL - PAD XOSC Enable Control */
#define SOCCIU_PAD_CONFIG0_PAD_XOSC_EN_SEL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_PAD_XOSC_EN_SEL_SHIFT)) & SOCCIU_PAD_CONFIG0_PAD_XOSC_EN_SEL_MASK)

#define SOCCIU_PAD_CONFIG0_ATEST_DIS_STDALONE_MODE_3_0_MASK (0xF000U)
#define SOCCIU_PAD_CONFIG0_ATEST_DIS_STDALONE_MODE_3_0_SHIFT (12U)
/*! ATEST_DIS_STDALONE_MODE_3_0 - ATEST Pin Force disable Bits in standalone mode */
#define SOCCIU_PAD_CONFIG0_ATEST_DIS_STDALONE_MODE_3_0(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_ATEST_DIS_STDALONE_MODE_3_0_SHIFT)) & SOCCIU_PAD_CONFIG0_ATEST_DIS_STDALONE_MODE_3_0_MASK)

#define SOCCIU_PAD_CONFIG0_RFU_STDALONE_MASK     (0x40000U)
#define SOCCIU_PAD_CONFIG0_RFU_STDALONE_SHIFT    (18U)
/*! RFU_STDALONE - RFU Standalone */
#define SOCCIU_PAD_CONFIG0_RFU_STDALONE(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_RFU_STDALONE_SHIFT)) & SOCCIU_PAD_CONFIG0_RFU_STDALONE_MASK)

#define SOCCIU_PAD_CONFIG0_CAU_STDALONE_MASK     (0x80000U)
#define SOCCIU_PAD_CONFIG0_CAU_STDALONE_SHIFT    (19U)
/*! CAU_STDALONE - CAU Standalone */
#define SOCCIU_PAD_CONFIG0_CAU_STDALONE(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_CAU_STDALONE_SHIFT)) & SOCCIU_PAD_CONFIG0_CAU_STDALONE_MASK)

#define SOCCIU_PAD_CONFIG0_BRF_STDALONE_MASK     (0x100000U)
#define SOCCIU_PAD_CONFIG0_BRF_STDALONE_SHIFT    (20U)
/*! BRF_STDALONE - BRF Standalone */
#define SOCCIU_PAD_CONFIG0_BRF_STDALONE(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_BRF_STDALONE_SHIFT)) & SOCCIU_PAD_CONFIG0_BRF_STDALONE_MASK)

#define SOCCIU_PAD_CONFIG0_BRF_W3_SOC_MASTER_SEL_MASK (0x400000U)
#define SOCCIU_PAD_CONFIG0_BRF_W3_SOC_MASTER_SEL_SHIFT (22U)
/*! BRF_W3_SOC_MASTER_SEL - BRF 3-Wire SoC Master Select */
#define SOCCIU_PAD_CONFIG0_BRF_W3_SOC_MASTER_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_BRF_W3_SOC_MASTER_SEL_SHIFT)) & SOCCIU_PAD_CONFIG0_BRF_W3_SOC_MASTER_SEL_MASK)

#define SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_MASK (0x1000000U)
#define SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_SHIFT (24U)
/*! BT_DBG_UART_MODE - BT Debug Uart Mode */
#define SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_SHIFT)) & SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_MASK)

#define SOCCIU_PAD_CONFIG0_WLAN_DBG_UART_MODE_4PIN_MASK (0x2000000U)
#define SOCCIU_PAD_CONFIG0_WLAN_DBG_UART_MODE_4PIN_SHIFT (25U)
/*! WLAN_DBG_UART_MODE_4PIN - WLAN 4 Pin Debug Uart Mode */
#define SOCCIU_PAD_CONFIG0_WLAN_DBG_UART_MODE_4PIN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_WLAN_DBG_UART_MODE_4PIN_SHIFT)) & SOCCIU_PAD_CONFIG0_WLAN_DBG_UART_MODE_4PIN_MASK)

#define SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_4PIN_MASK (0x4000000U)
#define SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_4PIN_SHIFT (26U)
/*! BT_DBG_UART_MODE_4PIN - BT 4 pin Debug Uart Mode */
#define SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_4PIN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_4PIN_SHIFT)) & SOCCIU_PAD_CONFIG0_BT_DBG_UART_MODE_4PIN_MASK)

#define SOCCIU_PAD_CONFIG0_TRACE_PORT_MODE_CPU_MASK (0x10000000U)
#define SOCCIU_PAD_CONFIG0_TRACE_PORT_MODE_CPU_SHIFT (28U)
/*! TRACE_PORT_MODE_CPU - Trace port mode enable */
#define SOCCIU_PAD_CONFIG0_TRACE_PORT_MODE_CPU(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_TRACE_PORT_MODE_CPU_SHIFT)) & SOCCIU_PAD_CONFIG0_TRACE_PORT_MODE_CPU_MASK)

#define SOCCIU_PAD_CONFIG0_LED_MODE_MASK         (0x20000000U)
#define SOCCIU_PAD_CONFIG0_LED_MODE_SHIFT        (29U)
/*! LED_MODE - to enable disable led mode */
#define SOCCIU_PAD_CONFIG0_LED_MODE(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_LED_MODE_SHIFT)) & SOCCIU_PAD_CONFIG0_LED_MODE_MASK)

#define SOCCIU_PAD_CONFIG0_SD_D3_PU_CTRL_MASK    (0x40000000U)
#define SOCCIU_PAD_CONFIG0_SD_D3_PU_CTRL_SHIFT   (30U)
/*! SD_D3_PU_CTRL - SD D3 PU Control function */
#define SOCCIU_PAD_CONFIG0_SD_D3_PU_CTRL(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_CONFIG0_SD_D3_PU_CTRL_SHIFT)) & SOCCIU_PAD_CONFIG0_SD_D3_PU_CTRL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL0 - Pad Power-down Control 0 */
/*! @{ */

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO0_PD_SEL_MASK (0x7U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO0_PD_SEL_SHIFT (0U)
/*! GPIO0_PD_SEL - Power down output value for GPIO[0] pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO0_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO0_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO0_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO1_PD_SEL_MASK (0x70U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO1_PD_SEL_SHIFT (4U)
/*! GPIO1_PD_SEL - Power Down Output Value for GPIO[1] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO1_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO1_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO1_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO2_PD_SEL_MASK (0x700U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO2_PD_SEL_SHIFT (8U)
/*! GPIO2_PD_SEL - Power Down Output Value for GPIO[2] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO2_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO2_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO2_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO3_PD_SEL_MASK (0x7000U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO3_PD_SEL_SHIFT (12U)
/*! GPIO3_PD_SEL - Power Down Output Value for GPIO[3] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO3_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO3_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO3_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO4_PD_SEL_MASK (0x70000U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO4_PD_SEL_SHIFT (16U)
/*! GPIO4_PD_SEL - Power Down Output Value for GPIO[4] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO4_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO4_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO4_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO5_PD_SEL_MASK (0x700000U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO5_PD_SEL_SHIFT (20U)
/*! GPIO5_PD_SEL - Power Down Output Value for GPIO[5] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO5_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO5_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO5_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO6_PD_SEL_MASK (0x7000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO6_PD_SEL_SHIFT (24U)
/*! GPIO6_PD_SEL - Power Down Output Value for GPIO[6] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO6_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO6_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO6_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO7_PD_SEL_MASK (0x70000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO7_PD_SEL_SHIFT (28U)
/*! GPIO7_PD_SEL - Power Down Output Value for GPIO[7] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL0_GPIO7_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL0_GPIO7_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL0_GPIO7_PD_SEL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL1 - Pad Power-down Control 1 */
/*! @{ */

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO8_PD_SEL_MASK (0x7U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO8_PD_SEL_SHIFT (0U)
/*! GPIO8_PD_SEL - Power Down Output Value for GPIO[8] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO8_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO8_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO8_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO9_PD_SEL_MASK (0x70U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO9_PD_SEL_SHIFT (4U)
/*! GPIO9_PD_SEL - Power Down Output Value for GPIO[9] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO9_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO9_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO9_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO10_PD_SEL_MASK (0x700U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO10_PD_SEL_SHIFT (8U)
/*! GPIO10_PD_SEL - Power Down Output Value for GPIO[10] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO10_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO10_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO10_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO11_PD_SEL_MASK (0x7000U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO11_PD_SEL_SHIFT (12U)
/*! GPIO11_PD_SEL - Power Down Output Value for GPIO[11] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO11_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO11_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO11_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO12_PD_SEL_MASK (0x70000U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO12_PD_SEL_SHIFT (16U)
/*! GPIO12_PD_SEL - Power Down Output Value for GPIO[12] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO12_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO12_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO12_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO13_PD_SEL_MASK (0x700000U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO13_PD_SEL_SHIFT (20U)
/*! GPIO13_PD_SEL - Power Down Output Value for GPIO[13] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO13_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO13_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO13_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO14_PD_SEL_MASK (0x7000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO14_PD_SEL_SHIFT (24U)
/*! GPIO14_PD_SEL - Power Down Output Value for GPIO[14] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO14_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO14_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO14_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO21_PD_SEL_MASK (0x70000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO21_PD_SEL_SHIFT (28U)
/*! GPIO21_PD_SEL - Power Down Output Value for GPIO[21] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL1_GPIO21_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL1_GPIO21_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL1_GPIO21_PD_SEL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL2 - Pad Power-down Control 2 */
/*! @{ */

#define SOCCIU_PAD_PWRDOWN_CTRL2_GPIO42_PD_SEL_MASK (0x7000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL2_GPIO42_PD_SEL_SHIFT (24U)
/*! GPIO42_PD_SEL - Power Down Output Value for GPIO[42] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL2_GPIO42_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL2_GPIO42_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL2_GPIO42_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL2_GPIO43_PD_SEL_MASK (0x70000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL2_GPIO43_PD_SEL_SHIFT (28U)
/*! GPIO43_PD_SEL - Power Down Output Value for GPIO[43] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL2_GPIO43_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL2_GPIO43_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL2_GPIO43_PD_SEL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL3 - Pad Power-down Control 3 */
/*! @{ */

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO44_PD_SEL_MASK (0x7U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO44_PD_SEL_SHIFT (0U)
/*! GPIO44_PD_SEL - Power Down Output Value for GPIO[44] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO44_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO44_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO44_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO45_PD_SEL_MASK (0x70U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO45_PD_SEL_SHIFT (4U)
/*! GPIO45_PD_SEL - Power Down Output Value for GPIO[45] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO45_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO45_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO45_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO46_PD_SEL_MASK (0x700U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO46_PD_SEL_SHIFT (8U)
/*! GPIO46_PD_SEL - Power Down Output Value for GPIO[46] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO46_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO46_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO46_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO47_PD_SEL_MASK (0x7000U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO47_PD_SEL_SHIFT (12U)
/*! GPIO47_PD_SEL - Power Down Output Value for GPIO[47] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO47_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO47_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO47_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO48_PD_SEL_MASK (0x70000U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO48_PD_SEL_SHIFT (16U)
/*! GPIO48_PD_SEL - Power Down Output Value for GPIO[48] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO48_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO48_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO48_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO49_PD_SEL_MASK (0x700000U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO49_PD_SEL_SHIFT (20U)
/*! GPIO49_PD_SEL - Power Down Output Value for GPIO[49] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO49_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO49_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO49_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO50_PD_SEL_MASK (0x7000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO50_PD_SEL_SHIFT (24U)
/*! GPIO50_PD_SEL - Power Down Output Value for GPIO[50] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO50_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO50_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO50_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO51_PD_SEL_MASK (0x70000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO51_PD_SEL_SHIFT (28U)
/*! GPIO51_PD_SEL - Power Down Output Value for GPIO[51] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL3_GPIO51_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL3_GPIO51_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL3_GPIO51_PD_SEL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL4 - Pad Power-down Control 4 */
/*! @{ */

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO52_PD_SEL_MASK (0x7U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO52_PD_SEL_SHIFT (0U)
/*! GPIO52_PD_SEL - Power Down Output Value for GPIO[52] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO52_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO52_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO52_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO53_PD_SEL_MASK (0x70U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO53_PD_SEL_SHIFT (4U)
/*! GPIO53_PD_SEL - Power Down Output Value for GPIO[53] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO53_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO53_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO53_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO54_PD_SEL_MASK (0x700U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO54_PD_SEL_SHIFT (8U)
/*! GPIO54_PD_SEL - Power Down Output Value for GPIO[54] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO54_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO54_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO54_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO55_PD_SEL_MASK (0x7000U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO55_PD_SEL_SHIFT (12U)
/*! GPIO55_PD_SEL - Power Down Output Value for GPIO[55] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO55_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO55_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO55_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO56_PD_SEL_MASK (0x70000U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO56_PD_SEL_SHIFT (16U)
/*! GPIO56_PD_SEL - Power Down Output Value for GPIO[56] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO56_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO56_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO56_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO57_PD_SEL_MASK (0x700000U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO57_PD_SEL_SHIFT (20U)
/*! GPIO57_PD_SEL - Power Down Output Value for GPIO[57] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO57_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO57_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO57_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO58_PD_SEL_MASK (0x7000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO58_PD_SEL_SHIFT (24U)
/*! GPIO58_PD_SEL - Power Down Output Value for GPIO[58] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO58_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO58_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO58_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO59_PD_SEL_MASK (0x70000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO59_PD_SEL_SHIFT (28U)
/*! GPIO59_PD_SEL - Power Down Output Value for GPIO[59] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL4_GPIO59_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL4_GPIO59_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL4_GPIO59_PD_SEL_MASK)
/*! @} */

/*! @name PAD_PWRDOWN_CTRL5 - Pad Power-down Control 5 */
/*! @{ */

#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO60_PD_SEL_MASK (0x7U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO60_PD_SEL_SHIFT (0U)
/*! GPIO60_PD_SEL - Power Down Output Value for GPIO[60] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO60_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_GPIO60_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_GPIO60_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO61_PD_SEL_MASK (0x70U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO61_PD_SEL_SHIFT (4U)
/*! GPIO61_PD_SEL - Power Down Output Value for GPIO[61] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO61_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_GPIO61_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_GPIO61_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO62_PD_SEL_MASK (0x700U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO62_PD_SEL_SHIFT (8U)
/*! GPIO62_PD_SEL - Power Down Output Value for GPIO[62] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO62_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_GPIO62_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_GPIO62_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO63_PD_SEL_MASK (0x7000U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO63_PD_SEL_SHIFT (12U)
/*! GPIO63_PD_SEL - Power Down Output Value for GPIO[63] Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_GPIO63_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_GPIO63_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_GPIO63_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL0_PD_SEL_MASK (0x70000U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL0_PD_SEL_SHIFT (16U)
/*! RF_CNTL0_PD_SEL - Power Down Output Value for rf_cntl0 Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL0_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL0_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL0_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL1_PD_SEL_MASK (0x700000U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL1_PD_SEL_SHIFT (20U)
/*! RF_CNTL1_PD_SEL - Power Down Output Value for rf_cntl1 Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL1_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL1_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL1_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL2_PD_SEL_MASK (0x7000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL2_PD_SEL_SHIFT (24U)
/*! RF_CNTL2_PD_SEL - Power Down Output Value for rf_cntl2 Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL2_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL2_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL2_PD_SEL_MASK)

#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL3_PD_SEL_MASK (0x70000000U)
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL3_PD_SEL_SHIFT (28U)
/*! RF_CNTL3_PD_SEL - Power Down Output Value for rf_cntl3 Pad */
#define SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL3_PD_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL3_PD_SEL_SHIFT)) & SOCCIU_PAD_PWRDOWN_CTRL5_RF_CNTL3_PD_SEL_MASK)
/*! @} */

/*! @name PAD_RF_SW_SLP_CONFIG - RF Switch Pad Sleep Mode Configuration */
/*! @{ */

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_EN_MASK (0x1U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_EN_SHIFT (0U)
/*! RF_CNTL0_SLP_EN - RF_CNTL0 Sleep Force Enable */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_EN_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_EN_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_EN_MASK (0x2U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_EN_SHIFT (1U)
/*! RF_CNTL1_SLP_EN - RF_CNTL1 Sleep Force Enable */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_EN_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_EN_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_EN_MASK (0x4U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_EN_SHIFT (2U)
/*! RF_CNTL2_SLP_EN - RF_CNTL2 Sleep Force Enable */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_EN_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_EN_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_EN_MASK (0x8U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_EN_SHIFT (3U)
/*! RF_CNTL3_SLP_EN - RF_CNTL3 Sleep Force Enable */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_EN_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_EN_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_VAL_MASK (0x10000U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_VAL_SHIFT (16U)
/*! RF_CNTL0_SLP_VAL - RF_CTRL0 Sleep Value */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_VAL_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL0_SLP_VAL_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_VAL_MASK (0x20000U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_VAL_SHIFT (17U)
/*! RF_CNTL1_SLP_VAL - RF_CTRL1 Sleep Value */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_VAL_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL1_SLP_VAL_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_VAL_MASK (0x40000U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_VAL_SHIFT (18U)
/*! RF_CNTL2_SLP_VAL - RF_CTRL2 Sleep Value */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_VAL_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL2_SLP_VAL_MASK)

#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_VAL_MASK (0x80000U)
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_VAL_SHIFT (19U)
/*! RF_CNTL3_SLP_VAL - RF_CTRL3 Sleep Value */
#define SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_VAL_SHIFT)) & SOCCIU_PAD_RF_SW_SLP_CONFIG_RF_CNTL3_SLP_VAL_MASK)
/*! @} */

/*! @name PAD_ATEST_SW_SLP_CONFIG - ATEST Pad Sleep Mode Configuration */
/*! @{ */

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_EN_MASK (0x1U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_EN_SHIFT (0U)
/*! ATEST0_SLP_EN - ATEST0 Sleep Force Enable */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_EN_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_EN_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_EN_MASK (0x2U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_EN_SHIFT (1U)
/*! ATEST1_SLP_EN - ATEST1 Sleep Force Enable */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_EN_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_EN_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_EN_MASK (0x4U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_EN_SHIFT (2U)
/*! ATEST2_SLP_EN - ATEST2 Sleep Force Enable */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_EN_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_EN_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_EN_MASK (0x8U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_EN_SHIFT (3U)
/*! ATEST3_SLP_EN - ATEST3 Sleep Force Enable */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_EN_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_EN_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_VAL_MASK (0x10000U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_VAL_SHIFT (16U)
/*! ATEST0_SLP_VAL - ATEST0 Sleep Value */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_VAL_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST0_SLP_VAL_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_VAL_MASK (0x20000U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_VAL_SHIFT (17U)
/*! ATEST1_SLP_VAL - ATEST1 Sleep Value */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_VAL_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST1_SLP_VAL_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_VAL_MASK (0x40000U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_VAL_SHIFT (18U)
/*! ATEST2_SLP_VAL - ATEST2 Sleep Value */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_VAL_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST2_SLP_VAL_MASK)

#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_VAL_MASK (0x80000U)
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_VAL_SHIFT (19U)
/*! ATEST3_SLP_VAL - ATEST3 Sleep Value */
#define SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_VAL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_VAL_SHIFT)) & SOCCIU_PAD_ATEST_SW_SLP_CONFIG_ATEST3_SLP_VAL_MASK)
/*! @} */

/*! @name SR_CONFIG0 - GPIO Slew Rate control */
/*! @{ */

#define SOCCIU_SR_CONFIG0_GPIO0_SR_MASK          (0x3U)
#define SOCCIU_SR_CONFIG0_GPIO0_SR_SHIFT         (0U)
/*! GPIO0_SR - Slew Rate Control for GPIO[0] */
#define SOCCIU_SR_CONFIG0_GPIO0_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO0_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO0_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO1_SR_MASK          (0xCU)
#define SOCCIU_SR_CONFIG0_GPIO1_SR_SHIFT         (2U)
/*! GPIO1_SR - Slew Rate Control for GPIO[1] */
#define SOCCIU_SR_CONFIG0_GPIO1_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO1_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO1_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO2_SR_MASK          (0x30U)
#define SOCCIU_SR_CONFIG0_GPIO2_SR_SHIFT         (4U)
/*! GPIO2_SR - Slew Rate Control for GPIO[2] */
#define SOCCIU_SR_CONFIG0_GPIO2_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO2_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO2_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO3_SR_MASK          (0xC0U)
#define SOCCIU_SR_CONFIG0_GPIO3_SR_SHIFT         (6U)
/*! GPIO3_SR - Slew Rate Control for GPIO[3] */
#define SOCCIU_SR_CONFIG0_GPIO3_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO3_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO3_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO4_SR_MASK          (0x300U)
#define SOCCIU_SR_CONFIG0_GPIO4_SR_SHIFT         (8U)
/*! GPIO4_SR - Slew Rate Control for GPIO[4] */
#define SOCCIU_SR_CONFIG0_GPIO4_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO4_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO4_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO5_SR_MASK          (0xC00U)
#define SOCCIU_SR_CONFIG0_GPIO5_SR_SHIFT         (10U)
/*! GPIO5_SR - Slew Rate Control for GPIO[5] */
#define SOCCIU_SR_CONFIG0_GPIO5_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO5_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO5_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO6_SR_MASK          (0x3000U)
#define SOCCIU_SR_CONFIG0_GPIO6_SR_SHIFT         (12U)
/*! GPIO6_SR - Slew Rate Control for GPIO[6] */
#define SOCCIU_SR_CONFIG0_GPIO6_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO6_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO6_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO7_SR_MASK          (0xC000U)
#define SOCCIU_SR_CONFIG0_GPIO7_SR_SHIFT         (14U)
/*! GPIO7_SR - Slew Rate Control for GPIO[7] */
#define SOCCIU_SR_CONFIG0_GPIO7_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO7_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO7_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO8_SR_MASK          (0x30000U)
#define SOCCIU_SR_CONFIG0_GPIO8_SR_SHIFT         (16U)
/*! GPIO8_SR - Slew Rate Control for GPIO[8] */
#define SOCCIU_SR_CONFIG0_GPIO8_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO8_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO8_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO9_SR_MASK          (0xC0000U)
#define SOCCIU_SR_CONFIG0_GPIO9_SR_SHIFT         (18U)
/*! GPIO9_SR - Slew Rate Control for GPIO[9] */
#define SOCCIU_SR_CONFIG0_GPIO9_SR(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO9_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO9_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO10_SR_MASK         (0x300000U)
#define SOCCIU_SR_CONFIG0_GPIO10_SR_SHIFT        (20U)
/*! GPIO10_SR - Slew Rate Control for GPIO[10] */
#define SOCCIU_SR_CONFIG0_GPIO10_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO10_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO10_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO11_SR_MASK         (0xC00000U)
#define SOCCIU_SR_CONFIG0_GPIO11_SR_SHIFT        (22U)
/*! GPIO11_SR - Slew Rate Control for GPIO[11] */
#define SOCCIU_SR_CONFIG0_GPIO11_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO11_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO11_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO12_SR_MASK         (0x3000000U)
#define SOCCIU_SR_CONFIG0_GPIO12_SR_SHIFT        (24U)
/*! GPIO12_SR - Slew Rate Control for GPIO[12] */
#define SOCCIU_SR_CONFIG0_GPIO12_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO12_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO12_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO13_SR_MASK         (0xC000000U)
#define SOCCIU_SR_CONFIG0_GPIO13_SR_SHIFT        (26U)
/*! GPIO13_SR - Slew Rate Control for GPIO[13] */
#define SOCCIU_SR_CONFIG0_GPIO13_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO13_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO13_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO14_SR_MASK         (0x30000000U)
#define SOCCIU_SR_CONFIG0_GPIO14_SR_SHIFT        (28U)
/*! GPIO14_SR - Slew Rate Control for GPIO[14] */
#define SOCCIU_SR_CONFIG0_GPIO14_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO14_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO14_SR_MASK)

#define SOCCIU_SR_CONFIG0_GPIO15_SR_MASK         (0xC0000000U)
#define SOCCIU_SR_CONFIG0_GPIO15_SR_SHIFT        (30U)
/*! GPIO15_SR - Slew Rate Control for GPIO[15] */
#define SOCCIU_SR_CONFIG0_GPIO15_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG0_GPIO15_SR_SHIFT)) & SOCCIU_SR_CONFIG0_GPIO15_SR_MASK)
/*! @} */

/*! @name SR_CONFIG1 - GPIO Slew Rate control */
/*! @{ */

#define SOCCIU_SR_CONFIG1_GPIO16_SR_MASK         (0x3U)
#define SOCCIU_SR_CONFIG1_GPIO16_SR_SHIFT        (0U)
/*! GPIO16_SR - Slew Rate Control for GPIO[16] */
#define SOCCIU_SR_CONFIG1_GPIO16_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO16_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO16_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO17_SR_MASK         (0xCU)
#define SOCCIU_SR_CONFIG1_GPIO17_SR_SHIFT        (2U)
/*! GPIO17_SR - Slew Rate Control for GPIO[17] */
#define SOCCIU_SR_CONFIG1_GPIO17_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO17_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO17_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO18_SR_MASK         (0x30U)
#define SOCCIU_SR_CONFIG1_GPIO18_SR_SHIFT        (4U)
/*! GPIO18_SR - Slew Rate Control for GPIO[18] */
#define SOCCIU_SR_CONFIG1_GPIO18_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO18_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO18_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO19_SR_MASK         (0xC0U)
#define SOCCIU_SR_CONFIG1_GPIO19_SR_SHIFT        (6U)
/*! GPIO19_SR - Slew Rate Control for GPIO[19] */
#define SOCCIU_SR_CONFIG1_GPIO19_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO19_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO19_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO20_SR_MASK         (0x300U)
#define SOCCIU_SR_CONFIG1_GPIO20_SR_SHIFT        (8U)
/*! GPIO20_SR - Slew Rate Control for GPIO[20] */
#define SOCCIU_SR_CONFIG1_GPIO20_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO20_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO20_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO21_SR_MASK         (0xC00U)
#define SOCCIU_SR_CONFIG1_GPIO21_SR_SHIFT        (10U)
/*! GPIO21_SR - Slew Rate Control for GPIO[21] */
#define SOCCIU_SR_CONFIG1_GPIO21_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO21_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO21_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO28_SR_MASK         (0x3000000U)
#define SOCCIU_SR_CONFIG1_GPIO28_SR_SHIFT        (24U)
/*! GPIO28_SR - Slew Rate Control for GPIO[28] */
#define SOCCIU_SR_CONFIG1_GPIO28_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO28_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO28_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO29_SR_MASK         (0xC000000U)
#define SOCCIU_SR_CONFIG1_GPIO29_SR_SHIFT        (26U)
/*! GPIO29_SR - Slew Rate Control for GPIO[29] */
#define SOCCIU_SR_CONFIG1_GPIO29_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO29_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO29_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO30_SR_MASK         (0x30000000U)
#define SOCCIU_SR_CONFIG1_GPIO30_SR_SHIFT        (28U)
/*! GPIO30_SR - Slew Rate Control for GPIO[30] */
#define SOCCIU_SR_CONFIG1_GPIO30_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO30_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO30_SR_MASK)

#define SOCCIU_SR_CONFIG1_GPIO31_SR_MASK         (0xC0000000U)
#define SOCCIU_SR_CONFIG1_GPIO31_SR_SHIFT        (30U)
/*! GPIO31_SR - Slew Rate Control for GPIO[31] */
#define SOCCIU_SR_CONFIG1_GPIO31_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG1_GPIO31_SR_SHIFT)) & SOCCIU_SR_CONFIG1_GPIO31_SR_MASK)
/*! @} */

/*! @name SR_CONFIG2 - GPIO Slew Rate control */
/*! @{ */

#define SOCCIU_SR_CONFIG2_GPIO32_SR_MASK         (0x3U)
#define SOCCIU_SR_CONFIG2_GPIO32_SR_SHIFT        (0U)
/*! GPIO32_SR - Slew Rate Control for GPIO[32] */
#define SOCCIU_SR_CONFIG2_GPIO32_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO32_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO32_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO33_SR_MASK         (0xCU)
#define SOCCIU_SR_CONFIG2_GPIO33_SR_SHIFT        (2U)
/*! GPIO33_SR - Slew Rate Control for GPIO[33] */
#define SOCCIU_SR_CONFIG2_GPIO33_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO33_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO33_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO34_SR_MASK         (0x30U)
#define SOCCIU_SR_CONFIG2_GPIO34_SR_SHIFT        (4U)
/*! GPIO34_SR - Slew Rate Control for GPIO[34] */
#define SOCCIU_SR_CONFIG2_GPIO34_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO34_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO34_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO35_SR_MASK         (0xC0U)
#define SOCCIU_SR_CONFIG2_GPIO35_SR_SHIFT        (6U)
/*! GPIO35_SR - Slew Rate Control for GPIO[35] */
#define SOCCIU_SR_CONFIG2_GPIO35_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO35_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO35_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO36_SR_MASK         (0x300U)
#define SOCCIU_SR_CONFIG2_GPIO36_SR_SHIFT        (8U)
/*! GPIO36_SR - Slew Rate Control for GPIO[36] */
#define SOCCIU_SR_CONFIG2_GPIO36_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO36_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO36_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO37_SR_MASK         (0xC00U)
#define SOCCIU_SR_CONFIG2_GPIO37_SR_SHIFT        (10U)
/*! GPIO37_SR - Slew Rate Control for GPIO[37] */
#define SOCCIU_SR_CONFIG2_GPIO37_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO37_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO37_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO38_SR_MASK         (0x3000U)
#define SOCCIU_SR_CONFIG2_GPIO38_SR_SHIFT        (12U)
/*! GPIO38_SR - Slew Rate Control for GPIO[38] */
#define SOCCIU_SR_CONFIG2_GPIO38_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO38_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO38_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO39_SR_MASK         (0xC000U)
#define SOCCIU_SR_CONFIG2_GPIO39_SR_SHIFT        (14U)
/*! GPIO39_SR - Slew Rate Control for GPIO[39] */
#define SOCCIU_SR_CONFIG2_GPIO39_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO39_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO39_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO40_SR_MASK         (0x30000U)
#define SOCCIU_SR_CONFIG2_GPIO40_SR_SHIFT        (16U)
/*! GPIO40_SR - Slew Rate Control for GPIO[40] */
#define SOCCIU_SR_CONFIG2_GPIO40_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO40_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO40_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO41_SR_MASK         (0xC0000U)
#define SOCCIU_SR_CONFIG2_GPIO41_SR_SHIFT        (18U)
/*! GPIO41_SR - Slew Rate Control for GPIO[41] */
#define SOCCIU_SR_CONFIG2_GPIO41_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO41_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO41_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO42_SR_MASK         (0x300000U)
#define SOCCIU_SR_CONFIG2_GPIO42_SR_SHIFT        (20U)
/*! GPIO42_SR - Slew Rate Control for GPIO[42] */
#define SOCCIU_SR_CONFIG2_GPIO42_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO42_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO42_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO43_SR_MASK         (0xC00000U)
#define SOCCIU_SR_CONFIG2_GPIO43_SR_SHIFT        (22U)
/*! GPIO43_SR - Slew Rate Control for GPIO[43] */
#define SOCCIU_SR_CONFIG2_GPIO43_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO43_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO43_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO44_SR_MASK         (0x3000000U)
#define SOCCIU_SR_CONFIG2_GPIO44_SR_SHIFT        (24U)
/*! GPIO44_SR - Slew Rate Control for GPIO[44] */
#define SOCCIU_SR_CONFIG2_GPIO44_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO44_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO44_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO45_SR_MASK         (0xC000000U)
#define SOCCIU_SR_CONFIG2_GPIO45_SR_SHIFT        (26U)
/*! GPIO45_SR - Slew Rate Control for GPIO[45] */
#define SOCCIU_SR_CONFIG2_GPIO45_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO45_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO45_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO46_SR_MASK         (0x30000000U)
#define SOCCIU_SR_CONFIG2_GPIO46_SR_SHIFT        (28U)
/*! GPIO46_SR - Slew Rate Control for GPIO[46] */
#define SOCCIU_SR_CONFIG2_GPIO46_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO46_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO46_SR_MASK)

#define SOCCIU_SR_CONFIG2_GPIO47_SR_MASK         (0xC0000000U)
#define SOCCIU_SR_CONFIG2_GPIO47_SR_SHIFT        (30U)
/*! GPIO47_SR - Slew Rate Control for GPIO[47] */
#define SOCCIU_SR_CONFIG2_GPIO47_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG2_GPIO47_SR_SHIFT)) & SOCCIU_SR_CONFIG2_GPIO47_SR_MASK)
/*! @} */

/*! @name SR_CONFIG3 - GPIO Slew Rate control */
/*! @{ */

#define SOCCIU_SR_CONFIG3_GPIO48_SR_MASK         (0x3U)
#define SOCCIU_SR_CONFIG3_GPIO48_SR_SHIFT        (0U)
/*! GPIO48_SR - Slew Rate Control for GPIO[48] */
#define SOCCIU_SR_CONFIG3_GPIO48_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO48_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO48_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO49_SR_MASK         (0xCU)
#define SOCCIU_SR_CONFIG3_GPIO49_SR_SHIFT        (2U)
/*! GPIO49_SR - Slew Rate Control for GPIO[49] */
#define SOCCIU_SR_CONFIG3_GPIO49_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO49_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO49_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO50_SR_MASK         (0x30U)
#define SOCCIU_SR_CONFIG3_GPIO50_SR_SHIFT        (4U)
/*! GPIO50_SR - Slew Rate Control for GPIO[50] */
#define SOCCIU_SR_CONFIG3_GPIO50_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO50_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO50_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO51_SR_MASK         (0xC0U)
#define SOCCIU_SR_CONFIG3_GPIO51_SR_SHIFT        (6U)
/*! GPIO51_SR - Slew Rate Control for GPIO[51] */
#define SOCCIU_SR_CONFIG3_GPIO51_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO51_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO51_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO52_SR_MASK         (0x300U)
#define SOCCIU_SR_CONFIG3_GPIO52_SR_SHIFT        (8U)
/*! GPIO52_SR - Slew Rate Control for GPIO[52] */
#define SOCCIU_SR_CONFIG3_GPIO52_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO52_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO52_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO53_SR_MASK         (0xC00U)
#define SOCCIU_SR_CONFIG3_GPIO53_SR_SHIFT        (10U)
/*! GPIO53_SR - Slew Rate Control for GPIO[53] */
#define SOCCIU_SR_CONFIG3_GPIO53_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO53_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO53_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO54_SR_MASK         (0x3000U)
#define SOCCIU_SR_CONFIG3_GPIO54_SR_SHIFT        (12U)
/*! GPIO54_SR - Slew Rate Control for GPIO[54] */
#define SOCCIU_SR_CONFIG3_GPIO54_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO54_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO54_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO55_SR_MASK         (0xC000U)
#define SOCCIU_SR_CONFIG3_GPIO55_SR_SHIFT        (14U)
/*! GPIO55_SR - Slew Rate Control for GPIO[55] */
#define SOCCIU_SR_CONFIG3_GPIO55_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO55_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO55_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO56_SR_MASK         (0x30000U)
#define SOCCIU_SR_CONFIG3_GPIO56_SR_SHIFT        (16U)
/*! GPIO56_SR - Slew Rate Control for GPIO[56] */
#define SOCCIU_SR_CONFIG3_GPIO56_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO56_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO56_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO57_SR_MASK         (0xC0000U)
#define SOCCIU_SR_CONFIG3_GPIO57_SR_SHIFT        (18U)
/*! GPIO57_SR - Slew Rate Control for GPIO[57] */
#define SOCCIU_SR_CONFIG3_GPIO57_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO57_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO57_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO58_SR_MASK         (0x300000U)
#define SOCCIU_SR_CONFIG3_GPIO58_SR_SHIFT        (20U)
/*! GPIO58_SR - Slew Rate Control for GPIO[58] */
#define SOCCIU_SR_CONFIG3_GPIO58_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO58_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO58_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO59_SR_MASK         (0xC00000U)
#define SOCCIU_SR_CONFIG3_GPIO59_SR_SHIFT        (22U)
/*! GPIO59_SR - Slew Rate Control for GPIO[59] */
#define SOCCIU_SR_CONFIG3_GPIO59_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO59_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO59_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO60_SR_MASK         (0x3000000U)
#define SOCCIU_SR_CONFIG3_GPIO60_SR_SHIFT        (24U)
/*! GPIO60_SR - Slew Rate Control for GPIO[60] */
#define SOCCIU_SR_CONFIG3_GPIO60_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO60_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO60_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO61_SR_MASK         (0xC000000U)
#define SOCCIU_SR_CONFIG3_GPIO61_SR_SHIFT        (26U)
/*! GPIO61_SR - Slew Rate Control for GPIO[61] */
#define SOCCIU_SR_CONFIG3_GPIO61_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO61_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO61_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO62_SR_MASK         (0x30000000U)
#define SOCCIU_SR_CONFIG3_GPIO62_SR_SHIFT        (28U)
/*! GPIO62_SR - Slew Rate Control for GPIO[62] */
#define SOCCIU_SR_CONFIG3_GPIO62_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO62_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO62_SR_MASK)

#define SOCCIU_SR_CONFIG3_GPIO63_SR_MASK         (0xC0000000U)
#define SOCCIU_SR_CONFIG3_GPIO63_SR_SHIFT        (30U)
/*! GPIO63_SR - Slew Rate Control for GPIO[63] */
#define SOCCIU_SR_CONFIG3_GPIO63_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG3_GPIO63_SR_SHIFT)) & SOCCIU_SR_CONFIG3_GPIO63_SR_MASK)
/*! @} */

/*! @name SR_CONFIG4 - GPIO Slew Rate control */
/*! @{ */

#define SOCCIU_SR_CONFIG4_ATEST0_SR_MASK         (0x3U)
#define SOCCIU_SR_CONFIG4_ATEST0_SR_SHIFT        (0U)
/*! ATEST0_SR - Slew Rate Control for atest0 */
#define SOCCIU_SR_CONFIG4_ATEST0_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_ATEST0_SR_SHIFT)) & SOCCIU_SR_CONFIG4_ATEST0_SR_MASK)

#define SOCCIU_SR_CONFIG4_ATEST1_SR_MASK         (0xCU)
#define SOCCIU_SR_CONFIG4_ATEST1_SR_SHIFT        (2U)
/*! ATEST1_SR - Slew Rate Control for atest1 */
#define SOCCIU_SR_CONFIG4_ATEST1_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_ATEST1_SR_SHIFT)) & SOCCIU_SR_CONFIG4_ATEST1_SR_MASK)

#define SOCCIU_SR_CONFIG4_ATEST2_SR_MASK         (0x30U)
#define SOCCIU_SR_CONFIG4_ATEST2_SR_SHIFT        (4U)
/*! ATEST2_SR - Slew Rate Control for atest2 */
#define SOCCIU_SR_CONFIG4_ATEST2_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_ATEST2_SR_SHIFT)) & SOCCIU_SR_CONFIG4_ATEST2_SR_MASK)

#define SOCCIU_SR_CONFIG4_ATEST3_SR_MASK         (0xC0U)
#define SOCCIU_SR_CONFIG4_ATEST3_SR_SHIFT        (6U)
/*! ATEST3_SR - Slew Rate Control for atest3 */
#define SOCCIU_SR_CONFIG4_ATEST3_SR(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_ATEST3_SR_SHIFT)) & SOCCIU_SR_CONFIG4_ATEST3_SR_MASK)

#define SOCCIU_SR_CONFIG4_RF_CNTL0_SR_MASK       (0x300U)
#define SOCCIU_SR_CONFIG4_RF_CNTL0_SR_SHIFT      (8U)
/*! RF_CNTL0_SR - Slew Rate Control for rf_cntl0 */
#define SOCCIU_SR_CONFIG4_RF_CNTL0_SR(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_RF_CNTL0_SR_SHIFT)) & SOCCIU_SR_CONFIG4_RF_CNTL0_SR_MASK)

#define SOCCIU_SR_CONFIG4_RF_CNTL1_SR_MASK       (0xC00U)
#define SOCCIU_SR_CONFIG4_RF_CNTL1_SR_SHIFT      (10U)
/*! RF_CNTL1_SR - Slew Rate Control for rf_cntl1 */
#define SOCCIU_SR_CONFIG4_RF_CNTL1_SR(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_RF_CNTL1_SR_SHIFT)) & SOCCIU_SR_CONFIG4_RF_CNTL1_SR_MASK)

#define SOCCIU_SR_CONFIG4_RF_CNTL2_SR_MASK       (0x3000U)
#define SOCCIU_SR_CONFIG4_RF_CNTL2_SR_SHIFT      (12U)
/*! RF_CNTL2_SR - Slew Rate Control for rf_cntl2 */
#define SOCCIU_SR_CONFIG4_RF_CNTL2_SR(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_RF_CNTL2_SR_SHIFT)) & SOCCIU_SR_CONFIG4_RF_CNTL2_SR_MASK)

#define SOCCIU_SR_CONFIG4_RF_CNTL3_SR_MASK       (0xC000U)
#define SOCCIU_SR_CONFIG4_RF_CNTL3_SR_SHIFT      (14U)
/*! RF_CNTL3_SR - Slew Rate Control for rf_cntl3 */
#define SOCCIU_SR_CONFIG4_RF_CNTL3_SR(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_SR_CONFIG4_RF_CNTL3_SR_SHIFT)) & SOCCIU_SR_CONFIG4_RF_CNTL3_SR_MASK)
/*! @} */

/*! @name PAD_WKUP0 - Pad Wakeup Mode Enable */
/*! @{ */

#define SOCCIU_PAD_WKUP0_ENABLE_MASK             (0x3U)
#define SOCCIU_PAD_WKUP0_ENABLE_SHIFT            (0U)
/*! ENABLE - Pad Wakeup Mode Enable [1:0] */
#define SOCCIU_PAD_WKUP0_ENABLE(x)               (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_WKUP0_ENABLE_SHIFT)) & SOCCIU_PAD_WKUP0_ENABLE_MASK)
/*! @} */

/*! @name PAD_PU_PD_EN0 - Pad Pull-up Pull-down Enable1 */
/*! @{ */

#define SOCCIU_PAD_PU_PD_EN0_GPIO0_PU_PD_EN_MASK (0x3U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO0_PU_PD_EN_SHIFT (0U)
/*! GPIO0_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[0] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO0_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO0_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO0_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO1_PU_PD_EN_MASK (0xCU)
#define SOCCIU_PAD_PU_PD_EN0_GPIO1_PU_PD_EN_SHIFT (2U)
/*! GPIO1_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[1] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO1_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO1_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO1_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO2_PU_PD_EN_MASK (0x30U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO2_PU_PD_EN_SHIFT (4U)
/*! GPIO2_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[2] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO2_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO2_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO2_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO3_PU_PD_EN_MASK (0xC0U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO3_PU_PD_EN_SHIFT (6U)
/*! GPIO3_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[3] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO3_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO3_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO3_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO4_PU_PD_EN_MASK (0x300U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO4_PU_PD_EN_SHIFT (8U)
/*! GPIO4_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[4] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO4_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO4_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO4_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO5_PU_PD_EN_MASK (0xC00U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO5_PU_PD_EN_SHIFT (10U)
/*! GPIO5_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[5] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO5_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO5_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO5_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO6_PU_PD_EN_MASK (0x3000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO6_PU_PD_EN_SHIFT (12U)
/*! GPIO6_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[6] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO6_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO6_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO6_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO7_PU_PD_EN_MASK (0xC000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO7_PU_PD_EN_SHIFT (14U)
/*! GPIO7_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[7] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO7_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO7_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO7_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO8_PU_PD_EN_MASK (0x30000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO8_PU_PD_EN_SHIFT (16U)
/*! GPIO8_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[8] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO8_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO8_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO8_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO9_PU_PD_EN_MASK (0xC0000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO9_PU_PD_EN_SHIFT (18U)
/*! GPIO9_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[9] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO9_PU_PD_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO9_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO9_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO10_PU_PD_EN_MASK (0x300000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO10_PU_PD_EN_SHIFT (20U)
/*! GPIO10_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[10] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO10_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO10_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO10_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO11_PU_PD_EN_MASK (0xC00000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO11_PU_PD_EN_SHIFT (22U)
/*! GPIO11_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[11] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO11_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO11_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO11_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO12_PU_PD_EN_MASK (0x3000000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO12_PU_PD_EN_SHIFT (24U)
/*! GPIO12_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[12] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO12_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO12_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO12_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO13_PU_PD_EN_MASK (0xC000000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO13_PU_PD_EN_SHIFT (26U)
/*! GPIO13_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[13] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO13_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO13_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO13_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO14_PU_PD_EN_MASK (0x30000000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO14_PU_PD_EN_SHIFT (28U)
/*! GPIO14_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[14] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO14_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO14_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO14_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN0_GPIO15_PU_PD_EN_MASK (0xC0000000U)
#define SOCCIU_PAD_PU_PD_EN0_GPIO15_PU_PD_EN_SHIFT (30U)
/*! GPIO15_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[15] */
#define SOCCIU_PAD_PU_PD_EN0_GPIO15_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN0_GPIO15_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN0_GPIO15_PU_PD_EN_MASK)
/*! @} */

/*! @name PAD_PU_PD_EN1 - Pad Pull-up Pull-down Enable2 */
/*! @{ */

#define SOCCIU_PAD_PU_PD_EN1_GPIO16_PU_PD_EN_MASK (0x3U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO16_PU_PD_EN_SHIFT (0U)
/*! GPIO16_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[16] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO16_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO16_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO16_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO17_PU_PD_EN_MASK (0xCU)
#define SOCCIU_PAD_PU_PD_EN1_GPIO17_PU_PD_EN_SHIFT (2U)
/*! GPIO17_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[17] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO17_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO17_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO17_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO18_PU_PD_EN_MASK (0x30U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO18_PU_PD_EN_SHIFT (4U)
/*! GPIO18_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[18] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO18_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO18_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO18_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO19_PU_PD_EN_MASK (0xC0U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO19_PU_PD_EN_SHIFT (6U)
/*! GPIO19_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[19] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO19_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO19_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO19_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO20_PU_PD_EN_MASK (0x300U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO20_PU_PD_EN_SHIFT (8U)
/*! GPIO20_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[20] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO20_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO20_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO20_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO21_PU_PD_EN_MASK (0xC00U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO21_PU_PD_EN_SHIFT (10U)
/*! GPIO21_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[21] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO21_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO21_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO21_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO28_PU_PD_EN_MASK (0x3000000U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO28_PU_PD_EN_SHIFT (24U)
/*! GPIO28_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[28] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO28_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO28_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO28_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO29_PU_PD_EN_MASK (0xC000000U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO29_PU_PD_EN_SHIFT (26U)
/*! GPIO29_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[29] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO29_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO29_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO29_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO30_PU_PD_EN_MASK (0x30000000U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO30_PU_PD_EN_SHIFT (28U)
/*! GPIO30_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[30] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO30_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO30_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO30_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN1_GPIO31_PU_PD_EN_MASK (0xC0000000U)
#define SOCCIU_PAD_PU_PD_EN1_GPIO31_PU_PD_EN_SHIFT (30U)
/*! GPIO31_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[31] */
#define SOCCIU_PAD_PU_PD_EN1_GPIO31_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN1_GPIO31_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN1_GPIO31_PU_PD_EN_MASK)
/*! @} */

/*! @name PAD_PU_PD_EN2 - Pad Pull-up Pull-down Enable2 */
/*! @{ */

#define SOCCIU_PAD_PU_PD_EN2_GPIO32_PU_PD_EN_MASK (0x3U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO32_PU_PD_EN_SHIFT (0U)
/*! GPIO32_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[32] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO32_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO32_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO32_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO33_PU_PD_EN_MASK (0xCU)
#define SOCCIU_PAD_PU_PD_EN2_GPIO33_PU_PD_EN_SHIFT (2U)
/*! GPIO33_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[33] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO33_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO33_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO33_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO34_PU_PD_EN_MASK (0x30U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO34_PU_PD_EN_SHIFT (4U)
/*! GPIO34_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[34] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO34_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO34_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO34_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO35_PU_PD_EN_MASK (0xC0U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO35_PU_PD_EN_SHIFT (6U)
/*! GPIO35_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[35] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO35_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO35_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO35_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO36_PU_PD_EN_MASK (0x300U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO36_PU_PD_EN_SHIFT (8U)
/*! GPIO36_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[36] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO36_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO36_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO36_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO37_PU_PD_EN_MASK (0xC00U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO37_PU_PD_EN_SHIFT (10U)
/*! GPIO37_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[37] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO37_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO37_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO37_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO38_PU_PD_EN_MASK (0x3000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO38_PU_PD_EN_SHIFT (12U)
/*! GPIO38_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[38] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO38_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO38_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO38_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO39_PU_PD_EN_MASK (0xC000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO39_PU_PD_EN_SHIFT (14U)
/*! GPIO39_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[39] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO39_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO39_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO39_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO40_PU_PD_EN_MASK (0x30000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO40_PU_PD_EN_SHIFT (16U)
/*! GPIO40_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[40] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO40_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO40_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO40_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO41_PU_PD_EN_MASK (0xC0000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO41_PU_PD_EN_SHIFT (18U)
/*! GPIO41_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[41] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO41_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO41_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO41_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO42_PU_PD_EN_MASK (0x300000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO42_PU_PD_EN_SHIFT (20U)
/*! GPIO42_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[42] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO42_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO42_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO42_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO43_PU_PD_EN_MASK (0xC00000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO43_PU_PD_EN_SHIFT (22U)
/*! GPIO43_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[43] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO43_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO43_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO43_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO44_PU_PD_EN_MASK (0x3000000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO44_PU_PD_EN_SHIFT (24U)
/*! GPIO44_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[44] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO44_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO44_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO44_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO45_PU_PD_EN_MASK (0xC000000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO45_PU_PD_EN_SHIFT (26U)
/*! GPIO45_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[45] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO45_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO45_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO45_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO46_PU_PD_EN_MASK (0x30000000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO46_PU_PD_EN_SHIFT (28U)
/*! GPIO46_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[46] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO46_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO46_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO46_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN2_GPIO47_PU_PD_EN_MASK (0xC0000000U)
#define SOCCIU_PAD_PU_PD_EN2_GPIO47_PU_PD_EN_SHIFT (30U)
/*! GPIO47_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[47] */
#define SOCCIU_PAD_PU_PD_EN2_GPIO47_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN2_GPIO47_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN2_GPIO47_PU_PD_EN_MASK)
/*! @} */

/*! @name PAD_PU_PD_EN3 - Pad Pull-up Pull-down Enable2 */
/*! @{ */

#define SOCCIU_PAD_PU_PD_EN3_GPIO48_PU_PD_EN_MASK (0x3U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO48_PU_PD_EN_SHIFT (0U)
/*! GPIO48_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[48] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO48_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO48_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO48_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO49_PU_PD_EN_MASK (0xCU)
#define SOCCIU_PAD_PU_PD_EN3_GPIO49_PU_PD_EN_SHIFT (2U)
/*! GPIO49_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[49] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO49_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO49_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO49_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO50_PU_PD_EN_MASK (0x30U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO50_PU_PD_EN_SHIFT (4U)
/*! GPIO50_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[50] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO50_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO50_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO50_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO51_PU_PD_EN_MASK (0xC0U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO51_PU_PD_EN_SHIFT (6U)
/*! GPIO51_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[51] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO51_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO51_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO51_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO52_PU_PD_EN_MASK (0x300U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO52_PU_PD_EN_SHIFT (8U)
/*! GPIO52_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[52] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO52_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO52_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO52_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO53_PU_PD_EN_MASK (0xC00U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO53_PU_PD_EN_SHIFT (10U)
/*! GPIO53_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[53] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO53_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO53_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO53_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO54_PU_PD_EN_MASK (0x3000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO54_PU_PD_EN_SHIFT (12U)
/*! GPIO54_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[54] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO54_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO54_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO54_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO55_PU_PD_EN_MASK (0xC000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO55_PU_PD_EN_SHIFT (14U)
/*! GPIO55_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[55] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO55_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO55_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO55_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO56_PU_PD_EN_MASK (0x30000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO56_PU_PD_EN_SHIFT (16U)
/*! GPIO56_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[56] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO56_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO56_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO56_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO57_PU_PD_EN_MASK (0xC0000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO57_PU_PD_EN_SHIFT (18U)
/*! GPIO57_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[57] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO57_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO57_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO57_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO58_PU_PD_EN_MASK (0x300000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO58_PU_PD_EN_SHIFT (20U)
/*! GPIO58_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[58] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO58_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO58_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO58_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO59_PU_PD_EN_MASK (0xC00000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO59_PU_PD_EN_SHIFT (22U)
/*! GPIO59_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[59] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO59_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO59_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO59_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO60_PU_PD_EN_MASK (0x3000000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO60_PU_PD_EN_SHIFT (24U)
/*! GPIO60_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[60] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO60_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO60_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO60_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO61_PU_PD_EN_MASK (0xC000000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO61_PU_PD_EN_SHIFT (26U)
/*! GPIO61_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[61] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO61_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO61_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO61_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO62_PU_PD_EN_MASK (0x30000000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO62_PU_PD_EN_SHIFT (28U)
/*! GPIO62_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[62] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO62_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO62_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO62_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN3_GPIO63_PU_PD_EN_MASK (0xC0000000U)
#define SOCCIU_PAD_PU_PD_EN3_GPIO63_PU_PD_EN_SHIFT (30U)
/*! GPIO63_PU_PD_EN - Internal Pd and Internal Pu Config for GPIO[63] */
#define SOCCIU_PAD_PU_PD_EN3_GPIO63_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN3_GPIO63_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN3_GPIO63_PU_PD_EN_MASK)
/*! @} */

/*! @name PAD_PU_PD_EN4 - Pad Pull-up Pull-down Enable2 */
/*! @{ */

#define SOCCIU_PAD_PU_PD_EN4_ATEST0_PU_PD_EN_MASK (0x3U)
#define SOCCIU_PAD_PU_PD_EN4_ATEST0_PU_PD_EN_SHIFT (0U)
/*! ATEST0_PU_PD_EN - Internal Pd and Internal Pu Config for ATEST0 */
#define SOCCIU_PAD_PU_PD_EN4_ATEST0_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN4_ATEST0_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN4_ATEST0_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN4_ATEST1_PU_PD_EN_MASK (0xCU)
#define SOCCIU_PAD_PU_PD_EN4_ATEST1_PU_PD_EN_SHIFT (2U)
/*! ATEST1_PU_PD_EN - Internal Pd and Internal Pu Config for ATEST1 */
#define SOCCIU_PAD_PU_PD_EN4_ATEST1_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN4_ATEST1_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN4_ATEST1_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN4_ATEST2_PU_PD_EN_MASK (0x30U)
#define SOCCIU_PAD_PU_PD_EN4_ATEST2_PU_PD_EN_SHIFT (4U)
/*! ATEST2_PU_PD_EN - Internal Pd and Internal Pu Config for ATEST2 */
#define SOCCIU_PAD_PU_PD_EN4_ATEST2_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN4_ATEST2_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN4_ATEST2_PU_PD_EN_MASK)

#define SOCCIU_PAD_PU_PD_EN4_ATEST3_PU_PD_EN_MASK (0xC0U)
#define SOCCIU_PAD_PU_PD_EN4_ATEST3_PU_PD_EN_SHIFT (6U)
/*! ATEST3_PU_PD_EN - Internal Pd and Internal Pu Config for ATEST3 */
#define SOCCIU_PAD_PU_PD_EN4_ATEST3_PU_PD_EN(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_PU_PD_EN4_ATEST3_PU_PD_EN_SHIFT)) & SOCCIU_PAD_PU_PD_EN4_ATEST3_PU_PD_EN_MASK)
/*! @} */

/*! @name PAD_SLP_EN0 - Pad Sleep Mode Enable */
/*! @{ */

#define SOCCIU_PAD_SLP_EN0_GPIO0_SLP_EN_MASK     (0x1U)
#define SOCCIU_PAD_SLP_EN0_GPIO0_SLP_EN_SHIFT    (0U)
/*! GPIO0_SLP_EN - Enable Forcing GPIO[0] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO0_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO0_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO0_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO1_SLP_EN_MASK     (0x2U)
#define SOCCIU_PAD_SLP_EN0_GPIO1_SLP_EN_SHIFT    (1U)
/*! GPIO1_SLP_EN - Enable Forcing GPIO[1] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO1_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO1_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO1_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO2_SLP_EN_MASK     (0x4U)
#define SOCCIU_PAD_SLP_EN0_GPIO2_SLP_EN_SHIFT    (2U)
/*! GPIO2_SLP_EN - Enable Forcing GPIO[2] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO2_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO2_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO2_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO3_SLP_EN_MASK     (0x8U)
#define SOCCIU_PAD_SLP_EN0_GPIO3_SLP_EN_SHIFT    (3U)
/*! GPIO3_SLP_EN - Enable Forcing GPIO[3] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO3_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO3_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO3_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO4_SLP_EN_MASK     (0x10U)
#define SOCCIU_PAD_SLP_EN0_GPIO4_SLP_EN_SHIFT    (4U)
/*! GPIO4_SLP_EN - Enable Forcing GPIO[4] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO4_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO4_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO4_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO5_SLP_EN_MASK     (0x20U)
#define SOCCIU_PAD_SLP_EN0_GPIO5_SLP_EN_SHIFT    (5U)
/*! GPIO5_SLP_EN - Enable Forcing GPIO[5] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO5_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO5_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO5_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO6_SLP_EN_MASK     (0x40U)
#define SOCCIU_PAD_SLP_EN0_GPIO6_SLP_EN_SHIFT    (6U)
/*! GPIO6_SLP_EN - Enable Forcing GPIO[6] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO6_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO6_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO6_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO7_SLP_EN_MASK     (0x80U)
#define SOCCIU_PAD_SLP_EN0_GPIO7_SLP_EN_SHIFT    (7U)
/*! GPIO7_SLP_EN - Enable Forcing GPIO[7] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO7_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO7_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO7_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO8_SLP_EN_MASK     (0x100U)
#define SOCCIU_PAD_SLP_EN0_GPIO8_SLP_EN_SHIFT    (8U)
/*! GPIO8_SLP_EN - Enable Forcing GPIO[8] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO8_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO8_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO8_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO9_SLP_EN_MASK     (0x200U)
#define SOCCIU_PAD_SLP_EN0_GPIO9_SLP_EN_SHIFT    (9U)
/*! GPIO9_SLP_EN - Enable Forcing GPIO[9] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO9_SLP_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO9_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO9_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO10_SLP_EN_MASK    (0x400U)
#define SOCCIU_PAD_SLP_EN0_GPIO10_SLP_EN_SHIFT   (10U)
/*! GPIO10_SLP_EN - Enable Forcing GPIO[10] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO10_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO10_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO10_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO11_SLP_EN_MASK    (0x800U)
#define SOCCIU_PAD_SLP_EN0_GPIO11_SLP_EN_SHIFT   (11U)
/*! GPIO11_SLP_EN - Enable Forcing GPIO[11] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO11_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO11_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO11_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO12_SLP_EN_MASK    (0x1000U)
#define SOCCIU_PAD_SLP_EN0_GPIO12_SLP_EN_SHIFT   (12U)
/*! GPIO12_SLP_EN - Enable Forcing GPIO[12] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO12_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO12_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO12_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO13_SLP_EN_MASK    (0x2000U)
#define SOCCIU_PAD_SLP_EN0_GPIO13_SLP_EN_SHIFT   (13U)
/*! GPIO13_SLP_EN - Enable Forcing GPIO[13] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO13_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO13_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO13_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO14_SLP_EN_MASK    (0x4000U)
#define SOCCIU_PAD_SLP_EN0_GPIO14_SLP_EN_SHIFT   (14U)
/*! GPIO14_SLP_EN - Enable Forcing GPIO[14] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO14_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO14_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO14_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO15_SLP_EN_MASK    (0x8000U)
#define SOCCIU_PAD_SLP_EN0_GPIO15_SLP_EN_SHIFT   (15U)
/*! GPIO15_SLP_EN - Enable Forcing GPIO[15] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO15_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO15_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO15_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO16_SLP_EN_MASK    (0x10000U)
#define SOCCIU_PAD_SLP_EN0_GPIO16_SLP_EN_SHIFT   (16U)
/*! GPIO16_SLP_EN - Enable Forcing GPIO[16] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO16_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO16_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO16_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO17_SLP_EN_MASK    (0x20000U)
#define SOCCIU_PAD_SLP_EN0_GPIO17_SLP_EN_SHIFT   (17U)
/*! GPIO17_SLP_EN - Enable Forcing GPIO[17] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO17_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO17_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO17_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO18_SLP_EN_MASK    (0x40000U)
#define SOCCIU_PAD_SLP_EN0_GPIO18_SLP_EN_SHIFT   (18U)
/*! GPIO18_SLP_EN - Enable Forcing GPIO[18] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO18_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO18_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO18_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO19_SLP_EN_MASK    (0x80000U)
#define SOCCIU_PAD_SLP_EN0_GPIO19_SLP_EN_SHIFT   (19U)
/*! GPIO19_SLP_EN - Enable Forcing GPIO[19] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO19_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO19_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO19_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO20_SLP_EN_MASK    (0x100000U)
#define SOCCIU_PAD_SLP_EN0_GPIO20_SLP_EN_SHIFT   (20U)
/*! GPIO20_SLP_EN - Enable Forcing GPIO[20] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO20_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO20_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO20_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO21_SLP_EN_MASK    (0x200000U)
#define SOCCIU_PAD_SLP_EN0_GPIO21_SLP_EN_SHIFT   (21U)
/*! GPIO21_SLP_EN - Enable Forcing GPIO[21] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO21_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO21_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO21_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO28_SLP_EN_MASK    (0x10000000U)
#define SOCCIU_PAD_SLP_EN0_GPIO28_SLP_EN_SHIFT   (28U)
/*! GPIO28_SLP_EN - Enable Forcing GPIO[28] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO28_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO28_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO28_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO29_SLP_EN_MASK    (0x20000000U)
#define SOCCIU_PAD_SLP_EN0_GPIO29_SLP_EN_SHIFT   (29U)
/*! GPIO29_SLP_EN - Enable Forcing GPIO[29] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO29_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO29_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO29_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO30_SLP_EN_MASK    (0x40000000U)
#define SOCCIU_PAD_SLP_EN0_GPIO30_SLP_EN_SHIFT   (30U)
/*! GPIO30_SLP_EN - Enable Forcing GPIO[30] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO30_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO30_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO30_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN0_GPIO31_SLP_EN_MASK    (0x80000000U)
#define SOCCIU_PAD_SLP_EN0_GPIO31_SLP_EN_SHIFT   (31U)
/*! GPIO31_SLP_EN - Enable Forcing GPIO[31] Output During Sleep */
#define SOCCIU_PAD_SLP_EN0_GPIO31_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN0_GPIO31_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN0_GPIO31_SLP_EN_MASK)
/*! @} */

/*! @name PAD_SLP_EN1 - Pad Sleep Mode Enable */
/*! @{ */

#define SOCCIU_PAD_SLP_EN1_GPIO32_SLP_EN_MASK    (0x1U)
#define SOCCIU_PAD_SLP_EN1_GPIO32_SLP_EN_SHIFT   (0U)
/*! GPIO32_SLP_EN - Enable Forcing GPIO[32] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO32_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO32_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO32_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO33_SLP_EN_MASK    (0x2U)
#define SOCCIU_PAD_SLP_EN1_GPIO33_SLP_EN_SHIFT   (1U)
/*! GPIO33_SLP_EN - Enable Forcing GPIO[33] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO33_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO33_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO33_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO34_SLP_EN_MASK    (0x4U)
#define SOCCIU_PAD_SLP_EN1_GPIO34_SLP_EN_SHIFT   (2U)
/*! GPIO34_SLP_EN - Enable Forcing GPIO[34] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO34_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO34_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO34_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO35_SLP_EN_MASK    (0x8U)
#define SOCCIU_PAD_SLP_EN1_GPIO35_SLP_EN_SHIFT   (3U)
/*! GPIO35_SLP_EN - Enable Forcing GPIO[35] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO35_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO35_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO35_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO36_SLP_EN_MASK    (0x10U)
#define SOCCIU_PAD_SLP_EN1_GPIO36_SLP_EN_SHIFT   (4U)
/*! GPIO36_SLP_EN - Enable Forcing GPIO[36] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO36_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO36_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO36_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO37_SLP_EN_MASK    (0x20U)
#define SOCCIU_PAD_SLP_EN1_GPIO37_SLP_EN_SHIFT   (5U)
/*! GPIO37_SLP_EN - Enable Forcing GPIO[37] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO37_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO37_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO37_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO38_SLP_EN_MASK    (0x40U)
#define SOCCIU_PAD_SLP_EN1_GPIO38_SLP_EN_SHIFT   (6U)
/*! GPIO38_SLP_EN - Enable Forcing GPIO[38] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO38_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO38_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO38_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO39_SLP_EN_MASK    (0x80U)
#define SOCCIU_PAD_SLP_EN1_GPIO39_SLP_EN_SHIFT   (7U)
/*! GPIO39_SLP_EN - Enable Forcing GPIO[39] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO39_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO39_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO39_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO40_SLP_EN_MASK    (0x100U)
#define SOCCIU_PAD_SLP_EN1_GPIO40_SLP_EN_SHIFT   (8U)
/*! GPIO40_SLP_EN - Enable Forcing GPIO[40] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO40_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO40_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO40_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO41_SLP_EN_MASK    (0x200U)
#define SOCCIU_PAD_SLP_EN1_GPIO41_SLP_EN_SHIFT   (9U)
/*! GPIO41_SLP_EN - Enable Forcing GPIO[41] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO41_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO41_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO41_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO42_SLP_EN_MASK    (0x400U)
#define SOCCIU_PAD_SLP_EN1_GPIO42_SLP_EN_SHIFT   (10U)
/*! GPIO42_SLP_EN - Enable Forcing GPIO[42] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO42_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO42_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO42_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO43_SLP_EN_MASK    (0x800U)
#define SOCCIU_PAD_SLP_EN1_GPIO43_SLP_EN_SHIFT   (11U)
/*! GPIO43_SLP_EN - Enable Forcing GPIO[43] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO43_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO43_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO43_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO44_SLP_EN_MASK    (0x1000U)
#define SOCCIU_PAD_SLP_EN1_GPIO44_SLP_EN_SHIFT   (12U)
/*! GPIO44_SLP_EN - Enable Forcing GPIO[44] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO44_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO44_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO44_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO45_SLP_EN_MASK    (0x2000U)
#define SOCCIU_PAD_SLP_EN1_GPIO45_SLP_EN_SHIFT   (13U)
/*! GPIO45_SLP_EN - Enable Forcing GPIO[45] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO45_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO45_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO45_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO46_SLP_EN_MASK    (0x4000U)
#define SOCCIU_PAD_SLP_EN1_GPIO46_SLP_EN_SHIFT   (14U)
/*! GPIO46_SLP_EN - Enable Forcing GPIO[46] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO46_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO46_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO46_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO47_SLP_EN_MASK    (0x8000U)
#define SOCCIU_PAD_SLP_EN1_GPIO47_SLP_EN_SHIFT   (15U)
/*! GPIO47_SLP_EN - Enable Forcing GPIO[47] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO47_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO47_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO47_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO48_SLP_EN_MASK    (0x10000U)
#define SOCCIU_PAD_SLP_EN1_GPIO48_SLP_EN_SHIFT   (16U)
/*! GPIO48_SLP_EN - Enable Forcing GPIO[48] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO48_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO48_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO48_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO49_SLP_EN_MASK    (0x20000U)
#define SOCCIU_PAD_SLP_EN1_GPIO49_SLP_EN_SHIFT   (17U)
/*! GPIO49_SLP_EN - Enable Forcing GPIO[49] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO49_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO49_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO49_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO50_SLP_EN_MASK    (0x40000U)
#define SOCCIU_PAD_SLP_EN1_GPIO50_SLP_EN_SHIFT   (18U)
/*! GPIO50_SLP_EN - Enable Forcing GPIO[50] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO50_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO50_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO50_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO51_SLP_EN_MASK    (0x80000U)
#define SOCCIU_PAD_SLP_EN1_GPIO51_SLP_EN_SHIFT   (19U)
/*! GPIO51_SLP_EN - Enable Forcing GPIO[51] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO51_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO51_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO51_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO52_SLP_EN_MASK    (0x100000U)
#define SOCCIU_PAD_SLP_EN1_GPIO52_SLP_EN_SHIFT   (20U)
/*! GPIO52_SLP_EN - Enable Forcing GPIO[52] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO52_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO52_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO52_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO53_SLP_EN_MASK    (0x200000U)
#define SOCCIU_PAD_SLP_EN1_GPIO53_SLP_EN_SHIFT   (21U)
/*! GPIO53_SLP_EN - Enable Forcing GPIO[53] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO53_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO53_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO53_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO54_SLP_EN_MASK    (0x400000U)
#define SOCCIU_PAD_SLP_EN1_GPIO54_SLP_EN_SHIFT   (22U)
/*! GPIO54_SLP_EN - Enable Forcing GPIO[54] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO54_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO54_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO54_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO55_SLP_EN_MASK    (0x800000U)
#define SOCCIU_PAD_SLP_EN1_GPIO55_SLP_EN_SHIFT   (23U)
/*! GPIO55_SLP_EN - Enable Forcing GPIO[55] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO55_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO55_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO55_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO56_SLP_EN_MASK    (0x1000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO56_SLP_EN_SHIFT   (24U)
/*! GPIO56_SLP_EN - Enable Forcing GPIO[56] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO56_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO56_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO56_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO57_SLP_EN_MASK    (0x2000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO57_SLP_EN_SHIFT   (25U)
/*! GPIO57_SLP_EN - Enable Forcing GPIO[57] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO57_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO57_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO57_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO58_SLP_EN_MASK    (0x4000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO58_SLP_EN_SHIFT   (26U)
/*! GPIO58_SLP_EN - Enable Forcing GPIO[58] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO58_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO58_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO58_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO59_SLP_EN_MASK    (0x8000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO59_SLP_EN_SHIFT   (27U)
/*! GPIO59_SLP_EN - Enable Forcing GPIO[59] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO59_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO59_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO59_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO60_SLP_EN_MASK    (0x10000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO60_SLP_EN_SHIFT   (28U)
/*! GPIO60_SLP_EN - Enable Forcing GPIO[60] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO60_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO60_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO60_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO61_SLP_EN_MASK    (0x20000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO61_SLP_EN_SHIFT   (29U)
/*! GPIO61_SLP_EN - Enable Forcing GPIO[61] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO61_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO61_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO61_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO62_SLP_EN_MASK    (0x40000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO62_SLP_EN_SHIFT   (30U)
/*! GPIO62_SLP_EN - Enable Forcing GPIO[62] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO62_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO62_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO62_SLP_EN_MASK)

#define SOCCIU_PAD_SLP_EN1_GPIO63_SLP_EN_MASK    (0x80000000U)
#define SOCCIU_PAD_SLP_EN1_GPIO63_SLP_EN_SHIFT   (31U)
/*! GPIO63_SLP_EN - Enable Forcing GPIO[63] Output During Sleep */
#define SOCCIU_PAD_SLP_EN1_GPIO63_SLP_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_EN1_GPIO63_SLP_EN_SHIFT)) & SOCCIU_PAD_SLP_EN1_GPIO63_SLP_EN_MASK)
/*! @} */

/*! @name PAD_SLP_VAL0 - Pad Sleep Mode Value */
/*! @{ */

#define SOCCIU_PAD_SLP_VAL0_GPIO0_SLP_VAL_MASK   (0x1U)
#define SOCCIU_PAD_SLP_VAL0_GPIO0_SLP_VAL_SHIFT  (0U)
/*! GPIO0_SLP_VAL - Force GPIO[0] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO0_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO0_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO0_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO1_SLP_VAL_MASK   (0x2U)
#define SOCCIU_PAD_SLP_VAL0_GPIO1_SLP_VAL_SHIFT  (1U)
/*! GPIO1_SLP_VAL - Force GPIO[1] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO1_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO1_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO1_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO2_SLP_VAL_MASK   (0x4U)
#define SOCCIU_PAD_SLP_VAL0_GPIO2_SLP_VAL_SHIFT  (2U)
/*! GPIO2_SLP_VAL - Force GPIO[2] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO2_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO2_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO2_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO3_SLP_VAL_MASK   (0x8U)
#define SOCCIU_PAD_SLP_VAL0_GPIO3_SLP_VAL_SHIFT  (3U)
/*! GPIO3_SLP_VAL - Force GPIO[3] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO3_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO3_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO3_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO4_SLP_VAL_MASK   (0x10U)
#define SOCCIU_PAD_SLP_VAL0_GPIO4_SLP_VAL_SHIFT  (4U)
/*! GPIO4_SLP_VAL - Force GPIO[4] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO4_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO4_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO4_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO5_SLP_VAL_MASK   (0x20U)
#define SOCCIU_PAD_SLP_VAL0_GPIO5_SLP_VAL_SHIFT  (5U)
/*! GPIO5_SLP_VAL - Force GPIO[5] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO5_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO5_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO5_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO6_SLP_VAL_MASK   (0x40U)
#define SOCCIU_PAD_SLP_VAL0_GPIO6_SLP_VAL_SHIFT  (6U)
/*! GPIO6_SLP_VAL - Force GPIO[6] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO6_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO6_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO6_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO7_SLP_VAL_MASK   (0x80U)
#define SOCCIU_PAD_SLP_VAL0_GPIO7_SLP_VAL_SHIFT  (7U)
/*! GPIO7_SLP_VAL - Force GPIO[7] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO7_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO7_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO7_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO8_SLP_VAL_MASK   (0x100U)
#define SOCCIU_PAD_SLP_VAL0_GPIO8_SLP_VAL_SHIFT  (8U)
/*! GPIO8_SLP_VAL - Force GPIO[8] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO8_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO8_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO8_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO9_SLP_VAL_MASK   (0x200U)
#define SOCCIU_PAD_SLP_VAL0_GPIO9_SLP_VAL_SHIFT  (9U)
/*! GPIO9_SLP_VAL - Force GPIO[9] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO9_SLP_VAL(x)     (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO9_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO9_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO10_SLP_VAL_MASK  (0x400U)
#define SOCCIU_PAD_SLP_VAL0_GPIO10_SLP_VAL_SHIFT (10U)
/*! GPIO10_SLP_VAL - Force GPIO[10] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO10_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO10_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO10_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO11_SLP_VAL_MASK  (0x800U)
#define SOCCIU_PAD_SLP_VAL0_GPIO11_SLP_VAL_SHIFT (11U)
/*! GPIO11_SLP_VAL - Force GPIO[11] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO11_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO11_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO11_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO12_SLP_VAL_MASK  (0x1000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO12_SLP_VAL_SHIFT (12U)
/*! GPIO12_SLP_VAL - Force GPIO[12] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO12_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO12_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO12_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO13_SLP_VAL_MASK  (0x2000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO13_SLP_VAL_SHIFT (13U)
/*! GPIO13_SLP_VAL - Force GPIO[13] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO13_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO13_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO13_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO14_SLP_VAL_MASK  (0x4000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO14_SLP_VAL_SHIFT (14U)
/*! GPIO14_SLP_VAL - Force GPIO[14] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO14_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO14_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO14_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO15_SLP_VAL_MASK  (0x8000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO15_SLP_VAL_SHIFT (15U)
/*! GPIO15_SLP_VAL - Force GPIO[15] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO15_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO15_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO15_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO16_SLP_VAL_MASK  (0x10000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO16_SLP_VAL_SHIFT (16U)
/*! GPIO16_SLP_VAL - Force GPIO[16] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO16_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO16_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO16_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO17_SLP_VAL_MASK  (0x20000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO17_SLP_VAL_SHIFT (17U)
/*! GPIO17_SLP_VAL - Force GPIO[17] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO17_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO17_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO17_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO18_SLP_VAL_MASK  (0x40000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO18_SLP_VAL_SHIFT (18U)
/*! GPIO18_SLP_VAL - Force GPIO[18] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO18_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO18_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO18_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO19_SLP_VAL_MASK  (0x80000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO19_SLP_VAL_SHIFT (19U)
/*! GPIO19_SLP_VAL - Force GPIO[19] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO19_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO19_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO19_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO20_SLP_VAL_MASK  (0x100000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO20_SLP_VAL_SHIFT (20U)
/*! GPIO20_SLP_VAL - Force GPIO[20] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO20_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO20_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO20_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO21_SLP_VAL_MASK  (0x200000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO21_SLP_VAL_SHIFT (21U)
/*! GPIO21_SLP_VAL - Force GPIO[21] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO21_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO21_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO21_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO28_SLP_VAL_MASK  (0x10000000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO28_SLP_VAL_SHIFT (28U)
/*! GPIO28_SLP_VAL - Force GPIO[28] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO28_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO28_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO28_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO29_SLP_VAL_MASK  (0x20000000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO29_SLP_VAL_SHIFT (29U)
/*! GPIO29_SLP_VAL - Force GPIO[29] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO29_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO29_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO29_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO30_SLP_VAL_MASK  (0x40000000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO30_SLP_VAL_SHIFT (30U)
/*! GPIO30_SLP_VAL - Force GPIO[30] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO30_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO30_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO30_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL0_GPIO31_SLP_VAL_MASK  (0x80000000U)
#define SOCCIU_PAD_SLP_VAL0_GPIO31_SLP_VAL_SHIFT (31U)
/*! GPIO31_SLP_VAL - Force GPIO[31] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL0_GPIO31_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL0_GPIO31_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL0_GPIO31_SLP_VAL_MASK)
/*! @} */

/*! @name PAD_SLP_VAL1 - Pad Sleep Mode Value */
/*! @{ */

#define SOCCIU_PAD_SLP_VAL1_GPIO32_SLP_VAL_MASK  (0x1U)
#define SOCCIU_PAD_SLP_VAL1_GPIO32_SLP_VAL_SHIFT (0U)
/*! GPIO32_SLP_VAL - Force GPIO[32] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO32_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO32_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO32_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO33_SLP_VAL_MASK  (0x2U)
#define SOCCIU_PAD_SLP_VAL1_GPIO33_SLP_VAL_SHIFT (1U)
/*! GPIO33_SLP_VAL - Force GPIO[33] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO33_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO33_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO33_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO34_SLP_VAL_MASK  (0x4U)
#define SOCCIU_PAD_SLP_VAL1_GPIO34_SLP_VAL_SHIFT (2U)
/*! GPIO34_SLP_VAL - Force GPIO[34] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO34_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO34_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO34_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO35_SLP_VAL_MASK  (0x8U)
#define SOCCIU_PAD_SLP_VAL1_GPIO35_SLP_VAL_SHIFT (3U)
/*! GPIO35_SLP_VAL - Force GPIO[35] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO35_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO35_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO35_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO36_SLP_VAL_MASK  (0x10U)
#define SOCCIU_PAD_SLP_VAL1_GPIO36_SLP_VAL_SHIFT (4U)
/*! GPIO36_SLP_VAL - Force GPIO[36] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO36_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO36_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO36_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO37_SLP_VAL_MASK  (0x20U)
#define SOCCIU_PAD_SLP_VAL1_GPIO37_SLP_VAL_SHIFT (5U)
/*! GPIO37_SLP_VAL - Force GPIO[37] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO37_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO37_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO37_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO38_SLP_VAL_MASK  (0x40U)
#define SOCCIU_PAD_SLP_VAL1_GPIO38_SLP_VAL_SHIFT (6U)
/*! GPIO38_SLP_VAL - Force GPIO[38] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO38_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO38_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO38_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO39_SLP_VAL_MASK  (0x80U)
#define SOCCIU_PAD_SLP_VAL1_GPIO39_SLP_VAL_SHIFT (7U)
/*! GPIO39_SLP_VAL - Force GPIO[39] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO39_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO39_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO39_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO40_SLP_VAL_MASK  (0x100U)
#define SOCCIU_PAD_SLP_VAL1_GPIO40_SLP_VAL_SHIFT (8U)
/*! GPIO40_SLP_VAL - Force GPIO[40] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO40_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO40_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO40_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO41_SLP_VAL_MASK  (0x200U)
#define SOCCIU_PAD_SLP_VAL1_GPIO41_SLP_VAL_SHIFT (9U)
/*! GPIO41_SLP_VAL - Force GPIO[41] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO41_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO41_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO41_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO42_SLP_VAL_MASK  (0x400U)
#define SOCCIU_PAD_SLP_VAL1_GPIO42_SLP_VAL_SHIFT (10U)
/*! GPIO42_SLP_VAL - Force GPIO[42] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO42_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO42_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO42_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO43_SLP_VAL_MASK  (0x800U)
#define SOCCIU_PAD_SLP_VAL1_GPIO43_SLP_VAL_SHIFT (11U)
/*! GPIO43_SLP_VAL - Force GPIO[43] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO43_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO43_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO43_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO44_SLP_VAL_MASK  (0x1000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO44_SLP_VAL_SHIFT (12U)
/*! GPIO44_SLP_VAL - Force GPIO[44] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO44_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO44_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO44_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO45_SLP_VAL_MASK  (0x2000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO45_SLP_VAL_SHIFT (13U)
/*! GPIO45_SLP_VAL - Force GPIO[45] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO45_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO45_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO45_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO46_SLP_VAL_MASK  (0x4000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO46_SLP_VAL_SHIFT (14U)
/*! GPIO46_SLP_VAL - Force GPIO[46] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO46_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO46_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO46_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO47_SLP_VAL_MASK  (0x8000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO47_SLP_VAL_SHIFT (15U)
/*! GPIO47_SLP_VAL - Force GPIO[47] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO47_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO47_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO47_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO48_SLP_VAL_MASK  (0x10000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO48_SLP_VAL_SHIFT (16U)
/*! GPIO48_SLP_VAL - Force GPIO[48] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO48_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO48_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO48_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO49_SLP_VAL_MASK  (0x20000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO49_SLP_VAL_SHIFT (17U)
/*! GPIO49_SLP_VAL - Force GPIO[49] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO49_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO49_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO49_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO50_SLP_VAL_MASK  (0x40000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO50_SLP_VAL_SHIFT (18U)
/*! GPIO50_SLP_VAL - Force GPIO[50] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO50_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO50_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO50_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO51_SLP_VAL_MASK  (0x80000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO51_SLP_VAL_SHIFT (19U)
/*! GPIO51_SLP_VAL - Force GPIO[51] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO51_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO51_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO51_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO52_SLP_VAL_MASK  (0x100000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO52_SLP_VAL_SHIFT (20U)
/*! GPIO52_SLP_VAL - Force GPIO[52] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO52_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO52_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO52_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO53_SLP_VAL_MASK  (0x200000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO53_SLP_VAL_SHIFT (21U)
/*! GPIO53_SLP_VAL - Force GPIO[53] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO53_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO53_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO53_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO54_SLP_VAL_MASK  (0x400000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO54_SLP_VAL_SHIFT (22U)
/*! GPIO54_SLP_VAL - Force GPIO[54] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO54_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO54_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO54_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO55_SLP_VAL_MASK  (0x800000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO55_SLP_VAL_SHIFT (23U)
/*! GPIO55_SLP_VAL - Force GPIO[55] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO55_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO55_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO55_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO56_SLP_VAL_MASK  (0x1000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO56_SLP_VAL_SHIFT (24U)
/*! GPIO56_SLP_VAL - Force GPIO[56] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO56_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO56_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO56_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO57_SLP_VAL_MASK  (0x2000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO57_SLP_VAL_SHIFT (25U)
/*! GPIO57_SLP_VAL - Force GPIO[57] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO57_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO57_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO57_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO58_SLP_VAL_MASK  (0x4000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO58_SLP_VAL_SHIFT (26U)
/*! GPIO58_SLP_VAL - Force GPIO[58] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO58_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO58_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO58_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO59_SLP_VAL_MASK  (0x8000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO59_SLP_VAL_SHIFT (27U)
/*! GPIO59_SLP_VAL - Force GPIO[59] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO59_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO59_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO59_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO60_SLP_VAL_MASK  (0x10000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO60_SLP_VAL_SHIFT (28U)
/*! GPIO60_SLP_VAL - Force GPIO[60] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO60_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO60_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO60_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO61_SLP_VAL_MASK  (0x20000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO61_SLP_VAL_SHIFT (29U)
/*! GPIO61_SLP_VAL - Force GPIO[61] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO61_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO61_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO61_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO62_SLP_VAL_MASK  (0x40000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO62_SLP_VAL_SHIFT (30U)
/*! GPIO62_SLP_VAL - Force GPIO[62] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO62_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO62_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO62_SLP_VAL_MASK)

#define SOCCIU_PAD_SLP_VAL1_GPIO63_SLP_VAL_MASK  (0x80000000U)
#define SOCCIU_PAD_SLP_VAL1_GPIO63_SLP_VAL_SHIFT (31U)
/*! GPIO63_SLP_VAL - Force GPIO[63] Output During Sleep */
#define SOCCIU_PAD_SLP_VAL1_GPIO63_SLP_VAL(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_VAL1_GPIO63_SLP_VAL_SHIFT)) & SOCCIU_PAD_SLP_VAL1_GPIO63_SLP_VAL_MASK)
/*! @} */

/*! @name PSW_VD2_RDY0 - Power Switch VD2_RDY Status */
/*! @{ */

#define SOCCIU_PSW_VD2_RDY0_PSW_STATUS_MASK      (0xFFFFFFFFU)
#define SOCCIU_PSW_VD2_RDY0_PSW_STATUS_SHIFT     (0U)
/*! PSW_STATUS - VD2_RDY Status of following Power Switches: */
#define SOCCIU_PSW_VD2_RDY0_PSW_STATUS(x)        (((uint32_t)(((uint32_t)(x)) << SOCCIU_PSW_VD2_RDY0_PSW_STATUS_SHIFT)) & SOCCIU_PSW_VD2_RDY0_PSW_STATUS_MASK)
/*! @} */

/*! @name PSW_ECO_CTRL - Power Switch ECO Control */
/*! @{ */

#define SOCCIU_PSW_ECO_CTRL_ECO_BITS_MASK        (0xFFFFFFFFU)
#define SOCCIU_PSW_ECO_CTRL_ECO_BITS_SHIFT       (0U)
/*! ECO_BITS - Reserved for ECOs */
#define SOCCIU_PSW_ECO_CTRL_ECO_BITS(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_PSW_ECO_CTRL_ECO_BITS_SHIFT)) & SOCCIU_PSW_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name CLK_SW - Clock Controls for SOC_CLK_TOP */
/*! @{ */

#define SOCCIU_CLK_SW_CAU_SIF_AHB_CLK_EN_MASK    (0x1U)
#define SOCCIU_CLK_SW_CAU_SIF_AHB_CLK_EN_SHIFT   (0U)
/*! CAU_SIF_AHB_CLK_EN - " */
#define SOCCIU_CLK_SW_CAU_SIF_AHB_CLK_EN(x)      (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_CAU_SIF_AHB_CLK_EN_SHIFT)) & SOCCIU_CLK_SW_CAU_SIF_AHB_CLK_EN_MASK)

#define SOCCIU_CLK_SW_AHB2APB_CLK_EN_MASK        (0x2U)
#define SOCCIU_CLK_SW_AHB2APB_CLK_EN_SHIFT       (1U)
/*! AHB2APB_CLK_EN - " */
#define SOCCIU_CLK_SW_AHB2APB_CLK_EN(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_AHB2APB_CLK_EN_SHIFT)) & SOCCIU_CLK_SW_AHB2APB_CLK_EN_MASK)

#define SOCCIU_CLK_SW_DAPCLKEN_MASK              (0x20U)
#define SOCCIU_CLK_SW_DAPCLKEN_SHIFT             (5U)
/*! DAPCLKEN - clk en for SOC DAP */
#define SOCCIU_CLK_SW_DAPCLKEN(x)                (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_DAPCLKEN_SHIFT)) & SOCCIU_CLK_SW_DAPCLKEN_MASK)

#define SOCCIU_CLK_SW_WL_CM3_DAPCLKEN_MASK       (0x40U)
#define SOCCIU_CLK_SW_WL_CM3_DAPCLKEN_SHIFT      (6U)
/*! WL_CM3_DAPCLKEN - clk en for CPU1 DAP interface */
#define SOCCIU_CLK_SW_WL_CM3_DAPCLKEN(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_WL_CM3_DAPCLKEN_SHIFT)) & SOCCIU_CLK_SW_WL_CM3_DAPCLKEN_MASK)

#define SOCCIU_CLK_SW_BT_CM3_DAPCLKEN_MASK       (0x80U)
#define SOCCIU_CLK_SW_BT_CM3_DAPCLKEN_SHIFT      (7U)
/*! BT_CM3_DAPCLKEN - clk en for CPU2 DAP interface */
#define SOCCIU_CLK_SW_BT_CM3_DAPCLKEN(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_BT_CM3_DAPCLKEN_SHIFT)) & SOCCIU_CLK_SW_BT_CM3_DAPCLKEN_MASK)

#define SOCCIU_CLK_SW_CSCLKEN_MASK               (0x200U)
#define SOCCIU_CLK_SW_CSCLKEN_SHIFT              (9U)
/*! CSCLKEN - clk en for SOC Coresight system (includes CTI, CTM, TPIU, ATB Funnel, ATB upsizer) */
#define SOCCIU_CLK_SW_CSCLKEN(x)                 (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_CSCLKEN_SHIFT)) & SOCCIU_CLK_SW_CSCLKEN_MASK)

#define SOCCIU_CLK_SW_WL_CM3_CSCLKEN_MASK        (0x400U)
#define SOCCIU_CLK_SW_WL_CM3_CSCLKEN_SHIFT       (10U)
/*! WL_CM3_CSCLKEN - clk en for CPU1 ATB (ETM and ITM) and CTI interface */
#define SOCCIU_CLK_SW_WL_CM3_CSCLKEN(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_WL_CM3_CSCLKEN_SHIFT)) & SOCCIU_CLK_SW_WL_CM3_CSCLKEN_MASK)

#define SOCCIU_CLK_SW_BT_CM3_CSCLKEN_MASK        (0x800U)
#define SOCCIU_CLK_SW_BT_CM3_CSCLKEN_SHIFT       (11U)
/*! BT_CM3_CSCLKEN - clk en for CPU2 ATB (ETM and ITM) and CTI interface */
#define SOCCIU_CLK_SW_BT_CM3_CSCLKEN(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_BT_CM3_CSCLKEN_SHIFT)) & SOCCIU_CLK_SW_BT_CM3_CSCLKEN_MASK)

#define SOCCIU_CLK_SW_CAU_SIF_CLK_SEL_MASK       (0x10000U)
#define SOCCIU_CLK_SW_CAU_SIF_CLK_SEL_SHIFT      (16U)
/*! CAU_SIF_CLK_SEL - CAU SIF Clock Frequency Select */
#define SOCCIU_CLK_SW_CAU_SIF_CLK_SEL(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_CAU_SIF_CLK_SEL_SHIFT)) & SOCCIU_CLK_SW_CAU_SIF_CLK_SEL_MASK)

#define SOCCIU_CLK_SW_SOC_AHB_CLK_SEL_MASK       (0x20000U)
#define SOCCIU_CLK_SW_SOC_AHB_CLK_SEL_SHIFT      (17U)
/*! SOC_AHB_CLK_SEL - SOC TOP AHB Clock Frequency Select */
#define SOCCIU_CLK_SW_SOC_AHB_CLK_SEL(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SW_SOC_AHB_CLK_SEL_SHIFT)) & SOCCIU_CLK_SW_SOC_AHB_CLK_SEL_MASK)
/*! @} */

/*! @name RST_SW - Reset Controls for SOC_RESET_GEN */
/*! @{ */

#define SOCCIU_RST_SW_CAU_SIF_RSTN_MASK          (0x1U)
#define SOCCIU_RST_SW_CAU_SIF_RSTN_SHIFT         (0U)
/*! CAU_SIF_RSTN - " */
#define SOCCIU_RST_SW_CAU_SIF_RSTN(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_CAU_SIF_RSTN_SHIFT)) & SOCCIU_RST_SW_CAU_SIF_RSTN_MASK)

#define SOCCIU_RST_SW_CAU_SIF_HRESETN_MASK       (0x2U)
#define SOCCIU_RST_SW_CAU_SIF_HRESETN_SHIFT      (1U)
/*! CAU_SIF_HRESETN - " */
#define SOCCIU_RST_SW_CAU_SIF_HRESETN(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_CAU_SIF_HRESETN_SHIFT)) & SOCCIU_RST_SW_CAU_SIF_HRESETN_MASK)

#define SOCCIU_RST_SW_SOC_PERI_HRESETN_MASK      (0x4U)
#define SOCCIU_RST_SW_SOC_PERI_HRESETN_SHIFT     (2U)
/*! SOC_PERI_HRESETN - Falling edge detected on this in RTL to reset the ahb bus */
#define SOCCIU_RST_SW_SOC_PERI_HRESETN(x)        (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_SOC_PERI_HRESETN_SHIFT)) & SOCCIU_RST_SW_SOC_PERI_HRESETN_MASK)

#define SOCCIU_RST_SW_DRO_RSTN_MASK              (0x8U)
#define SOCCIU_RST_SW_DRO_RSTN_SHIFT             (3U)
/*! DRO_RSTN - DRO Clock Reset */
#define SOCCIU_RST_SW_DRO_RSTN(x)                (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_DRO_RSTN_SHIFT)) & SOCCIU_RST_SW_DRO_RSTN_MASK)

#define SOCCIU_RST_SW_AHB2APB_HRESETN_MASK       (0x10U)
#define SOCCIU_RST_SW_AHB2APB_HRESETN_SHIFT      (4U)
/*! AHB2APB_HRESETN - " */
#define SOCCIU_RST_SW_AHB2APB_HRESETN(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_AHB2APB_HRESETN_SHIFT)) & SOCCIU_RST_SW_AHB2APB_HRESETN_MASK)

#define SOCCIU_RST_SW_HCLK__MASK                 (0x20U)
#define SOCCIU_RST_SW_HCLK__SHIFT                (5U)
/*! HCLK_ - Auto clear SW reset for socciu */
#define SOCCIU_RST_SW_HCLK_(x)                   (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_HCLK__SHIFT)) & SOCCIU_RST_SW_HCLK__MASK)

#define SOCCIU_RST_SW_CSSYS_RESETN_MASK          (0x400U)
#define SOCCIU_RST_SW_CSSYS_RESETN_SHIFT         (10U)
/*! CSSYS_RESETN - SW reset for the cssys cs resetn */
#define SOCCIU_RST_SW_CSSYS_RESETN(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_CSSYS_RESETN_SHIFT)) & SOCCIU_RST_SW_CSSYS_RESETN_MASK)

#define SOCCIU_RST_SW_DAP_RESETN_MASK            (0x800U)
#define SOCCIU_RST_SW_DAP_RESETN_SHIFT           (11U)
/*! DAP_RESETN - SW reset for the dap_resetn */
#define SOCCIU_RST_SW_DAP_RESETN(x)              (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_DAP_RESETN_SHIFT)) & SOCCIU_RST_SW_DAP_RESETN_MASK)

#define SOCCIU_RST_SW_WLAN_N_BLE_PORB_DELAY_MASK (0xFF000U)
#define SOCCIU_RST_SW_WLAN_N_BLE_PORB_DELAY_SHIFT (12U)
/*! WLAN_N_BLE_PORB_DELAY - PORB delay for wlan and ble, bootrom can use this feature if needed */
#define SOCCIU_RST_SW_WLAN_N_BLE_PORB_DELAY(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_WLAN_N_BLE_PORB_DELAY_SHIFT)) & SOCCIU_RST_SW_WLAN_N_BLE_PORB_DELAY_MASK)

#define SOCCIU_RST_SW_SOC_ITRC_CHIP_RSTB_EN_MASK (0x100000U)
#define SOCCIU_RST_SW_SOC_ITRC_CHIP_RSTB_EN_SHIFT (20U)
/*! SOC_ITRC_CHIP_RSTB_EN - Default the itrc chip reset can reset SOC, SW can disable this if not required */
#define SOCCIU_RST_SW_SOC_ITRC_CHIP_RSTB_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_SOC_ITRC_CHIP_RSTB_EN_SHIFT)) & SOCCIU_RST_SW_SOC_ITRC_CHIP_RSTB_EN_MASK)

#define SOCCIU_RST_SW_WL_ITRC_CHIP_RSTB_EN_MASK  (0x200000U)
#define SOCCIU_RST_SW_WL_ITRC_CHIP_RSTB_EN_SHIFT (21U)
/*! WL_ITRC_CHIP_RSTB_EN - Default the itrc chip reset can reset WLAN, SW can disable this if not required */
#define SOCCIU_RST_SW_WL_ITRC_CHIP_RSTB_EN(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_WL_ITRC_CHIP_RSTB_EN_SHIFT)) & SOCCIU_RST_SW_WL_ITRC_CHIP_RSTB_EN_MASK)

#define SOCCIU_RST_SW_BLE_ITRC_CHIP_RSTB_EN_MASK (0x400000U)
#define SOCCIU_RST_SW_BLE_ITRC_CHIP_RSTB_EN_SHIFT (22U)
/*! BLE_ITRC_CHIP_RSTB_EN - Default the itrc chip reset can reset BLE, SW can disable this if not required */
#define SOCCIU_RST_SW_BLE_ITRC_CHIP_RSTB_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_BLE_ITRC_CHIP_RSTB_EN_SHIFT)) & SOCCIU_RST_SW_BLE_ITRC_CHIP_RSTB_EN_MASK)

#define SOCCIU_RST_SW_MSC_ITRC_CHIP_RSTB_EN_MASK (0x800000U)
#define SOCCIU_RST_SW_MSC_ITRC_CHIP_RSTB_EN_SHIFT (23U)
/*! MSC_ITRC_CHIP_RSTB_EN - Default the itrc chip reset can reset MSC, SW can disable this if not required */
#define SOCCIU_RST_SW_MSC_ITRC_CHIP_RSTB_EN(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_RST_SW_MSC_ITRC_CHIP_RSTB_EN_SHIFT)) & SOCCIU_RST_SW_MSC_ITRC_CHIP_RSTB_EN_MASK)
/*! @} */

/*! @name CHIP_INFO - Chip Information */
/*! @{ */

#define SOCCIU_CHIP_INFO_REV_NUM_MASK            (0xFFU)
#define SOCCIU_CHIP_INFO_REV_NUM_SHIFT           (0U)
/*! REV_NUM - Chip Revision Number */
#define SOCCIU_CHIP_INFO_REV_NUM(x)              (((uint32_t)(((uint32_t)(x)) << SOCCIU_CHIP_INFO_REV_NUM_SHIFT)) & SOCCIU_CHIP_INFO_REV_NUM_MASK)

#define SOCCIU_CHIP_INFO_ID_MASK                 (0xFF00U)
#define SOCCIU_CHIP_INFO_ID_SHIFT                (8U)
/*! ID - Chip ID */
#define SOCCIU_CHIP_INFO_ID(x)                   (((uint32_t)(((uint32_t)(x)) << SOCCIU_CHIP_INFO_ID_SHIFT)) & SOCCIU_CHIP_INFO_ID_MASK)
/*! @} */

/*! @name MCI_POWER_MODE_STATUS - MCI POWER MODE Status */
/*! @{ */

#define SOCCIU_MCI_POWER_MODE_STATUS_MCI_SLP_STATE_MASK (0x7U)
#define SOCCIU_MCI_POWER_MODE_STATUS_MCI_SLP_STATE_SHIFT (0U)
/*! MCI_SLP_STATE - MCI Power Mode Status */
#define SOCCIU_MCI_POWER_MODE_STATUS_MCI_SLP_STATE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_POWER_MODE_STATUS_MCI_SLP_STATE_SHIFT)) & SOCCIU_MCI_POWER_MODE_STATUS_MCI_SLP_STATE_MASK)

#define SOCCIU_MCI_POWER_MODE_STATUS_MCI_BIST_DONE_MASK (0x8U)
#define SOCCIU_MCI_POWER_MODE_STATUS_MCI_BIST_DONE_SHIFT (3U)
/*! MCI_BIST_DONE - MCI g2bist done status */
#define SOCCIU_MCI_POWER_MODE_STATUS_MCI_BIST_DONE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_POWER_MODE_STATUS_MCI_BIST_DONE_SHIFT)) & SOCCIU_MCI_POWER_MODE_STATUS_MCI_BIST_DONE_MASK)

#define SOCCIU_MCI_POWER_MODE_STATUS_CM33_RESET_N_MASK (0x10U)
#define SOCCIU_MCI_POWER_MODE_STATUS_CM33_RESET_N_SHIFT (4U)
/*! CM33_RESET_N - CM33 Reset status. Active low. */
#define SOCCIU_MCI_POWER_MODE_STATUS_CM33_RESET_N(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_POWER_MODE_STATUS_CM33_RESET_N_SHIFT)) & SOCCIU_MCI_POWER_MODE_STATUS_CM33_RESET_N_MASK)

#define SOCCIU_MCI_POWER_MODE_STATUS_CPU3_FW_READY_MASK (0x20U)
#define SOCCIU_MCI_POWER_MODE_STATUS_CPU3_FW_READY_SHIFT (5U)
/*! CPU3_FW_READY - cpu3 FW sets this bit after cpu1 FW initialization is done. */
#define SOCCIU_MCI_POWER_MODE_STATUS_CPU3_FW_READY(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_POWER_MODE_STATUS_CPU3_FW_READY_SHIFT)) & SOCCIU_MCI_POWER_MODE_STATUS_CPU3_FW_READY_MASK)
/*! @} */

/*! @name PSW_VD2_RDY1 - Power Switch VD2_RDY Status */
/*! @{ */

#define SOCCIU_PSW_VD2_RDY1_PSW_STATUS_MASK      (0xFFFFFFFFU)
#define SOCCIU_PSW_VD2_RDY1_PSW_STATUS_SHIFT     (0U)
/*! PSW_STATUS - VD2_RDY Status of following Power Switches: */
#define SOCCIU_PSW_VD2_RDY1_PSW_STATUS(x)        (((uint32_t)(((uint32_t)(x)) << SOCCIU_PSW_VD2_RDY1_PSW_STATUS_SHIFT)) & SOCCIU_PSW_VD2_RDY1_PSW_STATUS_MASK)
/*! @} */

/*! @name WLAN_POWER_STATUS - WLAN POWER Status */
/*! @{ */

#define SOCCIU_WLAN_POWER_STATUS_WLRET_PSW_PD_MASK (0x1U)
#define SOCCIU_WLAN_POWER_STATUS_WLRET_PSW_PD_SHIFT (0U)
/*! WLRET_PSW_PD - WLAN-RETENTION domain power-switch control status: */
#define SOCCIU_WLAN_POWER_STATUS_WLRET_PSW_PD(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_WLAN_POWER_STATUS_WLRET_PSW_PD_SHIFT)) & SOCCIU_WLAN_POWER_STATUS_WLRET_PSW_PD_MASK)

#define SOCCIU_WLAN_POWER_STATUS_WLGATED_PSW_PD_MASK (0x2U)
#define SOCCIU_WLAN_POWER_STATUS_WLGATED_PSW_PD_SHIFT (1U)
/*! WLGATED_PSW_PD - WLAN-GATED domain power-switch control status: */
#define SOCCIU_WLAN_POWER_STATUS_WLGATED_PSW_PD(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_WLAN_POWER_STATUS_WLGATED_PSW_PD_SHIFT)) & SOCCIU_WLAN_POWER_STATUS_WLGATED_PSW_PD_MASK)

#define SOCCIU_WLAN_POWER_STATUS_CPU1_SLEEP_MASK (0x4U)
#define SOCCIU_WLAN_POWER_STATUS_CPU1_SLEEP_SHIFT (2U)
/*! CPU1_SLEEP - CPU1 cp15 sleep status */
#define SOCCIU_WLAN_POWER_STATUS_CPU1_SLEEP(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_WLAN_POWER_STATUS_CPU1_SLEEP_SHIFT)) & SOCCIU_WLAN_POWER_STATUS_CPU1_SLEEP_MASK)

#define SOCCIU_WLAN_POWER_STATUS_CPU1_BIST_DONE_MASK (0x8U)
#define SOCCIU_WLAN_POWER_STATUS_CPU1_BIST_DONE_SHIFT (3U)
/*! CPU1_BIST_DONE - CPU1 memories g2bist done status */
#define SOCCIU_WLAN_POWER_STATUS_CPU1_BIST_DONE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_WLAN_POWER_STATUS_CPU1_BIST_DONE_SHIFT)) & SOCCIU_WLAN_POWER_STATUS_CPU1_BIST_DONE_MASK)

#define SOCCIU_WLAN_POWER_STATUS_CPU1_RESET_N_MASK (0x10U)
#define SOCCIU_WLAN_POWER_STATUS_CPU1_RESET_N_SHIFT (4U)
/*! CPU1_RESET_N - CPU1 Reset status. Active low. */
#define SOCCIU_WLAN_POWER_STATUS_CPU1_RESET_N(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_WLAN_POWER_STATUS_CPU1_RESET_N_SHIFT)) & SOCCIU_WLAN_POWER_STATUS_CPU1_RESET_N_MASK)

#define SOCCIU_WLAN_POWER_STATUS_CPU1_FW_READY_MASK (0x20U)
#define SOCCIU_WLAN_POWER_STATUS_CPU1_FW_READY_SHIFT (5U)
/*! CPU1_FW_READY - cpu1 FW sets this bit after cpu1 FW initialization is done. */
#define SOCCIU_WLAN_POWER_STATUS_CPU1_FW_READY(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_WLAN_POWER_STATUS_CPU1_FW_READY_SHIFT)) & SOCCIU_WLAN_POWER_STATUS_CPU1_FW_READY_MASK)
/*! @} */

/*! @name BLE_POWER_STATUS - BLE POWER Status */
/*! @{ */

#define SOCCIU_BLE_POWER_STATUS_BLERET_PSW_PD_MASK (0x1U)
#define SOCCIU_BLE_POWER_STATUS_BLERET_PSW_PD_SHIFT (0U)
/*! BLERET_PSW_PD - BLE-RETENTION domain power-switch control status: */
#define SOCCIU_BLE_POWER_STATUS_BLERET_PSW_PD(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_BLE_POWER_STATUS_BLERET_PSW_PD_SHIFT)) & SOCCIU_BLE_POWER_STATUS_BLERET_PSW_PD_MASK)

#define SOCCIU_BLE_POWER_STATUS_BLEGATED_PSW_PD_MASK (0x2U)
#define SOCCIU_BLE_POWER_STATUS_BLEGATED_PSW_PD_SHIFT (1U)
/*! BLEGATED_PSW_PD - BLE-GATED domain power-switch control status: */
#define SOCCIU_BLE_POWER_STATUS_BLEGATED_PSW_PD(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_BLE_POWER_STATUS_BLEGATED_PSW_PD_SHIFT)) & SOCCIU_BLE_POWER_STATUS_BLEGATED_PSW_PD_MASK)

#define SOCCIU_BLE_POWER_STATUS_CPU2_SLEEP_MASK  (0x4U)
#define SOCCIU_BLE_POWER_STATUS_CPU2_SLEEP_SHIFT (2U)
/*! CPU2_SLEEP - CPU2 cp15 sleep status */
#define SOCCIU_BLE_POWER_STATUS_CPU2_SLEEP(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_BLE_POWER_STATUS_CPU2_SLEEP_SHIFT)) & SOCCIU_BLE_POWER_STATUS_CPU2_SLEEP_MASK)

#define SOCCIU_BLE_POWER_STATUS_CPU2_BIST_DONE_MASK (0x8U)
#define SOCCIU_BLE_POWER_STATUS_CPU2_BIST_DONE_SHIFT (3U)
/*! CPU2_BIST_DONE - CPU2 memories g2bist done status */
#define SOCCIU_BLE_POWER_STATUS_CPU2_BIST_DONE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_BLE_POWER_STATUS_CPU2_BIST_DONE_SHIFT)) & SOCCIU_BLE_POWER_STATUS_CPU2_BIST_DONE_MASK)

#define SOCCIU_BLE_POWER_STATUS_CPU2_RESET_N_MASK (0x10U)
#define SOCCIU_BLE_POWER_STATUS_CPU2_RESET_N_SHIFT (4U)
/*! CPU2_RESET_N - CPU2 Reset status. Active low. */
#define SOCCIU_BLE_POWER_STATUS_CPU2_RESET_N(x)  (((uint32_t)(((uint32_t)(x)) << SOCCIU_BLE_POWER_STATUS_CPU2_RESET_N_SHIFT)) & SOCCIU_BLE_POWER_STATUS_CPU2_RESET_N_MASK)

#define SOCCIU_BLE_POWER_STATUS_CPU2_FW_READY_MASK (0x20U)
#define SOCCIU_BLE_POWER_STATUS_CPU2_FW_READY_SHIFT (5U)
/*! CPU2_FW_READY - cpu2 FW sets this bit after cpu2 FW initialization is done. */
#define SOCCIU_BLE_POWER_STATUS_CPU2_FW_READY(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_BLE_POWER_STATUS_CPU2_FW_READY_SHIFT)) & SOCCIU_BLE_POWER_STATUS_CPU2_FW_READY_MASK)
/*! @} */

/*! @name PAD_VREG_VSENSOR_CTRL - Vsensor and Vreg Pad Control */
/*! @{ */

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_ENB_MASK (0x1U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_ENB_SHIFT (0U)
/*! VIO_REG1_ENB - VIO_X1 Pad Regulator */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_ENB(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_ENB_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_ENB_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_CTRL_EN_MASK (0x2U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_CTRL_EN_SHIFT (1U)
/*! VIO_REG1_CTRL_EN - VIO_X1 Pad Regulator control */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_CTRL_EN_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG1_CTRL_EN_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_ENB_MASK (0x4U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_ENB_SHIFT (2U)
/*! VIO_REG4_ENB - VIO_4 Pad Regulator */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_ENB(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_ENB_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_ENB_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_CTRL_EN_MASK (0x8U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_CTRL_EN_SHIFT (3U)
/*! VIO_REG4_CTRL_EN - VIO_X4 Pad Regulator control */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_CTRL_EN_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG4_CTRL_EN_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_ENB_MASK (0x10U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_ENB_SHIFT (4U)
/*! VIO_REG5_ENB - VIO_5 Pad Regulator */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_ENB(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_ENB_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_ENB_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_CTRL_EN_MASK (0x20U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_CTRL_EN_SHIFT (5U)
/*! VIO_REG5_CTRL_EN - VIO_X5 Pad Regulator control */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_CTRL_EN_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG5_CTRL_EN_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_ENB_MASK (0x40U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_ENB_SHIFT (6U)
/*! VIO_REG6_ENB - VIO_6 Pad Regulator */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_ENB(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_ENB_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_ENB_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_CTRL_EN_MASK (0x80U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_CTRL_EN_SHIFT (7U)
/*! VIO_REG6_CTRL_EN - VIO_X6 Pad Regulator control */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_CTRL_EN_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_REG6_CTRL_EN_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK (0x1000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT (12U)
/*! VSENSOR_BYPASS - Active High Enable Signal for Bypass Mode */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK (0x2000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT (13U)
/*! VSENSOR_V18EN_12_IN - Bypass Value when Vsensor_Bypass Bit Set */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK (0x4000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT (14U)
/*! VSENSOR_DISABLE_12 - Vsensor X1 disable */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK (0x8000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT (15U)
/*! VSENSOR_CLK_12 - Vsensor Clock */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK (0x10000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT (16U)
/*! VSENSOR_TE - Vsensor Test Enable */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK (0xE0000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT (17U)
/*! VSENSOR_TEST - Vsensor Test Point Mux Selection */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TEST(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK (0x100000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT (20U)
/*! VSENSOR_VTHRESH - Vsensor Detection Threshold */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_V25EN_CORE_MASK (0x200000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT (21U)
/*! V25EN_CORE - V25EN_CORE */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_V25EN_CORE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_V25EN_CORE_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X0_VSENSOR_DETECT_MASK (0x400000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X0_VSENSOR_DETECT_SHIFT (22U)
/*! VIO_X0_VSENSOR_DETECT - VIO_X0_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X0_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X0_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X0_VSENSOR_DETECT_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X1_VSENSOR_DETECT_MASK (0x800000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X1_VSENSOR_DETECT_SHIFT (23U)
/*! VIO_X1_VSENSOR_DETECT - VIO_X1_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X1_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X1_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X1_VSENSOR_DETECT_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X2_VSENSOR_DETECT_MASK (0x1000000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X2_VSENSOR_DETECT_SHIFT (24U)
/*! VIO_X2_VSENSOR_DETECT - VIO_X2_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X2_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X2_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X2_VSENSOR_DETECT_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X3_VSENSOR_DETECT_MASK (0x2000000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X3_VSENSOR_DETECT_SHIFT (25U)
/*! VIO_X3_VSENSOR_DETECT - VIO_X3_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X3_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X3_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X3_VSENSOR_DETECT_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X4_VSENSOR_DETECT_MASK (0x4000000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X4_VSENSOR_DETECT_SHIFT (26U)
/*! VIO_X4_VSENSOR_DETECT - VIO_X4_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X4_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X4_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X4_VSENSOR_DETECT_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X5_VSENSOR_DETECT_MASK (0x8000000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X5_VSENSOR_DETECT_SHIFT (27U)
/*! VIO_X5_VSENSOR_DETECT - VIO_X5_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X5_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X5_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X5_VSENSOR_DETECT_MASK)

#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X6_VSENSOR_DETECT_MASK (0x10000000U)
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X6_VSENSOR_DETECT_SHIFT (28U)
/*! VIO_X6_VSENSOR_DETECT - VIO_X6_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X6_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X6_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_VREG_VSENSOR_CTRL_VIO_X6_VSENSOR_DETECT_MASK)
/*! @} */

/*! @name PAD_RF_VREG_VSENSOR_CTRL - RF Vsensor and Vreg Pad Control */
/*! @{ */

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG_ENB_MASK (0x1U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG_ENB_SHIFT (0U)
/*! VIO_REG_ENB - VIO_RF Pad Regulator */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG_ENB(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG_ENB_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG_ENB_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG0_CTRL_EN_MASK (0x2U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG0_CTRL_EN_SHIFT (1U)
/*! VIO_REG0_CTRL_EN - VIO reg0 control enable Function */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG0_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG0_CTRL_EN_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VIO_REG0_CTRL_EN_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK (0x10U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT (4U)
/*! VSENSOR_BYPASS - Active High Enable Signal for Bypass Mode */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK (0x20U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT (5U)
/*! VSENSOR_V18EN_12_IN - Bypass Value when Vsensor_Bypass Bit Set */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK (0x40U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT (6U)
/*! VSENSOR_DISABLE_12 - Vsensor RF disable */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK (0x80U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT (7U)
/*! VSENSOR_CLK_12 - Vsensor RF Clock */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_CLK_12(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK (0x100U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT (8U)
/*! VSENSOR_TE - Vsensor RF Test Enable */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK (0xE00U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT (9U)
/*! VSENSOR_TEST - Vsensor RF Test Point Mux Selection */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TEST(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK (0x1000U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT (12U)
/*! VSENSOR_VTHRESH - Vsensor RF Detection Threshold */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_V25EN_CORE_MASK (0x2000U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT (13U)
/*! V25EN_CORE - V25EN_CORE */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_V25EN_CORE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_V25EN_CORE_MASK)

#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DETECT_MASK (0x4000U)
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DETECT_SHIFT (14U)
/*! VSENSOR_DETECT - VIO_RF_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_RF_VREG_VSENSOR_CTRL_VSENSOR_DETECT_MASK)
/*! @} */

/*! @name PAD_SD_VREG_VSENSOR_CTRL - SD Vsensor and Vreg Pad Control */
/*! @{ */

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_ENB_MASK (0x1U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_ENB_SHIFT (0U)
/*! VIO_REG_ENB - VIO_SD Pad Regulator */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_ENB(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_ENB_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_ENB_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_MASK (0x2U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_SHIFT (1U)
/*! VIO_REG_CTRL_EN - VIO reg control enable function */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VIO_REG_CTRL_EN_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK (0x10U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT (4U)
/*! VSENSOR_BYPASS - Active High Enable Signal for Bypass Mode */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_BYPASS_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK (0x20U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT (5U)
/*! VSENSOR_V18EN_12_IN - Bypass Value when Vsensor_Bypass Bit Set */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_V18EN_12_IN_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK (0x40U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT (6U)
/*! VSENSOR_DISABLE_12 - Vsensor SD disable */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DISABLE_12_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK (0x80U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT (7U)
/*! VSENSOR_CLK_12 - Vsensor SD Clock */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_CLK_12_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK (0x100U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT (8U)
/*! VSENSOR_TE - Vsensor SD Test Enable */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TE_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TE_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK (0xE00U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT (9U)
/*! VSENSOR_TEST - Vsensor SD Test Point Mux Selection */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TEST(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TEST_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_TEST_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK (0x1000U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT (12U)
/*! VSENSOR_VTHRESH - Vsensor SD Detection Threshold */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_VTHRESH_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_V25EN_CORE_MASK (0x2000U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT (13U)
/*! V25EN_CORE - V25EN_CORE */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_V25EN_CORE(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_V25EN_CORE_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_V25EN_CORE_MASK)

#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DETECT_MASK (0x4000U)
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DETECT_SHIFT (14U)
/*! VSENSOR_DETECT - VIO_SD_Vsensor_Detect_V18 Status */
#define SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DETECT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DETECT_SHIFT)) & SOCCIU_PAD_SD_VREG_VSENSOR_CTRL_VSENSOR_DETECT_MASK)
/*! @} */

/*! @name PAD_ECO_CTRL - Pad ECO Control */
/*! @{ */

#define SOCCIU_PAD_ECO_CTRL_ECO_BITS_MASK        (0xFFFFFFFFU)
#define SOCCIU_PAD_ECO_CTRL_ECO_BITS_SHIFT       (0U)
/*! ECO_BITS - Reserved for ECOs */
#define SOCCIU_PAD_ECO_CTRL_ECO_BITS(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_ECO_CTRL_ECO_BITS_SHIFT)) & SOCCIU_PAD_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name TST_TSTBUS_CTRL1 - Testbus Mux Control1 */
/*! @{ */

#define SOCCIU_TST_TSTBUS_CTRL1_SOC_PERI_TB_SEL_MASK (0x70U)
#define SOCCIU_TST_TSTBUS_CTRL1_SOC_PERI_TB_SEL_SHIFT (4U)
/*! SOC_PERI_TB_SEL - Select for soc peri testbus: */
#define SOCCIU_TST_TSTBUS_CTRL1_SOC_PERI_TB_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_TSTBUS_CTRL1_SOC_PERI_TB_SEL_SHIFT)) & SOCCIU_TST_TSTBUS_CTRL1_SOC_PERI_TB_SEL_MASK)
/*! @} */

/*! @name TST_TSTBUS_CTRL2 - Testbus Mux Control2 */
/*! @{ */

#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_SEL_MASK (0xF000U)
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_SEL_SHIFT (12U)
/*! CLK_OUT_SEL - PAGE 0 */
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_SEL(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_SEL_SHIFT)) & SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_SEL_MASK)

#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_PAGE_SEL_MASK (0x30000U)
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_PAGE_SEL_SHIFT (16U)
/*! CLK_OUT_PAGE_SEL - Clock out test page sel */
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_PAGE_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_PAGE_SEL_SHIFT)) & SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_PAGE_SEL_MASK)

#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_MASK  (0x40000U)
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_SHIFT (18U)
/*! CLK_OUT_EN - Clock Out Enable */
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN(x)    (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_SHIFT)) & SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_MASK)

#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_ALT_MASK (0x80000U)
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_ALT_SHIFT (19U)
/*! CLK_OUT_EN_ALT - Clock Out Enable */
#define SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_ALT(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_ALT_SHIFT)) & SOCCIU_TST_TSTBUS_CTRL2_CLK_OUT_EN_ALT_MASK)

#define SOCCIU_TST_TSTBUS_CTRL2_POR_MON_SEL_MASK (0x100000U)
#define SOCCIU_TST_TSTBUS_CTRL2_POR_MON_SEL_SHIFT (20U)
/*! POR_MON_SEL - POR MON Testbus Select (to support more power domain busses) */
#define SOCCIU_TST_TSTBUS_CTRL2_POR_MON_SEL(x)   (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_TSTBUS_CTRL2_POR_MON_SEL_SHIFT)) & SOCCIU_TST_TSTBUS_CTRL2_POR_MON_SEL_MASK)
/*! @} */

/*! @name TST_CTRL - Test Control */
/*! @{ */

#define SOCCIU_TST_CTRL_RBIST_DONE_MASK          (0xFU)
#define SOCCIU_TST_CTRL_RBIST_DONE_SHIFT         (0U)
/*! RBIST_DONE - [3]: HMAC g2bist finish */
#define SOCCIU_TST_CTRL_RBIST_DONE(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_CTRL_RBIST_DONE_SHIFT)) & SOCCIU_TST_CTRL_RBIST_DONE_MASK)
/*! @} */

/*! @name TST_ECO_CTRL - Test ECO Control */
/*! @{ */

#define SOCCIU_TST_ECO_CTRL_ECO_BITS_MASK        (0xFFFFFFFFU)
#define SOCCIU_TST_ECO_CTRL_ECO_BITS_SHIFT       (0U)
/*! ECO_BITS - Reserved for ECOs */
#define SOCCIU_TST_ECO_CTRL_ECO_BITS(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_TST_ECO_CTRL_ECO_BITS_SHIFT)) & SOCCIU_TST_ECO_CTRL_ECO_BITS_MASK)
/*! @} */

/*! @name DRO_CTRL - DRO Control */
/*! @{ */

#define SOCCIU_DRO_CTRL_DRO_EN_MASK              (0x1U)
#define SOCCIU_DRO_CTRL_DRO_EN_SHIFT             (0U)
/*! DRO_EN - DRO Counter Enable */
#define SOCCIU_DRO_CTRL_DRO_EN(x)                (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_CTRL_DRO_EN_SHIFT)) & SOCCIU_DRO_CTRL_DRO_EN_MASK)

#define SOCCIU_DRO_CTRL_DRO_CLK_GATE_EN_MASK     (0x2U)
#define SOCCIU_DRO_CTRL_DRO_CLK_GATE_EN_SHIFT    (1U)
/*! DRO_CLK_GATE_EN - DRO Clock Gate Enable */
#define SOCCIU_DRO_CTRL_DRO_CLK_GATE_EN(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_CTRL_DRO_CLK_GATE_EN_SHIFT)) & SOCCIU_DRO_CTRL_DRO_CLK_GATE_EN_MASK)

#define SOCCIU_DRO_CTRL_DRO_CNT_STATUS_MASK      (0x4U)
#define SOCCIU_DRO_CTRL_DRO_CNT_STATUS_SHIFT     (2U)
/*! DRO_CNT_STATUS - 0: dro counter is stopped; 1: dro counter is in process */
#define SOCCIU_DRO_CTRL_DRO_CNT_STATUS(x)        (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_CTRL_DRO_CNT_STATUS_SHIFT)) & SOCCIU_DRO_CTRL_DRO_CNT_STATUS_MASK)

#define SOCCIU_DRO_CTRL_DRO_COUNT_LIMIT_MASK     (0xFFFFFFF0U)
#define SOCCIU_DRO_CTRL_DRO_COUNT_LIMIT_SHIFT    (4U)
/*! DRO_COUNT_LIMIT - DRO Count Value */
#define SOCCIU_DRO_CTRL_DRO_COUNT_LIMIT(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_CTRL_DRO_COUNT_LIMIT_SHIFT)) & SOCCIU_DRO_CTRL_DRO_COUNT_LIMIT_MASK)
/*! @} */

/*! @name DRO_1_2_CNT - DRO1 and DRO2 Counter Read back */
/*! @{ */

#define SOCCIU_DRO_1_2_CNT_DRO2_CNT_MASK         (0xFFFFU)
#define SOCCIU_DRO_1_2_CNT_DRO2_CNT_SHIFT        (0U)
/*! DRO2_CNT - DRO2 Count */
#define SOCCIU_DRO_1_2_CNT_DRO2_CNT(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_1_2_CNT_DRO2_CNT_SHIFT)) & SOCCIU_DRO_1_2_CNT_DRO2_CNT_MASK)

#define SOCCIU_DRO_1_2_CNT_DRO1_CNT_MASK         (0xFFFF0000U)
#define SOCCIU_DRO_1_2_CNT_DRO1_CNT_SHIFT        (16U)
/*! DRO1_CNT - DRO1 Count */
#define SOCCIU_DRO_1_2_CNT_DRO1_CNT(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_1_2_CNT_DRO1_CNT_SHIFT)) & SOCCIU_DRO_1_2_CNT_DRO1_CNT_MASK)
/*! @} */

/*! @name DRO_3_CNT - DRO3 Counter Read back */
/*! @{ */

#define SOCCIU_DRO_3_CNT_DRO3_CNT_MASK           (0xFFFFU)
#define SOCCIU_DRO_3_CNT_DRO3_CNT_SHIFT          (0U)
/*! DRO3_CNT - DRO3 Count */
#define SOCCIU_DRO_3_CNT_DRO3_CNT(x)             (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_3_CNT_DRO3_CNT_SHIFT)) & SOCCIU_DRO_3_CNT_DRO3_CNT_MASK)
/*! @} */

/*! @name DRO_PAR_SEL - DRO Parallel Counter Selection */
/*! @{ */

#define SOCCIU_DRO_PAR_SEL_DRO1_PAR_SEL_MASK     (0x3U)
#define SOCCIU_DRO_PAR_SEL_DRO1_PAR_SEL_SHIFT    (0U)
/*! DRO1_PAR_SEL - DRO1 counter selection */
#define SOCCIU_DRO_PAR_SEL_DRO1_PAR_SEL(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_PAR_SEL_DRO1_PAR_SEL_SHIFT)) & SOCCIU_DRO_PAR_SEL_DRO1_PAR_SEL_MASK)

#define SOCCIU_DRO_PAR_SEL_DRO2_PAR_SEL_MASK     (0xCU)
#define SOCCIU_DRO_PAR_SEL_DRO2_PAR_SEL_SHIFT    (2U)
/*! DRO2_PAR_SEL - DRO2 counter selection */
#define SOCCIU_DRO_PAR_SEL_DRO2_PAR_SEL(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_PAR_SEL_DRO2_PAR_SEL_SHIFT)) & SOCCIU_DRO_PAR_SEL_DRO2_PAR_SEL_MASK)

#define SOCCIU_DRO_PAR_SEL_DRO3_PAR_SEL_MASK     (0x30U)
#define SOCCIU_DRO_PAR_SEL_DRO3_PAR_SEL_SHIFT    (4U)
/*! DRO3_PAR_SEL - DRO3 counter selection */
#define SOCCIU_DRO_PAR_SEL_DRO3_PAR_SEL(x)       (((uint32_t)(((uint32_t)(x)) << SOCCIU_DRO_PAR_SEL_DRO3_PAR_SEL_SHIFT)) & SOCCIU_DRO_PAR_SEL_DRO3_PAR_SEL_MASK)
/*! @} */

/*! @name CLK_SOCCLK_CTRL - SOC Clock Control */
/*! @{ */

#define SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_PCLK_DIV_SEL_MASK (0xFU)
#define SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_PCLK_DIV_SEL_SHIFT (0U)
/*! SOC_TOP_AHB2APB_PCLK_DIV_SEL - SOC_TOP AHB2APB PCLK Divider Select */
#define SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_PCLK_DIV_SEL(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_PCLK_DIV_SEL_SHIFT)) & SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_PCLK_DIV_SEL_MASK)

#define SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_WAIT_CYCLES_MASK (0xF0U)
#define SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_WAIT_CYCLES_SHIFT (4U)
/*! SOC_TOP_AHB2APB_WAIT_CYCLES - SOC_TOP AHB2APB Wait Cycles between each APB transaction */
#define SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_WAIT_CYCLES(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_WAIT_CYCLES_SHIFT)) & SOCCIU_CLK_SOCCLK_CTRL_SOC_TOP_AHB2APB_WAIT_CYCLES_MASK)
/*! @} */

/*! @name PAD_SLP_PU_PD_DIS0 - Pad Sleep Pullup and Pulldown Disable1 */
/*! @{ */

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO0_PU_PD_DIS_MASK (0x1U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO0_PU_PD_DIS_SHIFT (0U)
/*! GPIO0_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[0] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO0_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO0_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO0_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO1_PU_PD_DIS_MASK (0x2U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO1_PU_PD_DIS_SHIFT (1U)
/*! GPIO1_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[1] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO1_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO1_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO1_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO2_PU_PD_DIS_MASK (0x4U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO2_PU_PD_DIS_SHIFT (2U)
/*! GPIO2_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[2] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO2_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO2_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO2_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO3_PU_PD_DIS_MASK (0x8U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO3_PU_PD_DIS_SHIFT (3U)
/*! GPIO3_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[3] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO3_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO3_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO3_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO4_PU_PD_DIS_MASK (0x10U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO4_PU_PD_DIS_SHIFT (4U)
/*! GPIO4_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[4] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO4_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO4_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO4_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO5_PU_PD_DIS_MASK (0x20U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO5_PU_PD_DIS_SHIFT (5U)
/*! GPIO5_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[5] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO5_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO5_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO5_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO6_PU_PD_DIS_MASK (0x40U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO6_PU_PD_DIS_SHIFT (6U)
/*! GPIO6_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[6] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO6_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO6_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO6_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO7_PU_PD_DIS_MASK (0x80U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO7_PU_PD_DIS_SHIFT (7U)
/*! GPIO7_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[7] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO7_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO7_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO7_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO8_PU_PD_DIS_MASK (0x100U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO8_PU_PD_DIS_SHIFT (8U)
/*! GPIO8_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[8] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO8_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO8_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO8_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO9_PU_PD_DIS_MASK (0x200U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO9_PU_PD_DIS_SHIFT (9U)
/*! GPIO9_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[9] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO9_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO9_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO9_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO10_PU_PD_DIS_MASK (0x400U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO10_PU_PD_DIS_SHIFT (10U)
/*! GPIO10_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[10] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO10_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO10_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO10_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO11_PU_PD_DIS_MASK (0x800U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO11_PU_PD_DIS_SHIFT (11U)
/*! GPIO11_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[11] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO11_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO11_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO11_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO12_PU_PD_DIS_MASK (0x1000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO12_PU_PD_DIS_SHIFT (12U)
/*! GPIO12_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[12] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO12_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO12_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO12_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO13_PU_PD_DIS_MASK (0x2000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO13_PU_PD_DIS_SHIFT (13U)
/*! GPIO13_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[13] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO13_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO13_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO13_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO14_PU_PD_DIS_MASK (0x4000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO14_PU_PD_DIS_SHIFT (14U)
/*! GPIO14_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[14] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO14_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO14_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO14_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO15_PU_PD_DIS_MASK (0x8000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO15_PU_PD_DIS_SHIFT (15U)
/*! GPIO15_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[15] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO15_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO15_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO15_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO16_PU_PD_DIS_MASK (0x10000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO16_PU_PD_DIS_SHIFT (16U)
/*! GPIO16_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[16] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO16_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO16_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO16_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO17_PU_PD_DIS_MASK (0x20000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO17_PU_PD_DIS_SHIFT (17U)
/*! GPIO17_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[17] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO17_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO17_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO17_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO18_PU_PD_DIS_MASK (0x40000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO18_PU_PD_DIS_SHIFT (18U)
/*! GPIO18_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[18] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO18_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO18_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO18_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO19_PU_PD_DIS_MASK (0x80000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO19_PU_PD_DIS_SHIFT (19U)
/*! GPIO19_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[19] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO19_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO19_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO19_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO20_PU_PD_DIS_MASK (0x100000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO20_PU_PD_DIS_SHIFT (20U)
/*! GPIO20_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[20] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO20_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO20_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO20_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO21_PU_PD_DIS_MASK (0x200000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO21_PU_PD_DIS_SHIFT (21U)
/*! GPIO21_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[21] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO21_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO21_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO21_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO28_PU_PD_DIS_MASK (0x10000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO28_PU_PD_DIS_SHIFT (28U)
/*! GPIO28_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[28] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO28_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO28_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO28_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO29_PU_PD_DIS_MASK (0x20000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO29_PU_PD_DIS_SHIFT (29U)
/*! GPIO29_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[29] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO29_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO29_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO29_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO30_PU_PD_DIS_MASK (0x40000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO30_PU_PD_DIS_SHIFT (30U)
/*! GPIO30_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[30] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO30_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO30_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO30_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO31_PU_PD_DIS_MASK (0x80000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO31_PU_PD_DIS_SHIFT (31U)
/*! GPIO31_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[31] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO31_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO31_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS0_GPIO31_PU_PD_DIS_MASK)
/*! @} */

/*! @name PAD_SLP_PU_PD_DIS1 - Pad Sleep Pullup and Pulldown Disable2 */
/*! @{ */

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO32_PU_PD_DIS_MASK (0x1U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO32_PU_PD_DIS_SHIFT (0U)
/*! GPIO32_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[32] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO32_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO32_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO32_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO33_PU_PD_DIS_MASK (0x2U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO33_PU_PD_DIS_SHIFT (1U)
/*! GPIO33_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[33] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO33_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO33_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO33_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO34_PU_PD_DIS_MASK (0x4U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO34_PU_PD_DIS_SHIFT (2U)
/*! GPIO34_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[34] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO34_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO34_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO34_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO35_PU_PD_DIS_MASK (0x8U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO35_PU_PD_DIS_SHIFT (3U)
/*! GPIO35_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[35] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO35_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO35_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO35_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO36_PU_PD_DIS_MASK (0x10U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO36_PU_PD_DIS_SHIFT (4U)
/*! GPIO36_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[36] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO36_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO36_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO36_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO37_PU_PD_DIS_MASK (0x20U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO37_PU_PD_DIS_SHIFT (5U)
/*! GPIO37_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[37] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO37_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO37_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO37_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO38_PU_PD_DIS_MASK (0x40U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO38_PU_PD_DIS_SHIFT (6U)
/*! GPIO38_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[38] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO38_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO38_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO38_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO39_PU_PD_DIS_MASK (0x80U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO39_PU_PD_DIS_SHIFT (7U)
/*! GPIO39_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[39] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO39_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO39_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO39_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO40_PU_PD_DIS_MASK (0x100U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO40_PU_PD_DIS_SHIFT (8U)
/*! GPIO40_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[40] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO40_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO40_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO40_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO41_PU_PD_DIS_MASK (0x200U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO41_PU_PD_DIS_SHIFT (9U)
/*! GPIO41_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[41] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO41_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO41_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO41_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO42_PU_PD_DIS_MASK (0x400U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO42_PU_PD_DIS_SHIFT (10U)
/*! GPIO42_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[42] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO42_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO42_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO42_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO43_PU_PD_DIS_MASK (0x800U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO43_PU_PD_DIS_SHIFT (11U)
/*! GPIO43_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[43] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO43_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO43_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO43_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO44_PU_PD_DIS_MASK (0x1000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO44_PU_PD_DIS_SHIFT (12U)
/*! GPIO44_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[44] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO44_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO44_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO44_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO45_PU_PD_DIS_MASK (0x2000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO45_PU_PD_DIS_SHIFT (13U)
/*! GPIO45_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[45] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO45_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO45_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO45_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO46_PU_PD_DIS_MASK (0x4000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO46_PU_PD_DIS_SHIFT (14U)
/*! GPIO46_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[46] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO46_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO46_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO46_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO47_PU_PD_DIS_MASK (0x8000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO47_PU_PD_DIS_SHIFT (15U)
/*! GPIO47_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[47] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO47_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO47_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO47_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO48_PU_PD_DIS_MASK (0x10000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO48_PU_PD_DIS_SHIFT (16U)
/*! GPIO48_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[48] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO48_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO48_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO48_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO49_PU_PD_DIS_MASK (0x20000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO49_PU_PD_DIS_SHIFT (17U)
/*! GPIO49_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[49] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO49_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO49_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO49_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO50_PU_PD_DIS_MASK (0x40000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO50_PU_PD_DIS_SHIFT (18U)
/*! GPIO50_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[50] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO50_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO50_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO50_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO51_PU_PD_DIS_MASK (0x80000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO51_PU_PD_DIS_SHIFT (19U)
/*! GPIO51_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[51] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO51_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO51_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO51_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO52_PU_PD_DIS_MASK (0x100000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO52_PU_PD_DIS_SHIFT (20U)
/*! GPIO52_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[52] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO52_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO52_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO52_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO53_PU_PD_DIS_MASK (0x200000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO53_PU_PD_DIS_SHIFT (21U)
/*! GPIO53_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[53] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO53_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO53_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO53_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO54_PU_PD_DIS_MASK (0x400000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO54_PU_PD_DIS_SHIFT (22U)
/*! GPIO54_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[54] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO54_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO54_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO54_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO55_PU_PD_DIS_MASK (0x800000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO55_PU_PD_DIS_SHIFT (23U)
/*! GPIO55_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[55] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO55_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO55_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO55_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO56_PU_PD_DIS_MASK (0x1000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO56_PU_PD_DIS_SHIFT (24U)
/*! GPIO56_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[56] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO56_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO56_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO56_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO57_PU_PD_DIS_MASK (0x2000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO57_PU_PD_DIS_SHIFT (25U)
/*! GPIO57_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[57] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO57_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO57_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO57_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO58_PU_PD_DIS_MASK (0x4000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO58_PU_PD_DIS_SHIFT (26U)
/*! GPIO58_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[58] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO58_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO58_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO58_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO59_PU_PD_DIS_MASK (0x8000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO59_PU_PD_DIS_SHIFT (27U)
/*! GPIO59_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[59] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO59_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO59_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO59_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO60_PU_PD_DIS_MASK (0x10000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO60_PU_PD_DIS_SHIFT (28U)
/*! GPIO60_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[60] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO60_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO60_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO60_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO61_PU_PD_DIS_MASK (0x20000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO61_PU_PD_DIS_SHIFT (29U)
/*! GPIO61_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[61] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO61_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO61_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO61_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO62_PU_PD_DIS_MASK (0x40000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO62_PU_PD_DIS_SHIFT (30U)
/*! GPIO62_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[62] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO62_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO62_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO62_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO63_PU_PD_DIS_MASK (0x80000000U)
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO63_PU_PD_DIS_SHIFT (31U)
/*! GPIO63_PU_PD_DIS - Internal Pd and Internal Pu Disable for GPIO[63] During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO63_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO63_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS1_GPIO63_PU_PD_DIS_MASK)
/*! @} */

/*! @name PAD_SLP_PU_PD_DIS2 - Pad Sleep Pullup and Pulldown Disable4 */
/*! @{ */

#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST0_PU_PD_DIS_MASK (0x1U)
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST0_PU_PD_DIS_SHIFT (0U)
/*! ATEST0_PU_PD_DIS - Internal Pd and Internal Pu Disable for ATEST0 During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST0_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST0_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST0_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST1_PU_PD_DIS_MASK (0x2U)
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST1_PU_PD_DIS_SHIFT (1U)
/*! ATEST1_PU_PD_DIS - Internal Pd and Internal Pu Disable for ATEST1 During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST1_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST1_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST1_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST2_PU_PD_DIS_MASK (0x4U)
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST2_PU_PD_DIS_SHIFT (2U)
/*! ATEST2_PU_PD_DIS - Internal Pd and Internal Pu Disable for ATEST2 During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST2_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST2_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST2_PU_PD_DIS_MASK)

#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST3_PU_PD_DIS_MASK (0x8U)
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST3_PU_PD_DIS_SHIFT (3U)
/*! ATEST3_PU_PD_DIS - Internal Pd and Internal Pu Disable for ATEST3 During Sleep Mode */
#define SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST3_PU_PD_DIS(x) (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST3_PU_PD_DIS_SHIFT)) & SOCCIU_PAD_SLP_PU_PD_DIS2_ATEST3_PU_PD_DIS_MASK)
/*! @} */

/*! @name PAD_GPIO - GPIO Enable */
/*! @{ */

#define SOCCIU_PAD_GPIO_ENABLE_MASK              (0xFFFFU)
#define SOCCIU_PAD_GPIO_ENABLE_SHIFT             (0U)
/*! ENABLE - GPIO Automatic Forcing for GPIO[15:0] */
#define SOCCIU_PAD_GPIO_ENABLE(x)                (((uint32_t)(((uint32_t)(x)) << SOCCIU_PAD_GPIO_ENABLE_SHIFT)) & SOCCIU_PAD_GPIO_ENABLE_MASK)
/*! @} */

/*! @name MCI_IOMUX_EN0 - mci_iomux_enable control for GPIO[31:28] and GPIO[21:0] */
/*! @{ */

#define SOCCIU_MCI_IOMUX_EN0_EN_21_0_MASK        (0x3FFFFFU)
#define SOCCIU_MCI_IOMUX_EN0_EN_21_0_SHIFT       (0U)
/*! EN_21_0 - Bitwise enable control for mci_io_mux GPIO[21:0] */
#define SOCCIU_MCI_IOMUX_EN0_EN_21_0(x)          (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_IOMUX_EN0_EN_21_0_SHIFT)) & SOCCIU_MCI_IOMUX_EN0_EN_21_0_MASK)

#define SOCCIU_MCI_IOMUX_EN0_EN_31_28_MASK       (0xF0000000U)
#define SOCCIU_MCI_IOMUX_EN0_EN_31_28_SHIFT      (28U)
/*! EN_31_28 - Bitwise enable control for mci_io_mux GPIO[31:28] */
#define SOCCIU_MCI_IOMUX_EN0_EN_31_28(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_IOMUX_EN0_EN_31_28_SHIFT)) & SOCCIU_MCI_IOMUX_EN0_EN_31_28_MASK)
/*! @} */

/*! @name MCI_IOMUX_EN1 - mci_iomux_enable control for GPIO[63:32] */
/*! @{ */

#define SOCCIU_MCI_IOMUX_EN1_EN_MASK             (0xFFFFFFFFU)
#define SOCCIU_MCI_IOMUX_EN1_EN_SHIFT            (0U)
/*! EN - Bitwise enable control for mci_io_mux GPIO[63:32] */
#define SOCCIU_MCI_IOMUX_EN1_EN(x)               (((uint32_t)(((uint32_t)(x)) << SOCCIU_MCI_IOMUX_EN1_EN_SHIFT)) & SOCCIU_MCI_IOMUX_EN1_EN_MASK)
/*! @} */

/*! @name CAU_CTRL - CAU Control */
/*! @{ */

#define SOCCIU_CAU_CTRL_CAU_REFCLK_SEL_MASK      (0x800U)
#define SOCCIU_CAU_CTRL_CAU_REFCLK_SEL_SHIFT     (11U)
/*! CAU_REFCLK_SEL - CAU Reference Clock Select */
#define SOCCIU_CAU_CTRL_CAU_REFCLK_SEL(x)        (((uint32_t)(((uint32_t)(x)) << SOCCIU_CAU_CTRL_CAU_REFCLK_SEL_SHIFT)) & SOCCIU_CAU_CTRL_CAU_REFCLK_SEL_MASK)

#define SOCCIU_CAU_CTRL_CAU_BG_RDY_MASK          (0x80000U)
#define SOCCIU_CAU_CTRL_CAU_BG_RDY_SHIFT         (19U)
/*! CAU_BG_RDY - CAU bandgap Status */
#define SOCCIU_CAU_CTRL_CAU_BG_RDY(x)            (((uint32_t)(((uint32_t)(x)) << SOCCIU_CAU_CTRL_CAU_BG_RDY_SHIFT)) & SOCCIU_CAU_CTRL_CAU_BG_RDY_MASK)

#define SOCCIU_CAU_CTRL_BUCK_VOUT_RDY_MASK       (0x100000U)
#define SOCCIU_CAU_CTRL_BUCK_VOUT_RDY_SHIFT      (20U)
/*! BUCK_VOUT_RDY - Buck power Status */
#define SOCCIU_CAU_CTRL_BUCK_VOUT_RDY(x)         (((uint32_t)(((uint32_t)(x)) << SOCCIU_CAU_CTRL_BUCK_VOUT_RDY_SHIFT)) & SOCCIU_CAU_CTRL_BUCK_VOUT_RDY_MASK)

#define SOCCIU_CAU_CTRL_REF_CLK_RDY_MASK         (0x200000U)
#define SOCCIU_CAU_CTRL_REF_CLK_RDY_SHIFT        (21U)
/*! REF_CLK_RDY - CAU Ref clock Ready Status */
#define SOCCIU_CAU_CTRL_REF_CLK_RDY(x)           (((uint32_t)(((uint32_t)(x)) << SOCCIU_CAU_CTRL_REF_CLK_RDY_SHIFT)) & SOCCIU_CAU_CTRL_REF_CLK_RDY_MASK)
/*! @} */

/*! @name SYSPLL_CTRL - SYSPLL Control */
/*! @{ */

#define SOCCIU_SYSPLL_CTRL_T3_PLL_LOCK_MASK      (0x400U)
#define SOCCIU_SYSPLL_CTRL_T3_PLL_LOCK_SHIFT     (10U)
/*! T3_PLL_LOCK - PLL T3 Lock Status */
#define SOCCIU_SYSPLL_CTRL_T3_PLL_LOCK(x)        (((uint32_t)(((uint32_t)(x)) << SOCCIU_SYSPLL_CTRL_T3_PLL_LOCK_SHIFT)) & SOCCIU_SYSPLL_CTRL_T3_PLL_LOCK_MASK)
/*! @} */


/*!
 * @}
 */ /* end of group SOCCIU_Register_Masks */


/*!
 * @}
 */ /* end of group SOCCIU_Peripheral_Access_Layer */


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


#endif  /* SOCCIU_H_ */

