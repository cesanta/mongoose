/**
  ******************************************************************************
  * @file    stm32h5xx_hal_pwr_ex.c
  * @author  MCD Application Team
  * @brief   Extended PWR HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Power Controller extension peripheral :
  *           + Power Supply Control Functions
  *           + Voltage Monitoring Functions
  *           + Wakeup Pins configuration Functions
  *           + Memories Retention Functions
  *           + IO and JTAG Retention Functions
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

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup PWREx PWREx
  * @brief    PWR Extended HAL module driver
  * @{
  */

#if defined (HAL_PWR_MODULE_ENABLED)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup PWR_Extended_Private_Defines PWR Extended Private Defines
  * @{
  */
/* PORTI pins mask */
#define PWR_PORTI_AVAILABLE_PINS  (0xFFU)
/*!< Time out value of flags setting */
#define PWR_FLAG_SETTING_DELAY    (0x32U)

/** @defgroup PWR_PVM_Mode_Mask PWR PVM Mode Mask
  * @{
  */
#define PVM_RISING_EDGE  (0x01U)  /*!< Mask for rising edge set as PVM trigger                      */
#define PVM_FALLING_EDGE (0x02U)  /*!< Mask for falling edge set as PVM trigger                     */
#define PVM_MODE_IT      (0x04U)  /*!< Mask for interruption yielded by PVM threshold crossing      */
#define PVM_MODE_EVT     (0x08U)  /*!< Mask for event yielded by PVM threshold crossing             */
/**
  * @}
  */

/** @defgroup PWREx_WakeUp_Pins_Offsets PWREx Wake-Up Pins offsets
  * @{
  */

/* Wake-Up Pins PWR Pin Pull shift offsets */
#define PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET (2U)

/**
  * @}
  */

/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup PWREx_Exported_Functions PWR Extended Exported Functions
  * @{
  */

/** @defgroup PWREx_Exported_Functions_Group1 Power Supply Control Functions
  * @brief    Power supply control functions
  *
@verbatim
 ===============================================================================
                  ##### Power supply control functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief Configure the system Power Supply.
  * @param  SupplySource : Specifies the Power Supply source to set after a
  *                        system startup.
  *         This parameter can be one of the following values :
  *            @arg PWR_EXTERNAL_SOURCE_SUPPLY : The SMPS and the LDO are
  *                                              Bypassed. The Vcore Power
  *                                              Domains are supplied from
  *                                              external source.
  * @retval HAL status.
  */
HAL_StatusTypeDef HAL_PWREx_ConfigSupply(uint32_t SupplySource)
{
  uint32_t tickstart;

  /* Check the parameters */
  assert_param(IS_PWR_SUPPLY(SupplySource));

  if ((PWR->SCCR & PWR_SCCR_BYPASS) != (PWR_SCCR_BYPASS))
  {
    /* Set the power supply configuration */
    MODIFY_REG(PWR->SCCR, PWR_SUPPLY_CONFIG_MASK, SupplySource);

    /* Get tick */
    tickstart = HAL_GetTick();

    /* Wait till voltage level flag is set */
    while (__HAL_PWR_GET_FLAG(PWR_FLAG_ACTVOSRDY) == 0U)
    {
      if ((HAL_GetTick() - tickstart) > PWR_FLAG_SETTING_DELAY)
      {
        return HAL_ERROR;
      }
    }
  }

  return HAL_OK;
}

/**
  * @brief Get the power supply configuration.
  * @retval The supply configuration.
  */
uint32_t HAL_PWREx_GetSupplyConfig(void)
{
  return (PWR->SCCR & PWR_SUPPLY_CONFIG_MASK);
}

/**
  * @brief Configure the main internal regulator output voltage.
  * @param  VoltageScaling : Specifies the regulator output voltage to achieve
  *                          a tradeoff between performance and power
  *                          consumption.
  *          This parameter can be one of the following values :
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE0 : Regulator voltage output
  *                                                Scale 0 mode.
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE1 : Regulator voltage output
  *                                                range 1 mode.
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE2 : Regulator voltage output
  *                                                range 2 mode.
  *            @arg PWR_REGULATOR_VOLTAGE_SCALE3 : Regulator voltage output
  *                                                range 3 mode.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_PWREx_ControlVoltageScaling(uint32_t VoltageScaling)
{
  uint32_t tickstart = HAL_GetTick();

  /* Check the parameters */
  assert_param(IS_PWR_VOLTAGE_SCALING_RANGE(VoltageScaling));

  /* Get the voltage scaling  */
  if ((PWR->VOSSR & PWR_VOSSR_ACTVOS) == (VoltageScaling << 10U))
  {
    /* Old and new voltage scaling configuration match : nothing to do */
    return HAL_OK;
  }

  /* Set the voltage range */
  MODIFY_REG(PWR->VOSCR, PWR_VOSCR_VOS, VoltageScaling);

  /* Wait till voltage level flag is set */
  while (__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY) == 0U)
  {
    if ((HAL_GetTick() - tickstart) > PWR_FLAG_SETTING_DELAY)
    {
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

/**
  * @brief Get the main internal regulator output voltage. Reflecting the last
  *        VOS value applied to the PMU.
  * @retval The current applied VOS selection.
  */
uint32_t HAL_PWREx_GetVoltageRange(void)
{
  /* Get the active voltage scaling */
  return (PWR->VOSSR & PWR_VOSSR_ACTVOS);
}

/**
  * @brief Configure the main internal regulator output voltage in STOP mode.
  * @param  VoltageScaling : Specifies the regulator output voltage when the
  *         system enters Stop mode to achieve a tradeoff between performance
  *         and power consumption.
  *          This parameter can be one of the following values:
  *            @arg PWR_REGULATOR_SVOS_SCALE3 : Regulator voltage output range
  *                                             3 mode.
  *            @arg PWR_REGULATOR_SVOS_SCALE4 : Regulator voltage output range
  *                                             4 mode.
  *            @arg PWR_REGULATOR_SVOS_SCALE5 : Regulator voltage output range
  *                                             5 mode.
  * @note   The Stop mode voltage scaling for SVOS4 and SVOS5 sets the voltage
  *         regulator in Low-power (LP) mode to further reduce power consumption.
  *         When preselecting SVOS3, the use of the voltage regulator low-power
  *         mode (LP) can be selected by LPDS register bit.
  * @note   The selected SVOS4 and SVOS5 levels add an additional startup delay
  *         when exiting from system Stop mode.
  * @retval HAL Status.
  */
HAL_StatusTypeDef HAL_PWREx_ControlStopModeVoltageScaling(uint32_t VoltageScaling)
{
  /* Check the parameters */
  assert_param(IS_PWR_STOP_MODE_REGULATOR_VOLTAGE(VoltageScaling));

  /* Return the stop mode voltage range */
  MODIFY_REG(PWR->PMCR, PWR_PMCR_SVOS, VoltageScaling);

  return HAL_OK;
}

/**
  * @brief Get the main internal regulator output voltage in STOP mode.
  * @retval The actual applied VOS selection.
  */
uint32_t HAL_PWREx_GetStopModeVoltageRange(void)
{
  /* Return the stop voltage scaling */
  return (PWR->PMCR & PWR_PMCR_SVOS);
}
/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group2 Voltage Monitoring Functions
  * @brief    Voltage monitoring functions
  *
@verbatim
 ===============================================================================
                     ##### Voltage Monitoring Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief  Configure the event mode and the voltage threshold detected by the
  *         Analog Voltage Detector (AVD).
  * @param  sConfigAVD : Pointer to an PWREx_AVDTypeDef structure that contains
  *                      the configuration information for the AVD.
  * @note   Refer to the electrical characteristics of your device datasheet for
  *         more details about the voltage threshold corresponding to each
  *         detection level.
  * @retval None.
  */
void HAL_PWREx_ConfigAVD(const PWREx_AVDTypeDef *sConfigAVD)
{
  /* Check the parameters */
  assert_param(IS_PWR_AVD_LEVEL(sConfigAVD->AVDLevel));
  assert_param(IS_PWR_AVD_MODE(sConfigAVD->Mode));

  /* Set the ALS[10:9] bits according to AVDLevel value */
  MODIFY_REG(PWR->VMCR, PWR_VMCR_ALS, sConfigAVD->AVDLevel);

  /* Clear any previous config */
  __HAL_PWR_AVD_EXTI_DISABLE_EVENT();
  __HAL_PWR_AVD_EXTI_DISABLE_IT();
  __HAL_PWR_AVD_EXTI_DISABLE_RISING_EDGE();
  __HAL_PWR_AVD_EXTI_DISABLE_FALLING_EDGE();

  /* Configure the interrupt mode */
  if ((sConfigAVD->Mode & AVD_MODE_IT) == AVD_MODE_IT)
  {
    __HAL_PWR_AVD_EXTI_ENABLE_IT();
  }

  /* Configure the event mode */
  if ((sConfigAVD->Mode & AVD_MODE_EVT) == AVD_MODE_EVT)
  {
    __HAL_PWR_AVD_EXTI_ENABLE_EVENT();
  }

  /* Rising edge configuration */
  if ((sConfigAVD->Mode & AVD_RISING_EDGE) == AVD_RISING_EDGE)
  {
    __HAL_PWR_AVD_EXTI_ENABLE_RISING_EDGE();
  }

  /* Falling edge configuration */
  if ((sConfigAVD->Mode & AVD_FALLING_EDGE) == AVD_FALLING_EDGE)
  {
    __HAL_PWR_AVD_EXTI_ENABLE_FALLING_EDGE();
  }
}

/**
  * @brief Enable the Analog Voltage Detector (AVD).
  * @retval None.
  */
void HAL_PWREx_EnableAVD(void)
{
  /* Enable the Analog Voltage Detector */
  SET_BIT(PWR->VMCR, PWR_VMCR_AVDEN);
}

/**
  * @brief Disable the Analog Voltage Detector(AVD).
  * @retval None.
  */
void HAL_PWREx_DisableAVD(void)
{
  /* Disable the Analog Voltage Detector */
  CLEAR_BIT(PWR->VMCR, PWR_VMCR_AVDEN);
}

#if defined (PWR_USBSCR_USB33DEN)
/**
  * @brief Enable the USB voltage level detector.
  * @retval None.
  */
void HAL_PWREx_EnableUSBVoltageDetector(void)
{
  /* Enable the USB voltage detector */
  SET_BIT(PWR->USBSCR, PWR_USBSCR_USB33DEN);
}

/**
  * @brief Disable the USB voltage level detector.
  * @retval None.
  */
void HAL_PWREx_DisableUSBVoltageDetector(void)
{
  /* Disable the USB voltage detector */
  CLEAR_BIT(PWR->USBSCR, PWR_USBSCR_USB33DEN);
}

/**
  * @brief  Enable VDDUSB supply.
  * @note   Remove VDDUSB electrical and logical isolation, once VDDUSB supply
  *         is present for consumption saving.
  * @retval None.
  */
void HAL_PWREx_EnableVddUSB(void)
{
  SET_BIT(PWR->USBSCR, PWR_USBSCR_USB33SV);
}

/**
  * @brief  Disable VDDUSB supply.
  * @retval None.
  */
void HAL_PWREx_DisableVddUSB(void)
{
  CLEAR_BIT(PWR->USBSCR, PWR_USBSCR_USB33SV);
}
#endif /* PWR_USBSCR_USB33DEN */

#if defined (PWR_USBSCR_OTGHSEN)
/**
  * @brief Enable the USB OTGHS PHY.
  * @retval None.
  */
void HAL_PWREx_EnableUSBOTGHSPhy(void)
{
  /* Enable the USB OTGHS PHY */
  SET_BIT(PWR->USBSCR, PWR_USBSCR_OTGHSEN);
}

/**
  * @brief Disable the USB OTGHS PHY.
  * @retval None.
  */
void HAL_PWREx_DisableUSBOTGHSPhy(void)
{
  /* Disable the OTGHS PHY */
  CLEAR_BIT(PWR->USBSCR, PWR_USBSCR_OTGHSEN);
}
#endif /* PWR_USBSCR_OTGHSEN */

/**
  * @brief  Enable the VBAT and temperature monitoring.
  * @retval None.
  */
void HAL_PWREx_EnableMonitoring(void)
{
  SET_BIT(PWR->BDCR, PWR_BDCR_MONEN);
}

/**
  * @brief  Disable the VBAT and temperature monitoring.
  * @retval None.
  */
void HAL_PWREx_DisableMonitoring(void)
{
  CLEAR_BIT(PWR->BDCR, PWR_BDCR_MONEN);
}

#if defined (PWR_UCPDR_UCPD_STBY)
/**
  * @brief  Enable UCPD configuration memorization in Standby mode.
  * @retval None.
  */
void HAL_PWREx_EnableUCPDStandbyMode(void)
{
  SET_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STBY);
}

/**
  * @brief  Disable UCPD configuration memorization in Standby mode.
  * @note   This function must be called on exiting the Standby mode and before
  *         any UCPD configuration update.
  * @retval None.
  */
void HAL_PWREx_DisableUCPDStandbyMode(void)
{
  CLEAR_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_STBY);
}
#endif /* PWR_UCPDR_UCPD_STBY */

#if defined (PWR_UCPDR_UCPD_DBDIS)
/**
  * @brief  Enable dead battery behavior.
  * @note   After exiting reset, the USB Type-C (dead battery) behavior is
  *         enabled, which may have a pull-down effect on CC1 and CC2 pins.
  *         It is recommended to disable it in all cases, either to stop this
  *         pull-down or to handover control to the UCPD (the UCPD must be
  *         initialized before doing the disable).
  * @retval None.
  */
void HAL_PWREx_EnableUCPDDeadBattery(void)
{
  CLEAR_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS);
}

