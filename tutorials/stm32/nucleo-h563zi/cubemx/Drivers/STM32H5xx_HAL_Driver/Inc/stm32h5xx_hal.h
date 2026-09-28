/**
  ******************************************************************************
  * @file    stm32h5xx_hal.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the HAL
  *          module driver.
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
#ifndef __STM32H5xx_HAL_H
#define __STM32H5xx_HAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_conf.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup HAL
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup HAL_Exported_Types HAL Exported Types
  * @{
  */

/** @defgroup HAL_TICK_FREQ Tick Frequency
  * @{
  */
typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;
/**
  * @}
  */

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/** @defgroup HAL_Exported_Variables HAL Exported Variables
  * @{
  */
extern __IO uint32_t            uwTick;
extern uint32_t                 uwTickPrio;
extern HAL_TickFreqTypeDef      uwTickFreq;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup SBS_Exported_Constants SBS Exported Constants
  * @{
  */

/** @defgroup SBS_FPU_Interrupts FPU Interrupts
  * @{
  */
#define SBS_IT_FPU_IOC              SBS_FPUIMR_FPU_IE_0  /*!< Floating Point Unit Invalid operation Interrupt */
#define SBS_IT_FPU_DZC              SBS_FPUIMR_FPU_IE_1  /*!< Floating Point Unit Divide-by-zero Interrupt */
#define SBS_IT_FPU_UFC              SBS_FPUIMR_FPU_IE_2  /*!< Floating Point Unit Underflow Interrupt */
#define SBS_IT_FPU_OFC              SBS_FPUIMR_FPU_IE_3  /*!< Floating Point Unit Overflow Interrupt */
#define SBS_IT_FPU_IDC              SBS_FPUIMR_FPU_IE_4  /*!< Floating Point Unit Input denormal Interrupt */
#define SBS_IT_FPU_IXC              SBS_FPUIMR_FPU_IE_5  /*!< Floating Point Unit Inexact Interrupt */

/**
  * @}
  */

/** @defgroup SBS_BREAK_CONFIG SBS Break Config
  * @{
  */
#define SBS_BREAK_FLASH_ECC         SBS_CFGR2_ECCL  /*!< Enable and lock the FLASH ECC double error with TIM1/8/15/16/17
                                                         Break inputs.*/
#define SBS_BREAK_PVD               SBS_CFGR2_PVDL  /*!< Enable and lock the PVD connection with TIM1/8/15/16/17
                                                         Break inputs. */
#define SBS_BREAK_SRAM_ECC          SBS_CFGR2_SEL   /*!< Enable and lock the SRAM ECC double error signal with
                                                         TIM1/8/15/16/17 Break inputs.*/
#define SBS_BREAK_LOCKUP            SBS_CFGR2_CLL   /*!< Enable and lock the connection of Cortex-M33 LOCKUP (hardfault)
                                                         output to TIM1/8/15/16/17 Break inputs.*/

/**
  * @}
  */

#if defined(VREFBUF)
/** @defgroup VREFBUF_VoltageScale VREFBUF Voltage Scale
  * @{
  */
#define VREFBUF_VOLTAGE_SCALE0    ((uint32_t)0x00000000)                   /*!< Voltage reference scale 0 (VREF_OUT1) */
#define VREFBUF_VOLTAGE_SCALE1    VREFBUF_CSR_VRS_0                        /*!< Voltage reference scale 1 (VREF_OUT2) */
#define VREFBUF_VOLTAGE_SCALE2    VREFBUF_CSR_VRS_1                        /*!< Voltage reference scale 2 (VREF_OUT3) */
#define VREFBUF_VOLTAGE_SCALE3    (VREFBUF_CSR_VRS_0 | VREFBUF_CSR_VRS_1)  /*!< Voltage reference scale 3 (VREF_OUT4) */

/**
  * @}
  */

/** @defgroup VREFBUF_HighImpedance VREFBUF High Impedance
  * @{
  */
#define VREFBUF_HIGH_IMPEDANCE_DISABLE  ((uint32_t)0x00000000)             /*!< VREF_plus pin is internally connected to
                                                                                Voltage reference buffer output */
#define VREFBUF_HIGH_IMPEDANCE_ENABLE   VREFBUF_CSR_HIZ                    /*!< VREF_plus pin is high impedance */

/**
  * @}
  */
#endif /* VREFBUF */

/** @defgroup SBS_FastModePlus_GPIO Fast-mode Plus on GPIO
  * @{
  */

/** @brief  Fast-mode Plus driving capability on a specific GPIO
  */
#define SBS_FASTMODEPLUS_PB6        SBS_PMCR_PB6_FMP  /*!< Enable Fast-mode Plus on PB6 */
#define SBS_FASTMODEPLUS_PB7        SBS_PMCR_PB7_FMP  /*!< Enable Fast-mode Plus on PB7 */
#define SBS_FASTMODEPLUS_PB8        SBS_PMCR_PB8_FMP  /*!< Enable Fast-mode Plus on PB8 */
#if defined(SBS_PMCR_PB9_FMP)
#define SBS_FASTMODEPLUS_PB9        SBS_PMCR_PB9_FMP  /*!< Enable Fast-mode Plus on PB9 */
#endif /* SBS_PMCR_PB9_FMP */

/**
  * @}
  */

#if defined(SBS_PMCR_ETH_SEL_PHY)
/** @defgroup SBS_Ethernet_Config  Ethernet Config
  * @{
  */
#define SBS_ETH_MII             ((uint32_t)0x00000000)     /*!< Select the Media Independent Interface (MII) or GMII  */
#define SBS_ETH_RMII            SBS_PMCR_ETH_SEL_PHY_2     /*!< Select the Reduced Media Independent Interface (RMII) */

#define IS_SBS_ETHERNET_CONFIG(CONFIG) (((CONFIG) == SBS_ETH_MII)        || \
                                        ((CONFIG) == SBS_ETH_RMII))


