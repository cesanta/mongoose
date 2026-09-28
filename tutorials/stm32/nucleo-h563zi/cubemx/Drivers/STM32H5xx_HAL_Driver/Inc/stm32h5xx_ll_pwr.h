/**
  ******************************************************************************
  * @file    stm32h5xx_ll_pwr.h
  * @author  MCD Application Team
  * @brief   Header file of PWR LL module.
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
#ifndef STM32H5xx_LL_PWR_H
#define STM32H5xx_LL_PWR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx.h"

/** @addtogroup STM32H5xx_LL_Driver
  * @{
  */

#if defined (PWR)

/** @defgroup PWR_LL PWR
  * @{
  */

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup PWR_LL_Private_Constants PWR Private Constants
  * @{
  */

/** @defgroup PWR_LL_WAKEUP_PIN_OFFSET Wake-Up Pins register offsets Defines
  * @brief    Flags defines which can be used with LL_PWR_WriteReg function
  * @{
  */
/* Wake-Up Pins PWR register offsets */
#define LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET    2UL
#define LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK       0x7FU
/**
  * @}
  */
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @defgroup PWR_LL_Exported_Constants PWR Exported Constants
  * @{
  */

/** @defgroup PWR_LL_EC_CLEAR_FLAG Clear Flags Defines
  * @brief    Flags defines which can be used with LL_PWR_WriteReg function
  * @{
  */
#define LL_PWR_PMCR_CSSF       PWR_PMCR_CSSF   /*!< Clear STOP and STANDBY flags */
#define LL_PWR_WUSCR_CWUF1     PWR_WUSCR_CWUF1 /*!< Clear Wakeup flag 1          */
#define LL_PWR_WUSCR_CWUF2     PWR_WUSCR_CWUF2 /*!< Clear Wakeup flag 2          */
#define LL_PWR_WUSCR_CWUF3     PWR_WUSCR_CWUF3 /*!< Clear Wakeup flag 3          */
#define LL_PWR_WUSCR_CWUF4     PWR_WUSCR_CWUF4 /*!< Clear Wakeup flag 4          */
#define LL_PWR_WUSCR_CWUF5     PWR_WUSCR_CWUF5 /*!< Clear Wakeup flag 5          */
#define LL_PWR_WUSCR_CWUF6     PWR_WUSCR_CWUF6 /*!< Clear Wakeup flag 6          */
#define LL_PWR_WUSCR_CWUF7     PWR_WUSCR_CWUF7 /*!< Clear Wakeup flag 7          */
#define LL_PWR_WUSCR_CWUF8     PWR_WUSCR_CWUF8 /*!< Clear Wakeup flag 8          */
#define LL_PWR_WUSCR_CWUF_ALL  PWR_WUSCR_CWUF  /*!< Clear all Wakeup flags       */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_GET_FLAG Get Flags Defines
  * @brief    Flags defines which can be used with LL_PWR_ReadReg function
  * @{
  */
#define LL_PWR_FLAG_VOSRDY    PWR_VOSR_VOSRDY     /*!< Voltage scaling ready flag                               */
#define LL_PWR_FLAG_ACTVOSRDY PWR_VOSR_ACTOVSRDY  /*!< Currently applied VOS ready flag                         */
#define LL_PWR_FLAG_STOPF     PWR_PMSR_STOPF      /*!< STOP flag                                                */
#define LL_PWR_FLAG_SBF       PWR_PMSR_SBF        /*!< STANDBY flag                                             */
#define LL_PWR_FLAG_AVDO      PWR_VMSR_AVDO       /*!< Analog voltage detector output on VDDA flag              */
#define LL_PWR_FLAG_VDDIO2RDY PWR_VMSR_VDDIO2RDY  /*!< VDDIO2 ready flag                                        */
#define LL_PWR_FLAG_PVDO      PWR_VMSR_PVDO       /*!< Programmable voltage detect output flag                  */
#define LL_PWR_FLAG_USB33RDY  PWR_VMSR_USB33RDY   /*!< VDDUSB ready flag                                        */
#define LL_PWR_FLAG_TEMPH     PWR_BDSR_TEMPH      /*!< Temperature level flag (versus high threshold)           */
#define LL_PWR_FLAG_TEMPL     PWR_BDSR_TEMPL      /*!< Temperature level flag (versus low threshold)            */
#define LL_PWR_FLAG_VBATH     PWR_BDSR_VBATH      /*!< VBAT level flag (versus high threshold) */
#define LL_PWR_FLAG_VBATL     PWR_BDSR_VBATL      /*!< VBAT level flag (versus low threshold) */


#define LL_PWR_WAKEUP_FLAG1   PWR_WUSR_WUF1       /*!< Wakeup flag 1 */
#define LL_PWR_WAKEUP_FLAG2   PWR_WUSR_WUF2       /*!< Wakeup flag 2 */
#define LL_PWR_WAKEUP_FLAG3   PWR_WUSR_WUF3       /*!< Wakeup flag 3 */
#define LL_PWR_WAKEUP_FLAG4   PWR_WUSR_WUF4       /*!< Wakeup flag 4 */
#define LL_PWR_WAKEUP_FLAG5   PWR_WUSR_WUF5       /*!< Wakeup flag 5 */
#define LL_PWR_WAKEUP_FLAG6   PWR_WUSR_WUF6       /*!< Wakeup flag 6 */
#define LL_PWR_WAKEUP_FLAG7   PWR_WUSR_WUF7       /*!< Wakeup flag 7 */
#define LL_PWR_WAKEUP_FLAG8   PWR_WUSR_WUF8       /*!< Wakeup flag 8 */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_LOW_POWER_MODE_SELCTION  Low Power Mode Selection
  * @{
  */
#define LL_PWR_STOP_MODE    (0U)                   /*!< STOP 0 mode    */
#define LL_PWR_STANDBY_MODE PWR_PMCR_LPMS          /*!< STANDBY mode   */


/**
  * @}
  */

/** @defgroup PWR_LL_EC_VOLTAGE_SCALING_RANGE_SELECTION PWR Voltage scaling range selection
  * @{
  */
#define LL_PWR_REGU_VOLTAGE_SCALE0 PWR_VOSCR_VOS   /*!< Voltage scaling range 0 */
#define LL_PWR_REGU_VOLTAGE_SCALE1 PWR_VOSCR_VOS_1 /*!< Voltage scaling range 1 */
#define LL_PWR_REGU_VOLTAGE_SCALE2 PWR_VOSCR_VOS_0 /*!< Voltage scaling range 2 */
#define LL_PWR_REGU_VOLTAGE_SCALE3 0x00000000U     /*!< Voltage scaling range 3 */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_STOP_MODE_REGU_VOLTAGE Stop mode Regulator Voltage Scaling
  * @{
  */
#define LL_PWR_REGU_VOLTAGE_SVOS_SCALE5   PWR_PMCR_SVOS_0                     /*!< Select voltage scale 5 when system enters STOP mode */
#define LL_PWR_REGU_VOLTAGE_SVOS_SCALE4   PWR_PMCR_SVOS_1                     /*!< Select voltage scale 4 when system enters STOP mode */
#define LL_PWR_REGU_VOLTAGE_SVOS_SCALE3   (PWR_PMCR_SVOS_0 | PWR_PMCR_SVOS_1) /*!< Select voltage scale 3 when system enters STOP mode */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_PVD_LEVEL_SELECTION PWR Power Voltage Detector Level Selection
  * @{
  */
#define LL_PWR_PVDLEVEL_0 0U                                          /*!< Voltage threshold detected by PVD 1.95 V  */
#define LL_PWR_PVDLEVEL_1 PWR_VMCR_PLS_0                              /*!< Voltage threshold detected by PVD 2.10 V  */
#define LL_PWR_PVDLEVEL_2 PWR_VMCR_PLS_1                              /*!< Voltage threshold detected by PVD 2.25 V  */
#define LL_PWR_PVDLEVEL_3 (PWR_VMCR_PLS_0 | PWR_VMCR_PLS_1)           /*!< Voltage threshold detected by PVD 2.40 V  */
#define LL_PWR_PVDLEVEL_4 PWR_VMCR_PLS_2                              /*!< Voltage threshold detected by PVD 2.55 V  */
#define LL_PWR_PVDLEVEL_5 (PWR_VMCR_PLS_0 | PWR_VMCR_PLS_2)           /*!< Voltage threshold detected by PVD 2.70 V  */
#define LL_PWR_PVDLEVEL_6 (PWR_VMCR_PLS_1 | PWR_VMCR_PLS_2)           /*!< Voltage threshold detected by PVD 2.85 V  */
#define LL_PWR_PVDLEVEL_7 PWR_VMCR_PLS                                /*!< External input analog voltage on PVD_IN
                                                                       pin, compared to internal VREFINT level  */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_AVDLEVEL Power Analog Voltage Level Detector
  * @{
  */
#define LL_PWR_AVDLEVEL_0   0U                /*!< Analog Voltage threshold detected by AVD 1.7 V */
#define LL_PWR_AVDLEVEL_1   PWR_VMCR_ALS_0    /*!< Analog Voltage threshold detected by AVD 2.1 V */
#define LL_PWR_AVDLEVEL_2   PWR_VMCR_ALS_1    /*!< Analog Voltage threshold detected by AVD 2.5 V */
#define LL_PWR_AVDLEVEL_3   PWR_VMCR_ALS      /*!< Analog Voltage threshold detected by AVD 2.8 V */

/**
  * @}
  */

/** @defgroup PWR_LL_EC_WAKEUP_PIN PWR Wake Up Pin
  * @{
  */
#define LL_PWR_WAKEUP_PIN1 PWR_WUCR_WUPEN1 /*!< Wakeup pin 1 enable */
#define LL_PWR_WAKEUP_PIN2 PWR_WUCR_WUPEN2 /*!< Wakeup pin 2 enable */
#define LL_PWR_WAKEUP_PIN3 PWR_WUCR_WUPEN3 /*!< Wakeup pin 3 enable */
#define LL_PWR_WAKEUP_PIN4 PWR_WUCR_WUPEN4 /*!< Wakeup pin 4 enable */
#define LL_PWR_WAKEUP_PIN5 PWR_WUCR_WUPEN5 /*!< Wakeup pin 5 enable */
#define LL_PWR_WAKEUP_PIN6 PWR_WUCR_WUPEN6 /*!< Wakeup pin 6 enable */
#define LL_PWR_WAKEUP_PIN7 PWR_WUCR_WUPEN7 /*!< Wakeup pin 7 enable */
#define LL_PWR_WAKEUP_PIN8 PWR_WUCR_WUPEN8 /*!< Wakeup pin 8 enable */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_WAKEUP_PIN_PULL  Wakeup Pins pull configuration
  * @{
  */
#define LL_PWR_WAKEUP_PIN_NOPULL      0x00000000UL   /*!< Configure Wake-Up pin in no pull   */
#define LL_PWR_WAKEUP_PIN_PULLUP      0x00000001UL   /*!< Configure Wake-Up pin in pull Up   */
#define LL_PWR_WAKEUP_PIN_PULLDOWN    0x00000002UL   /*!< Configure Wake-Up pin in pull Down */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_SUPPLY_PWR Power supply source configuration
  * @{
  */
#define LL_PWR_EXTERNAL_SOURCE_SUPPLY PWR_SCCR_BYPASS   /*!< The SMPS and the LDO are Bypassed.
                                                        The Core domains are supplied from an external source */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_CHARGING_RESISTOR_SELECTION PWR VBAT Charging Resistor Selection
  * @{
  */
#define LL_PWR_BATT_CHARG_RESISTOR_5K   0U             /*!< Charge the battery through a 5 kO resistor   */
#define LL_PWR_BATT_CHARG_RESISTOR_1_5K PWR_BDCR_VBRS  /*!< Charge the battery through a 1.5 kO resistor */
/**
  * @}
  */

/** @defgroup PWR_LL_EC_ITEMS_SECURE_ATTRIBUTE PWR Items Secure Attribute
  * @{
  */
#define LL_PWR_WAKEUP_PIN1_NSEC 0U                  /* Wake up pin 1 nsecure mode            */
#define LL_PWR_WAKEUP_PIN1_SEC  PWR_SECCFGR_WUP1SEC /* Wake up pin 1 secure mode             */
#define LL_PWR_WAKEUP_PIN2_NSEC 0U                  /* Wake up pin 2 nsecure mode            */
#define LL_PWR_WAKEUP_PIN2_SEC  PWR_SECCFGR_WUP2SEC /* Wake up pin 2 secure mode             */
#define LL_PWR_WAKEUP_PIN3_NSEC 0U                  /* Wake up pin 3 nsecure mode            */
#define LL_PWR_WAKEUP_PIN3_SEC  PWR_SECCFGR_WUP3SEC /* Wake up pin 3 secure mode             */
#define LL_PWR_WAKEUP_PIN4_NSEC 0U                  /* Wake up pin 4 nsecure mode            */
#define LL_PWR_WAKEUP_PIN4_SEC  PWR_SECCFGR_WUP4SEC /* Wake up pin 4 secure mode             */
#define LL_PWR_WAKEUP_PIN5_NSEC 0U                  /* Wake up pin 5 nsecure mode            */
#define LL_PWR_WAKEUP_PIN5_SEC  PWR_SECCFGR_WUP5SEC /* Wake up pin 5 secure mode             */
#define LL_PWR_WAKEUP_PIN6_NSEC 0U                  /* Wake up pin 6 nsecure mode            */
#define LL_PWR_WAKEUP_PIN6_SEC  PWR_SECCFGR_WUP6SEC /* Wake up pin 6 secure mode             */
#define LL_PWR_WAKEUP_PIN7_NSEC 0U                  /* Wake up pin 7 nsecure mode            */
#define LL_PWR_WAKEUP_PIN7_SEC  PWR_SECCFGR_WUP7SEC /* Wake up pin 7 secure mode             */
#define LL_PWR_WAKEUP_PIN8_NSEC 0U                  /* Wake up pin 8 nsecure mode            */
#define LL_PWR_WAKEUP_PIN8_SEC  PWR_SECCFGR_WUP8SEC /* Wake up pin 8 secure mode             */

#define LL_PWR_RET_NSEC 0U                  /* Retention nsecure mode                        */
#define LL_PWR_RET_SEC  PWR_SECCFGR_RETSEC  /* Retention secure mode                         */
#define LL_PWR_LPM_NSEC 0U                  /* Low-power modes nsecure mode                  */
#define LL_PWR_LPM_SEC  PWR_SECCFGR_LPMSEC  /* Low-power modes secure mode                   */
#define LL_PWR_VDM_NSEC 0U                  /* Voltage detection and monitoring nsecure mode */
#define LL_PWR_VDM_SEC  PWR_SECCFGR_SCMSEC  /* Voltage detection and monitoring secure mode  */
#define LL_PWR_VB_NSEC  0U                  /* Backup domain nsecure mode                    */
#define LL_PWR_VB_SEC   PWR_SECCFGR_VBSEC   /* Backup domain secure mode                     */
#define LL_PWR_APC_NSEC 0U                  /* Pull-up/pull-down nsecure mode                */
#define LL_PWR_APC_SEC  PWR_SECCFGR_VUSBSEC /* Pull-up/pull-down secure mode                 */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup PWR_LL_Exported_Macros PWR Exported Macros
  * @{
  */

/** @defgroup PWR_LL_EM_WRITE_READ Common Write and Read Registers Macros
  * @{
  */

/**
  * @brief  Write a value in PWR register.
  * @param  __REG__      Register to be written.
  * @param  __VALUE__    Value to be written in the register.
  * @retval None.
  */
#define LL_PWR_WriteReg(__REG__, __VALUE__) WRITE_REG(PWR->__REG__, (__VALUE__))

/**
  * @brief  Read a value in PWR register.
  * @param  __REG__      Register to be read.
  * @retval Register value.
  */
#define LL_PWR_ReadReg(__REG__) READ_REG(PWR->__REG__)
/**
  * @}
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup PWR_LL_Exported_Functions PWR Exported Functions
  * @{
  */

/** @defgroup PWR_LL_EF_CONFIGURATION PWR Configuration
  * @{
  */

/**
  * @brief  Set system power mode.
  * @rmtoll PMCR              LPMS          LL_PWR_SetPowerMode
  * @param  Mode : This parameter can be one of the following values:
  *         @arg @ref LL_PWR_STOP_MODE
  *         @arg @ref LL_PWR_STANDBY_MODE
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetPowerMode(uint32_t Mode)
{
  MODIFY_REG(PWR->PMCR, PWR_PMCR_LPMS, Mode);
}

/**
  * @brief  Get system power mode.
  * @rmtoll PMCR             LPMS          LL_PWR_GetPowerMode
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_STOP_MODE
  *         @arg @ref LL_PWR_STANDBY_MODE
  */
__STATIC_INLINE uint32_t LL_PWR_GetPowerMode(void)
{
  return (READ_BIT(PWR->PMCR, PWR_PMCR_LPMS));
}

/**
  * @brief  Set the internal Regulator output voltage in STOP mode
  * @rmtoll PMCR    SVOS       LL_PWR_SetStopModeRegulVoltageScaling
  * @param  VoltageScaling This parameter can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SVOS_SCALE3
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SVOS_SCALE4
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SVOS_SCALE5
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetStopModeRegulVoltageScaling(uint32_t VoltageScaling)
{
  MODIFY_REG(PWR->PMCR, PWR_PMCR_SVOS, VoltageScaling);
}

/**
  * @brief  Get the internal Regulator output voltage in STOP mode
  * @rmtoll PMCR    SVOS       LL_PWR_GetStopModeRegulVoltageScaling
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SVOS_SCALE3
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SVOS_SCALE4
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SVOS_SCALE5
  */
__STATIC_INLINE uint32_t LL_PWR_GetStopModeRegulVoltageScaling(void)
{
  return (uint32_t)(READ_BIT(PWR->PMCR, PWR_PMCR_SVOS));
}

/**
  * @brief  Enable the Flash Power Down in Stop Mode
  * @rmtoll PMCR    FLPS       LL_PWR_EnableFlashPowerDown
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableFlashPowerDown(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_FLPS);
}

/**
  * @brief  Disable the Flash Power Down in Stop Mode
  * @rmtoll PMCR    FLPS       LL_PWR_DisableFlashPowerDown
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableFlashPowerDown(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_FLPS);
}

/**
  * @brief  Check if the Flash Power Down in Stop Mode is enabled
  * @rmtoll PMCR    FLPS       LL_PWR_IsEnabledFlashPowerDown
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledFlashPowerDown(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_FLPS) == (PWR_PMCR_FLPS)) ? 1UL : 0UL);
}

/**
  * @brief  Enable the Analog Voltage Booster (VDDA)
  * @rmtoll PMCR    BOOSTE     LL_PWR_EnableAnalogBooster
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAnalogBooster(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_BOOSTE);
}

/**
  * @brief  Disable the Analog Voltage Booster (VDDA)
  * @rmtoll PMCR    BOOSTE     LL_PWR_DisableAnalogBooster
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAnalogBooster(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_BOOSTE);
}

/**
  * @brief  Check if the Analog Voltage Booster (VDDA) is enabled
  * @rmtoll PMCR    BOOSTE     LL_PWR_IsEnabledAnalogBooster
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAnalogBooster(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_BOOSTE) == (PWR_PMCR_BOOSTE)) ? 1UL : 0UL);
}

/**
  * @brief  Enable the Analog Voltage Ready to isolate the BOOST IP until VDDA will be ready
  * @rmtoll PMCR    AVD_READY  LL_PWR_EnableAnalogVoltageReady
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAnalogVoltageReady(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_AVD_READY);
}

/**
  * @brief  Disable the Analog Voltage Ready (VDDA)
  * @rmtoll PMCR    AVD_READY  LL_PWR_DisableAnalogVoltageReady
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAnalogVoltageReady(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_AVD_READY);
}

/**
  * @brief  Check if the Analog Voltage Booster (VDDA) is enabled
  * @rmtoll PMCR    AVD_READY  LL_PWR_IsEnabledAnalogVoltageReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAnalogVoltageReady(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_AVD_READY) == (PWR_PMCR_AVD_READY)) ? 1UL : 0UL);
}

/**
  * @brief  Enable the AHB RAM1 shut-off in Stop mode
  * @rmtoll PMCR    SRAM1SO     LL_PWR_EnableAHBRAM1ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM1ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM1SO);
}

/**
  * @brief  Disable the AHB RAM1 shut-off in Stop mode
  * @rmtoll PMCR    SRAM1SO     LL_PWR_DisableAHBRAM1ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM1ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM1SO);
}

/**
  * @brief  Check if the AHB RAM1 shut-off in Stop mode is enabled
  * @rmtoll CR1    SRAM1SO     LL_PWR_IsEnabledAHBRAM1ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM1ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM1SO) == (PWR_PMCR_SRAM1SO)) ? 1UL : 0UL);
}
#if defined (PWR_PMCR_SRAM2_48SO)
/**
  * @brief  Enable the AHB RAM2 48K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_48SO     LL_PWR_EnableAHBRAM2_48K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2_48K_ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48SO);
}

/**
  * @brief  Disable the AHB RAM2 48K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_48SO     LL_PWR_DisableAHBRAM2_48K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2_48K_ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48SO);
}

/**
  * @brief  Check if the AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2_48SO     LL_PWR_IsEnabledAHBRAM2_48K_ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2_48K_ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48SO) == (PWR_PMCR_SRAM2_48SO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2_48SO */

#if defined (PWR_PMCR_SRAM2_16SO)
/**
  * @brief  Enable the AHB RAM2 16K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_16SO     LL_PWR_EnableAHBRAM2_16K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2_16K_ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16SO);
}

/**
  * @brief  Disable the AHB RAM2 16K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_16SO     LL_PWR_DisableAHBRAM2_16K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2_16K_ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16SO);
}

/**
  * @brief  Check if the AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2_16SO     LL_PWR_IsEnabledAHBRAM2_16K_ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2_16K_ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16SO) == (PWR_PMCR_SRAM2_16SO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2_16SO */

#if defined(PWR_PMCR_SRAM2_16HSO)
/**
  * @brief  Enable the AHB RAM2 high 16K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_16HSO     LL_PWR_EnableAHBRAM2_High_16K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2_High_16K_ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16HSO);
}

/**
  * @brief  Disable the AHB RAM2 high 16K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_16HSO     LL_PWR_DisableAHBRAM2_High_16K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2_High_16K_ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16HSO);
}

/**
  * @brief  Check if the high AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2_16HSO     LL_PWR_IsEnabledAHBRAM2_16K_ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2_High_16K_ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16HSO) == (PWR_PMCR_SRAM2_16HSO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2_16HSO */

#if defined(PWR_PMCR_SRAM2_16LSO)
/**
  * @brief  Enable the AHB RAM2 low 16K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_16LSO     LL_PWR_EnableAHBRAM2_Low_16K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2_Low_16K_ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16LSO);
}

/**
  * @brief  Disable the AHB RAM2 low 16K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_16LSO     LL_PWR_DisableAHBRAM2_Low_16K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2_Low_16K_ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16LSO);
}

/**
  * @brief  Check if the low 16K AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2_16LSO     LL_PWR_IsEnabledAHBRAM2_Low_16K_ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2_Low_16K_ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2_16LSO) == (PWR_PMCR_SRAM2_16LSO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2_16LSO */

#if defined(PWR_PMCR_SRAM2_48HSO)
/**
  * @brief  Enable the AHB RAM2 high 48K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_48HSO     LL_PWR_EnableAHBRAM2_High_48K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2_High_48K_ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48HSO);
}

/**
  * @brief  Disable the AHB RAM2 high 48K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_48HSO     LL_PWR_DisableAHBRAM2_High_48K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2_High_48K_ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48HSO);
}

/**
  * @brief  Check if the high AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2_48HSO     LL_PWR_IsEnabledAHBRAM2_High_48K_ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2_High_48K_ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48HSO) == (PWR_PMCR_SRAM2_48HSO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2_48HSO */

#if defined(PWR_PMCR_SRAM2_48LSO)
/**
  * @brief  Enable the AHB RAM2 low 48K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_48LSO     LL_PWR_EnableAHBRAM2_Low_48K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2_Low_48K_ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48LSO);
}

/**
  * @brief  Disable the AHB RAM2 low 48K Bytes shut-off in Stop mode
  * @rmtoll PMCR    SRAM2_48LSO     LL_PWR_DisableAHBRAM2_Low_48K_ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2_Low_48K_ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48LSO);
}

/**
  * @brief  Check if the low 48K AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2_48LSO     LL_PWR_IsEnabledAHBRAM2_Low_48K_ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2_Low_48K_ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2_48LSO) == (PWR_PMCR_SRAM2_48LSO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2_48LSO */

#if defined (PWR_PMCR_SRAM2SO)
/**
  * @brief  Enable the AHB RAM2 shut-off in Stop mode
  * @rmtoll PMCR    SRAM2SO     LL_PWR_EnableAHBRAM2ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM2ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM2SO);
}

/**
  * @brief  Disable the AHB RAM2 shut-off in Stop mode
  * @rmtoll PMCR    SRAM2SO     LL_PWR_DisableAHBRAM2ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM2ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM2SO);
}

/**
  * @brief  Check if the AHB RAM2 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM2SO     LL_PWR_IsEnabledAHBRAM2ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM2ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM2SO) == (PWR_PMCR_SRAM2SO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM2SO */

#if defined (PWR_PMCR_SRAM3SO)
/**
  * @brief  Enable the AHB RAM3 shut-off in Stop mode
  * @rmtoll PMCR    SRAM3SO     LL_PWR_EnableAHBRAM3ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM3ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM3SO);
}

/**
  * @brief  Disable the AHB RAM3 shut-off in Stop mode
  * @rmtoll PMCR    SRAM3SO     LL_PWR_DisableAHBRAM3ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM3ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM3SO);
}

/**
  * @brief  Check if the AHB RAM3 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM3SO     LL_PWR_IsEnabledAHBRAM3ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM3ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM3SO) == (PWR_PMCR_SRAM3SO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM3SO */

#if defined (PWR_PMCR_SRAM4SO)
/**
  * @brief  Enable the AHB RAM4 shut-off in Stop mode
  * @rmtoll PMCR    SRAM4SO     LL_PWR_EnableAHBRAM4ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM4ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM4SO);
}

/**
  * @brief  Disable the AHB RAM4 shut-off in Stop mode
  * @rmtoll PMCR    SRAM4SO     LL_PWR_DisableAHBRAM4ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM4ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM4SO);
}

/**
  * @brief  Check if the AHB RAM4 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM4SO     LL_PWR_IsEnabledAHBRAM4ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM4ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM4SO) == (PWR_PMCR_SRAM4SO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM4SO */

#if defined (PWR_PMCR_SRAM5SO)
/**
  * @brief  Enable the AHB RAM5 shut-off in Stop mode
  * @rmtoll PMCR    SRAM5SO     LL_PWR_EnableAHBRAM5ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAHBRAM5ShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_SRAM5SO);
}

/**
  * @brief  Disable the AHB RAM5 shut-off in Stop mode
  * @rmtoll PMCR    SRAM5SO     LL_PWR_DisableAHBRAM5ShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAHBRAM5ShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_SRAM5SO);
}

/**
  * @brief  Check if the AHB RAM5 shut-off in Stop mode is enabled
  * @rmtoll PMCR    SRAM5SO     LL_PWR_IsEnabledAHBRAM5ShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAHBRAM5ShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_SRAM5SO) == (PWR_PMCR_SRAM5SO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_SRAM5SO */

#if defined (PWR_PMCR_ETHERNETSO)
/**
  * @brief  Enable the ETHERNET RAM shut-off in Stop mode
  * @rmtoll PMCR    ETHERNETSO     LL_PWR_EnableETHERNETRAMShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableETHERNETRAMShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_ETHERNETSO);
}

/**
  * @brief  Disable the ETHERNET RAM shut-off in Stop mode
  * @rmtoll PMCR    ETHERNETSO     LL_PWR_DisableETHERNETRAMShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableETHERNETRAMShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_ETHERNETSO);
}

/**
  * @brief  Check if the ETHERNET RAM shut-off in Stop mode is enabled
  * @rmtoll PMCR    ETHERNETSO     LL_PWR_IsEnabledETHERNETRAMShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledETHERNETRAMShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_ETHERNETSO) == (PWR_PMCR_ETHERNETSO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_ETHERNETSO */

#if defined (PWR_PMCR_LTDCSO)
/**
  * @brief  Enable the LTDC RAM shut-off in Stop mode
  * @rmtoll PMCR    LTDCSO     LL_PWR_EnableLTDCRAMShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableLTDCRAMShutOff(void)
{
  SET_BIT(PWR->PMCR, PWR_PMCR_LTDCSO);
}

/**
  * @brief  Disable the LTDC RAM shut-off in Stop mode
  * @rmtoll PMCR    LTDCSO     LL_PWR_DisableLTDCRAMShutOff
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableLTDCRAMShutOff(void)
{
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_LTDCSO);
}

/**
  * @brief  Check if the LTDC RAM shut-off in Stop mode is enabled
  * @rmtoll PMCR    LTDCSO     LL_PWR_IsEnabledLTDCRAMShutOff
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledLTDCRAMShutOff(void)
{
  return ((READ_BIT(PWR->PMCR, PWR_PMCR_LTDCSO) == (PWR_PMCR_LTDCSO)) ? 1UL : 0UL);
}
#endif /* PWR_PMCR_LTDCSO */

/**
  * @brief  Set the regulator supply output voltage.
  * @rmtoll VOSCR          VOS           LL_PWR_SetRegulVoltageScaling
  * @param  VoltageScaling This parameter can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE0
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE1
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE2
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE3
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetRegulVoltageScaling(uint32_t VoltageScaling)
{
  MODIFY_REG(PWR->VOSCR, PWR_VOSCR_VOS, VoltageScaling);
}

/**
  * @brief  Get the regulator supply output voltage.
  * @rmtoll VOSCR          VOS           LL_PWR_GetRegulVoltageScaling
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE0
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE1
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE2
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE3
  */
__STATIC_INLINE uint32_t LL_PWR_GetRegulVoltageScaling(void)
{
  return (uint32_t)(READ_BIT(PWR->VOSCR, PWR_VOSCR_VOS));
}

/**
  * @brief  Get currently voltage scaling applied to VCORE.
  * @rmtoll VOSSR            ACTVOS[1:0]          LL_PWR_GetCurrentVOS
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE0
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE1
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE2
  *         @arg @ref LL_PWR_REGU_VOLTAGE_SCALE3
  */
__STATIC_INLINE uint32_t LL_PWR_GetCurrentVOS(void)
{
  return (READ_BIT(PWR->VOSSR, PWR_VOSSR_ACTVOS));
}

/**
  * @brief  Enable Backup Regulator
  * @rmtoll BDCR    BREN       LL_PWR_EnableBkUpRegulator
  * @note When set, the Backup Regulator (used to maintain backup SRAM content in Standby and
  *       VBAT modes) is enabled. If BRE is reset, the backup Regulator is switched off. The backup
  *       SRAM can still be used but its content will be lost in the Standby and VBAT modes. Once set,
  *       the application must wait that the Backup Regulator Ready flag (BRR) is set to indicate that
  *       the data written into the RAM will be maintained in the Standby and VBAT modes.
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableBkUpRegulator(void)
{
  SET_BIT(PWR->BDCR, PWR_BDCR_BREN);
}

/**
  * @brief  Disable Backup Regulator
  * @rmtoll BDCR    BREN       LL_PWR_DisableBkUpRegulator
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableBkUpRegulator(void)
{
  CLEAR_BIT(PWR->BDCR, PWR_BDCR_BREN);
}

/**
  * @brief  Check if the backup Regulator is enabled
  * @rmtoll BDCR    BREN      LL_PWR_IsEnabledBkUpRegulator
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledBkUpRegulator(void)
{
  return ((READ_BIT(PWR->BDCR, PWR_BDCR_BREN) == (PWR_BDCR_BREN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable VBAT and Temperature monitoring
  * @rmtoll BDCR    MONEN       LL_PWR_EnableMonitoring
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableMonitoring(void)
{
  SET_BIT(PWR->BDCR, PWR_BDCR_MONEN);
}

/**
  * @brief  Disable VBAT and Temperature monitoring
  * @rmtoll BDCR    MONEN       LL_PWR_DisableMonitoring
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableMonitoring(void)
{
  CLEAR_BIT(PWR->BDCR, PWR_BDCR_MONEN);
}

/**
  * @brief  Check if the VBAT and Temperature monitoring is enabled
  * @rmtoll BDCR    MONEN      LL_PWR_IsEnabledMonitoring
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledMonitoring(void)
{
  return ((READ_BIT(PWR->BDCR, PWR_BDCR_MONEN) == (PWR_BDCR_MONEN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable battery charging
  * @rmtoll BDCR          VBE           LL_PWR_EnableBatteryCharging
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableBatteryCharging(void)
{
  SET_BIT(PWR->BDCR, PWR_BDCR_VBE);
}

/**
  * @brief  Disable battery charging
  * @rmtoll BDCR          VBE           LL_PWR_DisableBatteryCharging
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableBatteryCharging(void)
{
  CLEAR_BIT(PWR->BDCR, PWR_BDCR_VBE);
}

/**
  * @brief  Check if battery charging is enabled
  * @rmtoll BDCR          VBE           LL_PWR_IsEnabledBatteryCharging
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledBatteryCharging(void)
{
  return ((READ_BIT(PWR->BDCR, PWR_BDCR_VBE) == (PWR_BDCR_VBE)) ? 1UL : 0UL);
}

/**
  * @brief  Set the Battery charge resistor impedance
  * @rmtoll BDCR          VBRS          LL_PWR_SetBattChargResistor
  * @param  Resistor This parameter can be one of the following values:
  *         @arg @ref LL_PWR_BATT_CHARG_RESISTOR_5K
  *         @arg @ref LL_PWR_BATT_CHARG_RESISTOR_1_5K
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetBattChargResistor(uint32_t Resistor)
{
  MODIFY_REG(PWR->BDCR, PWR_BDCR_VBRS, Resistor);
}

/**
  * @brief  Get the Battery charge resistor impedance
  * @rmtoll BDCR          VBRS          LL_PWR_GetBattChargResistor
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_BATT_CHARG_RESISTOR_5K
  *         @arg @ref LL_PWR_BATT_CHARG_RESISTOR_1_5K
  */
__STATIC_INLINE uint32_t LL_PWR_GetBattChargResistor(void)
{
  return (uint32_t)(READ_BIT(PWR->BDCR, PWR_BDCR_VBRS));
}

/**
  * @brief  Enable access to the backup domain
  * @rmtoll DBPCR    DBP       LL_PWR_EnableBkUpAccess
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableBkUpAccess(void)
{
  SET_BIT(PWR->DBPCR, PWR_DBPCR_DBP);
}

/**
  * @brief  Disable access to the backup domain
  * @rmtoll DBPCR    DBP       LL_PWR_DisableBkUpAccess
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableBkUpAccess(void)
{
  CLEAR_BIT(PWR->DBPCR, PWR_DBPCR_DBP);
}

/**
  * @brief  Check if the backup domain is enabled
  * @rmtoll DBPCR    DBP       LL_PWR_IsEnabledBkUpAccess
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledBkUpAccess(void)
{
  return ((READ_BIT(PWR->DBPCR, PWR_DBPCR_DBP) == (PWR_DBPCR_DBP)) ? 1UL : 0UL);
}

#if defined (PWR_UCPDR_UCPD_STBY)
/**
  * @brief  Enable the USB type-C and power delivery memorization in Standby
  *         mode.
  * @note   This function must be called just before entering Standby mode.
  * @rmtoll UCPDR          UCPD_STDBY    LL_PWR_EnableUCPDStandbyMode
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableUCPDStandbyMode(void)
{
  SET_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STBY);
}

/**
  * @brief  Disable the USB type-C and power delivery memorization in Standby
  *         mode.
  * @note   This function must be called after exiting Standby mode and before
  *         any UCPD configuration update.
  * @rmtoll UCPDR          UCPD_STDBY     LL_PWR_DisableUCPDStandbyMode
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableUCPDStandbyMode(void)
{
  CLEAR_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STBY);
}

/**
  * @brief  Check if the USB Type-C and Power Delivery Standby mode memorization
  *         is enabled.
  * @rmtoll UCPDR          UCPD_STDBY           LL_PWR_IsEnabledUCPDStandbyMode
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledUCPDStandbyMode(void)
{
  return ((READ_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STBY) == (PWR_UCPDR_UCPD_STBY)) ? 1UL : 0UL);
}
#endif /* PWR_UCPDR_UCPD_STBY */

#if defined (PWR_UCPDR_UCPD_DBDIS)
/**
  * @brief  Enable the USB Type-C and power delivery dead battery pull-down behavior
  *         on UCPD CC1 and CC2 pins.
  * @note   After exiting reset, the USB Type-C dead battery behavior is enabled,
  *         which may have a pull-down effect on CC1 and CC2 pins. It is recommended
  *         to disable it in all cases, either to stop this pull-down or to hand over
  *         control to the UCPD (which should therefore be initialized before doing the disable).
  * @rmtoll UCPDR          UCPD_DBDIS           LL_PWR_EnableUCPDDeadBattery
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableUCPDDeadBattery(void)
{
  CLEAR_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS);
}

/**
  * @brief  Disable the USB Type-C and power delivery dead battery pull-down behavior
  *         on UCPD CC1 and CC2 pins.
  * @note   After exiting reset, the USB Type-C dead battery behavior is enabled,
  *         which may have a pull-down effect on CC1 and CC2 pins. It is recommended
  *         to disable it in all cases, either to stop this pull-down or to hand over
  *         control to the UCPD (which should therefore be initialized before doing the disable).
  * @rmtoll UCPDR          UCPD_DBDIS           LL_PWR_DisableUCPDDeadBattery
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableUCPDDeadBattery(void)
{
  SET_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS);
}

/**
  * @brief  Check the USB Type-C and power delivery dead battery pull-down behavior
  *         on UCPD CC1 and CC2 pins.
  * @note   After exiting reset, the USB Type-C dead battery behavior is enabled,
  *         which may have a pull-down effect on CC1 and CC2 pins. It is recommended
  *         to disable it in all cases, either to stop this pull-down or to hand over
  *         control to the UCPD (which should therefore be initialized before doing the disable).
  * @rmtoll UCPDR          UCPD_DBDIS           LL_PWR_IsEnabledUCPDDeadBattery
  * @retval State of feature (1 : enabled; 0 : disabled).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledUCPDDeadBattery(void)
{
  return ((READ_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS) == (PWR_UCPDR_UCPD_DBDIS)) ? 0UL : 1UL);
}
#endif /* PWR_UCPDR_UCPD_DBDIS */

/**
  * @brief  Configure the PWR supply
  * @rmtoll SCCR          BYPASS          LL_PWR_ConfigSupply
  * @param  SupplySource This parameter can be one of the following values:
  *         @arg @ref LL_PWR_EXTERNAL_SOURCE_SUPPLY
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ConfigSupply(uint32_t SupplySource)
{
  /* Set the power supply configuration */
  MODIFY_REG(PWR->SCCR, (PWR_SCCR_BYPASS), SupplySource);
}

/**
  * @brief  Get the PWR supply
  * @rmtoll SCCR          BYPASS          LL_PWR_GetSupply
  * @retval The supply configuration.
  */
__STATIC_INLINE uint32_t LL_PWR_GetSupply(void)
{
#if defined (PWR_SCCR_SMPSEN)
  /* Get the power supply configuration */
  return (uint32_t)(READ_BIT(PWR->SCCR, (PWR_SCCR_SMPSEN | PWR_SCCR_LDOEN | PWR_SCCR_BYPASS)));
#else
  /* Get the power supply configuration */
  return (uint32_t)(READ_BIT(PWR->SCCR, (PWR_SCCR_LDOEN | PWR_SCCR_BYPASS)));
#endif /* PWR_SCCR_SMPSEN */
}

/**
  * @brief  Enable Power Voltage Detector
  * @rmtoll VMCR    PVDEN       LL_PWR_EnablePVD
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnablePVD(void)
{
  SET_BIT(PWR->VMCR, PWR_VMCR_PVDEN);
}

/**
  * @brief  Disable Power Voltage Detector
  * @rmtoll VMCR    PVDEN       LL_PWR_DisablePVD
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisablePVD(void)
{
  CLEAR_BIT(PWR->VMCR, PWR_VMCR_PVDEN);
}

/**
  * @brief  Check if Power Voltage Detector is enabled
  * @rmtoll VMCR    PVDEN       LL_PWR_IsEnabledPVD
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledPVD(void)
{
  return ((READ_BIT(PWR->VMCR, PWR_VMCR_PVDEN) == (PWR_VMCR_PVDEN)) ? 1UL : 0UL);
}

/**
  * @brief  Configure the voltage threshold detected by the Power Voltage Detector
  * @rmtoll VMCR    PLS       LL_PWR_SetPVDLevel
  * @param  PVDLevel This parameter can be one of the following values:
  *         @arg @ref LL_PWR_PVDLEVEL_0
  *         @arg @ref LL_PWR_PVDLEVEL_1
  *         @arg @ref LL_PWR_PVDLEVEL_2
  *         @arg @ref LL_PWR_PVDLEVEL_3
  *         @arg @ref LL_PWR_PVDLEVEL_4
  *         @arg @ref LL_PWR_PVDLEVEL_5
  *         @arg @ref LL_PWR_PVDLEVEL_6
  *         @arg @ref LL_PWR_PVDLEVEL_7
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetPVDLevel(uint32_t PVDLevel)
{
  MODIFY_REG(PWR->VMCR, PWR_VMCR_PLS, PVDLevel);
}

/**
  * @brief  Get the voltage threshold detection
  * @rmtoll VMCR    PLS       LL_PWR_GetPVDLevel
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_PVDLEVEL_0
  *         @arg @ref LL_PWR_PVDLEVEL_1
  *         @arg @ref LL_PWR_PVDLEVEL_2
  *         @arg @ref LL_PWR_PVDLEVEL_3
  *         @arg @ref LL_PWR_PVDLEVEL_4
  *         @arg @ref LL_PWR_PVDLEVEL_5
  *         @arg @ref LL_PWR_PVDLEVEL_6
  *         @arg @ref LL_PWR_PVDLEVEL_7
  */
__STATIC_INLINE uint32_t LL_PWR_GetPVDLevel(void)
{
  return (uint32_t)(READ_BIT(PWR->VMCR, PWR_VMCR_PLS));
}


/**
  * @brief  Enable Analog Power Voltage Detector
  * @rmtoll VMCR    AVDEN       LL_PWR_EnableAVD
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableAVD(void)
{
  SET_BIT(PWR->VMCR, PWR_VMCR_AVDEN);
}

/**
  * @brief  Disable Analog Power Voltage Detector
  * @rmtoll VMCR    AVDEN       LL_PWR_DisableAVD
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableAVD(void)
{
  CLEAR_BIT(PWR->VMCR, PWR_VMCR_AVDEN);
}

/**
  * @brief  Check if Analog Power Voltage Detector is enabled
  * @rmtoll VMCR    AVDEN       LL_PWR_IsEnabledAVD
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledAVD(void)
{
  return ((READ_BIT(PWR->VMCR, PWR_VMCR_AVDEN) == (PWR_VMCR_AVDEN)) ? 1UL : 0UL);
}

/**
  * @brief  Configure the voltage threshold to be detected by the Analog Power Voltage Detector
  * @rmtoll VMCR    ALS       LL_PWR_SetAVDLevel
  * @param  AVDLevel This parameter can be one of the following values:
  *         @arg @ref LL_PWR_AVDLEVEL_0
  *         @arg @ref LL_PWR_AVDLEVEL_1
  *         @arg @ref LL_PWR_AVDLEVEL_2
  *         @arg @ref LL_PWR_AVDLEVEL_3
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetAVDLevel(uint32_t AVDLevel)
{
  MODIFY_REG(PWR->VMCR, PWR_VMCR_ALS, AVDLevel);
}

/**
  * @brief  Get the Analog Voltage threshold to be detected by the Analog Power Voltage Detector
  * @rmtoll CR1    ALS       LL_PWR_GetAVDLevel
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_AVDLEVEL_0
  *         @arg @ref LL_PWR_AVDLEVEL_1
  *         @arg @ref LL_PWR_AVDLEVEL_2
  *         @arg @ref LL_PWR_AVDLEVEL_3
  */
__STATIC_INLINE uint32_t LL_PWR_GetAVDLevel(void)
{
  return (uint32_t)(READ_BIT(PWR->VMCR, PWR_VMCR_ALS));
}

#if defined (PWR_USBSCR_USB33DEN)
/**
  * @brief  Enable the USB voltage detector
  * @rmtoll USBSCR          USB33DEN           LL_PWR_EnableUSBVoltageDetector
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableUSBVoltageDetector(void)
{
  SET_BIT(PWR->USBSCR, PWR_USBSCR_USB33DEN);
}

/**
  * @brief  Disable the USB voltage detector
  * @rmtoll USBSCR          USB33DEN           LL_PWR_DisableUSBVoltageDetector
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableUSBVoltageDetector(void)
{
  CLEAR_BIT(PWR->USBSCR, PWR_USBSCR_USB33DEN);
}

/**
  * @brief  Check if the USB voltage detector is enabled
  * @rmtoll USBSCR          USB33DEN           LL_PWR_IsEnabledUSBVoltageDetector
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledUSBVoltageDetector(void)
{
  return ((READ_BIT(PWR->USBSCR, PWR_USBSCR_USB33DEN) == (PWR_USBSCR_USB33DEN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable the independent USB supply.
  * @rmtoll USBSCR    USB33SV       LL_PWR_EnableVddUSB
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableVddUSB(void)
{
  SET_BIT(PWR->USBSCR, PWR_USBSCR_USB33SV);
}
#define LL_PWR_EnableVDDUSB  LL_PWR_EnableVddUSB /* for API backward compatibility */

/**
  * @brief  Disable the independent USB supply.
  * @rmtoll USBSCR    USB33SV       LL_PWR_DisableVddUSB
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableVddUSB(void)
{
  CLEAR_BIT(PWR->USBSCR, PWR_USBSCR_USB33SV);
}
#define LL_PWR_DisableVDDUSB  LL_PWR_DisableVddUSB /* for API backward compatibility */

/**
  * @brief  Check if the independent USB supply is enabled.
  * @rmtoll USBSCR    USB33SV       LL_PWR_IsEnabledVddUSB
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledVddUSB(void)
{
  return ((READ_BIT(PWR->USBSCR, PWR_USBSCR_USB33SV) == (PWR_USBSCR_USB33SV)) ? 1UL : 0UL);
}
#define LL_PWR_IsEnabledVDDUSB  LL_PWR_IsEnabledVddUSB /* for API backward compatibility */
#endif /* PWR_USBSCR_USB33DEN */

#if defined (PWR_USBSCR_OTGHSEN)
/**
  * @brief  Enable the USB OTGHS phy
  * @rmtoll USBSCR          OTGHSEN           LL_PWR_EnableUSBOTGHSPhy
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableUSBOTGHSPhy(void)
{
  SET_BIT(PWR->USBSCR, PWR_USBSCR_OTGHSEN);
}

/**
  * @brief  Disable the USB OTGHS phy
  * @rmtoll USBSCR          OTGHSEN           LL_PWR_DisableUSBOTGHSPhy
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableUSBOTGHSPhy(void)
{
  CLEAR_BIT(PWR->USBSCR, PWR_USBSCR_OTGHSEN);
}

/**
  * @brief  Check if the USB OTGHS phy is enabled
  * @rmtoll USBSCR          OTGHSEN           LL_PWR_IsEnabledUSBOTGHSPhy
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledUSBOTGHSPhy(void)
{
  return ((READ_BIT(PWR->USBSCR, PWR_USBSCR_OTGHSEN) == (PWR_USBSCR_OTGHSEN)) ? 1UL : 0UL);
}
#endif /* PWR_USBSCR_OTGHSEN */

/**
  * @brief  Enable the wake up pin_x.
  * @rmtoll WUCR    WUPENx       LL_PWR_EnableWakeUpPin
  * @param  WakeUpPin This parameter can be a combination of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableWakeUpPin(uint32_t WakeUpPin)
{
  SET_BIT(PWR->WUCR, WakeUpPin);
}

/**
  * @brief  Disable the wake up pin_x.
  * @rmtoll WUCR    WUPENx       LL_PWR_DisableWakeUpPin
  * @param  WakeUpPin This parameter can be a combination of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableWakeUpPin(uint32_t WakeUpPin)
{
  CLEAR_BIT(PWR->WUCR, WakeUpPin);
}

/**
  * @brief  Check if the wake up pin_x is enabled.
  * @rmtoll WUCR    WUPPx       LL_PWR_IsEnabledWakeUpPin
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledWakeUpPin(uint32_t WakeUpPin)
{
  return ((READ_BIT(PWR->WUCR, WakeUpPin) == (WakeUpPin)) ? 1UL : 0UL);
}

/**
  * @brief  Set the Wake-Up pin polarity low for the event detection
  * @rmtoll WUCR   WKUPP1       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP2       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP3       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP4       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP5       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP6       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP7       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WKUPP8       LL_PWR_SetWakeUpPinPolarityLow
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetWakeUpPinPolarityLow(uint32_t WakeUpPin)
{
  SET_BIT(PWR->WUCR, (WakeUpPin << PWR_WUCR_WUPP1_Pos));
}

/**
  * @brief  Set the Wake-Up pin polarity high for the event detection
  * @rmtoll WUCR   WKUPP1       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP2       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP3       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP4       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP5       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP6       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP7       LL_PWR_SetWakeUpPinPolarityHigh\n
  *         WUCR   WKUPP8       LL_PWR_SetWakeUpPinPolarityHigh
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetWakeUpPinPolarityHigh(uint32_t WakeUpPin)
{
  CLEAR_BIT(PWR->WUCR, (WakeUpPin << PWR_WUCR_WUPP1_Pos));
}

/**
  * @brief  Get the Wake-Up pin polarity for the event detection
  * @rmtoll WUCR   WUPP1       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP2       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP3       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP4       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP5       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP6       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP7       LL_PWR_SetWakeUpPinPolarityLow\n
  *         WUCR   WUPP8       LL_PWR_SetWakeUpPinPolarityLow
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsWakeUpPinPolarityLow(uint32_t WakeUpPin)
{
  return ((READ_BIT(PWR->WUCR, (WakeUpPin << PWR_WUCR_WUPP1_Pos)) == (WakeUpPin << PWR_WUCR_WUPP1_Pos)) ? 1UL : 0UL);
}

/**
  * @brief  Set the Wake-Up pin Pull None
  * @rmtoll WUCR   WUPPUPD1       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD2       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD3       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD4       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD5       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD6       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD7       LL_PWR_SetWakeUpPinPullNone\n
  *         WUCR   WUPPUPD8       LL_PWR_SetWakeUpPinPullNone
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  *
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetWakeUpPinPullNone(uint32_t WakeUpPin)
{
  MODIFY_REG(PWR->WUCR,
             (PWR_WUCR_WUPPUPD1 << ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * (POSITION_VAL(WakeUpPin) & 0xFU)) & \
                                    LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)),
             (LL_PWR_WAKEUP_PIN_NOPULL << ((PWR_WUCR_WUPPUPD1_Pos + ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * \
                                                                      POSITION_VAL(WakeUpPin)) & 0xFU)) & \
                                           LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)));
}

/**
  * @brief  Set the Wake-Up pin Pull Up
  * @rmtoll WUCR   WUPPUPD1       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD2       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD3       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD4       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD5       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD6       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD7       LL_PWR_SetWakeUpPinPullUp\n
  *         WUCR   WUPPUPD8       LL_PWR_SetWakeUpPinPullUp
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  *
  *
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetWakeUpPinPullUp(uint32_t WakeUpPin)
{
  MODIFY_REG(PWR->WUCR,
             (PWR_WUCR_WUPPUPD1 << ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * (POSITION_VAL(WakeUpPin) & 0xFU)) & \
                                    LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)),
             (LL_PWR_WAKEUP_PIN_PULLUP << ((PWR_WUCR_WUPPUPD1_Pos + ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * \
                                                                      POSITION_VAL(WakeUpPin)) & 0xFU)) & \
                                           LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)));
}

/**
  * @brief  Set the Wake-Up pin Pull Down
  * @rmtoll WUCR   WUPPUPD1       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD2       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD3       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD4       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD5       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD6       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD7       LL_PWR_SetWakeUpPinPullDown\n
  *         WUCR   WUPPUPD8       LL_PWR_SetWakeUpPinPullDown
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  *
  * @retval None
  */
__STATIC_INLINE void LL_PWR_SetWakeUpPinPullDown(uint32_t WakeUpPin)
{
  MODIFY_REG(PWR->WUCR,
             (PWR_WUCR_WUPPUPD1 << ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * (POSITION_VAL(WakeUpPin) & 0xFU)) & \
                                    LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)),
             (LL_PWR_WAKEUP_PIN_PULLDOWN << ((PWR_WUCR_WUPPUPD1_Pos + ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * \
                                                                        POSITION_VAL(WakeUpPin)) & 0xFU)) & \
                                             LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)));
}

/**
  * @brief  Get the Wake-Up pin pull
  * @rmtoll WUCR   WUPPUPD1       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD2       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD3       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD4       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD5       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD7       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD7       LL_PWR_GetWakeUpPinPull\n
  *         WUCR   WUPPUPD8       LL_PWR_GetWakeUpPinPull
  * @param  WakeUpPin This parameter can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1
  *         @arg @ref LL_PWR_WAKEUP_PIN2
  *         @arg @ref LL_PWR_WAKEUP_PIN3
  *         @arg @ref LL_PWR_WAKEUP_PIN4
  *         @arg @ref LL_PWR_WAKEUP_PIN5
  *         @arg @ref LL_PWR_WAKEUP_PIN6
  *         @arg @ref LL_PWR_WAKEUP_PIN7
  *         @arg @ref LL_PWR_WAKEUP_PIN8
  *
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN_NOPULL
  *         @arg @ref LL_PWR_WAKEUP_PIN_PULLUP
  *         @arg @ref LL_PWR_WAKEUP_PIN_PULLDOWN
  */
__STATIC_INLINE uint32_t LL_PWR_GetWakeUpPinPull(uint32_t WakeUpPin)
{
  uint32_t regValue = READ_BIT(PWR->WUCR, (PWR_WUCR_WUPPUPD1 << ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * \
                                                                  (POSITION_VAL(WakeUpPin) & 0xFU)) & \
                                                                 LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK)));

  return (uint32_t)(regValue >> ((PWR_WUCR_WUPPUPD1_Pos + ((LL_PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET * \
                                                            POSITION_VAL(WakeUpPin)) & 0xFU)) & \
                                 LL_PWR_WAKEUP_PINS_MAX_SHIFT_MASK));
}

/**
  * @brief  Enable IO Retention
  * @rmtoll IORETR        IORETEN           LL_PWR_EnableIORetention
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableIORetention(void)
{
  SET_BIT(PWR->IORETR, PWR_IORETR_IORETEN);
}

/**
  * @brief  Disable IO Retention
  * @rmtoll IORETR        IORETEN           LL_PWR_DisableIORetention
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableIORetention(void)
{
  CLEAR_BIT(PWR->IORETR, PWR_IORETR_IORETEN);
}

/**
  * @brief  Check if IO Retention is enabled
  * @rmtoll IORETR        IORETEN           LL_PWR_IsEnabledIORetention
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledIORetention(void)
{
  return ((READ_BIT(PWR->IORETR, PWR_IORETR_IORETEN) == (PWR_IORETR_IORETEN)) ? 1UL : 0UL);
}

/**
  * @brief  Enable JTAGIO Retention
  * @rmtoll JTAGIORETR    JTAGIORETEN       LL_PWR_EnableJTAGIORetention
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableJTAGIORetention(void)
{
  SET_BIT(PWR->IORETR, PWR_IORETR_JTAGIORETEN);
}

/**
  * @brief  Disable JTAGIO Retention
  * @rmtoll JTAGIORETR     JTAGIORETEN      LL_PWR_DisableJTAGIORetention
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableJTAGIORetention(void)
{
  CLEAR_BIT(PWR->IORETR, PWR_IORETR_JTAGIORETEN);
}

/**
  * @brief  Check if JTAGIO Retention is enabled
  * @rmtoll IORETR     JTAGIORETEN          LL_PWR_IsEnabledJTAGIORetention
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledJTAGIORetention(void)
{
  return ((READ_BIT(PWR->IORETR, PWR_IORETR_JTAGIORETEN) == (PWR_IORETR_JTAGIORETEN)) ? 1UL : 0UL);
}
/**
  * @}
  */

/** @defgroup PWR_LL_EF_FLAG_MANAGEMENT PWR FLAG Management
  * @{
  */

/**
  * @brief  Indicate whether the regulator voltage output is above voltage
  *         scaling range or not.
  * @rmtoll VOSSR          VOSRDY          LL_PWR_IsActiveFlag_VOS
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VOS(void)
{
  return ((READ_BIT(PWR->VOSSR, PWR_VOSSR_VOSRDY) == (PWR_VOSSR_VOSRDY)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the system was in standby mode or not.
  * @rmtoll PMSR          SBF          LL_PWR_IsActiveFlag_SB
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_SB(void)
{
  return ((READ_BIT(PWR->PMSR, PWR_PMSR_SBF) == (PWR_PMSR_SBF)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the system was in stop mode or not.
  * @rmtoll PMSR          STOPF          LL_PWR_IsActiveFlag_STOP
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_STOP(void)
{
  return ((READ_BIT(PWR->PMSR, PWR_PMSR_STOPF) == (PWR_PMSR_STOPF)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the VDD voltage is below the threshold or not.
  * @rmtoll VMSR          PVDO          LL_PWR_IsActiveFlag_PVDO
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_PVDO(void)
{
  return ((READ_BIT(PWR->VMSR, PWR_VMSR_PVDO) == (PWR_VMSR_PVDO)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the VDDA voltage is below the threshold or not.
  * @rmtoll VMSR          AVDO          LL_PWR_IsActiveFlag_AVDO
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_AVDO(void)
{
  return ((READ_BIT(PWR->VMSR, PWR_VMSR_AVDO) == (PWR_VMSR_AVDO)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the regulator voltage output is equal to current
  *         used voltage scaling range or not.
  * @rmtoll VOSSR          ACTVOSRDY          LL_PWR_IsActiveFlag_ACTVOS
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_ACTVOS(void)
{
  return ((READ_BIT(PWR->VOSSR, PWR_VOSSR_ACTVOSRDY) == (PWR_VOSSR_ACTVOSRDY)) ? 1UL : 0UL);
}

#if defined (PWR_VMSR_USB33RDY)
/**
  * @brief  Indicate whether the VDDUSB is below the threshold of monitor or not.
  * @rmtoll VMSR          USB33RDY          LL_PWR_IsActiveFlag_VDDUSB
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VDDUSB(void)
{
  return ((READ_BIT(PWR->VMSR, PWR_VMSR_USB33RDY) == (PWR_VMSR_USB33RDY)) ? 1UL : 0UL);
}
#endif /* PWR_VMSR_USB33RDY */

/**
  * @brief  Indicate whether VDDMMC voltage is below 1V2
  * @rmtoll VMSR   VDDIO2RDY     LL_PWR_IsActiveFlag_VDDIO2
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VDDIO2(void)
{
  return ((READ_BIT(PWR->VMCR, PWR_VMSR_VDDIO2RDY) == (PWR_VMSR_VDDIO2RDY)) ? 1UL : 0UL);
}

/**
  * @brief  Get Backup Regulator ready Flag
  * @rmtoll BDSR   BRRDY       LL_PWR_IsActiveFlag_BRR
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_BRR(void)
{
  return ((READ_BIT(PWR->BDSR, PWR_BDSR_BRRDY) == (PWR_BDSR_BRRDY)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the VBAT level is below high threshold or not.
  * @rmtoll BDSR   VBATL       LL_PWR_IsActiveFlag_VBATL
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VBATL(void)
{
  return ((READ_BIT(PWR->BDSR, PWR_BDSR_VBATL) == (PWR_BDSR_VBATL)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the VBAT level is below high threshold or not.
  * @rmtoll BDSR   VBATH       LL_PWR_IsActiveFlag_VBATH
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_VBATH(void)
{
  return ((READ_BIT(PWR->BDSR, PWR_BDSR_VBATH) == (PWR_BDSR_VBATH)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the CPU temperature level is above low threshold or
  *         not.
  * @rmtoll BDSR   TEMPL       LL_PWR_IsActiveFlag_TEMPL
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_TEMPL(void)
{
  return ((READ_BIT(PWR->BDSR, PWR_BDSR_TEMPL) == (PWR_BDSR_TEMPL)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether the CPU temperature level is below high threshold
  *         or not.
  * @rmtoll BDSR   TEMPH       LL_PWR_IsActiveFlag_TEMPH
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_TEMPH(void)
{
  return ((READ_BIT(PWR->BDSR, PWR_BDSR_TEMPH) == (PWR_BDSR_TEMPH)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 1.
  * @rmtoll WUSR          WUF1          LL_PWR_IsActiveFlag_WU1
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU1(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF1) == (PWR_WUSR_WUF1)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 2.
  * @rmtoll WUSR          WUF2          LL_PWR_IsActiveFlag_WU2
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU2(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF2) == (PWR_WUSR_WUF2)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 3.
  * @rmtoll WUSR          WUF3          LL_PWR_IsActiveFlag_WU3
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU3(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF3) == (PWR_WUSR_WUF3)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 4.
  * @rmtoll WUSR          WUF4          LL_PWR_IsActiveFlag_WU4
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU4(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF4) == (PWR_WUSR_WUF4)) ? 1UL : 0UL);
}

/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 5.
  * @rmtoll WUSR          WUF5          LL_PWR_IsActiveFlag_WU5
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU5(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF5) == (PWR_WUSR_WUF5)) ? 1UL : 0UL);
}

#if defined (PWR_WUSR_WUF6)
/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 6.
  * @rmtoll WUSR          WUF6          LL_PWR_IsActiveFlag_WU6
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU6(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF6) == (PWR_WUSR_WUF6)) ? 1UL : 0UL);
}
#endif /* PWR_WUSR_WUF6 */

#if defined (PWR_WUSR_WUF7)
/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 7.
  * @rmtoll WUSR          WUF7          LL_PWR_IsActiveFlag_WU7
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU7(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF7) == (PWR_WUSR_WUF7)) ? 1UL : 0UL);
}
#endif /* PWR_WUSR_WUF7 */

#if defined (PWR_WUSR_WUF8)
/**
  * @brief  Indicate whether a wakeup event is detected on wake up pin 8.
  * @rmtoll WUSR          WUF8          LL_PWR_IsActiveFlag_WU8
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsActiveFlag_WU8(void)
{
  return ((READ_BIT(PWR->WUSR, PWR_WUSR_WUF8) == (PWR_WUSR_WUF8)) ? 1UL : 0UL);
}
#endif /* PWR_WUSR_WUF8 */

/**
  * @brief  Clear stop flag.
  * @rmtoll PMCR          CSSF          LL_PWR_ClearFlag_STOP
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_STOP(void)
{
  WRITE_REG(PWR->PMCR, PWR_PMCR_CSSF);
}

/**
  * @brief  Clear standby flag.
  * @rmtoll PMCR          CSSF          LL_PWR_ClearFlag_SB
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_SB(void)
{
  WRITE_REG(PWR->PMCR, PWR_PMCR_CSSF);
}

/**
  * @brief  Clear wake up flag 1.
  * @rmtoll WUSCR          CWUF1         LL_PWR_ClearFlag_WU1
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU1(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF1);
}

/**
  * @brief  Clear wake up flag 2.
  * @rmtoll WUSCR          CWUF2         LL_PWR_ClearFlag_WU2
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU2(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF2);
}

/**
  * @brief  Clear wake up flag 3.
  * @rmtoll WUSCR          CWUF3         LL_PWR_ClearFlag_WU3
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU3(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF3);
}

/**
  * @brief  Clear wake up flag 4.
  * @rmtoll WUSCR          CWUF4         LL_PWR_ClearFlag_WU4
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU4(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF4);
}

/**
  * @brief  Clear wake up flag 5.
  * @rmtoll WUSCR          CWUF5         LL_PWR_ClearFlag_WU5
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU5(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF5);
}

#if defined (PWR_WUSCR_CWUF6)
/**
  * @brief  Clear wake up flag 6.
  * @rmtoll WUSCR          CWUF6         LL_PWR_ClearFlag_WU6
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU6(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF6);
}
#endif /* PWR_WUSCR_CWUF6 */

#if defined (PWR_WUSCR_CWUF7)
/**
  * @brief  Clear wake up flag 7.
  * @rmtoll WUSCR          CWUF7         LL_PWR_ClearFlag_WU7
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU7(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF7);
}
#endif /* PWR_WUSCR_CWUF7 */

#if defined (PWR_WUSCR_CWUF8)
/**
  * @brief  Clear wake up flag 8.
  * @rmtoll WUSCR          CWUF8         LL_PWR_ClearFlag_WU8
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU8(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF8);
}
#endif /* PWR_WUSCR_CWUF8 */

/**
  * @brief  Clear all wake up flags.
  * @rmtoll WUSCR          CWUF         LL_PWR_ClearFlag_WU
  * @retval None
  */
__STATIC_INLINE void LL_PWR_ClearFlag_WU(void)
{
  WRITE_REG(PWR->WUSCR, PWR_WUSCR_CWUF);
}
/**
  * @}
  */

/** @defgroup PWR_LL_EF_ATTRIBUTE_MANAGEMENT PWR Attribute Management
  * @{
  */

#if defined(PWR_PRIVCFGR_NSPRIV)
/**
  * @brief  Enable privileged mode for nsecure items.
  * @rmtoll PRIVCFGR     NSPRIV        LL_PWR_EnableNSecurePrivilege
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableNSecurePrivilege(void)
{
  SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV);
}

/**
  * @brief  Disable privileged mode for nsecure items.
  * @rmtoll PRIVCFGR     NSPRIV          LL_PWR_DisableNSecurePrivilege
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableNSecurePrivilege(void)
{
  CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV);
}

/**
  * @brief  Check if privileged mode for nsecure items is enabled.
  * @rmtoll PRIVCFGR     NSPRIV          LL_PWR_IsEnabledNSecurePrivilege
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledNSecurePrivilege(void)
{
  return ((READ_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_NSPRIV) == PWR_PRIVCFGR_NSPRIV) ? 1UL : 0UL);
}
#else
/**
  * @brief  Enable privileged mode for nsecure items.
  * @rmtoll PRIVCFGR     NSPRIV        LL_PWR_EnableNSecurePrivilege
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableNSecurePrivilege(void)
{
  SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_PRIV);
}

/**
  * @brief  Disable privileged mode for nsecure items.
  * @rmtoll PRIVCFGR     NSPRIV          LL_PWR_DisableNSecurePrivilege
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableNSecurePrivilege(void)
{
  CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_PRIV);
}

/**
  * @brief  Check if privileged mode for nsecure items is enabled.
  * @rmtoll PRIVCFGR     NSPRIV          LL_PWR_IsEnabledNSecurePrivilege
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledNSecurePrivilege(void)
{
  return ((READ_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_PRIV) == PWR_PRIVCFGR_PRIV) ? 1UL : 0UL);
}
#endif /* RCC_PRIVCFGR_NSPRIV */

#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Enable privileged mode for secure items.
  * @rmtoll PRIVCFGR     SPRIV        LL_PWR_EnableSecurePrivilege
  * @retval None
  */
__STATIC_INLINE void LL_PWR_EnableSecurePrivilege(void)
{
  SET_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_SPRIV);
}

