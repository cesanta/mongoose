/**
  ******************************************************************************
  * @file    stm32h5xx_hal_pwr.h
  * @author  MCD Application Team
  * @brief   Header file of PWR HAL module.
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
#ifndef STM32H5xx_HAL_PWR_H
#define STM32H5xx_HAL_PWR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup PWR
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup PWR_Exported_Types PWR Exported Types
  * @{
  */

/**
  * @brief  PWR PVD configuration structure definition
  */
typedef struct
{
  uint32_t PVDLevel; /*!< Specifies the PVD detection level.
                          This parameter can be a value of
                          @ref PWR_PVD_Detection_Level.                       */

  uint32_t Mode;     /*!< Specifies the operating mode for the selected pins.
                          This parameter can be a value of @ref PWR_PVD_Mode. */
} PWR_PVDTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup PWR_Exported_Constants PWR Exported Constants
  * @{
  */

/** @defgroup PWR_PVD_Detection_Level Programmable Voltage Detection Level
  * @{
  */
#define PWR_PVDLEVEL_0 0x00000000UL                      /*!< PVD threshold around 1.95 V      */
#define PWR_PVDLEVEL_1 (PWR_VMCR_PLS_0)                  /*!< PVD threshold around 2.1  V      */
#define PWR_PVDLEVEL_2 (PWR_VMCR_PLS_1)                  /*!< PVD threshold around 2.25 V      */
#define PWR_PVDLEVEL_3 (PWR_VMCR_PLS_0 | PWR_VMCR_PLS_1) /*!< PVD threshold around 2.4  V      */
#define PWR_PVDLEVEL_4 (PWR_VMCR_PLS_2)                  /*!< PVD threshold around 2.55 V      */
#define PWR_PVDLEVEL_5 (PWR_VMCR_PLS_0 | PWR_VMCR_PLS_2) /*!< PVD threshold around 2.7  V      */
#define PWR_PVDLEVEL_6 (PWR_VMCR_PLS_1 | PWR_VMCR_PLS_2) /*!< PVD threshold around 2.85 V      */
#define PWR_PVDLEVEL_7 (PWR_VMCR_PLS)                    /*!< External input analog voltage
                                                              (compared internally to VREFINT) */
/**
  * @}
  */

/** @defgroup PWR_PVD_Mode PWR PVD Mode
  * @{
  */
#define PWR_PVD_MODE_NORMAL               (0x00U) /*!< Basic Mode is used                                            */
#define PWR_PVD_MODE_IT_RISING            (0x05U) /*!< External Interrupt Mode with Rising edge trigger detection    */
#define PWR_PVD_MODE_IT_FALLING           (0x06U) /*!< External Interrupt Mode with Falling
                                                       edge trigger detection                                        */
#define PWR_PVD_MODE_IT_RISING_FALLING    (0x07U) /*!< External Interrupt Mode with Rising/Falling
                                                       edge trigger detection                                        */
#define PWR_PVD_MODE_EVENT_RISING         (0x09U) /*!< Event Mode with Rising edge trigger detection                 */
#define PWR_PVD_MODE_EVENT_FALLING        (0x0AU) /*!< Event Mode with Falling edge trigger detection                */
#define PWR_PVD_MODE_EVENT_RISING_FALLING (0x0BU) /*!< Event Mode with Rising/Falling edge trigger detection         */
/**
  * @}
  */

/** @defgroup PWR_Regulator_In_LowPower_Mode PWR Regulator State in SLEEP/STOP Mode
  * @{
  */
#define PWR_MAINREGULATOR_ON     (0x00U)  /*!< Main Regulator ON in Run Mode       */
#define PWR_LOWPOWERREGULATOR_ON (0x00U)  /*!< Main Regulator ON in Low Power Mode */
/**
  * @}
  */

/** @defgroup PWR_SLEEP_Mode_Entry PWR SLEEP Mode Entry
  * @{
  */
#define PWR_SLEEPENTRY_WFI (0x01U) /*!< Wait For Interruption instruction to enter Sleep mode */
#define PWR_SLEEPENTRY_WFE (0x02U) /*!< Wait For Event instruction to enter Sleep mode        */
/**
  * @}
  */

/** @defgroup PWR_STOP_Mode_Entry PWR STOP Mode Entry
  * @{
  */
#define PWR_STOPENTRY_WFI (0x01U) /*!< Wait For Interruption instruction to enter Stop mode */
#define PWR_STOPENTRY_WFE (0x02U) /*!< Wait For Event instruction to enter Stop mode        */
/**
  * @}
  */

/** @defgroup PWR_Flags PWR Flags
  * @{
  */
#define PWR_FLAG_STOPF      (0x01U) /*!< STOP flag                                                */
#define PWR_FLAG_SBF        (0x02U) /*!< STANDBY flag                                             */
#define PWR_FLAG_VOSRDY     (0x03U) /*!< Voltage scaling ready flag                               */
#define PWR_FLAG_ACTVOSRDY  (0x04U) /*!< Currently applied VOS ready flag                         */
#define PWR_FLAG_BRR        (0x05U) /*!< Backup regulator ready flag                              */
#define PWR_FLAG_VBATL      (0x06U) /*!< Backup domain voltage level flag (versus low threshold)  */
#define PWR_FLAG_VBATH      (0x07U) /*!< Backup domain voltage level flag (versus high threshold) */
#define PWR_FLAG_TEMPL      (0x08U) /*!< Temperature level flag (versus low threshold)            */
#define PWR_FLAG_TEMPH      (0x09U) /*!< Temperature level flag (versus high threshold)           */
#define PWR_FLAG_AVDO       (0x0AU) /*!< VDDA voltage detector output flag                        */
#define PWR_FLAG_VDDIO2RDY  (0x0BU) /*!< VDDIO2 voltage detector output flag                      */
#define PWR_FLAG_PVDO       (0x0CU) /*!< VDD voltage detector output flag                         */
#define PWR_FLAG_USB33RDY   (0x0DU) /*!< VDDUSB33 ready flag                                      */

#define PWR_WAKEUP_FLAG1    (0x10U) /*!< Wake up line 1 flag                                      */
#define PWR_WAKEUP_FLAG2    (0x20U) /*!< Wake up line 2 flag                                      */
#define PWR_WAKEUP_FLAG3    (0x30U) /*!< Wake up line 3 flag                                      */
#define PWR_WAKEUP_FLAG4    (0x40U) /*!< Wake up line 4 flag                                      */
#define PWR_WAKEUP_FLAG5    (0x50U) /*!< Wake up line 5 flag                                      */
#define PWR_WAKEUP_FLAG6    (0x60U) /*!< Wake up line 6 flag                                      */
#define PWR_WAKEUP_FLAG7    (0x70U) /*!< Wake up line 7 flag                                      */
#define PWR_WAKEUP_FLAG8    (0x80U) /*!< Wake up line 8 flag                                      */
#define PWR_WAKEUP_ALL_FLAG (0x90U) /*!< Wakeup flag all                                          */

/**
  * @}
  */

/** @defgroup PWREx_WakeUp_Pins PWREx Wake-Up Pins
  * @{
  */
/* High level and No pull (default configuration) */
#define PWR_WAKEUP_PIN1      PWR_WUCR_WUPEN1
#define PWR_WAKEUP_PIN2      PWR_WUCR_WUPEN2
#define PWR_WAKEUP_PIN3      PWR_WUCR_WUPEN3
#define PWR_WAKEUP_PIN4      PWR_WUCR_WUPEN4
#define PWR_WAKEUP_PIN5      PWR_WUCR_WUPEN5
#if defined (PWR_WUCR_WUPEN6)
#define PWR_WAKEUP_PIN6      PWR_WUCR_WUPEN6
#define PWR_WAKEUP_PIN7      PWR_WUCR_WUPEN7
#define PWR_WAKEUP_PIN8      PWR_WUCR_WUPEN8
#endif /* PWR_WUCR_WUPEN6 */

/* High level and No pull */
#define PWR_WAKEUP_PIN1_HIGH PWR_WUCR_WUPEN1
#define PWR_WAKEUP_PIN2_HIGH PWR_WUCR_WUPEN2
#define PWR_WAKEUP_PIN3_HIGH PWR_WUCR_WUPEN3
#define PWR_WAKEUP_PIN4_HIGH PWR_WUCR_WUPEN4
#define PWR_WAKEUP_PIN5_HIGH PWR_WUCR_WUPEN5
#if defined (PWR_WUCR_WUPEN6)
#define PWR_WAKEUP_PIN6_HIGH PWR_WUCR_WUPEN6
#define PWR_WAKEUP_PIN7_HIGH PWR_WUCR_WUPEN7
#define PWR_WAKEUP_PIN8_HIGH PWR_WUCR_WUPEN8
#endif /* PWR_WUCR_WUPEN6 */

/* Low level and No pull */
#define PWR_WAKEUP_PIN1_LOW (PWR_WUCR_WUPP1 | PWR_WUCR_WUPEN1)
#define PWR_WAKEUP_PIN2_LOW (PWR_WUCR_WUPP2 | PWR_WUCR_WUPEN2)
#define PWR_WAKEUP_PIN3_LOW (PWR_WUCR_WUPP3 | PWR_WUCR_WUPEN3)
#define PWR_WAKEUP_PIN4_LOW (PWR_WUCR_WUPP4 | PWR_WUCR_WUPEN4)
#define PWR_WAKEUP_PIN5_LOW (PWR_WUCR_WUPP5 | PWR_WUCR_WUPEN5)
#if defined (PWR_WUCR_WUPEN6)
#define PWR_WAKEUP_PIN6_LOW (PWR_WUCR_WUPP6 | PWR_WUCR_WUPEN6)
#define PWR_WAKEUP_PIN7_LOW (PWR_WUCR_WUPP7 | PWR_WUCR_WUPEN7)
#define PWR_WAKEUP_PIN8_LOW (PWR_WUCR_WUPP8 | PWR_WUCR_WUPEN8)
#endif /* PWR_WUCR_WUPEN6 */

/**
  * @}
  */

/** @defgroup PWR_Items PWR Items
  * @{
  */
#if defined(PWR_SECCFGR_WUP1SEC)
#define PWR_WKUP1 (PWR_SECCFGR_WUP1SEC) /*!< WUP1 secure protection                             */
#define PWR_WKUP2 (PWR_SECCFGR_WUP2SEC) /*!< WUP2 secure protection                             */
#define PWR_WKUP3 (PWR_SECCFGR_WUP3SEC) /*!< WUP3 secure protection                             */
#define PWR_WKUP4 (PWR_SECCFGR_WUP4SEC) /*!< WUP4 secure protection                             */
#define PWR_WKUP5 (PWR_SECCFGR_WUP5SEC) /*!< WUP5 secure protection                             */
#define PWR_WKUP6 (PWR_SECCFGR_WUP6SEC) /*!< WUP6 secure protection                             */
#define PWR_WKUP7 (PWR_SECCFGR_WUP7SEC) /*!< WUP7 secure protection                             */
#define PWR_WKUP8 (PWR_SECCFGR_WUP8SEC) /*!< WUP8 secure protection                             */
#define PWR_RET   (PWR_SECCFGR_RETSEC)  /*!< IO Retention secure protection                     */
#define PWR_LPM   (PWR_SECCFGR_LPMSEC)  /*!< Low power modes secure protection                  */
#define PWR_SCM   (PWR_SECCFGR_SCMSEC)  /*!< Voltage detection and monitoring secure protection */
#define PWR_VB    (PWR_SECCFGR_VBSEC)   /*!< Backup domain secure protection                    */
#define PWR_VUSB  (PWR_SECCFGR_VUSBSEC) /*!< Voltage USB secure protection                      */
#define PWR_ALL   (PWR_WKUP1 | PWR_WKUP2 | PWR_WKUP3 | PWR_WKUP4    | \
                   PWR_WKUP5 | PWR_WKUP6 | PWR_WKUP7 | PWR_WKUP8    | \
                   PWR_LPM   | PWR_SCM   | PWR_VB    | PWR_VUSB     | \
                   PWR_RET)