/**
  * @}
  */
#endif /* SBS_PMCR_ETH_SEL_PHY */

/** @defgroup SBS_Memories_Erase_Flag_Status  Memory Erase Flags Status
  * @{
  */
#define SBS_MEMORIES_ERASE_FLAG_IPMEE     SBS_MESR_IPMEE    /*!< Select the Status of End Of Erase for ICACHE
                                                                 and PKA RAMs */
#define SBS_MEMORIES_ERASE_FLAG_MCLR      SBS_MESR_MCLR     /*!< Select the Status of Erase after Power-on Reset
                                                                (SRAM2, BKPRAM, ICACHE, DCACHE, PKA rams) */

#define IS_SBS_MEMORIES_ERASE_FLAG(FLAG) (((FLAG) == SBS_MEMORIES_ERASE_FLAG_IPMEE)        || \
                                          ((FLAG) == SBS_MEMORIES_ERASE_FLAG_MCLR))

/**
  * @}
  */

/** @defgroup SBS_IOCompenstionCell_Config  IOCompenstionCell Config
  * @{
  */
#define SBS_VDD_CELL_CODE                ((uint32_t)0x00000000)  /*!< Select Code from the cell */
#define SBS_VDD_REGISTER_CODE             SBS_CCCSR_CS1        /*!< Code from the SBS compensation cell code register */

#define IS_SBS_VDD_CODE_SELECT(SELECT)   (((SELECT) == SBS_VDD_CELL_CODE)|| \
                                          ((SELECT) == SBS_VDD_REGISTER_CODE))

#define SBS_VDDIO_CELL_CODE              ((uint32_t)0x00000000)  /*!< Select Code from the cell */
#define SBS_VDDIO_REGISTER_CODE           SBS_CCCSR_CS2        /*!< Code from the SBS compensation cell code register */

#define IS_SBS_VDDIO_CODE_SELECT(SELECT) (((SELECT) == SBS_VDDIO_CELL_CODE)|| \
                                          ((SELECT) == SBS_VDDIO_REGISTER_CODE))

#define IS_SBS_CODE_CONFIG(CONFIG) ((CONFIG) < (0x10UL))

/**
  * @}
  */

#if defined(SBS_EPOCHSELCR_EPOCH_SEL)
/** @defgroup SBS_EPOCH_Selection  EPOCH Selection
  * @{
  */
#define SBS_EPOCH_SEL_NONSECURE          0x0UL                         /*!< EPOCH non secure selected */
#define SBS_EPOCH_SEL_SECURE             SBS_EPOCHSELCR_EPOCH_SEL_0    /*!< EPOCH secure selected */
#define SBS_EPOCH_SEL_PUFCHECK           SBS_EPOCHSELCR_EPOCH_SEL_1    /*!< EPOCH all zeros for PUF integrity check */

#define IS_SBS_EPOCH_SELECTION(SELECT) (((SELECT) == SBS_EPOCH_SEL_SECURE)    || \
                                        ((SELECT) == SBS_EPOCH_SEL_NONSECURE) || \
                                        ((SELECT) == SBS_EPOCH_SEL_PUFCHECK))
/**
  * @}
  */
#endif /* SBS_EPOCHSELCR_EPOCH_SEL */

#if defined(SBS_NEXTHDPLCR_NEXTHDPL)
/** @defgroup SBS_NextHDPL_Selection  Next HDPL Selection
  * @{
  */
#define SBS_OBKHDPL_INCR_0                   0x00U                      /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
#define SBS_OBKHDPL_INCR_1                   SBS_NEXTHDPLCR_NEXTHDPL_0  /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
#define SBS_OBKHDPL_INCR_2                   SBS_NEXTHDPLCR_NEXTHDPL_1  /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
#define SBS_OBKHDPL_INCR_3                   SBS_NEXTHDPLCR_NEXTHDPL    /*!< Index to add to the current HDPL to point (through OBK-HDPL) to the next secure storage areas */
/**
  * @}
  */
#endif /* SBS_NEXTHDPLCR_NEXTHDPL */

/** @defgroup SBS_HDPL_Value  HDPL Value
  * @{
  */
#define SBS_HDPL_VALUE_0                     0x000000B4U   /*!< Hide protection level 0 */
#define SBS_HDPL_VALUE_1                     0x00000051U   /*!< Hide protection level 1 */
#define SBS_HDPL_VALUE_2                     0x0000008AU   /*!< Hide protection level 2 */
#define SBS_HDPL_VALUE_3                     0x0000006FU   /*!< Hide protection level 3 */
/**
  * @}
  */

#if defined(SBS_DBGCR_DBG_AUTH_SEC)
/** @defgroup SBS_DEBUG_SEC_Value  Debug sec Value
  * @{
  */
#define SBS_DEBUG_SEC_NSEC                   0x000000B4U   /*!< Debug opening for secure and non-secure */
#define SBS_DEBUG_NSEC                       0x0000003CU   /*!< Debug opening for non-secure only */
/**
  * @}
  */
#endif /* SBS_DBGCR_DBG_AUTH_SEC */

/** @defgroup SBS_Lock_items SBS Lock items
  * @brief SBS items to set lock on
  * @{
  */
#define SBS_MPU_NSEC                SBS_CNSLCKR_LOCKNSMPU            /*!< Non-secure MPU lock (privileged secure or
                                                                          non-secure only) */
#define SBS_VTOR_NSEC               SBS_CNSLCKR_LOCKNSVTOR           /*!< Non-secure VTOR lock (privileged secure or
                                                                          non-secure only) */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define SBS_SAU                     (SBS_CSLCKR_LOCKSAU << 16U)      /*!< SAU lock (privileged secure code only) */