/**
  * @brief  Disable dead battery behavior.
  * @note   After exiting reset, the USB Type-C (dead battery) behavior is
  *         enabled, which may have a pull-down effect on CC1 and CC2 pins.
  *         It is recommended to disable it in all cases, either to stop this
  *         pull-down or to handover control to the UCPD (the UCPD must be
  *         initialized before doing the disable).
  * @retval None.
  */
void HAL_PWREx_DisableUCPDDeadBattery(void)
{
  SET_BIT(PWR->UCPDR, PWR_UCPDR_UCPD_DBDIS);
}
#endif /* PWR_UCPDR_UCPD_DBDIS */

/**
  * @brief  Enable the Battery charging.
  * @note   When VDD is present, charge the external battery through an internal
  *         resistor.
  * @param  ResistorValue : Specifies the charging resistor.
  *                         This parameter can be one of the following values :
  *                         @arg PWR_BATTERY_CHARGING_RESISTOR_5   : 5 KOhm resistor.
  *                         @arg PWR_BATTERY_CHARGING_RESISTOR_1_5 : 1.5 KOhm resistor.
  * @retval None.
  */
void HAL_PWREx_EnableBatteryCharging(uint32_t ResistorValue)
{
  /* Check the parameter */
  assert_param(IS_PWR_BATTERY_RESISTOR_SELECT(ResistorValue));

  /* Specify the charging resistor */
  MODIFY_REG(PWR->BDCR, PWR_BDCR_VBRS, ResistorValue);

  /* Enable the Battery charging */
  SET_BIT(PWR->BDCR, PWR_BDCR_VBE);
}

