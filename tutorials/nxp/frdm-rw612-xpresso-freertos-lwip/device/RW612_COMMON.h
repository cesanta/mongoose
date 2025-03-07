/*
** ###################################################################
**     Processors:          RW612ETA2I
**                          RW612HNA2I
**                          RW612UKA2I
**
**     Compilers:           GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          Keil ARM C/C++ Compiler
**                          MCUXpresso Compiler
**
**     Reference manual:    RW61X User manual Rev. 0.95, June 2022
**     Version:             rev. 1.0, 2021-03-16
**     Build:               b240823
**
**     Abstract:
**         CMSIS Peripheral Access Layer for RW612
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
 * @file RW612_COMMON.h
 * @version 1.0
 * @date 2021-03-16
 * @brief CMSIS Peripheral Access Layer for RW612
 *
 * CMSIS Peripheral Access Layer for RW612
 */

#if !defined(RW612_COMMON_H_)
#define RW612_COMMON_H_                          /**< Symbol preventing repeated inclusion */

/** Memory map major version (memory maps with equal major version number are
 * compatible) */
#define MCU_MEM_MAP_VERSION 0x0100U
/** Memory map minor version */
#define MCU_MEM_MAP_VERSION_MINOR 0x0000U


/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
#define NUMBER_OF_INT_VECTORS 145                /**< Number of interrupts in the Vector table */

typedef enum IRQn {
  /* Auxiliary constants */
  NotAvail_IRQn                = -128,             /**< Not available device specific interrupt */

  /* Core interrupts */
  NonMaskableInt_IRQn          = -14,              /**< Non Maskable Interrupt */
  HardFault_IRQn               = -13,              /**< Cortex-M33 SV Hard Fault Interrupt */
  MemoryManagement_IRQn        = -12,              /**< Cortex-M33 Memory Management Interrupt */
  BusFault_IRQn                = -11,              /**< Cortex-M33 Bus Fault Interrupt */
  UsageFault_IRQn              = -10,              /**< Cortex-M33 Usage Fault Interrupt */
  SecureFault_IRQn             = -9,               /**< Cortex-M33 Secure Fault Interrupt */
  SVCall_IRQn                  = -5,               /**< Cortex-M33 SV Call Interrupt */
  DebugMonitor_IRQn            = -4,               /**< Cortex-M33 Debug Monitor Interrupt */
  PendSV_IRQn                  = -2,               /**< Cortex-M33 Pend SV Interrupt */
  SysTick_IRQn                 = -1,               /**< Cortex-M33 System Tick Interrupt */

  /* Device specific interrupts */
  WDT0_IRQn                    = 0,                /**< Windowed watchdog timer 0 (WDT0)(Cortex-M33 watchdog) */
  DMA0_IRQn                    = 1,                /**< Direct memory access (DMA) controller 0 (secure or Cortex-M33 DMA) */
  GPIO_INTA_IRQn               = 2,                /**< GPIO interrupt A */
  GPIO_INTB_IRQn               = 3,                /**< GPIO interrupt B */
  PIN_INT0_IRQn                = 4,                /**< Pin interrupt 0 or pattern match engine slice 0 */
  PIN_INT1_IRQn                = 5,                /**< Pin interrupt 1 or pattern match engine slice 1 */
  PIN_INT2_IRQn                = 6,                /**< Pin interrupt 2 or pattern match engine slice 2 */
  PIN_INT3_IRQn                = 7,                /**< Pin interrupt 3 or pattern match engine slice 3 */
  UTICK_IRQn                   = 8,                /**< Micro-tick Timer (UTICK) */
  MRT_IRQn                     = 9,                /**< Multi-Rate Timer (MRT). Global MRT interrupts */
  CTIMER0_IRQn                 = 10,               /**< Standard counter/timer CTIMER0 */
  CTIMER1_IRQn                 = 11,               /**< Standard counter/timer CTIMER1 */
  SCT0_IRQn                    = 12,               /**< SCTimer/PWM */
  CTIMER3_IRQn                 = 13,               /**< Standard counter/timer CTIMER3 */
  FLEXCOMM0_IRQn               = 14,               /**< Flexcomm Interface 0 (USART, SPI, I2C, I2S) */
  FLEXCOMM1_IRQn               = 15,               /**< Flexcomm Interface 1 (USART, SPI, I2C, I2S) */
  FLEXCOMM2_IRQn               = 16,               /**< Flexcomm Interface 2 (USART, SPI, I2C, I2S) */
  FLEXCOMM3_IRQn               = 17,               /**< Flexcomm Interface 3 (USART, SPI, I2C, I2S) */
  Reserved34_IRQn              = 18,               /**< Reserved interrupt */
  Reserved35_IRQn              = 19,               /**< Reserved interrupt */
  FLEXCOMM14_IRQn              = 20,               /**< Flexcomm Interface 14 (USART, SPI, I2C, I2S) */
  Reserved37_IRQn              = 21,               /**< Reserved interrupt */
  Reserved38_IRQn              = 22,               /**< Reserved interrupt */
  GFMRT_IRQn                   = 23,               /**< Free Multi-rate timer (GFMRT). Global MRT interrupts */
  Reserved40_IRQn              = 24,               /**< Reserved interrupt */
  DMIC_IRQn                    = 25,               /**< Digital microphone (DMIC) and DMIC subsystem */
  WKDEEPSLEEP_IRQn             = 26,               /**< Wake-up from deep sleep */
  HYPERVISOR_IRQn              = 27,               /**< Hypervisor service software interrupt */
  SECUREVIOLATION_IRQn         = 28,               /**< Secure violation */
  HWVAD_IRQn                   = 29,               /**< Hardware Voice Activity Detector */
  Reserved46_IRQn              = 30,               /**< Reserved interrupt */
  Reserved47_IRQn              = 31,               /**< Reserved interrupt */
  RTC_IRQn                     = 32,               /**< RTC alarm and wake-up */
  Reserved49_IRQn              = 33,               /**< Reserved interrupt */
  Reserved50_IRQn              = 34,               /**< Reserved interrupt */
  PIN_INT4_IRQn                = 35,               /**< Pin interrupt 4 or pattern match engine slice 4 */
  PIN_INT5_IRQn                = 36,               /**< Pin interrupt 5 or pattern match engine slice 5 */
  PIN_INT6_IRQn                = 37,               /**< Pin interrupt 6 or pattern match engine slice 6 */
  PIN_INT7_IRQn                = 38,               /**< Pin interrupt 7 or pattern match engine slice 7 */
  CTIMER2_IRQn                 = 39,               /**< Standard counter/timer CTIMER2 */
  CTIMER4_IRQn                 = 40,               /**< Standard counter/timer CTIMER4 */
  OS_EVENT_TIMER_IRQn          = 41,               /**< OS event timer 0 */
  FLEXSPI_IRQn                 = 42,               /**< FLEXSPI interface */
  Reserved59_IRQn              = 43,               /**< Reserved interrupt */
  Reserved60_IRQn              = 44,               /**< Reserved interrupt */
  Reserved61_IRQn              = 45,               /**< Reserved interrupt */
  SDIO_IRQn                    = 46,               /**< The secure digital interface */
  SGPIO_INTA_IRQn              = 47,               /**< Secure GPIO interrupt A */
  SGPIO_INTB_IRQn              = 48,               /**< Secure GPIO interrupt B */
  Reserved65_IRQn              = 49,               /**< Reserved interrupt */
  USB_IRQn                     = 50,               /**< High-speed USB device/host */
  Reserved67_IRQn              = 51,               /**< Reserved interrupt */
  Reserved68_IRQn              = 52,               /**< Reserved interrupt */
  Reserved69_IRQn              = 53,               /**< Reserved interrupt */
  DMA1_IRQn                    = 54,               /**< DMA controller 1 (non-secure or HiFi 4 DMA) */
  PUF_IRQn                     = 55,               /**< Physical Unclonable Function */
  POWERQUAD_IRQn               = 56,               /**< PowerQuad math coprocessor */
  Reserved73_IRQn              = 57,               /**< Reserved interrupt */
  Reserved74_IRQn              = 58,               /**< Reserved interrupt */
  Reserved75_IRQn              = 59,               /**< Reserved interrupt */
  Reserved76_IRQn              = 60,               /**< Reserved interrupt */
  LCD_IRQn                     = 61,               /**< LCDIC */
  CAPTIMER_IRQn                = 62,               /**< Capture timer */
  Reserved79_IRQn              = 63,               /**< Reserved interrupt */
  W2MWKUP_DONE0_IRQn           = 64,               /**< Wi-Fi to MCU, wakeup done 0 */
  W2MWKUP_DONE1_IRQn           = 65,               /**< Wi-Fi to MCU, wakeup done 1 */
  W2MWKUP_DONE2_IRQn           = 66,               /**< Wi-Fi to MCU, wakeup done 2 */
  W2MWKUP_DONE3_IRQn           = 67,               /**< Wi-Fi to MCU, wakeup done 3 */
  W2MWKUP_DONE4_IRQn           = 68,               /**< Wi-Fi to MCU, wakeup done 4 */
  W2MWKUP_DONE5_IRQn           = 69,               /**< Wi-Fi to MCU, wakeup done 5 */
  W2MWKUP_DONE6_IRQn           = 70,               /**< Wi-Fi to MCU, wakeup done 6 */
  W2MWKUP_DONE7_IRQn           = 71,               /**< Wi-Fi to MCU, wakeup done 7 */
  W2MWKUP0_IRQn                = 72,               /**< Wi-Fi to MCU, wakeup signal 0 */
  W2MWKUP1_IRQn                = 73,               /**< Wi-Fi to MCU, wakueup signal 1 */
  WL_MCI_INT0_IRQn             = 74,               /**< Wi-Fi to MCU interrupt 0 */
  WL_MCI_INT1_IRQn             = 75,               /**< Reserved for Wi-Fi to MCU */
  WL_MCI_INT2_IRQn             = 76,               /**< Reserved for Wi-Fi to MCU */
  WL_MCI_INT3_IRQn             = 77,               /**< Reserved for Wi-Fi to MCU */
  WL_MCI_INT4_IRQn             = 78,               /**< Reserved for Wi-Fi to MCU */
  WL_MCI_INT5_IRQn             = 79,               /**< Reserved for Wi-Fi to MCU */
  WL_MCI_INT6_IRQn             = 80,               /**< Reserved for Wi-Fi to MCU */
  WL_MCI_INT7_IRQn             = 81,               /**< Reserved for Wi-Fi to MCU */
  B2MWKUP_DONE0_IRQn           = 82,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 0 */
  B2MWKUP_DONE1_IRQn           = 83,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 1 */
  B2MWKUP_DONE2_IRQn           = 84,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 2 */
  B2MWKUP_DONE3_IRQn           = 85,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 3 */
  B2MWKUP_DONE4_IRQn           = 86,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 4 */
  B2MWKUP_DONE5_IRQn           = 87,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 5 */
  B2MWKUP_DONE6_IRQn           = 88,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 6 */
  B2MWKUP_DONE7_IRQn           = 89,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup done 7 */
  B2MWKUP0_IRQn                = 90,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup signal 0 */
  B2MWKUP1_IRQn                = 91,               /**< Bluetooth LE/802.15.4 radio to MCU, wakeup signal 1 */
  BLE_MCI_INT0_IRQn            = 92,               /**< Bluetooth LE/802.15.4 radio to MCU interrupt 0 */
  BLE_MCI_INT1_IRQn            = 93,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  BLE_MCI_INT2_IRQn            = 94,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  BLE_MCI_INT3_IRQn            = 95,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  BLE_MCI_INT4_IRQn            = 96,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  BLE_MCI_INT5_IRQn            = 97,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  BLE_MCI_INT6_IRQn            = 98,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  BLE_MCI_INT7_IRQn            = 99,               /**< Reserved for Bluetooth LE/802.15.4 radio to MCU */
  PIN0_INT_IRQn                = 100,              /**< From AON GPIO */
  PIN1_INT_IRQn                = 101,              /**< From AON GPIO */
  ELS_IRQn                     = 102,              /**< EdgeLock subsystem (ELS) */
  ELS_GDET_IRQn                = 103,              /**< ELS IRQ line for GDET error */
  ELS_GDET_UM_IRQn             = 104,              /**< ELS un-gated latched error */
  PKC_INT_IRQn                 = 105,              /**< Public key crypto-processor (PKC) interrupt */
  PKC_ERR_IRQn                 = 106,              /**< PKC error */
  CDOG_INT_IRQn                = 107,              /**< Code watch dog timmer interrupt */
  GAU_DAC_IRQn                 = 108,              /**< General analog unit (GAU) digital to analog converter (DAC) */
  GAU_ACOMP_WKUP_IRQn          = 109,              /**< GAU analog comparator (ACOMP) wake-up */
  GAU_ACOMP_IRQn               = 110,              /**< GAU analog comparator */
  GAU_ADC1_IRQn                = 111,              /**< GAU analog to digital converter 1 (ADC1) */
  GAU_ADC0_IRQn                = 112,              /**< GAU analog to digital converter 0 (ADC0) */
  USIM_IRQn                    = 113,              /**< Universal subscriber identity module (USIM) interface */
  OTP_IRQn                     = 114,              /**< One time programmable (OTP) memory interrupt */
  ENET_IRQn                    = 115,              /**< Etheret interrupt */
  ENET_TIMER_IRQn              = 116,              /**< Ethernet timer interrupt */
  PMIP_IRQn                    = 117,              /**< Power management IP (PMIP) */
  PMIP_CHANGE_IRQn             = 118,              /**< PMIP change from 1 to 0 */
  ITRC_IRQn                    = 119,              /**< Intrusion and tamper response controller (ITRC) interrupt request */
  Reserved136_IRQn             = 120,              /**< Reserved interrupt */
  Reserved137_IRQn             = 121,              /**< Reserved interrupt */
  Reserved138_IRQn             = 122,              /**< Reserved interrupt */
  TRNG_IRQn                    = 123,              /**< TRNG interrupt request */
  ACC_C_INT_IRQn               = 124,              /**< AHB memory access checker - Cortex-M33 code bus */
  ACC_S_INT_IRQn               = 125,              /**< AHB memory access checker - Cortex-M33 sys bus */
  WACC_IRQn                    = 126,              /**< Wi-Fi accessed during power off */
  BACC_IRQn                    = 127,              /**< Bluetooth LE/802.15.4 radio accessed during power off */
  GDMA_IRQn                    = 128               /**< General purpose direct memory access (GDMA) interrupt */
} IRQn_Type;

/*!
 * @}
 */ /* end of group Interrupt_vector_numbers */


/* ----------------------------------------------------------------------------
   -- Cortex M33 Core Configuration
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Cortex_Core_Configuration Cortex M33 Core Configuration
 * @{
 */

#define __MPU_PRESENT                  1         /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS               3         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         0         /**< Vendor specific implementation of SysTickConfig is defined */
#define __FPU_PRESENT                  1         /**< Defines if an FPU is present or not */
#define __DSP_PRESENT                  0         /**< Defines if Armv8-M Mainline core supports DSP instructions */
#define __SAUREGION_PRESENT            1         /**< Defines if an SAU is present or not */

#include "core_cm33.h"                 /* Core Peripheral Access Layer */
#include "system_RW612.h"              /* Device specific configuration file */

/*!
 * @}
 */ /* end of group Cortex_Core_Configuration */


#define RW612_SERIES
/* CPU specific feature definitions */
#include "RW612_features.h"

/* ACOMP - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral GAU_ACOMP base address */
  #define GAU_ACOMP_BASE                           (0x50038400u)
  /** Peripheral GAU_ACOMP base address */
  #define GAU_ACOMP_BASE_NS                        (0x40038400u)
  /** Peripheral GAU_ACOMP base pointer */
  #define GAU_ACOMP                                ((ACOMP_Type *)GAU_ACOMP_BASE)
  /** Peripheral GAU_ACOMP base pointer */
  #define GAU_ACOMP_NS                             ((ACOMP_Type *)GAU_ACOMP_BASE_NS)
  /** Array initializer of ACOMP peripheral base addresses */
  #define ACOMP_BASE_ADDRS                         { GAU_ACOMP_BASE }
  /** Array initializer of ACOMP peripheral base pointers */
  #define ACOMP_BASE_PTRS                          { GAU_ACOMP }
  /** Array initializer of ACOMP peripheral base addresses */
  #define ACOMP_BASE_ADDRS_NS                      { GAU_ACOMP_BASE_NS }
  /** Array initializer of ACOMP peripheral base pointers */
  #define ACOMP_BASE_PTRS_NS                       { GAU_ACOMP_NS }
#else
  /** Peripheral GAU_ACOMP base address */
  #define GAU_ACOMP_BASE                           (0x40038400u)
  /** Peripheral GAU_ACOMP base pointer */
  #define GAU_ACOMP                                ((ACOMP_Type *)GAU_ACOMP_BASE)
  /** Array initializer of ACOMP peripheral base addresses */
  #define ACOMP_BASE_ADDRS                         { GAU_ACOMP_BASE }
  /** Array initializer of ACOMP peripheral base pointers */
  #define ACOMP_BASE_PTRS                          { GAU_ACOMP }
#endif
/** Interrupt vectors for the ACOMP peripheral type */
#define ACOMP_FUNC_IRQS                          { GAU_ACOMP_IRQn }
#define ACOMP_WAKE_IRQS                          { GAU_ACOMP_WKUP_IRQn }

/* ADC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral GAU_GPADC0 base address */
  #define GAU_GPADC0_BASE                          (0x50038000u)
  /** Peripheral GAU_GPADC0 base address */
  #define GAU_GPADC0_BASE_NS                       (0x40038000u)
  /** Peripheral GAU_GPADC0 base pointer */
  #define GAU_GPADC0                               ((ADC_Type *)GAU_GPADC0_BASE)
  /** Peripheral GAU_GPADC0 base pointer */
  #define GAU_GPADC0_NS                            ((ADC_Type *)GAU_GPADC0_BASE_NS)
  /** Peripheral GAU_GPADC1 base address */
  #define GAU_GPADC1_BASE                          (0x50038100u)
  /** Peripheral GAU_GPADC1 base address */
  #define GAU_GPADC1_BASE_NS                       (0x40038100u)
  /** Peripheral GAU_GPADC1 base pointer */
  #define GAU_GPADC1                               ((ADC_Type *)GAU_GPADC1_BASE)
  /** Peripheral GAU_GPADC1 base pointer */
  #define GAU_GPADC1_NS                            ((ADC_Type *)GAU_GPADC1_BASE_NS)
  /** Array initializer of ADC peripheral base addresses */
  #define ADC_BASE_ADDRS                           { GAU_GPADC0_BASE, GAU_GPADC1_BASE }
  /** Array initializer of ADC peripheral base pointers */
  #define ADC_BASE_PTRS                            { GAU_GPADC0, GAU_GPADC1 }
  /** Array initializer of ADC peripheral base addresses */
  #define ADC_BASE_ADDRS_NS                        { GAU_GPADC0_BASE_NS, GAU_GPADC1_BASE_NS }
  /** Array initializer of ADC peripheral base pointers */
  #define ADC_BASE_PTRS_NS                         { GAU_GPADC0_NS, GAU_GPADC1_NS }
#else
  /** Peripheral GAU_GPADC0 base address */
  #define GAU_GPADC0_BASE                          (0x40038000u)
  /** Peripheral GAU_GPADC0 base pointer */
  #define GAU_GPADC0                               ((ADC_Type *)GAU_GPADC0_BASE)
  /** Peripheral GAU_GPADC1 base address */
  #define GAU_GPADC1_BASE                          (0x40038100u)
  /** Peripheral GAU_GPADC1 base pointer */
  #define GAU_GPADC1                               ((ADC_Type *)GAU_GPADC1_BASE)
  /** Array initializer of ADC peripheral base addresses */
  #define ADC_BASE_ADDRS                           { GAU_GPADC0_BASE, GAU_GPADC1_BASE }
  /** Array initializer of ADC peripheral base pointers */
  #define ADC_BASE_PTRS                            { GAU_GPADC0, GAU_GPADC1 }
#endif
/** Interrupt vectors for the ADC peripheral type */
#define ADC_IRQS                                 { GAU_ADC0_IRQn, GAU_ADC1_IRQn }

