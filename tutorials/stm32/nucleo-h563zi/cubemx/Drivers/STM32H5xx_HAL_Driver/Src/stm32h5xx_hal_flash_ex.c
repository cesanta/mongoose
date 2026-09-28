/**
  ******************************************************************************
  * @file    stm32h5xx_hal_flash_ex.c
  * @author  MCD Application Team
  * @brief   Extended FLASH HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the FLASH extension peripheral:
  *           + Extended programming operations functions
  *
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
 @verbatim
 ==============================================================================
                   ##### Flash Extension features #####
  ==============================================================================

  [..] Comparing to other previous devices, the FLASH interface for STM32H5xx
       devices contains the following additional features

       (+) Capacity up to 2 Mbyte with dual bank architecture supporting read-while-write
           capability (RWW)
       (+) Dual bank memory organization
       (+) Product State protection
       (+) Write protection
       (+) Secure access only protection
       (+) Bank / register swapping (when Dual-Bank)
       (+) Watermark-based secure protection
       (+) Block-based secure protection
       (+) Block-based privilege protection
       (+) Hide Protection areas

                        ##### How to use this driver #####
 ==============================================================================
  [..] This driver provides functions to configure and program the FLASH memory
       of all STM32H5xx devices. It includes
      (#) FLASH Memory Erase functions:
           (++) Lock and Unlock the FLASH interface using HAL_FLASH_Unlock() and
                HAL_FLASH_Lock() functions
           (++) Erase function: Sector erase, bank erase and dual-bank mass erase
           (++) There are two modes of erase :
             (+++) Polling Mode using HAL_FLASHEx_Erase()
             (+++) Interrupt Mode using HAL_FLASHEx_Erase_IT()

      (#) Option Bytes Programming functions: Use HAL_FLASHEx_OBProgram() to:
        (++) Configure the write protection per bank
        (++) Set the Product State
        (++) Program the user Option Bytes
        (++) Configure the watermark security for each area
        (++) Configure the Hide protection areas
        (++) Configure the Boot addresses

      (#) Get Option Bytes Configuration function: Use HAL_FLASHEx_OBGetConfig() to:
        (++) Get the value of a write protection area
        (++) Get the Product State
        (++) Get the value of the user Option Bytes
        (++) Get the configuration of watermark security areas
        (++) Get the configuration of Hide protection areas
        (++) Get the value of a boot address

      (#) Block-based secure / privilege area configuration function: Use HAL_FLASHEx_ConfigBBAttributes()
        (++) Bit-field allowing to secure or un-secure each sector
        (++) Bit-field allowing to privilege or un-privilege each sector

      (#) Get the block-based secure / privilege area configuration function: Use HAL_FLASHEx_GetConfigBBAttributes()
        (++) Return the configuration of the block-based security and privilege for all the sectors

      (#) Privilege mode configuration function: Use HAL_FLASHEx_ConfigPrivMode()
        (++) FLASH register can be protected against non-privilege accesses

      (#) Get the privilege mode configuration function: Use HAL_FLASHEx_GetPrivMode()
        (++) Return if the FLASH registers are protected against non-privilege accesses


 @endverbatim
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup FLASHEx  FLASHEx
  * @brief FLASH HAL Extension module driver
  * @{
  */

#ifdef HAL_FLASH_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/** @defgroup FLASHEx_Private_Functions FLASHEx Private Functions
  * @{
  */
static void FLASH_MassErase(uint32_t Banks);
#if defined (FLASH_SR_OBKERR)
static void FLASH_OBKErase(void);
#endif /* FLASH_SR_OBKERR */
static void FLASH_OB_EnableWRP(uint64_t WRPSector, uint32_t Banks);
static void FLASH_OB_DisableWRP(uint64_t WRPSector, uint32_t Bank);
static void FLASH_OB_GetWRP(uint32_t Bank, uint32_t *WRPState, uint64_t *WRPSector);
static void FLASH_OB_ProdStateConfig(uint32_t ProdStateConfig);
static uint32_t FLASH_OB_GetProdState(void);
static void FLASH_OB_UserConfig(uint32_t UserType, uint32_t UserConfig1, uint32_t UserConfig2);
static void FLASH_OB_GetUser(uint32_t *UserConfig1, uint32_t *UserConfig2);
static void FLASH_OB_BootAddrConfig(uint32_t BootOption, uint32_t BootAddress);
static void FLASH_OB_BootLockConfig(uint32_t BootLockOption, uint32_t BootLockConfig);
static void FLASH_OB_GetBootConfig(uint32_t BootOption, uint32_t *BootAddress, uint32_t *BootLockConfig);
static void FLASH_OB_OTP_LockConfig(uint32_t OTP_Block);
static uint32_t FLASH_OB_OTP_GetLock(void);
static void FLASH_OB_HDPConfig(uint32_t Banks, uint32_t HDPStartSector, uint32_t HDPEndSector);
static void FLASH_OB_GetHDP(uint32_t Bank, uint32_t *HDPStartSector, uint32_t *HDPEndSector);
#if defined(FLASH_EDATAR_EDATA_EN)
static void FLASH_OB_EDATAConfig(uint32_t Banks, uint32_t EDATASize);
static void FLASH_OB_GetEDATA(uint32_t Bank, uint32_t *EDATASize);
#endif /* FLASH_EDATAR_EDATA_EN */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
static void FLASH_OB_WMSECConfig(uint32_t Banks, uint32_t WMSecStartSector, uint32_t WMSecEndSector);
static void FLASH_OB_GetWMSEC(uint32_t Bank, uint32_t *WMSecStartSector, uint32_t *WMSecEndSector);
#endif /* __ARM_FEATURE_CMSE */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup FLASHEx_Exported_Functions FLASHEx Exported Functions
  * @{
  */

/** @defgroup FLASHEx_Exported_Functions_Group1 FLASHEx Extended IO operation functions
  *  @brief   FLASHEx Extended IO operation functions
  *
@verbatim
 ===============================================================================
                ##### Extended programming operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the Extended FLASH
    programming operations Operations.

@endverbatim
  * @{
  */
/**
  * @brief  Perform a mass erase or erase the specified FLASH memory sectors
  * @param[in]  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  *
  * @param[out]  SectorError pointer to variable that contains the configuration
  *          information on faulty sector in case of error (0xFFFFFFFF means that all
  *          the sectors have been correctly erased).
  *
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError)
{
  HAL_StatusTypeDef status;
  uint32_t sector_index;
  __IO uint32_t *reg_cr;

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Reset error code */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /* Current operation type */
    pFlash.ProcedureOnGoing = pEraseInit->TypeErase;

    /* Access to SECCR or NSCR depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
    reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
    reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

    if ((pEraseInit->TypeErase & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEERASE_MASSERASE)
    {
      /* Mass erase to be done */
      FLASH_MassErase(pEraseInit->Banks);

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    }
#if defined (FLASH_SR_OBKERR)
    else if (pEraseInit->TypeErase == FLASH_TYPEERASE_OBK_ALT)
    {
      /* OBK erase to be done */
      FLASH_OBKErase();

      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    }
#endif /* FLASH_SR_OBKERR */
    else
    {
      /* Initialization of SectorError variable */
      *SectorError = 0xFFFFFFFFU;

      /* Erase by sector by sector to be done*/
      for (sector_index = pEraseInit->Sector; sector_index < (pEraseInit->NbSectors + pEraseInit->Sector); \
           sector_index++)
      {
        FLASH_Erase_Sector(sector_index, pEraseInit->Banks);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

        if (status != HAL_OK)
        {
          /* In case of error, stop erase procedure and return the faulty sector */
          *SectorError = sector_index;
          break;
        }
      }
    }

    /* If the erase operation is completed, disable the associated bits */
    CLEAR_BIT((*reg_cr), (pEraseInit->TypeErase) & (~(FLASH_NON_SECURE_MASK)));
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Perform a mass erase or erase the specified FLASH memory sectors with interrupt enabled
  * @param  pEraseInit pointer to an FLASH_EraseInitTypeDef structure that
  *         contains the configuration information for the erasing.
  *
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit)
{
  HAL_StatusTypeDef status;
  __IO uint32_t *reg_cr;

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEERASE(pEraseInit->TypeErase));

  /* Reset error code */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status != HAL_OK)
  {
    return status;
  }
  else
  {
    /* Set internal variables used by the IRQ handler */
    pFlash.ProcedureOnGoing = pEraseInit->TypeErase;
    pFlash.Bank = pEraseInit->Banks;

    /* Access to SECCR or NSCR depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
    reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
    reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

    /* Enable End of Operation and Error interrupts */
#if defined (FLASH_SR_OBKERR)
    (*reg_cr) |= (FLASH_IT_EOP     | FLASH_IT_WRPERR | FLASH_IT_PGSERR | \
                  FLASH_IT_STRBERR | FLASH_IT_INCERR | FLASH_IT_OBKERR | \
                  FLASH_IT_OBKWERR);
#else
    (*reg_cr) |= (FLASH_IT_EOP     | FLASH_IT_WRPERR | FLASH_IT_PGSERR | \
                  FLASH_IT_STRBERR | FLASH_IT_INCERR);
#endif /* FLASH_SR_OBKERR */

    if ((pEraseInit->TypeErase & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEERASE_MASSERASE)
    {
      /* Mass erase to be done */
      FLASH_MassErase(pEraseInit->Banks);
    }
#if defined (FLASH_SR_OBKERR)
    else if (pEraseInit->TypeErase == FLASH_TYPEERASE_OBK_ALT)
    {
      /* OBK erase to be done */
      FLASH_OBKErase();
    }
#endif /* FLASH_SR_OBKERR */
    else
    {
      /* Erase by sector to be done */
      pFlash.NbSectorsToErase = pEraseInit->NbSectors;
      pFlash.Sector = pEraseInit->Sector;

      /* Erase first sector and wait for IT */
      FLASH_Erase_Sector(pEraseInit->Sector, pEraseInit->Banks);
    }
  }

  return status;
}

