/**
  ******************************************************************************
  * @file    stm32h5xx_hal_flash_ex.h
  * @author  MCD Application Team
  * @brief   Header file of FLASH HAL module.
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
#ifndef STM32H5xx_HAL_FLASH_EX_H
#define STM32H5xx_HAL_FLASH_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal_def.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASHEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Types FLASHEx Exported Types
  * @{
  */

/**
  * @brief  FLASH Erase structure definition
  */
typedef struct
{
  uint32_t TypeErase;   /*!< Mass erase or sector Erase.
                             This parameter can be a value of @ref FLASH_Type_Erase */

  uint32_t Banks;       /*!< Select bank to erase.
                             This parameter can be a value of @ref FLASH_Banks
                             (FLASH_BANK_BOTH should be used only for mass erase) */

  uint32_t Sector;      /*!< Initial FLASH sector to erase when Mass erase is disabled
                             This parameter can be a value of @ref FLASH_Sectors */

  uint32_t NbSectors;   /*!< Number of sectors to be erased.
                             This parameter can be a value between 1 and (max number of sectors in the bank -
                             value of initial sector)*/
} FLASH_EraseInitTypeDef;


/**
  * @brief  FLASH Option Bytes Program structure definition
  */
typedef struct
{
  uint32_t OptionType;     /*!< Option byte to be configured.
                                This parameter can be a value of @ref FLASH_Option_Type */

  uint32_t ProductState;   /*!< Set the product state.
                                This parameter can be a value of @ref FLASH_OB_Product_State */

  uint32_t USERType;       /*!< Select the User Option Byte(s) to be configured (used for OPTIONBYTE_USER).
                                This parameter can be a combination of @ref FLASH_OB_USER_Type */

  uint32_t USERConfig;     /*!< Value of the User Option Byte (used for OPTIONBYTE_USER).
                                This parameter can be a combination of @ref FLASH_OB_USER_BOR_LEVEL,
                                @ref FLASH_OB_USER_BORH_EN, @ref FLASH_OB_USER_IWDG_SW,
                                @ref FLASH_OB_USER_WWDG_SW, @ref FLASH_OB_USER_nRST_STOP,
                                @ref FLASH_OB_USER_nRST_STANDBY, @ref FLASH_OB_USER_IO_VDD_HSLV,
                                @ref FLASH_OB_USER_IO_VDDIO2_HSLV, @ref FLASH_OB_USER_IWDG_STOP,
                                @ref FLASH_OB_USER_IWDG_STANDBY, @ref FLASH_OB_USER_BOOT_UBE,
                                @ref FLASH_OB_USER_SWAP_BANK */

  uint32_t USERConfig2;    /*!< Value of the User Option Byte (used for OPTIONBYTE_USER).
                                This parameter can be a combination of @ref FLASH_OB_USER_SRAM1_3_RST,
                                @ref FLASH_OB_USER_SRAM2_RST, @ref FLASH_OB_USER_BKPRAM_ECC,
                                @ref FLASH_OB_USER_SRAM3_ECC, @ref FLASH_OB_USER_SRAM2_ECC,
                                @ref FLASH_OB_USER_SRAM1_RST, @ref FLASH_OB_USER_SRAM1_ECC,
                                @ref FLASH_OB_USER_USBPD_DIS, @ref FLASH_OB_USER_TZEN */

  uint32_t Banks;          /*!< Select banks for WRP , HDP and secure area configuration.
                                This parameter must be a value of @ref FLASH_Banks */

  uint32_t WRPState;       /*!< Write protection activation or deactivation.
                                This parameter can be a value of @ref FLASH_WRP_State */

  uint64_t WRPSector;      /*!< Specifies the sector(s) to be write protected.
                                The value of this parameter depend on device used within the same series */

  uint32_t BootConfig;     /*!< Specifies if the Boot Address to be configured: secure or non-secure.
                                This parameter must be a value of @ref FLASH_OB_BOOT_CONFIG enumeration */

  uint32_t BootAddr;       /*!< Boot address (used for OPTIONBYTE_BOOTADDR).
                                This parameter must be a value between 0x0 and 0xFFFFFF00 */

  uint32_t BootLock;       /*!< Configuration of the boot lock (used for OPTIONBYTE_BOOT_LOCK).
                                This parameter must be a value of @ref FLASH_OB_BOOT_LOCK */

  uint32_t OTPBlockLock;   /*!< Specifies the OTP block(s) to be locked.
                                This parameter must be a value of @ref FLASH_OTP_Blocks */

  uint32_t HDPStartSector; /*!< Start sector of HDP area (used for OPTIONBYTE_HDP).
                                This parameter must be a value between 0 and (max number of sectors in the bank - 1) */

  uint32_t HDPEndSector;   /*!< End sector of HDP area (used for OPTIONBYTE_HDP).
                                This parameter must be a value between 0 and (max number of sectors in the bank - 1) */

  uint32_t EDATASize;      /*!< Specifies the number of Flash high-cycle sectors.
                                This parameter must be a value between 0 and 8 (sectors) */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  uint32_t WMSecStartSector; /*!< Start sector of secure area (used for OPTIONBYTE_WMSEC).
                                  This parameter must be a value between 0 and (max number of sectors in the bank - 1)*/
  uint32_t WMSecEndSector;   /*!< End sector of secure area (used for OPTIONBYTE_WMSEC).
                                  This parameter must be a value between 0 and (max number of sectors in the bank - 1)*/
#endif /* __ARM_FEATURE_CMSE */

} FLASH_OBProgramInitTypeDef;

/**
  * @brief  FLASHEx Block-based attributes structure definition
  */
typedef struct
{
  uint32_t Bank;                                        /*!< Selection of the associated bank of Block-based Area.
                                                             This parameter must be a value of @ref FLASH_Banks */
  uint32_t BBAttributesType;                            /*!< Block-Based Attributes type.
                                                             This parameter must be a value of @ref FLASH_BB_Attributes
                                                             */
  uint32_t BBAttributes_array[FLASH_BLOCKBASED_NB_REG]; /*!< Each bit specifies the block-based attribute configuration
                                                             of a sector:
                                                             0 means sector non-protected, 1 means sector protected.
                                                             Protection (secure or privilege) depends on
                                                             BBAttributesType value */
} FLASH_BBAttributesTypeDef;

/**
  * @brief  FLASHEx Operation structure definition
  */
typedef struct
{
  uint32_t OperationType;    /*!< Flash operation Type.
                                  This parameter must be a value of @ref FLASH_Operation_Type */
  uint32_t FlashArea;        /*!< Flash operation memory area.
                                  This parameter must be a value of @ref FLASH_Operation_Area */
  uint32_t Address;          /*!< Flash operation Address offset.
                                  This parameter is given by bank, and must be a value between 0x0 and 0xFFFF0 */
} FLASH_OperationTypeDef;

/**
  * @brief  FLASH HDP Extension structure definition
  */
typedef struct
{
  uint32_t Banks;       /*!< Selection of the associated bank of HDP Area.
                             This parameter must be a value of @ref FLASH_Banks */
  uint32_t NbSectors;   /*!< Number of sectors to be HDP extended.
                             This parameter can be a value between 1 and max number of sectors in the bank */
} FLASH_HDPExtensionTypeDef;

/**
  * @brief  ECC Info Structure definition
  */
typedef struct
{
  uint32_t               Area;             /*!< Area from which an ECC was detected.
                                                This parameter can be a value of @ref FLASH_ECC_Area  */

  uint32_t               Address;          /*!< ECC error address */

  uint32_t               Data;             /*!< ECC failing data */
} FLASH_EccInfoTypeDef;

/**
  * @}
  */
/* Exported constants --------------------------------------------------------*/

/** @defgroup FLASHEx_Exported_Constants FLASHEx Exported Constants
  * @{
  */

/** @defgroup FLASH_Type_Erase FLASH Type Erase
  * @{
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define FLASH_TYPEERASE_SECTORS      FLASH_CR_SER                                           /*!< Secure flash sectors
                                                                                                 erase activation */
#define FLASH_TYPEERASE_SECTORS_NS   (FLASH_CR_SER | FLASH_NON_SECURE_MASK)                 /*!< Non-secure flash
                                                                                             sectors erase activation */
#define FLASH_TYPEERASE_MASSERASE    (FLASH_CR_BER | FLASH_CR_MER)                          /*!< Secure flash mass erase
                                                                                                 activation */
#define FLASH_TYPEERASE_MASSERASE_NS (FLASH_CR_BER | FLASH_CR_MER | FLASH_NON_SECURE_MASK)  /*!< Non-secure flash mass
                                                                                                 erase activation */
