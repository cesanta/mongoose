/**
  ******************************************************************************
  * @file    stm32h5xx_hal_flash.c
  * @author  MCD Application Team
  * @brief   FLASH HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the internal FLASH memory:
  *           + Program operations functions
  *           + Memory Control functions
  *           + Peripheral Errors functions
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
                        ##### FLASH peripheral features #####
  ==============================================================================

  [..] The Flash memory interface manages CPU AHB C-Bus accesses to the Flash memory.
       It implements the erase and program Flash memory operations and the read
       and write protection mechanisms.

  [..] The FLASH main features are:
      (+) Flash memory read operations
      (+) Flash memory program/erase operations
      (+) Read / write protections
      (+) Option bytes programming
      (+) TrustZone aware
      (+) Watermark-based area protection
      (+) Block-based sector protection
      (+) Error code correction (ECC)


                        ##### How to use this driver #####
 ==============================================================================
    [..]
      This driver provides functions and macros to configure and program the FLASH
      memory of all STM32H5xx devices.

      (#) FLASH Memory IO Programming functions:
           (++) Lock and Unlock the FLASH interface using HAL_FLASH_Unlock() and
                HAL_FLASH_Lock() functions
           (++) Flash memory programming by 128 bits (user area, OBKeys) and 16 bits (OTP and Flash high-cycle
                data area)
           (++) There Two modes of programming :
            (+++) Polling mode using HAL_FLASH_Program() function
            (+++) Interrupt mode using HAL_FLASH_Program_IT() function

      (#) Interrupts and flags management functions :
           (++) Handle FLASH interrupts by calling HAL_FLASH_IRQHandler()
           (++) Callback functions are called when the flash operations are finished :
                HAL_FLASH_EndOfOperationCallback() when everything is ok, otherwise
                HAL_FLASH_OperationErrorCallback()
           (++) Get error flag status by calling HAL_FLASH_GetError()

      (#) Option bytes management functions :
           (++) Lock and Unlock the option bytes using HAL_FLASH_OB_Unlock() and
                HAL_FLASH_OB_Lock() functions
           (++) Launch the reload of the option bytes using HAL_FLASH_OB_Launch() function.
                In this case, a reset is generated
    [..]
      In addition to these functions, this driver includes a set of macros allowing
      to handle the following operations:
       (+) Set the latency
       (+) Enable/Disable the FLASH interrupts
       (+) Monitor the FLASH flags status
     [..]
    (@) The contents of the Flash memory are not guaranteed if a device reset occurs during
        a Flash memory operation.

 @endverbatim
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h5xx_hal.h"

/** @addtogroup STM32H5xx_HAL_Driver
  * @{
  */

/** @defgroup FLASH FLASH
  * @brief FLASH HAL module driver
  * @{
  */

