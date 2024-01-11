/**
  ******************************************************************************
  * @file    stm32h7xx_hal_eth.c
  * @author  MCD Application Team
  * @brief   ETH HAL module driver.
  *          This file provides firmware functions to manage the following
  *          functionalities of the Ethernet (ETH) peripheral:
  *           + Initialization and deinitialization functions
  *           + IO operation functions
  *           + Peripheral Control functions
  *           + Peripheral State and Errors functions
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  @verbatim
  ==============================================================================
                    ##### How to use this driver #####
  ==============================================================================
     [..]
     The ETH HAL driver can be used as follows:

      (#)Declare a ETH_HandleTypeDef handle structure, for example:
         ETH_HandleTypeDef  heth;

      (#)Fill parameters of Init structure in heth handle

      (#)Call HAL_ETH_Init() API to initialize the Ethernet peripheral (MAC, DMA, ...)

      (#)Initialize the ETH low level resources through the HAL_ETH_MspInit() API:
          (##) Enable the Ethernet interface clock using
                (+++)  __HAL_RCC_ETH1MAC_CLK_ENABLE()
                (+++)  __HAL_RCC_ETH1TX_CLK_ENABLE()
                (+++)  __HAL_RCC_ETH1RX_CLK_ENABLE()

          (##) Initialize the related GPIO clocks
          (##) Configure Ethernet pinout
          (##) Configure Ethernet NVIC interrupt (in Interrupt mode)

      (#) Ethernet data reception is asynchronous, so call the following API
          to start the listening mode:
          (##) HAL_ETH_Start():
               This API starts the MAC and DMA transmission and reception process,
               without enabling end of transfer interrupts, in this mode user
               has to poll for data reception by calling HAL_ETH_ReadData()
          (##) HAL_ETH_Start_IT():
               This API starts the MAC and DMA transmission and reception process,
               end of transfer interrupts are enabled in this mode,
               HAL_ETH_RxCpltCallback() will be executed when an Ethernet packet is received

      (#) When data is received user can call the following API to get received data:
          (##) HAL_ETH_ReadData(): Read a received packet

      (#) For transmission path, two APIs are available:
         (##) HAL_ETH_Transmit(): Transmit an ETH frame in blocking mode
         (##) HAL_ETH_Transmit_IT(): Transmit an ETH frame in interrupt mode,
              HAL_ETH_TxCpltCallback() will be executed when end of transfer occur

      (#) Communication with an external PHY device:
         (##) HAL_ETH_ReadPHYRegister(): Read a register from an external PHY
         (##) HAL_ETH_WritePHYRegister(): Write data to an external RHY register

      (#) Configure the Ethernet MAC after ETH peripheral initialization
          (##) HAL_ETH_GetMACConfig(): Get MAC actual configuration into ETH_MACConfigTypeDef
          (##) HAL_ETH_SetMACConfig(): Set MAC configuration based on ETH_MACConfigTypeDef

      (#) Configure the Ethernet DMA after ETH peripheral initialization
          (##) HAL_ETH_GetDMAConfig(): Get DMA actual configuration into ETH_DMAConfigTypeDef
          (##) HAL_ETH_SetDMAConfig(): Set DMA configuration based on ETH_DMAConfigTypeDef

      (#) Configure the Ethernet PTP after ETH peripheral initialization
          (##) Define HAL_ETH_USE_PTP to use PTP APIs.
          (##) HAL_ETH_PTP_GetConfig(): Get PTP actual configuration into ETH_PTP_ConfigTypeDef
          (##) HAL_ETH_PTP_SetConfig(): Set PTP configuration based on ETH_PTP_ConfigTypeDef
          (##) HAL_ETH_PTP_GetTime(): Get Seconds and Nanoseconds for the Ethernet PTP registers
          (##) HAL_ETH_PTP_SetTime(): Set Seconds and Nanoseconds for the Ethernet PTP registers
          (##) HAL_ETH_PTP_AddTimeOffset(): Add Seconds and Nanoseconds offset for the Ethernet PTP registers
          (##) HAL_ETH_PTP_InsertTxTimestamp(): Insert Timestamp in transmission
          (##) HAL_ETH_PTP_GetTxTimestamp(): Get transmission timestamp
          (##) HAL_ETH_PTP_GetRxTimestamp(): Get reception timestamp

      -@- The ARP offload feature is not supported in this driver.

      -@- The PTP offload feature is not supported in this driver.

  *** Callback registration ***
  =============================================

  The compilation define  USE_HAL_ETH_REGISTER_CALLBACKS when set to 1
  allows the user to configure dynamically the driver callbacks.
  Use Function HAL_ETH_RegisterCallback() to register an interrupt callback.

  Function HAL_ETH_RegisterCallback() allows to register following callbacks:
    (+) TxCpltCallback   : Tx Complete Callback.
    (+) RxCpltCallback   : Rx Complete Callback.
    (+) ErrorCallback    : Error Callback.
    (+) PMTCallback      : Power Management Callback
    (+) EEECallback      : EEE Callback.
    (+) WakeUpCallback   : Wake UP Callback
    (+) MspInitCallback  : MspInit Callback.
    (+) MspDeInitCallback: MspDeInit Callback.

  This function takes as parameters the HAL peripheral handle, the Callback ID
  and a pointer to the user callback function.

  For specific callbacks RxAllocateCallback use dedicated register callbacks:
  respectively HAL_ETH_RegisterRxAllocateCallback().

  For specific callbacks RxLinkCallback use dedicated register callbacks:
  respectively HAL_ETH_RegisterRxLinkCallback().

  For specific callbacks TxFreeCallback use dedicated register callbacks:
  respectively HAL_ETH_RegisterTxFreeCallback().

  For specific callbacks TxPtpCallback use dedicated register callbacks:
  respectively HAL_ETH_RegisterTxPtpCallback().

  Use function HAL_ETH_UnRegisterCallback() to reset a callback to the default
  weak function.
  HAL_ETH_UnRegisterCallback takes as parameters the HAL peripheral handle,
  and the Callback ID.
  This function allows to reset following callbacks:
    (+) TxCpltCallback   : Tx Complete Callback.
    (+) RxCpltCallback   : Rx Complete Callback.
    (+) ErrorCallback    : Error Callback.
    (+) PMTCallback      : Power Management Callback
    (+) EEECallback      : EEE Callback.
    (+) WakeUpCallback   : Wake UP Callback
    (+) MspInitCallback  : MspInit Callback.
    (+) MspDeInitCallback: MspDeInit Callback.

  For specific callbacks RxAllocateCallback use dedicated unregister callbacks:
  respectively HAL_ETH_UnRegisterRxAllocateCallback().

  For specific callbacks RxLinkCallback use dedicated unregister callbacks:
  respectively HAL_ETH_UnRegisterRxLinkCallback().

  For specific callbacks TxFreeCallback use dedicated unregister callbacks:
  respectively HAL_ETH_UnRegisterTxFreeCallback().

  For specific callbacks TxPtpCallback use dedicated unregister callbacks:
  respectively HAL_ETH_UnRegisterTxPtpCallback().

  By default, after the HAL_ETH_Init and when the state is HAL_ETH_STATE_RESET
  all callbacks are set to the corresponding weak functions:
  examples HAL_ETH_TxCpltCallback(), HAL_ETH_RxCpltCallback().
  Exception done for MspInit and MspDeInit functions that are
  reset to the legacy weak function in the HAL_ETH_Init/ HAL_ETH_DeInit only when
  these callbacks are null (not registered beforehand).
  if not, MspInit or MspDeInit are not null, the HAL_ETH_Init/ HAL_ETH_DeInit
  keep and use the user MspInit/MspDeInit callbacks (registered beforehand)

  Callbacks can be registered/unregistered in HAL_ETH_STATE_READY state only.
  Exception done MspInit/MspDeInit that can be registered/unregistered
  in HAL_ETH_STATE_READY or HAL_ETH_STATE_RESET state,
  thus registered (user) MspInit/DeInit callbacks can be used during the Init/DeInit.
  In that case first register the MspInit/MspDeInit user callbacks
  using HAL_ETH_RegisterCallback() before calling HAL_ETH_DeInit
  or HAL_ETH_Init function.

  When The compilation define USE_HAL_ETH_REGISTER_CALLBACKS is set to 0 or
  not defined, the callback registration feature is not available and all callbacks
  are set to the corresponding weak functions.

  @endverbatim
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/** @addtogroup STM32H7xx_HAL_Driver
  * @{
  */
#ifdef HAL_ETH_MODULE_ENABLED

#if defined(ETH)

/** @defgroup ETH ETH
  * @brief ETH HAL module driver
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup ETH_Private_Constants ETH Private Constants
  * @{
  */
#define ETH_MACCR_MASK                0xFFFB7F7CU
#define ETH_MACECR_MASK               0x3F077FFFU
#define ETH_MACPFR_MASK               0x800007FFU
#define ETH_MACWTR_MASK               0x0000010FU
#define ETH_MACTFCR_MASK              0xFFFF00F2U
#define ETH_MACRFCR_MASK              0x00000003U
#define ETH_MTLTQOMR_MASK             0x00000072U
#define ETH_MTLRQOMR_MASK             0x0000007BU
							          
#define ETH_DMAMR_MASK                0x00007802U
#define ETH_DMASBMR_MASK              0x0000D001U
#define ETH_DMACCR_MASK               0x00013FFFU
#define ETH_DMACTCR_MASK              0x003F1010U
#define ETH_DMACRCR_MASK              0x803F0000U
#define ETH_MACPCSR_MASK              (ETH_MACPCSR_PWRDWN | ETH_MACPCSR_RWKPKTEN | \
                                       ETH_MACPCSR_MGKPKTEN | ETH_MACPCSR_GLBLUCAST | \
                                       ETH_MACPCSR_RWKPFE)

/* Timeout values */
#define ETH_DMARXNDESCWBF_ERRORS_MASK ((uint32_t)(ETH_DMARXNDESCWBF_DE | ETH_DMARXNDESCWBF_RE | \
                                                  ETH_DMARXNDESCWBF_OE | ETH_DMARXNDESCWBF_RWT |\
                                                  ETH_DMARXNDESCWBF_GP | ETH_DMARXNDESCWBF_CE))

#define ETH_MACTSCR_MASK              0x0087FF2FU

#define ETH_MACSTSUR_VALUE            0xFFFFFFFFU
#define ETH_MACSTNUR_VALUE            0xBB9ACA00U
#define ETH_SEGMENT_SIZE_DEFAULT      0x218U
/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup ETH_Private_Macros ETH Private Macros
  * @{
  */
/* Helper macros for TX descriptor handling */
#define INCR_TX_DESC_INDEX(inx, offset) do {\
                                             (inx) += (offset);\
                                             if ((inx) >= (uint32_t)ETH_TX_DESC_CNT){\
                                             (inx) = ((inx) - (uint32_t)ETH_TX_DESC_CNT);}\
                                           } while (0)

/* Helper macros for RX descriptor handling */
#define INCR_RX_DESC_INDEX(inx, offset) do {\
                                             (inx) += (offset);\
                                             if ((inx) >= (uint32_t)ETH_RX_DESC_CNT){\
                                             (inx) = ((inx) - (uint32_t)ETH_RX_DESC_CNT);}\
                                           } while (0)
/**
  * @}
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup ETH_Private_Functions   ETH Private Functions
  * @{
  */
static void ETH_SetMACConfig(ETH_HandleTypeDef *heth,  ETH_MACConfigTypeDef *macconf);
static void ETH_SetDMAConfig(ETH_HandleTypeDef *heth,  ETH_DMAConfigTypeDef *dmaconf);
static void ETH_MACDMAConfig(ETH_HandleTypeDef *heth);
static void ETH_DMATxDescListInit(ETH_HandleTypeDef *heth);
static void ETH_DMARxDescListInit(ETH_HandleTypeDef *heth);
static uint32_t ETH_Prepare_Tx_Descriptors(ETH_HandleTypeDef *heth, ETH_TxPacketConfig *pTxConfig, uint32_t ItMode);
static void ETH_UpdateDescriptor(ETH_HandleTypeDef *heth);

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
static void ETH_InitCallbacksToDefault(ETH_HandleTypeDef *heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup ETH_Exported_Functions ETH Exported Functions
  * @{
  */

/** @defgroup ETH_Exported_Functions_Group1 Initialization and deinitialization functions
  *  @brief    Initialization and Configuration functions
  *
@verbatim
===============================================================================
            ##### Initialization and Configuration functions #####
 ===============================================================================
    [..]  This subsection provides a set of functions allowing to initialize and
          deinitialize the ETH peripheral:

      (+) User must Implement HAL_ETH_MspInit() function in which he configures
          all related peripherals resources (CLOCK, GPIO and NVIC ).

      (+) Call the function HAL_ETH_Init() to configure the selected device with
          the selected configuration:
        (++) MAC address
        (++) Media interface (MII or RMII)
        (++) Rx DMA Descriptors Tab
        (++) Tx DMA Descriptors Tab
        (++) Length of Rx Buffers

      (+) Call the function HAL_ETH_DeInit() to restore the default configuration
          of the selected ETH peripheral.

@endverbatim
  * @{
  */

/**
  * @brief  Initialize the Ethernet peripheral registers.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Init(ETH_HandleTypeDef *heth)
{
  uint32_t tickstart;

  if (heth == NULL)
  {
    return HAL_ERROR;
  }
  if (heth->gState == HAL_ETH_STATE_RESET)
  {
    heth->gState = HAL_ETH_STATE_BUSY;

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)

    ETH_InitCallbacksToDefault(heth);

    if (heth->MspInitCallback == NULL)
    {
      heth->MspInitCallback = HAL_ETH_MspInit;
    }

    /* Init the low level hardware */
    heth->MspInitCallback(heth);
#else
    /* Init the low level hardware : GPIO, CLOCK, NVIC. */
    HAL_ETH_MspInit(heth);

#endif /* (USE_HAL_ETH_REGISTER_CALLBACKS) */
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  if (heth->Init.MediaInterface == HAL_ETH_MII_MODE)
  {
    HAL_SYSCFG_ETHInterfaceSelect(SYSCFG_ETH_MII);
  }
  else
  {
    HAL_SYSCFG_ETHInterfaceSelect(SYSCFG_ETH_RMII);
  }

  /* Dummy read to sync with ETH */
  (void)SYSCFG->PMCR;

  /* Ethernet Software reset */
  /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
  /* After reset all the registers holds their respective reset values */
  SET_BIT(heth->Instance->DMAMR, ETH_DMAMR_SWR);

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait for software reset */
  while (READ_BIT(heth->Instance->DMAMR, ETH_DMAMR_SWR) > 0U)
  {
    if (((HAL_GetTick() - tickstart) > ETH_SWRESET_TIMEOUT))
    {
      /* Set Error Code */
      heth->ErrorCode = HAL_ETH_ERROR_TIMEOUT;
      /* Set State as Error */
      heth->gState = HAL_ETH_STATE_ERROR;
      /* Return Error */
      return HAL_ERROR;
    }
  }

  /*------------------ MDIO CSR Clock Range Configuration --------------------*/
  HAL_ETH_SetMDIOClockRange(heth);

  /*------------------ MAC LPI 1US Tic Counter Configuration --------------------*/
  WRITE_REG(heth->Instance->MAC1USTCR, (((uint32_t)HAL_RCC_GetHCLKFreq() / ETH_MAC_US_TICK) - 1U));

  /*------------------ MAC, MTL and DMA default Configuration ----------------*/
  ETH_MACDMAConfig(heth);

  /* SET DSL to 64 bit */
  MODIFY_REG(heth->Instance->DMACCR, ETH_DMACCR_DSL, ETH_DMACCR_DSL_64BIT);

  /* Set Receive Buffers Length (must be a multiple of 4) */
  if ((heth->Init.RxBuffLen % 0x4U) != 0x0U)
  {
    /* Set Error Code */
    heth->ErrorCode = HAL_ETH_ERROR_PARAM;
    /* Set State as Error */
    heth->gState = HAL_ETH_STATE_ERROR;
    /* Return Error */
    return HAL_ERROR;
  }
  else
  {
    MODIFY_REG(heth->Instance->DMACRCR, ETH_DMACRCR_RBSZ, ((heth->Init.RxBuffLen) << 1));
  }

  /*------------------ DMA Tx Descriptors Configuration ----------------------*/
  ETH_DMATxDescListInit(heth);

  /*------------------ DMA Rx Descriptors Configuration ----------------------*/
  ETH_DMARxDescListInit(heth);

  /*--------------------- ETHERNET MAC Address Configuration ------------------*/
  /* Set MAC addr bits 32 to 47 */
  heth->Instance->MACA0HR = (((uint32_t)(heth->Init.MACAddr[5]) << 8) | (uint32_t)heth->Init.MACAddr[4]);
  /* Set MAC addr bits 0 to 31 */
  heth->Instance->MACA0LR = (((uint32_t)(heth->Init.MACAddr[3]) << 24) | ((uint32_t)(heth->Init.MACAddr[2]) << 16) |
                             ((uint32_t)(heth->Init.MACAddr[1]) << 8) | (uint32_t)heth->Init.MACAddr[0]);

  heth->ErrorCode = HAL_ETH_ERROR_NONE;
  heth->gState = HAL_ETH_STATE_READY;

  return HAL_OK;
}

/**
  * @brief  DeInitializes the ETH peripheral.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_DeInit(ETH_HandleTypeDef *heth)
{
  /* Set the ETH peripheral state to BUSY */
  heth->gState = HAL_ETH_STATE_BUSY;

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)

  if (heth->MspDeInitCallback == NULL)
  {
    heth->MspDeInitCallback = HAL_ETH_MspDeInit;
  }
  /* DeInit the low level hardware */
  heth->MspDeInitCallback(heth);