/* AHB_SECURE_CTRL - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral AHB_SECURE_CTRL base address */
  #define AHB_SECURE_CTRL_BASE                     (0x50148000u)
  /** Peripheral AHB_SECURE_CTRL base address */
  #define AHB_SECURE_CTRL_BASE_NS                  (0x40148000u)
  /** Peripheral AHB_SECURE_CTRL base pointer */
  #define AHB_SECURE_CTRL                          ((AHB_SECURE_CTRL_Type *)AHB_SECURE_CTRL_BASE)
  /** Peripheral AHB_SECURE_CTRL base pointer */
  #define AHB_SECURE_CTRL_NS                       ((AHB_SECURE_CTRL_Type *)AHB_SECURE_CTRL_BASE_NS)
  /** Array initializer of AHB_SECURE_CTRL peripheral base addresses */
  #define AHB_SECURE_CTRL_BASE_ADDRS               { AHB_SECURE_CTRL_BASE }
  /** Array initializer of AHB_SECURE_CTRL peripheral base pointers */
  #define AHB_SECURE_CTRL_BASE_PTRS                { AHB_SECURE_CTRL }
  /** Array initializer of AHB_SECURE_CTRL peripheral base addresses */
  #define AHB_SECURE_CTRL_BASE_ADDRS_NS            { AHB_SECURE_CTRL_BASE_NS }
  /** Array initializer of AHB_SECURE_CTRL peripheral base pointers */
  #define AHB_SECURE_CTRL_BASE_PTRS_NS             { AHB_SECURE_CTRL_NS }
#else
  /** Peripheral AHB_SECURE_CTRL base address */
  #define AHB_SECURE_CTRL_BASE                     (0x40148000u)
  /** Peripheral AHB_SECURE_CTRL base pointer */
  #define AHB_SECURE_CTRL                          ((AHB_SECURE_CTRL_Type *)AHB_SECURE_CTRL_BASE)
  /** Array initializer of AHB_SECURE_CTRL peripheral base addresses */
  #define AHB_SECURE_CTRL_BASE_ADDRS               { AHB_SECURE_CTRL_BASE }
  /** Array initializer of AHB_SECURE_CTRL peripheral base pointers */
  #define AHB_SECURE_CTRL_BASE_PTRS                { AHB_SECURE_CTRL }
#endif

/* AON_SOC_CIU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral AON_SOC_CIU base address */
  #define AON_SOC_CIU_BASE                         (0x55000800u)
  /** Peripheral AON_SOC_CIU base address */
  #define AON_SOC_CIU_BASE_NS                      (0x45000800u)
  /** Peripheral AON_SOC_CIU base pointer */
  #define AON_SOC_CIU                              ((AON_SOC_CIU_Type *)AON_SOC_CIU_BASE)
  /** Peripheral AON_SOC_CIU base pointer */
  #define AON_SOC_CIU_NS                           ((AON_SOC_CIU_Type *)AON_SOC_CIU_BASE_NS)
  /** Array initializer of AON_SOC_CIU peripheral base addresses */
  #define AON_SOC_CIU_BASE_ADDRS                   { AON_SOC_CIU_BASE }
  /** Array initializer of AON_SOC_CIU peripheral base pointers */
  #define AON_SOC_CIU_BASE_PTRS                    { AON_SOC_CIU }
  /** Array initializer of AON_SOC_CIU peripheral base addresses */
  #define AON_SOC_CIU_BASE_ADDRS_NS                { AON_SOC_CIU_BASE_NS }
  /** Array initializer of AON_SOC_CIU peripheral base pointers */
  #define AON_SOC_CIU_BASE_PTRS_NS                 { AON_SOC_CIU_NS }
#else
  /** Peripheral AON_SOC_CIU base address */
  #define AON_SOC_CIU_BASE                         (0x45000800u)
  /** Peripheral AON_SOC_CIU base pointer */
  #define AON_SOC_CIU                              ((AON_SOC_CIU_Type *)AON_SOC_CIU_BASE)
  /** Array initializer of AON_SOC_CIU peripheral base addresses */
  #define AON_SOC_CIU_BASE_ADDRS                   { AON_SOC_CIU_BASE }
  /** Array initializer of AON_SOC_CIU peripheral base pointers */
  #define AON_SOC_CIU_BASE_PTRS                    { AON_SOC_CIU }
#endif

/* APU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral APU0 base address */
  #define APU0_BASE                                (0x51258400u)
  /** Peripheral APU0 base address */
  #define APU0_BASE_NS                             (0x41258400u)
  /** Peripheral APU0 base pointer */
  #define APU0                                     ((APU_Type *)APU0_BASE)
  /** Peripheral APU0 base pointer */
  #define APU0_NS                                  ((APU_Type *)APU0_BASE_NS)
  /** Peripheral APU1 base address */
  #define APU1_BASE                                (0x54258400u)
  /** Peripheral APU1 base address */
  #define APU1_BASE_NS                             (0x44258400u)
  /** Peripheral APU1 base pointer */
  #define APU1                                     ((APU_Type *)APU1_BASE)
  /** Peripheral APU1 base pointer */
  #define APU1_NS                                  ((APU_Type *)APU1_BASE_NS)
  /** Array initializer of APU peripheral base addresses */
  #define APU_BASE_ADDRS                           { APU0_BASE, APU1_BASE }
  /** Array initializer of APU peripheral base pointers */
  #define APU_BASE_PTRS                            { APU0, APU1 }
  /** Array initializer of APU peripheral base addresses */
  #define APU_BASE_ADDRS_NS                        { APU0_BASE_NS, APU1_BASE_NS }
  /** Array initializer of APU peripheral base pointers */
  #define APU_BASE_PTRS_NS                         { APU0_NS, APU1_NS }
#else
  /** Peripheral APU0 base address */
  #define APU0_BASE                                (0x41258400u)
  /** Peripheral APU0 base pointer */
  #define APU0                                     ((APU_Type *)APU0_BASE)
  /** Peripheral APU1 base address */
  #define APU1_BASE                                (0x44258400u)
  /** Peripheral APU1 base pointer */
  #define APU1                                     ((APU_Type *)APU1_BASE)
  /** Array initializer of APU peripheral base addresses */
  #define APU_BASE_ADDRS                           { APU0_BASE, APU1_BASE }
  /** Array initializer of APU peripheral base pointers */
  #define APU_BASE_PTRS                            { APU0, APU1 }
#endif

/* BG - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral GAU_BG base address */
  #define GAU_BG_BASE                              (0x50038700u)
  /** Peripheral GAU_BG base address */
  #define GAU_BG_BASE_NS                           (0x40038700u)
  /** Peripheral GAU_BG base pointer */
  #define GAU_BG                                   ((BG_Type *)GAU_BG_BASE)
  /** Peripheral GAU_BG base pointer */
  #define GAU_BG_NS                                ((BG_Type *)GAU_BG_BASE_NS)
  /** Array initializer of BG peripheral base addresses */
  #define BG_BASE_ADDRS                            { GAU_BG_BASE }
  /** Array initializer of BG peripheral base pointers */
  #define BG_BASE_PTRS                             { GAU_BG }
  /** Array initializer of BG peripheral base addresses */
  #define BG_BASE_ADDRS_NS                         { GAU_BG_BASE_NS }
  /** Array initializer of BG peripheral base pointers */
  #define BG_BASE_PTRS_NS                          { GAU_BG_NS }
#else
  /** Peripheral GAU_BG base address */
  #define GAU_BG_BASE                              (0x40038700u)
  /** Peripheral GAU_BG base pointer */
  #define GAU_BG                                   ((BG_Type *)GAU_BG_BASE)
  /** Array initializer of BG peripheral base addresses */
  #define BG_BASE_ADDRS                            { GAU_BG_BASE }
  /** Array initializer of BG peripheral base pointers */
  #define BG_BASE_PTRS                             { GAU_BG }
#endif

/* BLEAPU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral BLEAPU base address */
  #define BLEAPU_BASE                              (0x54258000u)
  /** Peripheral BLEAPU base address */
  #define BLEAPU_BASE_NS                           (0x44258000u)
  /** Peripheral BLEAPU base pointer */
  #define BLEAPU                                   ((BLEAPU_Type *)BLEAPU_BASE)
  /** Peripheral BLEAPU base pointer */
  #define BLEAPU_NS                                ((BLEAPU_Type *)BLEAPU_BASE_NS)
  /** Array initializer of BLEAPU peripheral base addresses */
  #define BLEAPU_BASE_ADDRS                        { BLEAPU_BASE }
  /** Array initializer of BLEAPU peripheral base pointers */
  #define BLEAPU_BASE_PTRS                         { BLEAPU }
  /** Array initializer of BLEAPU peripheral base addresses */
  #define BLEAPU_BASE_ADDRS_NS                     { BLEAPU_BASE_NS }
  /** Array initializer of BLEAPU peripheral base pointers */
  #define BLEAPU_BASE_PTRS_NS                      { BLEAPU_NS }
#else
  /** Peripheral BLEAPU base address */
  #define BLEAPU_BASE                              (0x44258000u)
  /** Peripheral BLEAPU base pointer */
  #define BLEAPU                                   ((BLEAPU_Type *)BLEAPU_BASE)
  /** Array initializer of BLEAPU peripheral base addresses */
  #define BLEAPU_BASE_ADDRS                        { BLEAPU_BASE }
  /** Array initializer of BLEAPU peripheral base pointers */
  #define BLEAPU_BASE_PTRS                         { BLEAPU }
#endif

/* BUCK11 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral BUCK11 base address */
  #define BUCK11_BASE                              (0x55002000u)
  /** Peripheral BUCK11 base address */
  #define BUCK11_BASE_NS                           (0x45002000u)
  /** Peripheral BUCK11 base pointer */
  #define BUCK11                                   ((BUCK11_Type *)BUCK11_BASE)
  /** Peripheral BUCK11 base pointer */
  #define BUCK11_NS                                ((BUCK11_Type *)BUCK11_BASE_NS)
  /** Array initializer of BUCK11 peripheral base addresses */
  #define BUCK11_BASE_ADDRS                        { BUCK11_BASE }
  /** Array initializer of BUCK11 peripheral base pointers */
  #define BUCK11_BASE_PTRS                         { BUCK11 }
  /** Array initializer of BUCK11 peripheral base addresses */
  #define BUCK11_BASE_ADDRS_NS                     { BUCK11_BASE_NS }
  /** Array initializer of BUCK11 peripheral base pointers */
  #define BUCK11_BASE_PTRS_NS                      { BUCK11_NS }
#else
  /** Peripheral BUCK11 base address */
  #define BUCK11_BASE                              (0x45002000u)
  /** Peripheral BUCK11 base pointer */
  #define BUCK11                                   ((BUCK11_Type *)BUCK11_BASE)
  /** Array initializer of BUCK11 peripheral base addresses */
  #define BUCK11_BASE_ADDRS                        { BUCK11_BASE }
  /** Array initializer of BUCK11 peripheral base pointers */
  #define BUCK11_BASE_PTRS                         { BUCK11 }
#endif

/* BUCK18 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral BUCK18 base address */
  #define BUCK18_BASE                              (0x55002000u)
  /** Peripheral BUCK18 base address */
  #define BUCK18_BASE_NS                           (0x45002000u)
  /** Peripheral BUCK18 base pointer */
  #define BUCK18                                   ((BUCK18_Type *)BUCK18_BASE)
  /** Peripheral BUCK18 base pointer */
  #define BUCK18_NS                                ((BUCK18_Type *)BUCK18_BASE_NS)
  /** Array initializer of BUCK18 peripheral base addresses */
  #define BUCK18_BASE_ADDRS                        { BUCK18_BASE }
  /** Array initializer of BUCK18 peripheral base pointers */
  #define BUCK18_BASE_PTRS                         { BUCK18 }
  /** Array initializer of BUCK18 peripheral base addresses */
  #define BUCK18_BASE_ADDRS_NS                     { BUCK18_BASE_NS }
  /** Array initializer of BUCK18 peripheral base pointers */
  #define BUCK18_BASE_PTRS_NS                      { BUCK18_NS }
#else
  /** Peripheral BUCK18 base address */
  #define BUCK18_BASE                              (0x45002000u)
  /** Peripheral BUCK18 base pointer */
  #define BUCK18                                   ((BUCK18_Type *)BUCK18_BASE)
  /** Array initializer of BUCK18 peripheral base addresses */
  #define BUCK18_BASE_ADDRS                        { BUCK18_BASE }
  /** Array initializer of BUCK18 peripheral base pointers */
  #define BUCK18_BASE_PTRS                         { BUCK18 }
#endif

/* CACHE64_CTRL - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CACHE64_CTRL0 base address */
  #define CACHE64_CTRL0_BASE                       (0x50033000u)
  /** Peripheral CACHE64_CTRL0 base address */
  #define CACHE64_CTRL0_BASE_NS                    (0x40033000u)
  /** Peripheral CACHE64_CTRL0 base pointer */
  #define CACHE64_CTRL0                            ((CACHE64_CTRL_Type *)CACHE64_CTRL0_BASE)
  /** Peripheral CACHE64_CTRL0 base pointer */
  #define CACHE64_CTRL0_NS                         ((CACHE64_CTRL_Type *)CACHE64_CTRL0_BASE_NS)
  /** Peripheral CACHE64_CTRL1 base address */
  #define CACHE64_CTRL1_BASE                       (0x50034000u)
  /** Peripheral CACHE64_CTRL1 base address */
  #define CACHE64_CTRL1_BASE_NS                    (0x40034000u)
  /** Peripheral CACHE64_CTRL1 base pointer */
  #define CACHE64_CTRL1                            ((CACHE64_CTRL_Type *)CACHE64_CTRL1_BASE)
  /** Peripheral CACHE64_CTRL1 base pointer */
  #define CACHE64_CTRL1_NS                         ((CACHE64_CTRL_Type *)CACHE64_CTRL1_BASE_NS)
  /** Array initializer of CACHE64_CTRL peripheral base addresses */
  #define CACHE64_CTRL_BASE_ADDRS                  { CACHE64_CTRL0_BASE, CACHE64_CTRL1_BASE }
  /** Array initializer of CACHE64_CTRL peripheral base pointers */
  #define CACHE64_CTRL_BASE_PTRS                   { CACHE64_CTRL0, CACHE64_CTRL1 }
  /** Array initializer of CACHE64_CTRL peripheral base addresses */
  #define CACHE64_CTRL_BASE_ADDRS_NS               { CACHE64_CTRL0_BASE_NS, CACHE64_CTRL1_BASE_NS }
  /** Array initializer of CACHE64_CTRL peripheral base pointers */
  #define CACHE64_CTRL_BASE_PTRS_NS                { CACHE64_CTRL0_NS, CACHE64_CTRL1_NS }
#else
  /** Peripheral CACHE64_CTRL0 base address */
  #define CACHE64_CTRL0_BASE                       (0x40033000u)
  /** Peripheral CACHE64_CTRL0 base pointer */
  #define CACHE64_CTRL0                            ((CACHE64_CTRL_Type *)CACHE64_CTRL0_BASE)
  /** Peripheral CACHE64_CTRL1 base address */
  #define CACHE64_CTRL1_BASE                       (0x40034000u)
  /** Peripheral CACHE64_CTRL1 base pointer */
  #define CACHE64_CTRL1                            ((CACHE64_CTRL_Type *)CACHE64_CTRL1_BASE)
  /** Array initializer of CACHE64_CTRL peripheral base addresses */
  #define CACHE64_CTRL_BASE_ADDRS                  { CACHE64_CTRL0_BASE, CACHE64_CTRL1_BASE }
  /** Array initializer of CACHE64_CTRL peripheral base pointers */
  #define CACHE64_CTRL_BASE_PTRS                   { CACHE64_CTRL0, CACHE64_CTRL1 }
#endif
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
/** CACHE64_CTRL physical memory base address */
 #define CACHE64_CTRL_PHYMEM_BASES                { 0x18000000u, 0x38000000u }
/** CACHE64_CTRL physical memory size */
 #define CACHE64_CTRL_PHYMEM_SIZES                { 0x08000000u, 0x08000000u }
/** CACHE64_CTRL physical memory base address */
 #define CACHE64_CTRL_PHYMEM_BASES_NS             { 0x08000000u, 0x28000000u }
/** CACHE64_CTRL physical memory size */
 #define CACHE64_CTRL_PHYMEM_SIZES_NS             { 0x08000000u, 0x08000000u }
#else
/** CACHE64_CTRL physical memory base address */
 #define CACHE64_CTRL_PHYMEM_BASES                { 0x08000000u, 0x28000000u }
/** CACHE64_CTRL physical memory size */
 #define CACHE64_CTRL_PHYMEM_SIZES                { 0x08000000u, 0x08000000u }
#endif
/* Backward compatibility */
#define CACHE64_CTRL_CSAR_PHYADDR_MASK (CACHE64_CTRL_CSAR_PHYADDR27_1_MASK | CACHE64_CTRL_CSAR_PHYADDR31_29_MASK)


/* CACHE64_POLSEL - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CACHE64_POLSEL0 base address */
  #define CACHE64_POLSEL0_BASE                     (0x50033000u)
  /** Peripheral CACHE64_POLSEL0 base address */
  #define CACHE64_POLSEL0_BASE_NS                  (0x40033000u)
  /** Peripheral CACHE64_POLSEL0 base pointer */
  #define CACHE64_POLSEL0                          ((CACHE64_POLSEL_Type *)CACHE64_POLSEL0_BASE)
  /** Peripheral CACHE64_POLSEL0 base pointer */
  #define CACHE64_POLSEL0_NS                       ((CACHE64_POLSEL_Type *)CACHE64_POLSEL0_BASE_NS)
  /** Peripheral CACHE64_POLSEL1 base address */
  #define CACHE64_POLSEL1_BASE                     (0x50034000u)
  /** Peripheral CACHE64_POLSEL1 base address */
  #define CACHE64_POLSEL1_BASE_NS                  (0x40034000u)
  /** Peripheral CACHE64_POLSEL1 base pointer */
  #define CACHE64_POLSEL1                          ((CACHE64_POLSEL_Type *)CACHE64_POLSEL1_BASE)
  /** Peripheral CACHE64_POLSEL1 base pointer */
  #define CACHE64_POLSEL1_NS                       ((CACHE64_POLSEL_Type *)CACHE64_POLSEL1_BASE_NS)
  /** Array initializer of CACHE64_POLSEL peripheral base addresses */
  #define CACHE64_POLSEL_BASE_ADDRS                { CACHE64_POLSEL0_BASE, CACHE64_POLSEL1_BASE }
  /** Array initializer of CACHE64_POLSEL peripheral base pointers */
  #define CACHE64_POLSEL_BASE_PTRS                 { CACHE64_POLSEL0, CACHE64_POLSEL1 }
  /** Array initializer of CACHE64_POLSEL peripheral base addresses */
  #define CACHE64_POLSEL_BASE_ADDRS_NS             { CACHE64_POLSEL0_BASE_NS, CACHE64_POLSEL1_BASE_NS }
  /** Array initializer of CACHE64_POLSEL peripheral base pointers */
  #define CACHE64_POLSEL_BASE_PTRS_NS              { CACHE64_POLSEL0_NS, CACHE64_POLSEL1_NS }
#else
  /** Peripheral CACHE64_POLSEL0 base address */
  #define CACHE64_POLSEL0_BASE                     (0x40033000u)
  /** Peripheral CACHE64_POLSEL0 base pointer */
  #define CACHE64_POLSEL0                          ((CACHE64_POLSEL_Type *)CACHE64_POLSEL0_BASE)
  /** Peripheral CACHE64_POLSEL1 base address */
  #define CACHE64_POLSEL1_BASE                     (0x40034000u)
  /** Peripheral CACHE64_POLSEL1 base pointer */
  #define CACHE64_POLSEL1                          ((CACHE64_POLSEL_Type *)CACHE64_POLSEL1_BASE)
  /** Array initializer of CACHE64_POLSEL peripheral base addresses */
  #define CACHE64_POLSEL_BASE_ADDRS                { CACHE64_POLSEL0_BASE, CACHE64_POLSEL1_BASE }
  /** Array initializer of CACHE64_POLSEL peripheral base pointers */
  #define CACHE64_POLSEL_BASE_PTRS                 { CACHE64_POLSEL0, CACHE64_POLSEL1 }
#endif

/* CAU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CAU base address */
  #define CAU_BASE                                 (0x55002000u)
  /** Peripheral CAU base address */
  #define CAU_BASE_NS                              (0x45002000u)
  /** Peripheral CAU base pointer */
  #define CAU                                      ((CAU_Type *)CAU_BASE)
  /** Peripheral CAU base pointer */
  #define CAU_NS                                   ((CAU_Type *)CAU_BASE_NS)
  /** Array initializer of CAU peripheral base addresses */
  #define CAU_BASE_ADDRS                           { CAU_BASE }
  /** Array initializer of CAU peripheral base pointers */
  #define CAU_BASE_PTRS                            { CAU }
  /** Array initializer of CAU peripheral base addresses */
  #define CAU_BASE_ADDRS_NS                        { CAU_BASE_NS }
  /** Array initializer of CAU peripheral base pointers */
  #define CAU_BASE_PTRS_NS                         { CAU_NS }
#else
  /** Peripheral CAU base address */
  #define CAU_BASE                                 (0x45002000u)
  /** Peripheral CAU base pointer */
  #define CAU                                      ((CAU_Type *)CAU_BASE)
  /** Array initializer of CAU peripheral base addresses */
  #define CAU_BASE_ADDRS                           { CAU_BASE }
  /** Array initializer of CAU peripheral base pointers */
  #define CAU_BASE_PTRS                            { CAU }