#ifdef HAL_FLASH_MODULE_ENABLED

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @defgroup FLASH_Private_Variables FLASH Private Variables
  * @{
  */
/**
  * @brief  Variable used for Program/Erase sectors under interruption
  */
FLASH_ProcessTypeDef pFlash = {.Lock = HAL_UNLOCKED, \
                               .ErrorCode = HAL_FLASH_ERROR_NONE, \
                               .ProcedureOnGoing = 0U, \
                               .Address = 0U, \
                               .Bank = FLASH_BANK_1, \
                               .Sector = 0U, \
                               .NbSectorsToErase = 0U
                              };
/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup FLASH_Private_Functions FLASH Private Functions
  * @{
  */
static void          FLASH_Program_QuadWord(uint32_t FlashAddress, uint32_t DataAddress);
#if defined (FLASH_SR_OBKERR)
static void          FLASH_Program_QuadWord_OBK(uint32_t FlashAddress, uint32_t DataAddress);
#endif /* FLASH_SR_OBKERR */
static void          FLASH_Program_HalfWord(uint32_t FlashAddress, uint32_t DataAddress);
#if defined(FLASH_EDATAR_EDATA_EN)
static void          FLASH_Program_Word(uint32_t FlashAddress, uint32_t DataAddress);
#endif /* FLASH_EDATAR_EDATA_EN */

/**
  * @}
  */
/* Exported functions ---------------------------------------------------------*/

/** @defgroup FLASH_Exported_Functions FLASH Exported functions
  * @{
  */

/** @defgroup FLASH_Exported_Functions_Group1 Programming operation functions
  *  @brief   Programming operation functions
  *
@verbatim
 ===============================================================================
                  ##### Programming operation functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to manage the FLASH
    program operations.

@endverbatim
  * @{
  */

/**
  * @brief  Program a quad-word at a specified address.
  * @param  TypeProgram Indicate the way to program at a specified address.
  *         This parameter can be a value of @ref FLASH_Type_Program
  * @param  FlashAddress specifies the address to be programmed.
  *         This parameter shall be aligned to the Flash word (128-bit)
  * @param  DataAddress specifies the address of data to be programmed
  *         This parameter shall be 32-bit aligned
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress)
{
  HAL_StatusTypeDef status;
  __IO uint32_t *reg_cr;
#if defined (FLASH_SR_OBKERR)
  __IO uint32_t *reg_obkcfgr;
#endif /* FLASH_SR_OBKERR */

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEPROGRAM(TypeProgram));

  /* Reset error code */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status == HAL_OK)
  {
    /* Set current operation type */
    pFlash.ProcedureOnGoing = TypeProgram;

    /* Access to SECCR or NSCR depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
    reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
    reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

    if ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_QUADWORD)
    {
      /* Check the parameters */
      assert_param(IS_FLASH_USER_MEM_ADDRESS(FlashAddress));

      /* Program a quad-word (128-bit) at a specified address */
      FLASH_Program_QuadWord(FlashAddress, DataAddress);
    }
#if defined (FLASH_SR_OBKERR)
    else if ((TypeProgram == FLASH_TYPEPROGRAM_QUADWORD_OBK) || (TypeProgram == FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT))
    {
      /* Check the parameters */
      assert_param(IS_FLASH_OBK_ADDRESS(FlashAddress));

      /* Program a quad-word (128-bit) of OBK at a specified address */
      FLASH_Program_QuadWord_OBK(FlashAddress, DataAddress);
    }
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
    else if ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_HALFWORD_EDATA)
    {
      /* Check the parameters */
      assert_param(IS_FLASH_EDATA_ADDRESS(FlashAddress));

      /* Program a Flash high-cycle data half-word at a specified address */
      FLASH_Program_HalfWord(FlashAddress, DataAddress);
    }
    else if ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_WORD_EDATA)
    {
      /* Check the parameters */
      assert_param(IS_FLASH_EDATA_ADDRESS(FlashAddress));

      /* Program a Flash high-cycle data half-word at a specified address */
      FLASH_Program_Word(FlashAddress, DataAddress);
    }
#endif /* FLASH_EDATAR_EDATA_EN */
    else
    {
      /* Check the parameters */
      assert_param(IS_FLASH_OTP_ADDRESS(FlashAddress));

      /* Program an OTP half-word at a specified address */
      FLASH_Program_HalfWord(FlashAddress, DataAddress);
    }

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

#if defined (FLASH_SR_OBKERR)
    /* If the program operation is completed, disable the PG */
    CLEAR_BIT((*reg_cr), (TypeProgram & ~(FLASH_NON_SECURE_MASK | FLASH_OBK | FLASH_OTP | FLASH_OBKCFGR_ALT_SECT)));

    /* Clear alternate sector bit */
    if (TypeProgram == FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT)
    {
      reg_obkcfgr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECOBKCFGR) : &(FLASH_NS->NSOBKCFGR);
      CLEAR_BIT((*reg_obkcfgr), FLASH_OBKCFGR_ALT_SECT);
    }
#else
    /* If the program operation is completed, disable the PG */
    CLEAR_BIT((*reg_cr), (TypeProgram & ~(FLASH_NON_SECURE_MASK |  FLASH_OTP)));
#endif /* FLASH_SR_OBKERR */
  }
  /* return status */
  return status;
}

