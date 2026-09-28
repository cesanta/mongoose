/**
  ******************************************************************************
  * @file    stm32h5xx_hal_pwr_ex.h
  * @author  MCD Application Team
  * @brief   Header file of PWR HAL Extended module.
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
#ifndef STM32H5xx_HAL_PWR_EX_H
#define STM32H5xx_HAL_PWR_EX_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup PWREx
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @defgroup PWREx_Exported_Types PWR Extended Exported Types
  * @{
  */

/**
  * @brief  PWREx AVD configuration structure definition
  */
typedef struct
{
  uint32_t AVDLevel; /*!< AVDLevel: Specifies the AVD detection level. This
                                     parameter can be a value of @ref
                                     PWREx_AVD_detection_level
                     */

  uint32_t Mode;     /*!< Mode: Specifies the EXTI operating mode for the AVD
                                 event. This parameter can be a value of @ref
                                 PWREx_AVD_Mode.
                     */
} PWREx_AVDTypeDef;

/**
  * @brief  PWREx Wakeup pin configuration structure definition
  */
typedef struct
{
  uint32_t WakeUpPin;   /*!< WakeUpPin: Specifies the Wake-Up pin to be enabled.
                                        This parameter can be a value of @ref
                                        PWREx_WakeUp_Pins
                        */

  uint32_t PinPolarity; /*!< PinPolarity: Specifies the Wake-Up pin polarity.
                                          This parameter can be a value of @ref
                                          PWREx_PIN_Polarity
                        */

  uint32_t PinPull;     /*!< PinPull: Specifies the Wake-Up pin pull. This
                                      parameter can be a value of @ref
                                      PWREx_PIN_Pull
                        */
} PWREx_WakeupPinTypeDef;
/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup PWREx_Exported_Constants PWR Extended Exported Constants
  * @{
  */

/** @defgroup PWREx_Supply_configuration PWREx Supply configuration
  * @{
  */
#define PWR_EXTERNAL_SOURCE_SUPPLY PWR_SCCR_BYPASS  /*!< The SMPS disabled and the LDO Bypass. The Core domains
                                                     are supplied from an external source                     */

#if defined (SMPS)
#define PWR_SUPPLY_CONFIG_MASK (PWR_SCCR_SMPSEN | PWR_SCCR_LDOEN | PWR_SCCR_BYPASS)
#else
#define PWR_SUPPLY_CONFIG_MASK (PWR_SCCR_LDOEN | PWR_SCCR_BYPASS)
#endif /* defined (SMPS) */
/**
  * @}
  */

/** @defgroup PWREx_PIN_Polarity PWREx Pin Polarity configuration
  * @{
  */
#define PWR_PIN_POLARITY_HIGH (0x00000000U)
#define PWR_PIN_POLARITY_LOW  (0x00000001U)
/**
  * @}
  */

/** @defgroup PWREx_PIN_Pull PWREx Pin Pull configuration
  * @{
  */
#define PWR_PIN_NO_PULL   (0x00000000U)
#define PWR_PIN_PULL_UP   (0x00000001U)
#define PWR_PIN_PULL_DOWN (0x00000002U)
/**
  * @}
  */

/** @defgroup PWREx_AVD_detection_level PWREx AVD detection level
  * @{
  */
#define PWR_AVDLEVEL_0 (0x00000000U)  /*!< Analog voltage detector level 0 selection : 1V7     */
#define PWR_AVDLEVEL_1 PWR_VMCR_ALS_0 /*!< Analog voltage detector level 1 selection : 2V1     */
#define PWR_AVDLEVEL_2 PWR_VMCR_ALS_1 /*!< Analog voltage detector level 2 selection : 2V5     */
#define PWR_AVDLEVEL_3 PWR_VMCR_ALS   /*!< Analog voltage detector level 3 selection : 2V8     */
/**
  * @}
  */

/** @defgroup PWREx_AVD_Mode PWREx AVD Mode
  * @{
  */
#define PWR_AVD_MODE_NORMAL               (0x00000000U)/*!< Basic mode is used                                        */
#define PWR_AVD_MODE_IT_RISING            (0x00010001U)/*!< External Interrupt Mode with Rising edge trigger detection*/
#define PWR_AVD_MODE_IT_FALLING           (0x00010002U)/*!< External Interrupt Mode with
                                                            Falling edge trigger detection                            */
#define PWR_AVD_MODE_IT_RISING_FALLING    (0x00010003U)/*!< External Interrupt Mode with
                                                            Rising/Falling edge trigger detection                     */
#define PWR_AVD_MODE_EVENT_RISING         (0x00020001U)/*!< Event Mode with Rising edge trigger detection             */
#define PWR_AVD_MODE_EVENT_FALLING        (0x00020002U)/*!< Event Mode with Falling edge trigger detection            */
#define PWR_AVD_MODE_EVENT_RISING_FALLING (0x00020003U)/*!< Event Mode with Rising/Falling edge trigger detection     */
/**
  * @}
  */

/** @defgroup PWREx_Regulator_Voltage_Scale PWREx Regulator Voltage Scale
  * @{
  */
#define PWR_REGULATOR_VOLTAGE_SCALE0 PWR_VOSCR_VOS   /*!< Voltage scaling range 0 */
#define PWR_REGULATOR_VOLTAGE_SCALE1 PWR_VOSCR_VOS_1 /*!< Voltage scaling range 1 */
#define PWR_REGULATOR_VOLTAGE_SCALE2 PWR_VOSCR_VOS_0 /*!< Voltage scaling range 2 */
#define PWR_REGULATOR_VOLTAGE_SCALE3 (0U)            /*!< Voltage scaling range 3 */
/**
  * @}
  */

/** @defgroup PWREx_System_Stop_Mode_Voltage_Scale PWREx System Stop Mode Voltage Scale
  * @{
  */
#define PWR_REGULATOR_SVOS_SCALE5 (PWR_PMCR_SVOS_0)
#define PWR_REGULATOR_SVOS_SCALE4 (PWR_PMCR_SVOS_1)
#define PWR_REGULATOR_SVOS_SCALE3 (PWR_PMCR_SVOS_0 | PWR_PMCR_SVOS_1)
/**
  * @}
  */

/** @defgroup PWREx_VBAT_Battery_Charging_Selection PWR Extended Battery Charging Resistor Selection
  * @{
  */
#define PWR_BATTERY_CHARGING_RESISTOR_5   (0U)          /*!< VBAT charging through a 5 kOhms resistor   */
#define PWR_BATTERY_CHARGING_RESISTOR_1_5 PWR_BDCR_VBRS /*!< VBAT charging through a 1.5 kOhms resistor */
/**
  * @}
  */

/** @defgroup PWREx_Memory_Shut_Off Memory shut-off block selection
  * @{
  */
#define PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       /*!< RAM1 shut-off control in Stop mode                   */
#if defined (PWR_PMCR_SRAM2_16SO)
#define PWR_RAM2_16_MEMORY_BLOCK       PWR_PMCR_SRAM2_16SO    /*!< RAM2 16k byte shut-off control in Stop mode          */
#define PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    /*!< RAM2 48k byte shut-off control in Stop mode          */
#elif defined (PWR_PMCR_SRAM2_16LSO)
#define PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   /*!< RAM2 low 16k byte shut-off control in Stop mode      */
#define PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   /*!< RAM2 High 16k byte shut-off control in Stop mode     */
#if defined (PWR_PMCR_SRAM2_48LSO)
#define PWR_RAM2_LOW_48_MEMORY_BLOCK   PWR_PMCR_SRAM2_48LSO   /*!< RAM2 low 48k byte shut-off control in Stop mode      */
#define PWR_RAM2_HIGH_48_MEMORY_BLOCK  PWR_PMCR_SRAM2_48HSO   /*!< RAM2 High 48k byte shut-off control in Stop mode     */
#else
#define PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    /*!< RAM2 48k byte shut-off control in Stop mode          */
#endif /* PWR_PMCR_SRAM2_48LSO */
#else
#define PWR_RAM2_MEMORY_BLOCK          PWR_PMCR_SRAM2SO       /*!< RAM2 shut-off control in Stop mode                   */
#endif /* PWR_PMCR_SRAM2_16SO */
#if defined (PWR_PMCR_SRAM3SO)
#define PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       /*!< RAM3 shut-off control in Stop mode                   */
#endif /* PWR_PMCR_SRAM3SO */
#if defined (PWR_PMCR_SRAM4SO)
#define PWR_RAM4_MEMORY_BLOCK          PWR_PMCR_SRAM4SO       /*!< RAM4 shut-off control in Stop mode                   */
#endif /* PWR_PMCR_SRAM4SO */
#if defined (PWR_PMCR_SRAM5SO)
#define PWR_RAM5_MEMORY_BLOCK          PWR_PMCR_SRAM5SO       /*!< RAM5 shut-off control in Stop mode                   */
#endif /* PWR_PMCR_SRAM5SO */
#if defined (PWR_PMCR_ETHERNETSO)
#define PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    /*!< Ethernet shut-off control in Stop mode               */
#endif /* PWR_PMCR_ETHERNETSO */
#if defined (PWR_PMCR_LTDCSO)
#define PWR_LTDC_MEMORY_BLOCK          PWR_PMCR_LTDCSO        /*!< LTDC shut-off control in Stop mode               */
#endif /* PWR_PMCR_LTDCSO */

/**
  * @}
  */

/** @defgroup PWREx_AVD_EXTI_Line PWREx AVD EXTI Line 16
  * @{
  */
#define PWR_EXTI_LINE_AVD EXTI_IMR1_IM16 /*!< External interrupt line 16
                                              Connected to the AVD EXTI Line */
/**
  * @}
  */

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/

/** @defgroup PWREx_Exported_Macros PWR Extended Exported Macros
  * @{
  */

/**
  * @brief Enable the AVD EXTI Line 16.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_ENABLE_IT() SET_BIT(EXTI->IMR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Disable the AVD EXTI Line 16
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_DISABLE_IT() CLEAR_BIT(EXTI->IMR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Enable event on AVD EXTI Line 16.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_ENABLE_EVENT() SET_BIT(EXTI->EMR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Disable event on AVD EXTI Line 16.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_DISABLE_EVENT() CLEAR_BIT(EXTI->EMR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Enable the AVD Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_ENABLE_RISING_EDGE() SET_BIT(EXTI->RTSR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Disable the AVD Extended Interrupt Rising Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_DISABLE_RISING_EDGE() CLEAR_BIT(EXTI->RTSR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Enable the AVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_ENABLE_FALLING_EDGE() SET_BIT(EXTI->FTSR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Disable the AVD Extended Interrupt Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_DISABLE_FALLING_EDGE() CLEAR_BIT(EXTI->FTSR1, PWR_EXTI_LINE_AVD)

/**
  * @brief Enable the AVD Extended Interrupt Rising and Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_ENABLE_RISING_FALLING_EDGE()  \
  do {                                                   \
    __HAL_PWR_AVD_EXTI_ENABLE_RISING_EDGE();             \
    __HAL_PWR_AVD_EXTI_ENABLE_FALLING_EDGE();            \
  } while(0);

/**
  * @brief Disable the AVD Extended Interrupt Rising & Falling Trigger.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_DISABLE_RISING_FALLING_EDGE() \
  do {                                                   \
    __HAL_PWR_AVD_EXTI_DISABLE_RISING_EDGE();            \
    __HAL_PWR_AVD_EXTI_DISABLE_FALLING_EDGE();           \
  } while(0);

/**
  * @brief Check whether the specified AVD EXTI Rising interrupt flag is set or not.
  * @retval EXTI AVD Line Status.
  */

#define __HAL_PWR_PVD_AVD_EXTI_GET_RISING_FLAG() ((READ_BIT(EXTI->RPR1, PWR_EXTI_LINE_AVD)\
                                                   == PWR_EXTI_LINE_AVD) ? 1UL : 0UL)