#else
#define PWR_ALL    0xFF                 /*!< Dummy Value                                        */
#endif /* PWR_SECCFGR_WUP1SEC */

/**
  * @}
  */

/** @defgroup PWR_Attributes PWR Attributes
  * @brief PWR Privilege/NPrivilege and Secure/NSecure Attributes
  * @{
  */
#if defined(PWR_PRIVCFGR_NSPRIV)
#define PWR_NSEC_PRIV  (PWR_ITEM_ATTR_NSEC_PRIV_MASK | 0x01U) /*!< NSecure and Privileged attribute  */
#define PWR_NSEC_NPRIV (PWR_ITEM_ATTR_NSEC_PRIV_MASK)         /*!< NSecure and NPrivileged attribute */
#else
#define PWR_PRIV       (PWR_ITEM_ATTR_NSEC_PRIV_MASK | 0x01U) /*!<  Privileged attribute             */
#define PWR_NPRIV      (PWR_ITEM_ATTR_NSEC_PRIV_MASK)         /*!<  NPrivileged attribute            */
#endif /* PWR_PRIVCFGR_NSPRIV */
#define PWR_SEC_PRIV   (PWR_ITEM_ATTR_SEC_PRIV_MASK  | 0x02U) /*!< Secure and Privileged attribute   */
#define PWR_SEC_NPRIV  (PWR_ITEM_ATTR_SEC_PRIV_MASK)          /*!< Secure and NPrivileged attribute  */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup PWR_Exported_Macros PWR Exported Macros
  * @{
  */

/** @brief  Check PWR flags are set or not.
  * @param  __FLAG__ : Specifies the flag to check.
  *                    This parameter can be one of the following values :
  *                    @arg @ref PWR_FLAG_STOPF     : Stop flag.
  *                                                   Indicates that the device was resumed from Stop mode.
  *                    @arg @ref PWR_FLAG_SBF       : Standby flag.
  *                                                   Indicates that the device was resumed from Standby mode.
  *                    @arg @ref PWR_FLAG_VOSRDY    : Voltage scaling ready flag.
  *                                                   Indicates that the Vcore level at or above VOS selected level.
  *                    @arg @ref PWR_FLAG_ACTVOSRDY : Currently applied VOS ready flag.
  *                                                   Indicates that Vcore is equal to the current
  *                                                   voltage scaling provided by ACTVOS.
  *                    @arg @ref PWR_FLAG_BRR       : Backup regulator ready flag. This bit is not
  *                                                   reset when the device wakes up from STANDBY
  *                                                   mode or by a system reset or power-on reset.
  *                    @arg @ref PWR_FLAG_VBATL     : Backup domain voltage level flag (versus low threshold).
  *                                                   Indicates the backup domain voltage
  *                                                   level is equal or above low threshold.
  *                    @arg @ref PWR_FLAG_VBATH     : Backup domain voltage level flag (versus high threshold).
  *                                                   Indicates the backup domain voltage
  *                                                   level is equal or above high threshold.
  *                    @arg @ref PWR_FLAG_TEMPL     : Temperature level flag (versus low threshold).
  *                                                   Indicates the temperature is equal or above low threshold.
  *                    @arg @ref PWR_FLAG_TEMPH     : Temperature level flag (versus high threshold).
  *                                                   Indicates the temperature is equal or above high threshold.
  *                    @arg @ref PWR_FLAG_AVDO      : Regulator selection flag.
  *                                                   Indicates the regulator selected.
  *                    @arg @ref PWR_FLAG_VDDIO2RDY : VDDIO2 ready flag (versus 0.9 V threshold).
  *                                                   Indicates that VDDIO2 is equal or above the threshold
  *                                                   of the VDDIO2 voltage monitor (around 0.9 V).
  *                    @arg @ref PWR_FLAG_PVDO      : Voltage detector output flag.
  *                                                   Indicates that Vdd is equal or above
  *                                                   the PVD threshold selected by PVDLS.
  *                    @arg @ref PWR_FLAG_USB33RDY  : VDDUSB ready flag (versus 1.2 V threshold).
  *                                                   Indicates that VDDUSB is equal or above the threshold
  *                                                   of the VDDUSB voltage monitor (around 1.2 V).
  *                    @arg @ref PWR_WAKEUP_FLAG1   : Wakeup flag 1.
  *                                                   Indicates that a wakeup event was received from the WKUP line 1.
  *                    @arg @ref PWR_WAKEUP_FLAG2   : Wakeup flag 2.
  *                                                   Indicates that a wakeup event was received from the WKUP line 2.
  *                    @arg @ref PWR_WAKEUP_FLAG3   : Wakeup flag 3.
  *                                                   Indicates that a wakeup event was received from the WKUP line 3.
  *                    @arg @ref PWR_WAKEUP_FLAG4   : Wakeup flag 4.
  *                                                   Indicates that a wakeup event was received from the WKUP line 4.
  *                    @arg @ref PWR_WAKEUP_FLAG5   : Wakeup flag 5.
  *                                                   Indicates that a wakeup event was received from the WKUP line 5.
  *                    @arg @ref PWR_WAKEUP_FLAG6   : Wakeup flag 6.
  *                                                   Indicates that a wakeup event was received from the WKUP line 6.
  *                    @arg @ref PWR_WAKEUP_FLAG7   : Wakeup flag 7.
  *                                                   Indicates that a wakeup event was received from the WKUP line 7.
  *                    @arg @ref PWR_WAKEUP_FLAG8   : Wakeup flag 8.
  *                                                   Indicates that a wakeup event was received from the WKUP line 8.
  * @note   The PWR_WAKEUP_FLAG6, PWR_WAKEUP_FLAG7 AND PWR_WAKEUP_FLAG8 are not available for STM32H503xx devices.
  * @retval The state of __FLAG__ (TRUE or FALSE).
  */
#if defined (PWR_WUSR_WUF6)
#define __HAL_PWR_GET_FLAG(__FLAG__)                                                                           \
  (((__FLAG__) == PWR_FLAG_STOPF)     ? (READ_BIT(PWR->PMSR, PWR_PMSR_STOPF)           == PWR_PMSR_STOPF)      : \
   ((__FLAG__) == PWR_FLAG_SBF)       ? (READ_BIT(PWR->PMSR, PWR_PMSR_SBF)             == PWR_PMSR_SBF)        : \
   ((__FLAG__) == PWR_FLAG_VOSRDY)    ? (READ_BIT(PWR->VOSSR, PWR_VOSSR_VOSRDY)        == PWR_VOSSR_VOSRDY)    : \
   ((__FLAG__) == PWR_FLAG_ACTVOSRDY) ? (READ_BIT(PWR->VOSSR, PWR_VOSSR_ACTVOSRDY)     == PWR_VOSSR_ACTVOSRDY) : \
   ((__FLAG__) == PWR_FLAG_BRR)       ? (READ_BIT(PWR->BDSR, PWR_BDSR_BRRDY)           == PWR_BDSR_BRRDY)      : \
   ((__FLAG__) == PWR_FLAG_VBATL)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_VBATL)           == PWR_BDSR_VBATL)      : \
   ((__FLAG__) == PWR_FLAG_VBATH)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_VBATH)           == PWR_BDSR_VBATH)      : \
   ((__FLAG__) == PWR_FLAG_TEMPL)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_TEMPL)           == PWR_BDSR_TEMPL)      : \
   ((__FLAG__) == PWR_FLAG_TEMPH)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_TEMPH)           == PWR_BDSR_TEMPH)      : \
   ((__FLAG__) == PWR_FLAG_AVDO)      ? (READ_BIT(PWR->VMSR, PWR_VMSR_AVDO)            == PWR_VMSR_AVDO)       : \
   ((__FLAG__) == PWR_FLAG_VDDIO2RDY) ? (READ_BIT(PWR->VMSR, PWR_VMSR_VDDIO2RDY)       == PWR_VMSR_VDDIO2RDY)  : \
   ((__FLAG__) == PWR_FLAG_PVDO)      ? (READ_BIT(PWR->VMSR, PWR_VMSR_PVDO)            == PWR_VMSR_PVDO)       : \
   ((__FLAG__) == PWR_FLAG_USB33RDY)  ? (READ_BIT(PWR->VMSR, PWR_VMSR_USB33RDY)        == PWR_VMSR_USB33RDY)   : \
   ((__FLAG__) == PWR_WAKEUP_FLAG1)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF1)            == PWR_WUSR_WUF1)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG2)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF2)            == PWR_WUSR_WUF2)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG3)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF3)            == PWR_WUSR_WUF3)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG4)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF4)            == PWR_WUSR_WUF4)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG5)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF5)            == PWR_WUSR_WUF5)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG6)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF6)            == PWR_WUSR_WUF6)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG7)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF7)            == PWR_WUSR_WUF7)       : \
   (READ_BIT(PWR->WUSR, PWR_WUSR_WUF8) == PWR_WUSR_WUF8))