/**
  * @brief  Program a quad-word at a specified address with interrupt enabled.
  * @param  TypeProgram Indicate the way to program at a specified address.
  *                      This parameter can be a value of @ref FLASH_Type_Program
  * @param  FlashAddress specifies the address to be programmed.
  *         This parameter shall be aligned to the Flash word (128-bit)
  * @param  DataAddress specifies the address of data to be programmed
  *         This parameter shall be 32-bit aligned
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t FlashAddress, uint32_t DataAddress)
{
  HAL_StatusTypeDef status;
  __IO uint32_t *reg_cr;

  /* Check the parameters */
  assert_param(IS_FLASH_TYPEPROGRAM(TypeProgram));

  /* Reset error code */
  pFlash.ErrorCode = HAL_FLASH_ERROR_NONE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  if (status != HAL_OK)
  {
    /* Process Unlocked */
    __HAL_UNLOCK(&pFlash);
  }
  else
  {
    /* Set internal variables used by the IRQ handler */
    pFlash.ProcedureOnGoing = TypeProgram;
    pFlash.Address = FlashAddress;

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

    if ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_QUADWORD)
    {
      /* Check the parameters */
      assert_param(IS_FLASH_USER_MEM_ADDRESS(FlashAddress));

      /* Program a quad-word (128-bit) at a specified address */
      FLASH_Program_QuadWord(FlashAddress, DataAddress);
    }
#if defined (FLASH_SR_OBKERR)
    else if (((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_QUADWORD_OBK) || \
             ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_QUADWORD_OBK_ALT))
    {
      /* Check the parameters */
      assert_param(IS_FLASH_OBK_ADDRESS(FlashAddress));

      /* Program a quad-word (128-bit) of OBK at a specified address */
      FLASH_Program_QuadWord_OBK(FlashAddress, DataAddress);
    }
#endif /* FLASH_SR_OBKERR */
#if defined (FLASH_EDATAR_EDATA_EN)
    else if ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_HALFWORD_EDATA)
    {
      /* Check the parameters */
      assert_param(IS_FLASH_EDATA_ADDRESS(FlashAddress));

      /* Program a Flash high-cycle data half-word at a specified address */
      FLASH_Program_HalfWord(FlashAddress, DataAddress);
    }
     else if ((TypeProgram & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_WORD_EDATA)
    {
      /* Check the parameters */
      assert_param(IS_FLASH_EDATA_ADDRESS(FlashAddress));

      /* Program a Flash high-cycle data word at a specified address */
      FLASH_Program_Word(FlashAddress, DataAddress);
    }
#endif /* FLASH_EDATAR_EDATA_EN */
    else
    {
      /* Check the parameters */
      assert_param(IS_FLASH_OTP_ADDRESS(FlashAddress));

      /* Program an OTP word at a specified address */
      FLASH_Program_HalfWord(FlashAddress, DataAddress);
    }
  }

  /* return status */
  return status;
}

/**
  * @brief This function handles FLASH interrupt request.
  * @retval None
  */