#else

  /* De-Init the low level hardware : GPIO, CLOCK, NVIC. */
  HAL_ETH_MspDeInit(heth);

#endif /* (USE_HAL_ETH_REGISTER_CALLBACKS) */

  /* Set ETH HAL state to Disabled */
  heth->gState = HAL_ETH_STATE_RESET;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_MspInit could be implemented in the user file
  */
}

/**
  * @brief  DeInitializes ETH MSP.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_MspDeInit could be implemented in the user file
  */
}

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
/**
  * @brief  Register a User ETH Callback
  *         To be used instead of the weak predefined callback
  * @param heth eth handle
  * @param CallbackID ID of the callback to be registered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_ETH_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *          @arg @ref HAL_ETH_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *          @arg @ref HAL_ETH_ERROR_CB_ID       Error Callback ID
  *          @arg @ref HAL_ETH_PMT_CB_ID         Power Management Callback ID
  *          @arg @ref HAL_ETH_EEE_CB_ID         EEE Callback ID
  *          @arg @ref HAL_ETH_WAKEUP_CB_ID      Wake UP Callback ID
  *          @arg @ref HAL_ETH_MSPINIT_CB_ID     MspInit callback ID
  *          @arg @ref HAL_ETH_MSPDEINIT_CB_ID   MspDeInit callback ID
  * @param pCallback pointer to the Callback function
  * @retval status
  */