#endif

/* CDOG - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CDOG base address */
  #define CDOG_BASE                                (0x5014C000u)
  /** Peripheral CDOG base address */
  #define CDOG_BASE_NS                             (0x4014C000u)
  /** Peripheral CDOG base pointer */
  #define CDOG                                     ((CDOG_Type *)CDOG_BASE)
  /** Peripheral CDOG base pointer */
  #define CDOG_NS                                  ((CDOG_Type *)CDOG_BASE_NS)
  /** Array initializer of CDOG peripheral base addresses */
  #define CDOG_BASE_ADDRS                          { CDOG_BASE }
  /** Array initializer of CDOG peripheral base pointers */
  #define CDOG_BASE_PTRS                           { CDOG }
  /** Array initializer of CDOG peripheral base addresses */
  #define CDOG_BASE_ADDRS_NS                       { CDOG_BASE_NS }
  /** Array initializer of CDOG peripheral base pointers */
  #define CDOG_BASE_PTRS_NS                        { CDOG_NS }
#else
  /** Peripheral CDOG base address */
  #define CDOG_BASE                                (0x4014C000u)
  /** Peripheral CDOG base pointer */
  #define CDOG                                     ((CDOG_Type *)CDOG_BASE)
  /** Array initializer of CDOG peripheral base addresses */
  #define CDOG_BASE_ADDRS                          { CDOG_BASE }
  /** Array initializer of CDOG peripheral base pointers */
  #define CDOG_BASE_PTRS                           { CDOG }
#endif
/** Interrupt vectors for the CDOG peripheral type */
#define CDOG_IRQS                                { CDOG_INT_IRQn }

/* CIU1 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral WLCTRL base address */
  #define WLCTRL_BASE                              (0x51240000u)
  /** Peripheral WLCTRL base address */
  #define WLCTRL_BASE_NS                           (0x41240000u)
  /** Peripheral WLCTRL base pointer */
  #define WLCTRL                                   ((CIU1_Type *)WLCTRL_BASE)
  /** Peripheral WLCTRL base pointer */
  #define WLCTRL_NS                                ((CIU1_Type *)WLCTRL_BASE_NS)
  /** Array initializer of CIU1 peripheral base addresses */
  #define CIU1_BASE_ADDRS                          { WLCTRL_BASE }
  /** Array initializer of CIU1 peripheral base pointers */
  #define CIU1_BASE_PTRS                           { WLCTRL }
  /** Array initializer of CIU1 peripheral base addresses */
  #define CIU1_BASE_ADDRS_NS                       { WLCTRL_BASE_NS }
  /** Array initializer of CIU1 peripheral base pointers */
  #define CIU1_BASE_PTRS_NS                        { WLCTRL_NS }
#else
  /** Peripheral WLCTRL base address */
  #define WLCTRL_BASE                              (0x41240000u)
  /** Peripheral WLCTRL base pointer */
  #define WLCTRL                                   ((CIU1_Type *)WLCTRL_BASE)
  /** Array initializer of CIU1 peripheral base addresses */
  #define CIU1_BASE_ADDRS                          { WLCTRL_BASE }
  /** Array initializer of CIU1 peripheral base pointers */
  #define CIU1_BASE_PTRS                           { WLCTRL }
#endif

/* CIU2 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral BLECTRL base address */
  #define BLECTRL_BASE                             (0x54240000u)
  /** Peripheral BLECTRL base address */
  #define BLECTRL_BASE_NS                          (0x44240000u)
  /** Peripheral BLECTRL base pointer */
  #define BLECTRL                                  ((CIU2_Type *)BLECTRL_BASE)
  /** Peripheral BLECTRL base pointer */
  #define BLECTRL_NS                               ((CIU2_Type *)BLECTRL_BASE_NS)
  /** Array initializer of CIU2 peripheral base addresses */
  #define CIU2_BASE_ADDRS                          { BLECTRL_BASE }
  /** Array initializer of CIU2 peripheral base pointers */
  #define CIU2_BASE_PTRS                           { BLECTRL }
  /** Array initializer of CIU2 peripheral base addresses */
  #define CIU2_BASE_ADDRS_NS                       { BLECTRL_BASE_NS }
  /** Array initializer of CIU2 peripheral base pointers */
  #define CIU2_BASE_PTRS_NS                        { BLECTRL_NS }
#else
  /** Peripheral BLECTRL base address */
  #define BLECTRL_BASE                             (0x44240000u)
  /** Peripheral BLECTRL base pointer */
  #define BLECTRL                                  ((CIU2_Type *)BLECTRL_BASE)
  /** Array initializer of CIU2 peripheral base addresses */
  #define CIU2_BASE_ADDRS                          { BLECTRL_BASE }
  /** Array initializer of CIU2 peripheral base pointers */
  #define CIU2_BASE_PTRS                           { BLECTRL }
#endif

/* CLKCTL0 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CLKCTL0 base address */
  #define CLKCTL0_BASE                             (0x50001000u)
  /** Peripheral CLKCTL0 base address */
  #define CLKCTL0_BASE_NS                          (0x40001000u)
  /** Peripheral CLKCTL0 base pointer */
  #define CLKCTL0                                  ((CLKCTL0_Type *)CLKCTL0_BASE)
  /** Peripheral CLKCTL0 base pointer */
  #define CLKCTL0_NS                               ((CLKCTL0_Type *)CLKCTL0_BASE_NS)
  /** Array initializer of CLKCTL0 peripheral base addresses */
  #define CLKCTL0_BASE_ADDRS                       { CLKCTL0_BASE }
  /** Array initializer of CLKCTL0 peripheral base pointers */
  #define CLKCTL0_BASE_PTRS                        { CLKCTL0 }
  /** Array initializer of CLKCTL0 peripheral base addresses */
  #define CLKCTL0_BASE_ADDRS_NS                    { CLKCTL0_BASE_NS }
  /** Array initializer of CLKCTL0 peripheral base pointers */
  #define CLKCTL0_BASE_PTRS_NS                     { CLKCTL0_NS }
#else
  /** Peripheral CLKCTL0 base address */
  #define CLKCTL0_BASE                             (0x40001000u)
  /** Peripheral CLKCTL0 base pointer */
  #define CLKCTL0                                  ((CLKCTL0_Type *)CLKCTL0_BASE)
  /** Array initializer of CLKCTL0 peripheral base addresses */
  #define CLKCTL0_BASE_ADDRS                       { CLKCTL0_BASE }
  /** Array initializer of CLKCTL0 peripheral base pointers */
  #define CLKCTL0_BASE_PTRS                        { CLKCTL0 }
#endif

/* CLKCTL1 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CLKCTL1 base address */
  #define CLKCTL1_BASE                             (0x50021000u)
  /** Peripheral CLKCTL1 base address */
  #define CLKCTL1_BASE_NS                          (0x40021000u)
  /** Peripheral CLKCTL1 base pointer */
  #define CLKCTL1                                  ((CLKCTL1_Type *)CLKCTL1_BASE)
  /** Peripheral CLKCTL1 base pointer */
  #define CLKCTL1_NS                               ((CLKCTL1_Type *)CLKCTL1_BASE_NS)
  /** Array initializer of CLKCTL1 peripheral base addresses */
  #define CLKCTL1_BASE_ADDRS                       { CLKCTL1_BASE }
  /** Array initializer of CLKCTL1 peripheral base pointers */
  #define CLKCTL1_BASE_PTRS                        { CLKCTL1 }
  /** Array initializer of CLKCTL1 peripheral base addresses */
  #define CLKCTL1_BASE_ADDRS_NS                    { CLKCTL1_BASE_NS }
  /** Array initializer of CLKCTL1 peripheral base pointers */
  #define CLKCTL1_BASE_PTRS_NS                     { CLKCTL1_NS }
#else
  /** Peripheral CLKCTL1 base address */
  #define CLKCTL1_BASE                             (0x40021000u)
  /** Peripheral CLKCTL1 base pointer */
  #define CLKCTL1                                  ((CLKCTL1_Type *)CLKCTL1_BASE)
  /** Array initializer of CLKCTL1 peripheral base addresses */
  #define CLKCTL1_BASE_ADDRS                       { CLKCTL1_BASE }
  /** Array initializer of CLKCTL1 peripheral base pointers */
  #define CLKCTL1_BASE_PTRS                        { CLKCTL1 }
#endif

/* CRC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CRC base address */
  #define CRC_BASE                                 (0x50120000u)
  /** Peripheral CRC base address */
  #define CRC_BASE_NS                              (0x40120000u)
  /** Peripheral CRC base pointer */
  #define CRC                                      ((CRC_Type *)CRC_BASE)
  /** Peripheral CRC base pointer */
  #define CRC_NS                                   ((CRC_Type *)CRC_BASE_NS)
  /** Array initializer of CRC peripheral base addresses */
  #define CRC_BASE_ADDRS                           { CRC_BASE }
  /** Array initializer of CRC peripheral base pointers */
  #define CRC_BASE_PTRS                            { CRC }
  /** Array initializer of CRC peripheral base addresses */
  #define CRC_BASE_ADDRS_NS                        { CRC_BASE_NS }
  /** Array initializer of CRC peripheral base pointers */
  #define CRC_BASE_PTRS_NS                         { CRC_NS }
#else
  /** Peripheral CRC base address */
  #define CRC_BASE                                 (0x40120000u)
  /** Peripheral CRC base pointer */
  #define CRC                                      ((CRC_Type *)CRC_BASE)
  /** Array initializer of CRC peripheral base addresses */
  #define CRC_BASE_ADDRS                           { CRC_BASE }
  /** Array initializer of CRC peripheral base pointers */
  #define CRC_BASE_PTRS                            { CRC }
#endif

/* CTIMER - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral CTIMER0 base address */
  #define CTIMER0_BASE                             (0x50028000u)
  /** Peripheral CTIMER0 base address */
  #define CTIMER0_BASE_NS                          (0x40028000u)
  /** Peripheral CTIMER0 base pointer */
  #define CTIMER0                                  ((CTIMER_Type *)CTIMER0_BASE)
  /** Peripheral CTIMER0 base pointer */
  #define CTIMER0_NS                               ((CTIMER_Type *)CTIMER0_BASE_NS)
  /** Peripheral CTIMER1 base address */
  #define CTIMER1_BASE                             (0x50029000u)
  /** Peripheral CTIMER1 base address */
  #define CTIMER1_BASE_NS                          (0x40029000u)
  /** Peripheral CTIMER1 base pointer */
  #define CTIMER1                                  ((CTIMER_Type *)CTIMER1_BASE)
  /** Peripheral CTIMER1 base pointer */
  #define CTIMER1_NS                               ((CTIMER_Type *)CTIMER1_BASE_NS)
  /** Peripheral CTIMER2 base address */
  #define CTIMER2_BASE                             (0x5002A000u)
  /** Peripheral CTIMER2 base address */
  #define CTIMER2_BASE_NS                          (0x4002A000u)
  /** Peripheral CTIMER2 base pointer */
  #define CTIMER2                                  ((CTIMER_Type *)CTIMER2_BASE)
  /** Peripheral CTIMER2 base pointer */
  #define CTIMER2_NS                               ((CTIMER_Type *)CTIMER2_BASE_NS)
  /** Peripheral CTIMER3 base address */
  #define CTIMER3_BASE                             (0x5002B000u)
  /** Peripheral CTIMER3 base address */
  #define CTIMER3_BASE_NS                          (0x4002B000u)
  /** Peripheral CTIMER3 base pointer */
  #define CTIMER3                                  ((CTIMER_Type *)CTIMER3_BASE)
  /** Peripheral CTIMER3 base pointer */
  #define CTIMER3_NS                               ((CTIMER_Type *)CTIMER3_BASE_NS)
  /** Array initializer of CTIMER peripheral base addresses */
  #define CTIMER_BASE_ADDRS                        { CTIMER0_BASE, CTIMER1_BASE, CTIMER2_BASE, CTIMER3_BASE }
  /** Array initializer of CTIMER peripheral base pointers */
  #define CTIMER_BASE_PTRS                         { CTIMER0, CTIMER1, CTIMER2, CTIMER3 }
  /** Array initializer of CTIMER peripheral base addresses */
  #define CTIMER_BASE_ADDRS_NS                     { CTIMER0_BASE_NS, CTIMER1_BASE_NS, CTIMER2_BASE_NS, CTIMER3_BASE_NS }
  /** Array initializer of CTIMER peripheral base pointers */
  #define CTIMER_BASE_PTRS_NS                      { CTIMER0_NS, CTIMER1_NS, CTIMER2_NS, CTIMER3_NS }
#else
  /** Peripheral CTIMER0 base address */
  #define CTIMER0_BASE                             (0x40028000u)
  /** Peripheral CTIMER0 base pointer */
  #define CTIMER0                                  ((CTIMER_Type *)CTIMER0_BASE)
  /** Peripheral CTIMER1 base address */
  #define CTIMER1_BASE                             (0x40029000u)
  /** Peripheral CTIMER1 base pointer */
  #define CTIMER1                                  ((CTIMER_Type *)CTIMER1_BASE)
  /** Peripheral CTIMER2 base address */
  #define CTIMER2_BASE                             (0x4002A000u)
  /** Peripheral CTIMER2 base pointer */
  #define CTIMER2                                  ((CTIMER_Type *)CTIMER2_BASE)
  /** Peripheral CTIMER3 base address */
  #define CTIMER3_BASE                             (0x4002B000u)
  /** Peripheral CTIMER3 base pointer */
  #define CTIMER3                                  ((CTIMER_Type *)CTIMER3_BASE)
  /** Array initializer of CTIMER peripheral base addresses */
  #define CTIMER_BASE_ADDRS                        { CTIMER0_BASE, CTIMER1_BASE, CTIMER2_BASE, CTIMER3_BASE }
  /** Array initializer of CTIMER peripheral base pointers */
  #define CTIMER_BASE_PTRS                         { CTIMER0, CTIMER1, CTIMER2, CTIMER3 }
#endif
/** Interrupt vectors for the CTIMER peripheral type */
#define CTIMER_IRQS                              { CTIMER0_IRQn, CTIMER1_IRQn, CTIMER2_IRQn, CTIMER3_IRQn }

/* DAC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral GAU_DAC0 base address */
  #define GAU_DAC0_BASE                            (0x50038200u)
  /** Peripheral GAU_DAC0 base address */
  #define GAU_DAC0_BASE_NS                         (0x40038200u)
  /** Peripheral GAU_DAC0 base pointer */
  #define GAU_DAC0                                 ((DAC_Type *)GAU_DAC0_BASE)
  /** Peripheral GAU_DAC0 base pointer */
  #define GAU_DAC0_NS                              ((DAC_Type *)GAU_DAC0_BASE_NS)
  /** Array initializer of DAC peripheral base addresses */
  #define DAC_BASE_ADDRS                           { GAU_DAC0_BASE }
  /** Array initializer of DAC peripheral base pointers */
  #define DAC_BASE_PTRS                            { GAU_DAC0 }
  /** Array initializer of DAC peripheral base addresses */
  #define DAC_BASE_ADDRS_NS                        { GAU_DAC0_BASE_NS }
  /** Array initializer of DAC peripheral base pointers */
  #define DAC_BASE_PTRS_NS                         { GAU_DAC0_NS }
#else
  /** Peripheral GAU_DAC0 base address */
  #define GAU_DAC0_BASE                            (0x40038200u)
  /** Peripheral GAU_DAC0 base pointer */
  #define GAU_DAC0                                 ((DAC_Type *)GAU_DAC0_BASE)
  /** Array initializer of DAC peripheral base addresses */
  #define DAC_BASE_ADDRS                           { GAU_DAC0_BASE }
  /** Array initializer of DAC peripheral base pointers */
  #define DAC_BASE_PTRS                            { GAU_DAC0 }
#endif
/** Interrupt vectors for the DAC peripheral type */
#define DAC_IRQS                                 { GAU_DAC_IRQn }

/* DBGMAILBOX - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral DBGMAILBOX base address */
  #define DBGMAILBOX_BASE                          (0x5010F000u)
  /** Peripheral DBGMAILBOX base address */
  #define DBGMAILBOX_BASE_NS                       (0x4010F000u)
  /** Peripheral DBGMAILBOX base pointer */
  #define DBGMAILBOX                               ((DBGMAILBOX_Type *)DBGMAILBOX_BASE)
  /** Peripheral DBGMAILBOX base pointer */
  #define DBGMAILBOX_NS                            ((DBGMAILBOX_Type *)DBGMAILBOX_BASE_NS)
  /** Array initializer of DBGMAILBOX peripheral base addresses */
  #define DBGMAILBOX_BASE_ADDRS                    { DBGMAILBOX_BASE }
  /** Array initializer of DBGMAILBOX peripheral base pointers */
  #define DBGMAILBOX_BASE_PTRS                     { DBGMAILBOX }
  /** Array initializer of DBGMAILBOX peripheral base addresses */
  #define DBGMAILBOX_BASE_ADDRS_NS                 { DBGMAILBOX_BASE_NS }
  /** Array initializer of DBGMAILBOX peripheral base pointers */
  #define DBGMAILBOX_BASE_PTRS_NS                  { DBGMAILBOX_NS }
#else
  /** Peripheral DBGMAILBOX base address */
  #define DBGMAILBOX_BASE                          (0x4010F000u)
  /** Peripheral DBGMAILBOX base pointer */
  #define DBGMAILBOX                               ((DBGMAILBOX_Type *)DBGMAILBOX_BASE)
  /** Array initializer of DBGMAILBOX peripheral base addresses */
  #define DBGMAILBOX_BASE_ADDRS                    { DBGMAILBOX_BASE }
  /** Array initializer of DBGMAILBOX peripheral base pointers */
  #define DBGMAILBOX_BASE_PTRS                     { DBGMAILBOX }
#endif

/* DMA - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral DMA0 base address */
  #define DMA0_BASE                                (0x50104000u)
  /** Peripheral DMA0 base address */
  #define DMA0_BASE_NS                             (0x40104000u)
  /** Peripheral DMA0 base pointer */
  #define DMA0                                     ((DMA_Type *)DMA0_BASE)
  /** Peripheral DMA0 base pointer */
  #define DMA0_NS                                  ((DMA_Type *)DMA0_BASE_NS)
  /** Peripheral DMA1 base address */
  #define DMA1_BASE                                (0x50105000u)
  /** Peripheral DMA1 base address */
  #define DMA1_BASE_NS                             (0x40105000u)
  /** Peripheral DMA1 base pointer */
  #define DMA1                                     ((DMA_Type *)DMA1_BASE)
  /** Peripheral DMA1 base pointer */
  #define DMA1_NS                                  ((DMA_Type *)DMA1_BASE_NS)
  /** Array initializer of DMA peripheral base addresses */
  #define DMA_BASE_ADDRS                           { DMA0_BASE, DMA1_BASE }
  /** Array initializer of DMA peripheral base pointers */
  #define DMA_BASE_PTRS                            { DMA0, DMA1 }
  /** Array initializer of DMA peripheral base addresses */
  #define DMA_BASE_ADDRS_NS                        { DMA0_BASE_NS, DMA1_BASE_NS }
  /** Array initializer of DMA peripheral base pointers */
  #define DMA_BASE_PTRS_NS                         { DMA0_NS, DMA1_NS }
#else
  /** Peripheral DMA0 base address */
  #define DMA0_BASE                                (0x40104000u)
  /** Peripheral DMA0 base pointer */
  #define DMA0                                     ((DMA_Type *)DMA0_BASE)
  /** Peripheral DMA1 base address */
  #define DMA1_BASE                                (0x40105000u)
  /** Peripheral DMA1 base pointer */
  #define DMA1                                     ((DMA_Type *)DMA1_BASE)
  /** Array initializer of DMA peripheral base addresses */
  #define DMA_BASE_ADDRS                           { DMA0_BASE, DMA1_BASE }
  /** Array initializer of DMA peripheral base pointers */
  #define DMA_BASE_PTRS                            { DMA0, DMA1 }
#endif
/** Interrupt vectors for the DMA peripheral type */
#define DMA_IRQS                                 { DMA0_IRQn, DMA1_IRQn }

/* DMIC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral DMIC0 base address */
  #define DMIC0_BASE                               (0x50121000u)
  /** Peripheral DMIC0 base address */
  #define DMIC0_BASE_NS                            (0x40121000u)
  /** Peripheral DMIC0 base pointer */
  #define DMIC0                                    ((DMIC_Type *)DMIC0_BASE)
  /** Peripheral DMIC0 base pointer */
  #define DMIC0_NS                                 ((DMIC_Type *)DMIC0_BASE_NS)
  /** Array initializer of DMIC peripheral base addresses */
  #define DMIC_BASE_ADDRS                          { DMIC0_BASE }
  /** Array initializer of DMIC peripheral base pointers */
  #define DMIC_BASE_PTRS                           { DMIC0 }
  /** Array initializer of DMIC peripheral base addresses */
  #define DMIC_BASE_ADDRS_NS                       { DMIC0_BASE_NS }
  /** Array initializer of DMIC peripheral base pointers */
  #define DMIC_BASE_PTRS_NS                        { DMIC0_NS }