void HAL_FLASH_IRQHandler(void)
{
  uint32_t param = 0U;
  uint32_t errorflag;
  __IO uint32_t *reg_cr;
  __IO uint32_t *reg_ccr;
  const __IO uint32_t *reg_sr;
  const __IO uint32_t *reg_ecccorr;

  /* Access to CR, CCR and SR registers depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
  reg_ccr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCCR) : &(FLASH_NS->NSCCR);
  reg_sr  = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECSR) : &(FLASH_NS->NSSR);
#else
  reg_cr = &(FLASH_NS->NSCR);
  reg_ccr = &(FLASH_NS->NSCCR);
  reg_sr = &(FLASH_NS->NSSR);
#endif /* FLASH_OPTSR2_TZEN */
  reg_ecccorr = &(FLASH->ECCCORR);

  /* Save Flash errors */
  errorflag = (*reg_sr) & FLASH_FLAG_SR_ERRORS;
  /* Add option byte error flag, if any */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  errorflag |= (FLASH->NSSR & FLASH_FLAG_OPTCHANGEERR);
#endif /* __ARM_FEATURE_CMSE */

  /* Set parameter of the callback */
  if ((pFlash.ProcedureOnGoing & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEERASE_SECTORS)
  {
    param = pFlash.Sector;
  }
  else if ((pFlash.ProcedureOnGoing & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEERASE_MASSERASE)
  {
    param = pFlash.Bank;
  }
  else if ((pFlash.ProcedureOnGoing & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEPROGRAM_QUADWORD)
  {
    param = pFlash.Address;
  }
  else
  {
    /* Empty statement (to be compliant MISRA 15.7) */
  }

  /* Clear operation bit on the on-going procedure */
  CLEAR_BIT((*reg_cr), (pFlash.ProcedureOnGoing & ~(FLASH_NON_SECURE_MASK)));

  /* Check FLASH operation error flags */
  if (errorflag != 0U)
  {
    /* Save the error code */
    pFlash.ErrorCode |= errorflag;

    /* Clear error programming flags */
    (*reg_ccr) = errorflag & FLASH_FLAG_SR_ERRORS;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if ((errorflag & FLASH_FLAG_OPTCHANGEERR) != 0U)
    {
      FLASH->NSCCR = FLASH_FLAG_OPTCHANGEERR;
    }
#endif /* __ARM_FEATURE_CMSE */

    /* Stop the procedure ongoing */
    pFlash.ProcedureOnGoing = 0U;

    /* FLASH error interrupt user callback */
    HAL_FLASH_OperationErrorCallback(param);
  }

  /* Check FLASH End of Operation flag */
  if (((*reg_sr) & FLASH_FLAG_EOP) != 0U)
  {
    /* Clear FLASH End of Operation pending bit */
    (*reg_ccr) = FLASH_FLAG_EOP;

    if ((pFlash.ProcedureOnGoing & (~FLASH_NON_SECURE_MASK)) == FLASH_TYPEERASE_SECTORS)
    {
      /* Nb of sector to erased can be decreased */
      pFlash.NbSectorsToErase--;

      /* Check if there are still sectors to erase */
      if (pFlash.NbSectorsToErase != 0U)
      {
        /* Increment sector number */
        pFlash.Sector++;
        FLASH_Erase_Sector(pFlash.Sector, pFlash.Bank);
      }
      else
      {
        /* No more sectors to erase */
        /* Reset sector parameter and stop erase sectors procedure */
        param = 0xFFFFFFFFU;
        pFlash.ProcedureOnGoing = 0U;
      }
    }
    else
    {
      /* Clear the procedure ongoing */
      pFlash.ProcedureOnGoing = 0U;
    }

    /* FLASH EOP interrupt user callback */
    HAL_FLASH_EndOfOperationCallback(param);
  }

  /* Check FLASH ECC correction flag */
  if ((*reg_ecccorr & FLASH_ECCR_ECCC) != 0U)
  {
    /* Call User callback */
    HAL_FLASHEx_EccCorrectionCallback();

    /* Clear ECC correction flag in order to allow new ECC error record */
    FLASH->ECCCORR |= FLASH_ECCR_ECCC;
  }

  if (pFlash.ProcedureOnGoing == 0U)
  {
    /* Disable Flash Operation and Error source interrupt */
#if defined (FLASH_SR_OBKERR)
    (*reg_cr) &= ~(FLASH_IT_EOP     | FLASH_IT_WRPERR | FLASH_IT_PGSERR | \
                   FLASH_IT_STRBERR | FLASH_IT_INCERR | FLASH_IT_OBKERR | \
                   FLASH_IT_OBKWERR | FLASH_IT_OPTCHANGEERR);
#else
    (*reg_cr) &= ~(FLASH_IT_EOP     | FLASH_IT_WRPERR | FLASH_IT_PGSERR | \
                   FLASH_IT_STRBERR | FLASH_IT_INCERR | FLASH_IT_OPTCHANGEERR);
#endif /* FLASH_SR_OBKERR */
  }
}

/**
  * @brief  FLASH end of operation interrupt callback
  * @param  ReturnValue The value saved in this parameter depends on the ongoing procedure
  *                  Mass Erase: Bank number which has been requested to erase
  *                  Sectors Erase: Sector which has been erased
  *                    (if 0xFFFFFFFF, it means that all the selected sectors have been erased)
  *                  Program: Address which was selected for data program
  * @retval None
  */
__weak void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ReturnValue);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FLASH_EndOfOperationCallback could be implemented in the user file
   */
}

/**
  * @brief  FLASH operation error interrupt callback
  * @param  ReturnValue The value saved in this parameter depends on the ongoing procedure
  *                 Mass Erase: Bank number which has been requested to erase
  *                 Sectors Erase: Sector number which returned an error
  *                 Program: Address which was selected for data program
  * @retval None
  */
__weak void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(ReturnValue);

  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_FLASH_OperationErrorCallback could be implemented in the user file
   */
}