HAL_StatusTypeDef HAL_ETH_RegisterCallback(ETH_HandleTypeDef *heth, HAL_ETH_CallbackIDTypeDef CallbackID,
                                           pETH_CallbackTypeDef pCallback)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (pCallback == NULL)
  {
    /* Update the error code */
    heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;

    return HAL_ERROR;
  }

  if (heth->gState == HAL_ETH_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_ETH_TX_COMPLETE_CB_ID :
        heth->TxCpltCallback = pCallback;
        break;

      case HAL_ETH_RX_COMPLETE_CB_ID :
        heth->RxCpltCallback = pCallback;
        break;

      case HAL_ETH_ERROR_CB_ID :
        heth->ErrorCallback = pCallback;
        break;

      case HAL_ETH_PMT_CB_ID :
        heth->PMTCallback = pCallback;
        break;

      case HAL_ETH_EEE_CB_ID :
        heth->EEECallback = pCallback;
        break;

      case HAL_ETH_WAKEUP_CB_ID :
        heth->WakeUpCallback = pCallback;
        break;

      case HAL_ETH_MSPINIT_CB_ID :
        heth->MspInitCallback = pCallback;
        break;

      case HAL_ETH_MSPDEINIT_CB_ID :
        heth->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (heth->gState == HAL_ETH_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_ETH_MSPINIT_CB_ID :
        heth->MspInitCallback = pCallback;
        break;

      case HAL_ETH_MSPDEINIT_CB_ID :
        heth->MspDeInitCallback = pCallback;
        break;

      default :
        /* Update the error code */
        heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;
    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}

/**
  * @brief  Unregister an ETH Callback
  *         ETH callabck is redirected to the weak predefined callback
  * @param heth eth handle
  * @param CallbackID ID of the callback to be unregistered
  *        This parameter can be one of the following values:
  *          @arg @ref HAL_ETH_TX_COMPLETE_CB_ID Tx Complete Callback ID
  *          @arg @ref HAL_ETH_RX_COMPLETE_CB_ID Rx Complete Callback ID
  *          @arg @ref HAL_ETH_ERROR_CB_ID       Error Callback ID
  *          @arg @ref HAL_ETH_PMT_CB_ID         Power Management Callback ID
  *          @arg @ref HAL_ETH_EEE_CB_ID         EEE Callback ID
  *          @arg @ref HAL_ETH_WAKEUP_CB_ID      Wake UP Callback ID
  *          @arg @ref HAL_ETH_MSPINIT_CB_ID     MspInit callback ID
  *          @arg @ref HAL_ETH_MSPDEINIT_CB_ID   MspDeInit callback ID
  * @retval status
  */
HAL_StatusTypeDef HAL_ETH_UnRegisterCallback(ETH_HandleTypeDef *heth, HAL_ETH_CallbackIDTypeDef CallbackID)
{
  HAL_StatusTypeDef status = HAL_OK;

  if (heth->gState == HAL_ETH_STATE_READY)
  {
    switch (CallbackID)
    {
      case HAL_ETH_TX_COMPLETE_CB_ID :
        heth->TxCpltCallback = HAL_ETH_TxCpltCallback;
        break;

      case HAL_ETH_RX_COMPLETE_CB_ID :
        heth->RxCpltCallback = HAL_ETH_RxCpltCallback;
        break;

      case HAL_ETH_ERROR_CB_ID :
        heth->ErrorCallback = HAL_ETH_ErrorCallback;
        break;

      case HAL_ETH_PMT_CB_ID :
        heth->PMTCallback = HAL_ETH_PMTCallback;
        break;

      case HAL_ETH_EEE_CB_ID :
        heth->EEECallback = HAL_ETH_EEECallback;
        break;

      case HAL_ETH_WAKEUP_CB_ID :
        heth->WakeUpCallback = HAL_ETH_WakeUpCallback;
        break;

      case HAL_ETH_MSPINIT_CB_ID :
        heth->MspInitCallback = HAL_ETH_MspInit;
        break;

      case HAL_ETH_MSPDEINIT_CB_ID :
        heth->MspDeInitCallback = HAL_ETH_MspDeInit;
        break;

      default :
        /* Update the error code */
        heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else if (heth->gState == HAL_ETH_STATE_RESET)
  {
    switch (CallbackID)
    {
      case HAL_ETH_MSPINIT_CB_ID :
        heth->MspInitCallback = HAL_ETH_MspInit;
        break;

      case HAL_ETH_MSPDEINIT_CB_ID :
        heth->MspDeInitCallback = HAL_ETH_MspDeInit;
        break;

      default :
        /* Update the error code */
        heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;
        /* Return error status */
        status =  HAL_ERROR;
        break;
    }
  }
  else
  {
    /* Update the error code */
    heth->ErrorCode |= HAL_ETH_ERROR_INVALID_CALLBACK;
    /* Return error status */
    status =  HAL_ERROR;
  }

  return status;
}
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */

/**
  * @}
  */

/** @defgroup ETH_Exported_Functions_Group2 IO operation functions
  *  @brief ETH Transmit and Receive functions
  *
@verbatim
  ==============================================================================
                      ##### IO operation functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to manage the ETH
    data transfer.

@endverbatim
  * @{
  */

/**
  * @brief  Enables Ethernet MAC and DMA reception and transmission
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Start(ETH_HandleTypeDef *heth)
{
  if (heth->gState == HAL_ETH_STATE_READY)
  {
    heth->gState = HAL_ETH_STATE_BUSY;

    /* Set nombre of descriptors to build */
    heth->RxDescList.RxBuildDescCnt = ETH_RX_DESC_CNT;

    /* Build all descriptors */
    ETH_UpdateDescriptor(heth);

    /* Enable the MAC transmission */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Enable the MAC reception */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Set the Flush Transmit FIFO bit */
    SET_BIT(heth->Instance->MTLTQOMR, ETH_MTLTQOMR_FTQ);

    /* Enable the DMA transmission */
    SET_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_ST);

    /* Enable the DMA reception */
    SET_BIT(heth->Instance->DMACRCR, ETH_DMACRCR_SR);

    /* Clear Tx and Rx process stopped flags */
    heth->Instance->DMACSR |= (ETH_DMACSR_TPS | ETH_DMACSR_RPS);

    heth->gState = HAL_ETH_STATE_STARTED;

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Enables Ethernet MAC and DMA reception/transmission in Interrupt mode
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Start_IT(ETH_HandleTypeDef *heth)
{
  if (heth->gState == HAL_ETH_STATE_READY)
  {
    heth->gState = HAL_ETH_STATE_BUSY;

    /* save IT mode to ETH Handle */
    heth->RxDescList.ItMode = 1U;
    /* Disable Rx MMC Interrupts */
    SET_BIT(heth->Instance->MMCRIMR, ETH_MMCRIMR_RXLPITRCIM | ETH_MMCRIMR_RXLPIUSCIM | \
            ETH_MMCRIMR_RXUCGPIM | ETH_MMCRIMR_RXALGNERPIM | ETH_MMCRIMR_RXCRCERPIM);

    /* Disable Tx MMC Interrupts */
    SET_BIT(heth->Instance->MMCTIMR, ETH_MMCTIMR_TXLPITRCIM | ETH_MMCTIMR_TXLPIUSCIM | \
            ETH_MMCTIMR_TXGPKTIM | ETH_MMCTIMR_TXMCOLGPIM | ETH_MMCTIMR_TXSCOLGPIM);

    /* Set nombre of descriptors to build */
    heth->RxDescList.RxBuildDescCnt = ETH_RX_DESC_CNT;

    /* Build all descriptors */
    ETH_UpdateDescriptor(heth);

    /* Enable the MAC transmission */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Enable the MAC reception */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Set the Flush Transmit FIFO bit */
    SET_BIT(heth->Instance->MTLTQOMR, ETH_MTLTQOMR_FTQ);

    /* Enable the DMA transmission */
    SET_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_ST);

    /* Enable the DMA reception */
    SET_BIT(heth->Instance->DMACRCR, ETH_DMACRCR_SR);

    /* Clear Tx and Rx process stopped flags */
    heth->Instance->DMACSR |= (ETH_DMACSR_TPS | ETH_DMACSR_RPS);

    /* Enable ETH DMA interrupts:
    - Tx complete interrupt
    - Rx complete interrupt
    - Fatal bus interrupt
    */
    __HAL_ETH_DMA_ENABLE_IT(heth, (ETH_DMACIER_NIE | ETH_DMACIER_RIE | ETH_DMACIER_TIE  |
                                   ETH_DMACIER_FBEE | ETH_DMACIER_AIE | ETH_DMACIER_RBUE));

    heth->gState = HAL_ETH_STATE_STARTED;
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Stop Ethernet MAC and DMA reception/transmission
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Stop(ETH_HandleTypeDef *heth)
{
  if (heth->gState == HAL_ETH_STATE_STARTED)
  {
    /* Set the ETH peripheral state to BUSY */
    heth->gState = HAL_ETH_STATE_BUSY;
    /* Disable the DMA transmission */
    CLEAR_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_ST);

    /* Disable the DMA reception */
    CLEAR_BIT(heth->Instance->DMACRCR, ETH_DMACRCR_SR);

    /* Disable the MAC reception */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Set the Flush Transmit FIFO bit */
    SET_BIT(heth->Instance->MTLTQOMR, ETH_MTLTQOMR_FTQ);

    /* Disable the MAC transmission */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    heth->gState = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Stop Ethernet MAC and DMA reception/transmission in Interrupt mode
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Stop_IT(ETH_HandleTypeDef *heth)
{
  ETH_DMADescTypeDef *dmarxdesc;
  uint32_t descindex;

  if (heth->gState == HAL_ETH_STATE_STARTED)
  {
    /* Set the ETH peripheral state to BUSY */
    heth->gState = HAL_ETH_STATE_BUSY;

    /* Disable interrupts:
    - Tx complete interrupt
    - Rx complete interrupt
    - Fatal bus interrupt
    */
    __HAL_ETH_DMA_DISABLE_IT(heth, (ETH_DMACIER_NIE | ETH_DMACIER_RIE | ETH_DMACIER_TIE  |
                                    ETH_DMACIER_FBEE | ETH_DMACIER_AIE | ETH_DMACIER_RBUE));

    /* Disable the DMA transmission */
    CLEAR_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_ST);

    /* Disable the DMA reception */
    CLEAR_BIT(heth->Instance->DMACRCR, ETH_DMACRCR_SR);

    /* Disable the MAC reception */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_RE);
    /* Set the Flush Transmit FIFO bit */
    SET_BIT(heth->Instance->MTLTQOMR, ETH_MTLTQOMR_FTQ);

    /* Disable the MAC transmission */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Clear IOC bit to all Rx descriptors */
    for (descindex = 0; descindex < (uint32_t)ETH_RX_DESC_CNT; descindex++)
    {
      dmarxdesc = (ETH_DMADescTypeDef *)heth->RxDescList.RxDesc[descindex];
      CLEAR_BIT(dmarxdesc->DESC3, ETH_DMARXNDESCRF_IOC);
    }

    heth->RxDescList.ItMode = 0U;

    heth->gState = HAL_ETH_STATE_READY;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Sends an Ethernet Packet in polling mode.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pTxConfig: Hold the configuration of packet to be transmitted
  * @param  Timeout: timeout value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Transmit(ETH_HandleTypeDef *heth, ETH_TxPacketConfig *pTxConfig, uint32_t Timeout)
{
  uint32_t tickstart;
  ETH_DMADescTypeDef *dmatxdesc;

  if (pTxConfig == NULL)
  {
    heth->ErrorCode |= HAL_ETH_ERROR_PARAM;
    return HAL_ERROR;
  }

  if (heth->gState == HAL_ETH_STATE_STARTED)
  {
    /* Config DMA Tx descriptor by Tx Packet info */
    if (ETH_Prepare_Tx_Descriptors(heth, pTxConfig, 0) != HAL_ETH_ERROR_NONE)
    {
      /* Set the ETH error code */
      heth->ErrorCode |= HAL_ETH_ERROR_BUSY;
      return HAL_ERROR;
    }

    /* Ensure completion of descriptor preparation before transmission start */
    __DSB();

    dmatxdesc = (ETH_DMADescTypeDef *)(&heth->TxDescList)->TxDesc[heth->TxDescList.CurTxDesc];

    /* Incr current tx desc index */
    INCR_TX_DESC_INDEX(heth->TxDescList.CurTxDesc, 1U);

    /* Start transmission */
    /* issue a poll command to Tx DMA by writing address of next immediate free descriptor */
    WRITE_REG(heth->Instance->DMACTDTPR, (uint32_t)(heth->TxDescList.TxDesc[heth->TxDescList.CurTxDesc]));

    tickstart = HAL_GetTick();

    /* Wait for data to be transmitted or timeout occurred */
    while ((dmatxdesc->DESC3 & ETH_DMATXNDESCWBF_OWN) != (uint32_t)RESET)
    {
      if ((heth->Instance->DMACSR & ETH_DMACSR_FBE) != (uint32_t)RESET)
      {
        heth->ErrorCode |= HAL_ETH_ERROR_DMA;
        heth->DMAErrorCode = heth->Instance->DMACSR;
        /* Return function status */
        return HAL_ERROR;
      }

      /* Check for the Timeout */
      if (Timeout != HAL_MAX_DELAY)
      {
        if (((HAL_GetTick() - tickstart) > Timeout) || (Timeout == 0U))
        {
          heth->ErrorCode |= HAL_ETH_ERROR_TIMEOUT;
          /* Clear TX descriptor so that we can proceed */
          dmatxdesc->DESC3 = (ETH_DMATXNDESCWBF_FD | ETH_DMATXNDESCWBF_LD);
          return HAL_ERROR;
        }
      }
    }

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Sends an Ethernet Packet in interrupt mode.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pTxConfig: Hold the configuration of packet to be transmitted
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_Transmit_IT(ETH_HandleTypeDef *heth, ETH_TxPacketConfig *pTxConfig)
{
  if (pTxConfig == NULL)
  {
    heth->ErrorCode |= HAL_ETH_ERROR_PARAM;
    return HAL_ERROR;
  }

  if (heth->gState == HAL_ETH_STATE_STARTED)
  {
    /* Save the packet pointer to release.  */
    heth->TxDescList.CurrentPacketAddress = (uint32_t *)pTxConfig->pData;

    /* Config DMA Tx descriptor by Tx Packet info */
    if (ETH_Prepare_Tx_Descriptors(heth, pTxConfig, 1) != HAL_ETH_ERROR_NONE)
    {
      heth->ErrorCode |= HAL_ETH_ERROR_BUSY;
      return HAL_ERROR;
    }

    /* Ensure completion of descriptor preparation before transmission start */
    __DSB();

    /* Incr current tx desc index */
    INCR_TX_DESC_INDEX(heth->TxDescList.CurTxDesc, 1U);

    /* Start transmission */
    /* issue a poll command to Tx DMA by writing address of next immediate free descriptor */
    WRITE_REG(heth->Instance->DMACTDTPR, (uint32_t)(heth->TxDescList.TxDesc[heth->TxDescList.CurTxDesc]));

    return HAL_OK;

  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Read a received packet.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pAppBuff: Pointer to an application buffer to receive the packet.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_ReadData(ETH_HandleTypeDef *heth, void **pAppBuff)
{
  uint32_t descidx;
  ETH_DMADescTypeDef *dmarxdesc;
  uint32_t desccnt = 0U;
  uint32_t desccntmax;
  uint32_t bufflength;
  uint8_t rxdataready = 0U;


  if (pAppBuff == NULL)
  {
    heth->ErrorCode |= HAL_ETH_ERROR_PARAM;
    return HAL_ERROR;
  }

  if (heth->gState != HAL_ETH_STATE_STARTED)
  {
    return HAL_ERROR;
  }

  descidx = heth->RxDescList.RxDescIdx;
  dmarxdesc = (ETH_DMADescTypeDef *)heth->RxDescList.RxDesc[descidx];
  desccntmax = ETH_RX_DESC_CNT - heth->RxDescList.RxBuildDescCnt;

  /* Check if descriptor is not owned by DMA */
  while ((READ_BIT(dmarxdesc->DESC3, ETH_DMARXNDESCWBF_OWN) == (uint32_t)RESET) && (desccnt < desccntmax)
         && (rxdataready == 0U))
  {
    if (READ_BIT(dmarxdesc->DESC3,  ETH_DMARXNDESCWBF_CTXT)  != (uint32_t)RESET)
    {
      /* Get timestamp high */
      heth->RxDescList.TimeStamp.TimeStampHigh = dmarxdesc->DESC1;
      /* Get timestamp low */
      heth->RxDescList.TimeStamp.TimeStampLow  = dmarxdesc->DESC0;
    }
    if ((READ_BIT(dmarxdesc->DESC3, ETH_DMARXNDESCWBF_FD) != (uint32_t)RESET) || (heth->RxDescList.pRxStart != NULL))
    {
      /* Check if first descriptor */
      if (READ_BIT(dmarxdesc->DESC3, ETH_DMARXNDESCWBF_FD) != (uint32_t)RESET)
      {
        heth->RxDescList.RxDescCnt = 0;
        heth->RxDescList.RxDataLength = 0;
      }

      /* Check if last descriptor */
      bufflength = heth->Init.RxBuffLen;
      if (READ_BIT(dmarxdesc->DESC3, ETH_DMARXNDESCWBF_LD) != (uint32_t)RESET)
      {
        bufflength = READ_BIT(dmarxdesc->DESC3, ETH_DMARXNDESCWBF_PL) - heth->RxDescList.RxDataLength;

        /* Save Last descriptor index */
        heth->RxDescList.pRxLastRxDesc = dmarxdesc->DESC3;

        /* Packet ready */
        rxdataready = 1;
      }

      /* Link data */
#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /*Call registered Link callback*/
      heth->rxLinkCallback(&heth->RxDescList.pRxStart, &heth->RxDescList.pRxEnd,
                           (uint8_t *)dmarxdesc->BackupAddr0, bufflength);
#else
      /* Link callback */
      HAL_ETH_RxLinkCallback(&heth->RxDescList.pRxStart, &heth->RxDescList.pRxEnd,
                             (uint8_t *)dmarxdesc->BackupAddr0, (uint16_t) bufflength);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */
      heth->RxDescList.RxDescCnt++;
      heth->RxDescList.RxDataLength += bufflength;

      /* Clear buffer pointer */
      dmarxdesc->BackupAddr0 = 0;
    }

    /* Increment current rx descriptor index */
    INCR_RX_DESC_INDEX(descidx, 1U);
    /* Get current descriptor address */
    dmarxdesc = (ETH_DMADescTypeDef *)heth->RxDescList.RxDesc[descidx];
    desccnt++;
  }

  heth->RxDescList.RxBuildDescCnt += desccnt;
  if ((heth->RxDescList.RxBuildDescCnt) != 0U)
  {
    /* Update Descriptors */
    ETH_UpdateDescriptor(heth);
  }

  heth->RxDescList.RxDescIdx = descidx;

  if (rxdataready == 1U)
  {
    /* Return received packet */
    *pAppBuff = heth->RxDescList.pRxStart;
    /* Reset first element */
    heth->RxDescList.pRxStart = NULL;

    return HAL_OK;
  }

  /* Packet not ready */
  return HAL_ERROR;
}

/**
  * @brief  This function gives back Rx Desc of the last received Packet
  *         to the DMA, so ETH DMA will be able to use these descriptors
  *         to receive next Packets.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
static void ETH_UpdateDescriptor(ETH_HandleTypeDef *heth)
{
  uint32_t descidx;
  uint32_t desccount;
  ETH_DMADescTypeDef *dmarxdesc;
  uint8_t *buff = NULL;
  uint8_t allocStatus = 1U;

  descidx = heth->RxDescList.RxBuildDescIdx;
  dmarxdesc = (ETH_DMADescTypeDef *)heth->RxDescList.RxDesc[descidx];
  desccount = heth->RxDescList.RxBuildDescCnt;

  while ((desccount > 0U) && (allocStatus != 0U))
  {
    /* Check if a buffer's attached the descriptor */
    if (READ_REG(dmarxdesc->BackupAddr0) == 0U)
    {
      /* Get a new buffer. */
#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /*Call registered Allocate callback*/
      heth->rxAllocateCallback(&buff);
#else
      /* Allocate callback */
      HAL_ETH_RxAllocateCallback(&buff);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */
      if (buff == NULL)
      {
        allocStatus = 0U;
      }
      else
      {
        WRITE_REG(dmarxdesc->BackupAddr0, (uint32_t)buff);
        WRITE_REG(dmarxdesc->DESC0, (uint32_t)buff);
      }
    }

    if (allocStatus != 0U)
    {
      /* Ensure rest of descriptor is written to RAM before the OWN bit */
      __DMB();

      if (heth->RxDescList.ItMode != 0U)
      {
        WRITE_REG(dmarxdesc->DESC3, ETH_DMARXNDESCRF_OWN | ETH_DMARXNDESCRF_BUF1V | ETH_DMARXNDESCRF_IOC);
      }
      else
      {
        WRITE_REG(dmarxdesc->DESC3, ETH_DMARXNDESCRF_OWN | ETH_DMARXNDESCRF_BUF1V);
      }

      /* Increment current rx descriptor index */
      INCR_RX_DESC_INDEX(descidx, 1U);
      /* Get current descriptor address */
      dmarxdesc = (ETH_DMADescTypeDef *)heth->RxDescList.RxDesc[descidx];
      desccount--;
    }
  }

  if (heth->RxDescList.RxBuildDescCnt != desccount)
  {
    /* Set the Tail pointer address */
    WRITE_REG(heth->Instance->DMACRDTPR, 0);

    heth->RxDescList.RxBuildDescIdx = descidx;
    heth->RxDescList.RxBuildDescCnt = desccount;
  }
}

/**
  * @brief  Register the Rx alloc callback.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  rxAllocateCallback: pointer to function to alloc buffer
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_RegisterRxAllocateCallback(ETH_HandleTypeDef *heth,
                                                     pETH_rxAllocateCallbackTypeDef rxAllocateCallback)
{
  if (rxAllocateCallback == NULL)
  {
    /* No buffer to save */
    return HAL_ERROR;
  }

  /* Set function to allocate buffer */
  heth->rxAllocateCallback = rxAllocateCallback;

  return HAL_OK;
}

/**
  * @brief  Unregister the Rx alloc callback.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_UnRegisterRxAllocateCallback(ETH_HandleTypeDef *heth)
{
  /* Set function to allocate buffer */
  heth->rxAllocateCallback = HAL_ETH_RxAllocateCallback;

  return HAL_OK;
}

/**
  * @brief  Rx Allocate callback.
  * @param  buff: pointer to allocated buffer
  * @retval None
  */
__weak void HAL_ETH_RxAllocateCallback(uint8_t **buff)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(buff);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_RxAllocateCallback could be implemented in the user file
  */
}

/**
  * @brief  Rx Link callback.
  * @param  pStart: pointer to packet start
  * @param  pStart: pointer to packet end
  * @param  buff: pointer to received data
  * @param  Length: received data length
  * @retval None
  */
__weak void HAL_ETH_RxLinkCallback(void **pStart, void **pEnd, uint8_t *buff, uint16_t Length)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(pStart);
  UNUSED(pEnd);
  UNUSED(buff);
  UNUSED(Length);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_RxLinkCallback could be implemented in the user file
  */
}

/**
  * @brief  Set the Rx link data function.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  rxLinkCallback: pointer to function to link data
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_RegisterRxLinkCallback(ETH_HandleTypeDef *heth, pETH_rxLinkCallbackTypeDef rxLinkCallback)
{
  if (rxLinkCallback == NULL)
  {
    /* No buffer to save */
    return HAL_ERROR;
  }

  /* Set function to link data */
  heth->rxLinkCallback = rxLinkCallback;

  return HAL_OK;
}

/**
  * @brief  Unregister the Rx link callback.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_UnRegisterRxLinkCallback(ETH_HandleTypeDef *heth)
{
  /* Set function to allocate buffer */
  heth->rxLinkCallback = HAL_ETH_RxLinkCallback;

  return HAL_OK;
}

/**
  * @brief  Get the error state of the last received packet.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pErrorCode: pointer to uint32_t to hold the error code
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_GetRxDataErrorCode(ETH_HandleTypeDef *heth, uint32_t *pErrorCode)
{
  /* Get error bits. */
  *pErrorCode = READ_BIT(heth->RxDescList.pRxLastRxDesc, ETH_DMARXNDESCWBF_ERRORS_MASK);

  return HAL_OK;
}

/**
  * @brief  Set the Tx free function.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  txFreeCallback: pointer to function to release the packet
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_RegisterTxFreeCallback(ETH_HandleTypeDef *heth, pETH_txFreeCallbackTypeDef txFreeCallback)
{
  if (txFreeCallback == NULL)
  {
    /* No buffer to save */
    return HAL_ERROR;
  }

  /* Set function to free transmmitted packet */
  heth->txFreeCallback = txFreeCallback;

  return HAL_OK;
}

/**
  * @brief  Unregister the Tx free callback.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_UnRegisterTxFreeCallback(ETH_HandleTypeDef *heth)
{
  /* Set function to allocate buffer */
  heth->txFreeCallback = HAL_ETH_TxFreeCallback;

  return HAL_OK;
}

/**
  * @brief  Tx Free callback.
  * @param  buff: pointer to buffer to free
  * @retval None
  */
__weak void HAL_ETH_TxFreeCallback(uint32_t *buff)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(buff);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_TxFreeCallback could be implemented in the user file
  */
}