#else
  /** Peripheral DMIC0 base address */
  #define DMIC0_BASE                               (0x40121000u)
  /** Peripheral DMIC0 base pointer */
  #define DMIC0                                    ((DMIC_Type *)DMIC0_BASE)
  /** Array initializer of DMIC peripheral base addresses */
  #define DMIC_BASE_ADDRS                          { DMIC0_BASE }
  /** Array initializer of DMIC peripheral base pointers */
  #define DMIC_BASE_PTRS                           { DMIC0 }
#endif
/** Interrupt vectors for the DMIC peripheral type */
#define DMIC_IRQS                                { DMIC_IRQn }
#define DMIC_HWVAD_IRQS                          { HWVAD_IRQn }

/* ELS - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral ELS base address */
  #define ELS_BASE                                 (0x50007000u)
  /** Peripheral ELS base address */
  #define ELS_BASE_NS                              (0x40007000u)
  /** Peripheral ELS base pointer */
  #define ELS                                      ((ELS_Type *)ELS_BASE)
  /** Peripheral ELS base pointer */
  #define ELS_NS                                   ((ELS_Type *)ELS_BASE_NS)
  /** Array initializer of ELS peripheral base addresses */
  #define ELS_BASE_ADDRS                           { ELS_BASE }
  /** Array initializer of ELS peripheral base pointers */
  #define ELS_BASE_PTRS                            { ELS }
  /** Array initializer of ELS peripheral base addresses */
  #define ELS_BASE_ADDRS_NS                        { ELS_BASE_NS }
  /** Array initializer of ELS peripheral base pointers */
  #define ELS_BASE_PTRS_NS                         { ELS_NS }
#else
  /** Peripheral ELS base address */
  #define ELS_BASE                                 (0x40007000u)
  /** Peripheral ELS base pointer */
  #define ELS                                      ((ELS_Type *)ELS_BASE)
  /** Array initializer of ELS peripheral base addresses */
  #define ELS_BASE_ADDRS                           { ELS_BASE }
  /** Array initializer of ELS peripheral base pointers */
  #define ELS_BASE_PTRS                            { ELS }
#endif

/* ENET - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral ENET base address */
  #define ENET_BASE                                (0x50138000u)
  /** Peripheral ENET base address */
  #define ENET_BASE_NS                             (0x40138000u)
  /** Peripheral ENET base pointer */
  #define ENET                                     ((ENET_Type *)ENET_BASE)
  /** Peripheral ENET base pointer */
  #define ENET_NS                                  ((ENET_Type *)ENET_BASE_NS)
  /** Array initializer of ENET peripheral base addresses */
  #define ENET_BASE_ADDRS                          { ENET_BASE }
  /** Array initializer of ENET peripheral base pointers */
  #define ENET_BASE_PTRS                           { ENET }
  /** Array initializer of ENET peripheral base addresses */
  #define ENET_BASE_ADDRS_NS                       { ENET_BASE_NS }
  /** Array initializer of ENET peripheral base pointers */
  #define ENET_BASE_PTRS_NS                        { ENET_NS }
#else
  /** Peripheral ENET base address */
  #define ENET_BASE                                (0x40138000u)
  /** Peripheral ENET base pointer */
  #define ENET                                     ((ENET_Type *)ENET_BASE)
  /** Array initializer of ENET peripheral base addresses */
  #define ENET_BASE_ADDRS                          { ENET_BASE }
  /** Array initializer of ENET peripheral base pointers */
  #define ENET_BASE_PTRS                           { ENET }
#endif
/** Interrupt vectors for the ENET peripheral type */
#define ENET_Transmit_IRQS                       { ENET_IRQn }
#define ENET_Receive_IRQS                        { ENET_IRQn }
#define ENET_Error_IRQS                          { ENET_IRQn }
#define ENET_1588_Timer_IRQS                     { ENET_TIMER_IRQn }
#define ENET_Ts_IRQS                             { ENET_IRQn }

/* FLEXCOMM - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral FLEXCOMM0 base address */
  #define FLEXCOMM0_BASE                           (0x50106000u)
  /** Peripheral FLEXCOMM0 base address */
  #define FLEXCOMM0_BASE_NS                        (0x40106000u)
  /** Peripheral FLEXCOMM0 base pointer */
  #define FLEXCOMM0                                ((FLEXCOMM_Type *)FLEXCOMM0_BASE)
  /** Peripheral FLEXCOMM0 base pointer */
  #define FLEXCOMM0_NS                             ((FLEXCOMM_Type *)FLEXCOMM0_BASE_NS)
  /** Peripheral FLEXCOMM1 base address */
  #define FLEXCOMM1_BASE                           (0x50107000u)
  /** Peripheral FLEXCOMM1 base address */
  #define FLEXCOMM1_BASE_NS                        (0x40107000u)
  /** Peripheral FLEXCOMM1 base pointer */
  #define FLEXCOMM1                                ((FLEXCOMM_Type *)FLEXCOMM1_BASE)
  /** Peripheral FLEXCOMM1 base pointer */
  #define FLEXCOMM1_NS                             ((FLEXCOMM_Type *)FLEXCOMM1_BASE_NS)
  /** Peripheral FLEXCOMM2 base address */
  #define FLEXCOMM2_BASE                           (0x50108000u)
  /** Peripheral FLEXCOMM2 base address */
  #define FLEXCOMM2_BASE_NS                        (0x40108000u)
  /** Peripheral FLEXCOMM2 base pointer */
  #define FLEXCOMM2                                ((FLEXCOMM_Type *)FLEXCOMM2_BASE)
  /** Peripheral FLEXCOMM2 base pointer */
  #define FLEXCOMM2_NS                             ((FLEXCOMM_Type *)FLEXCOMM2_BASE_NS)
  /** Peripheral FLEXCOMM3 base address */
  #define FLEXCOMM3_BASE                           (0x50109000u)
  /** Peripheral FLEXCOMM3 base address */
  #define FLEXCOMM3_BASE_NS                        (0x40109000u)
  /** Peripheral FLEXCOMM3 base pointer */
  #define FLEXCOMM3                                ((FLEXCOMM_Type *)FLEXCOMM3_BASE)
  /** Peripheral FLEXCOMM3 base pointer */
  #define FLEXCOMM3_NS                             ((FLEXCOMM_Type *)FLEXCOMM3_BASE_NS)
  /** Peripheral FLEXCOMM14 base address */
  #define FLEXCOMM14_BASE                          (0x50126000u)
  /** Peripheral FLEXCOMM14 base address */
  #define FLEXCOMM14_BASE_NS                       (0x40126000u)
  /** Peripheral FLEXCOMM14 base pointer */
  #define FLEXCOMM14                               ((FLEXCOMM_Type *)FLEXCOMM14_BASE)
  /** Peripheral FLEXCOMM14 base pointer */
  #define FLEXCOMM14_NS                            ((FLEXCOMM_Type *)FLEXCOMM14_BASE_NS)
  /** Array initializer of FLEXCOMM peripheral base addresses */
  #define FLEXCOMM_BASE_ADDRS                      { FLEXCOMM0_BASE, FLEXCOMM1_BASE, FLEXCOMM2_BASE, FLEXCOMM3_BASE, FLEXCOMM14_BASE }
  /** Array initializer of FLEXCOMM peripheral base pointers */
  #define FLEXCOMM_BASE_PTRS                       { FLEXCOMM0, FLEXCOMM1, FLEXCOMM2, FLEXCOMM3, FLEXCOMM14 }
  /** Array initializer of FLEXCOMM peripheral base addresses */
  #define FLEXCOMM_BASE_ADDRS_NS                   { FLEXCOMM0_BASE_NS, FLEXCOMM1_BASE_NS, FLEXCOMM2_BASE_NS, FLEXCOMM3_BASE_NS, FLEXCOMM14_BASE_NS }
  /** Array initializer of FLEXCOMM peripheral base pointers */
  #define FLEXCOMM_BASE_PTRS_NS                    { FLEXCOMM0_NS, FLEXCOMM1_NS, FLEXCOMM2_NS, FLEXCOMM3_NS, FLEXCOMM14_NS }
#else
  /** Peripheral FLEXCOMM0 base address */
  #define FLEXCOMM0_BASE                           (0x40106000u)
  /** Peripheral FLEXCOMM0 base pointer */
  #define FLEXCOMM0                                ((FLEXCOMM_Type *)FLEXCOMM0_BASE)
  /** Peripheral FLEXCOMM1 base address */
  #define FLEXCOMM1_BASE                           (0x40107000u)
  /** Peripheral FLEXCOMM1 base pointer */
  #define FLEXCOMM1                                ((FLEXCOMM_Type *)FLEXCOMM1_BASE)
  /** Peripheral FLEXCOMM2 base address */
  #define FLEXCOMM2_BASE                           (0x40108000u)
  /** Peripheral FLEXCOMM2 base pointer */
  #define FLEXCOMM2                                ((FLEXCOMM_Type *)FLEXCOMM2_BASE)
  /** Peripheral FLEXCOMM3 base address */
  #define FLEXCOMM3_BASE                           (0x40109000u)
  /** Peripheral FLEXCOMM3 base pointer */
  #define FLEXCOMM3                                ((FLEXCOMM_Type *)FLEXCOMM3_BASE)
  /** Peripheral FLEXCOMM14 base address */
  #define FLEXCOMM14_BASE                          (0x40126000u)
  /** Peripheral FLEXCOMM14 base pointer */
  #define FLEXCOMM14                               ((FLEXCOMM_Type *)FLEXCOMM14_BASE)
  /** Array initializer of FLEXCOMM peripheral base addresses */
  #define FLEXCOMM_BASE_ADDRS                      { FLEXCOMM0_BASE, FLEXCOMM1_BASE, FLEXCOMM2_BASE, FLEXCOMM3_BASE, FLEXCOMM14_BASE }
  /** Array initializer of FLEXCOMM peripheral base pointers */
  #define FLEXCOMM_BASE_PTRS                       { FLEXCOMM0, FLEXCOMM1, FLEXCOMM2, FLEXCOMM3, FLEXCOMM14 }
#endif
/** Interrupt vectors for the FLEXCOMM peripheral type */
#define FLEXCOMM_IRQS                            { FLEXCOMM0_IRQn, FLEXCOMM1_IRQn, FLEXCOMM2_IRQn, FLEXCOMM3_IRQn, FLEXCOMM14_IRQn }

/* FLEXSPI - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral FLEXSPI base address */
  #define FLEXSPI_BASE                             (0x50134000u)
  /** Peripheral FLEXSPI base address */
  #define FLEXSPI_BASE_NS                          (0x40134000u)
  /** Peripheral FLEXSPI base pointer */
  #define FLEXSPI                                  ((FLEXSPI_Type *)FLEXSPI_BASE)
  /** Peripheral FLEXSPI base pointer */
  #define FLEXSPI_NS                               ((FLEXSPI_Type *)FLEXSPI_BASE_NS)
  /** Array initializer of FLEXSPI peripheral base addresses */
  #define FLEXSPI_BASE_ADDRS                       { FLEXSPI_BASE }
  /** Array initializer of FLEXSPI peripheral base pointers */
  #define FLEXSPI_BASE_PTRS                        { FLEXSPI }
  /** Array initializer of FLEXSPI peripheral base addresses */
  #define FLEXSPI_BASE_ADDRS_NS                    { FLEXSPI_BASE_NS }
  /** Array initializer of FLEXSPI peripheral base pointers */
  #define FLEXSPI_BASE_PTRS_NS                     { FLEXSPI_NS }
#else
  /** Peripheral FLEXSPI base address */
  #define FLEXSPI_BASE                             (0x40134000u)
  /** Peripheral FLEXSPI base pointer */
  #define FLEXSPI                                  ((FLEXSPI_Type *)FLEXSPI_BASE)
  /** Array initializer of FLEXSPI peripheral base addresses */
  #define FLEXSPI_BASE_ADDRS                       { FLEXSPI_BASE }
  /** Array initializer of FLEXSPI peripheral base pointers */
  #define FLEXSPI_BASE_PTRS                        { FLEXSPI }
#endif
/** Interrupt vectors for the FLEXSPI peripheral type */
#define FLEXSPI_IRQS                             { FLEXSPI_IRQn }
/** FlexSPI AMBA memory base alias count */
#define FLEXSPI_AMBA_BASE_ALIAS_COUNT     (3)
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
/* FlexSPI AMBA base address array. */
#define FlexSPI_AMBA_BASE_ARRAY                    { {0x18000000u, 0x38000000u, 0x58000000u} }
#define FlexSPI_AMBA_BASE_ARRAY_NS                 { {0x08000000u, 0x28000000u, 0x48000000u} }
/* FlexSPI AMBA end address array. */
#define FlexSPI_AMBA_END_ARRAY                     { {0x1FFFFFFFu, 0x3FFFFFFFu, 0x5FFFFFFFu} }
#define FlexSPI_AMBA_END_ARRAY_NS                  { {0x0FFFFFFFu, 0x2FFFFFFFu, 0x4FFFFFFFu} }
/** FlexSPI AMBA Cache0 address. */
#define FlexSPI_AMBA_PC_CACHE_BASE                 (0x18000000u)
/** FlexSPI AMBA Cache1 address. */
#define FlexSPI_AMBA_PS_CACHE_BASE                 (0x38000000u)
/** FlexSPI AMBA Non-Cache address. */
#define FlexSPI_AMBA_PS_NCACHE_BASE                (0x58000000u)
/** FlexSPI AMBA Cache0 address */
#define FlexSPI_AMBA_PC_CACHE_BASE_NS              (0x08000000u)
/** FlexSPI AMBA Cache1 address */
#define FlexSPI_AMBA_PS_CACHE_BASE_NS              (0x28000000u)
/** FlexSPI AMBA Non-Cache address */
#define FlexSPI_AMBA_PS_NCACHE_BASE_NS             (0x48000000u)
#else
/* FlexSPI AMBA base address array. */
#define FlexSPI_AMBA_BASE_ARRAY                    { {0x08000000u, 0x28000000u, 0x48000000u} }
/* FlexSPI AMBA end address array. */
#define FlexSPI_AMBA_END_ARRAY                     { {0x0FFFFFFFu, 0x2FFFFFFFu, 0x4FFFFFFFu} }
/** FlexSPI AMBA Cache0 address. */
#define FlexSPI_AMBA_PC_CACHE_BASE                 (0x08000000u)
/** FlexSPI AMBA Cache1 address. */
#define FlexSPI_AMBA_PS_CACHE_BASE                 (0x28000000u)
/** FlexSPI AMBA Non-cache address. */
#define FlexSPI_AMBA_PS_NCACHE_BASE                (0x48000000u)
#endif


/* FREQME - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral FREQME base address */
  #define FREQME_BASE                              (0x5002F000u)
  /** Peripheral FREQME base address */
  #define FREQME_BASE_NS                           (0x4002F000u)
  /** Peripheral FREQME base pointer */
  #define FREQME                                   ((FREQME_Type *)FREQME_BASE)
  /** Peripheral FREQME base pointer */
  #define FREQME_NS                                ((FREQME_Type *)FREQME_BASE_NS)
  /** Array initializer of FREQME peripheral base addresses */
  #define FREQME_BASE_ADDRS                        { FREQME_BASE }
  /** Array initializer of FREQME peripheral base pointers */
  #define FREQME_BASE_PTRS                         { FREQME }
  /** Array initializer of FREQME peripheral base addresses */
  #define FREQME_BASE_ADDRS_NS                     { FREQME_BASE_NS }
  /** Array initializer of FREQME peripheral base pointers */
  #define FREQME_BASE_PTRS_NS                      { FREQME_NS }
#else
  /** Peripheral FREQME base address */
  #define FREQME_BASE                              (0x4002F000u)
  /** Peripheral FREQME base pointer */
  #define FREQME                                   ((FREQME_Type *)FREQME_BASE)
  /** Array initializer of FREQME peripheral base addresses */
  #define FREQME_BASE_ADDRS                        { FREQME_BASE }
  /** Array initializer of FREQME peripheral base pointers */
  #define FREQME_BASE_PTRS                         { FREQME }
#endif

/* GDMA - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral GDMA base address */
  #define GDMA_BASE                                (0x5014E000u)
  /** Peripheral GDMA base address */
  #define GDMA_BASE_NS                             (0x4014E000u)
  /** Peripheral GDMA base pointer */
  #define GDMA                                     ((GDMA_Type *)GDMA_BASE)
  /** Peripheral GDMA base pointer */
  #define GDMA_NS                                  ((GDMA_Type *)GDMA_BASE_NS)
  /** Array initializer of GDMA peripheral base addresses */
  #define GDMA_BASE_ADDRS                          { GDMA_BASE }
  /** Array initializer of GDMA peripheral base pointers */
  #define GDMA_BASE_PTRS                           { GDMA }
  /** Array initializer of GDMA peripheral base addresses */
  #define GDMA_BASE_ADDRS_NS                       { GDMA_BASE_NS }
  /** Array initializer of GDMA peripheral base pointers */
  #define GDMA_BASE_PTRS_NS                        { GDMA_NS }
#else
  /** Peripheral GDMA base address */
  #define GDMA_BASE                                (0x4014E000u)
  /** Peripheral GDMA base pointer */
  #define GDMA                                     ((GDMA_Type *)GDMA_BASE)
  /** Array initializer of GDMA peripheral base addresses */
  #define GDMA_BASE_ADDRS                          { GDMA_BASE }
  /** Array initializer of GDMA peripheral base pointers */
  #define GDMA_BASE_PTRS                           { GDMA }
#endif
/** Interrupt vectors for the GDMA peripheral type */
#define GDMA_IRQS                                { GDMA_IRQn }

/* GPIO - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral GPIO base address */
  #define GPIO_BASE                                (0x50100000u)
  /** Peripheral GPIO base address */
  #define GPIO_BASE_NS                             (0x40100000u)
  /** Peripheral GPIO base pointer */
  #define GPIO                                     ((GPIO_Type *)GPIO_BASE)
  /** Peripheral GPIO base pointer */
  #define GPIO_NS                                  ((GPIO_Type *)GPIO_BASE_NS)
  /** Peripheral SECGPIO base address */
  #define SECGPIO_BASE                             (0x50154000u)
  /** Peripheral SECGPIO base address */
  #define SECGPIO_BASE_NS                          (0x40154000u)
  /** Peripheral SECGPIO base pointer */
  #define SECGPIO                                  ((GPIO_Type *)SECGPIO_BASE)
  /** Peripheral SECGPIO base pointer */
  #define SECGPIO_NS                               ((GPIO_Type *)SECGPIO_BASE_NS)
  /** Array initializer of GPIO peripheral base addresses */
  #define GPIO_BASE_ADDRS                          { GPIO_BASE, SECGPIO_BASE }
  /** Array initializer of GPIO peripheral base pointers */
  #define GPIO_BASE_PTRS                           { GPIO, SECGPIO }
  /** Array initializer of GPIO peripheral base addresses */
  #define GPIO_BASE_ADDRS_NS                       { GPIO_BASE_NS, SECGPIO_BASE_NS }
  /** Array initializer of GPIO peripheral base pointers */
  #define GPIO_BASE_PTRS_NS                        { GPIO_NS, SECGPIO_NS }
#else
  /** Peripheral GPIO base address */
  #define GPIO_BASE                                (0x40100000u)
  /** Peripheral GPIO base pointer */
  #define GPIO                                     ((GPIO_Type *)GPIO_BASE)
  /** Peripheral SECGPIO base address */
  #define SECGPIO_BASE                             (0x40154000u)
  /** Peripheral SECGPIO base pointer */
  #define SECGPIO                                  ((GPIO_Type *)SECGPIO_BASE)
  /** Array initializer of GPIO peripheral base addresses */
  #define GPIO_BASE_ADDRS                          { GPIO_BASE, SECGPIO_BASE }
  /** Array initializer of GPIO peripheral base pointers */
  #define GPIO_BASE_PTRS                           { GPIO, SECGPIO }
#endif