/**
  * @brief  Program option bytes
  * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  *
  * @note   To configure any option bytes, the option lock bit OPTLOCK must be
  *         cleared with the call of HAL_FLASH_OB_Unlock() function.
  * @note   New option bytes configuration will be taken into account in two cases:
  *         - after an option bytes launch through the call of HAL_FLASH_OB_Launch()
  *         - after a power-on reset (BOR reset or exit from Standby/Shutdown modes)
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit)
{
  HAL_StatusTypeDef status;

  /* Check the parameters */
  assert_param(IS_OPTIONBYTE(pOBInit->OptionType));

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Reset Error Code */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

  /* Current operation type */
  pFlash.ProcedureOnGoing = FLASH_TYPEPROGRAM_OB;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /*Write protection configuration*/
    if ((pOBInit->OptionType & OPTIONBYTE_WRP) != 0U)
    {
      assert_param(IS_WRPSTATE(pOBInit->WRPState));

      if (pOBInit->WRPState == OB_WRPSTATE_ENABLE)
      {
        /* Enable write protection on the selected sectors */
        FLASH_OB_EnableWRP(pOBInit->WRPSector, pOBInit->Banks);
      }
      else
      {
        /* Disable write protection on the selected sectors */
        FLASH_OB_DisableWRP(pOBInit->WRPSector, pOBInit->Banks);
      }
    }

    /* Product State configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_PROD_STATE) != 0U)
    {
      /* Configure the product state */
      FLASH_OB_ProdStateConfig(pOBInit->ProductState);
    }

    /* User Configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_USER) != 0U)
    {
      /* Configure the user option bytes */
      FLASH_OB_UserConfig(pOBInit->USERType, pOBInit->USERConfig, pOBInit->USERConfig2);
    }

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    /* Watermark secure configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_WMSEC) != 0U)
    {
      /* Configure the watermark-based secure area */
      FLASH_OB_WMSECConfig(pOBInit->Banks, pOBInit->WMSecStartSector, pOBInit->WMSecEndSector);
    }
#endif /* __ARM_FEATURE_CMSE */

    /* Boot Address configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_BOOTADDR) != 0U)
    {
      FLASH_OB_BootAddrConfig(pOBInit->BootConfig, pOBInit->BootAddr);
    }

    /* Unique boot entry point configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_BOOT_LOCK) != 0U)
    {
      /* Configure the unique boot entry point */
      FLASH_OB_BootLockConfig(pOBInit->BootConfig, pOBInit->BootLock);
    }

    /* OTP Block Lock configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_OTP_LOCK) != 0U)
    {
      FLASH_OB_OTP_LockConfig(pOBInit->OTPBlockLock);
    }

    /* Hide Protection area configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_HDP) != 0U)
    {
      FLASH_OB_HDPConfig(pOBInit->Banks, pOBInit->HDPStartSector, pOBInit->HDPEndSector);
    }

#if defined(FLASH_EDATAR_EDATA_EN)
    /* Flash high-cycle data area configuration */
    if ((pOBInit->OptionType & OPTIONBYTE_EDATA) != 0U)
    {
      FLASH_OB_EDATAConfig(pOBInit->Banks, pOBInit->EDATASize);
    }
#endif /* FLASH_EDATAR_EDATA_EN */
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief Get the Option byte configuration
  * @param  pOBInit pointer to an FLASH_OBInitStruct structure that
  *         contains the configuration information for the programming.
  * @note   The parameter Banks of the pOBInit structure must be set exclusively to FLASH_BANK_1 or FLASH_BANK_2,
  *         as this parameter is use to get the given Bank WRP, PCROP and secured area configuration.
  *
  * @retval None
  */
void HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit)
{
  pOBInit->OptionType = (OPTIONBYTE_USER | OPTIONBYTE_PROD_STATE);

  /* Get Product State */
  pOBInit->ProductState = FLASH_OB_GetProdState();

  /* Get the user option bytes */
  FLASH_OB_GetUser(&(pOBInit->USERConfig), &(pOBInit->USERConfig2));

  if ((pOBInit->Banks == FLASH_BANK_1) || (pOBInit->Banks == FLASH_BANK_2))
  {
    /* Get write protection on the selected area */
    pOBInit->OptionType |= OPTIONBYTE_WRP;
    FLASH_OB_GetWRP(pOBInit->Banks, &(pOBInit->WRPState), &(pOBInit->WRPSector));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    /* Get the configuration of the watermark secure area for the selected area */
    pOBInit->OptionType |= OPTIONBYTE_WMSEC;
    FLASH_OB_GetWMSEC(pOBInit->Banks, &(pOBInit->WMSecStartSector), &(pOBInit->WMSecEndSector));
#endif /* __ARM_FEATURE_CMSE */

    /* Get the configuration of the hide protection for the selected area */
    pOBInit->OptionType |= OPTIONBYTE_HDP;
    FLASH_OB_GetHDP(pOBInit->Banks, &(pOBInit->HDPStartSector), &(pOBInit->HDPEndSector));
#if defined (FLASH_EDATAR_EDATA_EN)
    /* Get the Flash high-cycle data configuration for the selected area */
    pOBInit->OptionType |= OPTIONBYTE_EDATA;
    FLASH_OB_GetEDATA(pOBInit->Banks, &(pOBInit->EDATASize));
#endif /* FLASH_EDATAR_EDATA_EN */
  }

  /* Get boot configuration */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  if ((pOBInit->BootConfig == OB_BOOT_NS) || (pOBInit->BootConfig == OB_BOOT_SEC))
#else
  if (pOBInit->BootConfig == OB_BOOT_NS)
#endif /* __ARM_FEATURE_CMSE */
  {
    pOBInit->OptionType |= OPTIONBYTE_BOOTADDR | OPTIONBYTE_BOOT_LOCK;
    FLASH_OB_GetBootConfig(pOBInit->BootConfig, &(pOBInit->BootAddr), &(pOBInit->BootLock));
  }

  /* Get OTP Block Lock */
  pOBInit->OptionType |= OPTIONBYTE_OTP_LOCK;
  pOBInit->OTPBlockLock = FLASH_OB_OTP_GetLock();
}

#if defined (FLASH_SR_OBKERR)
/**
  * @brief  Unlock the FLASH OBK register access
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBK_Unlock(void)
{
  HAL_StatusTypeDef status = HAL_OK;

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  if (READ_BIT(FLASH->SECOBKCFGR, FLASH_OBKCFGR_LOCK) != 0U)
  {
    /* Authorize the FLASH OBK Register access */
    WRITE_REG(FLASH->SECOBKKEYR, FLASH_OBK_KEY1);
    WRITE_REG(FLASH->SECOBKKEYR, FLASH_OBK_KEY2);

    /* Verify Flash OBK Register is unlocked */
    if (READ_BIT(FLASH->SECOBKCFGR, FLASH_OBKCFGR_LOCK) != 0U)
    {
      status = HAL_ERROR;
    }
  }
#else
  if (READ_BIT(FLASH->NSOBKCFGR, FLASH_OBKCFGR_LOCK) != 0U)
  {
    /* Authorize the FLASH OBK Register access */
    WRITE_REG(FLASH->NSOBKKEYR, FLASH_OBK_KEY1);
    WRITE_REG(FLASH->NSOBKKEYR, FLASH_OBK_KEY2);

    /* Verify Flash OBK Register is unlocked */
    if (READ_BIT(FLASH->NSOBKCFGR, FLASH_OBKCFGR_LOCK) != 0U)
    {
      status = HAL_ERROR;
    }
  }
#endif /* __ARM_FEATURE_CMSE */

  return status;
}

/**
  * @brief  Locks the FLASH OBK register access
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBK_Lock(void)
{
  HAL_StatusTypeDef status = HAL_ERROR;

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  /* Set the LOCK Bit to lock the FLASH OBK Register access */
  SET_BIT(FLASH->SECOBKCFGR, FLASH_OBKCFGR_LOCK);

  /* verify Flash is locked */
  if (READ_BIT(FLASH->SECOBKCFGR, FLASH_OBKCFGR_LOCK) != 0U)
  {
    status = HAL_OK;
  }
#else
  /* Set the LOCK Bit to lock the FLASH OBK Register access */
  SET_BIT(FLASH->NSOBKCFGR, FLASH_OBKCFGR_LOCK);

  /* Verify Flash OBK is locked */
  if (READ_BIT(FLASH->NSOBKCFGR, FLASH_OBKCFGR_LOCK) != 0U)
  {
    status = HAL_OK;
  }
#endif /* __ARM_FEATURE_CMSE */

  return status;
}

/**
  * @brief  Swap the FLASH Option Bytes Keys (OBK)
  * @param  SwapOffset Specifies the number of keys to be swapped.
  *         This parameter can be a value between 0 (no OBK data swapped) and 511 (all OBK data swapped).
  *         Typical value are available in @ref FLASH_OBK_SWAP_Offset
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBK_Swap(uint32_t SwapOffset)
{
  HAL_StatusTypeDef status;
  __IO uint32_t *reg_obkcfgr;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /* Access to SECOBKCFGR or NSOBKCFGR registers depends on operation type */
    reg_obkcfgr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECOBKCFGR) : &(FLASH_NS->NSOBKCFGR);

    /* Set OBK swap offset */
    MODIFY_REG((*reg_obkcfgr), FLASH_OBKCFGR_SWAP_OFFSET, (SwapOffset << FLASH_OBKCFGR_SWAP_OFFSET_Pos));

    /* Set OBK swap request */
    SET_BIT((*reg_obkcfgr), FLASH_OBKCFGR_SWAP_SECT_REQ);

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
  }

  return status;
}
/**
  * @brief  Swap the FLASH Option Bytes Keys (OBK) with interrupt enabled
  * @param  SwapOffset Specifies the number of keys to be swapped.
  *         This parameter can be a value between 0 (no OBK data swapped) and 511 (all OBK data swapped).
  *         Typical value are available in @ref FLASH_OBK_SWAP_Offset
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_OBK_Swap_IT(uint32_t SwapOffset)
{
  HAL_StatusTypeDef status;
  __IO uint32_t *reg_obkcfgr;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /* Access to SECOBKCFGR or NSOBKCFGR registers depends on operation type */
    reg_obkcfgr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECOBKCFGR) : &(FLASH_NS->NSOBKCFGR);

  /* Enable End of Operation and Error interrupts */
  (*reg_obkcfgr) |= (FLASH_IT_EOP | FLASH_IT_WRPERR | FLASH_IT_PGSERR | FLASH_IT_STRBERR | FLASH_IT_INCERR);

    /* Set OBK swap offset */
    MODIFY_REG((*reg_obkcfgr), FLASH_OBKCFGR_SWAP_OFFSET, (SwapOffset << FLASH_OBKCFGR_SWAP_OFFSET_Pos));

    /* Set OBK swap request */
    SET_BIT((*reg_obkcfgr), FLASH_OBKCFGR_SWAP_SECT_REQ);
  }

  return status;
}
#endif /* FLASH_SR_OBKERR */