/**
  * @brief  Release transmitted Tx packets.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_ReleaseTxPacket(ETH_HandleTypeDef *heth)
{
  ETH_TxDescListTypeDef *dmatxdesclist = &heth->TxDescList;
  uint32_t numOfBuf =  dmatxdesclist->BuffersInUse;
  uint32_t idx =       dmatxdesclist->releaseIndex;
  uint8_t pktTxStatus = 1U;
  uint8_t pktInUse;
#ifdef HAL_ETH_USE_PTP
  ETH_TimeStampTypeDef *timestamp = &heth->TxTimestamp;
#endif /* HAL_ETH_USE_PTP */

  /* Loop through buffers in use.  */
  while ((numOfBuf != 0U) && (pktTxStatus != 0U))
  {
    pktInUse = 1U;
    numOfBuf--;
    /* If no packet, just examine the next packet.  */
    if (dmatxdesclist->PacketAddress[idx] == NULL)
    {
      /* No packet in use, skip to next.  */
      idx = (idx + 1U) & (ETH_TX_DESC_CNT - 1U);
      pktInUse = 0U;
    }

    if (pktInUse != 0U)
    {
      /* Determine if the packet has been transmitted.  */
      if ((heth->Init.TxDesc[idx].DESC3 & ETH_DMATXNDESCRF_OWN) == 0U)
      {
#ifdef HAL_ETH_USE_PTP
        /* Disable Ptp transmission */
        CLEAR_BIT(heth->Init.TxDesc[idx].DESC3, (0x40000000U));

        /* Get timestamp low */
        timestamp->TimeStampLow = heth->Init.TxDesc[idx].DESC0;
        /* Get timestamp high */
        timestamp->TimeStampHigh = heth->Init.TxDesc[idx].DESC1;
#endif /* HAL_ETH_USE_PTP */

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
        /*Call registered callbacks*/
#ifdef HAL_ETH_USE_PTP
        /* Handle Ptp  */
        heth->txPtpCallback(dmatxdesclist->PacketAddress[idx], timestamp);
#endif  /* HAL_ETH_USE_PTP */
        /* Release the packet.  */
        heth->txFreeCallback(dmatxdesclist->PacketAddress[idx]);
#else
        /* Call callbacks */
#ifdef HAL_ETH_USE_PTP
        /* Handle Ptp  */
        HAL_ETH_TxPtpCallback(dmatxdesclist->PacketAddress[idx], timestamp);
#endif  /* HAL_ETH_USE_PTP */
        /* Release the packet.  */
        HAL_ETH_TxFreeCallback(dmatxdesclist->PacketAddress[idx]);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */

        /* Clear the entry in the in-use array.  */
        dmatxdesclist->PacketAddress[idx] = NULL;

        /* Update the transmit relesae index and number of buffers in use.  */
        idx = (idx + 1U) & (ETH_TX_DESC_CNT - 1U);
        dmatxdesclist->BuffersInUse = numOfBuf;
        dmatxdesclist->releaseIndex = idx;
      }
      else
      {
        /* Get out of the loop!  */
        pktTxStatus = 0U;
      }
    }
  }
  return HAL_OK;
}