#define SBS_MPU_SEC                 (SBS_CSLCKR_LOCKSMPU << 16U)     /*!< Secure MPU lock (privileged secure code only)
                                                                      */
#define SBS_VTOR_AIRCR_SEC          (SBS_CSLCKR_LOCKSVTAIRCR << 16U) /*!< VTOR_S and AIRCR lock (privileged secure
                                                                          code only) */
#define SBS_LOCK_ALL                (SBS_MPU_NSEC|SBS_VTOR_NSEC|SBS_SAU|SBS_MPU_SEC|SBS_VTOR_AIRCR_SEC)  /*!< All */
#else
#define SBS_LOCK_ALL                (SBS_MPU_NSEC|SBS_VTOR_NSEC)     /*!< All (privileged secure or non-secure only) */
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/** @defgroup SBS_Attributes_items SBS Attributes items
  * @brief SBS items to configure secure or non-secure attributes on
  * @{
  */
#define SBS_CLK                     SBS_SECCFGR_SBSSEC      /*!< SBS clock control */
#define SBS_CLASSB                  SBS_SECCFGR_CLASSBSEC   /*!< Class B */
#define SBS_FPU                     SBS_SECCFGR_FPUSEC      /*!< FPU */
#define SBS_ALL                     (SBS_CLK | SBS_CLASSB | SBS_FPU) /*!< All */
/**
  * @}
  */

/** @defgroup SBS_attributes SBS attributes
  * @brief SBS secure or non-secure attributes
  * @{
  */
#define SBS_SEC                     0x00000001U   /*!< Secure attribute      */
#define SBS_NSEC                    0x00000000U   /*!< Non-secure attribute  */
/**
  * @}
  */

#if defined(SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE)
/** @defgroup SBS_OTG_PHYTUNER_PreemphasisCurrent  OTG PHYTUNER Preemphasis Current
  * @{
  */

/** @brief  High-speed (HS) transmitter preemphasis current control
  */
#define SBS_OTG_HS_PHY_PREEMP_DISABLED   0x00000000U                                                                       /*!< HS transmitter preemphasis circuit disabled */
#define SBS_OTG_HS_PHY_PREEMP_1X         SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_0                                              /*!< HS transmitter preemphasis circuit sources 1x preemphasis current */
#define SBS_OTG_HS_PHY_PREEMP_2X         SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_1                                              /*!< HS transmitter preemphasis circuit sources 2x preemphasis current */
#define SBS_OTG_HS_PHY_PREEMP_3X         (SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_0 | SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE_1)     /*!< HS transmitter preemphasis circuit sources 3x preemphasis current */

/**
  * @}
  */
#endif /* SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE */

#if defined(SBS_OTGHSPHYTUNER2_SQRXTUNE)
/** @defgroup SBS_OTG_PHYTUNER_SquelchThreshold  OTG PHYTUNER Squelch Threshold
  * @{
  */

/** @brief Squelch threshold adjustment
  */
#define SBS_OTG_HS_PHY_SQUELCH_15PERCENT       0x00000000U                                                                        /*!< +15% (recommended value) */
#define SBS_OTG_HS_PHY_SQUELCH_0PERCENT        (SBS_OTGHSPHYTUNER2_SQRXTUNE_0 | SBS_OTGHSPHYTUNER2_SQRXTUNE_1)                   /*!< 0% (default value) */

/**
  * @}
  */
#endif /* SBS_OTGHSPHYTUNER2_SQRXTUNE */

#if defined(SBS_OTGHSPHYTUNER2_COMPDISTUNE)
/** @defgroup SBS_OTG_PHYTUNER_DisconnectThreshold  OTG PHYTUNER Disconnect Threshold
  * @{
  */

/** @brief Disconnect threshold adjustment
  */
#define SBS_OTG_HS_PHY_DISCONNECT_5_9PERCENT    SBS_OTGHSPHYTUNER2_COMPDISTUNE_1     /*!< +5.9% (recommended value) */
#define SBS_OTG_HS_PHY_DISCONNECT_0PERCENT      SBS_OTGHSPHYTUNER2_COMPDISTUNE_0     /*!< 0% (default value) */

/**
  * @}
  */