/**
  * @brief  Return the on-going Flash Operation. After a system reset, return
  *         the interrupted Flash operation, if any.
  * @param  pFlashOperation [out] pointer to a FLASH_OperationTypeDef structure
  *                               that contains the Flash operation information.
  * @retval None
  */
void HAL_FLASHEx_GetOperation(FLASH_OperationTypeDef *pFlashOperation)
{
  uint32_t opsr_reg = FLASH->OPSR;

  /* Get Flash operation Type */
  pFlashOperation->OperationType = opsr_reg & FLASH_OPSR_CODE_OP;

  /* Get Flash operation memory */
#if defined (FLASH_EDATAR_EDATA_EN)
  pFlashOperation->FlashArea = opsr_reg & (FLASH_OPSR_DATA_OP | FLASH_OPSR_BK_OP | \
                                           FLASH_OPSR_SYSF_OP | FLASH_OPSR_OTP_OP);
#else
  pFlashOperation->FlashArea = opsr_reg & (FLASH_OPSR_BK_OP | FLASH_OPSR_SYSF_OP | \
                                           FLASH_OPSR_OTP_OP);
#endif /* FLASH_EDATAR_EDATA_EN */
  /* Get Flash operation address */
  pFlashOperation->Address = opsr_reg & FLASH_OPSR_ADDR_OP;
}

/**
  * @}
  */

/** @defgroup FLASHEx_Exported_Functions_Group2 FLASHEx Extension Protection configuration functions
  *  @brief   Extension Protection configuration functions
  * @{
  */

/**
  * @brief  Configure the block-based secure area.
  *
  * @param  pBBAttributes pointer to an FLASH_BBAttributesTypeDef structure that
  *         contains the configuration information for the programming.
  *
  * @note   The field pBBAttributes->Bank should indicate which area is requested
  *         for the block-based attributes.
  * @note   The field pBBAttributes->BBAttributesType should indicate which
  *         block-base attribute type is requested: Secure or Privilege.
  *
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_ConfigBBAttributes(const FLASH_BBAttributesTypeDef *pBBAttributes)
{
  HAL_StatusTypeDef status;
  uint8_t index;
  __IO uint32_t *reg;

  /* Check the parameters */
  assert_param(IS_FLASH_BANK_EXCLUSIVE(pBBAttributes->Bank));
  assert_param(IS_FLASH_BB_EXCLUSIVE(pBBAttributes->BBAttributesType));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /* Set the first Block-Based register to write */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if (pBBAttributes->BBAttributesType == FLASH_BB_SEC)
    {
      if (pBBAttributes->Bank == FLASH_BANK_1)
      {
        reg = &(FLASH->SECBB1R1);
      }
      else
      {
        reg = &(FLASH->SECBB2R1);
      }
    }
    else
#endif /* __ARM_FEATURE_CMSE */
    {
      if (pBBAttributes->Bank == FLASH_BANK_1)
      {
        reg = &(FLASH->PRIVBB1R1);
      }
      else
      {
        reg = &(FLASH->PRIVBB2R1);
      }
    }

    /* Modify the register values and check that new attributes are taken in account */
    for (index = 0; index < FLASH_BLOCKBASED_NB_REG; index++)
    {
      *reg = pBBAttributes->BBAttributes_array[index] & FLASH_PRIVBBR_PRIVBB;
      if ((*reg) != (pBBAttributes->BBAttributes_array[index] & FLASH_PRIVBBR_PRIVBB))
      {
        status = HAL_ERROR;
      }
      reg++;
    }

    /* ISB instruction is called to be sure next instructions are performed with correct attributes */
    __ISB();
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Return the block-based attributes.
  *
  * @param  pBBAttributes [in/out] pointer to an FLASH_BBAttributesTypeDef structure
  *                 that contains the configuration information.
  * @note   The field pBBAttributes->Bank should indicate which area is requested
  *         for the block-based attributes.
  * @note   The field pBBAttributes->BBAttributesType should indicate which
  *         block-base attribute type is requested: Secure or Privilege.
  *
  * @retval None
  */
void HAL_FLASHEx_GetConfigBBAttributes(FLASH_BBAttributesTypeDef *pBBAttributes)
{
  uint8_t index;
  __IO uint32_t *reg;

  /* Check the parameters */
  assert_param(IS_FLASH_BANK_EXCLUSIVE(pBBAttributes->Bank));
  assert_param(IS_FLASH_BB_EXCLUSIVE(pBBAttributes->BBAttributesType));

  /* Set the first Block-Based register to read */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  if (pBBAttributes->BBAttributesType == FLASH_BB_SEC)
  {
    if (pBBAttributes->Bank == FLASH_BANK_1)
    {
      reg = &(FLASH->SECBB1R1);
    }
    else
    {
      reg = &(FLASH->SECBB2R1);
    }
  }
  else
#endif /* __ARM_FEATURE_CMSE */
  {
    if (pBBAttributes->Bank == FLASH_BANK_1)
    {
      reg = &(FLASH->PRIVBB1R1);
    }
    else
    {
      reg = &(FLASH->PRIVBB2R1);
    }
  }

  /* Read the register values */
  for (index = 0; index < FLASH_BLOCKBASED_NB_REG; index++)
  {
    pBBAttributes->BBAttributes_array[index] = (*reg) & FLASH_PRIVBBR_PRIVBB;
    reg++;
  }
}

/**
  * @brief  Configuration of the privilege attribute.
  *
  * @param  PrivMode indicate privilege mode configuration
  *      This parameter can be one of the following values:
  *      @arg FLASH_SPRIV_GRANTED: access to secure Flash registers is granted to privileged or unprivileged access
  *      @arg FLASH_SPRIV_DENIED: access to secure Flash registers is denied to unprivileged access
  *      @arg FLASH_NSPRIV_GRANTED: access to non-secure Flash registers is granted to privileged or unprivileged access
  *      @arg FLASH_NSPRIV_DENIED: access to non-secure Flash registers is denied to unprivilege access
  *
  * @retval None
  */
void HAL_FLASHEx_ConfigPrivMode(uint32_t PrivMode)
{
  /* Check the parameters */
  assert_param(IS_FLASH_CFGPRIVMODE(PrivMode));
#if defined (FLASH_PRIVCFGR_SPRIV)
  MODIFY_REG(FLASH->PRIVCFGR, (FLASH_PRIVCFGR_SPRIV | FLASH_PRIVCFGR_NSPRIV), PrivMode);
#else
  MODIFY_REG(FLASH->PRIVCFGR, FLASH_PRIVCFGR_NSPRIV, PrivMode);
#endif /* FLASH_PRIVCFGR_SPRIV */
}

/**
  * @brief  Return the value of the privilege attribute.
  *
  * @retval  It indicates the privilege mode configuration.
  *      This return value can be one of the following values:
  *      @arg FLASH_SPRIV_GRANTED: access to secure Flash registers is granted to privileged or unprivileged access
  *      @arg FLASH_SPRIV_DENIED: access to secure Flash registers is denied to unprivileged access
  *      @arg FLASH_NSPRIV_GRANTED: access to non-secure Flash registers is granted to privileged or unprivileged access
  *      @arg FLASH_NSPRIV_DENIED: access to Flash registers is denied to unprivilege accessP
  */
uint32_t HAL_FLASHEx_GetPrivMode(void)
{
#if defined (FLASH_PRIVCFGR_SPRIV)
  return (FLASH->PRIVCFGR & (FLASH_PRIVCFGR_SPRIV | FLASH_PRIVCFGR_NSPRIV));
#else
  return (FLASH->PRIVCFGR & FLASH_PRIVCFGR_NSPRIV);
#endif /* FLASH_PRIVCFGR_SPRIV */
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configuration of the security inversion.
  *
  * @param  SecInvState indicate the flash security state configuration
  *          This parameter can be one of the following values:
  *            @arg FLASH_SEC_INV_DISABLE: Security state of Flash is not inverted
  *            @arg FLASH_SEC_INV_ENABLE: Security state of Flash is inverted
  *
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_ConfigSecInversion(uint32_t SecInvState)
{
  HAL_StatusTypeDef status;

  /* Check the parameters */
  assert_param(IS_FLASH_CFGSECINV(SecInvState));

  /* Process Locked */
  __HAL_LOCK(&pFlash);

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    MODIFY_REG(FLASH->SECCR, FLASH_CR_INV, SecInvState);
  }

  /* Process Unlocked */
  __HAL_UNLOCK(&pFlash);

  return status;
}

/**
  * @brief  Return the value of the security inversion.
  *
  * @retval  It indicates the flash security state configuration
  *          This return value can be one of the following values:
  *            @arg FLASH_SEC_INV_DISABLE: Security state of Flash is not inverted
  *            @arg FLASH_SEC_INV_ENABLE: Security state of Flash is inverted
  */