/**
  * @brief  Disable the Battery charging.
  * @retval None.
  */
void HAL_PWREx_DisableBatteryCharging(void)
{
  CLEAR_BIT(PWR->BDCR, PWR_BDCR_VBE);
}

/**
  * @brief Enable the booster to guarantee the analog switch AC performance when
  *        the VDD supply voltage is below 2V7.
  * @note   The VDD supply voltage can be monitored through the PVD and the PLS
  *         field bits.
  * @retval None.
  */
void HAL_PWREx_EnableAnalogBooster(void)
{
  /* Enable the Analog voltage */
  SET_BIT(PWR->PMCR, PWR_PMCR_AVD_READY);

  /* Enable VDDA booster */
  SET_BIT(PWR->PMCR, PWR_PMCR_BOOSTE);
}

/**
  * @brief Disable the analog booster.
  * @retval None.
  */
void HAL_PWREx_DisableAnalogBooster(void)
{
  /* Disable VDDA booster */
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_BOOSTE);

  /* Disable the Analog voltage */
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_AVD_READY);
}

/**
  * @brief  This function handles the PWR PVD/AVD interrupt request.
  * @note   This API should be called under the PVD_AVD_IRQHandler().
  * @note   The use of this API is when the PVD and AVD are activated at the same time.
  * @retval None
  */