/**
  * @}
  */

/** @defgroup FLASH_Exported_Functions_Group2 Peripheral Control functions
  *  @brief    Management functions
  *
@verbatim
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================
    [..]
    This subsection provides a set of functions allowing to control the FLASH
    memory operations.

@endverbatim
  * @{
  */

/**
  * @brief  Unlock the FLASH control registers access
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Unlock(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (READ_BIT(FLASH->NSCR, FLASH_CR_LOCK) != 0U)
  {
    /* Authorize the FLASH Control Register access */
    WRITE_REG(FLASH->NSKEYR, FLASH_KEY1);
    WRITE_REG(FLASH->NSKEYR, FLASH_KEY2);

    /* Verify Flash CR is unlocked */
    if (READ_BIT(FLASH->NSCR, FLASH_CR_LOCK) != 0U)
    {
      status = HAL_ERROR;
    }
  }

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  if (status == HAL_OK)
  {
    if (READ_BIT(FLASH->SECCR, FLASH_CR_LOCK) != 0U)
    {
      /* Authorize the FLASH Control Register access */
      WRITE_REG(FLASH->SECKEYR, FLASH_KEY1);
      WRITE_REG(FLASH->SECKEYR, FLASH_KEY2);

      /* verify Flash CR is unlocked */
      if (READ_BIT(FLASH->SECCR, FLASH_CR_LOCK) != 0U)
      {
        status = HAL_ERROR;
      }
    }
  }
#endif /* __ARM_FEATURE_CMSE */

  return status;
}

/**
  * @brief  Locks the FLASH control registers access
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_Lock(void)
{
  HAL_StatusTypeDef status = HAL_OK;

  /* Set the LOCK Bit to lock the FLASH Control Register access */
  SET_BIT(FLASH->NSCR, FLASH_CR_LOCK);

  /* Verify Flash is locked */
  if (READ_BIT(FLASH->NSCR, FLASH_CR_LOCK) == 0U)
  {
    status = HAL_ERROR;
  }

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  if (status == HAL_OK)
  {
    /* Set the LOCK Bit to lock the FLASH Control Register access */
    SET_BIT(FLASH->SECCR, FLASH_CR_LOCK);

    /* verify Flash is locked */
    if (READ_BIT(FLASH->SECCR, FLASH_CR_LOCK) == 0U)
    {
      status = HAL_ERROR;
    }
  }
#endif /* __ARM_FEATURE_CMSE */

  return status;
}

/**
  * @brief  Unlock the FLASH Option Control Registers access.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void)
{
  if (READ_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTLOCK) != 0U)
  {
    /* Authorizes the Option Byte registers programming */
    WRITE_REG(FLASH->OPTKEYR, FLASH_OPT_KEY1);
    WRITE_REG(FLASH->OPTKEYR, FLASH_OPT_KEY2);

    /* Verify that the Option Bytes are unlocked */
    if (READ_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTLOCK) != 0U)
    {
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

/**
  * @brief  Lock the FLASH Option Control Registers access.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_OB_Lock(void)
{
  /* Set the OPTLOCK Bit to lock the FLASH Option Byte Registers access */
  SET_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTLOCK);

  /* Verify that the Option Bytes are locked */
  if (READ_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTLOCK) != 0U)
  {
    return HAL_OK;
  }

  return HAL_ERROR;
}

/**
  * @brief  Launch the option bytes loading.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASH_OB_Launch(void)
{
  HAL_StatusTypeDef status;

  /* Set OPTSTRT Bit */
  SET_BIT(FLASH->OPTCR, FLASH_OPTCR_OPTSTART);

  /* Wait for OB change operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  return status;
}

#if defined(FLASH_CR_PUF_LAUNCH)
/**
  * @brief  Launch the PUF preparation.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_FLASHEx_PUF_Launch(void)
{
  HAL_StatusTypeDef status;

  /* Set PUF_LAUNCH Bit */
  SET_BIT(FLASH->NSCR, FLASH_CR_PUF_LAUNCH);

  /* Wait for OB change operation to be completed */
  status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

  return status;
}
#endif /* FLASH_CR_PUF_LAUNCH */