/* I2C - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral I2C0 base address */
  #define I2C0_BASE                                (0x50106000u)
  /** Peripheral I2C0 base address */
  #define I2C0_BASE_NS                             (0x40106000u)
  /** Peripheral I2C0 base pointer */
  #define I2C0                                     ((I2C_Type *)I2C0_BASE)
  /** Peripheral I2C0 base pointer */
  #define I2C0_NS                                  ((I2C_Type *)I2C0_BASE_NS)
  /** Peripheral I2C1 base address */
  #define I2C1_BASE                                (0x50107000u)
  /** Peripheral I2C1 base address */
  #define I2C1_BASE_NS                             (0x40107000u)
  /** Peripheral I2C1 base pointer */
  #define I2C1                                     ((I2C_Type *)I2C1_BASE)
  /** Peripheral I2C1 base pointer */
  #define I2C1_NS                                  ((I2C_Type *)I2C1_BASE_NS)
  /** Peripheral I2C2 base address */
  #define I2C2_BASE                                (0x50108000u)
  /** Peripheral I2C2 base address */
  #define I2C2_BASE_NS                             (0x40108000u)
  /** Peripheral I2C2 base pointer */
  #define I2C2                                     ((I2C_Type *)I2C2_BASE)
  /** Peripheral I2C2 base pointer */
  #define I2C2_NS                                  ((I2C_Type *)I2C2_BASE_NS)
  /** Peripheral I2C3 base address */
  #define I2C3_BASE                                (0x50109000u)
  /** Peripheral I2C3 base address */
  #define I2C3_BASE_NS                             (0x40109000u)
  /** Peripheral I2C3 base pointer */
  #define I2C3                                     ((I2C_Type *)I2C3_BASE)
  /** Peripheral I2C3 base pointer */
  #define I2C3_NS                                  ((I2C_Type *)I2C3_BASE_NS)
  /** Peripheral I2C14 base address */
  #define I2C14_BASE                               (0x50126000u)
  /** Peripheral I2C14 base address */
  #define I2C14_BASE_NS                            (0x40126000u)
  /** Peripheral I2C14 base pointer */
  #define I2C14                                    ((I2C_Type *)I2C14_BASE)
  /** Peripheral I2C14 base pointer */
  #define I2C14_NS                                 ((I2C_Type *)I2C14_BASE_NS)
  /** Array initializer of I2C peripheral base addresses */
  #define I2C_BASE_ADDRS                           { I2C0_BASE, I2C1_BASE, I2C2_BASE, I2C3_BASE, I2C14_BASE }
  /** Array initializer of I2C peripheral base pointers */
  #define I2C_BASE_PTRS                            { I2C0, I2C1, I2C2, I2C3, I2C14 }
  /** Array initializer of I2C peripheral base addresses */
  #define I2C_BASE_ADDRS_NS                        { I2C0_BASE_NS, I2C1_BASE_NS, I2C2_BASE_NS, I2C3_BASE_NS, I2C14_BASE_NS }
  /** Array initializer of I2C peripheral base pointers */
  #define I2C_BASE_PTRS_NS                         { I2C0_NS, I2C1_NS, I2C2_NS, I2C3_NS, I2C14_NS }
#else
  /** Peripheral I2C0 base address */
  #define I2C0_BASE                                (0x40106000u)
  /** Peripheral I2C0 base pointer */
  #define I2C0                                     ((I2C_Type *)I2C0_BASE)
  /** Peripheral I2C1 base address */
  #define I2C1_BASE                                (0x40107000u)
  /** Peripheral I2C1 base pointer */
  #define I2C1                                     ((I2C_Type *)I2C1_BASE)
  /** Peripheral I2C2 base address */
  #define I2C2_BASE                                (0x40108000u)
  /** Peripheral I2C2 base pointer */
  #define I2C2                                     ((I2C_Type *)I2C2_BASE)
  /** Peripheral I2C3 base address */
  #define I2C3_BASE                                (0x40109000u)
  /** Peripheral I2C3 base pointer */
  #define I2C3                                     ((I2C_Type *)I2C3_BASE)
  /** Peripheral I2C14 base address */
  #define I2C14_BASE                               (0x40126000u)
  /** Peripheral I2C14 base pointer */
  #define I2C14                                    ((I2C_Type *)I2C14_BASE)
  /** Array initializer of I2C peripheral base addresses */
  #define I2C_BASE_ADDRS                           { I2C0_BASE, I2C1_BASE, I2C2_BASE, I2C3_BASE, I2C14_BASE }
  /** Array initializer of I2C peripheral base pointers */
  #define I2C_BASE_PTRS                            { I2C0, I2C1, I2C2, I2C3, I2C14 }
#endif
/** Interrupt vectors for the I2C peripheral type */
#define I2C_IRQS                                 { FLEXCOMM0_IRQn, FLEXCOMM1_IRQn, FLEXCOMM2_IRQn, FLEXCOMM3_IRQn, FLEXCOMM14_IRQn }

/* I2S - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral I2S0 base address */
  #define I2S0_BASE                                (0x50106000u)
  /** Peripheral I2S0 base address */
  #define I2S0_BASE_NS                             (0x40106000u)
  /** Peripheral I2S0 base pointer */
  #define I2S0                                     ((I2S_Type *)I2S0_BASE)
  /** Peripheral I2S0 base pointer */
  #define I2S0_NS                                  ((I2S_Type *)I2S0_BASE_NS)
  /** Peripheral I2S1 base address */
  #define I2S1_BASE                                (0x50107000u)
  /** Peripheral I2S1 base address */
  #define I2S1_BASE_NS                             (0x40107000u)
  /** Peripheral I2S1 base pointer */
  #define I2S1                                     ((I2S_Type *)I2S1_BASE)
  /** Peripheral I2S1 base pointer */
  #define I2S1_NS                                  ((I2S_Type *)I2S1_BASE_NS)
  /** Peripheral I2S2 base address */
  #define I2S2_BASE                                (0x50108000u)
  /** Peripheral I2S2 base address */
  #define I2S2_BASE_NS                             (0x40108000u)
  /** Peripheral I2S2 base pointer */
  #define I2S2                                     ((I2S_Type *)I2S2_BASE)
  /** Peripheral I2S2 base pointer */
  #define I2S2_NS                                  ((I2S_Type *)I2S2_BASE_NS)
  /** Peripheral I2S3 base address */
  #define I2S3_BASE                                (0x50109000u)
  /** Peripheral I2S3 base address */
  #define I2S3_BASE_NS                             (0x40109000u)
  /** Peripheral I2S3 base pointer */
  #define I2S3                                     ((I2S_Type *)I2S3_BASE)
  /** Peripheral I2S3 base pointer */
  #define I2S3_NS                                  ((I2S_Type *)I2S3_BASE_NS)
  /** Peripheral I2S14 base address */
  #define I2S14_BASE                               (0x50126000u)
  /** Peripheral I2S14 base address */
  #define I2S14_BASE_NS                            (0x40126000u)
  /** Peripheral I2S14 base pointer */
  #define I2S14                                    ((I2S_Type *)I2S14_BASE)
  /** Peripheral I2S14 base pointer */
  #define I2S14_NS                                 ((I2S_Type *)I2S14_BASE_NS)
  /** Array initializer of I2S peripheral base addresses */
  #define I2S_BASE_ADDRS                           { I2S0_BASE, I2S1_BASE, I2S2_BASE, I2S3_BASE, I2S14_BASE }
  /** Array initializer of I2S peripheral base pointers */
  #define I2S_BASE_PTRS                            { I2S0, I2S1, I2S2, I2S3, I2S14 }
  /** Array initializer of I2S peripheral base addresses */
  #define I2S_BASE_ADDRS_NS                        { I2S0_BASE_NS, I2S1_BASE_NS, I2S2_BASE_NS, I2S3_BASE_NS, I2S14_BASE_NS }
  /** Array initializer of I2S peripheral base pointers */
  #define I2S_BASE_PTRS_NS                         { I2S0_NS, I2S1_NS, I2S2_NS, I2S3_NS, I2S14_NS }
#else
  /** Peripheral I2S0 base address */
  #define I2S0_BASE                                (0x40106000u)
  /** Peripheral I2S0 base pointer */
  #define I2S0                                     ((I2S_Type *)I2S0_BASE)
  /** Peripheral I2S1 base address */
  #define I2S1_BASE                                (0x40107000u)
  /** Peripheral I2S1 base pointer */
  #define I2S1                                     ((I2S_Type *)I2S1_BASE)
  /** Peripheral I2S2 base address */
  #define I2S2_BASE                                (0x40108000u)
  /** Peripheral I2S2 base pointer */
  #define I2S2                                     ((I2S_Type *)I2S2_BASE)
  /** Peripheral I2S3 base address */
  #define I2S3_BASE                                (0x40109000u)
  /** Peripheral I2S3 base pointer */
  #define I2S3                                     ((I2S_Type *)I2S3_BASE)
  /** Peripheral I2S14 base address */
  #define I2S14_BASE                               (0x40126000u)
  /** Peripheral I2S14 base pointer */
  #define I2S14                                    ((I2S_Type *)I2S14_BASE)
  /** Array initializer of I2S peripheral base addresses */
  #define I2S_BASE_ADDRS                           { I2S0_BASE, I2S1_BASE, I2S2_BASE, I2S3_BASE, I2S14_BASE }
  /** Array initializer of I2S peripheral base pointers */
  #define I2S_BASE_PTRS                            { I2S0, I2S1, I2S2, I2S3, I2S14 }
#endif
/** Interrupt vectors for the I2S peripheral type */
#define I2S_IRQS                                 { FLEXCOMM0_IRQn, FLEXCOMM1_IRQn, FLEXCOMM2_IRQn, FLEXCOMM3_IRQn, FLEXCOMM14_IRQn }

/* INPUTMUX - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral INPUTMUX base address */
  #define INPUTMUX_BASE                            (0x50026000u)
  /** Peripheral INPUTMUX base address */
  #define INPUTMUX_BASE_NS                         (0x40026000u)
  /** Peripheral INPUTMUX base pointer */
  #define INPUTMUX                                 ((INPUTMUX_Type *)INPUTMUX_BASE)
  /** Peripheral INPUTMUX base pointer */
  #define INPUTMUX_NS                              ((INPUTMUX_Type *)INPUTMUX_BASE_NS)
  /** Array initializer of INPUTMUX peripheral base addresses */
  #define INPUTMUX_BASE_ADDRS                      { INPUTMUX_BASE }
  /** Array initializer of INPUTMUX peripheral base pointers */
  #define INPUTMUX_BASE_PTRS                       { INPUTMUX }
  /** Array initializer of INPUTMUX peripheral base addresses */
  #define INPUTMUX_BASE_ADDRS_NS                   { INPUTMUX_BASE_NS }
  /** Array initializer of INPUTMUX peripheral base pointers */
  #define INPUTMUX_BASE_PTRS_NS                    { INPUTMUX_NS }
#else
  /** Peripheral INPUTMUX base address */
  #define INPUTMUX_BASE                            (0x40026000u)
  /** Peripheral INPUTMUX base pointer */
  #define INPUTMUX                                 ((INPUTMUX_Type *)INPUTMUX_BASE)
  /** Array initializer of INPUTMUX peripheral base addresses */
  #define INPUTMUX_BASE_ADDRS                      { INPUTMUX_BASE }
  /** Array initializer of INPUTMUX peripheral base pointers */
  #define INPUTMUX_BASE_PTRS                       { INPUTMUX }
#endif

/* ITRC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral ITRC base address */
  #define ITRC_BASE                                (0x50024000u)
  /** Peripheral ITRC base address */
  #define ITRC_BASE_NS                             (0x40024000u)
  /** Peripheral ITRC base pointer */
  #define ITRC                                     ((ITRC_Type *)ITRC_BASE)
  /** Peripheral ITRC base pointer */
  #define ITRC_NS                                  ((ITRC_Type *)ITRC_BASE_NS)
  /** Array initializer of ITRC peripheral base addresses */
  #define ITRC_BASE_ADDRS                          { ITRC_BASE }
  /** Array initializer of ITRC peripheral base pointers */
  #define ITRC_BASE_PTRS                           { ITRC }
  /** Array initializer of ITRC peripheral base addresses */
  #define ITRC_BASE_ADDRS_NS                       { ITRC_BASE_NS }
  /** Array initializer of ITRC peripheral base pointers */
  #define ITRC_BASE_PTRS_NS                        { ITRC_NS }
#else
  /** Peripheral ITRC base address */
  #define ITRC_BASE                                (0x40024000u)
  /** Peripheral ITRC base pointer */
  #define ITRC                                     ((ITRC_Type *)ITRC_BASE)
  /** Array initializer of ITRC peripheral base addresses */
  #define ITRC_BASE_ADDRS                          { ITRC_BASE }
  /** Array initializer of ITRC peripheral base pointers */
  #define ITRC_BASE_PTRS                           { ITRC }
#endif

/* LCDIC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral LCDIC base address */
  #define LCDIC_BASE                               (0x50128000u)
  /** Peripheral LCDIC base address */
  #define LCDIC_BASE_NS                            (0x40128000u)
  /** Peripheral LCDIC base pointer */
  #define LCDIC                                    ((LCDIC_Type *)LCDIC_BASE)
  /** Peripheral LCDIC base pointer */
  #define LCDIC_NS                                 ((LCDIC_Type *)LCDIC_BASE_NS)
  /** Array initializer of LCDIC peripheral base addresses */
  #define LCDIC_BASE_ADDRS                         { LCDIC_BASE }
  /** Array initializer of LCDIC peripheral base pointers */
  #define LCDIC_BASE_PTRS                          { LCDIC }
  /** Array initializer of LCDIC peripheral base addresses */
  #define LCDIC_BASE_ADDRS_NS                      { LCDIC_BASE_NS }
  /** Array initializer of LCDIC peripheral base pointers */
  #define LCDIC_BASE_PTRS_NS                       { LCDIC_NS }
#else
  /** Peripheral LCDIC base address */
  #define LCDIC_BASE                               (0x40128000u)
  /** Peripheral LCDIC base pointer */
  #define LCDIC                                    ((LCDIC_Type *)LCDIC_BASE)
  /** Array initializer of LCDIC peripheral base addresses */
  #define LCDIC_BASE_ADDRS                         { LCDIC_BASE }
  /** Array initializer of LCDIC peripheral base pointers */
  #define LCDIC_BASE_PTRS                          { LCDIC }
#endif
/** Interrupt vectors for the LCDIC peripheral type */
#define LCDIC_IRQS                               { LCD_IRQn }

/* MCI_IO_MUX - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral MCI_IO_MUX base address */
  #define MCI_IO_MUX_BASE                          (0x50004000u)
  /** Peripheral MCI_IO_MUX base address */
  #define MCI_IO_MUX_BASE_NS                       (0x40004000u)
  /** Peripheral MCI_IO_MUX base pointer */
  #define MCI_IO_MUX                               ((MCI_IO_MUX_Type *)MCI_IO_MUX_BASE)
  /** Peripheral MCI_IO_MUX base pointer */
  #define MCI_IO_MUX_NS                            ((MCI_IO_MUX_Type *)MCI_IO_MUX_BASE_NS)
  /** Array initializer of MCI_IO_MUX peripheral base addresses */
  #define MCI_IO_MUX_BASE_ADDRS                    { MCI_IO_MUX_BASE }
  /** Array initializer of MCI_IO_MUX peripheral base pointers */
  #define MCI_IO_MUX_BASE_PTRS                     { MCI_IO_MUX }
  /** Array initializer of MCI_IO_MUX peripheral base addresses */
  #define MCI_IO_MUX_BASE_ADDRS_NS                 { MCI_IO_MUX_BASE_NS }
  /** Array initializer of MCI_IO_MUX peripheral base pointers */
  #define MCI_IO_MUX_BASE_PTRS_NS                  { MCI_IO_MUX_NS }
#else
  /** Peripheral MCI_IO_MUX base address */
  #define MCI_IO_MUX_BASE                          (0x40004000u)
  /** Peripheral MCI_IO_MUX base pointer */
  #define MCI_IO_MUX                               ((MCI_IO_MUX_Type *)MCI_IO_MUX_BASE)
  /** Array initializer of MCI_IO_MUX peripheral base addresses */
  #define MCI_IO_MUX_BASE_ADDRS                    { MCI_IO_MUX_BASE }
  /** Array initializer of MCI_IO_MUX peripheral base pointers */
  #define MCI_IO_MUX_BASE_PTRS                     { MCI_IO_MUX }
#endif

/* MRT - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral MRT0 base address */
  #define MRT0_BASE                                (0x5002D000u)
  /** Peripheral MRT0 base address */
  #define MRT0_BASE_NS                             (0x4002D000u)
  /** Peripheral MRT0 base pointer */
  #define MRT0                                     ((MRT_Type *)MRT0_BASE)
  /** Peripheral MRT0 base pointer */
  #define MRT0_NS                                  ((MRT_Type *)MRT0_BASE_NS)
  /** Peripheral MRT1 base address */
  #define MRT1_BASE                                (0x5003F000u)
  /** Peripheral MRT1 base address */
  #define MRT1_BASE_NS                             (0x4003F000u)
  /** Peripheral MRT1 base pointer */
  #define MRT1                                     ((MRT_Type *)MRT1_BASE)
  /** Peripheral MRT1 base pointer */
  #define MRT1_NS                                  ((MRT_Type *)MRT1_BASE_NS)
  /** Array initializer of MRT peripheral base addresses */
  #define MRT_BASE_ADDRS                           { MRT0_BASE, MRT1_BASE }
  /** Array initializer of MRT peripheral base pointers */
  #define MRT_BASE_PTRS                            { MRT0, MRT1 }
  /** Array initializer of MRT peripheral base addresses */
  #define MRT_BASE_ADDRS_NS                        { MRT0_BASE_NS, MRT1_BASE_NS }
  /** Array initializer of MRT peripheral base pointers */
  #define MRT_BASE_PTRS_NS                         { MRT0_NS, MRT1_NS }
#else
  /** Peripheral MRT0 base address */
  #define MRT0_BASE                                (0x4002D000u)
  /** Peripheral MRT0 base pointer */
  #define MRT0                                     ((MRT_Type *)MRT0_BASE)
  /** Peripheral MRT1 base address */
  #define MRT1_BASE                                (0x4003F000u)
  /** Peripheral MRT1 base pointer */
  #define MRT1                                     ((MRT_Type *)MRT1_BASE)
  /** Array initializer of MRT peripheral base addresses */
  #define MRT_BASE_ADDRS                           { MRT0_BASE, MRT1_BASE }
  /** Array initializer of MRT peripheral base pointers */
  #define MRT_BASE_PTRS                            { MRT0, MRT1 }
#endif
/** Interrupt vectors for the MRT peripheral type */
#define MRT_IRQS                                 { MRT_IRQn, GFMRT_IRQn }

/* OCOTP - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral OCOTP base address */
  #define OCOTP_BASE                               (0x5000A000u)
  /** Peripheral OCOTP base address */
  #define OCOTP_BASE_NS                            (0x4000A000u)
  /** Peripheral OCOTP base pointer */
  #define OCOTP                                    ((OCOTP_Type *)OCOTP_BASE)
  /** Peripheral OCOTP base pointer */
  #define OCOTP_NS                                 ((OCOTP_Type *)OCOTP_BASE_NS)
  /** Array initializer of OCOTP peripheral base addresses */
  #define OCOTP_BASE_ADDRS                         { OCOTP_BASE }
  /** Array initializer of OCOTP peripheral base pointers */
  #define OCOTP_BASE_PTRS                          { OCOTP }
  /** Array initializer of OCOTP peripheral base addresses */
  #define OCOTP_BASE_ADDRS_NS                      { OCOTP_BASE_NS }
  /** Array initializer of OCOTP peripheral base pointers */
  #define OCOTP_BASE_PTRS_NS                       { OCOTP_NS }
#else
  /** Peripheral OCOTP base address */
  #define OCOTP_BASE                               (0x4000A000u)
  /** Peripheral OCOTP base pointer */
  #define OCOTP                                    ((OCOTP_Type *)OCOTP_BASE)
  /** Array initializer of OCOTP peripheral base addresses */
  #define OCOTP_BASE_ADDRS                         { OCOTP_BASE }
  /** Array initializer of OCOTP peripheral base pointers */
  #define OCOTP_BASE_PTRS                          { OCOTP }
#endif

/* OSTIMER - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral OSTIMER base address */
  #define OSTIMER_BASE                             (0x5013B000u)
  /** Peripheral OSTIMER base address */
  #define OSTIMER_BASE_NS                          (0x4013B000u)
  /** Peripheral OSTIMER base pointer */
  #define OSTIMER                                  ((OSTIMER_Type *)OSTIMER_BASE)
  /** Peripheral OSTIMER base pointer */
  #define OSTIMER_NS                               ((OSTIMER_Type *)OSTIMER_BASE_NS)
  /** Array initializer of OSTIMER peripheral base addresses */
  #define OSTIMER_BASE_ADDRS                       { OSTIMER_BASE }
  /** Array initializer of OSTIMER peripheral base pointers */
  #define OSTIMER_BASE_PTRS                        { OSTIMER }
  /** Array initializer of OSTIMER peripheral base addresses */
  #define OSTIMER_BASE_ADDRS_NS                    { OSTIMER_BASE_NS }
  /** Array initializer of OSTIMER peripheral base pointers */
  #define OSTIMER_BASE_PTRS_NS                     { OSTIMER_NS }
#else
  /** Peripheral OSTIMER base address */
  #define OSTIMER_BASE                             (0x4013B000u)
  /** Peripheral OSTIMER base pointer */
  #define OSTIMER                                  ((OSTIMER_Type *)OSTIMER_BASE)
  /** Array initializer of OSTIMER peripheral base addresses */
  #define OSTIMER_BASE_ADDRS                       { OSTIMER_BASE }
  /** Array initializer of OSTIMER peripheral base pointers */
  #define OSTIMER_BASE_PTRS                        { OSTIMER }