void HAL_PWREx_PVD_AVD_IRQHandler(void)
{
  /* Check PWR PVD AVD EXTI Rising flag */
  if (__HAL_PWR_PVD_AVD_EXTI_GET_RISING_FLAG() != 0U)
  {
    /* Clear PWR PVD AVD EXTI Rising pending bit */
    WRITE_REG(EXTI->RPR1, PWR_EXTI_LINE_AVD);

    /* PWR PVD AVD Rising interrupt user callback */
    HAL_PWREx_PVD_AVD_Rising_Callback();
  }

  /* Check PWR PVD AVD EXTI Falling flag */
  if (__HAL_PWR_PVD_AVD_EXTI_GET_FALLING_FLAG() != 0U)
  {
    /* Clear PWR PVD AVD EXTI Falling pending bit */
    WRITE_REG(EXTI->FPR1, PWR_EXTI_LINE_AVD);

    /* PWR PVD AVD Falling interrupt user callback */
    HAL_PWREx_PVD_AVD_Falling_Callback();
  }
}

/**
  * @brief PWR PVD AVD Rising interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_PVD_AVD_Rising_Callback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PWR_AVDCallback can be implemented in the user file
  */
}

/**
  * @brief PWR PVD AVD Falling interrupt callback.
  * @retval None.
  */