uint32_t HAL_FLASHEx_GetSecInversion(void)
{
  return (FLASH->SECCR & FLASH_CR_INV);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Configure the HDP extension area.
  *
  * @param  pHDPExtension pointer to an FLASH_HDPExtentionTypeDef structure that
  *         contains the configuration information.
  *
  * @note   The field pHDPExtension->Banks should indicate which area is requested
  *         for the HDP extension.
  * @note   The field pHDPExtension->NbSectors should indicate the number of
  *         sector to be added to the HDP area.
  *
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_ConfigHDPExtension(const FLASH_HDPExtensionTypeDef *pHDPExtension)
{
  /* Check the parameters */
  assert_param(IS_FLASH_BANK(pHDPExtension->Banks));
  assert_param(IS_FLASH_SECTOR(pHDPExtension->NbSectors));

  /* Set the HDP extension register */
  if (pHDPExtension->Banks == FLASH_BANK_1)
  {
    MODIFY_REG(FLASH->HDPEXTR, FLASH_HDPEXTR_HDP1_EXT, pHDPExtension->NbSectors);
  }
  else if (pHDPExtension->Banks == FLASH_BANK_2)
  {
    MODIFY_REG(FLASH->HDPEXTR, FLASH_HDPEXTR_HDP2_EXT, (pHDPExtension->NbSectors << FLASH_HDPEXTR_HDP2_EXT_Pos));
  }
  else
  {
    FLASH->HDPEXTR = (pHDPExtension->NbSectors << FLASH_HDPEXTR_HDP2_EXT_Pos) | pHDPExtension->NbSectors;
  }

  return HAL_OK;
}

/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @addtogroup FLASHEx_Private_Functions
  * @{
  */

/**
  * @brief  Mass erase of FLASH memory
  * @param  Banks Banks to be erased
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: Bank1 to be erased
  *            @arg FLASH_BANK_2: Bank2 to be erased
  *            @arg FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  * @retval None
  */
static void FLASH_MassErase(uint32_t Banks)
{
  __IO uint32_t *reg_cr;

  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));

  /* Access to SECCR or NSCR registers depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
  reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

  /* Flash Mass Erase */
  if ((Banks & FLASH_BANK_BOTH) == FLASH_BANK_BOTH)
  {
    /* Set Mass Erase Bit */
    SET_BIT((*reg_cr), FLASH_CR_MER | FLASH_CR_START);
  }
  else
  {
    /* Proceed to erase Flash Bank  */
    if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
    {
      /* Erase Bank1 */
      MODIFY_REG((*reg_cr), (FLASH_CR_BKSEL | FLASH_CR_BER | FLASH_CR_START), (FLASH_CR_BER | FLASH_CR_START));
    }

    if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
    {
      /* Erase Bank2 */
      SET_BIT((*reg_cr), (FLASH_CR_BER | FLASH_CR_BKSEL | FLASH_CR_START));
    }
  }
}

/**
  * @brief  Erase the specified FLASH memory sector
  * @param  Sector FLASH sector to erase
  *          This parameter can be a value of @ref FLASH_Sectors
  * @param  Banks Bank(s) where the sector will be erased
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: Sector in bank 1 to be erased
  *            @arg FLASH_BANK_2: Sector in bank 2 to be erased
  * @retval None
  */
void FLASH_Erase_Sector(uint32_t Sector, uint32_t Banks)
{
  __IO uint32_t *reg_cr;

  /* Check the parameters */
  assert_param(IS_FLASH_SECTOR(Sector));
  assert_param(IS_FLASH_BANK_EXCLUSIVE(Banks));

  /* Access to SECCR or NSCR registers depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
  reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    /* Reset Sector Number for Bank1 */
    (*reg_cr) &= ~(FLASH_CR_SNB | FLASH_CR_BKSEL);

    (*reg_cr) |= (FLASH_CR_SER | (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
  }
  else
  {
    /* Reset Sector Number for Bank2 */
    (*reg_cr) &= ~(FLASH_CR_SNB);

    (*reg_cr) |= (FLASH_CR_SER | FLASH_CR_BKSEL | (Sector << FLASH_CR_SNB_Pos) | FLASH_CR_START);
  }
}

#if defined (FLASH_SR_OBKERR)
/**
  * @brief  Erase of FLASH OBK
  * @retval None
  */
static void FLASH_OBKErase()
{
  __IO uint32_t *reg_obkcfgr;

  /* Access to SECOBKCFGR or NSOBKCFGR registers depends on operation type */
  reg_obkcfgr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECOBKCFGR) : &(FLASH_NS->NSOBKCFGR);

  /* Set OBK Erase Bit */
  SET_BIT((*reg_obkcfgr), FLASH_OBKCFGR_ALT_SECT_ERASE);
}
#endif /* FLASH_SR_OBKERR */

/**
  * @brief  Enable the write protection of the desired bank1 or bank 2 sectors
  * @param  WRPSector specifies the sectors to be write protected.
  *         This parameter can be a value of @ref FLASH_OB_Write_Protection_Sectors
  *
  * @param  Banks the specific bank to apply WRP sectors
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: enable WRP on specified bank1 sectors
  *            @arg FLASH_BANK_2: enable WRP on specified bank2 sectors
  *            @arg FLASH_BANK_BOTH: enable WRP on both bank1 and bank2 specified sectors
  *
  * @retval None
  */
static void FLASH_OB_EnableWRP(uint64_t WRPSector, uint32_t Banks)
{
  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));
#if defined(OB_WRP_SECTOR_128TO131)
  uint32_t WRPSector_Group1 = (uint32_t)(WRPSector & 0xFFFFFFFF);
  uint32_t WRPSector_Group2 = (uint32_t)((WRPSector >> 32) & 0xFFFFFFFF);
  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    if (WRPSector_Group1 != 0)
    {
        /* Enable Write Protection for bank 1 Group1 */
        FLASH->WRP11R_PRG &= (~(WRPSector_Group1 & FLASH_WRPR_WRPSG));
    }
    if (WRPSector_Group2 != 0)
    {
        /* Enable Write Protection for bank 1 Group2 */
        FLASH->WRP12R_PRG &= (~(WRPSector_Group2 & FLASH_WRPR_WRPSG));
    }
  }
  if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
  {
    if (WRPSector_Group1 != 0)
    {
        /* Enable Write Protection for bank 2 Group1 */
        FLASH->WRP21R_PRG &= (~(WRPSector_Group1 & FLASH_WRPR_WRPSG));
    }
    if (WRPSector_Group2 != 0)
    {
        /* Enable Write Protection for bank 2 Group2 */
        FLASH->WRP22R_PRG &= (~(WRPSector_Group2 & FLASH_WRPR_WRPSG));
    }
  }
#else
  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    /* Enable Write Protection for bank 1 */
    FLASH->WRP1R_PRG &= (~((uint32_t)(WRPSector & FLASH_WRPR_WRPSG)));
  }

  if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
  {
    /* Enable Write Protection for bank 2 */
    FLASH->WRP2R_PRG &= (~((uint32_t)(WRPSector & FLASH_WRPR_WRPSG)));
  }
#endif /* OB_WRP_SECTOR_128TO131 */
}

/**
  * @brief  Disable the write protection of the desired bank1 or bank 2 sectors
  * @param  WRPSector specifies the sectors to disable write protection.
  *         This parameter can be a value of @ref FLASH_OB_Write_Protection_Sectors
  *
  * @param  Banks the specific bank to apply WRP sectors
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: disable WRP on specified bank1 sectors
  *            @arg FLASH_BANK_2: disable WRP on specified bank2 sectors
  *            @arg FLASH_BANK_BOTH: disable WRP on both bank1 and bank2 specified sectors
  *
  * @retval None
  */
static void FLASH_OB_DisableWRP(uint64_t WRPSector, uint32_t Banks)
{
  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));
#if defined(OB_WRP_SECTOR_128TO131)
  uint32_t WRPSector_Group1 = (uint32_t)(WRPSector & 0xFFFFFFFF);
  uint32_t WRPSector_Group2 = (uint32_t)((WRPSector >> 32) & 0xFFFFFFFF);
  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    if (WRPSector_Group1 != 0)
    {
      /* Disable Write Protection for bank 1 group1 */
      FLASH->WRP11R_PRG |= (WRPSector_Group1 & FLASH_WRPR_WRPSG);
    }
    if (WRPSector_Group2 != 0)
    {
      /* Disable Write Protection for bank 1 group2 */
      FLASH->WRP12R_PRG |= (WRPSector_Group2 & FLASH_WRPR_WRPSG);
    }
  }

  if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
  {
    if (WRPSector_Group1 != 0)
    {
      /* Disable Write Protection for bank 2 group1 */
      FLASH->WRP21R_PRG |= (WRPSector_Group1 & FLASH_WRPR_WRPSG);
    }
    if (WRPSector_Group2 != 0)
    {
      /* Disable Write Protection for bank 2 group2 */
      FLASH->WRP22R_PRG |= (WRPSector_Group2 & FLASH_WRPR_WRPSG);
    }
  }
#else
  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    /* Disable Write Protection for bank 1 */
    FLASH->WRP1R_PRG |= (uint32_t)(WRPSector & FLASH_WRPR_WRPSG);
  }

  if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
  {
    /* Disable Write Protection for bank 2 */
    FLASH->WRP2R_PRG |= (uint32_t)(WRPSector & FLASH_WRPR_WRPSG);
  }
#endif /* OB_WRP_SECTOR_128TO131 */
}

/**
  * @brief  Get the write protection of the given bank 1 or bank 2 sectors
  * @param[in]  Bank specifies the bank where to get the write protection sectors.
  *         This parameter can be exclusively one of the following values:
  *         @arg FLASH_BANK_1: Get bank1 WRP sectors
  *         @arg FLASH_BANK_2: Get bank2 WRP sectors
  *
  * @param[out]  WRPState returns the write protection state of the returned sectors.
  *         This parameter can be one of the following values:
  *         @arg WRPState: OB_WRPSTATE_DISABLE or OB_WRPSTATE_ENABLE

  * @param[out]  WRPSector returns the write protected sectors on the given bank .
  *         This parameter can be a value of @ref FLASH_OB_Write_Protection_Sectors
  *
  * @retval None
  */