/**
  * @brief  Disable privileged mode for secure items.
  * @rmtoll PRIVCFGR     SPRIV          LL_PWR_DisableSecurePrivilege
  * @retval None
  */
__STATIC_INLINE void LL_PWR_DisableSecurePrivilege(void)
{
  CLEAR_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_SPRIV);
}
#endif /* defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U) */

#if defined (PWR_PRIVCFGR_SPRIV)
/**
  * @brief  Check if privileged mode for secure items is enabled.
  * @rmtoll PRIVCFGR     SPRIV          LL_PWR_IsEnabledSecurePrivilege
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t LL_PWR_IsEnabledSecurePrivilege(void)
{
  return ((READ_BIT(PWR->PRIVCFGR, PWR_PRIVCFGR_SPRIV) == PWR_PRIVCFGR_SPRIV) ? 1UL : 0UL);
}
#endif /* PWR_PRIVCFGR_SPRIV */

#if defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure secure attribute mode.
  * @note   This API can be executed only by CPU in secure mode.
  * @rmtoll SECCFGR     WUP1SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP2SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP3SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP4SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP5SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP6SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP7SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     WUP8SEC       LL_PWR_ConfigSecure\n
  *         SECCFGR     RETSEC        LL_PWR_ConfigSecure\n
  *         SECCFGR     LPMSEC        LL_PWR_ConfigSecure\n
  *         SECCFGR     VDMSEC        LL_PWR_ConfigSecure\n
  *         SECCFGR     VBSEC         LL_PWR_ConfigSecure\n
  *         SECCFGR     APCSEC        LL_PWR_ConfigSecure
  * @param  SecureConfig This parameter can be the full combination
  *         of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1_NSEC or LL_PWR_WAKEUP_PIN1_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN2_NSEC or LL_PWR_WAKEUP_PIN2_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN3_NSEC or LL_PWR_WAKEUP_PIN3_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN4_NSEC or LL_PWR_WAKEUP_PIN4_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN5_NSEC or LL_PWR_WAKEUP_PIN5_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN6_NSEC or LL_PWR_WAKEUP_PIN6_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN7_NSEC or LL_PWR_WAKEUP_PIN7_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN8_NSEC or LL_PWR_WAKEUP_PIN8_SEC
  *         @arg @ref LL_PWR_RET_NSEC         or LL_PWR_RET_SEC
  *         @arg @ref LL_PWR_LPM_NSEC         or LL_PWR_LPM_SEC
  *         @arg @ref LL_PWR_VDM_NSEC         or LL_PWR_VDM_SEC
  *         @arg @ref LL_PWR_VB_NSEC          or LL_PWR_VB_SEC
  *         @arg @ref LL_PWR_APC_NSEC         or LL_PWR_APC_SEC
  * @retval None.
  */