#endif /* SBS_OTGHSPHYTUNER2_COMPDISTUNE */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup DBGMCU_Exported_Macros DBGMCU Exported Macros
  * @{
  */

/** @brief  Freeze/Unfreeze Peripherals in Debug mode
  */
#if defined(DBGMCU_APB1FZR1_DBG_TIM2_STOP)
#define __HAL_DBGMCU_FREEZE_TIM2()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM2_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM2()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM2_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM2_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM3_STOP)
#define __HAL_DBGMCU_FREEZE_TIM3()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM3_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM3()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM3_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM3_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM4_STOP)
#define __HAL_DBGMCU_FREEZE_TIM4()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM4_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM4()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM4_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM4_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM5_STOP)
#define __HAL_DBGMCU_FREEZE_TIM5()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM5_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM5()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM5_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM5_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM6_STOP)
#define __HAL_DBGMCU_FREEZE_TIM6()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM6_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM6()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM6_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM6_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM7_STOP)
#define __HAL_DBGMCU_FREEZE_TIM7()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM7_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM7()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM7_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM7_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM12_STOP)
#define __HAL_DBGMCU_FREEZE_TIM12()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM12_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM12()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM12_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM12_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM13_STOP)
#define __HAL_DBGMCU_FREEZE_TIM13()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM13_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM13()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM13_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM13_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_TIM14_STOP)
#define __HAL_DBGMCU_FREEZE_TIM14()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM14_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM14()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_TIM14_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_TIM14_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_WWDG_STOP)
#define __HAL_DBGMCU_FREEZE_WWDG()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_WWDG_STOP)
#define __HAL_DBGMCU_UNFREEZE_WWDG()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_WWDG_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_WWDG_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_IWDG_STOP)
#define __HAL_DBGMCU_FREEZE_IWDG()           SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_IWDG_STOP)
#define __HAL_DBGMCU_UNFREEZE_IWDG()         CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_IWDG_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_IWDG_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_I2C1_STOP)
#define __HAL_DBGMCU_FREEZE_I2C1()              SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_I2C1_STOP)
#define __HAL_DBGMCU_UNFREEZE_I2C1()            CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_I2C1_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_I2C1_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_I2C2_STOP)
#define __HAL_DBGMCU_FREEZE_I2C2()              SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_I2C2_STOP)
#define __HAL_DBGMCU_UNFREEZE_I2C2()            CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_I2C2_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_I2C2_STOP */

#if defined(DBGMCU_APB1FZR1_DBG_I3C1_STOP)
#define __HAL_DBGMCU_FREEZE_I3C1()              SET_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_I3C1_STOP)
#define __HAL_DBGMCU_UNFREEZE_I3C1()            CLEAR_BIT(DBGMCU->APB1FZR1, DBGMCU_APB1FZR1_DBG_I3C1_STOP)
#endif /* DBGMCU_APB1FZR1_DBG_I3C1_STOP */

#if defined(DBGMCU_APB1FZR2_DBG_LPTIM2_STOP)
#define __HAL_DBGMCU_FREEZE_LPTIM2()            SET_BIT(DBGMCU->APB1FZR2, DBGMCU_APB1FZR2_DBG_LPTIM2_STOP)
#define __HAL_DBGMCU_UNFREEZE_LPTIM2()          CLEAR_BIT(DBGMCU->APB1FZR2, DBGMCU_APB1FZR2_DBG_LPTIM2_STOP)
#endif /* DBGMCU_APB1FZR2_DBG_LPTIM2_STOP */

#if defined(DBGMCU_APB2FZR_DBG_TIM1_STOP)
#define __HAL_DBGMCU_FREEZE_TIM1()              SET_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM1_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM1()            CLEAR_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM1_STOP)
#endif /* DBGMCU_APB2FZR_DBG_TIM1_STOP */

#if defined(DBGMCU_APB2FZR_DBG_TIM8_STOP)
#define __HAL_DBGMCU_FREEZE_TIM8()              SET_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM8_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM8()            CLEAR_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM8_STOP)
#endif /* DBGMCU_APB2FZR_DBG_TIM8_STOP */

#if defined(DBGMCU_APB2FZR_DBG_TIM15_STOP)
#define __HAL_DBGMCU_FREEZE_TIM15()             SET_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM15_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM15()           CLEAR_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM15_STOP)
#endif /* DBGMCU_APB2FZR_DBG_TIM15_STOP */

#if defined(DBGMCU_APB2FZR_DBG_TIM16_STOP)
#define __HAL_DBGMCU_FREEZE_TIM16()             SET_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM16_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM16()           CLEAR_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM16_STOP)
#endif /* DBGMCU_APB2FZR_DBG_TIM16_STOP */

#if defined(DBGMCU_APB2FZR_DBG_TIM17_STOP)
#define __HAL_DBGMCU_FREEZE_TIM17()             SET_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM17_STOP)
#define __HAL_DBGMCU_UNFREEZE_TIM17()           CLEAR_BIT(DBGMCU->APB2FZR, DBGMCU_APB2FZR_DBG_TIM17_STOP)
#endif /* DBGMCU_APB2FZR_DBG_TIM17_STOP */

#if defined(DBGMCU_APB3FZR_DBG_I2C3_STOP)
#define __HAL_DBGMCU_FREEZE_I2C3()              SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_I2C3_STOP)
#define __HAL_DBGMCU_UNFREEZE_I2C3()            CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_I2C3_STOP)
#endif /* DBGMCU_APB3FZR_DBG_I2C3_STOP */

#if defined(DBGMCU_APB3FZR_DBG_I2C4_STOP)
#define __HAL_DBGMCU_FREEZE_I2C4()              SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_I2C4_STOP)
#define __HAL_DBGMCU_UNFREEZE_I2C4()            CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_I2C4_STOP)
#endif /* DBGMCU_APB3FZR_DBG_I2C4_STOP */

#if defined(DBGMCU_APB3FZR_DBG_I3C2_STOP)
#define __HAL_DBGMCU_FREEZE_I3C2()            SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_I3C2_STOP)
#define __HAL_DBGMCU_UNFREEZE_I3C2()          CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_I3C2_STOP)
#endif /* DBGMCU_APB3FZR_DBG_I3C2_STOP */

#if defined(DBGMCU_APB3FZR_DBG_LPTIM1_STOP)
#define __HAL_DBGMCU_FREEZE_LPTIM1()            SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM1_STOP)
#define __HAL_DBGMCU_UNFREEZE_LPTIM1()          CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM1_STOP)
#endif /* DBGMCU_APB3FZR_DBG_LPTIM1_STOP */

#if defined(DBGMCU_APB3FZR_DBG_LPTIM3_STOP)
#define __HAL_DBGMCU_FREEZE_LPTIM3()            SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM3_STOP)
#define __HAL_DBGMCU_UNFREEZE_LPTIM3()          CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM3_STOP)
#endif /* DBGMCU_APB3FZR_DBG_LPTIM3_STOP */

#if defined(DBGMCU_APB3FZR_DBG_LPTIM4_STOP)
#define __HAL_DBGMCU_FREEZE_LPTIM4()            SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM4_STOP)
#define __HAL_DBGMCU_UNFREEZE_LPTIM4()          CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM4_STOP)
#endif /* DBGMCU_APB3FZR_DBG_LPTIM4_STOP */

#if defined(DBGMCU_APB3FZR_DBG_LPTIM5_STOP)
#define __HAL_DBGMCU_FREEZE_LPTIM5()            SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM5_STOP)
#define __HAL_DBGMCU_UNFREEZE_LPTIM5()          CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM5_STOP)
#endif /* DBGMCU_APB3FZR_DBG_LPTIM5_STOP */

#if defined(DBGMCU_APB3FZR_DBG_LPTIM6_STOP)
#define __HAL_DBGMCU_FREEZE_LPTIM6()            SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM6_STOP)
#define __HAL_DBGMCU_UNFREEZE_LPTIM6()          CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_LPTIM6_STOP)
#endif /* DBGMCU_APB3FZR_DBG_LPTIM6_STOP */

#if defined(DBGMCU_APB3FZR_DBG_RTC_STOP)
#define __HAL_DBGMCU_FREEZE_RTC()               SET_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_RTC_STOP)
#define __HAL_DBGMCU_UNFREEZE_RTC()             CLEAR_BIT(DBGMCU->APB3FZR, DBGMCU_APB3FZR_DBG_RTC_STOP)
#endif /* DBGMCU_APB3FZR_DBG_RTC_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH0_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_0()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH0_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_0()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH0_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH0_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH1_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_1()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH1_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_1()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH1_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH1_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH2_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_2()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH2_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_2()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH2_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH2_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH3_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_3()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH3_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_3()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH3_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH3_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH4_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_4()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH4_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_4()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH4_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH4_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH5_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_5()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH5_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_5()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH5_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH5_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH6_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_6()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH6_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_6()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH6_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH6_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH7_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_7()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH7_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_7()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH7_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH7_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH8_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_8()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH8_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_8()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH8_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH8_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH9_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_9()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH9_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_9()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH9_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH9_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH10_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_10()           SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH10_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_10()         CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH10_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH10_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA1_CH11_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA1_11()           SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH11_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA1_11()         CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA1_CH11_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA1_CH11_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH0_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_0()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH0_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_0()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH0_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH0_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH1_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_1()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH1_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_1()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH1_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH1_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH2_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_2()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH2_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_2()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH2_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH2_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH3_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_3()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH3_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_3()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH3_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH3_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH4_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_4()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH4_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_4()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH4_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH4_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH5_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_5()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH5_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_5()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH5_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH5_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH6_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_6()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH6_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_6()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH6_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH6_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH7_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_7()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH7_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_7()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH7_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH7_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH8_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_8()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH8_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_8()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH8_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH8_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH9_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_9()            SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH9_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_9()          CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH9_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH9_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH10_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_10()           SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH10_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_10()         CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH10_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH10_STOP */

#if defined(DBGMCU_AHB1FZR_DBG_GPDMA2_CH11_STOP)
#define __HAL_DBGMCU_FREEZE_GPDMA2_11()           SET_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH11_STOP)
#define __HAL_DBGMCU_UNFREEZE_GPDMA2_11()         CLEAR_BIT(DBGMCU->AHB1FZR, DBGMCU_AHB1FZR_DBG_GPDMA2_CH11_STOP)
#endif /* DBGMCU_AHB1FZR_DBG_GPDMA2_CH11_STOP */
/**
  * @}
  */

/** @defgroup SBS_Exported_Macros SBS Exported Macros
  * @{
  */

/** @brief  Floating Point Unit interrupt enable/disable macros
  * @param __INTERRUPT__: This parameter can be a value of @ref SBS_FPU_Interrupts
  */
#define __HAL_SBS_FPU_INTERRUPT_ENABLE(__INTERRUPT__)    do {assert_param(IS_SBS_FPU_INTERRUPT((__INTERRUPT__)));\
                                                                SET_BIT(SBS->FPUIMR, (__INTERRUPT__));\
                                                            }while(0)