#else
#define __HAL_PWR_GET_FLAG(__FLAG__)                                                                           \
  (((__FLAG__) == PWR_FLAG_STOPF)     ? (READ_BIT(PWR->PMSR, PWR_PMSR_STOPF)           == PWR_PMSR_STOPF)      : \
   ((__FLAG__) == PWR_FLAG_SBF)       ? (READ_BIT(PWR->PMSR, PWR_PMSR_SBF)             == PWR_PMSR_SBF)        : \
   ((__FLAG__) == PWR_FLAG_VOSRDY)    ? (READ_BIT(PWR->VOSSR, PWR_VOSSR_VOSRDY)        == PWR_VOSSR_VOSRDY)    : \
   ((__FLAG__) == PWR_FLAG_ACTVOSRDY) ? (READ_BIT(PWR->VOSSR, PWR_VOSSR_ACTVOSRDY)     == PWR_VOSSR_ACTVOSRDY) : \
   ((__FLAG__) == PWR_FLAG_BRR)       ? (READ_BIT(PWR->BDSR, PWR_BDSR_BRRDY)           == PWR_BDSR_BRRDY)      : \
   ((__FLAG__) == PWR_FLAG_VBATL)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_VBATL)           == PWR_BDSR_VBATL)      : \
   ((__FLAG__) == PWR_FLAG_VBATH)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_VBATH)           == PWR_BDSR_VBATH)      : \
   ((__FLAG__) == PWR_FLAG_TEMPL)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_TEMPL)           == PWR_BDSR_TEMPL)      : \
   ((__FLAG__) == PWR_FLAG_TEMPH)     ? (READ_BIT(PWR->BDSR, PWR_BDSR_TEMPH)           == PWR_BDSR_TEMPH)      : \
   ((__FLAG__) == PWR_FLAG_AVDO)      ? (READ_BIT(PWR->VMSR, PWR_VMSR_AVDO)            == PWR_VMSR_AVDO)       : \
   ((__FLAG__) == PWR_FLAG_VDDIO2RDY) ? (READ_BIT(PWR->VMSR, PWR_VMSR_VDDIO2RDY)       == PWR_VMSR_VDDIO2RDY)  : \
   ((__FLAG__) == PWR_FLAG_PVDO)      ? (READ_BIT(PWR->VMSR, PWR_VMSR_PVDO)            == PWR_VMSR_PVDO)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG1)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF1)            == PWR_WUSR_WUF1)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG2)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF2)            == PWR_WUSR_WUF2)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG3)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF3)            == PWR_WUSR_WUF3)       : \
   ((__FLAG__) == PWR_WAKEUP_FLAG4)   ? (READ_BIT(PWR->WUSR, PWR_WUSR_WUF4)            == PWR_WUSR_WUF4)       : \
   (READ_BIT(PWR->WUSR, PWR_WUSR_WUF5) == PWR_WUSR_WUF5))