#ifdef HAL_ETH_USE_PTP
/**
  * @brief  Set the Ethernet PTP configuration.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  ptpconfig: pointer to a ETH_PTP_ConfigTypeDef structure that contains
  *         the configuration information for PTP
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_SetConfig(ETH_HandleTypeDef *heth, ETH_PTP_ConfigTypeDef *ptpconfig)
{
  uint32_t tmpTSCR;
  ETH_TimeTypeDef time;

  if (ptpconfig == NULL)
  {
    return HAL_ERROR;
  }

  tmpTSCR = ptpconfig->Timestamp |
            ((uint32_t)ptpconfig->TimestampUpdate << ETH_MACTSCR_TSUPDT_Pos) |
            ((uint32_t)ptpconfig->TimestampAll << ETH_MACTSCR_TSENALL_Pos) |
            ((uint32_t)ptpconfig->TimestampRolloverMode << ETH_MACTSCR_TSCTRLSSR_Pos) |
            ((uint32_t)ptpconfig->TimestampV2 << ETH_MACTSCR_TSVER2ENA_Pos) |
            ((uint32_t)ptpconfig->TimestampEthernet << ETH_MACTSCR_TSIPENA_Pos) |
            ((uint32_t)ptpconfig->TimestampIPv6 << ETH_MACTSCR_TSIPV6ENA_Pos) |
            ((uint32_t)ptpconfig->TimestampIPv4 << ETH_MACTSCR_TSIPV4ENA_Pos) |
            ((uint32_t)ptpconfig->TimestampEvent << ETH_MACTSCR_TSEVNTENA_Pos) |
            ((uint32_t)ptpconfig->TimestampMaster << ETH_MACTSCR_TSMSTRENA_Pos) |
            ((uint32_t)ptpconfig->TimestampSnapshots << ETH_MACTSCR_SNAPTYPSEL_Pos) |
            ((uint32_t)ptpconfig->TimestampFilter << ETH_MACTSCR_TSENMACADDR_Pos) |
            ((uint32_t)ptpconfig->TimestampChecksumCorrection << ETH_MACTSCR_CSC_Pos) |
            ((uint32_t)ptpconfig->TimestampStatusMode << ETH_MACTSCR_TXTSSTSM_Pos);

  /* Write to MACTSCR */
  MODIFY_REG(heth->Instance->MACTSCR, ETH_MACTSCR_MASK, tmpTSCR);

  /* Enable Timestamp */
  SET_BIT(heth->Instance->MACTSCR, ETH_MACTSCR_TSENA);
  WRITE_REG(heth->Instance->MACSSIR, ptpconfig->TimestampSubsecondInc);
  WRITE_REG(heth->Instance->MACTSAR, ptpconfig->TimestampAddend);

  /* Enable Timestamp */
  if (ptpconfig->TimestampAddendUpdate == ENABLE)
  {
    SET_BIT(heth->Instance->MACTSCR, ETH_MACTSCR_TSADDREG);
    while ((heth->Instance->MACTSCR & ETH_MACTSCR_TSADDREG) != 0) {}
  }

  /* Enable Update mode */
  if (ptpconfig->TimestampUpdateMode == ENABLE)
  {
    SET_BIT(heth->Instance->MACTSCR, ETH_MACTSCR_TSCFUPDT);
  }

  /* Initialize Time */
  time.Seconds = 0;
  time.NanoSeconds = 0;
  HAL_ETH_PTP_SetTime(heth, &time);

  /* Ptp Init */
  SET_BIT(heth->Instance->MACTSCR, ETH_MACTSCR_TSINIT);

  /* Set PTP Configuration done */
  heth->IsPtpConfigured = HAL_ETH_PTP_CONFIGURATED;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Get the Ethernet PTP configuration.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  ptpconfig: pointer to a ETH_PTP_ConfigTypeDef structure that contains
  *         the configuration information for PTP
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_GetConfig(ETH_HandleTypeDef *heth, ETH_PTP_ConfigTypeDef *ptpconfig)
{
  if (ptpconfig == NULL)
  {
    return HAL_ERROR;
  }
  ptpconfig->Timestamp = READ_BIT(heth->Instance->MACTSCR, ETH_MACTSCR_TSENA);
  ptpconfig->TimestampUpdate = ((READ_BIT(heth->Instance->MACTSCR,
                                          ETH_MACTSCR_TSCFUPDT) >> ETH_MACTSCR_TSUPDT_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampAll = ((READ_BIT(heth->Instance->MACTSCR,
                                       ETH_MACTSCR_TSENALL) >> ETH_MACTSCR_TSENALL_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampRolloverMode = ((READ_BIT(heth->Instance->MACTSCR,
                                                ETH_MACTSCR_TSCTRLSSR) >> ETH_MACTSCR_TSCTRLSSR_Pos) > 0U)
                                     ? ENABLE : DISABLE;
  ptpconfig->TimestampV2 = ((READ_BIT(heth->Instance->MACTSCR,
                                      ETH_MACTSCR_TSVER2ENA) >> ETH_MACTSCR_TSVER2ENA_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampEthernet = ((READ_BIT(heth->Instance->MACTSCR,
                                            ETH_MACTSCR_TSIPENA) >> ETH_MACTSCR_TSIPENA_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampIPv6 = ((READ_BIT(heth->Instance->MACTSCR,
                                        ETH_MACTSCR_TSIPV6ENA) >> ETH_MACTSCR_TSIPV6ENA_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampIPv4 = ((READ_BIT(heth->Instance->MACTSCR,
                                        ETH_MACTSCR_TSIPV4ENA) >> ETH_MACTSCR_TSIPV4ENA_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampEvent = ((READ_BIT(heth->Instance->MACTSCR,
                                         ETH_MACTSCR_TSEVNTENA) >> ETH_MACTSCR_TSEVNTENA_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampMaster = ((READ_BIT(heth->Instance->MACTSCR,
                                          ETH_MACTSCR_TSMSTRENA) >> ETH_MACTSCR_TSMSTRENA_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampSnapshots = ((READ_BIT(heth->Instance->MACTSCR,
                                             ETH_MACTSCR_SNAPTYPSEL) >> ETH_MACTSCR_SNAPTYPSEL_Pos) > 0U)
                                  ? ENABLE : DISABLE;
  ptpconfig->TimestampFilter = ((READ_BIT(heth->Instance->MACTSCR,
                                          ETH_MACTSCR_TSENMACADDR) >> ETH_MACTSCR_TSENMACADDR_Pos) > 0U)
                               ? ENABLE : DISABLE;
  ptpconfig->TimestampChecksumCorrection = ((READ_BIT(heth->Instance->MACTSCR,
                                                      ETH_MACTSCR_CSC) >> ETH_MACTSCR_CSC_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampStatusMode = ((READ_BIT(heth->Instance->MACTSCR,
                                              ETH_MACTSCR_TXTSSTSM) >> ETH_MACTSCR_TXTSSTSM_Pos) > 0U)
                                   ? ENABLE : DISABLE;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Set Seconds and Nanoseconds for the Ethernet PTP registers.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  heth: pointer to a ETH_TimeTypeDef structure that contains
  *         time to set
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_SetTime(ETH_HandleTypeDef *heth, ETH_TimeTypeDef *time)
{
  if (heth->IsPtpConfigured == HAL_ETH_PTP_CONFIGURATED)
  {
    /* Set Seconds */
    heth->Instance->MACSTSUR = time->Seconds;

    /* Set NanoSeconds */
    heth->Instance->MACSTNUR = time->NanoSeconds;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Get Seconds and Nanoseconds for the Ethernet PTP registers.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  heth: pointer to a ETH_TimeTypeDef structure that contains
  *         time to get
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_GetTime(ETH_HandleTypeDef *heth, ETH_TimeTypeDef *time)
{
  if (heth->IsPtpConfigured == HAL_ETH_PTP_CONFIGURATED)
  {
    /* Get Seconds */
    time->Seconds = heth->Instance->MACSTSUR;

    /* Get NanoSeconds */
    time->NanoSeconds = heth->Instance->MACSTNUR;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Update time for the Ethernet PTP registers.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  timeupdate: pointer to a ETH_TIMEUPDATETypeDef structure that contains
  *         the time update information
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_AddTimeOffset(ETH_HandleTypeDef *heth, ETH_PtpUpdateTypeDef ptpoffsettype,
                                            ETH_TimeTypeDef *timeoffset)
{
  if (heth->IsPtpConfigured == HAL_ETH_PTP_CONFIGURATED)
  {
    if (ptpoffsettype ==  HAL_ETH_PTP_NEGATIVE_UPDATE)
    {
      /* Set Seconds update */
      heth->Instance->MACSTSUR = ETH_MACSTSUR_VALUE - timeoffset->Seconds + 1U;

      if (READ_BIT(heth->Instance->MACTSCR, ETH_MACTSCR_TSCTRLSSR) == ETH_MACTSCR_TSCTRLSSR)
      {
        /* Set nanoSeconds update */
        heth->Instance->MACSTNUR = ETH_MACSTNUR_VALUE - timeoffset->NanoSeconds;
      }
      else
      {
        /* Set nanoSeconds update */
        heth->Instance->MACSTNUR = ETH_MACSTSUR_VALUE - timeoffset->NanoSeconds + 1U;
      }
    }
    else
    {
      /* Set Seconds update */
      heth->Instance->MACSTSUR = timeoffset->Seconds;
      /* Set nanoSeconds update */
      heth->Instance->MACSTNUR = timeoffset->NanoSeconds;
    }

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Insert Timestamp in transmission.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  txtimestampconf: Enable or Disable timestamp in transmission
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_InsertTxTimestamp(ETH_HandleTypeDef *heth)
{
  ETH_TxDescListTypeDef *dmatxdesclist = &heth->TxDescList;
  uint32_t descidx = dmatxdesclist->CurTxDesc;
  ETH_DMADescTypeDef *dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

  if (heth->IsPtpConfigured == HAL_ETH_PTP_CONFIGURATED)
  {
    /* Enable Time Stamp transmission */
    SET_BIT(dmatxdesc->DESC2, ETH_DMATXNDESCRF_TTSE);

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Get transmission timestamp.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  timestamp: pointer to ETH_TIMESTAMPTypeDef structure that contains
  *         transmission timestamp
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_GetTxTimestamp(ETH_HandleTypeDef *heth, ETH_TimeStampTypeDef *timestamp)
{
  ETH_TxDescListTypeDef *dmatxdesclist = &heth->TxDescList;
  uint32_t idx =       dmatxdesclist->releaseIndex;
  ETH_DMADescTypeDef *dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[idx];

  if (heth->IsPtpConfigured == HAL_ETH_PTP_CONFIGURATED)
  {
    /* Get timestamp low */
    timestamp->TimeStampLow = dmatxdesc->DESC0;
    /* Get timestamp high */
    timestamp->TimeStampHigh = dmatxdesc->DESC1;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Get receive timestamp.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  timestamp: pointer to ETH_TIMESTAMPTypeDef structure that contains
  *         receive timestamp
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_PTP_GetRxTimestamp(ETH_HandleTypeDef *heth, ETH_TimeStampTypeDef *timestamp)
{
  if (heth->IsPtpConfigured == HAL_ETH_PTP_CONFIGURATED)
  {
    /* Get timestamp low */
    timestamp->TimeStampLow = heth->RxDescList.TimeStamp.TimeStampLow;
    /* Get timestamp high */
    timestamp->TimeStampHigh = heth->RxDescList.TimeStamp.TimeStampHigh;

    /* Return function status */
    return HAL_OK;
  }
  else
  {
    /* Return function status */
    return HAL_ERROR;
  }
}

/**
  * @brief  Register the Tx Ptp callback.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  txPtpCallback: Function to handle Ptp transmission
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_RegisterTxPtpCallback(ETH_HandleTypeDef *heth, pETH_txPtpCallbackTypeDef txPtpCallback)
{
  if (txPtpCallback == NULL)
  {
    /* No buffer to save */
    return HAL_ERROR;
  }
  /* Set Function to handle Tx Ptp */
  heth->txPtpCallback = txPtpCallback;

  return HAL_OK;
}

/**
  * @brief  Unregister the Tx Ptp callback.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_UnRegisterTxPtpCallback(ETH_HandleTypeDef *heth)
{
  /* Set function to allocate buffer */
  heth->txPtpCallback = HAL_ETH_TxPtpCallback;

  return HAL_OK;
}

/**
  * @brief  Tx Ptp callback.
  * @param  buff: pointer to application buffer
  * @retval None
  */
__weak void HAL_ETH_TxPtpCallback(uint32_t *buff, ETH_TimeStampTypeDef *timestamp)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(buff);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_TxPtpCallback could be implemented in the user file
  */
}
#endif  /* HAL_ETH_USE_PTP */

/**
  * @brief  This function handles ETH interrupt request.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
void HAL_ETH_IRQHandler(ETH_HandleTypeDef *heth)
{
  uint32_t macirqenable;
  /* Packet received */
  if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMACSR_RI))
  {
    if (__HAL_ETH_DMA_GET_IT_SOURCE(heth, ETH_DMACIER_RIE))
    {
      /* Clear the Eth DMA Rx IT pending bits */
      __HAL_ETH_DMA_CLEAR_IT(heth, ETH_DMACSR_RI | ETH_DMACSR_NIS);

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /*Call registered Receive complete callback*/
      heth->RxCpltCallback(heth);
#else
      /* Receive complete callback */
      HAL_ETH_RxCpltCallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */
    }
  }

  /* Packet transmitted */
  if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMACSR_TI))
  {
    if (__HAL_ETH_DMA_GET_IT_SOURCE(heth, ETH_DMACIER_TIE))
    {
      /* Clear the Eth DMA Tx IT pending bits */
      __HAL_ETH_DMA_CLEAR_IT(heth, ETH_DMACSR_TI | ETH_DMACSR_NIS);

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /*Call registered Transmit complete callback*/
      heth->TxCpltCallback(heth);
#else
      /* Transfer complete callback */
      HAL_ETH_TxCpltCallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */
    }
  }


  /* ETH DMA Error */
  if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMACSR_AIS))
  {
    if (__HAL_ETH_DMA_GET_IT_SOURCE(heth, ETH_DMACIER_AIE))
    {
      heth->ErrorCode |= HAL_ETH_ERROR_DMA;

      /* if fatal bus error occurred */
      if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMACSR_FBE))
      {
        /* Get DMA error code  */
        heth->DMAErrorCode = READ_BIT(heth->Instance->DMACSR, (ETH_DMACSR_FBE | ETH_DMACSR_TPS | ETH_DMACSR_RPS));

        /* Disable all interrupts */
        __HAL_ETH_DMA_DISABLE_IT(heth, ETH_DMACIER_NIE | ETH_DMACIER_AIE);

        /* Set HAL state to ERROR */
        heth->gState = HAL_ETH_STATE_ERROR;
      }
      else
      {
        /* Get DMA error status  */
        heth->DMAErrorCode = READ_BIT(heth->Instance->DMACSR, (ETH_DMACSR_CDE | ETH_DMACSR_ETI | ETH_DMACSR_RWT |
                                                               ETH_DMACSR_RBU | ETH_DMACSR_AIS));

        /* Clear the interrupt summary flag */
        __HAL_ETH_DMA_CLEAR_IT(heth, (ETH_DMACSR_CDE | ETH_DMACSR_ETI | ETH_DMACSR_RWT |
                                      ETH_DMACSR_RBU | ETH_DMACSR_AIS));
      }
#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /* Call registered Error callback*/
      heth->ErrorCallback(heth);
#else
      /* Ethernet DMA Error callback */
      HAL_ETH_ErrorCallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */

    }
  }

  /* ETH MAC Error IT */
  macirqenable = heth->Instance->MACIER;
  if (((macirqenable & ETH_MACIER_RXSTSIE) == ETH_MACIER_RXSTSIE) || \
      ((macirqenable & ETH_MACIER_TXSTSIE) == ETH_MACIER_TXSTSIE))
  {
    heth->ErrorCode |= HAL_ETH_ERROR_MAC;

    /* Get MAC Rx Tx status and clear Status register pending bit */
    heth->MACErrorCode = READ_REG(heth->Instance->MACRXTXSR);

    heth->gState = HAL_ETH_STATE_ERROR;

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
    /* Call registered Error callback*/
    heth->ErrorCallback(heth);
#else
    /* Ethernet Error callback */
    HAL_ETH_ErrorCallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */

    heth->MACErrorCode = (uint32_t)(0x0U);
  }

  /* ETH PMT IT */
  if (__HAL_ETH_MAC_GET_IT(heth, ETH_MAC_PMT_IT))
  {
    /* Get MAC Wake-up source and clear the status register pending bit */
    heth->MACWakeUpEvent = READ_BIT(heth->Instance->MACPCSR, (ETH_MACPCSR_RWKPRCVD | ETH_MACPCSR_MGKPRCVD));

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
    /* Call registered PMT callback*/
    heth->PMTCallback(heth);
#else
    /* Ethernet PMT callback */
    HAL_ETH_PMTCallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */

    heth->MACWakeUpEvent = (uint32_t)(0x0U);
  }

  /* ETH EEE IT */
  if (__HAL_ETH_MAC_GET_IT(heth, ETH_MAC_LPI_IT))
  {
    /* Get MAC LPI interrupt source and clear the status register pending bit */
    heth->MACLPIEvent = READ_BIT(heth->Instance->MACPCSR, 0x0000000FU);

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
    /* Call registered EEE callback*/
    heth->EEECallback(heth);
#else
    /* Ethernet EEE callback */
    HAL_ETH_EEECallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */

    heth->MACLPIEvent = (uint32_t)(0x0U);
  }

#if defined(DUAL_CORE)
  if (HAL_GetCurrentCPUID() == CM7_CPUID)
  {
    /* check ETH WAKEUP exti flag */
    if (__HAL_ETH_WAKEUP_EXTI_GET_FLAG(ETH_WAKEUP_EXTI_LINE) != (uint32_t)RESET)
    {
      /* Clear ETH WAKEUP Exti pending bit */
      __HAL_ETH_WAKEUP_EXTI_CLEAR_FLAG(ETH_WAKEUP_EXTI_LINE);
#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /* Call registered WakeUp callback*/
      heth->WakeUpCallback(heth);
#else
      /* ETH WAKEUP callback */
      HAL_ETH_WakeUpCallback(heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */
    }
  }
  else
  {
    /* check ETH WAKEUP exti flag */
    if (__HAL_ETH_WAKEUP_EXTID2_GET_FLAG(ETH_WAKEUP_EXTI_LINE) != (uint32_t)RESET)
    {
      /* Clear ETH WAKEUP Exti pending bit */
      __HAL_ETH_WAKEUP_EXTID2_CLEAR_FLAG(ETH_WAKEUP_EXTI_LINE);
#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
      /* Call registered WakeUp callback*/
      heth->WakeUpCallback(heth);
#else
      /* ETH WAKEUP callback */
      HAL_ETH_WakeUpCallback(heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */
    }
  }
#else /* USE_HAL_ETH_REGISTER_CALLBACKS */
  /* check ETH WAKEUP exti flag */
  if (__HAL_ETH_WAKEUP_EXTI_GET_FLAG(ETH_WAKEUP_EXTI_LINE) != (uint32_t)RESET)
  {
    /* Clear ETH WAKEUP Exti pending bit */
    __HAL_ETH_WAKEUP_EXTI_CLEAR_FLAG(ETH_WAKEUP_EXTI_LINE);
#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
    /* Call registered WakeUp callback*/
    heth->WakeUpCallback(heth);
#else
    /* ETH WAKEUP callback */
    HAL_ETH_WakeUpCallback(heth);
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */
  }
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */
}

/**
  * @brief  Tx Transfer completed callbacks.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_TxCpltCallback could be implemented in the user file
  */
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_RxCpltCallback could be implemented in the user file
  */
}

/**
  * @brief  Ethernet transfer error callbacks
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_ErrorCallback could be implemented in the user file
  */
}

/**
  * @brief  Ethernet Power Management module IT callback
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_PMTCallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_PMTCallback could be implemented in the user file
  */
}

/**
  * @brief  Energy Efficient Etherent IT callback
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_EEECallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
  the HAL_ETH_EEECallback could be implemented in the user file
  */
}

/**
  * @brief  ETH WAKEUP interrupt callback
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
__weak void HAL_ETH_WakeUpCallback(ETH_HandleTypeDef *heth)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ETH_WakeUpCallback could be implemented in the user file
   */
}

/**
  * @brief  Read a PHY register
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  PHYAddr: PHY port address, must be a value from 0 to 31
  * @param  PHYReg: PHY register address, must be a value from 0 to 31
  * @param pRegValue: parameter to hold read value
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_ReadPHYRegister(ETH_HandleTypeDef *heth, uint32_t PHYAddr, uint32_t PHYReg,
                                          uint32_t *pRegValue)
{
  uint32_t tickstart;
  uint32_t tmpreg;

  /* Check for the Busy flag */
  if (READ_BIT(heth->Instance->MACMDIOAR, ETH_MACMDIOAR_MB) != (uint32_t)RESET)
  {
    return HAL_ERROR;
  }

  /* Get the  MACMDIOAR value */
  WRITE_REG(tmpreg, heth->Instance->MACMDIOAR);

  /* Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the read mode
     - Set the MII Busy bit */

  MODIFY_REG(tmpreg, ETH_MACMDIOAR_PA, (PHYAddr << 21));
  MODIFY_REG(tmpreg, ETH_MACMDIOAR_RDA, (PHYReg << 16));
  MODIFY_REG(tmpreg, ETH_MACMDIOAR_MOC, ETH_MACMDIOAR_MOC_RD);
  SET_BIT(tmpreg, ETH_MACMDIOAR_MB);

  /* Write the result value into the MDII Address register */
  WRITE_REG(heth->Instance->MACMDIOAR, tmpreg);

  tickstart = HAL_GetTick();

  /* Wait for the Busy flag */
  while (READ_BIT(heth->Instance->MACMDIOAR, ETH_MACMDIOAR_MB) > 0U)
  {
    if (((HAL_GetTick() - tickstart) > ETH_MDIO_BUS_TIMEOUT))
    {
      return HAL_ERROR;
    }
  }

  /* Get MACMIIDR value */
  WRITE_REG(*pRegValue, (uint16_t)heth->Instance->MACMDIODR);

  return HAL_OK;
}


/**
  * @brief  Writes to a PHY register.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  PHYAddr: PHY port address, must be a value from 0 to 31
  * @param  PHYReg: PHY register address, must be a value from 0 to 31
  * @param  RegValue: the value to write
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_WritePHYRegister(ETH_HandleTypeDef *heth, uint32_t PHYAddr, uint32_t PHYReg,
                                           uint32_t RegValue)
{
  uint32_t tickstart;
  uint32_t tmpreg;

  /* Check for the Busy flag */
  if (READ_BIT(heth->Instance->MACMDIOAR, ETH_MACMDIOAR_MB) != (uint32_t)RESET)
  {
    return HAL_ERROR;
  }

  /* Get the  MACMDIOAR value */
  WRITE_REG(tmpreg, heth->Instance->MACMDIOAR);

  /* Prepare the MDIO Address Register value
     - Set the PHY device address
     - Set the PHY register address
     - Set the write mode
     - Set the MII Busy bit */

  MODIFY_REG(tmpreg, ETH_MACMDIOAR_PA, (PHYAddr << 21));
  MODIFY_REG(tmpreg, ETH_MACMDIOAR_RDA, (PHYReg << 16));
  MODIFY_REG(tmpreg, ETH_MACMDIOAR_MOC, ETH_MACMDIOAR_MOC_WR);
  SET_BIT(tmpreg, ETH_MACMDIOAR_MB);


  /* Give the value to the MII data register */
  WRITE_REG(ETH->MACMDIODR, (uint16_t)RegValue);

  /* Write the result value into the MII Address register */
  WRITE_REG(ETH->MACMDIOAR, tmpreg);

  tickstart = HAL_GetTick();

  /* Wait for the Busy flag */
  while (READ_BIT(heth->Instance->MACMDIOAR, ETH_MACMDIOAR_MB) > 0U)
  {
    if (((HAL_GetTick() - tickstart) > ETH_MDIO_BUS_TIMEOUT))
    {
      return HAL_ERROR;
    }
  }

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup ETH_Exported_Functions_Group3 Peripheral Control functions
  *  @brief   ETH control functions
  *
@verbatim
  ==============================================================================
                      ##### Peripheral Control functions #####
  ==============================================================================
  [..]
    This subsection provides a set of functions allowing to control the ETH
    peripheral.

@endverbatim
  * @{
  */
/**
  * @brief  Get the configuration of the MAC and MTL subsystems.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  macconf: pointer to a ETH_MACConfigTypeDef structure that will hold
  *         the configuration of the MAC.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_ETH_GetMACConfig(ETH_HandleTypeDef *heth, ETH_MACConfigTypeDef *macconf)
{
  if (macconf == NULL)
  {
    return HAL_ERROR;
  }

  /* Get MAC parameters */
  macconf->PreambleLength = READ_BIT(heth->Instance->MACCR, ETH_MACCR_PRELEN);
  macconf->DeferralCheck = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_DC) >> 4) > 0U) ? ENABLE : DISABLE;
  macconf->BackOffLimit = READ_BIT(heth->Instance->MACCR, ETH_MACCR_BL);
  macconf->RetryTransmission = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_DR) >> 8) == 0U) ? ENABLE : DISABLE;
  macconf->CarrierSenseDuringTransmit = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_DCRS) >> 9) > 0U)
                                        ? ENABLE : DISABLE;
  macconf->ReceiveOwn = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_DO) >> 10) == 0U) ? ENABLE : DISABLE;
  macconf->CarrierSenseBeforeTransmit = ((READ_BIT(heth->Instance->MACCR,
                                                   ETH_MACCR_ECRSFD) >> 11) > 0U) ? ENABLE : DISABLE;
  macconf->LoopbackMode = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_LM) >> 12) > 0U) ? ENABLE : DISABLE;
  macconf->DuplexMode = READ_BIT(heth->Instance->MACCR, ETH_MACCR_DM);
  macconf->Speed = READ_BIT(heth->Instance->MACCR, ETH_MACCR_FES);
  macconf->JumboPacket = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_JE) >> 16) > 0U) ? ENABLE : DISABLE;
  macconf->Jabber = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_JD) >> 17) == 0U) ? ENABLE : DISABLE;
  macconf->Watchdog = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_WD) >> 19) == 0U) ? ENABLE : DISABLE;
  macconf->AutomaticPadCRCStrip = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_ACS) >> 20) > 0U) ? ENABLE : DISABLE;
  macconf->CRCStripTypePacket = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_CST) >> 21) > 0U) ? ENABLE : DISABLE;
  macconf->Support2KPacket = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_S2KP) >> 22) > 0U) ? ENABLE : DISABLE;
  macconf->GiantPacketSizeLimitControl = ((READ_BIT(heth->Instance->MACCR,
                                                    ETH_MACCR_GPSLCE) >> 23) > 0U) ? ENABLE : DISABLE;
  macconf->InterPacketGapVal = READ_BIT(heth->Instance->MACCR, ETH_MACCR_IPG);
  macconf->ChecksumOffload = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_IPC) >> 27) > 0U) ? ENABLE : DISABLE;
  macconf->SourceAddrControl = READ_BIT(heth->Instance->MACCR, ETH_MACCR_SARC);

  macconf->GiantPacketSizeLimit = READ_BIT(heth->Instance->MACECR, ETH_MACECR_GPSL);
  macconf->CRCCheckingRxPackets = ((READ_BIT(heth->Instance->MACECR, ETH_MACECR_DCRCC) >> 16) == 0U) ? ENABLE : DISABLE;
  macconf->SlowProtocolDetect = ((READ_BIT(heth->Instance->MACECR, ETH_MACECR_SPEN) >> 17) > 0U) ? ENABLE : DISABLE;
  macconf->UnicastSlowProtocolPacketDetect = ((READ_BIT(heth->Instance->MACECR,
                                                        ETH_MACECR_USP) >> 18) > 0U) ? ENABLE : DISABLE;
  macconf->ExtendedInterPacketGap = ((READ_BIT(heth->Instance->MACECR, ETH_MACECR_EIPGEN) >> 24) > 0U)
                                    ? ENABLE : DISABLE;
  macconf->ExtendedInterPacketGapVal = READ_BIT(heth->Instance->MACECR, ETH_MACECR_EIPG) >> 25;


  macconf->ProgrammableWatchdog = ((READ_BIT(heth->Instance->MACWTR, ETH_MACWTR_PWE) >> 8) > 0U) ? ENABLE : DISABLE;
  macconf->WatchdogTimeout = READ_BIT(heth->Instance->MACWTR, ETH_MACWTR_WTO);

  macconf->TransmitFlowControl = ((READ_BIT(heth->Instance->MACTFCR, ETH_MACTFCR_TFE) >> 1) > 0U) ? ENABLE : DISABLE;
  macconf->ZeroQuantaPause = ((READ_BIT(heth->Instance->MACTFCR, ETH_MACTFCR_DZPQ) >> 7) == 0U) ? ENABLE : DISABLE;
  macconf->PauseLowThreshold = READ_BIT(heth->Instance->MACTFCR, ETH_MACTFCR_PLT);
  macconf->PauseTime = (READ_BIT(heth->Instance->MACTFCR, ETH_MACTFCR_PT) >> 16);


  macconf->ReceiveFlowControl = (READ_BIT(heth->Instance->MACRFCR, ETH_MACRFCR_RFE) > 0U) ? ENABLE : DISABLE;
  macconf->UnicastPausePacketDetect = ((READ_BIT(heth->Instance->MACRFCR, ETH_MACRFCR_UP) >> 1) > 0U)
                                      ? ENABLE : DISABLE;

  macconf->TransmitQueueMode = READ_BIT(heth->Instance->MTLTQOMR, (ETH_MTLTQOMR_TTC | ETH_MTLTQOMR_TSF));

  macconf->ReceiveQueueMode = READ_BIT(heth->Instance->MTLRQOMR, (ETH_MTLRQOMR_RTC | ETH_MTLRQOMR_RSF));
  macconf->ForwardRxUndersizedGoodPacket = ((READ_BIT(heth->Instance->MTLRQOMR,
                                                      ETH_MTLRQOMR_FUP) >> 3) > 0U) ? ENABLE : DISABLE;
  macconf->ForwardRxErrorPacket = ((READ_BIT(heth->Instance->MTLRQOMR, ETH_MTLRQOMR_FEP) >> 4) > 0U) ? ENABLE : DISABLE;
  macconf->DropTCPIPChecksumErrorPacket = ((READ_BIT(heth->Instance->MTLRQOMR,
                                                     ETH_MTLRQOMR_DISTCPEF) >> 6) == 0U) ? ENABLE : DISABLE;

  return HAL_OK;
}