#define __HAL_SBS_FPU_INTERRUPT_DISABLE(__INTERRUPT__)   do {assert_param(IS_SBS_FPU_INTERRUPT((__INTERRUPT__)));\
                                                                CLEAR_BIT(SBS->FPUIMR, (__INTERRUPT__));\
                                                            }while(0)

/** @brief  SBS Break ECC lock.
  *         Enable and lock the connection of Flash ECC error connection to TIM1/8/15/16/17 Break input.
  * @note   The selected configuration is locked and can be unlocked only by system reset.
  */
#define __HAL_SBS_BREAK_ECC_LOCK()        SET_BIT(SBS->CFGR2, SBS_CFGR2_ECCL)

/** @brief  SBS Break Cortex-M33 Lockup lock.
  *         Enable and lock the connection of Cortex-M33 LOCKUP (Hardfault) output to TIM1/8/15/16/17 Break input.
  * @note   The selected configuration is locked and can be unlocked only by system reset.
  */
#define __HAL_SBS_BREAK_LOCKUP_LOCK()     SET_BIT(SBS->CFGR2, SBS_CFGR2_CLL)

/** @brief  SBS Break PVD lock.
  *         Enable and lock the PVD connection to Timer1/8/15/16/17 Break input, as well as the PVDE and PLS[2:0]
  *         in the PWR_CR2 register.
  * @note   The selected configuration is locked and can be unlocked only by system reset.
  */
#define __HAL_SBS_BREAK_PVD_LOCK()        SET_BIT(SBS->CFGR2, SBS_CFGR2_PVDL)

/** @brief  SBS Break SRAM double ECC lock.
  *         Enable and lock the connection of SRAM double ECC error to TIM1/8/15/16/17 Break input.
  * @note   The selected configuration is locked and can be unlocked only by system reset.
  */