#endif /* PWR_WUSR_WUF6 */

/** @brief  Clear PWR flags.
  * @param  __FLAG__ : Specifies the flag to clear.
  *                    This parameter can be one of the following values :
  *                    @arg @ref PWR_FLAG_STOPF     : STOP flag.
  *                                                   Indicates that the device was resumed from STOP mode.
  *                    @arg @ref PWR_FLAG_SBF       : STANDBY flag.
  *                                                   Indicates that the device was resumed from STANDBY mode.
  *                    @arg @ref PWR_WAKEUP_FLAG1   : Wakeup flag 1.
  *                                                   Indicates that a wakeup event was received from the WKUP line 1.
  *                    @arg @ref PWR_WAKEUP_FLAG2   : Wakeup flag 2.
  *                                                   Indicates that a wakeup event was received from the WKUP line 2.
  *                    @arg @ref PWR_WAKEUP_FLAG3   : Wakeup flag 3.
  *                                                   Indicates that a wakeup event was received from the WKUP line 3.
  *                    @arg @ref PWR_WAKEUP_FLAG4   : Wakeup flag 4.
  *                                                   Indicates that a wakeup event was received from the WKUP line 4.
  *                    @arg @ref PWR_WAKEUP_FLAG5   : Wakeup flag 5.
  *                                                   Indicates that a wakeup event was received from the WKUP line 5.
  *                    @arg @ref PWR_WAKEUP_FLAG6   : Wakeup flag 6.
  *                                                   Indicates that a wakeup event was received from the WKUP line 6.
  *                    @arg @ref PWR_WAKEUP_FLAG7   : Wakeup flag 7.
  *                                                   Indicates that a wakeup event was received from the WKUP line 7.
  *                    @arg @ref PWR_WAKEUP_FLAG8   : Wakeup flag 8.
  *                                                   Indicates that a wakeup event was received from the WKUP line 8.
  * @note   The PWR_WAKEUP_FLAG6, PWR_WAKEUP_FLAG7 AND PWR_WAKEUP_FLAG8 are not available for STM32H503xx devices.
  * @retval None.
  */