#if defined (FLASH_SR_OBKERR)
#define FLASH_TYPEERASE_OBK_ALT       FLASH_OBKCFGR_ALT_SECT_ERASE                           /*!< Flash OBK erase
                                                                                                  activation */
#endif /* FLASH_SR_OBKERR */
#else
#define FLASH_TYPEERASE_SECTORS      FLASH_CR_SER                                           /*!< Flash sectors erase
                                                                                                 activation */
#define FLASH_TYPEERASE_MASSERASE    (FLASH_CR_BER | FLASH_CR_MER)                          /*!< Flash mass erase
                                                                                                 activation */
#if defined (FLASH_SR_OBKERR)
#define FLASH_TYPEERASE_OBK_ALT      (FLASH_OBKCFGR_ALT_SECT_ERASE | FLASH_NON_SECURE_MASK) /*!< Flash OBK erase
                                                                                                  activation */
#endif /* FLASH_SR_OBKERR */
#endif /* __ARM_FEATURE_CMSE */

/** @defgroup FLASH_ECC_Area FLASH ECC Area
  * @brief    FLASH ECC Area
  * @{
  */
#define FLASH_ECC_AREA_USER_BANK1         0x00000000U               /*!< FLASH bank 1 area                */
#define FLASH_ECC_AREA_USER_BANK2         FLASH_ECCR_BK_ECC         /*!< FLASH bank 2 area                */
#define FLASH_ECC_AREA_SYSTEM             FLASH_ECCR_SYSF_ECC       /*!< System FLASH area                */
#if defined (FLASH_SR_OBKERR)
#define FLASH_ECC_AREA_OBK                FLASH_ECCR_OBK_ECC        /*!< FLASH OBK area                   */
#endif /* FLASH_SR_OBKERR */
#define FLASH_ECC_AREA_OTP                FLASH_ECCR_OTP_ECC        /*!< FLASH OTP area                   */
#if defined (FLASH_EDATAR_EDATA_EN)
#define FLASH_ECC_AREA_EDATA_BANK1        FLASH_ECCR_DATA_ECC       /*!< FLASH high-cycle data area       */
#define FLASH_ECC_AREA_EDATA_BANK2        (FLASH_ECCR_DATA_ECC | FLASH_ECCR_BK_ECC)    /*!< FLASH high-cycle data area       */
#endif /* FLASH_EDATAR_EDATA_EN */
/**
  * @}
  */

/**
  * @}
  */

/** @defgroup FLASH_Option_Type FLASH Option Type
  * @{
  */
#define OPTIONBYTE_WRP           0x0001U  /*!< WRP option byte configuration  */
#define OPTIONBYTE_PROD_STATE    0x0002U  /*!< RDP option byte configuration  */
#define OPTIONBYTE_USER          0x0004U  /*!< USER option byte configuration */
#define OPTIONBYTE_BOOTADDR      0x0008U  /*!< BOOT address option byte configuration */
#define OPTIONBYTE_BOOT_LOCK     0x0010U  /*!< Boot lock option byte configuration */
#define OPTIONBYTE_OTP_LOCK      0x0020U  /*!< OTP Lock option byte configuration */
#define OPTIONBYTE_HDP           0x0040U  /*!< Hide Protection area option byte configuration */
#if defined (FLASH_EDATAR_EDATA_EN)
#define OPTIONBYTE_EDATA         0x0080U  /*!< Flash high-cycle data area option byte configuration */
#endif /* FLASH_EDATAR_EDATA_EN */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define OPTIONBYTE_WMSEC         0x0200U   /*!< Watermark-based secure area option byte configuration */
#endif /* __ARM_FEATURE_CMSE */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define OPTIONBYTE_ALL           (OPTIONBYTE_WRP         | OPTIONBYTE_PROD_STATE | OPTIONBYTE_USER     |\
                                  OPTIONBYTE_BOOTADDR    | OPTIONBYTE_BOOT_LOCK  | OPTIONBYTE_OTP_LOCK |\
                                  OPTIONBYTE_HDP         | OPTIONBYTE_EDATA      | OPTIONBYTE_WMSEC) /*!< All option
byte configuration */
#else
#if defined (FLASH_EDATAR_EDATA_EN)
#define OPTIONBYTE_ALL           (OPTIONBYTE_WRP         | OPTIONBYTE_PROD_STATE | OPTIONBYTE_USER |\
                                  OPTIONBYTE_BOOTADDR    | OPTIONBYTE_BOOT_LOCK  | OPTIONBYTE_OTP_LOCK |\
                                  OPTIONBYTE_HDP         | OPTIONBYTE_EDATA) /*!< All option byte configuration */
#else
#define OPTIONBYTE_ALL           (OPTIONBYTE_WRP         | OPTIONBYTE_PROD_STATE | OPTIONBYTE_USER |\
                                  OPTIONBYTE_BOOTADDR    | OPTIONBYTE_BOOT_LOCK  | OPTIONBYTE_OTP_LOCK |\
                                  OPTIONBYTE_HDP) /*!< All option byte configuration */
#endif /* FLASH_EDATAR_EDATA_EN */
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_Type  FLASH OB USER Type
  * @{
  */
#define OB_USER_BOR_LEV           0x00000001U     /*!< BOR reset Level */
#define OB_USER_BORH_EN           0x00000002U     /*!< BOR high enable status */
#define OB_USER_IWDG_SW           0x00000004U     /*!< Independent watchdog selection */
#define OB_USER_WWDG_SW           0x00000008U     /*!< Window watchdog selection */
#define OB_USER_NRST_STOP         0x00000010U     /*!< Reset generated when entering the stop mode */
#define OB_USER_NRST_STDBY        0x00000020U     /*!< Reset generated when entering the standby mode */
#define OB_USER_IO_VDD_HSLV       0x00000040U     /*!< High speed IO at low voltage configuration bit */
#define OB_USER_IO_VDDIO2_HSLV    0x00000080U     /*!< High speed IO2 at low voltage configuration bit */
#define OB_USER_IWDG_STOP         0x00000100U     /*!< Independent watchdog counter freeze in stop mode */
#define OB_USER_IWDG_STDBY        0x00000200U     /*!< Independent watchdog counter freeze in standby mode */
#if defined (FLASH_OPTSR_BOOT_UBE)
#define OB_USER_BOOT_UBE          0x00000400U     /*!< Unique Boot entry */
#endif /* FLASH_OPTSR_BOOT_UBE */
#define OB_USER_SWAP_BANK         0x00000800U     /*!< Swap banks */

#if defined (FLASH_OPTSR2_SRAM1_3_RST)
#define OB_USER_SRAM1_3_RST       0x00001000U     /*!< SRAM1 and SRAM3 erase upon system reset */
#elif defined (FLASH_OPTSR2_SRAM1_3_4_5_RST)
#define OB_USER_SRAM1_3_4_5_RST   0x00001000U     /*!< SRAM1, SRAM3, SRAM4 and SRAM5 erase upon system reset */
#endif /* FLASH_OPTSR2_SRAM1_3_RST */
#if defined (FLASH_OPTSR2_SRAM1_RST)
#define OB_USER_SRAM1_RST         0x00001000U     /*!< SRAM1 Erase when system reset */
#endif /* FLASH_OPTSR2_SRAM1_RST */
#define OB_USER_SRAM2_RST         0x00002000U     /*!< SRAM2 Erase when system reset */
#define OB_USER_BKPRAM_ECC        0x00004000U     /*!< Backup RAM ECC detection and correction enable */
#define OB_USER_SRAM3_ECC         0x00008000U     /*!< SRAM3 ECC detection and correction enable */
#define OB_USER_SRAM2_ECC         0x00010000U     /*!< SRAM2 ECC detection and correction enable */
#define OB_USER_SRAM1_ECC         0x00020000U     /*!< SRAM1 ECC detection and correction enable */
#if defined (FLASH_OPTSR2_USBPD_DIS)
#define OB_USER_USBPD_DIS         0x00040000U     /*!< USB power delivery configuration enable */
#endif /*FLASH_OPTSR2_USBPD_DIS*/
#if defined (FLASH_OPTSR2_TZEN)
#define OB_USER_TZEN              0x00080000U     /*!< Global TrustZone security enable */
#endif /* FLASH_OPTSR2_TZEN */
#if defined (FLASH_OPTSR2_HUK_PUF)
#define OB_USER_HUK_PUF           0x00100000U     /*!< HUK source sent to the SAES is RHUK/PUF */
#endif /* FLASH_OPTSR2_HUK_PUF */

#if defined (FLASH_OPTSR2_SRAM1_3_RST) && defined (FLASH_OPTSR_BOOT_UBE) && defined (FLASH_OPTSR2_USBPD_DIS)
#define OB_USER_ALL (OB_USER_BOR_LEV        | OB_USER_BORH_EN        | OB_USER_IWDG_SW     |\
                     OB_USER_WWDG_SW        | OB_USER_NRST_STOP      | OB_USER_NRST_STDBY  |\
                     OB_USER_IO_VDD_HSLV    | OB_USER_IO_VDDIO2_HSLV | OB_USER_IWDG_STOP   |\
                     OB_USER_IWDG_STDBY     | OB_USER_BOOT_UBE       | OB_USER_SWAP_BANK   |\
                     OB_USER_SRAM1_3_RST    | OB_USER_SRAM2_RST      | OB_USER_BKPRAM_ECC  |\
                     OB_USER_SRAM3_ECC      | OB_USER_SRAM2_ECC      | OB_USER_USBPD_DIS   |\
                     OB_USER_TZEN)