#define __HAL_SBS_BREAK_SRAM_ECC_LOCK()    SET_BIT(SBS->CFGR2, SBS_CFGR2_SEL)

/** @brief  Fast-mode Plus driving capability enable/disable macros
  * @param __FASTMODEPLUS__: This parameter can be a value of :
  *     @arg @ref SBS_FASTMODEPLUS_PB6 Fast-mode Plus driving capability activation on PB6
  *     @arg @ref SBS_FASTMODEPLUS_PB7 Fast-mode Plus driving capability activation on PB7
  *     @arg @ref SBS_FASTMODEPLUS_PB8 Fast-mode Plus driving capability activation on PB8
  *     @arg @ref SBS_FASTMODEPLUS_PB9 Fast-mode Plus driving capability activation on PB9
  */
#define __HAL_SBS_FASTMODEPLUS_ENABLE(__FASTMODEPLUS__)  do {assert_param(IS_SBS_FASTMODEPLUS((__FASTMODEPLUS__)));\
                                                                SET_BIT(SBS->PMCR, (__FASTMODEPLUS__));\
                                                               }while(0)

#define __HAL_SBS_FASTMODEPLUS_DISABLE(__FASTMODEPLUS__) do {assert_param(IS_SBS_FASTMODEPLUS((__FASTMODEPLUS__)));\
                                                                CLEAR_BIT(SBS->PMCR, (__FASTMODEPLUS__));\
                                                               }while(0)

/** @brief  Check SBS Memories Erase Status Flags.
  * @param  __FLAG__: specifies the flag to check.
  *         This parameter can be one of the following values:
  *            @arg @ref SBS_MEMORIES_ERASE_FLAG_IPMEE   Status of End Of Erase for ICACHE and PKA RAMs
  *            @arg @ref SBS_MEMORIES_ERASE_FLAG_MCLR    Status of Erase after Power-on Reset ((SRAM2, BKPRAM,
  *                                                      ICACHE, DCACHE, PKA RAMs)
  * @retval The new state of __FLAG__ (TRUE or FALSE).
  */
#define __HAL_SBS_GET_MEMORIES_ERASE_STATUS(__FLAG__)      ((((SBS->MESR) & (__FLAG__))!= 0) ? 1 : 0)

/** @brief  Clear SBS Memories Erase Status Flags.
  * @param  __FLAG__: specifies the flag to clear.
  *         This parameter can be one of the following values:
  *            @arg @ref SBS_MEMORIES_ERASE_FLAG_IPMEE   Status of End Of Erase for ICACHE and PKA RAMs
  *            @arg @ref SBS_MEMORIES_ERASE_FLAG_MCLR    Status of Erase after Power-on Reset ((SRAM2, BKPRAM,
  *                                                      ICACHE, DCACHE, PKA RAMs)
  */
#define __HAL_SBS_CLEAR_MEMORIES_ERASE_STATUS(__FLAG__)    do {assert_param(IS_SBS_MEMORIES_ERASE_FLAG((__FLAG__)));\
                                                                WRITE_REG(SBS->MESR, (__FLAG__));\
                                                              }while(0)

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/** @defgroup SBS_Private_Macros SBS Private Macros
  * @{
  */

#define IS_SBS_FPU_INTERRUPT(__INTERRUPT__)    ((((__INTERRUPT__) & SBS_IT_FPU_IOC) == SBS_IT_FPU_IOC) || \
                                                (((__INTERRUPT__) & SBS_IT_FPU_DZC) == SBS_IT_FPU_DZC) || \
                                                (((__INTERRUPT__) & SBS_IT_FPU_UFC) == SBS_IT_FPU_UFC) || \
                                                (((__INTERRUPT__) & SBS_IT_FPU_OFC) == SBS_IT_FPU_OFC) || \
                                                (((__INTERRUPT__) & SBS_IT_FPU_IDC) == SBS_IT_FPU_IDC) || \
                                                (((__INTERRUPT__) & SBS_IT_FPU_IXC) == SBS_IT_FPU_IXC))

#define IS_SBS_BREAK_CONFIG(__CONFIG__)    (((__CONFIG__) == SBS_BREAK_FLASH_ECC)  || \
                                            ((__CONFIG__) == SBS_BREAK_PVD)        || \
                                            ((__CONFIG__) == SBS_BREAK_SRAM_ECC)   || \
                                            ((__CONFIG__) == SBS_BREAK_LOCKUP))

#if defined(VREFBUF)
#define IS_VREFBUF_VOLTAGE_SCALE(__SCALE__)  (((__SCALE__) == VREFBUF_VOLTAGE_SCALE0) || \
                                              ((__SCALE__) == VREFBUF_VOLTAGE_SCALE1) || \
                                              ((__SCALE__) == VREFBUF_VOLTAGE_SCALE2) || \
                                              ((__SCALE__) == VREFBUF_VOLTAGE_SCALE3))

#define IS_VREFBUF_HIGH_IMPEDANCE(__VALUE__)  (((__VALUE__) == VREFBUF_HIGH_IMPEDANCE_DISABLE) || \
                                               ((__VALUE__) == VREFBUF_HIGH_IMPEDANCE_ENABLE))

#define IS_VREFBUF_TRIMMING(__VALUE__)  (((__VALUE__) > 0U) && ((__VALUE__) <= VREFBUF_CCR_TRIM))
#endif /* VREFBUF*/

#if defined(SBS_FASTMODEPLUS_PB9)
#define IS_SBS_FASTMODEPLUS(__PIN__)    ((((__PIN__) & SBS_FASTMODEPLUS_PB6) == SBS_FASTMODEPLUS_PB6) || \
                                         (((__PIN__) & SBS_FASTMODEPLUS_PB7) == SBS_FASTMODEPLUS_PB7) || \
                                         (((__PIN__) & SBS_FASTMODEPLUS_PB8) == SBS_FASTMODEPLUS_PB8) || \
                                         (((__PIN__) & SBS_FASTMODEPLUS_PB9) == SBS_FASTMODEPLUS_PB9))