/**
  * @brief Check whether the specified AVD EXTI Falling interrupt flag is set or not.
  * @retval EXTI AVD Line Status.
  */

#define __HAL_PWR_PVD_AVD_EXTI_GET_FALLING_FLAG() ((READ_BIT(EXTI->FPR1, PWR_EXTI_LINE_AVD)\
                                                    == PWR_EXTI_LINE_AVD) ? 1UL : 0UL)

/**
  * @brief  Clear the AVD EXTI flag.
  * @retval None.
  */
#define __HAL_PWR_PVD_AVD_EXTI_CLEAR_FLAG()   \
  do                                          \
  {                                           \
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_AVD); \
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_AVD); \
  } while(0)

/**
  * @brief  Generates a Software interrupt on AVD EXTI line.
  * @retval None.
  */
#define __HAL_PWR_AVD_EXTI_GENERATE_SWIT() SET_BIT(EXTI->SWIER1, PWR_EXTI_LINE_AVD)

/**
  * @brief Configure the main internal regulator output voltage.
  * @note  This macro is similar to HAL_PWREx_ControlVoltageScaling() API but
  *        doesn't check whether or not VOSREADY flag is set. User may resort
  *        to __HAL_PWR_GET_FLAG() macro to check VOSF bit state.
  * @param  __REGULATOR__ : Specifies the regulator output voltage to achieve a
  *                         tradeoff between performance and power consumption.
  *                         This parameter can be one of the following values :
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE0 : Regulator voltage output scale 0.
  *                                                                  Provides a typical output voltage at 1.2 V.
  *                                                                  Used when system clock frequency is up to 160 MHz.
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE1 : Regulator voltage output scale 1.
  *                                                                  Provides a typical output voltage at 1.1 V.
  *                                                                  Used when system clock frequency is up to 100 MHz.
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE2 : Regulator voltage output scale 2.
  *                                                                  Provides a typical output voltage at 1.0 V.
  *                                                                  Used when system clock frequency is up to 50 MHz.
  *                         @arg @ref PWR_REGULATOR_VOLTAGE_SCALE3 : Regulator voltage output scale 3.
  *                                                                  Provides a typical output voltage at 0.9 V.
  *                                                                  Used when system clock frequency is up to 24 MHz.
  * @retval None.
  */