#endif
/** Interrupt vectors for the OSTIMER peripheral type */
#define OSTIMER_IRQS                             { OS_EVENT_TIMER_IRQn }

/* PINT - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral PINT base address */
  #define PINT_BASE                                (0x50025000u)
  /** Peripheral PINT base address */
  #define PINT_BASE_NS                             (0x40025000u)
  /** Peripheral PINT base pointer */
  #define PINT                                     ((PINT_Type *)PINT_BASE)
  /** Peripheral PINT base pointer */
  #define PINT_NS                                  ((PINT_Type *)PINT_BASE_NS)
  /** Array initializer of PINT peripheral base addresses */
  #define PINT_BASE_ADDRS                          { PINT_BASE }
  /** Array initializer of PINT peripheral base pointers */
  #define PINT_BASE_PTRS                           { PINT }
  /** Array initializer of PINT peripheral base addresses */
  #define PINT_BASE_ADDRS_NS                       { PINT_BASE_NS }
  /** Array initializer of PINT peripheral base pointers */
  #define PINT_BASE_PTRS_NS                        { PINT_NS }
#else
  /** Peripheral PINT base address */
  #define PINT_BASE                                (0x40025000u)
  /** Peripheral PINT base pointer */
  #define PINT                                     ((PINT_Type *)PINT_BASE)
  /** Array initializer of PINT peripheral base addresses */
  #define PINT_BASE_ADDRS                          { PINT_BASE }
  /** Array initializer of PINT peripheral base pointers */
  #define PINT_BASE_PTRS                           { PINT }
#endif
/** Interrupt vectors for the PINT peripheral type */
#define PINT_IRQS                                { PIN_INT0_IRQn, PIN_INT1_IRQn, PIN_INT2_IRQn, PIN_INT3_IRQn, PIN_INT4_IRQn, PIN_INT5_IRQn, PIN_INT6_IRQn, PIN_INT7_IRQn }

/* PKC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral PKC base address */
  #define PKC_BASE                                 (0x50009000u)
  /** Peripheral PKC base address */
  #define PKC_BASE_NS                              (0x40009000u)
  /** Peripheral PKC base pointer */
  #define PKC                                      ((PKC_Type *)PKC_BASE)
  /** Peripheral PKC base pointer */
  #define PKC_NS                                   ((PKC_Type *)PKC_BASE_NS)
  /** Array initializer of PKC peripheral base addresses */
  #define PKC_BASE_ADDRS                           { PKC_BASE }
  /** Array initializer of PKC peripheral base pointers */
  #define PKC_BASE_PTRS                            { PKC }
  /** Array initializer of PKC peripheral base addresses */
  #define PKC_BASE_ADDRS_NS                        { PKC_BASE_NS }
  /** Array initializer of PKC peripheral base pointers */
  #define PKC_BASE_PTRS_NS                         { PKC_NS }
#else
  /** Peripheral PKC base address */
  #define PKC_BASE                                 (0x40009000u)
  /** Peripheral PKC base pointer */
  #define PKC                                      ((PKC_Type *)PKC_BASE)
  /** Array initializer of PKC peripheral base addresses */
  #define PKC_BASE_ADDRS                           { PKC_BASE }
  /** Array initializer of PKC peripheral base pointers */
  #define PKC_BASE_PTRS                            { PKC }
#endif

/* PMU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral PMU base address */
  #define PMU_BASE                                 (0x50031000u)
  /** Peripheral PMU base address */
  #define PMU_BASE_NS                              (0x40031000u)
  /** Peripheral PMU base pointer */
  #define PMU                                      ((PMU_Type *)PMU_BASE)
  /** Peripheral PMU base pointer */
  #define PMU_NS                                   ((PMU_Type *)PMU_BASE_NS)
  /** Array initializer of PMU peripheral base addresses */
  #define PMU_BASE_ADDRS                           { PMU_BASE }
  /** Array initializer of PMU peripheral base pointers */
  #define PMU_BASE_PTRS                            { PMU }
  /** Array initializer of PMU peripheral base addresses */
  #define PMU_BASE_ADDRS_NS                        { PMU_BASE_NS }
  /** Array initializer of PMU peripheral base pointers */
  #define PMU_BASE_PTRS_NS                         { PMU_NS }
#else
  /** Peripheral PMU base address */
  #define PMU_BASE                                 (0x40031000u)
  /** Peripheral PMU base pointer */
  #define PMU                                      ((PMU_Type *)PMU_BASE)
  /** Array initializer of PMU peripheral base addresses */
  #define PMU_BASE_ADDRS                           { PMU_BASE }
  /** Array initializer of PMU peripheral base pointers */
  #define PMU_BASE_PTRS                            { PMU }
#endif

/* POWERQUAD - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral POWERQUAD base address */
  #define POWERQUAD_BASE                           (0x50150000u)
  /** Peripheral POWERQUAD base address */
  #define POWERQUAD_BASE_NS                        (0x40150000u)
  /** Peripheral POWERQUAD base pointer */
  #define POWERQUAD                                ((POWERQUAD_Type *)POWERQUAD_BASE)
  /** Peripheral POWERQUAD base pointer */
  #define POWERQUAD_NS                             ((POWERQUAD_Type *)POWERQUAD_BASE_NS)
  /** Array initializer of POWERQUAD peripheral base addresses */
  #define POWERQUAD_BASE_ADDRS                     { POWERQUAD_BASE }
  /** Array initializer of POWERQUAD peripheral base pointers */
  #define POWERQUAD_BASE_PTRS                      { POWERQUAD }
  /** Array initializer of POWERQUAD peripheral base addresses */
  #define POWERQUAD_BASE_ADDRS_NS                  { POWERQUAD_BASE_NS }
  /** Array initializer of POWERQUAD peripheral base pointers */
  #define POWERQUAD_BASE_PTRS_NS                   { POWERQUAD_NS }
#else
  /** Peripheral POWERQUAD base address */
  #define POWERQUAD_BASE                           (0x40150000u)
  /** Peripheral POWERQUAD base pointer */
  #define POWERQUAD                                ((POWERQUAD_Type *)POWERQUAD_BASE)
  /** Array initializer of POWERQUAD peripheral base addresses */
  #define POWERQUAD_BASE_ADDRS                     { POWERQUAD_BASE }
  /** Array initializer of POWERQUAD peripheral base pointers */
  #define POWERQUAD_BASE_PTRS                      { POWERQUAD }
#endif
/** Interrupt vectors for the POWERQUAD peripheral type */
#define POWERQUAD_IRQS                           { POWERQUAD_IRQn }

/* PUF - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral PUF base address */
  #define PUF_BASE                                 (0x50006000u)
  /** Peripheral PUF base address */
  #define PUF_BASE_NS                              (0x40006000u)
  /** Peripheral PUF base pointer */
  #define PUF                                      ((PUF_Type *)PUF_BASE)
  /** Peripheral PUF base pointer */
  #define PUF_NS                                   ((PUF_Type *)PUF_BASE_NS)
  /** Array initializer of PUF peripheral base addresses */
  #define PUF_BASE_ADDRS                           { PUF_BASE }
  /** Array initializer of PUF peripheral base pointers */
  #define PUF_BASE_PTRS                            { PUF }
  /** Array initializer of PUF peripheral base addresses */
  #define PUF_BASE_ADDRS_NS                        { PUF_BASE_NS }
  /** Array initializer of PUF peripheral base pointers */
  #define PUF_BASE_PTRS_NS                         { PUF_NS }
#else
  /** Peripheral PUF base address */
  #define PUF_BASE                                 (0x40006000u)
  /** Peripheral PUF base pointer */
  #define PUF                                      ((PUF_Type *)PUF_BASE)
  /** Array initializer of PUF peripheral base addresses */
  #define PUF_BASE_ADDRS                           { PUF_BASE }
  /** Array initializer of PUF peripheral base pointers */
  #define PUF_BASE_PTRS                            { PUF }
#endif

/* RF_SYSCON - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral RF_SYSCON base address */
  #define RF_SYSCON_BASE                           (0x5003B000u)
  /** Peripheral RF_SYSCON base address */
  #define RF_SYSCON_BASE_NS                        (0x4003B000u)
  /** Peripheral RF_SYSCON base pointer */
  #define RF_SYSCON                                ((RF_SYSCON_Type *)RF_SYSCON_BASE)
  /** Peripheral RF_SYSCON base pointer */
  #define RF_SYSCON_NS                             ((RF_SYSCON_Type *)RF_SYSCON_BASE_NS)
  /** Array initializer of RF_SYSCON peripheral base addresses */
  #define RF_SYSCON_BASE_ADDRS                     { RF_SYSCON_BASE }
  /** Array initializer of RF_SYSCON peripheral base pointers */
  #define RF_SYSCON_BASE_PTRS                      { RF_SYSCON }
  /** Array initializer of RF_SYSCON peripheral base addresses */
  #define RF_SYSCON_BASE_ADDRS_NS                  { RF_SYSCON_BASE_NS }
  /** Array initializer of RF_SYSCON peripheral base pointers */
  #define RF_SYSCON_BASE_PTRS_NS                   { RF_SYSCON_NS }
#else
  /** Peripheral RF_SYSCON base address */
  #define RF_SYSCON_BASE                           (0x4003B000u)
  /** Peripheral RF_SYSCON base pointer */
  #define RF_SYSCON                                ((RF_SYSCON_Type *)RF_SYSCON_BASE)
  /** Array initializer of RF_SYSCON peripheral base addresses */
  #define RF_SYSCON_BASE_ADDRS                     { RF_SYSCON_BASE }
  /** Array initializer of RF_SYSCON peripheral base pointers */
  #define RF_SYSCON_BASE_PTRS                      { RF_SYSCON }
#endif

/* ROMCP - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral ROMCP base address */
  #define ROMCP_BASE                               (0x5013C000u)
  /** Peripheral ROMCP base address */
  #define ROMCP_BASE_NS                            (0x4013C000u)
  /** Peripheral ROMCP base pointer */
  #define ROMCP                                    ((ROMCP_Type *)ROMCP_BASE)
  /** Peripheral ROMCP base pointer */
  #define ROMCP_NS                                 ((ROMCP_Type *)ROMCP_BASE_NS)
  /** Array initializer of ROMCP peripheral base addresses */
  #define ROMCP_BASE_ADDRS                         { ROMCP_BASE }
  /** Array initializer of ROMCP peripheral base pointers */
  #define ROMCP_BASE_PTRS                          { ROMCP }
  /** Array initializer of ROMCP peripheral base addresses */
  #define ROMCP_BASE_ADDRS_NS                      { ROMCP_BASE_NS }
  /** Array initializer of ROMCP peripheral base pointers */
  #define ROMCP_BASE_PTRS_NS                       { ROMCP_NS }
#else
  /** Peripheral ROMCP base address */
  #define ROMCP_BASE                               (0x4013C000u)
  /** Peripheral ROMCP base pointer */
  #define ROMCP                                    ((ROMCP_Type *)ROMCP_BASE)
  /** Array initializer of ROMCP peripheral base addresses */
  #define ROMCP_BASE_ADDRS                         { ROMCP_BASE }
  /** Array initializer of ROMCP peripheral base pointers */
  #define ROMCP_BASE_PTRS                          { ROMCP }
#endif

/* RSTCTL0 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral RSTCTL0 base address */
  #define RSTCTL0_BASE                             (0x50000000u)
  /** Peripheral RSTCTL0 base address */
  #define RSTCTL0_BASE_NS                          (0x40000000u)
  /** Peripheral RSTCTL0 base pointer */
  #define RSTCTL0                                  ((RSTCTL0_Type *)RSTCTL0_BASE)
  /** Peripheral RSTCTL0 base pointer */
  #define RSTCTL0_NS                               ((RSTCTL0_Type *)RSTCTL0_BASE_NS)
  /** Array initializer of RSTCTL0 peripheral base addresses */
  #define RSTCTL0_BASE_ADDRS                       { RSTCTL0_BASE }
  /** Array initializer of RSTCTL0 peripheral base pointers */
  #define RSTCTL0_BASE_PTRS                        { RSTCTL0 }
  /** Array initializer of RSTCTL0 peripheral base addresses */
  #define RSTCTL0_BASE_ADDRS_NS                    { RSTCTL0_BASE_NS }
  /** Array initializer of RSTCTL0 peripheral base pointers */
  #define RSTCTL0_BASE_PTRS_NS                     { RSTCTL0_NS }
#else
  /** Peripheral RSTCTL0 base address */
  #define RSTCTL0_BASE                             (0x40000000u)
  /** Peripheral RSTCTL0 base pointer */
  #define RSTCTL0                                  ((RSTCTL0_Type *)RSTCTL0_BASE)
  /** Array initializer of RSTCTL0 peripheral base addresses */
  #define RSTCTL0_BASE_ADDRS                       { RSTCTL0_BASE }
  /** Array initializer of RSTCTL0 peripheral base pointers */
  #define RSTCTL0_BASE_PTRS                        { RSTCTL0 }
#endif

/* RSTCTL1 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral RSTCTL1 base address */
  #define RSTCTL1_BASE                             (0x50020000u)
  /** Peripheral RSTCTL1 base address */
  #define RSTCTL1_BASE_NS                          (0x40020000u)
  /** Peripheral RSTCTL1 base pointer */
  #define RSTCTL1                                  ((RSTCTL1_Type *)RSTCTL1_BASE)
  /** Peripheral RSTCTL1 base pointer */
  #define RSTCTL1_NS                               ((RSTCTL1_Type *)RSTCTL1_BASE_NS)
  /** Array initializer of RSTCTL1 peripheral base addresses */
  #define RSTCTL1_BASE_ADDRS                       { RSTCTL1_BASE }
  /** Array initializer of RSTCTL1 peripheral base pointers */
  #define RSTCTL1_BASE_PTRS                        { RSTCTL1 }
  /** Array initializer of RSTCTL1 peripheral base addresses */
  #define RSTCTL1_BASE_ADDRS_NS                    { RSTCTL1_BASE_NS }
  /** Array initializer of RSTCTL1 peripheral base pointers */
  #define RSTCTL1_BASE_PTRS_NS                     { RSTCTL1_NS }
#else
  /** Peripheral RSTCTL1 base address */
  #define RSTCTL1_BASE                             (0x40020000u)
  /** Peripheral RSTCTL1 base pointer */
  #define RSTCTL1                                  ((RSTCTL1_Type *)RSTCTL1_BASE)
  /** Array initializer of RSTCTL1 peripheral base addresses */
  #define RSTCTL1_BASE_ADDRS                       { RSTCTL1_BASE }
  /** Array initializer of RSTCTL1 peripheral base pointers */
  #define RSTCTL1_BASE_PTRS                        { RSTCTL1 }
#endif

/* RTC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral RTC base address */
  #define RTC_BASE                                 (0x50030000u)
  /** Peripheral RTC base address */
  #define RTC_BASE_NS                              (0x40030000u)
  /** Peripheral RTC base pointer */
  #define RTC                                      ((RTC_Type *)RTC_BASE)
  /** Peripheral RTC base pointer */
  #define RTC_NS                                   ((RTC_Type *)RTC_BASE_NS)
  /** Array initializer of RTC peripheral base addresses */
  #define RTC_BASE_ADDRS                           { RTC_BASE }
  /** Array initializer of RTC peripheral base pointers */
  #define RTC_BASE_PTRS                            { RTC }
  /** Array initializer of RTC peripheral base addresses */
  #define RTC_BASE_ADDRS_NS                        { RTC_BASE_NS }
  /** Array initializer of RTC peripheral base pointers */
  #define RTC_BASE_PTRS_NS                         { RTC_NS }
#else
  /** Peripheral RTC base address */
  #define RTC_BASE                                 (0x40030000u)
  /** Peripheral RTC base pointer */
  #define RTC                                      ((RTC_Type *)RTC_BASE)
  /** Array initializer of RTC peripheral base addresses */
  #define RTC_BASE_ADDRS                           { RTC_BASE }
  /** Array initializer of RTC peripheral base pointers */
  #define RTC_BASE_PTRS                            { RTC }
#endif
/** Interrupt vectors for the RTC peripheral type */
#define RTC_IRQS                                 { RTC_IRQn }

/* SCT - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SCT0 base address */
  #define SCT0_BASE                                (0x50146000u)
  /** Peripheral SCT0 base address */
  #define SCT0_BASE_NS                             (0x40146000u)
  /** Peripheral SCT0 base pointer */
  #define SCT0                                     ((SCT_Type *)SCT0_BASE)
  /** Peripheral SCT0 base pointer */
  #define SCT0_NS                                  ((SCT_Type *)SCT0_BASE_NS)
  /** Array initializer of SCT peripheral base addresses */
  #define SCT_BASE_ADDRS                           { SCT0_BASE }
  /** Array initializer of SCT peripheral base pointers */
  #define SCT_BASE_PTRS                            { SCT0 }
  /** Array initializer of SCT peripheral base addresses */
  #define SCT_BASE_ADDRS_NS                        { SCT0_BASE_NS }
  /** Array initializer of SCT peripheral base pointers */
  #define SCT_BASE_PTRS_NS                         { SCT0_NS }
#else
  /** Peripheral SCT0 base address */
  #define SCT0_BASE                                (0x40146000u)
  /** Peripheral SCT0 base pointer */
  #define SCT0                                     ((SCT_Type *)SCT0_BASE)
  /** Array initializer of SCT peripheral base addresses */
  #define SCT_BASE_ADDRS                           { SCT0_BASE }
  /** Array initializer of SCT peripheral base pointers */
  #define SCT_BASE_PTRS                            { SCT0 }
#endif
/** Interrupt vectors for the SCT peripheral type */
#define SCT_IRQS                                 { SCT0_IRQn }

/* SDU_FBR_CARD - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SDU_FBR_CARD base address */
  #define SDU_FBR_CARD_BASE                        (0x50158020u)
  /** Peripheral SDU_FBR_CARD base address */
  #define SDU_FBR_CARD_BASE_NS                     (0x40158020u)
  /** Peripheral SDU_FBR_CARD base pointer */
  #define SDU_FBR_CARD                             ((SDU_FBR_CARD_Type *)SDU_FBR_CARD_BASE)
  /** Peripheral SDU_FBR_CARD base pointer */
  #define SDU_FBR_CARD_NS                          ((SDU_FBR_CARD_Type *)SDU_FBR_CARD_BASE_NS)
  /** Array initializer of SDU_FBR_CARD peripheral base addresses */
  #define SDU_FBR_CARD_BASE_ADDRS                  { SDU_FBR_CARD_BASE }
  /** Array initializer of SDU_FBR_CARD peripheral base pointers */
  #define SDU_FBR_CARD_BASE_PTRS                   { SDU_FBR_CARD }
  /** Array initializer of SDU_FBR_CARD peripheral base addresses */
  #define SDU_FBR_CARD_BASE_ADDRS_NS               { SDU_FBR_CARD_BASE_NS }
  /** Array initializer of SDU_FBR_CARD peripheral base pointers */
  #define SDU_FBR_CARD_BASE_PTRS_NS                { SDU_FBR_CARD_NS }
#else
  /** Peripheral SDU_FBR_CARD base address */
  #define SDU_FBR_CARD_BASE                        (0x40158020u)
  /** Peripheral SDU_FBR_CARD base pointer */
  #define SDU_FBR_CARD                             ((SDU_FBR_CARD_Type *)SDU_FBR_CARD_BASE)
  /** Array initializer of SDU_FBR_CARD peripheral base addresses */
  #define SDU_FBR_CARD_BASE_ADDRS                  { SDU_FBR_CARD_BASE }
  /** Array initializer of SDU_FBR_CARD peripheral base pointers */
  #define SDU_FBR_CARD_BASE_PTRS                   { SDU_FBR_CARD }
#endif

/* SDU_FN0_CARD - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SDU_FN0_CARD base address */
  #define SDU_FN0_CARD_BASE                        (0x50158000u)
  /** Peripheral SDU_FN0_CARD base address */
  #define SDU_FN0_CARD_BASE_NS                     (0x40158000u)
  /** Peripheral SDU_FN0_CARD base pointer */
  #define SDU_FN0_CARD                             ((SDU_FN0_CARD_Type *)SDU_FN0_CARD_BASE)
  /** Peripheral SDU_FN0_CARD base pointer */
  #define SDU_FN0_CARD_NS                          ((SDU_FN0_CARD_Type *)SDU_FN0_CARD_BASE_NS)
  /** Array initializer of SDU_FN0_CARD peripheral base addresses */
  #define SDU_FN0_CARD_BASE_ADDRS                  { SDU_FN0_CARD_BASE }
  /** Array initializer of SDU_FN0_CARD peripheral base pointers */
  #define SDU_FN0_CARD_BASE_PTRS                   { SDU_FN0_CARD }
  /** Array initializer of SDU_FN0_CARD peripheral base addresses */
  #define SDU_FN0_CARD_BASE_ADDRS_NS               { SDU_FN0_CARD_BASE_NS }
  /** Array initializer of SDU_FN0_CARD peripheral base pointers */
  #define SDU_FN0_CARD_BASE_PTRS_NS                { SDU_FN0_CARD_NS }