#if defined (PWR_WUSCR_CWUF6)
#define __HAL_PWR_CLEAR_FLAG(__FLAG__)                                       \
  (((__FLAG__) == PWR_FLAG_STOPF)   ? (SET_BIT(PWR->PMCR, PWR_PMCR_CSSF))    : \
   ((__FLAG__) == PWR_FLAG_SBF)     ? (SET_BIT(PWR->PMCR, PWR_PMCR_CSSF))    : \
   ((__FLAG__) == PWR_WAKEUP_FLAG1) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF1)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG2) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF2)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG3) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF3)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG4) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF4)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG5) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF5)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG6) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF6)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG7) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF7)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG8) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF8)) : \
   (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF)))
#else
#define __HAL_PWR_CLEAR_FLAG(__FLAG__)                                       \
  (((__FLAG__) == PWR_FLAG_STOPF)   ? (SET_BIT(PWR->PMCR, PWR_PMCR_CSSF))    : \
   ((__FLAG__) == PWR_FLAG_SBF)     ? (SET_BIT(PWR->PMCR, PWR_PMCR_CSSF))    : \
   ((__FLAG__) == PWR_WAKEUP_FLAG1) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF1)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG2) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF2)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG3) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF3)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG4) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF4)) : \
   ((__FLAG__) == PWR_WAKEUP_FLAG5) ? (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF5)) : \
   (SET_BIT(PWR->WUSCR, PWR_WUSCR_CWUF)))