#define __HAL_PWR_VOLTAGESCALING_CONFIG(__REGULATOR__)      \
  do                                                        \
  {                                                         \
    __IO uint32_t tmpreg;                                   \
    MODIFY_REG(PWR->VOSCR, PWR_VOSCR_VOS, (__REGULATOR__)); \
    /* Delay after an RCC peripheral clock enabling */      \
    tmpreg = READ_BIT(PWR->VOSCR, PWR_VOSCR_VOS);           \
    UNUSED(tmpreg);                                         \
  } while(0)
/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/

/** @defgroup PWREx_Private_Constants PWR Extended Private Constants
  * @{
  */

/** @defgroup PWREx_AVD_Mode_Mask PWR Extended AVD Mode Mask
  * @{
  */
#define AVD_MODE_IT             (0x00010000U)
#define AVD_MODE_EVT            (0x00020000U)
#define AVD_RISING_EDGE         (0x00000001U)
#define AVD_FALLING_EDGE        (0x00000002U)
#define AVD_RISING_FALLING_EDGE (0x00000003U)
/**
  * @}
  */

/**
  * @}
  */

/* Private macros --------------------------------------------------------*/

/** @defgroup PWREx_Private_Macros PWR Extended Private Macros
  * @{
  */
/* Check PWR regulator configuration parameter */
#define IS_PWR_SUPPLY(PWR_SOURCE) ((PWR_SOURCE) == PWR_EXTERNAL_SOURCE_SUPPLY)