static void FLASH_OB_GetWRP(uint32_t Bank, uint32_t *WRPState, uint64_t *WRPSector)
{
  uint32_t regvalue1 = 0U;

#if defined(OB_WRP_SECTOR_128TO131)
  uint32_t regvalue2 = 0U;

  if (Bank == FLASH_BANK_1)
  {
    regvalue1 = FLASH->WRP11R_CUR;
    regvalue2 = FLASH->WRP12R_CUR;
  }

  if (Bank == FLASH_BANK_2)
  {
    regvalue1 = FLASH->WRP21R_CUR;
    regvalue2 = FLASH->WRP22R_CUR;
  }

  uint64_t wrp_sector_group1 = (~regvalue1) & FLASH_WRPR_WRPSG;
  uint64_t wrp_sector_group2 = (~regvalue2) & FLASH_WRPR_WRPSG;

  *WRPSector = (wrp_sector_group2 << 32) | wrp_sector_group1;

  if (*WRPSector == 0U)
  {
    *WRPState = OB_WRPSTATE_DISABLE;
  }
  else
  {
    *WRPState = OB_WRPSTATE_ENABLE;
  }
#else
  if (Bank == FLASH_BANK_1)
  {
    regvalue1 = FLASH->WRP1R_CUR;
  }

  if (Bank == FLASH_BANK_2)
  {
    regvalue1 = FLASH->WRP2R_CUR;
  }
  
  *WRPSector = ((uint64_t)(~regvalue1) & (uint64_t)FLASH_WRPR_WRPSG);

  if (*WRPSector == 0U)
  {
    *WRPState = OB_WRPSTATE_DISABLE;
  }
  else
  {
    *WRPState = OB_WRPSTATE_ENABLE;
  }
#endif /* OB_WRP_SECTOR_128TO131 */
}

/**
  * @brief  Set the product state.
  *
  * @note   To configure the product state, the option lock bit OPTLOCK must be
  *         cleared with the call of the HAL_FLASH_OB_Unlock() function.
  * @note   To validate the product state, the option bytes must be reloaded
  *         through the call of the HAL_FLASH_OB_Launch() function.
  *
  * @param  ProductState specifies the product state.
  *         This parameter can be a value of @ref FLASH_OB_Product_State
  *
  * @retval None
  */
static void FLASH_OB_ProdStateConfig(uint32_t ProductState)
{
  /* Check the parameters */
  assert_param(IS_OB_PRODUCT_STATE(ProductState));

  /* Configure the Product State in the option bytes register */
  MODIFY_REG(FLASH->OPTSR_PRG, FLASH_OPTSR_PRODUCT_STATE, ProductState);
}

/**
  * @brief  Get the the product state.
  * @retval ProductState returns the product state.
  *         This returned value can a value of @ref FLASH_OB_Product_State
  */
static uint32_t FLASH_OB_GetProdState(void)
{
  return (FLASH->OPTSR_CUR & FLASH_OPTSR_PRODUCT_STATE);
}

/**
  * @brief  Program the FLASH User Option Byte.
  *
  * @note   To configure the user option bytes, the option lock bit OPTLOCK must
  *         be cleared with the call of the HAL_FLASH_OB_Unlock() function.
  * @note   To validate the user option bytes, the option bytes must be reloaded
  *         through the call of the HAL_FLASH_OB_Launch() function.
  *
  * @param  UserType specifies The FLASH User Option Bytes to be modified.
  *         This parameter can be a combination of @ref FLASH_OB_USER_Type
  *
  * @param  UserConfig1 specifies values of the selected User Option Bytes.
  *         This parameter can be a combination of @ref FLASH_OB_USER_BOR_LEVEL,
  *         @ref FLASH_OB_USER_BORH_EN, @ref FLASH_OB_USER_IWDG_SW,
  *         @ref FLASH_OB_USER_WWDG_SW, @ref FLASH_OB_USER_nRST_STOP,
  *         @ref FLASH_OB_USER_nRST_STANDBY, @ref FLASH_OB_USER_IO_VDD_HSLV,
  *         @ref FLASH_OB_USER_IO_VDDIO2_HSLV, @ref FLASH_OB_USER_IWDG_STOP,
  *         @ref FLASH_OB_USER_IWDG_STANDBY, @ref FLASH_OB_USER_BOOT_UBE and @ref OB_USER_SWAP_BANK.
  * @param  UserConfig2 specifies values of the selected User Option Bytes.
  *         @ref FLASH_OB_USER_SRAM1_3_RST, @ref FLASH_OB_USER_SRAM2_RST,
  *         @ref FLASH_OB_USER_BKPRAM_ECC, @ref FLASH_OB_USER_SRAM3_ECC,
  *         @ref FLASH_OB_USER_SRAM2_ECC, @ref FLASH_OB_USER_SRAM1_ECC,
  *         @ref FLASH_OB_USER_SRAM1_RST and @ref OB_USER_TZEN.
  * @retval None
  */