#endif /* PWR_WUSCR_CWUF6 */

/**
  * @brief  Enable the PVD Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_IT() SET_BIT(EXTI->IMR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Disable the PVD Extended Interrupt Line.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_IT() CLEAR_BIT(EXTI->IMR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Enable the PVD Event Line.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_EVENT() SET_BIT(EXTI->EMR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Disable the PVD Event Line.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_EVENT() CLEAR_BIT(EXTI->EMR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Enable the PVD Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE() SET_BIT(EXTI->RTSR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Disable the PVD Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE() CLEAR_BIT(EXTI->RTSR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Enable the PVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE() SET_BIT(EXTI->FTSR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Disable the PVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE() CLEAR_BIT(EXTI->FTSR1, PWR_EXTI_LINE_PVD)

/**
  * @brief  Enable the PVD Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_ENABLE_RISING_FALLING_EDGE()   \
  do                                                      \
  {                                                       \
    __HAL_PWR_PVD_EXTI_ENABLE_RISING_EDGE();              \
    __HAL_PWR_PVD_EXTI_ENABLE_FALLING_EDGE();             \
  } while(0)

/**
  * @brief  Disable the PVD Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_DISABLE_RISING_FALLING_EDGE()   \
  do                                                       \
  {                                                        \
    __HAL_PWR_PVD_EXTI_DISABLE_RISING_EDGE();              \
    __HAL_PWR_PVD_EXTI_DISABLE_FALLING_EDGE();             \
  } while(0)

/**
  * @brief  Generate a Software Interrupt on selected EXTI line.
  * @retval None
  */
#define __HAL_PWR_PVD_EXTI_GENERATE_SWIT() SET_BIT(EXTI->SWIER1, PWR_EXTI_LINE_PVD)

/**
  * @brief Check whether the specified PVD EXTI Rising interrupt flag is set or not.
  * @retval EXTI PVD Line Status.
  */