/**
  * @brief  Get the configuration of the DMA.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  dmaconf: pointer to a ETH_DMAConfigTypeDef structure that will hold
  *         the configuration of the ETH DMA.
  * @retval HAL Status
  */
HAL_StatusTypeDef HAL_ETH_GetDMAConfig(ETH_HandleTypeDef *heth, ETH_DMAConfigTypeDef *dmaconf)
{
  if (dmaconf == NULL)
  {
    return HAL_ERROR;
  }

  dmaconf->AddressAlignedBeats = ((READ_BIT(heth->Instance->DMASBMR, ETH_DMASBMR_AAL) >> 12) > 0U) ? ENABLE : DISABLE;
  dmaconf->BurstMode = READ_BIT(heth->Instance->DMASBMR, ETH_DMASBMR_FB | ETH_DMASBMR_MB);
  dmaconf->RebuildINCRxBurst = ((READ_BIT(heth->Instance->DMASBMR, ETH_DMASBMR_RB) >> 15) > 0U) ? ENABLE : DISABLE;

  dmaconf->DMAArbitration = READ_BIT(heth->Instance->DMAMR, (ETH_DMAMR_TXPR | ETH_DMAMR_PR | ETH_DMAMR_DA));

  dmaconf->PBLx8Mode = ((READ_BIT(heth->Instance->DMACCR, ETH_DMACCR_8PBL) >> 16) > 0U) ? ENABLE : DISABLE;
  dmaconf->MaximumSegmentSize = READ_BIT(heth->Instance->DMACCR, ETH_DMACCR_MSS);

  dmaconf->FlushRxPacket = ((READ_BIT(heth->Instance->DMACRCR,  ETH_DMACRCR_RPF) >> 31) > 0U) ? ENABLE : DISABLE;
  dmaconf->RxDMABurstLength = READ_BIT(heth->Instance->DMACRCR, ETH_DMACRCR_RPBL);

  dmaconf->SecondPacketOperate = ((READ_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_OSP) >> 4) > 0U) ? ENABLE : DISABLE;
  dmaconf->TCPSegmentation = ((READ_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_TSE) >> 12) > 0U) ? ENABLE : DISABLE;
  dmaconf->TxDMABurstLength = READ_BIT(heth->Instance->DMACTCR, ETH_DMACTCR_TPBL);
  return HAL_OK;
}

/**
  * @brief  Set the MAC configuration.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  macconf: pointer to a ETH_MACConfigTypeDef structure that contains
  *         the configuration of the MAC.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_SetMACConfig(ETH_HandleTypeDef *heth,  ETH_MACConfigTypeDef *macconf)
{
  if (macconf == NULL)
  {
    return HAL_ERROR;
  }

  if (heth->gState == HAL_ETH_STATE_READY)
  {
    ETH_SetMACConfig(heth, macconf);

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Set the ETH DMA configuration.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  dmaconf: pointer to a ETH_DMAConfigTypeDef structure that will hold
  *         the configuration of the ETH DMA.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_SetDMAConfig(ETH_HandleTypeDef *heth,  ETH_DMAConfigTypeDef *dmaconf)
{
  if (dmaconf == NULL)
  {
    return HAL_ERROR;
  }

  if (heth->gState == HAL_ETH_STATE_READY)
  {
    ETH_SetDMAConfig(heth, dmaconf);

    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}

/**
  * @brief  Configures the Clock range of ETH MDIO interface.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
void HAL_ETH_SetMDIOClockRange(ETH_HandleTypeDef *heth)
{
  uint32_t hclk;
  uint32_t tmpreg;

  /* Get the ETHERNET MACMDIOAR value */
  tmpreg = (heth->Instance)->MACMDIOAR;

  /* Clear CSR Clock Range bits */
  tmpreg &= ~ETH_MACMDIOAR_CR;

  /* Get hclk frequency value */
  hclk = HAL_RCC_GetHCLKFreq();

  /* Set CR bits depending on hclk value */
  if ((hclk >= 20000000U) && (hclk < 35000000U))
  {
    /* CSR Clock Range between 20-35 MHz */
    tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV16;
  }
  else if ((hclk >= 35000000U) && (hclk < 60000000U))
  {
    /* CSR Clock Range between 35-60 MHz */
    tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV26;
  }
  else if ((hclk >= 60000000U) && (hclk < 100000000U))
  {
    /* CSR Clock Range between 60-100 MHz */
    tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV42;
  }
  else if ((hclk >= 100000000U) && (hclk < 150000000U))
  {
    /* CSR Clock Range between 100-150 MHz */
    tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV62;
  }
  else /* (hclk >= 150000000)&&(hclk <= 200000000) */
  {
    /* CSR Clock Range between 150-200 MHz */
    tmpreg |= (uint32_t)ETH_MACMDIOAR_CR_DIV102;
  }

  /* Configure the CSR Clock Range */
  (heth->Instance)->MACMDIOAR = (uint32_t)tmpreg;
}

/**
  * @brief  Set the ETH MAC (L2) Filters configuration.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pFilterConfig: pointer to a ETH_MACFilterConfigTypeDef structure that contains
  *         the configuration of the ETH MAC filters.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_SetMACFilterConfig(ETH_HandleTypeDef *heth, ETH_MACFilterConfigTypeDef *pFilterConfig)
{
  uint32_t filterconfig;

  if (pFilterConfig == NULL)
  {
    return HAL_ERROR;
  }

  filterconfig = ((uint32_t)pFilterConfig->PromiscuousMode |
                  ((uint32_t)pFilterConfig->HashUnicast << 1) |
                  ((uint32_t)pFilterConfig->HashMulticast << 2)  |
                  ((uint32_t)pFilterConfig->DestAddrInverseFiltering << 3) |
                  ((uint32_t)pFilterConfig->PassAllMulticast << 4) |
                  ((uint32_t)((pFilterConfig->BroadcastFilter == DISABLE) ? 1U : 0U) << 5) |
                  ((uint32_t)pFilterConfig->SrcAddrInverseFiltering << 8) |
                  ((uint32_t)pFilterConfig->SrcAddrFiltering << 9) |
                  ((uint32_t)pFilterConfig->HachOrPerfectFilter << 10) |
                  ((uint32_t)pFilterConfig->ReceiveAllMode << 31) |
                  pFilterConfig->ControlPacketsFilter);

  MODIFY_REG(heth->Instance->MACPFR, ETH_MACPFR_MASK, filterconfig);

  return HAL_OK;
}

/**
  * @brief  Get the ETH MAC (L2) Filters configuration.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pFilterConfig: pointer to a ETH_MACFilterConfigTypeDef structure that will hold
  *         the configuration of the ETH MAC filters.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_GetMACFilterConfig(ETH_HandleTypeDef *heth, ETH_MACFilterConfigTypeDef *pFilterConfig)
{
  if (pFilterConfig == NULL)
  {
    return HAL_ERROR;
  }

  pFilterConfig->PromiscuousMode = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_PR)) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->HashUnicast = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_HUC) >> 1) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->HashMulticast = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_HMC) >> 2) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->DestAddrInverseFiltering = ((READ_BIT(heth->Instance->MACPFR,
                                                       ETH_MACPFR_DAIF) >> 3) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->PassAllMulticast = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_PM) >> 4) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->BroadcastFilter = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_DBF) >> 5) == 0U) ? ENABLE : DISABLE;
  pFilterConfig->ControlPacketsFilter = READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_PCF);
  pFilterConfig->SrcAddrInverseFiltering = ((READ_BIT(heth->Instance->MACPFR,
                                                      ETH_MACPFR_SAIF) >> 8) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->SrcAddrFiltering = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_SAF) >> 9) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->HachOrPerfectFilter = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_HPF) >> 10) > 0U)
                                       ? ENABLE : DISABLE;
  pFilterConfig->ReceiveAllMode = ((READ_BIT(heth->Instance->MACPFR, ETH_MACPFR_RA) >> 31) > 0U) ? ENABLE : DISABLE;

  return HAL_OK;
}

/**
  * @brief  Set the source MAC Address to be matched.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  AddrNbr: The MAC address to configure
  *          This parameter must be a value of the following:
  *            ETH_MAC_ADDRESS1
  *            ETH_MAC_ADDRESS2
  *            ETH_MAC_ADDRESS3
  * @param  pMACAddr: Pointer to MAC address buffer data (6 bytes)
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_SetSourceMACAddrMatch(ETH_HandleTypeDef *heth, uint32_t AddrNbr, uint8_t *pMACAddr)
{
  uint32_t macaddrlr;
  uint32_t macaddrhr;

  if (pMACAddr == NULL)
  {
    return HAL_ERROR;
  }

  /* Get mac addr high reg offset */
  macaddrhr = ((uint32_t) &(heth->Instance->MACA0HR) + AddrNbr);
  /* Get mac addr low reg offset */
  macaddrlr = ((uint32_t) &(heth->Instance->MACA0LR) + AddrNbr);

  /* Set MAC addr bits 32 to 47 */
  (*(__IO uint32_t *)macaddrhr) = (((uint32_t)(pMACAddr[5]) << 8) | (uint32_t)pMACAddr[4]);
  /* Set MAC addr bits 0 to 31 */
  (*(__IO uint32_t *)macaddrlr) = (((uint32_t)(pMACAddr[3]) << 24) | ((uint32_t)(pMACAddr[2]) << 16) |
                                   ((uint32_t)(pMACAddr[1]) << 8) | (uint32_t)pMACAddr[0]);

  /* Enable address and set source address bit */
  (*(__IO uint32_t *)macaddrhr) |= (ETH_MACAHR_SA | ETH_MACAHR_AE);

  return HAL_OK;
}

/**
  * @brief  Set the ETH Hash Table Value.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pHashTable: pointer to a table of two 32 bit values, that contains
  *         the 64 bits of the hash table.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_ETH_SetHashTable(ETH_HandleTypeDef *heth, uint32_t *pHashTable)
{
  if (pHashTable == NULL)
  {
    return HAL_ERROR;
  }

  heth->Instance->MACHT0R = pHashTable[0];
  heth->Instance->MACHT1R = pHashTable[1];

  return HAL_OK;
}

/**
  * @brief  Set the VLAN Identifier for Rx packets
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  ComparisonBits: 12 or 16 bit comparison mode
            must be a value of @ref ETH_VLAN_Tag_Comparison
  * @param  VLANIdentifier: VLAN Identifier value
  * @retval None
  */
void HAL_ETH_SetRxVLANIdentifier(ETH_HandleTypeDef *heth, uint32_t ComparisonBits, uint32_t VLANIdentifier)
{
  if (ComparisonBits == ETH_VLANTAGCOMPARISON_16BIT)
  {
    MODIFY_REG(heth->Instance->MACVTR, ETH_MACVTR_VL, VLANIdentifier);
    CLEAR_BIT(heth->Instance->MACVTR, ETH_MACVTR_ETV);
  }
  else
  {
    MODIFY_REG(heth->Instance->MACVTR, ETH_MACVTR_VL_VID, VLANIdentifier);
    SET_BIT(heth->Instance->MACVTR, ETH_MACVTR_ETV);
  }
}