__weak void HAL_PWREx_PVD_AVD_Falling_Callback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_PWR_AVDCallback can be implemented in the user file
  */
}
/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group3 Wakeup Pins configuration Functions
  * @brief    Wakeup Pins configuration functions
  *
@verbatim
 ===============================================================================
                     ##### Wakeup Pins configuration Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief Enable the Wake-up PINx functionality.
  * @param  sPinParams : Pointer to a PWREx_WakeupPinTypeDef structure that
  *                      contains the configuration information for the wake-up
  *                      Pin.
  * @retval None.
  */
void HAL_PWREx_EnableWakeUpPin(const PWREx_WakeupPinTypeDef *sPinParams)
{
  uint32_t pinConfig;
  uint32_t regMask;
  const uint32_t pullMask = PWR_WUCR_WUPPUPD1;

  /* Check the parameters */
  assert_param(IS_PWR_WAKEUP_PIN(sPinParams->WakeUpPin));
  assert_param(IS_PWR_WAKEUP_PIN_POLARITY(sPinParams->PinPolarity));
  assert_param(IS_PWR_WAKEUP_PIN_PULL(sPinParams->PinPull));

  pinConfig = sPinParams->WakeUpPin | \
              (sPinParams->PinPolarity << ((POSITION_VAL(sPinParams->WakeUpPin) + PWR_WUCR_WUPP1_Pos) & 0x1FU)) | \
              (sPinParams->PinPull << (((POSITION_VAL(sPinParams->WakeUpPin) * PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET) \
                                        + PWR_WUCR_WUPPUPD1_Pos) & 0x1FU));

  regMask   = sPinParams->WakeUpPin | \
              (PWR_WUCR_WUPP1 << (POSITION_VAL(sPinParams->WakeUpPin) & 0x1FU)) | \
              (pullMask << ((POSITION_VAL(sPinParams->WakeUpPin) * PWR_WAKEUP_PINS_PULL_SHIFT_OFFSET) & 0x1FU));

  /* Enable and Specify the Wake-Up pin polarity and the pull configuration
     for the event detection (rising or falling edge) */
  MODIFY_REG(PWR->WUCR, regMask, pinConfig);
}