#elif defined (FLASH_OPTSR2_SRAM1_3_4_5_RST) && defined (FLASH_OPTSR_BOOT_UBE) && defined (FLASH_OPTSR2_USBPD_DIS)
#define OB_USER_ALL (OB_USER_BOR_LEV         | OB_USER_BORH_EN        | OB_USER_IWDG_SW     |\
                     OB_USER_WWDG_SW         | OB_USER_NRST_STOP      | OB_USER_NRST_STDBY  |\
                     OB_USER_IO_VDD_HSLV     | OB_USER_IO_VDDIO2_HSLV | OB_USER_IWDG_STOP   |\
                     OB_USER_IWDG_STDBY      | OB_USER_BOOT_UBE       | OB_USER_SWAP_BANK   |\
                     OB_USER_SRAM1_3_4_5_RST | OB_USER_SRAM2_RST      | OB_USER_BKPRAM_ECC  |\
                     OB_USER_SRAM3_ECC       | OB_USER_SRAM2_ECC      | OB_USER_USBPD_DIS   |\
                     OB_USER_TZEN            | OB_USER_HUK_PUF)
#else
#define OB_USER_ALL (OB_USER_BOR_LEV        | OB_USER_BORH_EN        | OB_USER_IWDG_SW     |\
                     OB_USER_WWDG_SW        | OB_USER_NRST_STOP      | OB_USER_NRST_STDBY  |\
                     OB_USER_IO_VDD_HSLV    | OB_USER_IO_VDDIO2_HSLV | OB_USER_IWDG_STOP   |\
                     OB_USER_IWDG_STDBY     | OB_USER_SWAP_BANK      | OB_USER_SRAM1_RST   |\
                     OB_USER_SRAM2_RST      | OB_USER_BKPRAM_ECC     | OB_USER_SRAM3_ECC   |\
                     OB_USER_SRAM2_ECC      | OB_USER_SRAM1_ECC)