#else
  /** Peripheral SDU_FN0_CARD base address */
  #define SDU_FN0_CARD_BASE                        (0x40158000u)
  /** Peripheral SDU_FN0_CARD base pointer */
  #define SDU_FN0_CARD                             ((SDU_FN0_CARD_Type *)SDU_FN0_CARD_BASE)
  /** Array initializer of SDU_FN0_CARD peripheral base addresses */
  #define SDU_FN0_CARD_BASE_ADDRS                  { SDU_FN0_CARD_BASE }
  /** Array initializer of SDU_FN0_CARD peripheral base pointers */
  #define SDU_FN0_CARD_BASE_PTRS                   { SDU_FN0_CARD }
#endif

/* SDU_FN_CARD - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SDU_FN_CARD base address */
  #define SDU_FN_CARD_BASE                         (0x50158100u)
  /** Peripheral SDU_FN_CARD base address */
  #define SDU_FN_CARD_BASE_NS                      (0x40158100u)
  /** Peripheral SDU_FN_CARD base pointer */
  #define SDU_FN_CARD                              ((SDU_FN_CARD_Type *)SDU_FN_CARD_BASE)
  /** Peripheral SDU_FN_CARD base pointer */
  #define SDU_FN_CARD_NS                           ((SDU_FN_CARD_Type *)SDU_FN_CARD_BASE_NS)
  /** Array initializer of SDU_FN_CARD peripheral base addresses */
  #define SDU_FN_CARD_BASE_ADDRS                   { SDU_FN_CARD_BASE }
  /** Array initializer of SDU_FN_CARD peripheral base pointers */
  #define SDU_FN_CARD_BASE_PTRS                    { SDU_FN_CARD }
  /** Array initializer of SDU_FN_CARD peripheral base addresses */
  #define SDU_FN_CARD_BASE_ADDRS_NS                { SDU_FN_CARD_BASE_NS }
  /** Array initializer of SDU_FN_CARD peripheral base pointers */
  #define SDU_FN_CARD_BASE_PTRS_NS                 { SDU_FN_CARD_NS }
#else
  /** Peripheral SDU_FN_CARD base address */
  #define SDU_FN_CARD_BASE                         (0x40158100u)
  /** Peripheral SDU_FN_CARD base pointer */
  #define SDU_FN_CARD                              ((SDU_FN_CARD_Type *)SDU_FN_CARD_BASE)
  /** Array initializer of SDU_FN_CARD peripheral base addresses */
  #define SDU_FN_CARD_BASE_ADDRS                   { SDU_FN_CARD_BASE }
  /** Array initializer of SDU_FN_CARD peripheral base pointers */
  #define SDU_FN_CARD_BASE_PTRS                    { SDU_FN_CARD }
#endif

/* SENSOR_CTRL - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SENSOR_CTRL base address */
  #define SENSOR_CTRL_BASE                         (0x55004000u)
  /** Peripheral SENSOR_CTRL base address */
  #define SENSOR_CTRL_BASE_NS                      (0x45004000u)
  /** Peripheral SENSOR_CTRL base pointer */
  #define SENSOR_CTRL                              ((SENSOR_CTRL_Type *)SENSOR_CTRL_BASE)
  /** Peripheral SENSOR_CTRL base pointer */
  #define SENSOR_CTRL_NS                           ((SENSOR_CTRL_Type *)SENSOR_CTRL_BASE_NS)
  /** Array initializer of SENSOR_CTRL peripheral base addresses */
  #define SENSOR_CTRL_BASE_ADDRS                   { SENSOR_CTRL_BASE }
  /** Array initializer of SENSOR_CTRL peripheral base pointers */
  #define SENSOR_CTRL_BASE_PTRS                    { SENSOR_CTRL }
  /** Array initializer of SENSOR_CTRL peripheral base addresses */
  #define SENSOR_CTRL_BASE_ADDRS_NS                { SENSOR_CTRL_BASE_NS }
  /** Array initializer of SENSOR_CTRL peripheral base pointers */
  #define SENSOR_CTRL_BASE_PTRS_NS                 { SENSOR_CTRL_NS }
#else
  /** Peripheral SENSOR_CTRL base address */
  #define SENSOR_CTRL_BASE                         (0x45004000u)
  /** Peripheral SENSOR_CTRL base pointer */
  #define SENSOR_CTRL                              ((SENSOR_CTRL_Type *)SENSOR_CTRL_BASE)
  /** Array initializer of SENSOR_CTRL peripheral base addresses */
  #define SENSOR_CTRL_BASE_ADDRS                   { SENSOR_CTRL_BASE }
  /** Array initializer of SENSOR_CTRL peripheral base pointers */
  #define SENSOR_CTRL_BASE_PTRS                    { SENSOR_CTRL }
#endif

/* SOCCIU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SOCCTRL base address */
  #define SOCCTRL_BASE                             (0x55001000u)
  /** Peripheral SOCCTRL base address */
  #define SOCCTRL_BASE_NS                          (0x45001000u)
  /** Peripheral SOCCTRL base pointer */
  #define SOCCTRL                                  ((SOCCIU_Type *)SOCCTRL_BASE)
  /** Peripheral SOCCTRL base pointer */
  #define SOCCTRL_NS                               ((SOCCIU_Type *)SOCCTRL_BASE_NS)
  /** Array initializer of SOCCIU peripheral base addresses */
  #define SOCCIU_BASE_ADDRS                        { SOCCTRL_BASE }
  /** Array initializer of SOCCIU peripheral base pointers */
  #define SOCCIU_BASE_PTRS                         { SOCCTRL }
  /** Array initializer of SOCCIU peripheral base addresses */
  #define SOCCIU_BASE_ADDRS_NS                     { SOCCTRL_BASE_NS }
  /** Array initializer of SOCCIU peripheral base pointers */
  #define SOCCIU_BASE_PTRS_NS                      { SOCCTRL_NS }
#else
  /** Peripheral SOCCTRL base address */
  #define SOCCTRL_BASE                             (0x45001000u)
  /** Peripheral SOCCTRL base pointer */
  #define SOCCTRL                                  ((SOCCIU_Type *)SOCCTRL_BASE)
  /** Array initializer of SOCCIU peripheral base addresses */
  #define SOCCIU_BASE_ADDRS                        { SOCCTRL_BASE }
  /** Array initializer of SOCCIU peripheral base pointers */
  #define SOCCIU_BASE_PTRS                         { SOCCTRL }
#endif

/* SOC_OTP_CTRL - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SOC_OTP_CTRL base address */
  #define SOC_OTP_CTRL_BASE                        (0x55003400u)
  /** Peripheral SOC_OTP_CTRL base address */
  #define SOC_OTP_CTRL_BASE_NS                     (0x45003400u)
  /** Peripheral SOC_OTP_CTRL base pointer */
  #define SOC_OTP_CTRL                             ((SOC_OTP_CTRL_Type *)SOC_OTP_CTRL_BASE)
  /** Peripheral SOC_OTP_CTRL base pointer */
  #define SOC_OTP_CTRL_NS                          ((SOC_OTP_CTRL_Type *)SOC_OTP_CTRL_BASE_NS)
  /** Array initializer of SOC_OTP_CTRL peripheral base addresses */
  #define SOC_OTP_CTRL_BASE_ADDRS                  { SOC_OTP_CTRL_BASE }
  /** Array initializer of SOC_OTP_CTRL peripheral base pointers */
  #define SOC_OTP_CTRL_BASE_PTRS                   { SOC_OTP_CTRL }
  /** Array initializer of SOC_OTP_CTRL peripheral base addresses */
  #define SOC_OTP_CTRL_BASE_ADDRS_NS               { SOC_OTP_CTRL_BASE_NS }
  /** Array initializer of SOC_OTP_CTRL peripheral base pointers */
  #define SOC_OTP_CTRL_BASE_PTRS_NS                { SOC_OTP_CTRL_NS }
#else
  /** Peripheral SOC_OTP_CTRL base address */
  #define SOC_OTP_CTRL_BASE                        (0x45003400u)
  /** Peripheral SOC_OTP_CTRL base pointer */
  #define SOC_OTP_CTRL                             ((SOC_OTP_CTRL_Type *)SOC_OTP_CTRL_BASE)
  /** Array initializer of SOC_OTP_CTRL peripheral base addresses */
  #define SOC_OTP_CTRL_BASE_ADDRS                  { SOC_OTP_CTRL_BASE }
  /** Array initializer of SOC_OTP_CTRL peripheral base pointers */
  #define SOC_OTP_CTRL_BASE_PTRS                   { SOC_OTP_CTRL }
#endif

/* SPI - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SPI0 base address */
  #define SPI0_BASE                                (0x50106000u)
  /** Peripheral SPI0 base address */
  #define SPI0_BASE_NS                             (0x40106000u)
  /** Peripheral SPI0 base pointer */
  #define SPI0                                     ((SPI_Type *)SPI0_BASE)
  /** Peripheral SPI0 base pointer */
  #define SPI0_NS                                  ((SPI_Type *)SPI0_BASE_NS)
  /** Peripheral SPI1 base address */
  #define SPI1_BASE                                (0x50107000u)
  /** Peripheral SPI1 base address */
  #define SPI1_BASE_NS                             (0x40107000u)
  /** Peripheral SPI1 base pointer */
  #define SPI1                                     ((SPI_Type *)SPI1_BASE)
  /** Peripheral SPI1 base pointer */
  #define SPI1_NS                                  ((SPI_Type *)SPI1_BASE_NS)
  /** Peripheral SPI2 base address */
  #define SPI2_BASE                                (0x50108000u)
  /** Peripheral SPI2 base address */
  #define SPI2_BASE_NS                             (0x40108000u)
  /** Peripheral SPI2 base pointer */
  #define SPI2                                     ((SPI_Type *)SPI2_BASE)
  /** Peripheral SPI2 base pointer */
  #define SPI2_NS                                  ((SPI_Type *)SPI2_BASE_NS)
  /** Peripheral SPI3 base address */
  #define SPI3_BASE                                (0x50109000u)
  /** Peripheral SPI3 base address */
  #define SPI3_BASE_NS                             (0x40109000u)
  /** Peripheral SPI3 base pointer */
  #define SPI3                                     ((SPI_Type *)SPI3_BASE)
  /** Peripheral SPI3 base pointer */
  #define SPI3_NS                                  ((SPI_Type *)SPI3_BASE_NS)
  /** Peripheral SPI14 base address */
  #define SPI14_BASE                               (0x50126000u)
  /** Peripheral SPI14 base address */
  #define SPI14_BASE_NS                            (0x40126000u)
  /** Peripheral SPI14 base pointer */
  #define SPI14                                    ((SPI_Type *)SPI14_BASE)
  /** Peripheral SPI14 base pointer */
  #define SPI14_NS                                 ((SPI_Type *)SPI14_BASE_NS)
  /** Array initializer of SPI peripheral base addresses */
  #define SPI_BASE_ADDRS                           { SPI0_BASE, SPI1_BASE, SPI2_BASE, SPI3_BASE, SPI14_BASE }
  /** Array initializer of SPI peripheral base pointers */
  #define SPI_BASE_PTRS                            { SPI0, SPI1, SPI2, SPI3, SPI14 }
  /** Array initializer of SPI peripheral base addresses */
  #define SPI_BASE_ADDRS_NS                        { SPI0_BASE_NS, SPI1_BASE_NS, SPI2_BASE_NS, SPI3_BASE_NS, SPI14_BASE_NS }
  /** Array initializer of SPI peripheral base pointers */
  #define SPI_BASE_PTRS_NS                         { SPI0_NS, SPI1_NS, SPI2_NS, SPI3_NS, SPI14_NS }
#else
  /** Peripheral SPI0 base address */
  #define SPI0_BASE                                (0x40106000u)
  /** Peripheral SPI0 base pointer */
  #define SPI0                                     ((SPI_Type *)SPI0_BASE)
  /** Peripheral SPI1 base address */
  #define SPI1_BASE                                (0x40107000u)
  /** Peripheral SPI1 base pointer */
  #define SPI1                                     ((SPI_Type *)SPI1_BASE)
  /** Peripheral SPI2 base address */
  #define SPI2_BASE                                (0x40108000u)
  /** Peripheral SPI2 base pointer */
  #define SPI2                                     ((SPI_Type *)SPI2_BASE)
  /** Peripheral SPI3 base address */
  #define SPI3_BASE                                (0x40109000u)
  /** Peripheral SPI3 base pointer */
  #define SPI3                                     ((SPI_Type *)SPI3_BASE)
  /** Peripheral SPI14 base address */
  #define SPI14_BASE                               (0x40126000u)
  /** Peripheral SPI14 base pointer */
  #define SPI14                                    ((SPI_Type *)SPI14_BASE)
  /** Array initializer of SPI peripheral base addresses */
  #define SPI_BASE_ADDRS                           { SPI0_BASE, SPI1_BASE, SPI2_BASE, SPI3_BASE, SPI14_BASE }
  /** Array initializer of SPI peripheral base pointers */
  #define SPI_BASE_PTRS                            { SPI0, SPI1, SPI2, SPI3, SPI14 }
#endif
/** Interrupt vectors for the SPI peripheral type */
#define SPI_IRQS                                 { FLEXCOMM0_IRQn, FLEXCOMM1_IRQn, FLEXCOMM2_IRQn, FLEXCOMM3_IRQn, FLEXCOMM14_IRQn }

/* SYSCTL0 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SYSCTL0 base address */
  #define SYSCTL0_BASE                             (0x50002000u)
  /** Peripheral SYSCTL0 base address */
  #define SYSCTL0_BASE_NS                          (0x40002000u)
  /** Peripheral SYSCTL0 base pointer */
  #define SYSCTL0                                  ((SYSCTL0_Type *)SYSCTL0_BASE)
  /** Peripheral SYSCTL0 base pointer */
  #define SYSCTL0_NS                               ((SYSCTL0_Type *)SYSCTL0_BASE_NS)
  /** Array initializer of SYSCTL0 peripheral base addresses */
  #define SYSCTL0_BASE_ADDRS                       { SYSCTL0_BASE }
  /** Array initializer of SYSCTL0 peripheral base pointers */
  #define SYSCTL0_BASE_PTRS                        { SYSCTL0 }
  /** Array initializer of SYSCTL0 peripheral base addresses */
  #define SYSCTL0_BASE_ADDRS_NS                    { SYSCTL0_BASE_NS }
  /** Array initializer of SYSCTL0 peripheral base pointers */
  #define SYSCTL0_BASE_PTRS_NS                     { SYSCTL0_NS }
#else
  /** Peripheral SYSCTL0 base address */
  #define SYSCTL0_BASE                             (0x40002000u)
  /** Peripheral SYSCTL0 base pointer */
  #define SYSCTL0                                  ((SYSCTL0_Type *)SYSCTL0_BASE)
  /** Array initializer of SYSCTL0 peripheral base addresses */
  #define SYSCTL0_BASE_ADDRS                       { SYSCTL0_BASE }
  /** Array initializer of SYSCTL0 peripheral base pointers */
  #define SYSCTL0_BASE_PTRS                        { SYSCTL0 }
#endif

/* SYSCTL1 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SYSCTL1 base address */
  #define SYSCTL1_BASE                             (0x50022000u)
  /** Peripheral SYSCTL1 base address */
  #define SYSCTL1_BASE_NS                          (0x40022000u)
  /** Peripheral SYSCTL1 base pointer */
  #define SYSCTL1                                  ((SYSCTL1_Type *)SYSCTL1_BASE)
  /** Peripheral SYSCTL1 base pointer */
  #define SYSCTL1_NS                               ((SYSCTL1_Type *)SYSCTL1_BASE_NS)
  /** Array initializer of SYSCTL1 peripheral base addresses */
  #define SYSCTL1_BASE_ADDRS                       { SYSCTL1_BASE }
  /** Array initializer of SYSCTL1 peripheral base pointers */
  #define SYSCTL1_BASE_PTRS                        { SYSCTL1 }
  /** Array initializer of SYSCTL1 peripheral base addresses */
  #define SYSCTL1_BASE_ADDRS_NS                    { SYSCTL1_BASE_NS }
  /** Array initializer of SYSCTL1 peripheral base pointers */
  #define SYSCTL1_BASE_PTRS_NS                     { SYSCTL1_NS }
#else
  /** Peripheral SYSCTL1 base address */
  #define SYSCTL1_BASE                             (0x40022000u)
  /** Peripheral SYSCTL1 base pointer */
  #define SYSCTL1                                  ((SYSCTL1_Type *)SYSCTL1_BASE)
  /** Array initializer of SYSCTL1 peripheral base addresses */
  #define SYSCTL1_BASE_ADDRS                       { SYSCTL1_BASE }
  /** Array initializer of SYSCTL1 peripheral base pointers */
  #define SYSCTL1_BASE_PTRS                        { SYSCTL1 }
#endif

/* SYSCTL2 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SYSCTL2 base address */
  #define SYSCTL2_BASE                             (0x50003000u)
  /** Peripheral SYSCTL2 base address */
  #define SYSCTL2_BASE_NS                          (0x40003000u)
  /** Peripheral SYSCTL2 base pointer */
  #define SYSCTL2                                  ((SYSCTL2_Type *)SYSCTL2_BASE)
  /** Peripheral SYSCTL2 base pointer */
  #define SYSCTL2_NS                               ((SYSCTL2_Type *)SYSCTL2_BASE_NS)
  /** Array initializer of SYSCTL2 peripheral base addresses */
  #define SYSCTL2_BASE_ADDRS                       { SYSCTL2_BASE }
  /** Array initializer of SYSCTL2 peripheral base pointers */
  #define SYSCTL2_BASE_PTRS                        { SYSCTL2 }
  /** Array initializer of SYSCTL2 peripheral base addresses */
  #define SYSCTL2_BASE_ADDRS_NS                    { SYSCTL2_BASE_NS }
  /** Array initializer of SYSCTL2 peripheral base pointers */
  #define SYSCTL2_BASE_PTRS_NS                     { SYSCTL2_NS }
#else
  /** Peripheral SYSCTL2 base address */
  #define SYSCTL2_BASE                             (0x40003000u)
  /** Peripheral SYSCTL2 base pointer */
  #define SYSCTL2                                  ((SYSCTL2_Type *)SYSCTL2_BASE)
  /** Array initializer of SYSCTL2 peripheral base addresses */
  #define SYSCTL2_BASE_ADDRS                       { SYSCTL2_BASE }
  /** Array initializer of SYSCTL2 peripheral base pointers */
  #define SYSCTL2_BASE_PTRS                        { SYSCTL2 }
#endif

/* SYSPLL_T3 - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SYSPLL_T3 base address */
  #define SYSPLL_T3_BASE                           (0x55002000u)
  /** Peripheral SYSPLL_T3 base address */
  #define SYSPLL_T3_BASE_NS                        (0x45002000u)
  /** Peripheral SYSPLL_T3 base pointer */
  #define SYSPLL_T3                                ((SYSPLL_T3_Type *)SYSPLL_T3_BASE)
  /** Peripheral SYSPLL_T3 base pointer */
  #define SYSPLL_T3_NS                             ((SYSPLL_T3_Type *)SYSPLL_T3_BASE_NS)
  /** Array initializer of SYSPLL_T3 peripheral base addresses */
  #define SYSPLL_T3_BASE_ADDRS                     { SYSPLL_T3_BASE }
  /** Array initializer of SYSPLL_T3 peripheral base pointers */
  #define SYSPLL_T3_BASE_PTRS                      { SYSPLL_T3 }
  /** Array initializer of SYSPLL_T3 peripheral base addresses */
  #define SYSPLL_T3_BASE_ADDRS_NS                  { SYSPLL_T3_BASE_NS }
  /** Array initializer of SYSPLL_T3 peripheral base pointers */
  #define SYSPLL_T3_BASE_PTRS_NS                   { SYSPLL_T3_NS }
#else
  /** Peripheral SYSPLL_T3 base address */
  #define SYSPLL_T3_BASE                           (0x45002000u)
  /** Peripheral SYSPLL_T3 base pointer */
  #define SYSPLL_T3                                ((SYSPLL_T3_Type *)SYSPLL_T3_BASE)
  /** Array initializer of SYSPLL_T3 peripheral base addresses */
  #define SYSPLL_T3_BASE_ADDRS                     { SYSPLL_T3_BASE }
  /** Array initializer of SYSPLL_T3 peripheral base pointers */
  #define SYSPLL_T3_BASE_PTRS                      { SYSPLL_T3 }
#endif