/* Check wake up pin polarity parameter */
#define IS_PWR_WAKEUP_PIN_POLARITY(POLARITY) (((POLARITY) == PWR_PIN_POLARITY_HIGH) ||\
                                              ((POLARITY) == PWR_PIN_POLARITY_LOW))

/* Check wake up pin pull configuration parameter */
#define IS_PWR_WAKEUP_PIN_PULL(PULL) (((PULL) == PWR_PIN_NO_PULL) ||\
                                      ((PULL) == PWR_PIN_PULL_UP) ||\
                                      ((PULL) == PWR_PIN_PULL_DOWN))

/* Check wake up flag parameter */
#define IS_PWR_WAKEUP_FLAG(FLAG) (((FLAG) == PWR_WAKEUP_FLAG1) ||\
                                  ((FLAG) == PWR_WAKEUP_FLAG2) ||\
                                  ((FLAG) == PWR_WAKEUP_FLAG3) ||\
                                  ((FLAG) == PWR_WAKEUP_FLAG4) ||\
                                  ((FLAG) == PWR_WAKEUP_FLAG5) ||\
                                  ((FLAG) == PWR_WAKEUP_FLAG6) ||\
                                  ((FLAG) == PWR_WAKEUP_FLAG_ALL))

/* Voltage scaling range check macro */
#define IS_PWR_VOLTAGE_SCALING_RANGE(RANGE) (((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE0) ||\
                                             ((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE1) ||\
                                             ((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE2) ||\
                                             ((RANGE) == PWR_REGULATOR_VOLTAGE_SCALE3))

/* Check PWR regulator configuration in STOP mode parameter */
#define IS_PWR_STOP_MODE_REGULATOR_VOLTAGE(VOLTAGE) (((VOLTAGE) == PWR_REGULATOR_SVOS_SCALE3)  ||\
                                                     ((VOLTAGE) == PWR_REGULATOR_SVOS_SCALE4)  ||\
                                                     ((VOLTAGE) == PWR_REGULATOR_SVOS_SCALE5))