#else
#define IS_SBS_FASTMODEPLUS(__PIN__)    ((((__PIN__) & SBS_FASTMODEPLUS_PB6) == SBS_FASTMODEPLUS_PB6) || \
                                         (((__PIN__) & SBS_FASTMODEPLUS_PB7) == SBS_FASTMODEPLUS_PB7) || \
                                         (((__PIN__) & SBS_FASTMODEPLUS_PB8) == SBS_FASTMODEPLUS_PB8))
#endif /* SBS_FASTMODEPLUS_PB9 */

#define IS_SBS_HDPL(__LEVEL__)          (((__LEVEL__) == SBS_HDPL_VALUE_0) || ((__LEVEL__) == SBS_HDPL_VALUE_1) || \
                                         ((__LEVEL__) == SBS_HDPL_VALUE_2) || ((__LEVEL__) == SBS_HDPL_VALUE_3))

#define IS_SBS_OBKHDPL_SELECTION(__SELECT__)    (((__SELECT__) == SBS_OBKHDPL_INCR_0)  || \
                                                 ((__SELECT__) == SBS_OBKHDPL_INCR_1)  || \
                                                 ((__SELECT__) == SBS_OBKHDPL_INCR_2)  || \
                                                 ((__SELECT__) == SBS_OBKHDPL_INCR_3))

#define IS_SBS_ITEMS_ATTRIBUTES(__ITEM__) ((((__ITEM__) & SBS_CLK)    == SBS_CLK)    || \
                                           (((__ITEM__) & SBS_CLASSB) == SBS_CLASSB) || \
                                           (((__ITEM__) & SBS_FPU)    == SBS_FPU)    || \
                                           (((__ITEM__) & ~(SBS_ALL)) == 0U))

#define IS_SBS_ATTRIBUTES(__ATTRIBUTES__) (((__ATTRIBUTES__) == SBS_SEC)  ||\
                                           ((__ATTRIBUTES__) == SBS_NSEC))

#if defined(SBS_OTGHSPHYTUNER2_COMPDISTUNE)
#define IS_SBS_OTGPHY_DISCONNECT(__VALUE__)        (((__VALUE__) == SBS_OTG_HS_PHY_DISCONNECT_5_9PERCENT) || \
                                                    ((__VALUE__) == SBS_OTG_HS_PHY_DISCONNECT_0PERCENT))
#endif /* SBS_OTGHSPHYTUNER2_COMPDISTUNE*/
#if defined(SBS_OTGHSPHYTUNER2_SQRXTUNE)
#define IS_SBS_OTGPHY_SQUELCH(__VALUE__)           (((__VALUE__) == SBS_OTG_HS_PHY_SQUELCH_0PERCENT) || \
                                                    ((__VALUE__) == SBS_OTG_HS_PHY_SQUELCH_15PERCENT))
#endif /* SBS_OTGHSPHYTUNER2_SQRXTUNE */

#if defined(SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE)
#define IS_SBS_OTGPHY_PREEMPHASIS(__VALUE__)       (((__VALUE__) == SBS_OTG_HS_PHY_PREEMP_DISABLED) || \
                                                    ((__VALUE__) == SBS_OTG_HS_PHY_PREEMP_1X) || \
                                                    ((__VALUE__) == SBS_OTG_HS_PHY_PREEMP_2X) || \
                                                    ((__VALUE__) == SBS_OTG_HS_PHY_PREEMP_3X))
#endif /* SBS_OTGHSPHYTUNER2_TXPREEMPAMPTUNE */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)

#define IS_SBS_LOCK_ITEMS(__ITEM__) ((((__ITEM__) & SBS_MPU_NSEC)       == SBS_MPU_NSEC)       || \
                                     (((__ITEM__) & SBS_VTOR_NSEC)      == SBS_VTOR_NSEC)      || \
                                     (((__ITEM__) & SBS_SAU)            == SBS_SAU)            || \
                                     (((__ITEM__) & SBS_MPU_SEC)        == SBS_MPU_SEC)        || \
                                     (((__ITEM__) & SBS_VTOR_AIRCR_SEC) == SBS_VTOR_AIRCR_SEC) || \
                                     (((__ITEM__) & ~(SBS_LOCK_ALL)) == 0U))

#else

#define IS_SBS_LOCK_ITEMS(__ITEM__) ((((__ITEM__) & SBS_MPU_NSEC)  == SBS_MPU_NSEC)    || \
                                     (((__ITEM__) & SBS_VTOR_NSEC) == SBS_VTOR_NSEC)   || \
                                     (((__ITEM__) & ~(SBS_LOCK_ALL)) == 0U))


#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/** @defgroup HAL_Private_Macros HAL Private Macros
  * @{
  */
#define IS_TICKFREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ)  || \
                           ((FREQ) == HAL_TICK_FREQ_100HZ) || \
                           ((FREQ) == HAL_TICK_FREQ_1KHZ))
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/

/** @addtogroup HAL_Exported_Functions
  * @{
  */

/** @addtogroup HAL_Exported_Functions_Group1
  * @{
  */