#endif /* FLASH_OPTSR2_SRAM1_3_RST && FLASH_OPTSR_BOOT_UBE */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BOR_LEVEL FLASH BOR Reset Level
  * @{
  */
#define OB_BOR_LEVEL_1        0U                                               /*!< Reset level 1 threshold */
#define OB_BOR_LEVEL_2        FLASH_OPTSR_BOR_LEV_0                            /*!< Reset level 2 threshold */
#define OB_BOR_LEVEL_3        FLASH_OPTSR_BOR_LEV_1                            /*!< Reset level 3 threshold */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BORH_EN FLASH BOR High Enable Status
  * @{
  */
#define OB_BORH_DISABLE        0x00000000U                /*!< BOR high status bit disabled */
#define OB_BORH_ENABLE         FLASH_OPTSR_BORH_EN        /*!< BOR high status bit enabled  */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_SW  FLASH Option Bytes User IWDG Type
  * @{
  */
#define OB_IWDG_HW            0x00000000U          /*!< Hardware independent watchdog */
#define OB_IWDG_SW            FLASH_OPTSR_IWDG_SW  /*!< Software independent watchdog */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_WWDG_SW FLASH Option Bytes User WWDG Type
  * @{
  */
#define OB_WWDG_HW                0x00000000U              /*!< Hardware window watchdog */
#define OB_WWDG_SW                FLASH_OPTSR_WWDG_SW       /*!< Software window watchdog */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_STOP FLASH Option Bytes nRST_STOP
  * @{
  */
#define OB_STOP_RST          0x00000000U           /*!< Reset generated when entering in stop mode    */
#define OB_STOP_NORST        FLASH_OPTSR_NRST_STOP /*!< No reset generated when entering in stop mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_nRST_STANDBY FLASH Option Bytes nRST_STDBY
  * @{
  */
#define OB_STANDBY_RST         0x00000000U            /*!< Reset generated when entering in standby mode    */
#define OB_STANDBY_NORST       FLASH_OPTSR_NRST_STDBY /*!< No reset generated when entering in standby mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_Product_State FLASH Product State
  * @{
  */
#define OB_PROD_STATE_OPEN                   (0xEDUL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_PROVISIONING           (0x17UL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_IROT_PROVISIONED       (0x2EUL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_TZ_CLOSED              (0xC6UL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_CLOSED                 (0x72UL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_LOCKED                 (0x5CUL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_REGRESSION             (0x9AUL << FLASH_OPTSR_PRODUCT_STATE_Pos)
#define OB_PROD_STATE_NS_REGRESSION          (0xA3UL << FLASH_OPTSR_PRODUCT_STATE_Pos)
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IO_VDD_HSLV FLASH Option Bytes VDD IO HSLV
  * @{
  */
#define OB_IO_VDD_HSLV_DISABLE      0x00000000U             /*!< High-speed IO at low VDD voltage feature disabled */
#define OB_IO_VDD_HSLV_ENABLE       FLASH_OPTSR_IO_VDD_HSLV /*!< High-speed IO at low VDD voltage feature enabled */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IO_VDDIO2_HSLV FLASH Option Bytes VDDIO2 IO HSLV
  * @{
  */
#define OB_IO_VDDIO2_HSLV_DISABLE   0x00000000U                /*!< High-speed IO at low VDDIO2 voltage feature
                                                                    disabled */
#define OB_IO_VDDIO2_HSLV_ENABLE    FLASH_OPTSR_IO_VDDIO2_HSLV /*!< High-speed IO at low VDDIO2 voltage feature
                                                                    enabled */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_STOP FLASH IWDG Counter Freeze in STOP
  * @{
  */
#define OB_IWDG_STOP_FREEZE  0x00000000U              /*!< IWDG counter frozen in STOP mode */
#define OB_IWDG_STOP_ACTIVE  FLASH_OPTSR_IWDG_STOP    /*!< IWDG counter active in STOP mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_IWDG_STANDBY FLASH IWDG Counter Freeze in STANDBY
  * @{
  */
#define OB_IWDG_STDBY_FREEZE 0x00000000U               /*!< IWDG counter frozen in STANDBY mode */
#define OB_IWDG_STDBY_ACTIVE FLASH_OPTSR_IWDG_STDBY    /*!< IWDG counter active in STANDBY mode */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BOOT_UBE FLASH OB Boot UBE
  * @{
  */
#if defined (FLASH_OPTSR_BOOT_UBE)
#define OB_UBE_OEM_IROT   (0xB4UL << FLASH_OPTSR_BOOT_UBE_Pos) /*!< OEM-iRoT (user flash) selected  */
#define OB_UBE_ST_IROT    (0xC3UL << FLASH_OPTSR_BOOT_UBE_Pos) /*!< ST-iRoT (system flash) selected */
#endif /* FLASH_OPTSR_BOOT_UBE */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SWAP_BANK  FLASH OB SWAP BANK
  * @{
  */
#define OB_SWAP_BANK_DISABLE   0x00000000U           /*!< Bank swap disabled */
#define OB_SWAP_BANK_ENABLE    FLASH_OPTSR_SWAP_BANK /*!< Bank swap enabled */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM1_3_RST FLASH Option Bytes SRAM1_3 Erase On Reset
  * @{
  */
#if defined (FLASH_OPTSR2_SRAM1_3_RST)
#define OB_SRAM1_3_RST_ERASE      0x00000000U               /*!< SRAM1 and SRAM3 erased when a system reset occurs    */
#define OB_SRAM1_3_RST_NOT_ERASE  FLASH_OPTSR2_SRAM1_3_RST  /*!< SRAM1 and SRAM3 are not erased when a system reset
                                                                 occurs */
#endif /* FLASH_OPTSR2_SRAM1_3_RST */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM1_3_4_5_RST FLASH Option Bytes SRAM1_3 Erase On Reset
  * @{
  */
#if defined (FLASH_OPTSR2_SRAM1_3_4_5_RST)
#define OB_SRAM1_3_4_5_RST_ERASE      0x00000000U                   /*!< SRAM1, SRAM3, SRAM4, and SRAM5 erased when a system reset occurs    */
#define OB_SRAM1_3_4_5_RST_NOT_ERASE  FLASH_OPTSR2_SRAM1_3_4_5_RST  /*!< SRAM1, SRAM3, SRAM4, and SRAM5 when a system reset occurs */
#endif /* FLASH_OPTSR2_SRAM1_3_4_5_RST */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM1_RST FLASH Option Bytes SRAM1 Erase On Reset
  * @{
  */
#if defined (FLASH_OPTSR2_SRAM1_RST)
#define OB_SRAM1_RST_ERASE        0x00000000U              /*!< SRAM1 erased when a system reset occurs        */
#define OB_SRAM1_RST_NOT_ERASE    FLASH_OPTSR2_SRAM1_RST   /*!< SRAM1 is not erased when a system reset occurs */
#endif /* FLASH_OPTSR2_SRAM1_RST */
/**
  * @}
  */


/** @defgroup FLASH_OB_USER_SRAM2_RST FLASH Option Bytes SRAM2 Erase On Reset
  * @{
  */
#define OB_SRAM2_RST_ERASE        0x00000000U              /*!< SRAM2 erased when a system reset occurs        */
#define OB_SRAM2_RST_NOT_ERASE    FLASH_OPTSR2_SRAM2_RST   /*!< SRAM2 is not erased when a system reset occurs */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_BKPRAM_ECC FLASH Option Bytes User BKPRAM ECC check
  * @{
  */
#define OB_BKPRAM_ECC_ENABLE      0x00000000U             /*!< BKPRAM ECC check enable  */
#define OB_BKPRAM_ECC_DISABLE     FLASH_OPTSR2_BKPRAM_ECC /*!< BKPRAM ECC check disable */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM3_ECC FLASH Option Bytes User SRAM3 ECC check
  * @{
  */
#if defined (FLASH_OPTSR2_SRAM3_ECC)
#define OB_SRAM3_ECC_ENABLE       0x00000000U            /*!< SRAM3 ECC check enable  */
#define OB_SRAM3_ECC_DISABLE      FLASH_OPTSR2_SRAM3_ECC /*!< SRAM3 ECC check disable */
#endif /* FLASH_OPTSR2_SRAM3_ECC */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM2_ECC FLASH Option Bytes User SRAM2 ECC check
  * @{
  */
#define OB_SRAM2_ECC_ENABLE    0x00000000U                /*!< SRAM2 ECC check enable */
#define OB_SRAM2_ECC_DISABLE   FLASH_OPTSR2_SRAM2_ECC     /*!< SRAM2 ECC check disable */
/**
  * @}
  */

/** @defgroup FLASH_OB_HUK_PUF FLASH Option Bytes User PUF STATE check
  * @{
  */
#if defined (FLASH_SR_PUF_STATE)
#define FLASH_FLAG_PUF_NOT_READY    0x00000000U              /*!< PUF is not ready. Retry PUF_LAUNCH */
#define FLASH_FLAG_PUF_READY        FLASH_SR_PUF_STATE       /*!< PUF is ready for use */
#endif /* FLASH_SR_PUF_STATE */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_SRAM1_ECC FLASH Option Bytes User SRAM1 ECC check
  * @{
  */
#if defined (FLASH_OPTSR2_SRAM1_ECC)
#define OB_SRAM1_ECC_ENABLE       0x00000000U            /*!< SRAM1 ECC check enable  */
#define OB_SRAM1_ECC_DISABLE      FLASH_OPTSR2_SRAM1_ECC /*!< SRAM1 ECC check disable */
#endif /* FLASH_OPTSR2_SRAM1_ECC */
/**
  * @}
  */

/** @defgroup FLASH_OB_USER_USBPD_DIS FLASH Option Bytes USB power delivery configuration
  * @{
  */
#if defined (FLASH_OPTSR2_USBPD_DIS)
#define OB_USBPD_DIS_ENABLE       0x00000000U            /*!< USB power delivery check enable  */
#define OB_USBPD_DIS_DISABLE      FLASH_OPTSR2_USBPD_DIS /*!< USB power delivery check disable */
#endif /* FLASH_OPTSR2_USBPD_DIS */
/**
  * @}
  */
/** @defgroup FLASH_OB_USER_TZEN FLASH Option Bytes Global TrustZone
  * @{
  */
#if defined (FLASH_OPTSR2_TZEN)
#define OB_TZEN_DISABLE   (0xC3UL << FLASH_OPTSR2_TZEN_Pos) /*!< Global TrustZone security disabled */
#define OB_TZEN_ENABLE    (0xB4UL << FLASH_OPTSR2_TZEN_Pos) /*!< Global TrustZone security enabled */
#endif /* FLASH_OPTSR2_TZEN */
/**
  * @}
  */

/** @defgroup FLASH_OPTSR2_HUK_PUF FLASH Option Bytes Global TrustZone
  * @{
  */
#if defined (FLASH_OPTSR2_HUK_PUF)
#define OB_UNIQUE_KEY_HUK    0x00000000U                 /*!< HUK source is RHUK */
#define OB_UNIQUE_KEY_PUF    FLASH_OPTSR2_HUK_PUF        /*!< HUK source is PUF */
#endif /* FLASH_OPTSR2_TZEN */
/**
  * @}
  */

/** @defgroup FLASH_Banks FLASH Banks
  * @{
  */
#define FLASH_BANK_1             0x00000001U                   /*!< Bank 1   */
#define FLASH_BANK_2             0x00000002U                   /*!< Bank 2   */
#define FLASH_BANK_BOTH          (FLASH_BANK_1 | FLASH_BANK_2) /*!< Bank1 and Bank2 */
/**
  * @}
  */

/** @defgroup FLASH_OB_Write_Protection_Sectors FLASH Option Bytes Write Protection Sectors
  * @{
  */
#if (FLASH_SECTOR_NB == 8)
#define OB_WRP_SECTOR_0          0x0000000000000001U /*!< Write protection of Sector0                */
#define OB_WRP_SECTOR_1          0x0000000000000002U /*!< Write protection of Sector1                */
#define OB_WRP_SECTOR_2          0x0000000000000004U /*!< Write protection of Sector2                */
#define OB_WRP_SECTOR_3          0x0000000000000008U /*!< Write protection of Sector3                */
#define OB_WRP_SECTOR_4          0x0000000000000010U /*!< Write protection of Sector4                */
#define OB_WRP_SECTOR_5          0x0000000000000020U /*!< Write protection of Sector5                */
#define OB_WRP_SECTOR_6          0x0000000000000040U /*!< Write protection of Sector6                */
#define OB_WRP_SECTOR_7          0x0000000000000080U /*!< Write protection of Sector7                */
#define OB_WRP_SECTOR_ALL        0x00000000000000FFU /*!< Write protection of all Sectors            */
#else
#define OB_WRP_SECTOR_0TO3       0x0000000000000001U /*!< Write protection of Sector0  to Sector3    */
#define OB_WRP_SECTOR_4TO7       0x0000000000000002U /*!< Write protection of Sector4  to Sector7    */
#define OB_WRP_SECTOR_8TO11      0x0000000000000004U /*!< Write protection of Sector8  to Sector11   */
#define OB_WRP_SECTOR_12TO15     0x0000000000000008U /*!< Write protection of Sector12 to Sector15   */
#define OB_WRP_SECTOR_16TO19     0x0000000000000010U /*!< Write protection of Sector16 to Sector19   */
#define OB_WRP_SECTOR_20TO23     0x0000000000000020U /*!< Write protection of Sector20 to Sector23   */
#define OB_WRP_SECTOR_24TO27     0x0000000000000040U /*!< Write protection of Sector24 to Sector27   */
#define OB_WRP_SECTOR_28TO31     0x0000000000000080U /*!< Write protection of Sector28 to Sector31   */
#if (FLASH_SECTOR_NB == 64) || (FLASH_SECTOR_NB == 128) || (FLASH_SECTOR_NB == 256)
#define OB_WRP_SECTOR_32TO35     0x0000000000000100U /*!< Write protection of Sector32 to Sector35   */
#define OB_WRP_SECTOR_36TO39     0x0000000000000200U /*!< Write protection of Sector36 to Sector39   */
#define OB_WRP_SECTOR_40TO43     0x0000000000000400U /*!< Write protection of Sector40 to Sector43   */
#define OB_WRP_SECTOR_44TO47     0x0000000000000800U /*!< Write protection of Sector44 to Sector47   */
#define OB_WRP_SECTOR_48TO51     0x0000000000001000U /*!< Write protection of Sector48 to Sector51   */
#define OB_WRP_SECTOR_52TO55     0x0000000000002000U /*!< Write protection of Sector52 to Sector55   */
#define OB_WRP_SECTOR_56TO59     0x0000000000004000U /*!< Write protection of Sector56 to Sector59   */
#define OB_WRP_SECTOR_60TO63     0x0000000000008000U /*!< Write protection of Sector60 to Sector63   */
#endif /* (FLASH_SECTOR_NB == 64) || (FLASH_SECTOR_NB == 128) || (FLASH_SECTOR_NB == 256) */
#if (FLASH_SECTOR_NB == 128) || (FLASH_SECTOR_NB == 256)
#define OB_WRP_SECTOR_64TO67     0x0000000000010000U /*!< Write protection of Sector64 to Sector67   */
#define OB_WRP_SECTOR_68TO71     0x0000000000020000U /*!< Write protection of Sector68 to Sector71   */
#define OB_WRP_SECTOR_72TO75     0x0000000000040000U /*!< Write protection of Sector72 to Sector75   */
#define OB_WRP_SECTOR_76TO79     0x0000000000080000U /*!< Write protection of Sector76 to Sector79   */
#define OB_WRP_SECTOR_80TO83     0x0000000000100000U /*!< Write protection of Sector80 to Sector83   */
#define OB_WRP_SECTOR_84TO87     0x0000000000200000U /*!< Write protection of Sector84 to Sector87   */
#define OB_WRP_SECTOR_88TO91     0x0000000000400000U /*!< Write protection of Sector88 to Sector91   */
#define OB_WRP_SECTOR_92TO95     0x0000000000800000U /*!< Write protection of Sector92 to Sector95   */
#define OB_WRP_SECTOR_96TO99     0x0000000001000000U /*!< Write protection of Sector96  to Sector99  */
#define OB_WRP_SECTOR_100TO103   0x0000000002000000U /*!< Write protection of Sector100 to Sector103 */
#define OB_WRP_SECTOR_104TO107   0x0000000004000000U /*!< Write protection of Sector104 to Sector107 */
#define OB_WRP_SECTOR_108TO111   0x0000000008000000U /*!< Write protection of Sector108 to Sector111 */
#define OB_WRP_SECTOR_112TO115   0x0000000010000000U /*!< Write protection of Sector112 to Sector115 */
#define OB_WRP_SECTOR_116TO119   0x0000000020000000U /*!< Write protection of Sector116 to Sector119 */
#define OB_WRP_SECTOR_120TO123   0x0000000040000000U /*!< Write protection of Sector120 to Sector123 */
#define OB_WRP_SECTOR_124TO127   0x0000000080000000U /*!< Write protection of Sector124 to Sector127 */
#endif /* (FLASH_SECTOR_NB == 128) || (FLASH_SECTOR_NB == 256) */
#if (FLASH_SECTOR_NB == 256)
#define OB_WRP_SECTOR_128TO131   0x0000000100000000U /*!< Write protection of Sector128 to Sector131 */
#define OB_WRP_SECTOR_132TO135   0x0000000200000000U /*!< Write protection of Sector132 to Sector135 */
#define OB_WRP_SECTOR_136TO139   0x0000000400000000U /*!< Write protection of Sector136 to Sector139 */
#define OB_WRP_SECTOR_140TO143   0x0000000800000000U /*!< Write protection of Sector140 to Sector143 */
#define OB_WRP_SECTOR_144TO147   0x0000001000000000U /*!< Write protection of Sector144 to Sector147 */
#define OB_WRP_SECTOR_148TO151   0x0000002000000000U /*!< Write protection of Sector148 to Sector151 */
#define OB_WRP_SECTOR_152TO155   0x0000004000000000U /*!< Write protection of Sector152 to Sector155 */
#define OB_WRP_SECTOR_156TO159   0x0000008000000000U /*!< Write protection of Sector156 to Sector159 */
#define OB_WRP_SECTOR_160TO163   0x0000010000000000U /*!< Write protection of Sector160 to Sector163 */
#define OB_WRP_SECTOR_164TO167   0x0000020000000000U /*!< Write protection of Sector164 to Sector167 */
#define OB_WRP_SECTOR_168TO171   0x0000040000000000U /*!< Write protection of Sector168 to Sector171 */
#define OB_WRP_SECTOR_172TO175   0x0000080000000000U /*!< Write protection of Sector172 to Sector175 */
#define OB_WRP_SECTOR_176TO179   0x0000100000000000U /*!< Write protection of Sector176 to Sector179 */
#define OB_WRP_SECTOR_180TO183   0x0000200000000000U /*!< Write protection of Sector180 to Sector183 */
#define OB_WRP_SECTOR_184TO187   0x0000400000000000U /*!< Write protection of Sector184 to Sector187 */
#define OB_WRP_SECTOR_188TO191   0x0000800000000000U /*!< Write protection of Sector188 to Sector191 */
#define OB_WRP_SECTOR_192TO195   0x0001000000000000U /*!< Write protection of Sector192 to Sector195 */
#define OB_WRP_SECTOR_196TO199   0x0002000000000000U /*!< Write protection of Sector196 to Sector199 */
#define OB_WRP_SECTOR_200TO203   0x0004000000000000U /*!< Write protection of Sector200 to Sector203 */
#define OB_WRP_SECTOR_204TO207   0x0008000000000000U /*!< Write protection of Sector204 to Sector207 */
#define OB_WRP_SECTOR_208TO211   0x0010000000000000U /*!< Write protection of Sector208 to Sector211 */
#define OB_WRP_SECTOR_212TO215   0x0020000000000000U /*!< Write protection of Sector212 to Sector215 */
#define OB_WRP_SECTOR_216TO219   0x0040000000000000U /*!< Write protection of Sector216 to Sector219 */
#define OB_WRP_SECTOR_220TO223   0x0080000000000000U /*!< Write protection of Sector220 to Sector223 */
#define OB_WRP_SECTOR_224TO227   0x0100000000000000U /*!< Write protection of Sector224 to Sector227 */
#define OB_WRP_SECTOR_228TO231   0x0200000000000000U /*!< Write protection of Sector228 to Sector231 */
#define OB_WRP_SECTOR_232TO235   0x0400000000000000U /*!< Write protection of Sector232 to Sector235 */
#define OB_WRP_SECTOR_236TO239   0x0800000000000000U /*!< Write protection of Sector236 to Sector239 */
#define OB_WRP_SECTOR_240TO243   0x1000000000000000U /*!< Write protection of Sector240 to Sector243 */
#define OB_WRP_SECTOR_244TO247   0x2000000000000000U /*!< Write protection of Sector244 to Sector247 */
#define OB_WRP_SECTOR_248TO251   0x4000000000000000U /*!< Write protection of Sector248 to Sector251 */
#define OB_WRP_SECTOR_252TO255   0x8000000000000000U /*!< Write protection of Sector252 to Sector255 */
#endif /* (FLASH_SECTOR_NB == 256) */
#endif /* (FLASH_SECTOR_NB == 8) */
#if (FLASH_SECTOR_NB == 32)
#define OB_WRP_SECTOR_ALL        0x00000000000000FFU /*!< Write protection of all Sectors            */
#elif (FLASH_SECTOR_NB == 64)
#define OB_WRP_SECTOR_ALL        0x000000000000FFFFU /*!< Write protection of all Sectors            */
#elif (FLASH_SECTOR_NB == 128)
#define OB_WRP_SECTOR_ALL        0x00000000FFFFFFFFU /*!< Write protection of all Sectors            */
#elif (FLASH_SECTOR_NB == 256)
#define OB_WRP_SECTOR_ALL        0xFFFFFFFFFFFFFFFFU /*!< Write protection of all Sectors            */
#endif /* FLASH_SECTOR_NB == 32 */

/**
  * @}
  */

/** @defgroup FLASH_Programming_Delay FLASH Programming Delay
  * @{
  */
#define FLASH_PROGRAMMING_DELAY_0   0x00000000U            /*!< programming delay set for Flash running at 84 MHz or
                                                                below */
#define FLASH_PROGRAMMING_DELAY_1   FLASH_ACR_WRHIGHFREQ_0 /*!< programming delay set for Flash running between 84 MHz
                                                                and 168 MHz */
#define FLASH_PROGRAMMING_DELAY_2   FLASH_ACR_WRHIGHFREQ_1 /*!< programming delay set for Flash running between 168 MHz
                                                                and 250 MHz */
/**
  * @}
  */

/** @defgroup FLASH_OTP_Blocks FLASH OTP blocks
  * @{
  */
#define FLASH_OTP_BLOCK_0          0x00000001U /*!< OTP Block0     */
#define FLASH_OTP_BLOCK_1          0x00000002U /*!< OTP Block1     */
#define FLASH_OTP_BLOCK_2          0x00000004U /*!< OTP Block2     */
#define FLASH_OTP_BLOCK_3          0x00000008U /*!< OTP Block3     */
#define FLASH_OTP_BLOCK_4          0x00000010U /*!< OTP Block4     */
#define FLASH_OTP_BLOCK_5          0x00000020U /*!< OTP Block5     */
#define FLASH_OTP_BLOCK_6          0x00000040U /*!< OTP Block6     */
#define FLASH_OTP_BLOCK_7          0x00000080U /*!< OTP Block7     */
#define FLASH_OTP_BLOCK_8          0x00000100U /*!< OTP Block8     */
#define FLASH_OTP_BLOCK_9          0x00000200U /*!< OTP Block9     */
#define FLASH_OTP_BLOCK_10         0x00000400U /*!< OTP Block10    */
#define FLASH_OTP_BLOCK_11         0x00000800U /*!< OTP Block11    */
#define FLASH_OTP_BLOCK_12         0x00001000U /*!< OTP Block12    */
#define FLASH_OTP_BLOCK_13         0x00002000U /*!< OTP Block13    */
#define FLASH_OTP_BLOCK_14         0x00004000U /*!< OTP Block14    */
#define FLASH_OTP_BLOCK_15         0x00008000U /*!< OTP Block15    */
#define FLASH_OTP_BLOCK_16         0x00010000U /*!< OTP Block16    */
#define FLASH_OTP_BLOCK_17         0x00020000U /*!< OTP Block17    */
#define FLASH_OTP_BLOCK_18         0x00040000U /*!< OTP Block18    */
#define FLASH_OTP_BLOCK_19         0x00080000U /*!< OTP Block19    */
#define FLASH_OTP_BLOCK_20         0x00100000U /*!< OTP Block20    */
#define FLASH_OTP_BLOCK_21         0x00200000U /*!< OTP Block21    */
#define FLASH_OTP_BLOCK_22         0x00400000U /*!< OTP Block22    */
#define FLASH_OTP_BLOCK_23         0x00800000U /*!< OTP Block23    */
#define FLASH_OTP_BLOCK_24         0x01000000U /*!< OTP Block24    */
#define FLASH_OTP_BLOCK_25         0x02000000U /*!< OTP Block25    */
#define FLASH_OTP_BLOCK_26         0x04000000U /*!< OTP Block26    */
#define FLASH_OTP_BLOCK_27         0x08000000U /*!< OTP Block27    */
#define FLASH_OTP_BLOCK_28         0x10000000U /*!< OTP Block28    */
#define FLASH_OTP_BLOCK_29         0x20000000U /*!< OTP Block29    */
#define FLASH_OTP_BLOCK_30         0x40000000U /*!< OTP Block30    */
#define FLASH_OTP_BLOCK_31         0x80000000U /*!< OTP Block31    */
#define FLASH_OTP_BLOCK_ALL        0xFFFFFFFFU /*!< OTP All Blocks */
/**
  * @}
  */

/** @defgroup FLASH_WRP_State FLASH WRP State
  * @{
  */
#define OB_WRPSTATE_DISABLE       0x00000000U  /*!< Disable the write protection of the desired flash sectors */
#define OB_WRPSTATE_ENABLE        0x00000001U  /*!< Enable the write protection of the desired flash sectors  */
/**
  * @}
  */

/** @defgroup FLASH_OB_BOOT_CONFIG FLASH Option Bytes Boot configuration
  * @{
  */
#define OB_BOOT_NS      0x00000001U   /*!< Non-secure boot address */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define OB_BOOT_SEC     0x00000002U   /*!< Secure boot address */
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/** @defgroup FLASH_OB_BOOT_LOCK FLASH Option Bytes Boot Lock
  * @{
  */
#define OB_BOOT_LOCK_DISABLE   0xC3U  /*!< Boot lock disable */
#define OB_BOOT_LOCK_ENABLE    0xB4U  /*!< Boot lock enable */
/**
  * @}
  */

/** @defgroup FLASH_BB_Attributes FLASH Block-Base Attributes
  * @{
  */
#define FLASH_BB_SEC           0x01U    /*!< Flash Block-Based Security Attributes */
#define FLASH_BB_PRIV          0x02U    /*!< Flash Block-Based Privilege Attributes */
/**
  * @}
  */

/** @defgroup FLASH_PRIV_MODE FLASH privilege mode
  * @{
  */
#define FLASH_NSPRIV_GRANTED   0x00000000U           /*!< access to non-secure Flash registers is granted to privileged
                                                          or unprivileged access */
#define FLASH_NSPRIV_DENIED    FLASH_PRIVCFGR_NSPRIV /*!< access to non-secure Flash registers is denied to
                                                          non-privilege access */

#define FLASH_SPRIV_GRANTED    0x00000000U           /*!< access to secure Flash registers is granted to privileged or
                                                          unprivileged access */
#if defined (FLASH_PRIVCFGR_SPRIV)
#define FLASH_SPRIV_DENIED     FLASH_PRIVCFGR_SPRIV  /*!< access to secure Flash registers is denied to non-privilege
                                                          access */
#endif /* FLASH_PRIVCFGR_SPRIV */
/**
  * @}
  */

#if defined (FLASH_SR_OBKERR)
/** @defgroup FLASH_OBK_SWAP_Offset FLASH OBK Swap Offset
  * @{
  */
#define FLASH_OBK_SWAP_OFFSET_NO_DATA       0x000U   /*!< No data will be copied from current to alternate OBK        */
#define FLASH_OBK_SWAP_OFFSET_HDPL0         0x010U   /*!< HDPL0 data will be copied from current to alternate OBK     */
#define FLASH_OBK_SWAP_OFFSET_HDPL1         0x090U   /*!< HDPL0/1 data will be copied from current to alternate OBK   */
#define FLASH_OBK_SWAP_OFFSET_HDPL2         0x0C0U   /*!< HDPL0/1/2 data will be copied from current to alternate OBK */
#define FLASH_OBK_SWAP_OFFSET_HDPL3_S       0x180U   /*!< HDPL0/1/2/3_S data will be copied from current to alternate
                                                           OBK */
#define FLASH_OBK_SWAP_OFFSET_ALL           0x1FFU   /*!< All OBK data (511) will be copied from current to alternate
                                                           OBK */
/**
  * @}
  */
#endif /* FLASH_SR_OBKERR */

/** @defgroup FLASH_Operation_Type FLASH Operation Type
  * @{
  */
#define FLASH_OPERATION_TYPE_NONE         00000000U                                     /*!< No Flash operation       */
#define FLASH_OPERATION_TYPE_QUADWORD     FLASH_OPSR_CODE_OP_0                          /*!< Single write operation   */
#if defined (FLASH_SR_OBKERR)
#define FLASH_OPERATION_TYPE_OBKALTERASE  FLASH_OPSR_CODE_OP_1                          /*!< OBK alternate sector erase
                                                                                             operation */
#endif /* FLASH_SR_OBKERR */
#define FLASH_OPERATION_TYPE_SECTORERASE  (FLASH_OPSR_CODE_OP_1 | FLASH_OPSR_CODE_OP_0) /*!< Sector erase operation   */
#define FLASH_OPERATION_TYPE_BANKERASE    FLASH_OPSR_CODE_OP_2                          /*!< Bank erase operation     */
#define FLASH_OPERATION_TYPE_MASSERASE    (FLASH_OPSR_CODE_OP_2 | FLASH_OPSR_CODE_OP_0) /*!< Mass erase operation     */
#define FLASH_OPERATION_TYPE_OPTIONCHANGE (FLASH_OPSR_CODE_OP_2 | FLASH_OPSR_CODE_OP_1) /*!< Option change operation  */
#if defined (FLASH_SR_OBKERR)
#define FLASH_OPERATION_TYPE_OBKSWAP      (FLASH_OPSR_CODE_OP_2 | FLASH_OPSR_CODE_OP_1 | FLASH_OPSR_CODE_OP_0) /*!< OBK
                                                                                                       swap operation */
#endif /* FLASH_SR_OBKERR */
/**
  * @}
  */

/** @defgroup FLASH_Operation_Area FLASH Operation Area
  * @{
  */
#define FLASH_OPERATION_AREA_BANK_1        00000000U               /*!< Operation in Flash Bank 1               */
#define FLASH_OPERATION_AREA_BANK_2        FLASH_OPSR_BK_OP        /*!< Operation in Flash Bank 2               */
#define FLASH_OPERATION_AREA_SYSF          FLASH_OPSR_SYSF_OP      /*!< Operation in System Flash memory        */
#if defined (FLASH_EDATAR_EDATA_EN)
#define FLASH_OPERATION_AREA_DATA          FLASH_OPSR_DATA_OP      /*!< Operation in Flash high-cycle data area */
#endif /* FLASH_EDATAR_EDATA_EN */
#define FLASH_OPERATION_AREA_OTP           FLASH_OPSR_OTP_OP       /*!< Operation in Flash OTP area             */
/**
  * @}
  */

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/** @defgroup SEC_INVERSION_CFG FLASH security inversion configuration
  * @{
  */
#define FLASH_INV_DISABLE      0x00000000U        /*!< Security state of Flash is not inverted */
#define FLASH_INV_ENABLE       FLASH_CR_INV       /*!< Security state of Flash is inverted */
/**
  * @}
  */
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */
/* Exported macros ------------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Macros FLASHEx Exported Macros
  * @{
  */

/**
  * @brief  Enable the FLASH prefetch buffer.
  * @retval None
  */
#define __HAL_FLASH_PREFETCH_BUFFER_ENABLE()    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN)

/**
  * @brief  Disable the FLASH prefetch buffer.
  * @retval None
  */
#define __HAL_FLASH_PREFETCH_BUFFER_DISABLE()   CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTEN)

/**
  * @brief  Enable the FLASH smart prefetch buffer.
  * @retval None
  */
#define __HAL_FLASH_SMART_PREFETCH_BUFFER_ENABLE()    SET_BIT(FLASH->ACR, FLASH_ACR_S_PRFTEN)

/**
  * @brief  Disable the FLASH smart prefetch buffer.
  * @retval None
  */
#define __HAL_FLASH_SMART_PREFETCH_BUFFER_DISABLE()   CLEAR_BIT(FLASH->ACR, FLASH_ACR_S_PRFTEN)

/**
  * @brief  Set the FLASH Programming Delay.
  * @param  __DELAY__ FLASH Programming Delay
  *         This parameter can be a value of @ref FLASH_Programming_Delay
  * @retval none
  */
#define __HAL_FLASH_SET_PROGRAM_DELAY(__DELAY__)  MODIFY_REG(FLASH->ACR, FLASH_ACR_WRHIGHFREQ, (__DELAY__))

/**
  * @brief  Get the FLASH Programming Delay.
  * @retval FLASH Programming Delay
  *         This return value can be a value of @ref FLASH_Programming_Delay
  */
#define __HAL_FLASH_GET_PROGRAM_DELAY()     READ_BIT(FLASH->ACR, FLASH_ACR_WRHIGHFREQ)

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup FLASHEx_Exported_Functions
  * @{
  */

/** @addtogroup FLASHEx_Exported_Functions_Group1
  * @{
  */
/* Extension Erase and OB Program operation functions  ******************************/
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError);
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);
void              HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);
#if defined (FLASH_SR_OBKERR)
HAL_StatusTypeDef HAL_FLASHEx_OBK_Unlock(void);
HAL_StatusTypeDef HAL_FLASHEx_OBK_Lock(void);
HAL_StatusTypeDef HAL_FLASHEx_OBK_Swap(uint32_t SwapOffset);
HAL_StatusTypeDef HAL_FLASHEx_OBK_Swap_IT(uint32_t SwapOffset);
#endif /* FLASH_SR_OBKERR */
void              HAL_FLASHEx_GetOperation(FLASH_OperationTypeDef *pFlashOperation);
/**
  * @}
  */

/** @addtogroup FLASHEx_Exported_Functions_Group2
  * @{
  */
/* Extension Protection configuration functions  *************************************/
HAL_StatusTypeDef HAL_FLASHEx_ConfigBBAttributes(const FLASH_BBAttributesTypeDef *pBBAttributes);
void              HAL_FLASHEx_GetConfigBBAttributes(FLASH_BBAttributesTypeDef *pBBAttributes);
void              HAL_FLASHEx_ConfigPrivMode(uint32_t PrivMode);
uint32_t          HAL_FLASHEx_GetPrivMode(void);
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
HAL_StatusTypeDef HAL_FLASHEx_ConfigSecInversion(uint32_t SecInvState);
uint32_t          HAL_FLASHEx_GetSecInversion(void);
#endif /* __ARM_FEATURE_CMSE */
HAL_StatusTypeDef HAL_FLASHEx_ConfigHDPExtension(const FLASH_HDPExtensionTypeDef *pHDPExtension);
/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup FLASHEx_Exported_Functions_Group3
  * @{
  */
void              HAL_FLASHEx_EnableEccCorrectionInterrupt(void);
void              HAL_FLASHEx_DisableEccCorrectionInterrupt(void);
void              HAL_FLASHEx_GetEccInfo(FLASH_EccInfoTypeDef *pData);
void              HAL_FLASHEx_ECCD_IRQHandler(void);
__weak void       HAL_FLASHEx_EccDetectionCallback(void);
__weak void       HAL_FLASHEx_EccCorrectionCallback(void);
/**
  * @}
  */
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Constants FLASHEx Private Constants
  * @{
  */
#define FLASH_TYPEPROGRAM_OB (0x00008000U | FLASH_NON_SECURE_MASK) /*!< Program Option Bytes operation type */
#define FLASH_ADDRESS_OFFSET_OTP       (0x00000600U)               /*!< Flash address offset of OTP area */
#if defined(STM32H5F5xx)
#define FLASH_ADDRESS_OFFSET_EDATA     (0x0001E000U)               /*!< Flash address offset of EDATA area */
#elif defined(STM32H533xx) || defined(STM32H523xx)
#define FLASH_ADDRESS_OFFSET_EDATA     (0x00003000U)               /*!< Flash address offset of EDATA area */
#elif defined(STM32H553xx) || defined(STM32H543xx)
#define FLASH_ADDRESS_OFFSET_EDATA     (0x00007000U)               /*!< Flash address offset of EDATA area */
#else
#define FLASH_ADDRESS_OFFSET_EDATA     (0x0000F000U)               /*!< Flash address offset of EDATA area */
#endif /* STM32H533xx || STM32H523xx */
#if defined(STM32H5F5xx)
#define FLASH_EDATA_BANK_SIZE          (0x00018000U)               /*!< FLASH EDATA Bank Size */
#else
#define FLASH_EDATA_BANK_SIZE          (0x0000C000U)               /*!< FLASH EDATA Bank Size */
#endif /* STM32H5F5xx */
/**
  * @}
  */
/* Private macros ------------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Macros FLASHEx Private Macros
  * @{
  */

/** @defgroup FLASHEx_IS_FLASH_Definitions FLASHEx Private macros to check input parameters
  * @{
  */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_FLASH_TYPEERASE(VALUE)        (((VALUE) == FLASH_TYPEERASE_SECTORS)                         || \
                                          ((VALUE) == FLASH_TYPEERASE_SECTORS_NS)                      || \
                                          ((VALUE) == FLASH_TYPEERASE_MASSERASE)                       || \
                                          ((VALUE) == FLASH_TYPEERASE_MASSERASE_NS)                    || \
                                          ((VALUE) == FLASH_TYPEERASE_OBK_ALT))
#else
#if defined (FLASH_SR_OBKERR)
#define IS_FLASH_TYPEERASE(VALUE)        (((VALUE) == FLASH_TYPEERASE_SECTORS)                         || \
                                          ((VALUE) == FLASH_TYPEERASE_MASSERASE)                       || \
                                          ((VALUE) == FLASH_TYPEERASE_OBK_ALT))
#else
#define IS_FLASH_TYPEERASE(VALUE)        (((VALUE) == FLASH_TYPEERASE_SECTORS)                         || \
                                          ((VALUE) == FLASH_TYPEERASE_MASSERASE))
#endif /* FLASH_SR_OBKERR */
#endif /* __ARM_FEATURE_CMSE */

#define IS_WRPSTATE(VALUE)               (((VALUE) == OB_WRPSTATE_DISABLE)                             || \
                                          ((VALUE) == OB_WRPSTATE_ENABLE))

#define IS_OPTIONBYTE(VALUE)             ((((VALUE) & OPTIONBYTE_ALL) != 0U)                           && \
                                          (((VALUE) & ~OPTIONBYTE_ALL) == 0U))

#define IS_OB_PRODUCT_STATE(STATE)       (((STATE) == OB_PROD_STATE_OPEN)                              || \
                                          ((STATE) == OB_PROD_STATE_PROVISIONING)                      || \
                                          ((STATE) == OB_PROD_STATE_IROT_PROVISIONED)                  || \
                                          ((STATE) == OB_PROD_STATE_TZ_CLOSED)                         || \
                                          ((STATE) == OB_PROD_STATE_CLOSED)                            || \
                                          ((STATE) == OB_PROD_STATE_LOCKED)                            || \
                                          ((STATE) == OB_PROD_STATE_REGRESSION)                        || \
                                          ((STATE) == OB_PROD_STATE_NS_REGRESSION))

#define IS_OB_USER_BOR_LEVEL(LEVEL)      (((LEVEL) == OB_BOR_LEVEL_1) || ((LEVEL) == OB_BOR_LEVEL_2)   || \
                                          ((LEVEL) == OB_BOR_LEVEL_3))

#define IS_OB_USER_BORH_EN(VALUE)        (((VALUE) == OB_BORH_DISABLE) || ((VALUE) == OB_BORH_ENABLE))

#define IS_OB_USER_IWDG(VALUE)           (((VALUE) == OB_IWDG_HW) || ((VALUE) == OB_IWDG_SW))

#define IS_OB_USER_WWDG(VALUE)           (((VALUE) == OB_WWDG_HW) || ((VALUE) == OB_WWDG_SW))

#define IS_OB_USER_STOP(VALUE)           (((VALUE) == OB_STOP_RST) || ((VALUE) == OB_STOP_NORST))

#define IS_OB_USER_STANDBY(VALUE)        (((VALUE) == OB_STANDBY_RST) || ((VALUE) == OB_STANDBY_NORST))

#define IS_OB_USER_IO_VDD_HSLV(VALUE)    (((VALUE) == OB_IO_VDD_HSLV_DISABLE)                         || \
                                          ((VALUE) == OB_IO_VDD_HSLV_ENABLE))

#define IS_OB_USER_IO_VDDIO2_HSLV(VALUE) (((VALUE) == OB_IO_VDDIO2_HSLV_DISABLE)                      || \
                                          ((VALUE) == OB_IO_VDDIO2_HSLV_ENABLE))

#define IS_OB_USER_IWDG_STOP(VALUE)      (((VALUE) == OB_IWDG_STOP_FREEZE) || ((VALUE) == OB_IWDG_STOP_ACTIVE))

#define IS_OB_USER_IWDG_STDBY(VALUE)     (((VALUE) == OB_IWDG_STDBY_FREEZE) || ((VALUE) == OB_IWDG_STDBY_ACTIVE))

#define IS_OB_USER_BOOT_UBE(VALUE)       (((VALUE) == OB_UBE_OEM_IROT) || ((VALUE) == OB_UBE_ST_IROT))

#define IS_OB_USER_SWAP_BANK(VALUE)      (((VALUE) == OB_SWAP_BANK_DISABLE) || ((VALUE) == OB_SWAP_BANK_ENABLE))

#if defined (FLASH_OPTSR2_SRAM1_3_RST)
#define IS_OB_USER_SRAM1_3_RST(VALUE)    (((VALUE) == OB_SRAM1_3_RST_ERASE) || ((VALUE) == OB_SRAM1_3_RST_NOT_ERASE))
#endif /* FLASH_OPTSR2_SRAM1_3_RST */

#if defined (FLASH_OPTSR2_SRAM1_3_4_5_RST)
#define IS_OB_USER_SRAM1_3_4_5_RST(VALUE) (((VALUE) == OB_SRAM1_3_4_5_RST_ERASE) || \
                                           ((VALUE) == OB_SRAM1_3_4_5_RST_NOT_ERASE))
#endif /* FLASH_OPTSR2_SRAM1_3_4_5_RST */

#if defined (FLASH_OPTSR2_SRAM1_RST)
#define IS_OB_USER_SRAM1_RST(VALUE)      (((VALUE) == OB_SRAM1_RST_ERASE) || ((VALUE) == OB_SRAM1_RST_NOT_ERASE))
#endif /* FLASH_OPTSR2_SRAM1_RST */

#define IS_OB_USER_SRAM2_RST(VALUE)      (((VALUE) == OB_SRAM2_RST_ERASE) || ((VALUE) == OB_SRAM2_RST_NOT_ERASE))

#define IS_OB_USER_BKPRAM_ECC(VALUE)     (((VALUE) == OB_BKPRAM_ECC_ENABLE) || ((VALUE) == OB_BKPRAM_ECC_DISABLE))

#if defined (FLASH_OPTSR2_SRAM3_ECC)
#define IS_OB_USER_SRAM3_ECC(VALUE)      (((VALUE) == OB_SRAM3_ECC_ENABLE) || ((VALUE) == OB_SRAM3_ECC_DISABLE))
#endif /* FLASH_OPTSR2_SRAM3_ECC */

#if defined (FLASH_OPTSR2_SRAM1_ECC)
#define IS_OB_USER_SRAM1_ECC(VALUE)      (((VALUE) == OB_SRAM1_ECC_ENABLE) || ((VALUE) == OB_SRAM1_ECC_DISABLE))
#endif /* FLASH_OPTSR2_SRAM1_ECC */

#define IS_OB_USER_SRAM2_ECC(VALUE)      (((VALUE) == OB_SRAM2_ECC_ENABLE) || ((VALUE) == OB_SRAM2_ECC_DISABLE))

#if defined (FLASH_OPTSR2_HUK_PUF)
#define IS_OB_USER_HUK_PUF(VALUE)        (((VALUE) == OB_UNIQUE_KEY_PUF)    || ((VALUE) == OB_UNIQUE_KEY_HUK))
#endif /* FLASH_OPTSR2_HUK_PUF */

#if defined(FLASH_OPTSR2_USBPD_DIS)
#define IS_OB_USER_USBPD_DIS(VALUE)      (((VALUE) == OB_USBPD_DIS_ENABLE) || ((VALUE) == OB_USBPD_DIS_DISABLE))
#endif /* FLASH_OPTSR2_USBPD_DIS */
#define IS_OB_USER_TZEN(VALUE)           (((VALUE) == OB_TZEN_DISABLE) || ((VALUE) == OB_TZEN_ENABLE))

#define IS_OB_USER_TYPE(TYPE)           ((((TYPE) & OB_USER_ALL) != 0U) && \
                                         (((TYPE) & ~OB_USER_ALL) == 0U))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_OB_BOOT_CONFIG(CFG)           (((CFG) == OB_BOOT_NS) || ((CFG) == OB_BOOT_SEC))
#else
#define IS_OB_BOOT_CONFIG(CFG)            ((CFG) == OB_BOOT_NS)
#endif /* __ARM_FEATURE_CMSE */

#define IS_OB_BOOT_LOCK(VALUE)           (((VALUE) == OB_BOOT_LOCK_DISABLE) || ((VALUE) == OB_BOOT_LOCK_ENABLE))

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_FLASH_BB_EXCLUSIVE(CFG)       (((CFG) == FLASH_BB_SEC)  || ((CFG) == FLASH_BB_PRIV))
#else
#define IS_FLASH_BB_EXCLUSIVE(CFG)        ((CFG) == FLASH_BB_PRIV)
#endif /* __ARM_FEATURE_CMSE */

#define IS_FLASH_CFGPRIVMODE(CFG)        (((CFG) & 0xFFFFFFFCU) == 0U)

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
#define IS_FLASH_CFGSECINV(CFG)          (((CFG) == FLASH_INV_DISABLE) || ((CFG) == FLASH_INV_ENABLE))
#endif /* __ARM_FEATURE_CMSE */

#define IS_FLASH_EDATA_SIZE(SECTOR)      ((SECTOR) <= FLASH_EDATA_SECTOR_NB)
/**
  * @}
  */

/**
  * @}
  */
/* Private functions ---------------------------------------------------------*/
/** @defgroup FLASHEx_Private_Functions FLASHEx Private Functions
  * @{
  */
void FLASH_Erase_Sector(uint32_t Sector, uint32_t Banks);
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
#endif

#endif /* STM32H5xx_HAL_FLASH_EX_H */