#define __HAL_PWR_PVD_EXTI_GET_RISING_FLAG() \
  ((READ_BIT(EXTI->RPR1, PWR_EXTI_LINE_PVD) == PWR_EXTI_LINE_PVD) ? 1UL : 0UL)

/**
  * @brief Check whether the specified PVD EXTI Falling interrupt flag is set or not.
  * @retval EXTI PVD Line Status.
  */
#define __HAL_PWR_PVD_EXTI_GET_FALLING_FLAG()\
  ((READ_BIT(EXTI->FPR1, PWR_EXTI_LINE_PVD) == PWR_EXTI_LINE_PVD) ? 1UL : 0UL)

/**
  * @brief  Clear the PVD EXTI Interrupt Rising flag.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_CLEAR_RISING_FLAG() WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_PVD);

/**
  * @brief  Clear the PVD EXTI Interrupt Falling flag.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_CLEAR_FALLING_FLAG() WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_PVD);

/**
  * @brief  Clear the PVD EXTI Interrupt flag.
  * @retval None.
  */
#define __HAL_PWR_PVD_EXTI_CLEAR_FLAG()       \
  do                                          \
  {                                           \
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_PVD); \
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_PVD); \
  } while(0)
/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/

/** @defgroup PWR_Private_Constants PWR Private Constants
  * @{
  */
/* Define PVD extended interrupts and event line */
#define PWR_EXTI_LINE_PVD EXTI_IMR1_IM16 /*!< PVD EXTI Line */

/* Defines wake up lines shift */
#define  PWR_EWUP_MASK (0x0FFF3F3FU)

/* Defines attribute */
#define PWR_ITEM_ATTR_NSEC_PRIV_MASK (0x10U) /*!< NSecure Privilege / NPrivilege attribute item mask */
#define PWR_ITEM_ATTR_SEC_PRIV_MASK (0x20U)  /*!< Secure Privilege / NPrivilege attribute item mask  */
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/

/** @defgroup PWR_Private_Macros PWR Private Macros
  * @{
  */
#if defined(PWR_WUCR_WUPEN6)
/* Check wake up pin parameter */
#define IS_PWR_WAKEUP_PIN(PIN)                                        \
  (((PIN) == PWR_WAKEUP_PIN1)      || ((PIN) == PWR_WAKEUP_PIN2)      ||\
   ((PIN) == PWR_WAKEUP_PIN3)      || ((PIN) == PWR_WAKEUP_PIN4)      ||\
   ((PIN) == PWR_WAKEUP_PIN5)      || ((PIN) == PWR_WAKEUP_PIN6)      ||\
   ((PIN) == PWR_WAKEUP_PIN7)      || ((PIN) == PWR_WAKEUP_PIN8)      ||\
   ((PIN) == PWR_WAKEUP_PIN1_HIGH) || ((PIN) == PWR_WAKEUP_PIN2_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN3_HIGH) || ((PIN) == PWR_WAKEUP_PIN4_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN5_HIGH) || ((PIN) == PWR_WAKEUP_PIN6_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN7_HIGH) || ((PIN) == PWR_WAKEUP_PIN8_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN1_LOW)  || ((PIN) == PWR_WAKEUP_PIN2_LOW)  ||\
   ((PIN) == PWR_WAKEUP_PIN3_LOW)  || ((PIN) == PWR_WAKEUP_PIN4_LOW)  ||\
   ((PIN) == PWR_WAKEUP_PIN5_LOW)  || ((PIN) == PWR_WAKEUP_PIN6_LOW)  ||\
   ((PIN) == PWR_WAKEUP_PIN7_LOW)  || ((PIN) == PWR_WAKEUP_PIN8_LOW))
#else
/* Check wake up pin parameter */
#define IS_PWR_WAKEUP_PIN(PIN)                                        \
  (((PIN) == PWR_WAKEUP_PIN1)      || ((PIN) == PWR_WAKEUP_PIN2)      ||\
   ((PIN) == PWR_WAKEUP_PIN3)      || ((PIN) == PWR_WAKEUP_PIN4)      ||\
   ((PIN) == PWR_WAKEUP_PIN5)      || ((PIN) == PWR_WAKEUP_PIN1_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN2_HIGH) || ((PIN) == PWR_WAKEUP_PIN3_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN4_HIGH) || ((PIN) == PWR_WAKEUP_PIN5_HIGH) ||\
   ((PIN) == PWR_WAKEUP_PIN1_LOW)  || ((PIN) == PWR_WAKEUP_PIN2_LOW)  ||\
   ((PIN) == PWR_WAKEUP_PIN3_LOW)  || ((PIN) == PWR_WAKEUP_PIN4_LOW)  ||\
   ((PIN) == PWR_WAKEUP_PIN5_LOW))
#endif /* PWR_WUCR_WUPEN6 */