/* Initialization and de-initialization functions  ******************************/
HAL_StatusTypeDef    HAL_Init(void);
HAL_StatusTypeDef    HAL_DeInit(void);
void                 HAL_MspInit(void);
void                 HAL_MspDeInit(void);
HAL_StatusTypeDef    HAL_InitTick(uint32_t TickPriority);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group2
  * @{
  */

/* Peripheral Control functions  ************************************************/
void                 HAL_IncTick(void);
void                 HAL_Delay(uint32_t Delay);
uint32_t             HAL_GetTick(void);
uint32_t             HAL_GetTickPrio(void);
HAL_StatusTypeDef    HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef  HAL_GetTickFreq(void);
void                 HAL_SuspendTick(void);
void                 HAL_ResumeTick(void);
uint32_t             HAL_GetHalVersion(void);
uint32_t             HAL_GetREVID(void);
uint32_t             HAL_GetDEVID(void);
uint32_t             HAL_GetUIDw0(void);
uint32_t             HAL_GetUIDw1(void);
uint32_t             HAL_GetUIDw2(void);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group3
  * @{
  */

/* DBGMCU Peripheral Control functions  *****************************************/
void                 HAL_DBGMCU_EnableDBGStopMode(void);
void                 HAL_DBGMCU_DisableDBGStopMode(void);
void                 HAL_DBGMCU_EnableDBGStandbyMode(void);
void                 HAL_DBGMCU_DisableDBGStandbyMode(void);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group4
  * @{
  */

/* VREFBUF Control functions  ****************************************************/
#if defined(VREFBUF)
void                 HAL_VREFBUF_VoltageScalingConfig(uint32_t VoltageScaling);
void                 HAL_VREFBUF_HighImpedanceConfig(uint32_t Mode);
void                 HAL_VREFBUF_TrimmingConfig(uint32_t TrimmingValue);
HAL_StatusTypeDef    HAL_EnableVREFBUF(void);
void                 HAL_DisableVREFBUF(void);
#endif /* VREFBUF */

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group5
  * @{
  */

/* SBS System Configuration functions  *******************************************/
void                 HAL_SBS_ETHInterfaceSelect(uint32_t SBS_ETHInterface);
void                 HAL_SBS_EnableVddIO1CompensationCell(void);
void                 HAL_SBS_DisableVddIO1CompensationCell(void);
void                 HAL_SBS_EnableVddIO2CompensationCell(void);
void                 HAL_SBS_DisableVddIO2CompensationCell(void);
void                 HAL_SBS_VDDCompensationCodeSelect(uint32_t SBS_CompCode);
void                 HAL_SBS_VDDIOCompensationCodeSelect(uint32_t SBS_CompCode);
uint32_t             HAL_SBS_GetVddIO1CompensationCellReadyFlag(void);
uint32_t             HAL_SBS_GetVddIO2CompensationCellReadyFlag(void);
void                 HAL_SBS_VDDCompensationCodeConfig(uint32_t SBS_PMOSCode, uint32_t SBS_NMOSCode);
void                 HAL_SBS_VDDIOCompensationCodeConfig(uint32_t SBS_PMOSCode, uint32_t SBS_NMOSCode);
uint32_t             HAL_SBS_GetNMOSVddCompensationValue(void);
uint32_t             HAL_SBS_GetPMOSVddCompensationValue(void);
uint32_t             HAL_SBS_GetNMOSVddIO2CompensationValue(void);
uint32_t             HAL_SBS_GetPMOSVddIO2CompensationValue(void);
void                 HAL_SBS_FLASH_EnableECCNMI(void);
void                 HAL_SBS_FLASH_DisableECCNMI(void);
uint32_t             HAL_SBS_FLASH_ECCNMI_IsDisabled(void);
void                 HAL_SBS_SetOTGPHYDisconnectThreshold(uint32_t DisconnectThreshold);
uint32_t             HAL_SBS_GetOTGPHYDisconnectThreshold(void);
void                 HAL_SBS_SetOTGPHYSquelchThreshold(uint32_t SquelchThreshold);
uint32_t             HAL_SBS_GetOTGPHYSquelchThreshold(void);
void                 HAL_SBS_SetOTGPHYPreemphasisCurrent(uint32_t PreemphasisCurrent);
uint32_t             HAL_SBS_GetOTGPHYPreemphasisCurrent(void);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group6
  * @{
  */

/* SBS Boot control functions  ***************************************************/
void                 HAL_SBS_IncrementHDPLValue(void);
uint32_t             HAL_SBS_GetHDPLValue(void);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group7
  * @{
  */

/* SBS Hardware secure storage control functions  ********************************/
void                 HAL_SBS_EPOCHSelection(uint32_t Epoch_Selection);
uint32_t             HAL_SBS_GetEPOCHSelection(void);
void                 HAL_SBS_SetOBKHDPL(uint32_t OBKHDPL_Value);
uint32_t             HAL_SBS_GetOBKHDPL(void);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group8
  * @{
  */

/* SBS Debug control functions  ***************************************************/
void                 HAL_SBS_OpenAccessPort(void);
void                 HAL_SBS_OpenDebug(void);
HAL_StatusTypeDef    HAL_SBS_ConfigDebugLevel(uint32_t Level);
uint32_t             HAL_SBS_GetDebugLevel(void);
void                 HAL_SBS_LockDebugConfig(void);
void                 HAL_SBS_ConfigDebugSecurity(uint32_t Security);
uint32_t             HAL_SBS_GetDebugSecurity(void);

/**
  * @}
  */


/** @addtogroup HAL_Exported_Functions_Group9
  * @{
  */

/* SBS Lock functions ********************************************/
void              HAL_SBS_Lock(uint32_t Item);
HAL_StatusTypeDef HAL_SBS_GetLock(uint32_t *pItem);

/**
  * @}
  */

/** @addtogroup HAL_Exported_Functions_Group10
  * @{
  */

/* SBS Attributes functions ********************************************/
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
void              HAL_SBS_ConfigAttributes(uint32_t Item, uint32_t Attributes);
HAL_StatusTypeDef HAL_SBS_GetConfigAttributes(uint32_t Item, uint32_t *pAttributes);
#endif /* __ARM_FEATURE_CMSE */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32H5xx_HAL_H */