/**
  * @brief Disable the Wake-up PINx functionality.
  * @param  WakeUpPinx : Specifies the Wake-Up pin to be disabled.
  *          This parameter can be one of the following values:
  *           @arg PWR_WAKEUP_PIN1
  *           @arg PWR_WAKEUP_PIN2
  *           @arg PWR_WAKEUP_PIN3
  *           @arg PWR_WAKEUP_PIN4
  *           @arg PWR_WAKEUP_PIN5
  *           @arg PWR_WAKEUP_PIN6
  *           @arg PWR_WAKEUP_PIN7
  *           @arg PWR_WAKEUP_PIN8
  * @note   The PWR_WAKEUP_PIN6, PWR_WAKEUP_PIN7 and PWR_WAKEUP_PIN8 are not available for
  *         STM32H503xx devices.
  * @retval None
  */
void HAL_PWREx_DisableWakeUpPin(uint32_t WakeUpPinx)
{
  /* Check the parameter */
  assert_param(IS_PWR_WAKEUP_PIN(WakeUpPinx));

  /* Disable the WakeUpPin */
  CLEAR_BIT(PWR->WUCR, (PWR_WUCR_WUPEN & WakeUpPinx));
}

/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group4 Memories Retention Functions
  * @brief    Memories retention functions
  *
@verbatim
 ===============================================================================
                     ##### Memories Retention Functions #####
 ===============================================================================
    [..]