/**
  * @}
  */

/** @defgroup FLASH_Exported_Functions_Group3 Peripheral State and Errors functions
  *  @brief   Peripheral Errors functions
  *
@verbatim
 ===============================================================================
                ##### Peripheral Errors functions #####
 ===============================================================================
    [..]
    This subsection permits to get in run-time Errors of the FLASH peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Get the specific FLASH error flag.
  * @retval HAL_FLASH_ERRORCode The returned value can be:
  *            @arg HAL_FLASH_ERROR_NONE  : No error set
  *            @arg HAL_FLASH_ERROR_WRP   : Write Protection Error
  *            @arg HAL_FLASH_ERROR_PGS   : Program Sequence Error
  *            @arg HAL_FLASH_ERROR_STRB  : Strobe Error
  *            @arg HAL_FLASH_ERROR_INC   : Inconsistency Error
  *            @arg HAL_FLASH_ERROR_OBK   : OBK Error
  *            @arg HAL_FLASH_ERROR_OBKW  : OBK Write Error
  *            @arg HAL_FLASH_ERROR_OB_CHANGE : Option Byte Change Error
  *            @arg HAL_FLASH_ERROR_ECCC  : ECC Single Correction Error
  *            @arg HAL_FLASH_ERROR_ECCD  : ECC Double Detection Error
  */
uint32_t HAL_FLASH_GetError(void)
{
  return pFlash.ErrorCode;
}

/**
  * @}
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/

/** @addtogroup FLASH_Private_Functions
  * @{
  */

/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout maximum flash operation timeout
  * @retval HAL_StatusTypeDef HAL Status
  */
HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout)
{
  /* Wait for the FLASH operation to complete by polling on BUSY, WBNE and DBNE flags to be reset.
     Even if the FLASH operation fails, the BUSY, WBNE and DBNE flags will be reset and an error
     flag will be set */

  uint32_t errorflag;
  const __IO uint32_t *reg_sr;
  __IO uint32_t *reg_ccr;

  uint32_t tickstart = HAL_GetTick();

  /* Access to SR register depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_sr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECSR) : &(FLASH_NS->NSSR);
#else
  reg_sr = &(FLASH_NS->NSSR);
#endif /* FLASH_OPTSR2_TZEN */

  /* Wait on BSY, WBNE and DBNE flags to be reset */
  while (((*reg_sr) & (FLASH_FLAG_BSY | FLASH_FLAG_WBNE | FLASH_FLAG_DBNE)) != 0U)
  {
    if (Timeout != HAL_MAX_DELAY)
    {
      if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
      {
        return HAL_TIMEOUT;
      }
    }
  }

  /* Access to CCR register depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_ccr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCCR) : &(FLASH_NS->NSCCR);
#else
  reg_ccr = &(FLASH_NS->NSCCR);
#endif /* FLASH_OPTSR2_TZEN */

  /* Check FLASH operation error flags */
  errorflag = ((*reg_sr) & FLASH_FLAG_SR_ERRORS);
  /* Add option byte error flag, if any */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  errorflag |= (FLASH->NSSR & FLASH_FLAG_OPTCHANGEERR);
#endif /* __ARM_FEATURE_CMSE */

  /* In case of error reported in Flash SR or OPTSR registers */
  if (errorflag != 0U)
  {
    /*Save the error code*/
    pFlash.ErrorCode |= errorflag;

    /* Clear error flags */
    (*reg_ccr) = errorflag & FLASH_FLAG_SR_ERRORS;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    if ((errorflag & FLASH_FLAG_OPTCHANGEERR) != 0U)
    {
      FLASH->NSCCR = FLASH_FLAG_OPTCHANGEERR;
    }
#endif /* __ARM_FEATURE_CMSE */

    return HAL_ERROR;
  }

  /* Check FLASH End of Operation flag  */
  if (((*reg_sr) & FLASH_FLAG_EOP) != 0U)
  {
    /* Clear FLASH End of Operation pending bit */
    (*reg_ccr) = FLASH_FLAG_EOP;
  }

  /* If there is no error flag set */
  return HAL_OK;
}