__STATIC_INLINE void LL_PWR_ConfigSecure(uint32_t SecureConfig)
{
  WRITE_REG(PWR->SECCFGR, SecureConfig);
}

/**
  * @brief  Get secure attribute configuration.
  * @note   This API can be executed only by CPU in secure mode.
  * @rmtoll SECCFGR     WUP1SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP2SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP3SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP4SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP5SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP6SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP7SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     WUP8SEC       LL_PWR_GetConfigSecure\n
  *         SECCFGR     RETSEC        LL_PWR_ConfigSecure\n
  *         SECCFGR     LPMSEC        LL_PWR_GetConfigSecure\n
  *         SECCFGR     VDMSEC        LL_PWR_GetConfigSecure\n
  *         SECCFGR     VBSEC         LL_PWR_GetConfigSecure\n
  *         SECCFGR     APCSEC        LL_PWR_GetConfigSecure
  * @retval Returned value is the combination of the following values:
  *         @arg @ref LL_PWR_WAKEUP_PIN1_NSEC or LL_PWR_WAKEUP_PIN1_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN2_NSEC or LL_PWR_WAKEUP_PIN2_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN3_NSEC or LL_PWR_WAKEUP_PIN3_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN4_NSEC or LL_PWR_WAKEUP_PIN4_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN5_NSEC or LL_PWR_WAKEUP_PIN5_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN6_NSEC or LL_PWR_WAKEUP_PIN6_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN7_NSEC or LL_PWR_WAKEUP_PIN7_SEC
  *         @arg @ref LL_PWR_WAKEUP_PIN8_NSEC or LL_PWR_WAKEUP_PIN8_SEC
  *         @arg @ref LL_PWR_RET_NSEC         or LL_PWR_RET_SEC
  *         @arg @ref LL_PWR_LPM_NSEC         or LL_PWR_LPM_SEC
  *         @arg @ref LL_PWR_VDM_NSEC         or LL_PWR_VDM_SEC
  *         @arg @ref LL_PWR_VB_NSEC          or LL_PWR_VB_SEC
  *         @arg @ref LL_PWR_APC_NSEC         or LL_PWR_APC_SEC
  */
__STATIC_INLINE uint32_t LL_PWR_GetConfigSecure(void)
{
  return (READ_REG(PWR->SECCFGR));
}
#endif /* defined (__ARM_FEATURE_CMSE) &&  (__ARM_FEATURE_CMSE == 3U) */
/**
  * @}
  */

#if defined (USE_FULL_LL_DRIVER)
/** @defgroup PWR_LL_EF_Init De-initialization function
  * @{
  */
ErrorStatus LL_PWR_DeInit(void);
/**
  * @}
  */
#endif /* defined (USE_FULL_LL_DRIVER) */


/**
  * @}
  */

/**
  * @}
  */

#endif /* defined (PWR) */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32H5xx_LL_PWR_H */