/**
  * @brief  Enters the Power down mode.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pPowerDownConfig: a pointer to ETH_PowerDownConfigTypeDef structure
  *         that contains the Power Down configuration
  * @retval None.
  */
void HAL_ETH_EnterPowerDownMode(ETH_HandleTypeDef *heth, ETH_PowerDownConfigTypeDef *pPowerDownConfig)
{
  uint32_t powerdownconfig;

  powerdownconfig = (((uint32_t)pPowerDownConfig->MagicPacket << 1) |
                     ((uint32_t)pPowerDownConfig->WakeUpPacket << 2) |
                     ((uint32_t)pPowerDownConfig->GlobalUnicast << 9) |
                     ((uint32_t)pPowerDownConfig->WakeUpForward << 10) |
                     ETH_MACPCSR_PWRDWN);

  /* Enable PMT interrupt */
  __HAL_ETH_MAC_ENABLE_IT(heth, ETH_MACIER_PMTIE);

  MODIFY_REG(heth->Instance->MACPCSR, ETH_MACPCSR_MASK, powerdownconfig);
}

/**
  * @brief  Exits from the Power down mode.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None.
  */
void HAL_ETH_ExitPowerDownMode(ETH_HandleTypeDef *heth)
{
  /* clear wake up sources */
  CLEAR_BIT(heth->Instance->MACPCSR, ETH_MACPCSR_RWKPKTEN | ETH_MACPCSR_MGKPKTEN | ETH_MACPCSR_GLBLUCAST |
            ETH_MACPCSR_RWKPFE);

  if (READ_BIT(heth->Instance->MACPCSR, ETH_MACPCSR_PWRDWN) != (uint32_t)RESET)
  {
    /* Exit power down mode */
    CLEAR_BIT(heth->Instance->MACPCSR, ETH_MACPCSR_PWRDWN);
  }

  /* Disable PMT interrupt */
  __HAL_ETH_MAC_DISABLE_IT(heth, ETH_MACIER_PMTIE);
}

/**
  * @brief  Set the WakeUp filter.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pFilter: pointer to filter registers values
  * @param  Count: number of filter registers, must be from 1 to 8.
  * @retval None.
  */
HAL_StatusTypeDef HAL_ETH_SetWakeUpFilter(ETH_HandleTypeDef *heth, uint32_t *pFilter, uint32_t Count)
{
  uint32_t regindex;

  if (pFilter == NULL)
  {
    return HAL_ERROR;
  }

  /* Reset Filter Pointer */
  SET_BIT(heth->Instance->MACPCSR, ETH_MACPCSR_RWKFILTRST);

  /* Wake up packet filter config */
  for (regindex = 0; regindex < Count; regindex++)
  {
    /* Write filter regs */
    WRITE_REG(heth->Instance->MACRWKPFR, pFilter[regindex]);
  }

  return HAL_OK;
}

/**
  * @}
  */

/** @defgroup ETH_Exported_Functions_Group4 Peripheral State and Errors functions
  *  @brief   ETH State and Errors functions
  *
@verbatim
  ==============================================================================
                 ##### Peripheral State and Errors functions #####
  ==============================================================================
 [..]
   This subsection provides a set of functions allowing to return the State of
   ETH communication process, return Peripheral Errors occurred during communication
   process


@endverbatim
  * @{
  */

/**
  * @brief  Returns the ETH state.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL state
  */
HAL_ETH_StateTypeDef HAL_ETH_GetState(ETH_HandleTypeDef *heth)
{
  return heth->gState;
}

/**
  * @brief  Returns the ETH error code
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval ETH Error Code
  */
uint32_t HAL_ETH_GetError(ETH_HandleTypeDef *heth)
{
  return heth->ErrorCode;
}

/**
  * @brief  Returns the ETH DMA error code
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval ETH DMA Error Code
  */
uint32_t HAL_ETH_GetDMAError(ETH_HandleTypeDef *heth)
{
  return heth->DMAErrorCode;
}

/**
  * @brief  Returns the ETH MAC error code
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval ETH MAC Error Code
  */
uint32_t HAL_ETH_GetMACError(ETH_HandleTypeDef *heth)
{
  return heth->MACErrorCode;
}

/**
  * @brief  Returns the ETH MAC WakeUp event source
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval ETH MAC WakeUp event source
  */
uint32_t HAL_ETH_GetMACWakeUpSource(ETH_HandleTypeDef *heth)
{
  return heth->MACWakeUpEvent;
}

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup ETH_Private_Functions   ETH Private Functions
  * @{
  */


static void ETH_SetMACConfig(ETH_HandleTypeDef *heth,  ETH_MACConfigTypeDef *macconf)
{
  uint32_t macregval;

  /*------------------------ MACCR Configuration --------------------*/
  macregval = (macconf->InterPacketGapVal |
               macconf->SourceAddrControl |
               ((uint32_t)macconf->ChecksumOffload << 27) |
               ((uint32_t)macconf->GiantPacketSizeLimitControl << 23) |
               ((uint32_t)macconf->Support2KPacket  << 22) |
               ((uint32_t)macconf->CRCStripTypePacket << 21) |
               ((uint32_t)macconf->AutomaticPadCRCStrip << 20) |
               ((uint32_t)((macconf->Watchdog == DISABLE) ? 1U : 0U) << 19) |
               ((uint32_t)((macconf->Jabber == DISABLE) ? 1U : 0U) << 17) |
               ((uint32_t)macconf->JumboPacket << 16) |
               macconf->Speed |
               macconf->DuplexMode |
               ((uint32_t)macconf->LoopbackMode << 12) |
               ((uint32_t)macconf->CarrierSenseBeforeTransmit << 11) |
               ((uint32_t)((macconf->ReceiveOwn == DISABLE) ? 1U : 0U) << 10) |
               ((uint32_t)macconf->CarrierSenseDuringTransmit << 9) |
               ((uint32_t)((macconf->RetryTransmission == DISABLE) ? 1U : 0U) << 8) |
               macconf->BackOffLimit |
               ((uint32_t)macconf->DeferralCheck << 4) |
               macconf->PreambleLength);

  /* Write to MACCR */
  MODIFY_REG(heth->Instance->MACCR, ETH_MACCR_MASK, macregval);

  /*------------------------ MACECR Configuration --------------------*/
  macregval = ((macconf->ExtendedInterPacketGapVal << 25) |
               ((uint32_t)macconf->ExtendedInterPacketGap << 24) |
               ((uint32_t)macconf->UnicastSlowProtocolPacketDetect << 18) |
               ((uint32_t)macconf->SlowProtocolDetect << 17) |
               ((uint32_t)((macconf->CRCCheckingRxPackets == DISABLE) ? 1U : 0U) << 16) |
               macconf->GiantPacketSizeLimit);

  /* Write to MACECR */
  MODIFY_REG(heth->Instance->MACECR, ETH_MACECR_MASK, macregval);

  /*------------------------ MACWTR Configuration --------------------*/
  macregval = (((uint32_t)macconf->ProgrammableWatchdog << 8) |
               macconf->WatchdogTimeout);

  /* Write to MACWTR */
  MODIFY_REG(heth->Instance->MACWTR, ETH_MACWTR_MASK, macregval);

  /*------------------------ MACTFCR Configuration --------------------*/
  macregval = (((uint32_t)macconf->TransmitFlowControl << 1) |
               macconf->PauseLowThreshold |
               ((uint32_t)((macconf->ZeroQuantaPause == DISABLE) ? 1U : 0U) << 7) |
               (macconf->PauseTime << 16));

  /* Write to MACTFCR */
  MODIFY_REG(heth->Instance->MACTFCR, ETH_MACTFCR_MASK, macregval);

  /*------------------------ MACRFCR Configuration --------------------*/
  macregval = ((uint32_t)macconf->ReceiveFlowControl |
               ((uint32_t)macconf->UnicastPausePacketDetect << 1));

  /* Write to MACRFCR */
  MODIFY_REG(heth->Instance->MACRFCR, ETH_MACRFCR_MASK, macregval);

  /*------------------------ MTLTQOMR Configuration --------------------*/
  /* Write to MTLTQOMR */
  MODIFY_REG(heth->Instance->MTLTQOMR, ETH_MTLTQOMR_MASK, macconf->TransmitQueueMode);

  /*------------------------ MTLRQOMR Configuration --------------------*/
  macregval = (macconf->ReceiveQueueMode |
               ((uint32_t)((macconf->DropTCPIPChecksumErrorPacket == DISABLE) ? 1U : 0U) << 6) |
               ((uint32_t)macconf->ForwardRxErrorPacket << 4) |
               ((uint32_t)macconf->ForwardRxUndersizedGoodPacket << 3));

  /* Write to MTLRQOMR */
  MODIFY_REG(heth->Instance->MTLRQOMR, ETH_MTLRQOMR_MASK, macregval);
}

static void ETH_SetDMAConfig(ETH_HandleTypeDef *heth,  ETH_DMAConfigTypeDef *dmaconf)
{
  uint32_t dmaregval;

  /*------------------------ DMAMR Configuration --------------------*/
  MODIFY_REG(heth->Instance->DMAMR, ETH_DMAMR_MASK, dmaconf->DMAArbitration);

  /*------------------------ DMASBMR Configuration --------------------*/
  dmaregval = (((uint32_t)dmaconf->AddressAlignedBeats << 12) |
               dmaconf->BurstMode |
               ((uint32_t)dmaconf->RebuildINCRxBurst << 15));

  MODIFY_REG(heth->Instance->DMASBMR, ETH_DMASBMR_MASK, dmaregval);

  /*------------------------ DMACCR Configuration --------------------*/
  dmaregval = (((uint32_t)dmaconf->PBLx8Mode << 16) |
               dmaconf->MaximumSegmentSize);

  MODIFY_REG(heth->Instance->DMACCR, ETH_DMACCR_MASK, dmaregval);

  /*------------------------ DMACTCR Configuration --------------------*/
  dmaregval = (dmaconf->TxDMABurstLength |
               ((uint32_t)dmaconf->SecondPacketOperate << 4) |
               ((uint32_t)dmaconf->TCPSegmentation << 12));

  MODIFY_REG(heth->Instance->DMACTCR, ETH_DMACTCR_MASK, dmaregval);

  /*------------------------ DMACRCR Configuration --------------------*/
  dmaregval = (((uint32_t)dmaconf->FlushRxPacket  << 31) |
               dmaconf->RxDMABurstLength);

  /* Write to DMACRCR */
  MODIFY_REG(heth->Instance->DMACRCR, ETH_DMACRCR_MASK, dmaregval);
}

/**
  * @brief  Configures Ethernet MAC and DMA with default parameters.
  *         called by HAL_ETH_Init() API.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval HAL status
  */
static void ETH_MACDMAConfig(ETH_HandleTypeDef *heth)
{
  ETH_MACConfigTypeDef macDefaultConf;
  ETH_DMAConfigTypeDef dmaDefaultConf;

  /*--------------- ETHERNET MAC registers default Configuration --------------*/
  macDefaultConf.AutomaticPadCRCStrip = ENABLE;
  macDefaultConf.BackOffLimit = ETH_BACKOFFLIMIT_10;
  macDefaultConf.CarrierSenseBeforeTransmit = DISABLE;
  macDefaultConf.CarrierSenseDuringTransmit = DISABLE;
  macDefaultConf.ChecksumOffload = ENABLE;
  macDefaultConf.CRCCheckingRxPackets = ENABLE;
  macDefaultConf.CRCStripTypePacket = ENABLE;
  macDefaultConf.DeferralCheck = DISABLE;
  macDefaultConf.DropTCPIPChecksumErrorPacket = ENABLE;
  macDefaultConf.DuplexMode = ETH_FULLDUPLEX_MODE;
  macDefaultConf.ExtendedInterPacketGap = DISABLE;
  macDefaultConf.ExtendedInterPacketGapVal = 0x0;
  macDefaultConf.ForwardRxErrorPacket = DISABLE;
  macDefaultConf.ForwardRxUndersizedGoodPacket = DISABLE;
  macDefaultConf.GiantPacketSizeLimit = 0x618;
  macDefaultConf.GiantPacketSizeLimitControl = DISABLE;
  macDefaultConf.InterPacketGapVal = ETH_INTERPACKETGAP_96BIT;
  macDefaultConf.Jabber = ENABLE;
  macDefaultConf.JumboPacket = DISABLE;
  macDefaultConf.LoopbackMode = DISABLE;
  macDefaultConf.PauseLowThreshold = ETH_PAUSELOWTHRESHOLD_MINUS_4;
  macDefaultConf.PauseTime = 0x0;
  macDefaultConf.PreambleLength = ETH_PREAMBLELENGTH_7;
  macDefaultConf.ProgrammableWatchdog = DISABLE;
  macDefaultConf.ReceiveFlowControl = DISABLE;
  macDefaultConf.ReceiveOwn = ENABLE;
  macDefaultConf.ReceiveQueueMode = ETH_RECEIVESTOREFORWARD;
  macDefaultConf.RetryTransmission = ENABLE;
  macDefaultConf.SlowProtocolDetect = DISABLE;
  macDefaultConf.SourceAddrControl = ETH_SOURCEADDRESS_REPLACE_ADDR0;
  macDefaultConf.Speed = ETH_SPEED_100M;
  macDefaultConf.Support2KPacket = DISABLE;
  macDefaultConf.TransmitQueueMode = ETH_TRANSMITSTOREFORWARD;
  macDefaultConf.TransmitFlowControl = DISABLE;
  macDefaultConf.UnicastPausePacketDetect = DISABLE;
  macDefaultConf.UnicastSlowProtocolPacketDetect = DISABLE;
  macDefaultConf.Watchdog = ENABLE;
  macDefaultConf.WatchdogTimeout =  ETH_MACWTR_WTO_2KB;
  macDefaultConf.ZeroQuantaPause = ENABLE;

  /* MAC default configuration */
  ETH_SetMACConfig(heth, &macDefaultConf);

  /*--------------- ETHERNET DMA registers default Configuration --------------*/
  dmaDefaultConf.AddressAlignedBeats = ENABLE;
  dmaDefaultConf.BurstMode = ETH_BURSTLENGTH_FIXED;
  dmaDefaultConf.DMAArbitration = ETH_DMAARBITRATION_RX1_TX1;
  dmaDefaultConf.FlushRxPacket = DISABLE;
  dmaDefaultConf.PBLx8Mode = DISABLE;
  dmaDefaultConf.RebuildINCRxBurst = DISABLE;
  dmaDefaultConf.RxDMABurstLength = ETH_RXDMABURSTLENGTH_32BEAT;
  dmaDefaultConf.SecondPacketOperate = DISABLE;
  dmaDefaultConf.TxDMABurstLength = ETH_TXDMABURSTLENGTH_32BEAT;
  dmaDefaultConf.TCPSegmentation = DISABLE;
  dmaDefaultConf.MaximumSegmentSize = ETH_SEGMENT_SIZE_DEFAULT;

  /* DMA default configuration */
  ETH_SetDMAConfig(heth, &dmaDefaultConf);
}


/**
  * @brief  Initializes the DMA Tx descriptors.
  *         called by HAL_ETH_Init() API.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
static void ETH_DMATxDescListInit(ETH_HandleTypeDef *heth)
{
  ETH_DMADescTypeDef *dmatxdesc;
  uint32_t i;

  /* Fill each DMATxDesc descriptor with the right values */
  for (i = 0; i < (uint32_t)ETH_TX_DESC_CNT; i++)
  {
    dmatxdesc = heth->Init.TxDesc + i;

    WRITE_REG(dmatxdesc->DESC0, 0x0);
    WRITE_REG(dmatxdesc->DESC1, 0x0);
    WRITE_REG(dmatxdesc->DESC2, 0x0);
    WRITE_REG(dmatxdesc->DESC3, 0x0);

    WRITE_REG(heth->TxDescList.TxDesc[i], (uint32_t)dmatxdesc);

  }

  heth->TxDescList.CurTxDesc = 0;

  /* Set Transmit Descriptor Ring Length */
  WRITE_REG(heth->Instance->DMACTDRLR, (ETH_TX_DESC_CNT - 1U));

  /* Set Transmit Descriptor List Address */
  WRITE_REG(heth->Instance->DMACTDLAR, (uint32_t) heth->Init.TxDesc);

  /* Set Transmit Descriptor Tail pointer */
  WRITE_REG(heth->Instance->DMACTDTPR, (uint32_t) heth->Init.TxDesc);
}