/* Battery charging resistor selection check macro */
#define IS_PWR_BATTERY_RESISTOR_SELECT(RESISTOR) (((RESISTOR) == PWR_BATTERY_CHARGING_RESISTOR_5) ||\
                                                  ((RESISTOR) == PWR_BATTERY_CHARGING_RESISTOR_1_5))

/* Check memory block parameter */
#if defined (PWR_PMCR_SRAM2_16SO)
#define IS_PWR_MEMORY_BLOCK(BLOCK) (((BLOCK) == PWR_ETHERNET_MEMORY_BLOCK) || \
                                    ((BLOCK) == PWR_RAM3_MEMORY_BLOCK)     || \
                                    ((BLOCK) == PWR_RAM2_16_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM2_48_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM1_MEMORY_BLOCK))

#elif defined (PWR_PMCR_SRAM2_48SO)
#if defined (PWR_PMCR_ETHERNETSO)
#define IS_PWR_MEMORY_BLOCK(BLOCK) (((BLOCK) == PWR_ETHERNET_MEMORY_BLOCK)      || \
                                    ((BLOCK) == PWR_RAM3_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM2_LOW_16_MEMORY_BLOCK)   || \
                                    ((BLOCK) == PWR_RAM2_HIGH_16_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM2_48_MEMORY_BLOCK)       || \
                                    ((BLOCK) == PWR_RAM1_MEMORY_BLOCK))
#else
#define IS_PWR_MEMORY_BLOCK(BLOCK) (((BLOCK) == PWR_RAM3_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM2_LOW_16_MEMORY_BLOCK)   || \
                                    ((BLOCK) == PWR_RAM2_HIGH_16_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM2_48_MEMORY_BLOCK)       || \
                                    ((BLOCK) == PWR_RAM1_MEMORY_BLOCK))
#endif /* PWR_PMCR_ETHERNETSO */
#elif defined (PWR_PMCR_SRAM2_48LSO)
#define IS_PWR_MEMORY_BLOCK(BLOCK) (((BLOCK) == PWR_ETHERNET_MEMORY_BLOCK)      || \
                                    ((BLOCK) == PWR_LTDC_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM5_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM4_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM3_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM2_LOW_16_MEMORY_BLOCK)   || \
                                    ((BLOCK) == PWR_RAM2_HIGH_16_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM2_LOW_48_MEMORY_BLOCK)   || \
                                    ((BLOCK) == PWR_RAM2_HIGH_48_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM1_MEMORY_BLOCK))
#elif defined (PWR_PMCR_SRAM2_16LSO)
#define IS_PWR_MEMORY_BLOCK(BLOCK) (((BLOCK) == PWR_RAM3_MEMORY_BLOCK)          || \
                                    ((BLOCK) == PWR_RAM2_LOW_16_MEMORY_BLOCK)   || \
                                    ((BLOCK) == PWR_RAM2_HIGH_16_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM2_48_MEMORY_BLOCK)       || \
                                    ((BLOCK) == PWR_RAM1_MEMORY_BLOCK))
#else
#define IS_PWR_MEMORY_BLOCK(BLOCK) (((BLOCK) == PWR_RAM2_MEMORY_BLOCK)  || \
                                    ((BLOCK) == PWR_RAM1_MEMORY_BLOCK))
#endif /* PWR_PMCR_SRAM2_16SO */