@endverbatim
  * @{
  */

/**
  * @brief Enable the Flash Power Down in Stop mode.
  * @note   When Flash Power Down is enabled  the Flash memory enters low-power
  *         mode. This feature allows to
  *         obtain the best trade-off between low-power consumption and restart
  *         time when exiting from Stop mode.
  * @retval None.
  */
void HAL_PWREx_EnableFlashPowerDown(void)
{
  /* Enable the Flash Power Down */
  SET_BIT(PWR->PMCR, PWR_PMCR_FLPS);
}

/**
  * @brief Disable the Flash Power Down in Stop mode.
  * @note   When Flash Power Down is disabled  the Flash memory is kept on
  *         normal mode. This feature allows
  *         to obtain the best trade-off between low-power consumption and
  *         restart time when exiting from Stop mode.
  * @retval None.
  */
void HAL_PWREx_DisableFlashPowerDown(void)
{
  /* Disable the Flash Power Down */
  CLEAR_BIT(PWR->PMCR, PWR_PMCR_FLPS);
}

/**
  * @brief Enable memory block shut-off in Stop mode
  * @note   In Stop mode, the content of the memory blocks is
  *         maintained. Further power optimization can be obtained by switching
  *         off some memory blocks. This optimization implies loss of the memory
  *         content. The user can select which memory is discarded during STOP
  *         mode by means of xxSO bits.
  * @param  MemoryBlock : Specifies the memory block to shut-off during Stop mode.
  *          This parameter can be one of the following values for STM32H5F5xx/STM32H5F4xx/STM32H5E5xx/STM32H5E4xx :
  *            @arg PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    : Ethernet shut-off control in Stop mode
  *            @arg PWR_LTDC_MEMORY_BLOCK          PWR_PMCR_LTDCSO        : LTDC shut-off control in Stop mode
  *            @arg PWR_RAM5_MEMORY_BLOCK          PWR_PMCR_SRAM5SO       : RAM5 shut-off control in Stop mode
  *            @arg PWR_RAM4_MEMORY_BLOCK          PWR_PMCR_SRAM4SO       : RAM4 shut-off control in Stop mode
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   : RAM2 Low 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   : RAM2 High 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_LOW_48_MEMORY_BLOCK   PWR_PMCR_SRAM2_48LSO   : RAM2 Low 48k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_48_MEMORY_BLOCK  PWR_PMCR_SRAM2_48HSO   : RAM2 High 48k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H553xx/STM32H543xx :
  *            @arg PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    : Ethernet shut-off control in Stop mode
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   : RAM2 Low 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   : RAM2 High 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    : RAM2 48k byte shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H573xx/STM32H563xx/STM32H562xx :
  *            @arg PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    : Ethernet shut-off control in Stop mode
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_16_MEMORY_BLOCK       PWR_PMCR_SRAM2_16SO    : RAM2 16k byte shut-off control in Stop mode
  *            @arg PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    : RAM2 48k byte shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H533xx/STM32H523xx :
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   : RAM2 Low 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   : RAM2 High 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    : RAM2 48k byte shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H503xx :
  *            @arg PWR_RAM2_MEMORY_BLOCK          PWR_PMCR_SRAM2SO       : RAM2 shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  * @retval None.
  */
void HAL_PWREx_EnableMemoryShutOff(uint32_t MemoryBlock)
{
  /* Check the parameter */
  assert_param(IS_PWR_MEMORY_BLOCK(MemoryBlock));

  /* Enable memory block shut-off */
  SET_BIT(PWR->PMCR, MemoryBlock);
}

/**
  * @brief Disable memory block shut-off in Stop mode
  * @param  MemoryBlock : Specifies the memory block to keep content during
  *                       Stop mode.
  *          This parameter can be one of the following values for STM32H5F5xx/STM32H5F4xx/STM32H5E5xx/STM32H5E4xx :
  *            @arg PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    : Ethernet shut-off control in Stop mode
  *            @arg PWR_LTDC_MEMORY_BLOCK          PWR_PMCR_LTDCSO        : LTDC shut-off control in Stop mode
  *            @arg PWR_RAM5_MEMORY_BLOCK          PWR_PMCR_SRAM5SO       : RAM5 shut-off control in Stop mode
  *            @arg PWR_RAM4_MEMORY_BLOCK          PWR_PMCR_SRAM4SO       : RAM4 shut-off control in Stop mode
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   : RAM2 Low 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   : RAM2 High 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_LOW_48_MEMORY_BLOCK   PWR_PMCR_SRAM2_48LSO   : RAM2 Low 48k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_48_MEMORY_BLOCK  PWR_PMCR_SRAM2_48HSO   : RAM2 High 48k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H553xx/STM32H543xx :
  *            @arg PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    : Ethernet shut-off control in Stop mode
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   : RAM2 Low 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   : RAM2 High 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    : RAM2 48k byte shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H573xx/STM32H563xx/STM32H562xx :
  *            @arg PWR_ETHERNET_MEMORY_BLOCK      PWR_PMCR_ETHERNETSO    : Ethernet shut-off control in Stop mode
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_16_MEMORY_BLOCK       PWR_PMCR_SRAM2_16SO    : RAM2 16k byte shut-off control in Stop mode
  *            @arg PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    : RAM2 48k byte shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H533xx/STM32H523xx :
  *            @arg PWR_RAM3_MEMORY_BLOCK          PWR_PMCR_SRAM3SO       : RAM3 shut-off control in Stop mode
  *            @arg PWR_RAM2_LOW_16_MEMORY_BLOCK   PWR_PMCR_SRAM2_16LSO   : RAM2 Low 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_HIGH_16_MEMORY_BLOCK  PWR_PMCR_SRAM2_16HSO   : RAM2 High 16k byte shut-off control
  *            in Stop mode
  *            @arg PWR_RAM2_48_MEMORY_BLOCK       PWR_PMCR_SRAM2_48SO    : RAM2 48k byte shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  *          This parameter can be one of the following values for STM32H503xx :
  *            @arg PWR_RAM2_MEMORY_BLOCK          PWR_PMCR_SRAM2SO       : RAM2 shut-off control in Stop mode
  *            @arg PWR_RAM1_MEMORY_BLOCK          PWR_PMCR_SRAM1SO       : RAM1 shut-off control in Stop mode
  * @retval None.
  */
void HAL_PWREx_DisableMemoryShutOff(uint32_t MemoryBlock)
{
  /* Check the parameter */
  assert_param(IS_PWR_MEMORY_BLOCK(MemoryBlock));

  /* Disable memory block shut-off */
  CLEAR_BIT(PWR->PMCR, MemoryBlock);
}

/**
  * @brief  Enable the Backup RAM retention in Standby and VBAT modes.
  * @note   If BREN is reset, the backup RAM can still be used in Run, Sleep and
  *         Stop modes. However, its content is lost in Standby, Shutdown and
  *         VBAT modes. This bit can be writte
  * @retval None.
  */
HAL_StatusTypeDef HAL_PWREx_EnableBkupRAMRetention(void)
{
  SET_BIT(PWR->BDCR, PWR_BDCR_BREN);

  return HAL_OK;
}

/**
  * @brief  Disable the Backup RAM retention in Standby and VBAT modes.
  * @note   If BREN is reset, the backup RAM can still be used in Run, Sleep and
  *         Stop modes. However, its content is lost in Standby, Shutdown and
  *         VBAT modes. This bit can be write
  * @retval None.
  */
void HAL_PWREx_DisableBkupRAMRetention(void)
{
  CLEAR_BIT(PWR->BDCR, PWR_BDCR_BREN);
}
/**
  * @}
  */

/** @defgroup PWREx_Exported_Functions_Group5 IO and JTAG Retention Functions
  * @brief    IO and JTAG Retention functions
  *
@verbatim
 ===============================================================================
                     ##### IO and JTAG Retention Functions #####
 ===============================================================================
    [..]
      In the Standby mode, the I/Os are by default in floating state. If the IORETEN bit in the
      PWR_IORETR register is set, the I/Os output state is retained. IO Retention mode is
      enabled for all IO except the IO support the standby functionality and JTAG IOs (PA13,
      PA14, PA15 and PB4). When entering into Standby mode, the state of the output is
      sampled, and pull-up or pull-down resistor are set to maintain the IO output during Standby
      mode.
      If the JTAGIORETEN bit in the PWR_IORETR register is set, the I/Os output state is
      retained. IO Retention mode is enabled for PA13, PA14, PA15 and PB4 (default JTAG pullup/
      pull-down after wakeup are not enabled).
@endverbatim
  * @{
  */

/**
  * @brief Enable GPIO state retention in Standby mode.
  * @note   When entering into standby mode, the output is sampled, and applied to the output IO during
  *         the standby power mode
  * @retval None.
  */
void HAL_PWREx_EnableStandbyIORetention(void)
{
  /* Enable GPIO state retention */
  SET_BIT(PWR->IORETR, PWR_IORETR_IORETEN);
}

/**
  * @brief Disable GPIO state retention in Standby mode.
  * @retval None.
  */
void HAL_PWREx_DisableStandbyIORetention(void)
{
  /* Disable GPIO state retention */
  CLEAR_BIT(PWR->IORETR, PWR_IORETR_IORETEN);
}

/**
  * @brief  Enable JTAG IOs state retention in Standby mode.
  * @note   when entering into standby mode, the output is sampled, and applied to the output IO during
  *         the standby power mode
  * @retval None.
  */
void HAL_PWREx_EnableStandbyJTAGIORetention(void)
{
  /* Enable JTAG IOs state retention */
  SET_BIT(PWR->IORETR, PWR_IORETR_JTAGIORETEN);
}

/**
  * @brief Disable JTAG IOs state retention in Standby mode.
  * @retval None.
  */
void HAL_PWREx_DisableStandbyJTAGIORetention(void)
{
  /* Enable JTAG IOs state retention */
  CLEAR_BIT(PWR->IORETR, PWR_IORETR_JTAGIORETEN);
}

/**
  * @}
  */
#endif /* defined (HAL_PWR_MODULE_ENABLED) */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