/**
  * @brief  Initializes the DMA Rx descriptors in chain mode.
  *         called by HAL_ETH_Init() API.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
static void ETH_DMARxDescListInit(ETH_HandleTypeDef *heth)
{
  ETH_DMADescTypeDef *dmarxdesc;
  uint32_t i;

  for (i = 0; i < (uint32_t)ETH_RX_DESC_CNT; i++)
  {
    dmarxdesc =  heth->Init.RxDesc + i;

    WRITE_REG(dmarxdesc->DESC0, 0x0);
    WRITE_REG(dmarxdesc->DESC1, 0x0);
    WRITE_REG(dmarxdesc->DESC2, 0x0);
    WRITE_REG(dmarxdesc->DESC3, 0x0);
    WRITE_REG(dmarxdesc->BackupAddr0, 0x0);
    WRITE_REG(dmarxdesc->BackupAddr1, 0x0);


    /* Set Rx descritors addresses */
    WRITE_REG(heth->RxDescList.RxDesc[i], (uint32_t)dmarxdesc);

  }

  WRITE_REG(heth->RxDescList.RxDescIdx, 0);
  WRITE_REG(heth->RxDescList.RxDescCnt, 0);
  WRITE_REG(heth->RxDescList.RxBuildDescIdx, 0);
  WRITE_REG(heth->RxDescList.RxBuildDescCnt, 0);
  WRITE_REG(heth->RxDescList.ItMode, 0);

  /* Set Receive Descriptor Ring Length */
  WRITE_REG(heth->Instance->DMACRDRLR, ((uint32_t)(ETH_RX_DESC_CNT - 1U)));

  /* Set Receive Descriptor List Address */
  WRITE_REG(heth->Instance->DMACRDLAR, (uint32_t) heth->Init.RxDesc);

  /* Set Receive Descriptor Tail pointer Address */
  WRITE_REG(heth->Instance->DMACRDTPR, ((uint32_t)(heth->Init.RxDesc + (uint32_t)(ETH_RX_DESC_CNT - 1U))));
}

/**
  * @brief  Prepare Tx DMA descriptor before transmission.
  *         called by HAL_ETH_Transmit_IT and HAL_ETH_Transmit_IT() API.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  pTxConfig: Tx packet configuration
  * @param  ItMode: Enable or disable Tx EOT interrept
  * @retval Status
  */
static uint32_t ETH_Prepare_Tx_Descriptors(ETH_HandleTypeDef *heth, ETH_TxPacketConfig *pTxConfig, uint32_t ItMode)
{
  ETH_TxDescListTypeDef *dmatxdesclist = &heth->TxDescList;
  uint32_t descidx = dmatxdesclist->CurTxDesc;
  uint32_t firstdescidx = dmatxdesclist->CurTxDesc;
  uint32_t idx;
  uint32_t descnbr = 0;
  ETH_DMADescTypeDef *dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

  ETH_BufferTypeDef  *txbuffer = pTxConfig->TxBuffer;
  uint32_t           bd_count = 0;

  /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
  if ((READ_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCWBF_OWN) == ETH_DMATXNDESCWBF_OWN)
      || (dmatxdesclist->PacketAddress[descidx] != NULL))
  {
    return HAL_ETH_ERROR_BUSY;
  }

  /***************************************************************************/
  /*****************    Context descriptor configuration (Optional) **********/
  /***************************************************************************/
  /* If VLAN tag is enabled for this packet */
  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_VLANTAG) != (uint32_t)RESET)
  {
    /* Set vlan tag value */
    MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXCDESC_VT, pTxConfig->VlanTag);
    /* Set vlan tag valid bit */
    SET_BIT(dmatxdesc->DESC3, ETH_DMATXCDESC_VLTV);
    /* Set the descriptor as the vlan input source */
    SET_BIT(heth->Instance->MACVIR, ETH_MACVIR_VLTI);

    /* if inner VLAN is enabled */
    if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_INNERVLANTAG) != (uint32_t)RESET)
    {
      /* Set inner vlan tag value */
      MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXCDESC_IVT, (pTxConfig->InnerVlanTag << 16));
      /* Set inner vlan tag valid bit */
      SET_BIT(dmatxdesc->DESC3, ETH_DMATXCDESC_IVLTV);

      /* Set Vlan Tag control */
      MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXCDESC_IVTIR, pTxConfig->InnerVlanCtrl);

      /* Set the descriptor as the inner vlan input source */
      SET_BIT(heth->Instance->MACIVIR, ETH_MACIVIR_VLTI);
      /* Enable double VLAN processing */
      SET_BIT(heth->Instance->MACVTR, ETH_MACVTR_EDVLP);
    }
  }

  /* if tcp segmentation is enabled for this packet */
  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_TSO) != (uint32_t)RESET)
  {
    /* Set MSS value */
    MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXCDESC_MSS, pTxConfig->MaxSegmentSize);
    /* Set MSS valid bit */
    SET_BIT(dmatxdesc->DESC3, ETH_DMATXCDESC_TCMSSV);
  }

  if ((READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_VLANTAG) != (uint32_t)RESET)
      || (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_TSO) != (uint32_t)RESET))
  {
    /* Set as context descriptor */
    SET_BIT(dmatxdesc->DESC3, ETH_DMATXCDESC_CTXT);
    /* Ensure rest of descriptor is written to RAM before the OWN bit */
    __DMB();
    /* Set own bit */
    SET_BIT(dmatxdesc->DESC3, ETH_DMATXCDESC_OWN);
    /* Increment current tx descriptor index */
    INCR_TX_DESC_INDEX(descidx, 1U);
    /* Get current descriptor address */
    dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

    descnbr += 1U;

    /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
    if (READ_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCWBF_OWN) == ETH_DMATXNDESCWBF_OWN)
    {
      dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[firstdescidx];
      /* Ensure rest of descriptor is written to RAM before the OWN bit */
      __DMB();
      /* Clear own bit */
      CLEAR_BIT(dmatxdesc->DESC3, ETH_DMATXCDESC_OWN);

      return HAL_ETH_ERROR_BUSY;
    }
  }

  /***************************************************************************/
  /*****************    Normal descriptors configuration     *****************/
  /***************************************************************************/

  descnbr += 1U;

  /* Set header or buffer 1 address */
  WRITE_REG(dmatxdesc->DESC0, (uint32_t)txbuffer->buffer);
  /* Set header or buffer 1 Length */
  MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_B1L, txbuffer->len);

  if (txbuffer->next != NULL)
  {
    txbuffer = txbuffer->next;
    /* Set buffer 2 address */
    WRITE_REG(dmatxdesc->DESC1, (uint32_t)txbuffer->buffer);
    /* Set buffer 2 Length */
    MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_B2L, (txbuffer->len << 16));
  }
  else
  {
    WRITE_REG(dmatxdesc->DESC1, 0x0);
    /* Set buffer 2 Length */
    MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_B2L, 0x0U);
  }

  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_TSO) != (uint32_t)RESET)
  {
    /* Set TCP Header length */
    MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_THL, (pTxConfig->TCPHeaderLen << 19));
    /* Set TCP payload length */
    MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_TPL, pTxConfig->PayloadLen);
    /* Set TCP Segmentation Enabled bit */
    SET_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_TSE);
  }
  else
  {
    MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_FL, pTxConfig->Length);

    if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_CSUM) != (uint32_t)RESET)
    {
      MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_CIC, pTxConfig->ChecksumCtrl);
    }

    if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_CRCPAD) != (uint32_t)RESET)
    {
      MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_CPC, pTxConfig->CRCPadCtrl);
    }
  }

  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_VLANTAG) != (uint32_t)RESET)
  {
    /* Set Vlan Tag control */
    MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_VTIR, pTxConfig->VlanCtrl);
  }

  /* Mark it as First Descriptor */
  SET_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_FD);
  /* Mark it as NORMAL descriptor */
  CLEAR_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_CTXT);
  /* Ensure rest of descriptor is written to RAM before the OWN bit */
  __DMB();
  /* set OWN bit of FIRST descriptor */
  SET_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_OWN);

  /* If source address insertion/replacement is enabled for this packet */
  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_SAIC) != (uint32_t)RESET)
  {
    MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_SAIC, pTxConfig->SrcAddrCtrl);
  }

  /* only if the packet is split into more than one descriptors > 1 */
  while (txbuffer->next != NULL)
  {
    /* Clear the LD bit of previous descriptor */
    CLEAR_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_LD);
    /* Increment current tx descriptor index */
    INCR_TX_DESC_INDEX(descidx, 1U);
    /* Get current descriptor address */
    dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

    /* Clear the FD bit of new Descriptor */
    CLEAR_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_FD);

    /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
    if ((READ_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_OWN) == ETH_DMATXNDESCRF_OWN)
        || (dmatxdesclist->PacketAddress[descidx] != NULL))
    {
      descidx = firstdescidx;
      dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

      /* clear previous desc own bit */
      for (idx = 0; idx < descnbr; idx ++)
      {
        /* Ensure rest of descriptor is written to RAM before the OWN bit */
        __DMB();

        CLEAR_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_OWN);

        /* Increment current tx descriptor index */
        INCR_TX_DESC_INDEX(descidx, 1U);
        /* Get current descriptor address */
        dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];
      }

      return HAL_ETH_ERROR_BUSY;
    }

    descnbr += 1U;

    /* Get the next Tx buffer in the list */
    txbuffer = txbuffer->next;

    /* Set header or buffer 1 address */
    WRITE_REG(dmatxdesc->DESC0, (uint32_t)txbuffer->buffer);
    /* Set header or buffer 1 Length */
    MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_B1L, txbuffer->len);

    if (txbuffer->next != NULL)
    {
      /* Get the next Tx buffer in the list */
      txbuffer = txbuffer->next;
      /* Set buffer 2 address */
      WRITE_REG(dmatxdesc->DESC1, (uint32_t)txbuffer->buffer);
      /* Set buffer 2 Length */
      MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_B2L, (txbuffer->len << 16));
    }
    else
    {
      WRITE_REG(dmatxdesc->DESC1, 0x0);
      /* Set buffer 2 Length */
      MODIFY_REG(dmatxdesc->DESC2, ETH_DMATXNDESCRF_B2L, 0x0U);
    }

    if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_TSO) != (uint32_t)RESET)
    {
      /* Set TCP payload length */
      MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_TPL, pTxConfig->PayloadLen);
      /* Set TCP Segmentation Enabled bit */
      SET_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_TSE);
    }
    else
    {
      /* Set the packet length */
      MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_FL, pTxConfig->Length);

      if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_CSUM) != (uint32_t)RESET)
      {
        /* Checksum Insertion Control */
        MODIFY_REG(dmatxdesc->DESC3, ETH_DMATXNDESCRF_CIC, pTxConfig->ChecksumCtrl);
      }
    }

    bd_count += 1U;

    /* Ensure rest of descriptor is written to RAM before the OWN bit */
    __DMB();
    /* Set Own bit */
    SET_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_OWN);
    /* Mark it as NORMAL descriptor */
    CLEAR_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_CTXT);
  }

  if (ItMode != ((uint32_t)RESET))
  {
    /* Set Interrupt on completion bit */
    SET_BIT(dmatxdesc->DESC2, ETH_DMATXNDESCRF_IOC);
  }
  else
  {
    /* Clear Interrupt on completion bit */
    CLEAR_BIT(dmatxdesc->DESC2, ETH_DMATXNDESCRF_IOC);
  }

  /* Mark it as LAST descriptor */
  SET_BIT(dmatxdesc->DESC3, ETH_DMATXNDESCRF_LD);
  /* Save the current packet address to expose it to the application */
  dmatxdesclist->PacketAddress[descidx] = dmatxdesclist->CurrentPacketAddress;

  dmatxdesclist->CurTxDesc = descidx;

  /* disable the interrupt */
  __disable_irq();

  dmatxdesclist->BuffersInUse += bd_count + 1U;

  /* Enable interrupts back */
  __enable_irq();


  /* Return function status */
  return HAL_ETH_ERROR_NONE;
}

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
static void ETH_InitCallbacksToDefault(ETH_HandleTypeDef *heth)
{
  /* Init the ETH Callback settings */
  heth->TxCpltCallback   = HAL_ETH_TxCpltCallback;    /* Legacy weak TxCpltCallback   */
  heth->RxCpltCallback   = HAL_ETH_RxCpltCallback;    /* Legacy weak RxCpltCallback   */
  heth->ErrorCallback    = HAL_ETH_ErrorCallback;     /* Legacy weak ErrorCallback */
  heth->PMTCallback      = HAL_ETH_PMTCallback;       /* Legacy weak PMTCallback      */
  heth->EEECallback      = HAL_ETH_EEECallback;       /* Legacy weak EEECallback      */
  heth->WakeUpCallback   = HAL_ETH_WakeUpCallback;    /* Legacy weak WakeUpCallback   */
  heth->rxLinkCallback   = HAL_ETH_RxLinkCallback;    /* Legacy weak RxLinkCallback   */
  heth->txFreeCallback   = HAL_ETH_TxFreeCallback;    /* Legacy weak TxFreeCallback   */
#ifdef HAL_ETH_USE_PTP
  heth->txPtpCallback    = HAL_ETH_TxPtpCallback;     /* Legacy weak TxPtpCallback   */
#endif /* HAL_ETH_USE_PTP */
  heth->rxAllocateCallback = HAL_ETH_RxAllocateCallback; /* Legacy weak RxAllocateCallback */
}
#endif /* USE_HAL_ETH_REGISTER_CALLBACKS */

/**
  * @}
  */

/**
  * @}
  */

#endif /* ETH */

#endif /* HAL_ETH_MODULE_ENABLED */

/**
  * @}
  */