/* Check wake up flag parameter */
#define IS_PWR_AVD_LEVEL(LEVEL) (((LEVEL) == PWR_AVDLEVEL_0) ||\
                                 ((LEVEL) == PWR_AVDLEVEL_1) ||\
                                 ((LEVEL) == PWR_AVDLEVEL_2) ||\
                                 ((LEVEL) == PWR_AVDLEVEL_3))

/* Check AVD mode parameter */
#define IS_PWR_AVD_MODE(MODE) (((MODE) == PWR_AVD_MODE_IT_RISING)         ||\
                               ((MODE) == PWR_AVD_MODE_IT_FALLING)        ||\
                               ((MODE) == PWR_AVD_MODE_IT_RISING_FALLING) ||\
                               ((MODE) == PWR_AVD_MODE_EVENT_RISING)      ||\
                               ((MODE) == PWR_AVD_MODE_EVENT_FALLING)     ||\
                               ((MODE) == PWR_AVD_MODE_NORMAL)            ||\
                               ((MODE) == PWR_AVD_MODE_EVENT_RISING_FALLING))
/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions
  * @{
  */

/** @addtogroup PWREx_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_PWREx_ConfigSupply(uint32_t SupplySource);
uint32_t          HAL_PWREx_GetSupplyConfig(void);
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling);
uint32_t          HAL_PWREx_GetVoltageRange(void);
HAL_StatusTypeDef HAL_PWREx_ControlStopModeVoltageScaling(uint32_t VoltageScaling);
uint32_t          HAL_PWREx_GetStopModeVoltageRange(void);

/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group2
  * @{
  */
void HAL_PWREx_ConfigAVD(const PWREx_AVDTypeDef *sConfigAVD);
void HAL_PWREx_EnableAVD(void);
void HAL_PWREx_DisableAVD(void);
#if defined (PWR_USBSCR_USB33DEN)
void HAL_PWREx_EnableUSBVoltageDetector(void);
void HAL_PWREx_DisableUSBVoltageDetector(void);
void HAL_PWREx_EnableVddUSB(void);
void HAL_PWREx_DisableVddUSB(void);
void HAL_PWREx_EnableUSBOTGHSPhy(void);
void HAL_PWREx_DisableUSBOTGHSPhy(void);
#endif /* PWR_USBSCR_USB33DEN */
void HAL_PWREx_EnableMonitoring(void);
void HAL_PWREx_DisableMonitoring(void);
void HAL_PWREx_EnableUCPDStandbyMode(void);
void HAL_PWREx_DisableUCPDStandbyMode(void);
void HAL_PWREx_EnableUCPDDeadBattery(void);
void HAL_PWREx_DisableUCPDDeadBattery(void);
void HAL_PWREx_EnableBatteryCharging(uint32_t ResistorValue);
void HAL_PWREx_DisableBatteryCharging(void);
void HAL_PWREx_EnableAnalogBooster(void);
void HAL_PWREx_DisableAnalogBooster(void);
void HAL_PWREx_PVD_AVD_IRQHandler(void);
void HAL_PWREx_PVD_AVD_Rising_Callback(void);
void HAL_PWREx_PVD_AVD_Falling_Callback(void);

/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group3
  * @{
  */

void HAL_PWREx_EnableWakeUpPin(const PWREx_WakeupPinTypeDef *sPinParams);
void HAL_PWREx_DisableWakeUpPin(uint32_t WakeUpPinx);

/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group4
  * @{
  */
void HAL_PWREx_EnableFlashPowerDown(void);
void HAL_PWREx_DisableFlashPowerDown(void);
void HAL_PWREx_EnableMemoryShutOff(uint32_t MemoryBlock);
void HAL_PWREx_DisableMemoryShutOff(uint32_t MemoryBlock);
HAL_StatusTypeDef HAL_PWREx_EnableBkupRAMRetention(void);
void HAL_PWREx_DisableBkupRAMRetention(void);

/**
  * @}
  */

/** @addtogroup PWREx_Exported_Functions_Group5
  * @{
  */
void HAL_PWREx_EnableStandbyIORetention(void);
void HAL_PWREx_DisableStandbyIORetention(void);
void HAL_PWREx_EnableStandbyJTAGIORetention(void);
void HAL_PWREx_DisableStandbyJTAGIORetention(void);

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


#endif /* STM32H5xx_HAL_PWR_EX_H */