/**
  * @brief  Program a quad-word (128-bit) at a specified address.
  * @param  FlashAddress specifies the address to be programmed.
  * @param  DataAddress specifies the address of data to be programmed.
  * @retval None
  */
static void FLASH_Program_QuadWord(uint32_t FlashAddress, uint32_t DataAddress)
{
  uint8_t index = 4;
  uint32_t *dest_addr = (uint32_t *)FlashAddress;
  uint32_t *src_addr  = (uint32_t *)DataAddress;
  uint32_t primask_bit;
  __IO uint32_t *reg_cr;

  /* Access to SECCR or NSCR registers depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
  reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

  /* Set PG bit */
  SET_BIT((*reg_cr), FLASH_CR_PG);

  /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Program the quad-word */
  do
  {
    *dest_addr = *src_addr;
    dest_addr++;
    src_addr++;
    index--;
  } while (index != 0U);

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}

#if defined (FLASH_SR_OBKERR)
/**
  * @brief  Program a quad-word (128-bit) of OBK at a specified address.
  * @param  FlashAddress specifies the address to be programmed.
  * @param  DataAddress specifies the address of data to be programmed.
  * @retval None
  */
static void FLASH_Program_QuadWord_OBK(uint32_t FlashAddress, uint32_t DataAddress)
{
  uint8_t index = 4;
  uint32_t *dest_addr = (uint32_t *)FlashAddress;
  uint32_t *src_addr  = (uint32_t *)DataAddress;
  uint32_t primask_bit;
  __IO uint32_t *reg_cr;
  __IO uint32_t *reg_obkcfgr;

  /* Access to SECCR or NSCR registers depends on operation type */
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
  reg_obkcfgr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECOBKCFGR) : &(FLASH_NS->NSOBKCFGR);

  /* Set PG bit */
  SET_BIT((*reg_cr), FLASH_CR_PG);

  /* Set ALT_SECT bit */
  SET_BIT((*reg_obkcfgr), pFlash.ProcedureOnGoing & FLASH_OBKCFGR_ALT_SECT);

  /* Enter critical section: Disable interrupts to avoid any interruption during the loop */
  primask_bit = __get_PRIMASK();
  __disable_irq();

  /* Program the quad-word */
  do
  {
    *dest_addr = *src_addr;
    dest_addr++;
    src_addr++;
    index--;
  } while (index != 0U);

  /* Exit critical section: restore previous priority mask */
  __set_PRIMASK(primask_bit);
}
#endif /* FLASH_SR_OBKERR */

/**
  * @brief  Program a half-word (16-bit) at a specified address.
  * @param  FlashAddress specifies the address to be programmed.
  * @param  DataAddress specifies the address of data to be programmed.
  * @retval None
  */
static void FLASH_Program_HalfWord(uint32_t FlashAddress, uint32_t DataAddress)
{
  __IO uint32_t *reg_cr;

  /* Access to SECCR or NSCR registers depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
  reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

  /* Set HalfWord_PG bit */
  SET_BIT((*reg_cr), FLASH_CR_PG);

  /* Program a halfword word (16 bits) */
  *(__IO uint16_t *)FlashAddress = *(__IO uint16_t *)DataAddress;
}

#if defined(FLASH_EDATAR_EDATA_EN)
/**
  * @brief  Program a word (32-bit) at a specified address.
  * @param  FlashAddress specifies the address to be programmed.
  * @param  DataAddress specifies the address of data to be programmed.
  * @retval None
  */
static void FLASH_Program_Word(uint32_t FlashAddress, uint32_t DataAddress)
{
  __IO uint32_t *reg_cr;

  /* Access to SECCR or NSCR registers depends on operation type */
#if defined (FLASH_OPTSR2_TZEN)
  reg_cr = IS_FLASH_SECURE_OPERATION() ? &(FLASH->SECCR) : &(FLASH_NS->NSCR);
#else
  reg_cr = &(FLASH_NS->NSCR);
#endif /* FLASH_OPTSR2_TZEN */

  /* Set PG bit */
  SET_BIT((*reg_cr), FLASH_CR_PG);

    *(__IO uint32_t *)FlashAddress = *(__IO uint32_t *)DataAddress;
}
#endif /* FLASH_EDATAR_EDATA_EN */

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