static void FLASH_OB_UserConfig(uint32_t UserType, uint32_t UserConfig1, uint32_t UserConfig2)
{
  uint32_t optr_reg1_val = 0U;
  uint32_t optr_reg1_mask = 0U;
  uint32_t optr_reg2_val = 0U;
  uint32_t optr_reg2_mask = 0U;

  /* Check the parameters */
  assert_param(IS_OB_USER_TYPE(UserType));

  if ((UserType & OB_USER_BOR_LEV) != 0U)
  {
    /* BOR level option byte should be modified */
    assert_param(IS_OB_USER_BOR_LEVEL(UserConfig1 & FLASH_OPTSR_BOR_LEV));

    /* Set value and mask for BOR level option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_BOR_LEV);
    optr_reg1_mask |= FLASH_OPTSR_BOR_LEV;
  }

  if ((UserType & OB_USER_BORH_EN) != 0U)
  {
    /* BOR high enable status bit should be modified */
    assert_param(IS_OB_USER_BORH_EN(UserConfig1 & FLASH_OPTSR_BORH_EN));

    /* Set value and mask for BOR high enable status bit */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_BORH_EN);
    optr_reg1_mask |= FLASH_OPTSR_BORH_EN;
  }

  if ((UserType & OB_USER_IWDG_SW) != 0U)
  {
    /* IWDG_SW option byte should be modified */
    assert_param(IS_OB_USER_IWDG(UserConfig1 & FLASH_OPTSR_IWDG_SW));

    /* Set value and mask for IWDG_SW option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_IWDG_SW);
    optr_reg1_mask |= FLASH_OPTSR_IWDG_SW;
  }

  if ((UserType & OB_USER_WWDG_SW) != 0U)
  {
    /* WWDG_SW option byte should be modified */
    assert_param(IS_OB_USER_WWDG(UserConfig1 & FLASH_OPTSR_WWDG_SW));

    /* Set value and mask for WWDG_SW option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_WWDG_SW);
    optr_reg1_mask |= FLASH_OPTSR_WWDG_SW;
  }

  if ((UserType & OB_USER_NRST_STOP) != 0U)
  {
    /* nRST_STOP option byte should be modified */
    assert_param(IS_OB_USER_STOP(UserConfig1 & FLASH_OPTSR_NRST_STOP));

    /* Set value and mask for nRST_STOP option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_NRST_STOP);
    optr_reg1_mask |= FLASH_OPTSR_NRST_STOP;
  }

  if ((UserType & OB_USER_NRST_STDBY) != 0U)
  {
    /* nRST_STDBY option byte should be modified */
    assert_param(IS_OB_USER_STANDBY(UserConfig1 & FLASH_OPTSR_NRST_STDBY));

    /* Set value and mask for nRST_STDBY option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_NRST_STDBY);
    optr_reg1_mask |= FLASH_OPTSR_NRST_STDBY;
  }

  if ((UserType & OB_USER_IO_VDD_HSLV) != 0U)
  {
    /* IO_VDD_HSLV option byte should be modified */
    assert_param(IS_OB_USER_IO_VDD_HSLV(UserConfig1 & FLASH_OPTSR_IO_VDD_HSLV));

    /* Set value and mask for IO_VDD_HSLV option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_IO_VDD_HSLV);
    optr_reg1_mask |= FLASH_OPTSR_IO_VDD_HSLV;
  }

  if ((UserType & OB_USER_IO_VDDIO2_HSLV) != 0U)
  {
    /* IO_VDD_HSLV option byte should be modified */
    assert_param(IS_OB_USER_IO_VDDIO2_HSLV(UserConfig1 & FLASH_OPTSR_IO_VDDIO2_HSLV));

    /* Set value and mask for IO_VDD_HSLV option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_IO_VDDIO2_HSLV);
    optr_reg1_mask |= FLASH_OPTSR_IO_VDDIO2_HSLV;
  }

  if ((UserType & OB_USER_IWDG_STOP) != 0U)
  {
    /* IWDG_STOP option byte should be modified */
    assert_param(IS_OB_USER_IWDG_STOP(UserConfig1 & FLASH_OPTSR_IWDG_STOP));

    /* Set value and mask for IWDG_STOP option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_IWDG_STOP);
    optr_reg1_mask |= FLASH_OPTSR_IWDG_STOP;
  }

  if ((UserType & OB_USER_IWDG_STDBY) != 0U)
  {
    /* IWDG_STDBY option byte should be modified */
    assert_param(IS_OB_USER_IWDG_STDBY(UserConfig1 & FLASH_OPTSR_IWDG_STDBY));

    /* Set value and mask for IWDG_STDBY option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_IWDG_STDBY);
    optr_reg1_mask |= FLASH_OPTSR_IWDG_STDBY;
  }

#if defined (FLASH_OPTSR_BOOT_UBE)
  if ((UserType & OB_USER_BOOT_UBE) != 0U)
  {
    /* SWAP_BANK option byte should be modified */
    assert_param(IS_OB_USER_BOOT_UBE(UserConfig1 & FLASH_OPTSR_BOOT_UBE));

    /* Set value and mask for BOOT_UBE option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_BOOT_UBE);
    optr_reg1_mask |= FLASH_OPTSR_BOOT_UBE;
  }
#endif /* FLASH_OPTSR_BOOT_UBE */

  if ((UserType & OB_USER_SWAP_BANK) != 0U)
  {
    /* SWAP_BANK option byte should be modified */
    assert_param(IS_OB_USER_SWAP_BANK(UserConfig1 & FLASH_OPTSR_SWAP_BANK));

    /* Set value and mask for SWAP_BANK option byte */
    optr_reg1_val |= (UserConfig1 & FLASH_OPTSR_SWAP_BANK);
    optr_reg1_mask |= FLASH_OPTSR_SWAP_BANK;
  }

#if defined (FLASH_OPTSR2_SRAM1_3_RST)
  if ((UserType & OB_USER_SRAM1_3_RST) != 0U)
  {
    /* SRAM13_RST option byte should be modified */
    assert_param(IS_OB_USER_SRAM1_3_RST(UserConfig2 & FLASH_OPTSR2_SRAM1_3_RST));

    /* Set value and mask for SRAM13_RST option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM1_3_RST);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM1_3_RST;
  }
#elif defined (FLASH_OPTSR2_SRAM1_3_4_5_RST)
  if ((UserType & OB_USER_SRAM1_3_4_5_RST) != 0U)
  {
    /* SRAM13_RST option byte should be modified */
    assert_param(IS_OB_USER_SRAM1_3_4_5_RST(UserConfig2 & FLASH_OPTSR2_SRAM1_3_4_5_RST));

    /* Set value and mask for SRAM13_RST option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM1_3_4_5_RST);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM1_3_4_5_RST;
  }
#endif /* FLASH_OPTSR2_SRAM1_3_RST */

#if defined (FLASH_OPTSR2_SRAM1_RST)
  if ((UserType & OB_USER_SRAM1_RST) != 0U)
  {
    /* SRAM1_RST option byte should be modified */
    assert_param(IS_OB_USER_SRAM1_RST(UserConfig2 & FLASH_OPTSR2_SRAM1_RST));

    /* Set value and mask for SRAM1_RST option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM1_RST);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM1_RST;
  }
#endif /* FLASH_OPTSR2_SRAM1_RST */

  if ((UserType & OB_USER_SRAM2_RST) != 0U)
  {
    /* SRAM2_RST option byte should be modified */
    assert_param(IS_OB_USER_SRAM2_RST(UserConfig2 & FLASH_OPTSR2_SRAM2_RST));

    /* Set value and mask for SRAM2_RST option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM2_RST);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM2_RST;
  }

  if ((UserType & OB_USER_BKPRAM_ECC) != 0U)
  {
    /* BKPRAM_ECC option byte should be modified */
    assert_param(IS_OB_USER_BKPRAM_ECC(UserConfig2 & FLASH_OPTSR2_BKPRAM_ECC));

    /* Set value and mask for BKPRAM_ECC option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_BKPRAM_ECC);
    optr_reg2_mask |= FLASH_OPTSR2_BKPRAM_ECC;
  }

#if defined (FLASH_OPTSR2_SRAM3_ECC)
  if ((UserType & OB_USER_SRAM3_ECC) != 0U)
  {
    /* SRAM3_ECC option byte should be modified */
    assert_param(IS_OB_USER_SRAM3_ECC(UserConfig2 & FLASH_OPTSR2_SRAM3_ECC));

    /* Set value and mask for SRAM3_ECC option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM3_ECC);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM3_ECC;
  }
#endif /* FLASH_OPTSR2_SRAM3_ECC */

  if ((UserType & OB_USER_SRAM2_ECC) != 0U)
  {
    /* SRAM2_ECC option byte should be modified */
    assert_param(IS_OB_USER_SRAM2_ECC(UserConfig2 & FLASH_OPTSR2_SRAM2_ECC));

    /* Set value and mask for SRAM2_ECC option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM2_ECC);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM2_ECC;
  }

#if defined (FLASH_OPTSR2_SRAM1_ECC)
  if ((UserType & OB_USER_SRAM1_ECC) != 0U)
  {
    /* SRAM2_ECC option byte should be modified */
    assert_param(IS_OB_USER_SRAM1_ECC(UserConfig2 & FLASH_OPTSR2_SRAM1_ECC));

    /* Set value and mask for SRAM2_ECC option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_SRAM1_ECC);
    optr_reg2_mask |= FLASH_OPTSR2_SRAM1_ECC;
  }
#endif /* FLASH_OPTSR2_SRAM1_ECC */

#if defined (FLASH_OPTSR2_USBPD_DIS)
  if ((UserType & OB_USER_USBPD_DIS) != 0U)
  {
    /* USBPD_DIS option byte should be modified */
    assert_param(IS_OB_USER_USBPD_DIS(UserConfig2 & FLASH_OPTSR2_USBPD_DIS));

    /* Set value and mask for USBPD_DIS option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_USBPD_DIS);
    optr_reg2_mask |= FLASH_OPTSR2_USBPD_DIS;
  }
#endif /* FLASH_OPTSR2_USBPD_DIS */

#if defined (FLASH_OPTSR2_TZEN)
  if ((UserType & OB_USER_TZEN) != 0U)
  {
    /* TZEN option byte should be modified */
    assert_param(IS_OB_USER_TZEN(UserConfig2 & FLASH_OPTSR2_TZEN));

    /* Set value and mask for TZEN option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_TZEN);
    optr_reg2_mask |= FLASH_OPTSR2_TZEN;
  }
#endif /* FLASH_OPTSR2_TZEN */
#if defined(FLASH_OPTSR2_HUK_PUF)
  if ((UserType & OB_USER_HUK_PUF) != 0U)
  {
    /* TZEN option byte should be modified */
    assert_param(IS_OB_USER_HUK_PUF(UserConfig2 & FLASH_OPTSR2_HUK_PUF));

    /* Set value and mask for TZEN option byte */
    optr_reg2_val |= (UserConfig2 & FLASH_OPTSR2_HUK_PUF);
    optr_reg2_mask |= FLASH_OPTSR2_HUK_PUF;
  }
#endif /* FLASH_OPTSR2_HUK_PUF */

  /* Check to write first User OB register or/and second one */
  if ((UserType & 0xFFFU) != 0U)
  {
    /* Configure the option bytes register */
    MODIFY_REG(FLASH->OPTSR_PRG, optr_reg1_mask, optr_reg1_val);
  }
  if ((UserType & 0x1FF000U) != 0U)
  {
    /* Configure the option bytes register */
    MODIFY_REG(FLASH->OPTSR2_PRG, optr_reg2_mask, optr_reg2_val);
  }
}

/**
  * @brief  Return the FLASH User Option Byte values.
  * @param UserConfig1 FLASH User Option Bytes values
  *         2M: IWDG_SW(Bit3), WWDG_SW(Bit4), nRST_STOP(Bit 6), nRST_STDY(Bit 7),
  *         PRODUCT_STATE(Bit[8:15]), IO_VDD_HSLV(Bit 16), IO_VDDTO2_HSLV(Bit 17),
  *         IWDG_STOP(Bit 20), IWDG_STDBY (Bit 21), BOOT_UBE(Bit[22:29]) and SWAP_BANK(Bit 31).
  *         128K: IWDG_SW(Bit3), WWDG_SW(Bit4), nRST_STOP(Bit 6), nRST_STDY(Bit 7),
  *         PRODUCT_STATE(Bit[8:15]), IO_VDD_HSLV(Bit16), IO_VDDIO2_HSLV(Bit17), IWDG_STOP(Bit 20),
  *         IWDG_STDBY (Bit 21) and SWAP_BANK(Bit 31).
  * @param UserConfig2 FLASH User Option Bytes values
  *         2M: SRAM1_3_RST(Bit2), SRAM2_RST(Bit 3), BKPRAM_ECC(Bit 4), SRAM3_ECC(Bit 5),
  *         SRAM2_ECC(Bit 6).
  *         128K: SRAM2_RST(Bit 3), BKPRAM_ECC(Bit 4), SRAM2_ECC(Bit 6),
  *         SRAM1_RST(Bit9), SRAM1_ECC(Bit10).
  * @retval None
  */
static void FLASH_OB_GetUser(uint32_t *UserConfig1, uint32_t *UserConfig2)
{
  (*UserConfig1) = FLASH->OPTSR_CUR & (~FLASH_OPTSR_PRODUCT_STATE);

  (*UserConfig2) = FLASH->OPTSR2_CUR;
}

/**
  * @brief  Configure Boot address
  * @param  BootOption specifies the Boot address option byte to be programmed.
  *         This parameter can be one of the following values:
  *            @arg OB_BOOTADDR_NS: Non-secure boot address
  *            @arg OB_BOOTADDR_SEC: Secure boot address
  * @param  BootAddress: specifies the boot address value
  *         This parameter can be sector number between 0 and 0xFFFFFF00
  * @retval None
  */
static void FLASH_OB_BootAddrConfig(uint32_t BootOption, uint32_t BootAddress)
{
  /* Check the parameters */
  assert_param(IS_OB_BOOT_CONFIG(BootOption));

  if (BootOption == OB_BOOT_NS)
  {
    MODIFY_REG(FLASH->NSBOOTR_PRG, FLASH_BOOTR_BOOTADD, BootAddress);
  }
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  else if (BootOption == OB_BOOT_SEC)
  {
    MODIFY_REG(FLASH->SECBOOTR_PRG, FLASH_BOOTR_BOOTADD, BootAddress);
  }
#endif /* __ARM_FEATURE_CMSE */
  else
  {
    /* Empty statement (to be compliant MISRA 15.7) */
  }
}

/**
  * @brief  Configure the boot lock.
  *
  * @param  BootOption select the BOOT_LOCK option: secure or non-secure.
  *          This parameter can be one of the following values:
  *            @arg OB_BOOT_LOCK_SEC: Boot Lock mode deactivated
  *            @arg OB_BOOT_LOCK_NS: Boot Lock mode activated
  *
  * @param  BootLockConfig specifies the activation of the BOOT_LOCK.
  *          This parameter can be one of the following values:
  *            @arg OB_BOOT_LOCK_DISABLE: Boot Lock mode deactivated
  *            @arg OB_BOOT_LOCK_ENABLE: Boot Lock mode activated
  *
  * @retval None
  */
static void FLASH_OB_BootLockConfig(uint32_t BootOption, uint32_t BootLockConfig)
{
  /* Check the parameters */
  assert_param(IS_OB_BOOT_CONFIG(BootOption));
  assert_param(IS_OB_BOOT_LOCK(BootLockConfig));

  /* Configure the option bytes register */
  if (BootOption == OB_BOOT_NS)
  {
    MODIFY_REG(FLASH->NSBOOTR_PRG, FLASH_BOOTR_BOOT_LOCK, BootLockConfig);
  }
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  else if (BootOption == OB_BOOT_SEC)
  {
    MODIFY_REG(FLASH->SECBOOTR_PRG, FLASH_BOOTR_BOOT_LOCK, BootLockConfig);
  }
#endif /* __ARM_FEATURE_CMSE */
  else
  {
    /* Empty statement (to be compliant MISRA 15.7) */
  }
}

/**
  * @brief  Get the boot configuration
  * @param[in]  BootOption specifies the boot address option byte to be returned.
  *             This parameter can be one of the following values:
  *                @arg OB_BOOT_NS: Non-secure boot address
  *                @arg OB_BOOT_SEC: Secure boot address
  *
  * @param[out]  BootAddress specifies the boot address value
  *
  * @param[out] BootLockConfig returns the activation of the BOOT_LOCK.
  *             This parameter can be one of the following values:
  *               @arg OB_BOOT_LOCK_DISABLE: Boot Lock mode deactivated
  *               @arg OB_BOOT_LOCK_ENABLE: Boot Lock mode activated
  * @retval None
  */
static void FLASH_OB_GetBootConfig(uint32_t BootOption, uint32_t *BootAddress, uint32_t *BootLockConfig)
{
  if (BootOption == OB_BOOT_NS)
  {
    *BootAddress    = FLASH->NSBOOTR_CUR & FLASH_BOOTR_BOOTADD;
    *BootLockConfig = FLASH->NSBOOTR_CUR & FLASH_BOOTR_BOOT_LOCK;
  }
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  else if (BootOption == OB_BOOT_SEC)
  {
    *BootAddress    = (FLASH->SECBOOTR_CUR & FLASH_BOOTR_BOOTADD);
    *BootLockConfig = (FLASH->SECBOOTR_CUR & FLASH_BOOTR_BOOT_LOCK);
  }
#endif /* __ARM_FEATURE_CMSE */
  else
  {
    /* Empty statement (to be compliant MISRA 15.7) */
  }
}

/**
  * @brief  Configure the OTP Block Lock.
  * @param  OTP_Block specifies the OTP Block to lock.
  *         This parameter can be a value of @ref FLASH_OTP_Blocks
  * @retval None
  */
static void FLASH_OB_OTP_LockConfig(uint32_t OTP_Block)
{
  /* Configure the OTP Block lock in the option bytes register */
  FLASH->OTPBLR_PRG |= OTP_Block;
}

/**
  * @brief  Get the OTP Block Lock.
  * @retval OTP_Block specifies the OTP Block to lock.
  *         This return value can be a value of @ref FLASH_OTP_Blocks
  */
static uint32_t FLASH_OB_OTP_GetLock(void)
{
  return (FLASH->OTPBLR_CUR);
}

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
/**
  * @brief  Configure the watermark-based secure area.
  *
  * @param  Banks specifies the bank where to apply Watermark protection
  *         This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: configure Watermark on bank1
  *            @arg FLASH_BANK_2: configure Watermark on bank2
  *            @arg FLASH_BANK_BOTH: configure Watermark on both bank1 and bank2
  *
  * @param  WMSecStartSector specifies the start sector of the secure area
  *         This parameter can be sector number between 0 and (max number of sectors in the bank - 1)
  *
  * @param  WMSecEndSector specifies the end sector of the secure area
  *         This parameter can be sector number between WMSecStartSector and WMSecEndSector(max number of sectors
  *         in the bank - 1)
  *
  * @retval None
  */
static void FLASH_OB_WMSECConfig(uint32_t Banks, uint32_t WMSecStartSector, uint32_t WMSecEndSector)
{
  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));
  assert_param(IS_FLASH_SECTOR(WMSecStartSector));
  assert_param(IS_FLASH_SECTOR(WMSecEndSector));

  /* Write SECWM registers */
  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    /* Configure Watermark Protection for bank 1 */
    FLASH->SECWM1R_PRG = ((WMSecEndSector << FLASH_SECWMR_SECWM_END_Pos) | WMSecStartSector);
  }

  if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
  {
    /* Configure Watermark Protection for bank 2 */
    FLASH->SECWM2R_PRG = ((WMSecEndSector << FLASH_SECWMR_SECWM_END_Pos) | WMSecStartSector);
  }
}

/**
  * @brief  Return the watermark-based secure area configuration.
  *
  * @param  Bank [in] specifies the bank where to get the watermark protection.
  *         This parameter can be exclusively one of the following values:
  *         @arg FLASH_BANK_1: Get bank1 watermark configuration
  *         @arg FLASH_BANK_2: Get bank2 watermark configuration
  *
  * @param  WMSecStartSector [out] specifies the start sector of the secure area
  *
  * @param  WMSecEndSector [out] specifies the end sector of the secure area
  *
  * @retval None
  */
static void FLASH_OB_GetWMSEC(uint32_t Bank, uint32_t *WMSecStartSector, uint32_t *WMSecEndSector)
{
  uint32_t regvalue = 0U;

  /* Read SECWM register */
  if (Bank == FLASH_BANK_1)
  {
    regvalue = FLASH->SECWM1R_CUR;
  }

  if (Bank == FLASH_BANK_2)
  {
    regvalue = FLASH->SECWM2R_CUR;
  }

  /* Get configuration of secure area */
  *WMSecStartSector = (regvalue & FLASH_SECWMR_SECWM_STRT);
  *WMSecEndSector = ((regvalue & FLASH_SECWMR_SECWM_END) >> FLASH_SECWMR_SECWM_END_Pos);
}
#endif /* __ARM_FEATURE_CMSE */

/**
  * @brief  Configure the hide protection area.
  *
  * @param  Banks specifies the bank where to apply hide protection
  *         This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: configure HDP on bank1
  *            @arg FLASH_BANK_2: configure HDP on bank2
  *            @arg FLASH_BANK_BOTH: configure HDP on both bank1 and bank2
  *
  * @param  HDPStartSector specifies the start sector of the hide protection area
  *         This parameter can be sector number between 0 and (max number of sectors in the bank - 1)
  *
  * @param  HDPEndSector specifies the end sector of the hide protection area
  *         This parameter can be sector number between HDPStartSector and HDPEndSector (max number of sectors
  *         in the bank - 1)
  *
  * @retval None
  */
static void FLASH_OB_HDPConfig(uint32_t Banks, uint32_t HDPStartSector, uint32_t HDPEndSector)
{
  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));
  assert_param(IS_FLASH_SECTOR(HDPStartSector));
  assert_param(IS_FLASH_SECTOR(HDPEndSector));

  /* Write HDP registers */
  if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
  {
    /* Configure hide Protection for bank 1 */
    FLASH->HDP1R_PRG = ((HDPEndSector << FLASH_HDPR_HDP_END_Pos) | HDPStartSector);
  }

  if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
  {
    /* Configure hide Protection for bank 2 */
    FLASH->HDP2R_PRG = ((HDPEndSector << FLASH_HDPR_HDP_END_Pos) | HDPStartSector);
  }
}

/**
  * @brief  Return the hide protection area configuration.
  *
  * @param  Bank [in] specifies the bank where to get the HDP protection.
  *         This parameter can be exclusively one of the following values:
  *         @arg FLASH_BANK_1: Get bank1 HDP configuration
  *         @arg FLASH_BANK_2: Get bank2 HDP configuration
  *
  * @param  HDPStartSector [out] specifies the start sector of the HDP area
  *
  * @param  HDPEndSector [out] specifies the end sector of the HDP area
  *
  * @retval None
  */
static void FLASH_OB_GetHDP(uint32_t Bank, uint32_t *HDPStartSector, uint32_t *HDPEndSector)
{
  uint32_t regvalue = 0U;

  /* Read SECWM register */
  if (Bank == FLASH_BANK_1)
  {
    regvalue = FLASH->HDP1R_CUR;
  }

  if (Bank == FLASH_BANK_2)
  {
    regvalue = FLASH->HDP2R_CUR;
  }

  /* Get configuration of HDP area */
  *HDPStartSector = (regvalue & FLASH_HDPR_HDP_STRT);
  *HDPEndSector = ((regvalue & FLASH_HDPR_HDP_END) >> FLASH_HDPR_HDP_END_Pos);
}

#if defined(FLASH_EDATAR_EDATA_EN)
/**
  * @brief  Configure the Flash high-cycle area.
  *
  * @param  Banks specifies the bank where to apply Flash high-cycle data area
  *          This parameter can be one of the following values:
  *            @arg FLASH_BANK_1: configure Flash high-cycle area on bank1
  *            @arg FLASH_BANK_2: configure Flash high-cycle area on bank2
  *            @arg FLASH_BANK_BOTH: configure Flash high-cycle area on both bank1 and bank2
  *
  * @param  EDATASize specifies the size (in sectors) of the Flash high-cycle data area
  *         This parameter can be sectors number between 0 and 8
  *         0: Disable all EDATA sectors.
  *         1: The last sector is reserved for flash high-cycle data.
  *         2: The two last sectors are reserved for flash high-cycle data.
  *         3: The three last sectors are reserved for flash high-cycle data
  *         4: The four last sectors is reserved for flash high-cycle data.
  *         5: The five last sectors are reserved for flash high-cycle data.
  *         6: The six last sectors are reserved for flash high-cycle data.
  *         7: The seven last sectors are reserved for flash high-cycle data.
  *         8: The eight last sectors are reserved for flash high-cycle data.
  *
  * @retval None
  */
static void FLASH_OB_EDATAConfig(uint32_t Banks, uint32_t EDATASize)
{
  /* Check the parameters */
  assert_param(IS_FLASH_BANK(Banks));
  assert_param(IS_FLASH_EDATA_SIZE(EDATASize));

  if (EDATASize != 0U)
  {
    /* Write EDATA registers */
    if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
    {
      /* Configure Flash high-cycle data for bank 1 */
      FLASH->EDATA1R_PRG = (FLASH_EDATAR_EDATA_EN | (EDATASize - 1U));
    }

    if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
    {
      /* Configure Flash high-cycle data for bank 2 */
      FLASH->EDATA2R_PRG = (FLASH_EDATAR_EDATA_EN | (EDATASize - 1U));
    }
  }
  else
  {
    /* Write EDATA registers */
    if ((Banks & FLASH_BANK_1) == FLASH_BANK_1)
    {
      /* Disable Flash high-cycle data for bank 1 */
      FLASH->EDATA1R_PRG = 0U;
    }

    if ((Banks & FLASH_BANK_2) == FLASH_BANK_2)
    {
      /* Disable Flash high-cycle data for bank 2 */
      FLASH->EDATA2R_PRG = 0U;
    }
  }
}

/**
  * @brief  Return the Flash high-cycle data area configuration.
  *
  * @param  Bank [in] specifies the bank where to get the Flash high-cycle data configuration.
  *         This parameter can be exclusively one of the following values:
  *         @arg FLASH_BANK_1: Get bank1 Flash high-cycle data configuration
  *         @arg FLASH_BANK_2: Get bank2 Flash high-cycle data configuration
  *
  * @param  EDATASize [out] specifies the size (in sectors) of the Flash high-cycle data area
  *
  * @retval None
  */
static void FLASH_OB_GetEDATA(uint32_t Bank, uint32_t *EDATASize)
{
  uint32_t regvalue = 0U;

  /* Read SECWM register */
  if (Bank == FLASH_BANK_1)
  {
    regvalue = FLASH->EDATA1R_CUR;
  }

  if (Bank == FLASH_BANK_2)
  {
    regvalue = FLASH->EDATA2R_CUR;
  }

  /* Get configuration of secure area */
  if ((regvalue & FLASH_EDATAR_EDATA_EN) != 0U)
  {
    /* Encoding of Edata Area size is register value + 1 */
    *EDATASize = (regvalue & FLASH_EDATAR_EDATA_STRT) + 1U;
  }
  else
  {
    /* No defined Edata area */
    *EDATASize = 0U;
  }

}
#endif /* FLASH_EDATAR_EDATA_EN */

/**
  * @}
  */

/** @defgroup FLASHEx_Exported_Functions_Group3 Extended ECC operation functions
  *  @brief   Extended ECC operation functions
  *
@verbatim
 ===============================================================================
                  ##### Extended ECC operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the Extended FLASH
    ECC Operations.

@endverbatim
  * @{
  */
/**
  * @brief  Enable ECC correction interrupt
  * @retval None
  */
void HAL_FLASHEx_EnableEccCorrectionInterrupt(void)
{
  __HAL_FLASH_ENABLE_IT(FLASH_IT_ECCC);
}

/**
  * @brief  Disable ECC correction interrupt
  * @retval None
  */
void HAL_FLASHEx_DisableEccCorrectionInterrupt(void)
{
  __HAL_FLASH_DISABLE_IT(FLASH_IT_ECCC);
}

/**
  * @brief  Get the ECC error information.
  * @param  pData Pointer to an FLASH_EccInfoTypeDef structure that contains the
  *         ECC error information.
  * @note   This function should be called before ECC bit is cleared
  *         (in callback function)
  * @retval None
  */
void HAL_FLASHEx_GetEccInfo(FLASH_EccInfoTypeDef *pData)
{
  uint32_t correction_reg = FLASH->ECCCORR;
  uint32_t detection_reg = FLASH->ECCDETR;
  uint32_t data_reg = FLASH->ECCDR;
  uint32_t addr_reg = 0xFFFFFFFFU;

  /* Check if the operation is a correction or a detection*/
  if ((correction_reg & FLASH_ECCR_ECCC) != 0U)
  {
    /*  Get area and offset address values from ECCCORR register*/
    pData->Area = correction_reg & (~(FLASH_ECCR_ECCIE | FLASH_ECCR_ADDR_ECC | FLASH_ECCR_ECCC));
    addr_reg = (correction_reg & FLASH_ECCR_ADDR_ECC);
  }
  else if ((detection_reg & FLASH_ECCR_ECCD) != 0U)
  {
    /* Get area and offset address values from ECCDETR register */
    pData->Area = detection_reg & (~(FLASH_ECCR_ADDR_ECC | FLASH_ECCR_ECCD));
    addr_reg = (detection_reg & FLASH_ECCR_ADDR_ECC);
  }
  else
  {
    /* Do nothing */
  }

  /* Check that an ECC single or double error has occurred to continue the calculation of area address */
  if (addr_reg != 0xFFFFFFFFU)
  {
    /* Get address value according to area value*/
    switch (pData->Area)
    {
      case FLASH_ECC_AREA_USER_BANK1:
        /*
         * One error detection/correction or two error detections per 128-bit flash word
         * Therefore, the address returned by ECC registers in bank1 represents 128-bit flash word,
         * to get the correct address value, we must do a shift by 4 bits
        */
        addr_reg = ((uint32_t)addr_reg << 4U) & 0xFFFFFFFFU;
        pData->Address = (FLASH_BASE + addr_reg) & 0xFFFFFFFFU;
        break;
      case FLASH_ECC_AREA_USER_BANK2:
        /*
         * One error detection/correction or two error detections per 128-bit flash word
         * Therefore, the address returned by ECC registers in bank2 represents 128-bit flash word,
         * to get the correct address value, we must do a shift by 4 bits
        */
        addr_reg = ((uint32_t)addr_reg << 4U) & 0xFFFFFFFFU;
        pData->Address = (FLASH_BASE + FLASH_BANK_SIZE + addr_reg) & 0xFFFFFFFFU;
        break;
      case FLASH_ECC_AREA_SYSTEM:
        /* check system flash bank */
        if ((correction_reg & FLASH_ECCR_BK_ECC) == FLASH_ECCR_BK_ECC)
        {
          pData->Address = (FLASH_SYSTEM_BASE + FLASH_SYSTEM_SIZE + addr_reg) & 0xFFFFFFFFU;
        }
        else
        {
          pData->Address = (FLASH_SYSTEM_BASE + addr_reg) & 0xFFFFFFFFU;
        }
        break;
#if defined (FLASH_SR_OBKERR)
      case FLASH_ECC_AREA_OBK:
        pData->Address = (FLASH_OBK_BASE + addr_reg) & 0xFFFFFFFFU;
        break;
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
      case FLASH_ECC_AREA_EDATA_BANK1:
        /* check flash high-cycle data bank */
          /*
           * addr_reg is the address returned by the ECC register along with an offset value depends on area
           * To calculate the exact address set by user while an ECC occurred, we must subtract the offset value,
           * In addition, the address returned by ECC registers represents 128-bit flash word (multiply by 4),
          */
          pData->Address = (FLASH_EDATA_BASE + ((addr_reg - FLASH_ADDRESS_OFFSET_EDATA) * 4U)) & 0xFFFFFFFFU;
        break;
      case FLASH_ECC_AREA_EDATA_BANK2:
        /* check flash high-cycle data bank */
          /*
           * addr_reg is the address returned by the ECC register along with an offset value depends on area
           * To calculate the exact address set by user while an ECC occurred, we must subtract the offset value,
           * In addition, the address returned by ECC registers represents 128-bit flash word (multiply by 4),
          */
          pData->Address = (FLASH_EDATA_BASE + FLASH_EDATA_BANK_SIZE + \
                           ((addr_reg - FLASH_ADDRESS_OFFSET_EDATA) * 4U)) & 0xFFFFFFFFU;
        break;
#endif /* FLASH_EDATAR_EDATA_EN */
      case FLASH_ECC_AREA_OTP:
        /* Address returned by the ECC is an halfword, multiply by 4 to get the exact address*/
        pData->Address = (FLASH_OTP_BASE + ((addr_reg - FLASH_ADDRESS_OFFSET_OTP) * 4U)) & 0xFFFFFFFFU;
        break;

      default:
        /* Do nothing */
        break;
    }
  }

  pData->Data = data_reg & FLASH_ECCR_ADDR_ECC;
}

/**
  * @brief Handle Flash ECC Detection interrupt request.
  * @retval None
  */
void HAL_FLASHEx_ECCD_IRQHandler(void)
{
  /* Check if the ECC double error occurred*/
  if (READ_BIT(FLASH->ECCDETR, FLASH_ECCR_ECCD) != 0U)
  {
    /* FLASH ECC detection user callback */
    HAL_FLASHEx_EccDetectionCallback();

    /* Clear ECCD flag
    note : this step will clear all the information related to the flash ecc detection
    */
    SET_BIT(FLASH->ECCDETR, FLASH_ECCR_ECCD);
  }
}

/**
  * @brief  FLASH ECC Correction interrupt callback.
  * @retval None
  */
__weak void HAL_FLASHEx_EccCorrectionCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FLASHEx_EccCorrectionCallback could be implemented in the user file
   */
}

/**
  * @brief  FLASH ECC Detection interrupt callback.
  * @retval None
  */
__weak void HAL_FLASHEx_EccDetectionCallback(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_FLASHEx_EccDetectionCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/**
  * @}
  */
#endif /* HAL_FLASH_MODULE_ENABLED */

/**
  * @}
  */

/**
  * @}
  */