/* PVD level check macro */
#define IS_PWR_PVD_LEVEL(LEVEL)                               \
  (((LEVEL) == PWR_PVDLEVEL_0) || ((LEVEL) == PWR_PVDLEVEL_1) ||\
   ((LEVEL) == PWR_PVDLEVEL_2) || ((LEVEL) == PWR_PVDLEVEL_3) ||\
   ((LEVEL) == PWR_PVDLEVEL_4) || ((LEVEL) == PWR_PVDLEVEL_5) ||\
   ((LEVEL) == PWR_PVDLEVEL_6) || ((LEVEL) == PWR_PVDLEVEL_7))

/* PVD mode check macro */
#define IS_PWR_PVD_MODE(MODE)                     \
  (((MODE) == PWR_PVD_MODE_NORMAL)                ||\
   ((MODE) == PWR_PVD_MODE_IT_RISING)             ||\
   ((MODE) == PWR_PVD_MODE_IT_FALLING)            ||\
   ((MODE) == PWR_PVD_MODE_IT_RISING_FALLING)     ||\
   ((MODE) == PWR_PVD_MODE_EVENT_RISING)          ||\
   ((MODE) == PWR_PVD_MODE_EVENT_FALLING)         ||\
   ((MODE) == PWR_PVD_MODE_EVENT_RISING_FALLING))

/* SLEEP mode entry check macro */
#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPENTRY_WFI) || ((ENTRY) == PWR_SLEEPENTRY_WFE))

/* STOP mode entry check macro */
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPENTRY_WFI) || ((ENTRY) == PWR_STOPENTRY_WFE))

#if defined (PWR_SECCFGR_WUP1SEC)
/* PWR items check macro */
#define IS_PWR_ITEMS_ATTRIBUTES(ITEM) ((((ITEM) & (~PWR_ALL)) == 0U) && ((ITEM) != 0U))
#endif /* PWR_SECCFGR_WUP1SEC */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/* PWR attribute check macro (Secure) */
#define IS_PWR_ATTRIBUTES(ATTRIBUTES) \
  ((((~(((ATTRIBUTES) & 0xF0U) >> 4U)) &((ATTRIBUTES) & 0x0FU)) == 0U) && (((ATTRIBUTES) & 0xFFFFFFCCU) == 0U))
#elif defined(PWR_PRIVCFGR_NSPRIV)
/* PWR attribute check macro (NSecure) */
#define IS_PWR_ATTRIBUTES(ATTRIBUTES) (((ATTRIBUTES) == PWR_NSEC_NPRIV) || ((ATTRIBUTES) == PWR_NSEC_PRIV))
#else
/* PWR attribute check macro (NSecure) */
#define IS_PWR_ATTRIBUTES(ATTRIBUTES) (((ATTRIBUTES) == PWR_NPRIV) || ((ATTRIBUTES) == PWR_PRIV))
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/* Include PWR HAL Extended module */
#include "stm32h5xx_hal_pwr_ex.h"

/* Exported functions --------------------------------------------------------*/

/** @addtogroup PWR_Exported_Functions
  * @{
  */

/** @addtogroup PWR_Exported_Functions_Group1
  * @{
  */
/* Initialization and de-initialization functions *****************************/
void HAL_PWR_DeInit(void);
void HAL_PWR_EnableBkUpAccess(void);
void HAL_PWR_DisableBkUpAccess(void);
/**
  * @}
  */

/** @addtogroup PWR_Exported_Functions_Group2
  * @{
  */
/* Programmable voltage detector functions ************************************/
HAL_StatusTypeDef HAL_PWR_ConfigPVD(const PWR_PVDTypeDef *sConfigPVD);
void              HAL_PWR_EnablePVD(void);
void              HAL_PWR_DisablePVD(void);

/* Wake up pins configuration functions ***************************************/
void HAL_PWR_EnableWakeUpPin(uint32_t WakeUpPinPolarity);
void HAL_PWR_DisableWakeUpPin(uint32_t WakeUpPinx);

/* Low power modes configuration functions ************************************/
void HAL_PWR_EnterSLEEPMode(uint32_t Regulator, uint8_t SLEEPEntry);
void HAL_PWR_EnterSTOPMode(uint32_t Regulator, uint8_t STOPEntry);
void HAL_PWR_EnterSTANDBYMode(void);

/* Sleep on exit and sev on pending configuration functions *******************/
void HAL_PWR_EnableSleepOnExit(void);
void HAL_PWR_DisableSleepOnExit(void);
void HAL_PWR_EnableSEVOnPend(void);
void HAL_PWR_DisableSEVOnPend(void);

/* Interrupt handler functions ************************************************/
void HAL_PWR_PVD_IRQHandler(void);
void HAL_PWR_PVDCallback(void);
/**
  * @}
  */

/** @addtogroup PWR_Exported_Functions_Group3
  * @{
  */
/* Privileges and security configuration functions ****************************/
void              HAL_PWR_ConfigAttributes(uint32_t Item, uint32_t Attributes);
HAL_StatusTypeDef HAL_PWR_GetConfigAttributes(uint32_t Item, uint32_t *pAttributes);
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

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STM32H5xx_HAL_PWR_H */
