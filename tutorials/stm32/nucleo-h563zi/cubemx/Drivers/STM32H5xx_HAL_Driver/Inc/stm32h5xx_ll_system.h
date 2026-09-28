/**
  ******************************************************************************
  * @file    stm32h5xx_ll_system.h
  * @author  MCD Application Team
  * @brief   Header file of SYSTEM LL module.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================
    [..]
    The LL SYSTEM driver contains a set of generic APIs that can be
    used by user:
      (+) Some of the FLASH features need to be handled in the SYSTEM file.
      (+) Access to DBGCMU registers
      (+) Access to SBS registers
      (+) Access to VREFBUF registers
  @endverbatim
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H5xx_LL_SYSTEM_H
#define STM32H5xx_LL_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx.h"

/** @addtogroup STM32H5xx_LL_Driver
  * @{
  */

#if defined (FLASH) || defined (SBS) || defined (DBGMCU) || defined (VREFBUF)

/** @defgroup SYSTEM_LL SYSTEM
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/
/** @defgroup SYSTEM_LL_Private_Constants SYSTEM Private Constants
  * @{
  */
#define LL_SBS_HDPL_INCREMENT_VALUE   0x6AU                               /*!< Define used for the HDPL increment */
#define LL_SBS_DBG_UNLOCK             (0xB4UL << SBS_DBGCR_DBG_UNLOCK_Pos) /*!< Define used to unlock debug */
#define LL_SBS_ACCESS_PORT_UNLOCK     0xB4U                               /*!< Define used to unlock access port */
#define LL_SBS_DBG_CONFIG_LOCK        0xC3U                               /*!< Define used to lock debug configuration */
#define LL_SBS_DBG_CONFIG_UNLOCK      0xB4U                               /*!< Define used to unlock debug configuration */
#define LL_SBS_DEBUG_SEC_NSEC         0xB4U                               /*!< Define used to open debug for secure and non-secure */
#define LL_SBS_DEBUG_NSEC             0x3CU                               /*!< Define used to open debug for non-secure only */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/** @defgroup SYSTEM_LL_Exported_Constants SYSTEM Exported Constants
  * @{
  */

/** @defgroup SYSTEM_LL_SBS_EC_FASTMODEPLUS SBS FASTMODEPLUS
  * @{
  */
#define LL_SBS_FASTMODEPLUS_PB6     SBS_PMCR_PB6_FMP    /*!< Enable Fast Mode Plus on PB6 */
#define LL_SBS_FASTMODEPLUS_PB7     SBS_PMCR_PB7_FMP    /*!< Enable Fast Mode Plus on PB7 */
#define LL_SBS_FASTMODEPLUS_PB8     SBS_PMCR_PB8_FMP    /*!< Enable Fast Mode Plus on PB8 */
#if defined(SBS_PMCR_PB9_FMP)
#define LL_SBS_FASTMODEPLUS_PB9     SBS_PMCR_PB9_FMP    /*!< Enable Fast Mode Plus on PB9 */
#endif /* SBS_PMCR_PB9_FMP */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EC_CS1 SBS Vdd compensation cell Code selection
  * @{
  */
#define LL_SBS_VDD_CELL_CODE                  0x0UL            /*!< VDD I/Os code from the cell (available in the SBS_CCVALR) */
#define LL_SBS_VDD_REGISTER_CODE              SBS_CCCSR_CS1    /*!< VDD I/Os code from the SBS compensation cell code register (SBS_CCSWCR) */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EC_CS2 SBS VddIO compensation cell Code selection
  * @{
  */
#define LL_SBS_VDDIO_CELL_CODE                0x0UL            /*!< VDDIO I/Os code from the cell (available in the SBS_CCVALR)*/
#define LL_SBS_VDDIO_REGISTER_CODE            SBS_CCCSR_CS2    /*!< VDDIO I/Os code from the SBS compensation cell code register (SBS_CCSWCR)*/
/**
  * @}
  */

#if defined(SBS_PMCR_ETH_SEL_PHY)
/** @defgroup SYSTEM_LL_SBS_ETHERNET_CONFIG  ETHENET CONFIG
  * @{
  */
#define LL_SBS_ETH_MII                   0x0UL                    /*!< Select the Media Independent Interface (MII) or GMII  */
#define LL_SBS_ETH_RMII                  SBS_PMCR_ETH_SEL_PHY_2   /*!< Select the Reduced Media Independent Interface (RMII) */

/**
  * @}
  */
#endif /* SBS_PMCR_ETH_SEL_PHY */

/** @defgroup SYSTEM_Memories_Erase_Flag_Status  Memories Erase Flags Status
  * @{
  */
#define LL_SBS_MEMORIES_ERASE_MCLR_ON_GOING       0x0UL              /*!< Erase after Power-on Reset of SRAM2, BKPRAM, ICACHE, DCACHE and PKA RAMs on going or cleared by SW */
#define LL_SBS_MEMORIES_ERASE_MCLR_ENDED          SBS_MESR_MCLR      /*!< Erase after Power-on Reset of SRAM2, BKPRAM, ICACHE, DCACHE and PKA RAMs done */
#define LL_SBS_MEMORIES_ERASE_IPMEE_ON_GOING      0x0UL              /*!< Erase after Power-on Reset or Tamper detection for ICACHE and PKA RAMs on going or cleared by SW */
#define LL_SBS_MEMORIES_ERASE_IPMEE_ENDED         SBS_MESR_IPMEE     /*!< Erase after Power-on Reset or Tamper detection for ICACHE and PKA RAMs done */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EC_TIMBREAK SBS TIMER BREAK
  * @{
  */
#define LL_SBS_TIMBREAK_ECC             SBS_CFGR2_ECCL    /*!< Enables and locks the Flash ECC double error signal
                                                                   with Break Input of TIM1/8/15/16/17  */
#define LL_SBS_TIMBREAK_PVD             SBS_CFGR2_PVDL    /*!< Enables and locks the PVD connection
                                                                   with TIM1/8/15/16/17 Break Input and also the PVDE
                                                                   and PLS bits of the Power Control Interface */
#define LL_SBS_TIMBREAK_SRAM_ECC        SBS_CFGR2_SEL     /*!< Enables and locks the SRAM ECC double error signal
                                                                   with Break Input of TIM1/8/15/16/17                */
#define LL_SBS_TIMBREAK_LOCKUP          SBS_CFGR2_CLL     /*!< Enables and locks the LOCKUP (Hardfault) output of
                                                                    Cortex-M33 with Break Input of TIM1/15/16/17      */
/**
  * @}
  */


/** @defgroup SYSTEM_LL_SBS_EPOCH_Selection  EPOCH Selection
  * @{
  */
#define LL_SBS_EPOCH_SEL_NONSECURE      0x0UL                         /*!< EPOCH non secure selected */
#define LL_SBS_EPOCH_SEL_SECURE         SBS_EPOCHSELCR_EPOCH_SEL_0    /*!< EPOCH secure selected */
#define LL_SBS_EPOCH_SEL_PUFCHECK       SBS_EPOCHSELCR_EPOCH_SEL_1    /*!< EPOCH all zeros for PUF integrity check */

/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_NextHDPL_Selection  Next HDPL Selection
  * @{
  */
#define LL_SBS_OBKHDPL_INCR_0           0x00000000U                   /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
#define LL_SBS_OBKHDPL_INCR_1           SBS_NEXTHDPLCR_NEXTHDPL_0     /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
#define LL_SBS_OBKHDPL_INCR_2           SBS_NEXTHDPLCR_NEXTHDPL_1     /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
#define LL_SBS_OBKHDPL_INCR_3           SBS_NEXTHDPLCR_NEXTHDPL       /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_HDPL_Value  HDPL Value
  * @{
  */
#define LL_SBS_HDPL_VALUE_0             0x000000B4U                   /*!< Hide protection level 0 */
#define LL_SBS_HDPL_VALUE_1             0x00000051U                   /*!< Hide protection level 1 */
#define LL_SBS_HDPL_VALUE_2             0x0000008AU                   /*!< Hide protection level 2 */
#define LL_SBS_HDPL_VALUE_3             0x0000006FU                   /*!< Hide protection level 3 */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_NS_Lock_items Lock items
  * @brief SBS non secure items to set lock on
  * @{
  */
#define LL_SBS_MPU_NSEC                SBS_CNSLCKR_LOCKNSMPU           /*!< Non-secure MPU lock (privileged secure or non-secure only) */
#define LL_SBS_VTOR_NSEC               SBS_CNSLCKR_LOCKNSVTOR          /*!< Non-secure VTOR lock (privileged secure or non-secure only) */
#define LL_SBS_LOCK_ALL_NSEC          (LL_SBS_MPU_NSEC | LL_SBS_VTOR_NSEC)   /*!< lock all Non-secure  (privileged secure or non-secure only) */
/**
  * @}
  */
#if defined(SBS_OTGHSPHYTUNER2_SQRXTUNE)
/** @defgroup SYSTEM_LL_SBS_OTG_SQUELSH OTG High-speed (HS) PHY Squelch threshold adjustment
  * @{
  */
#define LL_SBS_OTGHSPHY_SQUELCH_15PERCENT  0x00000000U                                                            /*!< +15% (recommended value) */
#define LL_SBS_OTGHSPHY_SQUELCH_0PERCENT   (SBS_OTGHSPHYTUNER2_SQRXTUNE_0 | SBS_OTGHSPHYTUNER2_SQRXTUNE_1)  /*!< 0% (default value) */
/**
  * @}
  */
#endif /* SBS_OTGHSPHYTUNER2_SQRXTUNE */

#if defined(SBS_OTGHSPHYTUNER2_COMPDISTUNE)
/** @defgroup SYSTEM_LL_SBS_OTG_PHYTUNER_DISCONNECT_THRESTHOLD OTG High-speed (HS) PHYTUNER disconnect threshold
  * @{
  */
#define LL_SBS_OTGHSPHY_DISCONNECT_5_9PERCENT  SBS_OTGHSPHYTUNER2_COMPDISTUNE_1  /*!< +5.9% (recommended value) */
#define LL_SBS_OTGHSPHY_DISCONNECT_0PERCENT    SBS_OTGHSPHYTUNER2_COMPDISTUNE_0  /*!< 0% (default value) */
/**
  * @}
  */
#endif /* SBS_OTGHSPHYTUNER2_COMPDISTUNE */

#if defined(SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE)
/** @defgroup SYSTEM_LL_SBS_OTG_TRANSMITTER_PREEMPHASIS_CURRENT OTG High-speed (HS) transmitter preemphasis current control
  * @{
  */
#define LL_SBS_OTGHSPHY_PREEMP_DISABLED  0x00000000U                              /*!< HS transmitter preemphasis circuit disabled */
#define LL_SBS_OTGHSPHY_PREEMP_1X        SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_0     /*!< HS transmitter preemphasis circuit sources 1x preemphasis current */
#define LL_SBS_OTGHSPHY_PREEMP_2X        SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_1     /*!< HS transmitter preemphasis circuit sources 2x preemphasis current */
#define LL_SBS_OTGHSPHY_PREEMP_3X       (SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_0 | \
                                            SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_1) /*!< HS transmitter preemphasis circuit sources 3x preemphasis current */
/**
  * @}
  */
#endif /* SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/** @defgroup SYSTEM_LL_SBS_S_Lock_items SBS Lock items
  * @brief SBS secure items to set lock on
  * @{
  */
#define LL_SBS_SAU                     SBS_CSLCKR_LOCKSAU              /*!< SAU lock (privileged secure code only) */
#define LL_SBS_MPU_SEC                 SBS_CSLCKR_LOCKSMPU             /*!< Secure MPU lock (privileged secure code only) */
#define LL_SBS_VTOR_AIRCR_SEC          SBS_CSLCKR_LOCKSVTAIRCR         /*!< VTOR_S and AIRCR lock (privileged secure code only) */
#define LL_SBS_LOCK_ALL_SEC           (LL_SBS_SAU | LL_SBS_MPU_SEC | LL_SBS_VTOR_AIRCR_SEC)  /*!< lock all secure (privileged secure only)  */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EC_SECURE_ATTRIBUTES Secure attributes
  * @note Only available when system implements security (TZEN=1)
  * @{
  */
#define LL_SBS_CLOCK_SEC                SBS_SECCFGR_SBSSEC      /*!< SBS clock configuration secure-only access */
#define LL_SBS_CLOCK_NSEC               0U                      /*!< SBS clock configuration secure/non-secure access */
#define LL_SBS_CLASSB_SEC               SBS_SECCFGR_CLASSBSEC   /*!< Class B configuration secure-only access */
#define LL_SBS_CLASSB_NSEC              0U                      /*!< Class B configuration secure/non-secure access */
#define LL_SBS_FPU_SEC                  SBS_SECCFGR_FPUSEC      /*!< FPU configuration secure-only access */
#define LL_SBS_FPU_NSEC                 0U                      /*!< FPU configuration secure/non-secure access */
/**
  * @}
  */
#endif /* __ARM_FEATURE_CMSE */

/** @defgroup SYSTEM_LL_DBGMCU_EC_TRACE DBGMCU TRACE Pin Assignment
  * @{
  */
#define LL_DBGMCU_TRACE_NONE               0x00000000U                                     /*!< TRACE pins not assigned (default state) */
#define LL_DBGMCU_TRACE_ASYNCH             DBGMCU_CR_TRACE_IOEN                            /*!< TRACE pin assignment for Asynchronous Mode */
#define LL_DBGMCU_TRACE_SYNCH_SIZE1        (DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_0) /*!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 1 */
#define LL_DBGMCU_TRACE_SYNCH_SIZE2        (DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_1) /*!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 2 */
#define LL_DBGMCU_TRACE_SYNCH_SIZE4        (DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE)   /*!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 4 */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_DBGMCU_EC_APB1_GRP1_STOP_IP DBGMCU APB1 GRP1 STOP IP
  * @{
  */
#define LL_DBGMCU_APB1_GRP1_TIM2_STOP      DBGMCU_APB1FZR1_DBG_TIM2_STOP   /*!< The counter clock of TIM2 is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_TIM3_STOP      DBGMCU_APB1FZR1_DBG_TIM3_STOP   /*!< The counter clock of TIM3 is stopped when the core is halted*/
#if defined(TIM4)
#define LL_DBGMCU_APB1_GRP1_TIM4_STOP      DBGMCU_APB1FZR1_DBG_TIM4_STOP   /*!< The counter clock of TIM4 is stopped when the core is halted*/
#endif /* TIM4 */
#if defined(TIM5)
#define LL_DBGMCU_APB1_GRP1_TIM5_STOP      DBGMCU_APB1FZR1_DBG_TIM5_STOP   /*!< The counter clock of TIM5 is stopped when the core is halted*/
#endif /* TIM5 */
#define LL_DBGMCU_APB1_GRP1_TIM6_STOP      DBGMCU_APB1FZR1_DBG_TIM6_STOP   /*!< The counter clock of TIM6 is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_TIM7_STOP      DBGMCU_APB1FZR1_DBG_TIM7_STOP   /*!< The counter clock of TIM7 is stopped when the core is halted*/
#if defined(TIM12)
#define LL_DBGMCU_APB1_GRP1_TIM12_STOP     DBGMCU_APB1FZR1_DBG_TIM12_STOP  /*!< The counter clock of TIM12 is stopped when the core is halted*/
#endif /* TIM12 */
#if defined(TIM13)
#define LL_DBGMCU_APB1_GRP1_TIM13_STOP     DBGMCU_APB1FZR1_DBG_TIM13_STOP  /*!< The counter clock of TIM13 is stopped when the core is halted*/
#endif /* TIM13 */
#if defined(TIM14)
#define LL_DBGMCU_APB1_GRP1_TIM14_STOP     DBGMCU_APB1FZR1_DBG_TIM14_STOP  /*!< The counter clock of TIM14 is stopped when the core is halted*/
#endif /* TIM14 */
#define LL_DBGMCU_APB1_GRP1_WWDG_STOP      DBGMCU_APB1FZR1_DBG_WWDG_STOP   /*!< The window watchdog counter clock is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_IWDG_STOP      DBGMCU_APB1FZR1_DBG_IWDG_STOP   /*!< The independent watchdog counter clock is stopped when the core is halted*/
#define LL_DBGMCU_APB1_GRP1_I2C1_STOP      DBGMCU_APB1FZR1_DBG_I2C1_STOP   /*!< The I2C1 SMBus timeout is frozen*/
#define LL_DBGMCU_APB1_GRP1_I2C2_STOP      DBGMCU_APB1FZR1_DBG_I2C2_STOP   /*!< The I2C2 SMBus timeout is frozen*/
#define LL_DBGMCU_APB1_GRP1_I3C1_STOP      DBGMCU_APB1FZR1_DBG_I3C1_STOP   /*!< The I3C1 SMBus timeout is frozen*/
/**
  * @}
  */

/** @defgroup SYSTEM_LL_DBGMCU_EC_APB1_GRP2_STOP_IP DBGMCU APB1 GRP2 STOP IP
  * @{
  */
#define LL_DBGMCU_APB1_GRP2_LPTIM2_STOP    DBGMCU_APB1FZR2_DBG_LPTIM2_STOP /*!< The counter clock of LPTIM2 is stopped when the core is halted*/
/**
  * @}
  */

/** @defgroup SYSTEM_LL_DBGMCU_EC_APB2_GRP1_STOP_IP DBGMCU APB2 GRP1 STOP IP
  * @{
  */
#define LL_DBGMCU_APB2_GRP1_TIM1_STOP      DBGMCU_APB2FZR_DBG_TIM1_STOP    /*!< The counter clock of TIM1 is stopped when the core is halted*/
#if defined(TIM8)
#define LL_DBGMCU_APB2_GRP1_TIM8_STOP      DBGMCU_APB2FZR_DBG_TIM8_STOP    /*!< The counter clock of TIM8 is stopped when the core is halted*/
#endif /* TIM8 */
#if defined(TIM15)
#define LL_DBGMCU_APB2_GRP1_TIM15_STOP     DBGMCU_APB2FZR_DBG_TIM15_STOP   /*!< The counter clock of TIM15 is stopped when the core is halted*/
#endif /* TIM15 */
#if defined(TIM16)
#define LL_DBGMCU_APB2_GRP1_TIM16_STOP     DBGMCU_APB2FZR_DBG_TIM16_STOP   /*!< The counter clock of TIM16 is stopped when the core is halted*/
#endif /* TIM16 */
#if defined(TIM17)
#define LL_DBGMCU_APB2_GRP1_TIM17_STOP     DBGMCU_APB2FZR_DBG_TIM17_STOP   /*!< The counter clock of TIM17 is stopped when the core is halted*/
#endif /* TIM17 */
/**
  * @}
  */

/** @defgroup SYSTEM_LL_DBGMCU_EC_APB3_GRP1_STOP_IP DBGMCU APB3 GRP1 STOP IP
  * @{
  */
#if defined(I2C3)
#define LL_DBGMCU_APB3_GRP1_I2C3_STOP      DBGMCU_APB3FZR_DBG_I2C3_STOP    /*!< The counter clock of I2C3 is stopped when the core is halted*/
#endif /* I2C3 */
#if defined(I2C4)
#define LL_DBGMCU_APB3_GRP1_I2C4_STOP      DBGMCU_APB3FZR_DBG_I2C4_STOP    /*!< The counter clock of I2C4 is stopped when the core is halted*/
#endif /* I2C4 */
#if defined(I3C2)
#define LL_DBGMCU_APB3_GRP1_I3C2_STOP      DBGMCU_APB3FZR_DBG_I3C2_STOP    /*!< The counter clock of I3C2 is stopped when the core is halted*/
#endif /* I3C2 */
#define LL_DBGMCU_APB3_GRP1_LPTIM1_STOP    DBGMCU_APB3FZR_DBG_LPTIM1_STOP  /*!< The counter clock of LPTIM1 is stopped when the core is halted*/
#if defined(LPTIM3)
#define LL_DBGMCU_APB3_GRP1_LPTIM3_STOP    DBGMCU_APB3FZR_DBG_LPTIM3_STOP  /*!< The counter clock of LPTIM3 is stopped when the core is halted*/
#endif /* LPTIM3 */
#if defined(LPTIM4)
#define LL_DBGMCU_APB3_GRP1_LPTIM4_STOP    DBGMCU_APB3FZR_DBG_LPTIM4_STOP  /*!< The counter clock of LPTIM4 is stopped when the core is halted*/
#endif /* LPTIM4 */
#if defined(LPTIM5)
#define LL_DBGMCU_APB3_GRP1_LPTIM5_STOP    DBGMCU_APB3FZR_DBG_LPTIM5_STOP  /*!< The counter clock of LPTIM5 is stopped when the core is halted*/
#endif /* LPTIM5 */
#if defined(LPTIM6)
#define LL_DBGMCU_APB3_GRP1_LPTIM6_STOP    DBGMCU_APB3FZR_DBG_LPTIM6_STOP  /*!< The counter clock of LPTIM6 is stopped when the core is halted*/
#endif /* LPTIM6 */
#define LL_DBGMCU_APB3_GRP1_RTC_STOP       DBGMCU_APB3FZR_DBG_RTC_STOP     /*!< The counter clock of RTC is stopped when the core is halted*/
/**
  * @}
  */


#if defined(VREFBUF)
/** @defgroup SYSTEM_LL_VREFBUF_EC_VOLTAGE VREFBUF VOLTAGE
  * @{
  */
#define LL_VREFBUF_VOLTAGE_SCALE0          ((uint32_t)0x00000000)                     /*!< Voltage reference scale 0 (VREF_OUT1) */
#define LL_VREFBUF_VOLTAGE_SCALE1          VREFBUF_CSR_VRS_0                          /*!< Voltage reference scale 1 (VREF_OUT2) */
#define LL_VREFBUF_VOLTAGE_SCALE2          VREFBUF_CSR_VRS_1                          /*!< Voltage reference scale 2 (VREF_OUT3) */
#define LL_VREFBUF_VOLTAGE_SCALE3          (VREFBUF_CSR_VRS_0 |  VREFBUF_CSR_VRS_1)   /*!< Voltage reference scale 3 (VREF_OUT4) */
/**
  * @}
  */
#endif /* VREFBUF */

/** @defgroup SYSTEM_LL_FLASH_EC_LATENCY FLASH LATENCY
  * @{
  */
#define LL_FLASH_LATENCY_0                 FLASH_ACR_LATENCY_0WS   /*!< FLASH zero wait state */
#define LL_FLASH_LATENCY_1                 FLASH_ACR_LATENCY_1WS   /*!< FLASH one wait state */
#define LL_FLASH_LATENCY_2                 FLASH_ACR_LATENCY_2WS   /*!< FLASH two wait states */
#define LL_FLASH_LATENCY_3                 FLASH_ACR_LATENCY_3WS   /*!< FLASH three wait states */
#define LL_FLASH_LATENCY_4                 FLASH_ACR_LATENCY_4WS   /*!< FLASH four wait states */
#define LL_FLASH_LATENCY_5                 FLASH_ACR_LATENCY_5WS   /*!< FLASH five wait states */
#define LL_FLASH_LATENCY_6                 FLASH_ACR_LATENCY_6WS   /*!< FLASH six wait state */
#define LL_FLASH_LATENCY_7                 FLASH_ACR_LATENCY_7WS   /*!< FLASH Seven wait states */
#define LL_FLASH_LATENCY_8                 FLASH_ACR_LATENCY_8WS   /*!< FLASH Eight wait states */
#define LL_FLASH_LATENCY_9                 FLASH_ACR_LATENCY_9WS   /*!< FLASH nine wait states */
#define LL_FLASH_LATENCY_10                FLASH_ACR_LATENCY_10WS  /*!< FLASH ten wait states */
#define LL_FLASH_LATENCY_11                FLASH_ACR_LATENCY_11WS  /*!< FLASH eleven wait states */
#define LL_FLASH_LATENCY_12                FLASH_ACR_LATENCY_12WS  /*!< FLASH twelve wait states */
#define LL_FLASH_LATENCY_13                FLASH_ACR_LATENCY_13WS  /*!< FLASH thirteen wait states */
#define LL_FLASH_LATENCY_14                FLASH_ACR_LATENCY_14WS  /*!< FLASH fourteen wait states */
#define LL_FLASH_LATENCY_15                FLASH_ACR_LATENCY_15WS  /*!< FLASH fifteen wait states */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup SYSTEM_LL_Exported_Functions SYSTEM Exported Functions
  * @{
  */

/** @defgroup SYSTEM_LL_EF_SBS SBS
  * @{
  */

#if defined(SBS_PMCR_ETH_SEL_PHY)
/**
  * @brief  Select Ethernet PHY interface
  * @rmtoll PMCR    EPIS_SEL    LL_SBS_SetPHYInterface
  * @param  Interface This parameter can be one of the following values:
  *         @arg @ref LL_SBS_ETH_MII
  *         @arg @ref LL_SBS_ETH_RMII
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetPHYInterface(uint32_t Interface)
{
  MODIFY_REG(SBS->PMCR, SBS_PMCR_ETH_SEL_PHY, Interface);
}

/**
  * @brief  Get Ethernet PHY interface
  * @rmtoll PMCR    EPIS_SEL    LL_SBS_GetPHYInterface
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SBS_ETH_MII
  *         @arg @ref LL_SBS_ETH_RMII
  */
__STATIC_INLINE uint32_t LL_SBS_GetPHYInterface(void)
{
  return (uint32_t)(READ_BIT(SBS->PMCR, SBS_PMCR_ETH_SEL_PHY));
}
#endif /* SBS_PMCR_ETH_SEL_PHY */

/**
  * @brief  Enable the fast mode plus driving capability.
  * @rmtoll PMCR     PBx_FMP   LL_SBS_EnableFastModePlus\n
  *         PMCR     PBx_FMP   LL_SBS_EnableFastModePlus
  * @param  ConfigFastModePlus This parameter can be a combination of the following values:
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB6
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB7
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB8
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB9
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableFastModePlus(uint32_t ConfigFastModePlus)
{
  SET_BIT(SBS->PMCR, ConfigFastModePlus);
}

/**
  * @brief  Disable the fast mode plus driving capability.
  * @rmtoll PMCR     PBx_FMP   LL_SBS_DisableFastModePlus\n
  *         PMCR     PBx_FMP      LL_SBS_DisableFastModePlus
  * @param  ConfigFastModePlus This parameter can be a combination of the following values:
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB6
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB7
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB8
  *         @arg @ref LL_SBS_FASTMODEPLUS_PB9
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableFastModePlus(uint32_t ConfigFastModePlus)
{
  CLEAR_BIT(SBS->PMCR, ConfigFastModePlus);
}

/**
  * @brief  Enable Floating Point Unit Invalid operation Interrupt
  * @rmtoll FPUIMR     FPU_IE_0      LL_SBS_EnableIT_FPU_IOC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableIT_FPU_IOC(void)
{
  SET_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_0);
}

/**
  * @brief  Enable Floating Point Unit Divide-by-zero Interrupt
  * @rmtoll FPUIMR     FPU_IE_1      LL_SBS_EnableIT_FPU_DZC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableIT_FPU_DZC(void)
{
  SET_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_1);
}

/**
  * @brief  Enable Floating Point Unit Underflow Interrupt
  * @rmtoll FPUIMR     FPU_IE_2      LL_SBS_EnableIT_FPU_UFC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableIT_FPU_UFC(void)
{
  SET_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_2);
}

/**
  * @brief  Enable Floating Point Unit Overflow Interrupt
  * @rmtoll FPUIMR     FPU_IE_3      LL_SBS_EnableIT_FPU_OFC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableIT_FPU_OFC(void)
{
  SET_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_3);
}

/**
  * @brief  Enable Floating Point Unit Input denormal Interrupt
  * @rmtoll FPUIMR     FPU_IE_4      LL_SBS_EnableIT_FPU_IDC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableIT_FPU_IDC(void)
{
  SET_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_4);
}

/**
  * @brief  Enable Floating Point Unit Inexact Interrupt
  * @rmtoll FPUIMR     FPU_IE_5      LL_SBS_EnableIT_FPU_IXC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableIT_FPU_IXC(void)
{
  SET_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_5);
}

/**
  * @brief  Disable Floating Point Unit Invalid operation Interrupt
  * @rmtoll FPUIMR     FPU_IE_0      LL_SBS_DisableIT_FPU_IOC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableIT_FPU_IOC(void)
{
  CLEAR_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_0);
}

/**
  * @brief  Disable Floating Point Unit Divide-by-zero Interrupt
  * @rmtoll FPUIMR      FPU_IE_1      LL_SBS_DisableIT_FPU_DZC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableIT_FPU_DZC(void)
{
  CLEAR_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_1);
}

/**
  * @brief  Disable Floating Point Unit Underflow Interrupt
  * @rmtoll FPUIMR     FPU_IE_2      LL_SBS_DisableIT_FPU_UFC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableIT_FPU_UFC(void)
{
  CLEAR_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_2);
}

/**
  * @brief  Disable Floating Point Unit Overflow Interrupt
  * @rmtoll FPUIMR     FPU_IE_3      LL_SBS_DisableIT_FPU_OFC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableIT_FPU_OFC(void)
{
  CLEAR_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_3);
}

/**
  * @brief  Disable Floating Point Unit Input denormal Interrupt
  * @rmtoll FPUIMR     FPU_IE_4      LL_SBS_DisableIT_FPU_IDC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableIT_FPU_IDC(void)
{
  CLEAR_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_4);
}

/**
  * @brief  Disable Floating Point Unit Inexact Interrupt
  * @rmtoll FPUIMR     FPU_IE_5      LL_SBS_DisableIT_FPU_IXC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableIT_FPU_IXC(void)
{
  CLEAR_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_5);
}

/**
  * @brief  Check if Floating Point Unit Invalid operation Interrupt source is enabled or disabled.
  * @rmtoll FPUIMR     FPU_IE_0      LL_SBS_IsEnabledIT_FPU_IOC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabledIT_FPU_IOC(void)
{
  return ((READ_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_0) == SBS_FPUIMR_FPU_IE_0) ? 1UL : 0UL);
}

/**
  * @brief  Check if Floating Point Unit Divide-by-zero Interrupt source is enabled or disabled.
  * @rmtoll FPUIMR     FPU_IE_1      LL_SBS_IsEnabledIT_FPU_DZC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabledIT_FPU_DZC(void)
{
  return ((READ_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_1) == SBS_FPUIMR_FPU_IE_1) ? 1UL : 0UL);
}

/**
  * @brief  Check if Floating Point Unit Underflow Interrupt source is enabled or disabled.
  * @rmtoll FPUIMR     FPU_IE_2      LL_SBS_IsEnabledIT_FPU_UFC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabledIT_FPU_UFC(void)
{
  return ((READ_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_2) == SBS_FPUIMR_FPU_IE_2) ? 1UL : 0UL);
}

/**
  * @brief  Check if Floating Point Unit Overflow Interrupt source is enabled or disabled.
  * @rmtoll FPUIMR     FPU_IE_3      LL_SBS_IsEnabledIT_FPU_OFC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabledIT_FPU_OFC(void)
{
  return ((READ_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_3) == SBS_FPUIMR_FPU_IE_3) ? 1UL : 0UL);
}

/**
  * @brief  Check if Floating Point Unit Input denormal Interrupt source is enabled or disabled.
  * @rmtoll FPUIMR FPU_IE_4      LL_SBS_IsEnabledIT_FPU_IDC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabledIT_FPU_IDC(void)
{
  return ((READ_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_4) == SBS_FPUIMR_FPU_IE_4) ? 1UL : 0UL);
}

/**
  * @brief  Check if Floating Point Unit Inexact Interrupt source is enabled or disabled.
  * @rmtoll FPUIMR FPU_IE_5      LL_SBS_IsEnabledIT_FPU_IXC
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabledIT_FPU_IXC(void)
{
  return ((READ_BIT(SBS->FPUIMR, SBS_FPUIMR_FPU_IE_5) == SBS_FPUIMR_FPU_IE_5) ? 1UL : 0UL);
}

/**
  * @brief  Set connections to TIM1/8/15/16/17 Break inputs
  * @rmtoll CFGR2     CLL       LL_SBS_SetTIMBreakInputs\n
  *         CFGR2     SEL       LL_SBS_SetTIMBreakInputs\n
  *         CFGR2     PVDL      LL_SBS_SetTIMBreakInputs\n
  *         CFGR2     ECCL      LL_SBS_SetTIMBreakInputs
  * @param  Break This parameter can be a combination of the following values:
  *         where non selected TIMBREAK input is disconnected.
  *         @arg @ref LL_SBS_TIMBREAK_ECC
  *         @arg @ref LL_SBS_TIMBREAK_PVD
  *         @arg @ref LL_SBS_TIMBREAK_SRAM_ECC
  *         @arg @ref LL_SBS_TIMBREAK_LOCKUP
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetTIMBreakInputs(uint32_t Break)
{
  MODIFY_REG(SBS->CFGR2, SBS_CFGR2_CLL | SBS_CFGR2_SEL | SBS_CFGR2_PVDL | SBS_CFGR2_ECCL, Break);
}

/**
  * @brief  Get connections to TIM1/8/15/16/17 Break inputs
  * @rmtoll CFGR2     CLL        LL_SBS_GetTIMBreakInputs\n
  *         CFGR2     SEL        LL_SBS_GetTIMBreakInputs\n
  *         CFGR2     PVDL       LL_SBS_GetTIMBreakInputs\n
  *         CFGR2     ECCL       LL_SBS_GetTIMBreakInputs
  * @retval Returned value can be a combination of the following values:
  *         @arg @ref LL_SBS_TIMBREAK_ECC
  *         @arg @ref LL_SBS_TIMBREAK_PVD
  *         @arg @ref LL_SBS_TIMBREAK_SRAM_ECC
  *         @arg @ref LL_SBS_TIMBREAK_LOCKUP
  */
__STATIC_INLINE uint32_t LL_SBS_GetTIMBreakInputs(void)
{
  return (uint32_t)(READ_BIT(SBS->CFGR2, SBS_CFGR2_CLL | SBS_CFGR2_SEL | SBS_CFGR2_PVDL | SBS_CFGR2_ECCL));
}

#if defined(SBS_EPOCHSELCR_EPOCH_SEL)
/**
  * @brief  Select EPOCH security sent to SAES IP to encrypt/decrypt keys
  * @rmtoll EPOCHSELCR     EPOCH_SEL      LL_SBS_EPOCHSelection
  * @param  Epoch_Selection: Select EPOCH security
  *         This parameter can be one of the following values:
  *         @arg LL_SBS_EPOCH_SEL_SECURE    : EPOCH secure selected.
  *         @arg LL_SBS_EPOCH_SEL_NONSECURE : EPOCH non secure selected.
  *         @arg LL_SBS_EPOCH_SEL_PUFCHECK  : EPOCH all zeros for PUF integrity check.
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EPOCHSelection(uint32_t Epoch_Selection)
{
  MODIFY_REG(SBS->EPOCHSELCR, SBS_EPOCHSELCR_EPOCH_SEL, (uint32_t)(Epoch_Selection));
}

/**
  * @brief  Get EPOCH security selection
  * @rmtoll EPOCHSELCR     EPOCH_SEL      LL_SBS_GetEPOCHSelection
  * @retval Returned value can be one of the following values:
  *         @arg LL_SBS_EPOCH_SEL_SECURE    : EPOCH secure selected.
  *         @arg LL_SBS_EPOCH_SEL_NONSECURE : EPOCH non secure selected.
  *         @arg LL_SBS_EPOCH_SEL_PUFCHECK  : EPOCH all zeros for PUF integrity check.
  */
__STATIC_INLINE uint32_t LL_SBS_GetEPOCHSelection(void)
{
  return (uint32_t)(READ_BIT(SBS->EPOCHSELCR, SBS_EPOCHSELCR_EPOCH_SEL));
}
#endif /* SBS_EPOCHSELCR_EPOCH_SEL */

/**
  * @brief  Disable the NMI in case of double ECC error in FLASH Interface.
  * @rmtoll ECCNMIR     SBS_ECCNMIR_ECCNMI_MASK_EN      LL_SBS_FLASH_DisableECCNMI
  * @retval None
  */
__STATIC_INLINE  void LL_SBS_FLASH_DisableECCNMI(void)
{
  SET_BIT(SBS->ECCNMIR, SBS_ECCNMIR_ECCNMI_MASK_EN);
}

/**
  * @brief  Enable the NMI in case of double ECC error in FLASH Interface.
  * @rmtoll ECCNMIR     SBS_ECCNMIR_ECCNMI_MASK_EN      LL_SBS_FLASH_EnableECCNMI
  * @retval None
  */
__STATIC_INLINE  void LL_SBS_FLASH_EnableECCNMI(void)
{
  CLEAR_BIT(SBS->ECCNMIR, SBS_ECCNMIR_ECCNMI_MASK_EN);
}

/** @defgroup SYSTEM_LL_SBS_EF_HDPL_Management HDPL Management
  * @{
  */

/**
  * @brief  Increment by 1 the HDPL value
  * @rmtoll HDPLCR     HDPL_INCR      LL_SBS_IncrementHDPLValue
  * @retval None
  */
__STATIC_INLINE void LL_SBS_IncrementHDPLValue(void)
{
  MODIFY_REG(SBS->HDPLCR, SBS_HDPLCR_INCR_HDPL, LL_SBS_HDPL_INCREMENT_VALUE);
}

/**
  * @brief  Get the HDPL Value.
  * @rmtoll HDPLSR     HDPL      LL_SBS_GetHDPLValue
  * @retval  Returns the HDPL value
  *          This return value can be one of the following values:
  *           @arg LL_SBS_HDPL_VALUE_0: HDPL0
  *           @arg LL_SBS_HDPL_VALUE_1: HDPL1
  *           @arg LL_SBS_HDPL_VALUE_2: HDPL2
  *           @arg LL_SBS_HDPL_VALUE_3: HDPL3
  */
__STATIC_INLINE uint32_t LL_SBS_GetHDPLValue(void)
{
  return (uint32_t)(READ_BIT(SBS->HDPLSR, SBS_HDPLSR_HDPL));
}

#if defined(SBS_NEXTHDPLCR_NEXTHDPL)
/**
  * @brief  Set the OBK-HDPL Value.
  * @rmtoll NEXTHDPLCR     NEXTHDPL      LL_SBS_SetOBKHDPL
  * @param  OBKHDPL_Value Value of increment to add to HDPL value to generate the OBK-HDPL.
  *         This parameter can be one of the following values:
  *         @arg LL_SBS_OBKHDPL_INCR_0 : HDPL
  *         @arg LL_SBS_OBKHDPL_INCR_1 : HDPL + 1
  *         @arg LL_SBS_OBKHDPL_INCR_2 : HDPL + 2
  *         @arg LL_SBS_OBKHDPL_INCR_3 : HDPL + 3
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetOBKHDPL(uint32_t OBKHDPL_Value)
{
  MODIFY_REG(SBS->NEXTHDPLCR, SBS_NEXTHDPLCR_NEXTHDPL, (uint32_t)(OBKHDPL_Value));
}

/**
  * @brief  Get the OBK-HDPL Value.
  * @rmtoll NEXTHDPLCR     NEXTHDPL      LL_SBS_GetOBKHDPL
  * @retval  Returns the incremement to add to HDPL value to generate OBK-HDPL
  *          This return value can be one of the following values:
  *          @arg LL_SBS_OBKHDPL_INCR_0: HDPL
  *          @arg LL_SBS_OBKHDPL_INCR_1: HDPL + 1
  *          @arg LL_SBS_OBKHDPL_INCR_2: HDPL + 2
  *          @arg LL_SBS_OBKHDPL_INCR_3: HDPL + 3
  */
__STATIC_INLINE  uint32_t LL_SBS_GetOBKHDPL(void)
{
  return (uint32_t)(READ_BIT(SBS->NEXTHDPLCR, SBS_NEXTHDPLCR_NEXTHDPL));
}
#endif /* SBS_NEXTHDPLCR_NEXTHDPL */

/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EF_Debug_Control Debug Control
  * @{
  */

/**
  * @brief  Set the authenticated debug hide protection level
  * @rmtoll SBS_DBGCR DBG_AUTH_HDPL     LL_SBS_SetAuthDbgHDPL
  * @param  Level This parameter can be one of the following values:
  *         @arg @ref LL_SBS_HDPL_VALUE_1
  *         @arg @ref LL_SBS_HDPL_VALUE_2
  *         @arg @ref LL_SBS_HDPL_VALUE_3
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetAuthDbgHDPL(uint32_t Level)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_DBG_AUTH_HDPL, (Level << SBS_DBGCR_DBG_AUTH_HDPL_Pos));
}

/**
  * @brief  Get current hide protection level
  * @rmtoll SBS_DBGCR DBG_AUTH_HDPL     LL_SBS_GetAuthDbgHDPL
  * @retval Returned value is the hide protection level where the authenticated debug is opened:
  *         @arg @ref LL_SBS_HDPL_VALUE_1
  *         @arg @ref LL_SBS_HDPL_VALUE_2
  *         @arg @ref LL_SBS_HDPL_VALUE_3
  */
__STATIC_INLINE uint32_t LL_SBS_GetAuthDbgHDPL(void)
{
  return (uint32_t)(READ_BIT(SBS->DBGCR, SBS_DBGCR_DBG_AUTH_HDPL) >> SBS_DBGCR_DBG_AUTH_HDPL_Pos);
}

#if defined(SBS_DBGCR_DBG_AUTH_SEC)
/**
  * @brief  Configure the authenticated debug security access.
  * @rmtoll SBS_DBGCR DBG_AUTH_SEC     LL_SBS_SetAuthDbgSec
  * @param  Security debug opening secure/non-secure or non-secure only
  *         This parameter can be one of the following values:
  *            @arg LL_SBS_DEBUG_SEC_NSEC: debug opening for secure and non-secure.
  *            @arg LL_SBS_DEBUG_NSEC: debug opening for non-secure only.
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetAuthDbgSec(uint32_t Security)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_DBG_AUTH_SEC, (Security << SBS_DBGCR_DBG_AUTH_SEC_Pos));
}

/**
  * @brief  Get the current value of the hide protection level.
  * @rmtoll SBS_DBGCR DBG_AUTH_SEC     LL_SBS_GetAuthDbgSec
  * @note   This function can be only used when device state is Closed.
  * @retval Returned value can be one of the following values:
  *            @arg SBS_DEBUG_SEC_NSEC: debug opening for secure and non-secure.
  *            @arg any other value: debug opening for non-secure only.
  */
__STATIC_INLINE uint32_t LL_SBS_GetAuthDbgSec(void)
{
  return ((SBS->DBGCR & SBS_DBGCR_DBG_AUTH_SEC) >> SBS_DBGCR_DBG_AUTH_SEC_Pos);
}
#endif /* SBS_DBGCR_DBG_AUTH_SEC */

/**
  * @brief  Unlock the debug
  * @rmtoll SBS_DBGCR DBG_UNLOCK     LL_SBS_UnlockDebug
  * @retval None
  */
__STATIC_INLINE void LL_SBS_UnlockDebug(void)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_DBG_UNLOCK, LL_SBS_DBG_UNLOCK);
}

/**
  * @brief  Check if the debug is unlocked
  * @rmtoll SBS_DBGCR DBG_UNLOCK     LL_SBS_IsUnlockedDebug
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsUnlockedDebug(void)
{
  return ((READ_BIT(SBS->DBGCR, SBS_DBGCR_DBG_UNLOCK) == LL_SBS_DBG_UNLOCK) ? 1UL : 0UL);
}

/**
  * @brief  Unlock the access port
  * @rmtoll SBS_DBGCR AP_UNLOCK     LL_SBS_UnlockAccessPort
  * @retval None
  */
__STATIC_INLINE void LL_SBS_UnlockAccessPort(void)
{
  MODIFY_REG(SBS->DBGCR, SBS_DBGCR_AP_UNLOCK, LL_SBS_ACCESS_PORT_UNLOCK);
}

/**
  * @brief  Check if the access port is unlocked
  * @rmtoll SBS_DBGCR AP_UNLOCK     LL_SBS_IsUnlockedAccessPort
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsUnlockedAccessPort(void)
{
  return ((READ_BIT(SBS->DBGCR, SBS_DBGCR_AP_UNLOCK) == LL_SBS_ACCESS_PORT_UNLOCK) ? 1UL : 0UL);
}

/**
  * @brief  Lock the debug configuration
  * @rmtoll SBS_DBGLOCKR DBGCFG_LOCK     LL_SBS_LockDebugConfig
  * @retval None
  */
__STATIC_INLINE void LL_SBS_LockDebugConfig(void)
{
  MODIFY_REG(SBS->DBGLOCKR, SBS_DBGLOCKR_DBGCFG_LOCK, LL_SBS_DBG_CONFIG_LOCK);
}

/**
  * @brief  Check if the debug configuration is locked
  * @rmtoll SBS_DBGLOCKR DBGCFG_LOCK     LL_SBS_IsLockedDebugConfig
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsLockedDebugConfig(void)
{
  return ((READ_BIT(SBS->DBGLOCKR, SBS_DBGLOCKR_DBGCFG_LOCK) != LL_SBS_DBG_CONFIG_UNLOCK) ? 1UL : 0UL);
}

/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EF_lock_Management lock Management
  * @{
  */

/**
  * @brief  Non-secure Lock of SBS item(s).
  * @note   Setting lock(s) depends on privilege mode in secure/non-secure code
  *         Lock(s) cleared only at system reset
  * @rmtoll CNSLCKR     LOCKNSVTOR     LL_SBS_NonSecureLock\n
  *         CNSLCKR     LOCKNSMPU      LL_SBS_NonSecureLock
  * @param  Item Item(s) to set lock on.
  *         This parameter can be one of the following values :
  *          @arg LL_SBS_VTOR_NSEC : VTOR_NS register lock
  *          @arg LL_SBS_MPU_NSEC : Non-secure MPU registers lock
  *          @arg LL_SBS_LOCK_ALL_NSEC : Non-secure MPU and VTOR_NS lock
  * @retval None
  */
__STATIC_INLINE void LL_SBS_NonSecureLock(uint32_t Item)
{
  /* Privilege secure/non-secure locks */
  SBS->CNSLCKR = Item;
}

/**
  * @brief  Get the non secure lock state of SBS items.
  * @note   Getting lock(s) depends on privilege mode in secure/non-secure code
  * @rmtoll CNSLCKR     LOCKNSVTOR     LL_SBS_NonSecureLock\n
  *         CNSLCKR     LOCKNSMPU      LL_SBS_NonSecureLock
  * @retval the return value can be one of the following values :
  *          @arg LL_SBS_VTOR_NSEC : VTOR_NS register lock
  *          @arg LL_SBS_MPU_NSEC : Non-secure MPU registers lock
  *          @arg LL_SBS_LOCK_ALL_NSEC : VTOR_NS and Non-secure MPU registers lock
  */
__STATIC_INLINE uint32_t LL_SBS_GetNonSecureLock(void)
{
  return (uint32_t)(READ_BIT(SBS->CNSLCKR, LL_SBS_LOCK_ALL_NSEC));
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Secure Lock of System item(s).
  * @note   Setting lock(s) depends on privilege mode in secure code
  *         Lock(s) cleared only at system reset
  * @rmtoll CSLCKR     LOCKSVTAIRCR     LL_SBS_SecureLock\n
  *         CSLCKR     LOCKSMPU         LL_SBS_SecureLock\n
  *         CSLCKR     LOCKSAU          LL_SBS_SecureLock
  * @param  Item Item(s) to set lock on.
  *         This parameter can be a combination of the following values :
  *          @arg LL_SBS_VTOR_AIRCR_SEC : VTOR_S and AIRCR registers lock
  *          @arg LL_SBS_MPU_SEC : Secure MPU registers lock
  *          @arg LL_SBS_SAU : SAU registers lock
  *          @arg LL_SBS_LOCK_ALL_SEC : VTOR_S, AIRCR, Secure MPU and SAU registers lock
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SecureLock(uint32_t Item)
{
  /* Privilege secure only locks */
  SBS->CSLCKR = Item;
}

/**
  * @brief  Get the secure lock state of System items.
  * @note   Getting lock(s) depends on privilege mode in secure code
  * @rmtoll CSLCKR     LOCKSVTAIRCR     LL_SBS_GetSecureLock\n
  *         CSLCKR     LOCKSMPU         LL_SBS_GetSecureLock\n
  *         CSLCKR     LOCKSAU          LL_SBS_GetSecureLock
  * @retval the return value is a combination  of the following values :
  *          @arg LL_SBS_VTOR_AIRCR_SEC : VTOR_S and AIRCR registers lock
  *          @arg LL_SBS_MPU_SEC : Secure MPU registers lock
  *          @arg LL_SBS_SAU : SAU registers lock
  *          @arg LL_SBS_LOCK_ALL_SEC : VTOR_S, AIRCR, Secure MPU and SAU registers lock
  */
__STATIC_INLINE uint32_t LL_SBS_GetSecureLock(void)
{
  return (uint32_t)(READ_BIT(SBS->CSLCKR, LL_SBS_LOCK_ALL_SEC));
}
#endif /* __ARM_FEATURE_CMSE && __ARM_FEATURE_CMSE == 3U */

/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EF_Secure_Management Secure Management
  * @{
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure Secure mode
  * @note Only available from secure state when system implements security (TZEN=1)
  * @rmtoll SECCFGR     SBSSEC        LL_SBS_ConfigSecure\n
  *         SECCFGR     CLASSBSEC     LL_SBS_ConfigSecure\n
  *         SECCFGR     FPUSEC        LL_SBS_ConfigSecure
  * @param  Configuration This parameter shall be the full combination
  *         of the following values:
  *         @arg @ref LL_SBS_CLOCK_SEC or LL_SBS_CLOCK_NSEC
  *         @arg @ref LL_SBS_CLASSB_SEC or LL_SBS_CLASSB_NSEC
  *         @arg @ref LL_SBS_FPU_SEC or LL_SBS_FPU_NSEC
  * @retval None
  */
__STATIC_INLINE void LL_SBS_ConfigSecure(uint32_t Configuration)
{
  WRITE_REG(SBS->SECCFGR, Configuration);
}

/**
  * @brief  Get Secure mode configuration
  * @note Only available when system implements security (TZEN=1)
  * @rmtoll SECCFGR     SBSSEC        LL_SBS_ConfigSecure\n
  *         SECCFGR     CLASSBSEC     LL_SBS_ConfigSecure\n
  *         SECCFGR     FPUSEC        LL_SBS_ConfigSecure
  * @retval Returned value is the combination of the following values:
  *         @arg @ref LL_SBS_CLOCK_SEC or LL_SBS_CLOCK_NSEC
  *         @arg @ref LL_SBS_CLASSB_SEC or LL_SBS_CLASSB_NSEC
  *         @arg @ref LL_SBS_FPU_SEC or LL_SBS_FPU_NSEC
  */
__STATIC_INLINE uint32_t LL_SBS_GetConfigSecure(void)
{
  return (uint32_t)(READ_BIT(SBS->SECCFGR, LL_SBS_CLOCK_SEC | LL_SBS_CLASSB_SEC | LL_SBS_FPU_SEC));
}
#endif /* __ARM_FEATURE_CMSE && __ARM_FEATURE_CMSE == 3U */

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup SYSTEM_LL_SBS_EF_COMPENSATION Compensation Cell Control
  * @{
  */

/**
  * @brief  Get the compensation cell value of the GPIO PMOS transistor supplied by VDD
  * @rmtoll CCVALR    APSRC1   LL_SBS_GetPMOSVddCompensationValue
  * @retval Returned value is the PMOS compensation cell
  */
__STATIC_INLINE uint32_t LL_SBS_GetPMOSVddCompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_APSRC1));
}

/**
  * @brief  Get the compensation cell value of the GPIO NMOS transistor supplied by VDD
  * @rmtoll CCVALR    ANSRC1   LL_SBS_GetNMOSVddCompensationValue
  * @retval Returned value is the NMOS compensation cell
  */
__STATIC_INLINE uint32_t LL_SBS_GetNMOSVddCompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_ANSRC1));
}

/**
  * @brief  Get the compensation cell value of the GPIO PMOS transistor supplied by VDDIO2
  * @rmtoll CCVALR    APSRC2   LL_SBS_GetPMOSVddIO2CompensationValue
  * @retval Returned value is the PMOS compensation cell
  */
__STATIC_INLINE uint32_t LL_SBS_GetPMOSVddIO2CompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_APSRC2));
}

/**
  * @brief  Get the compensation cell value of the GPIO NMOS transistor supplied by VDDIO2
  * @rmtoll CCVALR    ANSRC2   LL_SBS_GetNMOSVddIO2CompensationValue
  * @retval Returned value is the NMOS compensation cell
  */
__STATIC_INLINE uint32_t LL_SBS_GetNMOSVddIO2CompensationValue(void)
{
  return (uint32_t)(READ_BIT(SBS->CCVALR, SBS_CCVALR_ANSRC2));
}

/**
  * @brief  Set the compensation cell code of the GPIO PMOS transistor supplied by VDD
  * @rmtoll CCSWCR    SW_APSRC1  LL_SBS_SetPMOSVddCompensationCode
  * @param  PMOSCode PMOS compensation code
  *         This code is applied to the PMOS compensation cell when the CS1 bit of the
  *         SBS_CCCSR is set
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetPMOSVddCompensationCode(uint32_t PMOSCode)
{
  MODIFY_REG(SBS->CCSWCR, SBS_CCSWCR_SW_APSRC1, PMOSCode << SBS_CCSWCR_SW_APSRC1_Pos);
}

/**
  * @brief  Get the compensation cell code of the GPIO PMOS transistor supplied by VDD
  * @rmtoll CCSWCR    SW_APSRC1   LL_SBS_GetPMOSVddCompensationCode
  * @retval Returned value is the PMOS compensation cell
  */
__STATIC_INLINE uint32_t LL_SBS_GetPMOSVddCompensationCode(void)
{
  return (uint32_t)(READ_BIT(SBS->CCSWCR, SBS_CCSWCR_SW_APSRC1));
}

/**
  * @brief  Set the compensation cell code of the GPIO PMOS transistor supplied by VDDIO
  * @rmtoll CCSWCR    SW_APSRC2  LL_SBS_SetPMOSVddIOCompensationCode
  * @param  PMOSCode PMOS compensation code
  *         This code is applied to the PMOS compensation cell when the CS2 bit of the
  *         SBS_CCCSR is set
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetPMOSVddIOCompensationCode(uint32_t PMOSCode)
{
  MODIFY_REG(SBS->CCSWCR, SBS_CCSWCR_SW_APSRC2, PMOSCode << SBS_CCSWCR_SW_APSRC2_Pos);
}

/**
  * @brief  Get the compensation cell code of the GPIO PMOS transistor supplied by VDDIO
  * @rmtoll CCSWCR    SW_APSRC2   LL_SBS_GetPMOSVddIOCompensationCode
  * @retval Returned value is the PMOS compensation
  */
__STATIC_INLINE uint32_t LL_SBS_GetPMOSVddIOCompensationCode(void)
{
  return (uint32_t)(READ_BIT(SBS->CCSWCR, SBS_CCSWCR_SW_APSRC2));
}

/**
  * @brief  Set the compensation cell code of the GPIO NMOS transistor supplied by VDD
  * @rmtoll CCSWCR    PCC2  LL_SBS_SetNMOSVddCompensationCode
  * @param  NMOSCode NMOS compensation code
  *         This code is applied to the NMOS compensation cell when the CS2 bit of the
  *         SBS_CCCSR is set
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetNMOSVddCompensationCode(uint32_t NMOSCode)
{
  MODIFY_REG(SBS->CCSWCR, SBS_CCSWCR_SW_ANSRC1, NMOSCode << SBS_CCSWCR_SW_ANSRC1_Pos);
}

/**
  * @brief  Get the compensation cell code of the GPIO NMOS transistor supplied by VDD
  * @rmtoll CCSWCR    NCC1   LL_SBS_GetNMOSVddCompensationCode
  * @retval Returned value is the Vdd compensation cell code for NMOS transistors
  */
__STATIC_INLINE uint32_t LL_SBS_GetNMOSVddCompensationCode(void)
{
  return (uint32_t)(READ_BIT(SBS->CCSWCR, SBS_CCSWCR_SW_ANSRC1));
}

/**
  * @brief  Set the compensation cell code of the GPIO NMOS transistor supplied by VDDIO
  * @rmtoll CCSWCR    NCC2  LL_SBS_SetNMOSVddIOCompensationCode
  * @param  NMOSCode NMOS compensation cell code
  *         This code is applied to the NMOS compensation cell when the CS2 bit of the
  *         SBS_CCCSR is set
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetNMOSVddIOCompensationCode(uint32_t NMOSCode)
{
  MODIFY_REG(SBS->CCSWCR, SBS_CCSWCR_SW_ANSRC2, NMOSCode << SBS_CCSWCR_SW_ANSRC2_Pos);
}


/**
  * @brief  Get the compensation cell code of the GPIO NMOS transistor supplied by VDDIO
  * @rmtoll CCSWCR    NCC2   LL_SBS_GetNMOSVddIOCompensationCode
  * @retval Returned value is the NMOS compensation cell code
  */
__STATIC_INLINE uint32_t LL_SBS_GetNMOSVddIOCompensationCode(void)
{
  return (uint32_t)(READ_BIT(SBS->CCSWCR, SBS_CCSWCR_SW_ANSRC2));
}

/**
  * @brief  Enable the Compensation Cell of GPIO supplied by VDD
  * @rmtoll CCCSR   EN1    LL_SBS_EnableVddCompensationCell
  * @note   The vdd compensation cell can be used only when the device supply
  *         voltage ranges from 1.71 to 3.6 V
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableVddCompensationCell(void)
{
  SET_BIT(SBS->CCCSR, SBS_CCCSR_EN1);
}

/**
  * @brief  Enable the Compensation Cell of GPIO supplied by VDDIO
  * @rmtoll CCCSR   EN2    LL_SBS_EnableVddIOCompensationCell
  * @note   The Vdd I/O compensation cell can be used only when the device supply
  *         voltage ranges from 1.08 to 3.6 V
  * @retval None
  */
__STATIC_INLINE void LL_SBS_EnableVddIOCompensationCell(void)
{
  SET_BIT(SBS->CCCSR, SBS_CCCSR_EN2);
}

/**
  * @brief  Disable the Compensation Cell of GPIO supplied by VDD
  * @rmtoll CCCSR   EN1    LL_SBS_DisableVddCompensationCell
  * @note   The Vdd compensation cell can be used only when the device supply
  *         voltage ranges from 1.71 to 3.6 V
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableVddCompensationCell(void)
{
  CLEAR_BIT(SBS->CCCSR, SBS_CCCSR_EN1);
}

/**
  * @brief  Disable the Compensation Cell of GPIO supplied by VDDIO
  * @rmtoll CCCSR   EN2    LL_SBS_DisableVddIOCompensationCell
  * @note   The Vdd I/O compensation cell can be used only when the device supply
  *         voltage ranges from 1.08 to 3.6 V
  * @retval None
  */
__STATIC_INLINE void LL_SBS_DisableVddIOCompensationCell(void)
{
  CLEAR_BIT(SBS->CCCSR, SBS_CCCSR_EN2);
}

/**
  * @brief  Check if the Compensation Cell of GPIO supplied by VDD is enable
  * @rmtoll CCCSR   EN1    LL_SBS_IsEnabled_VddCompensationCell
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabled_VddCompensationCell(void)
{
  return ((READ_BIT(SBS->CCCSR, SBS_CCCSR_EN1) == SBS_CCCSR_EN1) ? 1UL : 0UL);
}

/**
  * @brief  Check if the Compensation Cell of GPIO supplied by VDDIO is enable
  * @rmtoll CCCSR   EN2   LL_SBS_IsEnabled_VddIOCompensationCell
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsEnabled_VddIOCompensationCell(void)
{
  return ((READ_BIT(SBS->CCCSR, SBS_CCCSR_EN2) == SBS_CCCSR_EN2) ? 1UL : 0UL);
}

/**
  * @brief  Get Compensation Cell ready Flag of GPIO supplied by VDD
  * @rmtoll CCCSR   RDY1   LL_SBS_IsActiveFlag_VddCMPCR
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsActiveFlag_VddCMPCR(void)
{
  return ((READ_BIT(SBS->CCCSR, SBS_CCCSR_RDY1) == (SBS_CCCSR_RDY1)) ? 1UL : 0UL);
}

/**
  * @brief  Get Compensation Cell ready Flag of GPIO supplied by VDDIO
  * @rmtoll CCCSR   RDY2   LL_SBS_IsActiveFlag_VddIOCMPCR
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_SBS_IsActiveFlag_VddIOCMPCR(void)
{
  return ((READ_BIT(SBS->CCCSR, SBS_CCCSR_RDY2) == (SBS_CCCSR_RDY2)) ? 1UL : 0UL);
}


/**
  * @brief  Set the compensation cell code selection of GPIO supplied by VDD
  * @rmtoll CCCSR   CS1    LL_SBS_SetVddCellCompensationCode
  * @param  CompCode: Selects the code to be applied for the Vdd compensation cell
  *   This parameter can be one of the following values:
  *   @arg LL_SBS_VDD_CELL_CODE : Select Code from the cell (available in the SBS_CCVALR)
  *   @arg LL_SBS_VDD_REGISTER_CODE: Select Code from the SBS compensation cell code register (SBS_CCSWCR)
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetVddCellCompensationCode(uint32_t CompCode)
{
  SET_BIT(SBS->CCCSR, CompCode);
}

/**
  * @brief  Set the compensation cell code selection of GPIO supplied by VDDIO
  * @rmtoll CCCSR   CS2    LL_SBS_SetVddIOCellCompensationCode
  * @param  CompCode: Selects the code to be applied for the VddIO compensation cell
  *   This parameter can be one of the following values:
  *   @arg LL_SBS_VDDIO_CELL_CODE : Select Code from the cell (available in the SBS_CCVALR)
  *   @arg LL_SBS_VDDIO_REGISTER_CODE: Select Code from the SBS compensation cell code register (SBS_CCSWCR)
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetVddIOCellCompensationCode(uint32_t CompCode)
{
  SET_BIT(SBS->CCCSR, CompCode);
}

/**
  * @brief  Get the compensation cell code selection of GPIO supplied by VDD
  * @rmtoll CCCSR   CS1    LL_SBS_GetVddCellCompensationCode
  * @retval Returned value can be one of the following values:
  *   @arg LL_SBS_VDD_CELL_CODE : Selected Code is from the cell (available in the SBS_CCVALR)
  *   @arg LL_SBS_VDD_REGISTER_CODE: Selected Code is from the SBS compensation cell code register (SBS_CCSWCR)
  */
__STATIC_INLINE uint32_t LL_SBS_GetVddCellCompensationCode(void)
{
  return (uint32_t)(READ_BIT(SBS->CCCSR, SBS_CCCSR_CS1));
}

/**
  * @brief  Get the compensation cell code selection of GPIO supplied by VDDIO
  * @rmtoll CCCSR   CS2    LL_SBS_GetVddIOCellCompensationCode
  * @retval Returned value can be one of the following values:
  *   @arg LL_SBS_VDDIO_CELL_CODE : Selected Code is from the cell (available in the SBS_CCVALR)
  *   @arg LL_SBS_VDDIO_REGISTER_CODE: Selected Code is from the SBS compensation cell code register (SBS_CCSWCR)
  */
__STATIC_INLINE uint32_t LL_SBS_GetVddIOCellCompensationCode(void)
{
  return (uint32_t)(READ_BIT(SBS->CCCSR, SBS_CCCSR_CS2));
}

#if defined(SBS_OTGHSPHYTUNER2_COMPDISTUNE)
/**
  * @brief  Set the OTG high-speed PHY disconnect threshold adjustment.
  * @rmtoll SBS_OTGHSPHYTUNER2   COMPDISTUNE   LL_SBS_SetOTGPHYDisconnectThresholdAdjustment
  * @param  DisconnectThreshold This parameter can be one of the following values:
  *         @arg @ref LL_SBS_OTGHSPHY_DISCONNECT_5_9PERCENT
  *         @arg @ref LL_SBS_OTGHSPHY_DISCONNECT_0PERCENT
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetOTGPHYDisconnectThresholdAdjustment(uint32_t DisconnectThreshold)
{
  MODIFY_REG(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_COMPDISTUNE, DisconnectThreshold);
}

/**
  * @brief  Get the OTG high-speed PHY disconnect threshold adjustment.
  * @rmtoll SBS_OTGHSPHYTUNER2   COMPDISTUNE   LL_SBS_GetOTGPHYDisconnectThresholdAdjustment
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SBS_OTGHSPHY_DISCONNECT_5_9PERCENT
  *         @arg @ref LL_SBS_OTGHSPHY_DISCONNECT_0PERCENT
  */
__STATIC_INLINE uint32_t LL_SBS_GetOTGPHYDisconnectThresholdAdjustment(void)
{
  return (uint32_t)(READ_BIT(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_COMPDISTUNE));
}
#endif /* SBS_OTGHSPHYTUNER2_COMPDISTUNE */

#if defined(SBS_OTGHSPHYTUNER2_SQRXTUNE)
/**
  * @brief  Set the voltage level for the threshold used to detect valid high-speed data.
  * @rmtoll SBS_OTGHSPHYTUNER2   SQRXTUNE   LL_SBS_SetOTGPHYSquelchThresholdAdjustment
  * @param  SquelchThreshold This parameter can be one of the following values:
  *         @arg @ref LL_SBS_OTGHSPHY_SQUELCH_15PERCENT
  *         @arg @ref LL_SBS_OTGHSPHY_SQUELCH_0PERCENT
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetOTGPHYSquelchThresholdAdjustment(uint32_t SquelchThreshold)
{
  MODIFY_REG(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_SQRXTUNE, SquelchThreshold);
}

/**
  * @brief  Get the voltage level for the threshold used to detect valid high-speed data.
  * @rmtoll SBS_OTGHSPHYTUNER2   SQRXTUNE   LL_SBS_GetOTGPHYSquelchThresholdAdjustment
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SBS_OTGHSPHY_SQUELCH_15PERCENT
  *         @arg @ref LL_SBS_OTGHSPHY_SQUELCH_0PERCENT
  */
__STATIC_INLINE uint32_t LL_SBS_GetOTGPHYSquelchThresholdAdjustment(void)
{
  return (uint32_t)(READ_BIT(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_SQRXTUNE));
}
#endif /* SBS_OTGHSPHYTUNER2_SQRXTUNE */

#if defined(SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE)
/**
  * @brief  Set the OTG high-speed PHY transmitter preemphasis current control.
  * @rmtoll SBS_OTGHSPHYTUNER2   TXPREEMPAMPTUNE   LL_SBS_SetOTGPHYTransmitterPreemphasisCurrent
  * @param  PreemphasisCurrent This parameter can be one of the following values:
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_DISABLED
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_1X
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_2X
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_3X
  * @retval None
  */
__STATIC_INLINE void LL_SBS_SetOTGPHYTransmitterPreemphasisCurrent(uint32_t PreemphasisCurrent)
{
  MODIFY_REG(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE, PreemphasisCurrent);
}

/**
  * @brief  Get the OTG high-speed PHY transmitter preemphasis current control.
  * @rmtoll SBS_OTGHSPHYTUNER2   TXPREEMPAMPTUNE   LL_SBS_GetOTGPHYTransmitterPreemphasisCurrent
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_DISABLED
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_1X
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_2X
  *         @arg @ref LL_SBS_OTGHSPHY_PREEMP_3X
  */
__STATIC_INLINE uint32_t LL_SBS_GetOTGPHYTransmitterPreemphasisCurrent(void)
{
  return (uint32_t)(READ_BIT(SBS->OTGHSPHYTUNER2, SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE));
}
#endif /* SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE */

/**
  * @}
  */

/** @defgroup SYSTEM_LL_DBGMCU_EF DBGMCU
  * @{
  */

/**
  * @brief  Return the device identifier
  * @rmtoll DBGMCU_IDCODE DEV_ID        LL_DBGMCU_GetDeviceID
  * @retval Values between Min_Data=0x00 and Max_Data=0xFFFF (ex: device ID is 0x6415)
  */
__STATIC_INLINE uint32_t LL_DBGMCU_GetDeviceID(void)
{
  return (uint32_t)(READ_BIT(DBGMCU->IDCODE, DBGMCU_IDCODE_DEV_ID));
}

/**
  * @brief  Return the device revision identifier
  * @note This field indicates the revision of the device.
  * @rmtoll DBGMCU_IDCODE REV_ID        LL_DBGMCU_GetRevisionID
  * @retval Values between Min_Data=0x00 and Max_Data=0xFFFF
  */
__STATIC_INLINE uint32_t LL_DBGMCU_GetRevisionID(void)
{
  return (uint32_t)(READ_BIT(DBGMCU->IDCODE, DBGMCU_IDCODE_REV_ID) >> DBGMCU_IDCODE_REV_ID_Pos);
}

/**
  * @brief  Enable the Debug Module during STOP mode
  * @rmtoll DBGMCU_CR    DBG_STOP      LL_DBGMCU_EnableDBGStopMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_EnableDBGStopMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief  Disable the Debug Module during STOP mode
  * @rmtoll DBGMCU_CR    DBG_STOP      LL_DBGMCU_DisableDBGStopMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_DisableDBGStopMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STOP);
}

/**
  * @brief  Enable the Debug Module during STANDBY mode
  * @rmtoll DBGMCU_CR    DBG_STANDBY   LL_DBGMCU_EnableDBGStandbyMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_EnableDBGStandbyMode(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}

/**
  * @brief  Disable the Debug Module during STANDBY mode
  * @rmtoll DBGMCU_CR    DBG_STANDBY   LL_DBGMCU_DisableDBGStandbyMode
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_DisableDBGStandbyMode(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_DBG_STANDBY);
}


/**
  * @brief  Enable the Debug Clock Trace
  * @rmtoll DBGMCU_CR    TRACE_CLKEN   LL_DBGMCU_EnableTraceClock
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_EnableTraceClock(void)
{
  SET_BIT(DBGMCU->CR, DBGMCU_CR_TRACE_CLKEN);
}

/**
  * @brief  Disable the Debug Clock Trace
  * @rmtoll DBGMCU_CR    TRACE_CLKEN   LL_DBGMCU_DisableTraceClock
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_DisableTraceClock(void)
{
  CLEAR_BIT(DBGMCU->CR, DBGMCU_CR_TRACE_CLKEN);
}


/**
  * @brief  Check if clock trace is enabled or disabled.
  * @rmtoll DBGMCU_CR_TRACE_CLKEN      LL_DBGMCU_IsEnabledTraceClock
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_DBGMCU_IsEnabledTraceClock(void)
{
  return ((READ_BIT(DBGMCU->CR, DBGMCU_CR_TRACE_CLKEN) == DBGMCU_CR_TRACE_CLKEN) ? 1UL : 0UL);
}

/**
  * @brief  Set Trace pin assignment control
  * @rmtoll DBGMCU_CR    TRACE_IOEN    LL_DBGMCU_SetTracePinAssignment\n
  *         DBGMCU_CR    TRACE_MODE    LL_DBGMCU_SetTracePinAssignment
  * @param  PinAssignment This parameter can be one of the following values:
  *         @arg @ref LL_DBGMCU_TRACE_NONE
  *         @arg @ref LL_DBGMCU_TRACE_ASYNCH
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE1
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE2
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE4
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_SetTracePinAssignment(uint32_t PinAssignment)
{
  MODIFY_REG(DBGMCU->CR, DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE, PinAssignment);
}

/**
  * @brief  Get Trace pin assignment control
  * @rmtoll DBGMCU_CR    TRACE_IOEN    LL_DBGMCU_GetTracePinAssignment\n
  *         DBGMCU_CR    TRACE_MODE    LL_DBGMCU_GetTracePinAssignment
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_DBGMCU_TRACE_NONE
  *         @arg @ref LL_DBGMCU_TRACE_ASYNCH
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE1
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE2
  *         @arg @ref LL_DBGMCU_TRACE_SYNCH_SIZE4
  */
__STATIC_INLINE uint32_t LL_DBGMCU_GetTracePinAssignment(void)
{
  return (uint32_t)(READ_BIT(DBGMCU->CR, DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE));
}

/**
  * @brief  Freeze APB1 peripherals (group1 peripherals)
  * @rmtoll DBGMCU_APB1FZR1 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP1_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM3_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM4_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM5_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM6_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM7_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM12_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM13_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM14_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_WWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_IWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C1_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I3C1_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP1_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB1FZR1, Periphs);
}

/**
  * @brief  Freeze APB1 peripherals (group2 peripherals)
  * @rmtoll DBGMCU_APB1FZR2 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP2_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP2_LPTIM2_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP2_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB1FZR2, Periphs);
}

/**
  * @brief  Unfreeze APB1 peripherals (group1 peripherals)
  * @rmtoll DBGMCU_APB1FZR1 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP1_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM3_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM4_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM5_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM6_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM7_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM12_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM13_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_TIM14_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_WWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_IWDG_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C1_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I2C2_STOP
  *         @arg @ref LL_DBGMCU_APB1_GRP1_I3C1_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP1_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB1FZR1, Periphs);
}

/**
  * @brief  Unfreeze APB1 peripherals (group2 peripherals)
  * @rmtoll DBGMCU_APB1FZR2 DBG_xxxx_STOP  LL_DBGMCU_APB1_GRP2_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB1_GRP2_LPTIM2_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB1_GRP2_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB1FZR2, Periphs);
}

/**
  * @brief  Freeze APB2 peripherals
  * @rmtoll DBGMCU_APB2FZ DBG_TIMx_STOP  LL_DBGMCU_APB2_GRP1_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM1_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM8_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM15_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM16_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM17_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB2_GRP1_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB2FZR, Periphs);
}

/**
  * @brief  Unfreeze APB2 peripherals
  * @rmtoll DBGMCU_APB2FZR DBG_TIMx_STOP  LL_DBGMCU_APB2_GRP1_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM1_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM8_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM15_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM16_STOP
  *         @arg @ref LL_DBGMCU_APB2_GRP1_TIM17_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB2_GRP1_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB2FZR, Periphs);
}

/**
  * @brief  Freeze APB3 peripherals
  * @rmtoll DBGMCU_APB3FZ DBG_TIMx_STOP  LL_DBGMCU_APB3_GRP1_FreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB3_GRP1_I2C3_STOP
  *         @arg @ref LL_DBGMCU_APB3_GRP1_I2C4_STOP
  *         @arg @ref LL_DBGMCU_APB3_GRP1_LPTIM1_STOP
  *         @arg @ref LL_DBGMCU_APB3_GRP1_RTC_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB3_GRP1_FreezePeriph(uint32_t Periphs)
{
  SET_BIT(DBGMCU->APB3FZR, Periphs);
}

/**
  * @brief  Unfreeze APB3 peripherals
  * @rmtoll DBGMCU_APB3FZR DBG_TIMx_STOP  LL_DBGMCU_APB3_GRP1_UnFreezePeriph
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_DBGMCU_APB3_GRP1_I2C3_STOP
  *         @arg @ref LL_DBGMCU_APB3_GRP1_I2C4_STOP
  *         @arg @ref LL_DBGMCU_APB3_GRP1_LPTIM1_STOP
  *         @arg @ref LL_DBGMCU_APB3_GRP1_RTC_STOP
  * @retval None
  */
__STATIC_INLINE void LL_DBGMCU_APB3_GRP1_UnFreezePeriph(uint32_t Periphs)
{
  CLEAR_BIT(DBGMCU->APB3FZR, Periphs);
}

/**
  * @}
  */

#if defined(VREFBUF)
/** @defgroup SYSTEM_LL_VREFBUF_EF VREFBUF
  * @{
  */

/**
  * @brief  Enable Internal voltage reference
  * @rmtoll VREFBUF_CSR  ENVR          LL_VREFBUF_Enable
  * @retval None
  */
__STATIC_INLINE void LL_VREFBUF_Enable(void)
{
  SET_BIT(VREFBUF->CSR, VREFBUF_CSR_ENVR);
}

/**
  * @brief  Disable Internal voltage reference
  * @rmtoll VREFBUF_CSR  ENVR          LL_VREFBUF_Disable
  * @retval None
  */
__STATIC_INLINE void LL_VREFBUF_Disable(void)
{
  CLEAR_BIT(VREFBUF->CSR, VREFBUF_CSR_ENVR);
}

/**
  * @brief  Enable high impedance (VREF+pin is high impedance)
  * @rmtoll VREFBUF_CSR  HIZ           LL_VREFBUF_EnableHIZ
  * @retval None
  */
__STATIC_INLINE void LL_VREFBUF_EnableHIZ(void)
{
  SET_BIT(VREFBUF->CSR, VREFBUF_CSR_HIZ);
}

/**
  * @brief  Disable high impedance (VREF+pin is internally connected to the voltage reference buffer output)
  * @rmtoll VREFBUF_CSR  HIZ           LL_VREFBUF_DisableHIZ
  * @retval None
  */
__STATIC_INLINE void LL_VREFBUF_DisableHIZ(void)
{
  CLEAR_BIT(VREFBUF->CSR, VREFBUF_CSR_HIZ);
}

/**
  * @brief  Set the Voltage reference scale
  * @rmtoll VREFBUF_CSR  VRS           LL_VREFBUF_SetVoltageScaling
  * @param  Scale This parameter can be one of the following values:
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE0
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE1
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE2
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE3
  * @retval None
  */
__STATIC_INLINE void LL_VREFBUF_SetVoltageScaling(uint32_t Scale)
{
  MODIFY_REG(VREFBUF->CSR, VREFBUF_CSR_VRS, Scale);
}

/**
  * @brief  Get the Voltage reference scale
  * @rmtoll VREFBUF_CSR  VRS           LL_VREFBUF_GetVoltageScaling
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE0
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE1
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE2
  *         @arg @ref LL_VREFBUF_VOLTAGE_SCALE3
  */
__STATIC_INLINE uint32_t LL_VREFBUF_GetVoltageScaling(void)
{
  return (uint32_t)(READ_BIT(VREFBUF->CSR, VREFBUF_CSR_VRS));
}

/**
  * @brief  Check if Voltage reference buffer is ready
  * @rmtoll VREFBUF_CSR  VRR           LL_VREFBUF_IsVREFReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_VREFBUF_IsVREFReady(void)
{
  return ((READ_BIT(VREFBUF->CSR, VREFBUF_CSR_VRR) == VREFBUF_CSR_VRR) ? 1UL : 0UL);
}

/**
  * @brief  Get the trimming code for VREFBUF calibration
  * @rmtoll VREFBUF_CCR  TRIM          LL_VREFBUF_GetTrimming
  * @retval Between 0 and 0x3F
  */
__STATIC_INLINE uint32_t LL_VREFBUF_GetTrimming(void)
{
  return (uint32_t)(READ_BIT(VREFBUF->CCR, VREFBUF_CCR_TRIM));
}

/**
  * @brief  Set the trimming code for VREFBUF calibration (Tune the internal reference buffer voltage)
  * @rmtoll VREFBUF_CCR  TRIM          LL_VREFBUF_SetTrimming
  * @param  Value Between 0 and 0x3F
  * @retval None
  */
__STATIC_INLINE void LL_VREFBUF_SetTrimming(uint32_t Value)
{
  WRITE_REG(VREFBUF->CCR, Value);
}

/**
  * @}
  */
#endif /* VREFBUF */

/** @defgroup SYSTEM_LL_FLASH_EF FLASH
  * @{
  */
/**
  * @brief  Set FLASH Latency
  * @rmtoll FLASH_ACR    LATENCY       LL_FLASH_SetLatency
  * @param  Latency This parameter can be one of the following values:
  *         @arg @ref LL_FLASH_LATENCY_0
  *         @arg @ref LL_FLASH_LATENCY_1
  *         @arg @ref LL_FLASH_LATENCY_2
  *         @arg @ref LL_FLASH_LATENCY_3
  *         @arg @ref LL_FLASH_LATENCY_4
  *         @arg @ref LL_FLASH_LATENCY_5
  *         @arg @ref LL_FLASH_LATENCY_6
  *         @arg @ref LL_FLASH_LATENCY_7
  *         @arg @ref LL_FLASH_LATENCY_8
  *         @arg @ref LL_FLASH_LATENCY_9
  *         @arg @ref LL_FLASH_LATENCY_10
  *         @arg @ref LL_FLASH_LATENCY_11
  *         @arg @ref LL_FLASH_LATENCY_12
  *         @arg @ref LL_FLASH_LATENCY_13
  *         @arg @ref LL_FLASH_LATENCY_14
  *         @arg @ref LL_FLASH_LATENCY_15
  * @retval None
  */
__STATIC_INLINE void LL_FLASH_SetLatency(uint32_t Latency)
{
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, Latency);
}

/**
  * @brief  Get FLASH Latency
  * @rmtoll FLASH_ACR    LATENCY       LL_FLASH_GetLatency
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_FLASH_LATENCY_0
  *         @arg @ref LL_FLASH_LATENCY_1
  *         @arg @ref LL_FLASH_LATENCY_2
  *         @arg @ref LL_FLASH_LATENCY_3
  *         @arg @ref LL_FLASH_LATENCY_4
  *         @arg @ref LL_FLASH_LATENCY_5
  *         @arg @ref LL_FLASH_LATENCY_6
  *         @arg @ref LL_FLASH_LATENCY_7
  *         @arg @ref LL_FLASH_LATENCY_8
  *         @arg @ref LL_FLASH_LATENCY_9
  *         @arg @ref LL_FLASH_LATENCY_10
  *         @arg @ref LL_FLASH_LATENCY_11
  *         @arg @ref LL_FLASH_LATENCY_12
  *         @arg @ref LL_FLASH_LATENCY_13
  *         @arg @ref LL_FLASH_LATENCY_14
  *         @arg @ref LL_FLASH_LATENCY_15
  */
__STATIC_INLINE uint32_t LL_FLASH_GetLatency(void)
{
  return (uint32_t)(READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY));
}

/**
  * @}
  */


/** @defgroup SYSTEM_LL_SBS_EF_ERASE_MEMORY_STATUS_CLEAR  Erase Memory Status
  * @{
  */

/**
  * @brief  Clear Status of End of Erase for ICACHE and PKA RAMs
  * @rmtoll MESR   IPMEE    LL_SBS_ClearEraseEndStatus
  * @retval None
  */
__STATIC_INLINE void LL_SBS_ClearEraseEndStatus(void)
{
  WRITE_REG(SBS->MESR, SBS_MESR_IPMEE);
}

/**
  * @brief  Get Status of End of Erase for ICACHE and PKA RAMs
  * @rmtoll MESR   IPMEE    LL_SBS_GetEraseEndStatus
  * @retval Returned value can be one of the following values:
  *   @arg LL_SBS_MEMORIES_ERASE_IPMEE_ON_GOING : Erase of ICACHE and PKA RAMs on going or flag cleared by SW
  *   @arg LL_SBS_MEMORIES_ERASE_IPMEE_ENDED: Erase of ICACHE and PKA RAMs ended
  */
__STATIC_INLINE uint32_t LL_SBS_GetEraseEndStatus(void)
{
  return (uint32_t)(READ_BIT(SBS->MESR, SBS_MESR_IPMEE));
}

/**
  * @brief  Clear Status of End of Erase after Power-on Reset for SRAM2, BKPRAM, ICACHE, DCACHE and PKA RAMs
  * @rmtoll MESR   MCLR    LL_SBS_ClearEraseAfterResetStatus
  * @retval None
  */
__STATIC_INLINE void LL_SBS_ClearEraseAfterResetStatus(void)
{
  WRITE_REG(SBS->MESR, SBS_MESR_MCLR);
}

/**
  * @brief  Get Status of End of Erase after Power-on Reset for SRAM2, BKPRAM, ICACHE, DCACHE and PKA RAMs
  * @rmtoll MESR   MCLR    LL_SBS_GetEraseAfterResetStatus
  * @retval Returned value can be one of the following values:
  *   @arg LL_SBS_MEMORIES_ERASE_MCLR_ON_GOING : Erase of memories on going or flag cleared by SW
  *   @arg LL_SBS_MEMORIES_ERASE_MCLR_ENDED: Erase of memories ended
  */
__STATIC_INLINE uint32_t LL_SBS_GetEraseAfterResetStatus(void)
{
  return (uint32_t)(READ_BIT(SBS->MESR, SBS_MESR_MCLR));
}
/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */

#endif /* defined (FLASH) || defined (SBS) || defined (DBGMCU) || defined (VREFBUF) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H5xx_LL_SYSTEM_H */