/* SYSPLL_TCPU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral SYSPLL_TCPU base address */
  #define SYSPLL_TCPU_BASE                         (0x55002000u)
  /** Peripheral SYSPLL_TCPU base address */
  #define SYSPLL_TCPU_BASE_NS                      (0x45002000u)
  /** Peripheral SYSPLL_TCPU base pointer */
  #define SYSPLL_TCPU                              ((SYSPLL_TCPU_Type *)SYSPLL_TCPU_BASE)
  /** Peripheral SYSPLL_TCPU base pointer */
  #define SYSPLL_TCPU_NS                           ((SYSPLL_TCPU_Type *)SYSPLL_TCPU_BASE_NS)
  /** Array initializer of SYSPLL_TCPU peripheral base addresses */
  #define SYSPLL_TCPU_BASE_ADDRS                   { SYSPLL_TCPU_BASE }
  /** Array initializer of SYSPLL_TCPU peripheral base pointers */
  #define SYSPLL_TCPU_BASE_PTRS                    { SYSPLL_TCPU }
  /** Array initializer of SYSPLL_TCPU peripheral base addresses */
  #define SYSPLL_TCPU_BASE_ADDRS_NS                { SYSPLL_TCPU_BASE_NS }
  /** Array initializer of SYSPLL_TCPU peripheral base pointers */
  #define SYSPLL_TCPU_BASE_PTRS_NS                 { SYSPLL_TCPU_NS }
#else
  /** Peripheral SYSPLL_TCPU base address */
  #define SYSPLL_TCPU_BASE                         (0x45002000u)
  /** Peripheral SYSPLL_TCPU base pointer */
  #define SYSPLL_TCPU                              ((SYSPLL_TCPU_Type *)SYSPLL_TCPU_BASE)
  /** Array initializer of SYSPLL_TCPU peripheral base addresses */
  #define SYSPLL_TCPU_BASE_ADDRS                   { SYSPLL_TCPU_BASE }
  /** Array initializer of SYSPLL_TCPU peripheral base pointers */
  #define SYSPLL_TCPU_BASE_PTRS                    { SYSPLL_TCPU }
#endif

/* SYSPLL_TDDR - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral REG_SYSPLL_TDDR base address */
  #define REG_SYSPLL_TDDR_BASE                     (0x55002000u)
  /** Peripheral REG_SYSPLL_TDDR base address */
  #define REG_SYSPLL_TDDR_BASE_NS                  (0x45002000u)
  /** Peripheral REG_SYSPLL_TDDR base pointer */
  #define REG_SYSPLL_TDDR                          ((SYSPLL_TDDR_Type *)REG_SYSPLL_TDDR_BASE)
  /** Peripheral REG_SYSPLL_TDDR base pointer */
  #define REG_SYSPLL_TDDR_NS                       ((SYSPLL_TDDR_Type *)REG_SYSPLL_TDDR_BASE_NS)
  /** Array initializer of SYSPLL_TDDR peripheral base addresses */
  #define SYSPLL_TDDR_BASE_ADDRS                   { REG_SYSPLL_TDDR_BASE }
  /** Array initializer of SYSPLL_TDDR peripheral base pointers */
  #define SYSPLL_TDDR_BASE_PTRS                    { REG_SYSPLL_TDDR }
  /** Array initializer of SYSPLL_TDDR peripheral base addresses */
  #define SYSPLL_TDDR_BASE_ADDRS_NS                { REG_SYSPLL_TDDR_BASE_NS }
  /** Array initializer of SYSPLL_TDDR peripheral base pointers */
  #define SYSPLL_TDDR_BASE_PTRS_NS                 { REG_SYSPLL_TDDR_NS }
#else
  /** Peripheral REG_SYSPLL_TDDR base address */
  #define REG_SYSPLL_TDDR_BASE                     (0x45002000u)
  /** Peripheral REG_SYSPLL_TDDR base pointer */
  #define REG_SYSPLL_TDDR                          ((SYSPLL_TDDR_Type *)REG_SYSPLL_TDDR_BASE)
  /** Array initializer of SYSPLL_TDDR peripheral base addresses */
  #define SYSPLL_TDDR_BASE_ADDRS                   { REG_SYSPLL_TDDR_BASE }
  /** Array initializer of SYSPLL_TDDR peripheral base pointers */
  #define SYSPLL_TDDR_BASE_PTRS                    { REG_SYSPLL_TDDR }
#endif

/* TRNG - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral TRNG base address */
  #define TRNG_BASE                                (0x50014000u)
  /** Peripheral TRNG base address */
  #define TRNG_BASE_NS                             (0x40014000u)
  /** Peripheral TRNG base pointer */
  #define TRNG                                     ((TRNG_Type *)TRNG_BASE)
  /** Peripheral TRNG base pointer */
  #define TRNG_NS                                  ((TRNG_Type *)TRNG_BASE_NS)
  /** Array initializer of TRNG peripheral base addresses */
  #define TRNG_BASE_ADDRS                          { TRNG_BASE }
  /** Array initializer of TRNG peripheral base pointers */
  #define TRNG_BASE_PTRS                           { TRNG }
  /** Array initializer of TRNG peripheral base addresses */
  #define TRNG_BASE_ADDRS_NS                       { TRNG_BASE_NS }
  /** Array initializer of TRNG peripheral base pointers */
  #define TRNG_BASE_PTRS_NS                        { TRNG_NS }
#else
  /** Peripheral TRNG base address */
  #define TRNG_BASE                                (0x40014000u)
  /** Peripheral TRNG base pointer */
  #define TRNG                                     ((TRNG_Type *)TRNG_BASE)
  /** Array initializer of TRNG peripheral base addresses */
  #define TRNG_BASE_ADDRS                          { TRNG_BASE }
  /** Array initializer of TRNG peripheral base pointers */
  #define TRNG_BASE_PTRS                           { TRNG }
#endif

/* USART - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral USART0 base address */
  #define USART0_BASE                              (0x50106000u)
  /** Peripheral USART0 base address */
  #define USART0_BASE_NS                           (0x40106000u)
  /** Peripheral USART0 base pointer */
  #define USART0                                   ((USART_Type *)USART0_BASE)
  /** Peripheral USART0 base pointer */
  #define USART0_NS                                ((USART_Type *)USART0_BASE_NS)
  /** Peripheral USART1 base address */
  #define USART1_BASE                              (0x50107000u)
  /** Peripheral USART1 base address */
  #define USART1_BASE_NS                           (0x40107000u)
  /** Peripheral USART1 base pointer */
  #define USART1                                   ((USART_Type *)USART1_BASE)
  /** Peripheral USART1 base pointer */
  #define USART1_NS                                ((USART_Type *)USART1_BASE_NS)
  /** Peripheral USART2 base address */
  #define USART2_BASE                              (0x50108000u)
  /** Peripheral USART2 base address */
  #define USART2_BASE_NS                           (0x40108000u)
  /** Peripheral USART2 base pointer */
  #define USART2                                   ((USART_Type *)USART2_BASE)
  /** Peripheral USART2 base pointer */
  #define USART2_NS                                ((USART_Type *)USART2_BASE_NS)
  /** Peripheral USART3 base address */
  #define USART3_BASE                              (0x50109000u)
  /** Peripheral USART3 base address */
  #define USART3_BASE_NS                           (0x40109000u)
  /** Peripheral USART3 base pointer */
  #define USART3                                   ((USART_Type *)USART3_BASE)
  /** Peripheral USART3 base pointer */
  #define USART3_NS                                ((USART_Type *)USART3_BASE_NS)
  /** Peripheral USART14 base address */
  #define USART14_BASE                             (0x50126000u)
  /** Peripheral USART14 base address */
  #define USART14_BASE_NS                          (0x40126000u)
  /** Peripheral USART14 base pointer */
  #define USART14                                  ((USART_Type *)USART14_BASE)
  /** Peripheral USART14 base pointer */
  #define USART14_NS                               ((USART_Type *)USART14_BASE_NS)
  /** Array initializer of USART peripheral base addresses */
  #define USART_BASE_ADDRS                         { USART0_BASE, USART1_BASE, USART2_BASE, USART3_BASE, USART14_BASE }
  /** Array initializer of USART peripheral base pointers */
  #define USART_BASE_PTRS                          { USART0, USART1, USART2, USART3, USART14 }
  /** Array initializer of USART peripheral base addresses */
  #define USART_BASE_ADDRS_NS                      { USART0_BASE_NS, USART1_BASE_NS, USART2_BASE_NS, USART3_BASE_NS, USART14_BASE_NS }
  /** Array initializer of USART peripheral base pointers */
  #define USART_BASE_PTRS_NS                       { USART0_NS, USART1_NS, USART2_NS, USART3_NS, USART14_NS }
#else
  /** Peripheral USART0 base address */
  #define USART0_BASE                              (0x40106000u)
  /** Peripheral USART0 base pointer */
  #define USART0                                   ((USART_Type *)USART0_BASE)
  /** Peripheral USART1 base address */
  #define USART1_BASE                              (0x40107000u)
  /** Peripheral USART1 base pointer */
  #define USART1                                   ((USART_Type *)USART1_BASE)
  /** Peripheral USART2 base address */
  #define USART2_BASE                              (0x40108000u)
  /** Peripheral USART2 base pointer */
  #define USART2                                   ((USART_Type *)USART2_BASE)
  /** Peripheral USART3 base address */
  #define USART3_BASE                              (0x40109000u)
  /** Peripheral USART3 base pointer */
  #define USART3                                   ((USART_Type *)USART3_BASE)
  /** Peripheral USART14 base address */
  #define USART14_BASE                             (0x40126000u)
  /** Peripheral USART14 base pointer */
  #define USART14                                  ((USART_Type *)USART14_BASE)
  /** Array initializer of USART peripheral base addresses */
  #define USART_BASE_ADDRS                         { USART0_BASE, USART1_BASE, USART2_BASE, USART3_BASE, USART14_BASE }
  /** Array initializer of USART peripheral base pointers */
  #define USART_BASE_PTRS                          { USART0, USART1, USART2, USART3, USART14 }
#endif
/** Interrupt vectors for the USART peripheral type */
#define USART_IRQS                               { FLEXCOMM0_IRQn, FLEXCOMM1_IRQn, FLEXCOMM2_IRQn, FLEXCOMM3_IRQn, FLEXCOMM14_IRQn }

/* USBC - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral USBOTG base address */
  #define USBOTG_BASE                              (0x50145000u)
  /** Peripheral USBOTG base address */
  #define USBOTG_BASE_NS                           (0x40145000u)
  /** Peripheral USBOTG base pointer */
  #define USBOTG                                   ((USBC_Type *)USBOTG_BASE)
  /** Peripheral USBOTG base pointer */
  #define USBOTG_NS                                ((USBC_Type *)USBOTG_BASE_NS)
  /** Array initializer of USBC peripheral base addresses */
  #define USBC_BASE_ADDRS                          { USBOTG_BASE }
  /** Array initializer of USBC peripheral base pointers */
  #define USBC_BASE_PTRS                           { USBOTG }
  /** Array initializer of USBC peripheral base addresses */
  #define USBC_BASE_ADDRS_NS                       { USBOTG_BASE_NS }
  /** Array initializer of USBC peripheral base pointers */
  #define USBC_BASE_PTRS_NS                        { USBOTG_NS }
#else
  /** Peripheral USBOTG base address */
  #define USBOTG_BASE                              (0x40145000u)
  /** Peripheral USBOTG base pointer */
  #define USBOTG                                   ((USBC_Type *)USBOTG_BASE)
  /** Array initializer of USBC peripheral base addresses */
  #define USBC_BASE_ADDRS                          { USBOTG_BASE }
  /** Array initializer of USBC peripheral base pointers */
  #define USBC_BASE_PTRS                           { USBOTG }
#endif

/* USIM - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral USIM base address */
  #define USIM_BASE                                (0x50008000u)
  /** Peripheral USIM base address */
  #define USIM_BASE_NS                             (0x40008000u)
  /** Peripheral USIM base pointer */
  #define USIM                                     ((USIM_Type *)USIM_BASE)
  /** Peripheral USIM base pointer */
  #define USIM_NS                                  ((USIM_Type *)USIM_BASE_NS)
  /** Array initializer of USIM peripheral base addresses */
  #define USIM_BASE_ADDRS                          { USIM_BASE }
  /** Array initializer of USIM peripheral base pointers */
  #define USIM_BASE_PTRS                           { USIM }
  /** Array initializer of USIM peripheral base addresses */
  #define USIM_BASE_ADDRS_NS                       { USIM_BASE_NS }
  /** Array initializer of USIM peripheral base pointers */
  #define USIM_BASE_PTRS_NS                        { USIM_NS }
#else
  /** Peripheral USIM base address */
  #define USIM_BASE                                (0x40008000u)
  /** Peripheral USIM base pointer */
  #define USIM                                     ((USIM_Type *)USIM_BASE)
  /** Array initializer of USIM peripheral base addresses */
  #define USIM_BASE_ADDRS                          { USIM_BASE }
  /** Array initializer of USIM peripheral base pointers */
  #define USIM_BASE_PTRS                           { USIM }
#endif
/** Interrupt vectors for the USIM peripheral type */
#define USIM_IRQS                                { USIM_IRQn }

/* UTICK - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral UTICK base address */
  #define UTICK_BASE                               (0x5000F000u)
  /** Peripheral UTICK base address */
  #define UTICK_BASE_NS                            (0x4000F000u)
  /** Peripheral UTICK base pointer */
  #define UTICK                                    ((UTICK_Type *)UTICK_BASE)
  /** Peripheral UTICK base pointer */
  #define UTICK_NS                                 ((UTICK_Type *)UTICK_BASE_NS)
  /** Array initializer of UTICK peripheral base addresses */
  #define UTICK_BASE_ADDRS                         { UTICK_BASE }
  /** Array initializer of UTICK peripheral base pointers */
  #define UTICK_BASE_PTRS                          { UTICK }
  /** Array initializer of UTICK peripheral base addresses */
  #define UTICK_BASE_ADDRS_NS                      { UTICK_BASE_NS }
  /** Array initializer of UTICK peripheral base pointers */
  #define UTICK_BASE_PTRS_NS                       { UTICK_NS }
#else
  /** Peripheral UTICK base address */
  #define UTICK_BASE                               (0x4000F000u)
  /** Peripheral UTICK base pointer */
  #define UTICK                                    ((UTICK_Type *)UTICK_BASE)
  /** Array initializer of UTICK peripheral base addresses */
  #define UTICK_BASE_ADDRS                         { UTICK_BASE }
  /** Array initializer of UTICK peripheral base pointers */
  #define UTICK_BASE_PTRS                          { UTICK }
#endif
/** Interrupt vectors for the UTICK peripheral type */
#define UTICK_IRQS                               { UTICK_IRQn }

/* WLAPU - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral WLAPU base address */
  #define WLAPU_BASE                               (0x51258000u)
  /** Peripheral WLAPU base address */
  #define WLAPU_BASE_NS                            (0x41258000u)
  /** Peripheral WLAPU base pointer */
  #define WLAPU                                    ((WLAPU_Type *)WLAPU_BASE)
  /** Peripheral WLAPU base pointer */
  #define WLAPU_NS                                 ((WLAPU_Type *)WLAPU_BASE_NS)
  /** Array initializer of WLAPU peripheral base addresses */
  #define WLAPU_BASE_ADDRS                         { WLAPU_BASE }
  /** Array initializer of WLAPU peripheral base pointers */
  #define WLAPU_BASE_PTRS                          { WLAPU }
  /** Array initializer of WLAPU peripheral base addresses */
  #define WLAPU_BASE_ADDRS_NS                      { WLAPU_BASE_NS }
  /** Array initializer of WLAPU peripheral base pointers */
  #define WLAPU_BASE_PTRS_NS                       { WLAPU_NS }
#else
  /** Peripheral WLAPU base address */
  #define WLAPU_BASE                               (0x41258000u)
  /** Peripheral WLAPU base pointer */
  #define WLAPU                                    ((WLAPU_Type *)WLAPU_BASE)
  /** Array initializer of WLAPU peripheral base addresses */
  #define WLAPU_BASE_ADDRS                         { WLAPU_BASE }
  /** Array initializer of WLAPU peripheral base pointers */
  #define WLAPU_BASE_PTRS                          { WLAPU }
#endif

/* WWDT - Peripheral instance base addresses */
#if (defined(__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE & 0x2))
  /** Peripheral WWDT0 base address */
  #define WWDT0_BASE                               (0x5000E000u)
  /** Peripheral WWDT0 base address */
  #define WWDT0_BASE_NS                            (0x4000E000u)
  /** Peripheral WWDT0 base pointer */
  #define WWDT0                                    ((WWDT_Type *)WWDT0_BASE)
  /** Peripheral WWDT0 base pointer */
  #define WWDT0_NS                                 ((WWDT_Type *)WWDT0_BASE_NS)
  /** Array initializer of WWDT peripheral base addresses */
  #define WWDT_BASE_ADDRS                          { WWDT0_BASE }
  /** Array initializer of WWDT peripheral base pointers */
  #define WWDT_BASE_PTRS                           { WWDT0 }
  /** Array initializer of WWDT peripheral base addresses */
  #define WWDT_BASE_ADDRS_NS                       { WWDT0_BASE_NS }
  /** Array initializer of WWDT peripheral base pointers */
  #define WWDT_BASE_PTRS_NS                        { WWDT0_NS }
#else
  /** Peripheral WWDT0 base address */
  #define WWDT0_BASE                               (0x4000E000u)
  /** Peripheral WWDT0 base pointer */
  #define WWDT0                                    ((WWDT_Type *)WWDT0_BASE)
  /** Array initializer of WWDT peripheral base addresses */
  #define WWDT_BASE_ADDRS                          { WWDT0_BASE }
  /** Array initializer of WWDT peripheral base pointers */
  #define WWDT_BASE_PTRS                           { WWDT0 }
#endif
/** Interrupt vectors for the WWDT peripheral type */
#define WWDT_IRQS                                { WDT0_IRQn }

/* ----------------------------------------------------------------------------
   -- Macros for use with bit field definitions (xxx_SHIFT, xxx_MASK).
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Bit_Field_Generic_Macros Macros for use with bit field definitions (xxx_SHIFT, xxx_MASK).
 * @{
 */

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang system_header
  #endif
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma system_include
#endif

/**
 * @brief Mask and left-shift a bit field value for use in a register bit range.
 * @param field Name of the register bit field.
 * @param value Value of the bit field.
 * @return Masked and shifted value.
 */
#define NXP_VAL2FLD(field, value)    (((value) << (field ## _SHIFT)) & (field ## _MASK))
/**
 * @brief Mask and right-shift a register value to extract a bit field value.
 * @param field Name of the register bit field.
 * @param value Value of the register.
 * @return Masked and shifted bit field value.
 */
#define NXP_FLD2VAL(field, value)    (((value) & (field ## _MASK)) >> (field ## _SHIFT))

/*!
 * @}
 */ /* end of group Bit_Field_Generic_Macros */


/* ----------------------------------------------------------------------------
   -- SDK Compatibility
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDK_Compatibility_Symbols SDK Compatibility
 * @{
 */

/** Used for get the base address of ROM API */
#define FSL_ROM_API_BASE_ADDR 0x1303f000U
/** Used for get the address of OTP INIT API in ROM */
#define FSL_ROM_OTP_INIT_ADDR 0x13009FF9U
/** Used for get the address of OTP DEINIT API in ROM */
#define FSL_ROM_OTP_DEINIT_ADDR 0x1300a047U
/** Used for get the address of OTP FUSE READ API in ROM */
#define FSL_ROM_OTP_FUSE_READ_ADDR 0x1300a057U

/*! @brief IMU message link between current CPU and remote peer CPU. */
typedef enum
{
    kIMU_LinkCpu1Cpu3 = 0, /*! Message link between CPU1 and CPU3. */
    kIMU_LinkCpu2Cpu3,     /*! Message link between CPU2 and CPU3. */
    kIMU_LinkMax           /*! Message link count used for boundary check. */
} imu_link_t;

/*! @brief IMU base register for current CPU. */
#define IMU_CUR_CPU_BASE(link) (((link) == kIMU_LinkCpu1Cpu3) ? (&(WLCTRL->CIU1_IMU_CPU3_WR_MSG_TO_CPU1)) : (&(BLECTRL->CIU2_IMU_CPU3_WR_MSG_TO_CPU2)))
/*! @brief IMU base register for peer CPU. */
#define IMU_PEER_CPU_BASE(link) (((link) == kIMU_LinkCpu1Cpu3) ? (&(WLCTRL->CIU1_IMU_CPU1_WR_MSG_TO_CPU3)) : (&(BLECTRL->CIU2_IMU_CPU2_WR_MSG_TO_CPU3)))

/*! @brief IMU CPU index for current CPU. */
#define IMU_CPU_INDEX (3U)

/*!
 * @}
 */ /* end of group SDK_Compatibility_Symbols */


#endif  /* RW612_COMMON_H_ */

