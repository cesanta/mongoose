/**
  ******************************************************************************
  * @file    stm32f4xx_hal_eth.c
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
  * Copyright (c) 2016 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/** @addtogroup STM32F4xx_HAL_Driver
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
#define ETH_MACCR_MASK          0xFFFB7F7CU
#define ETH_MACECR_MASK         0x3F077FFFU
#define ETH_MACFFR_MASK         0x800007FFU
#define ETH_MACWTR_MASK         0x0000010FU
#define ETH_MACTFCR_MASK        0xFFFF00F2U
#define ETH_MACRFCR_MASK        0x00000003U
#define ETH_MTLTQOMR_MASK       0x00000072U
#define ETH_MTLRQOMR_MASK       0x0000007BU
							    
#define ETH_DMAMR_MASK          0x00007802U
#define ETH_DMASBMR_MASK        0x0000D001U
#define ETH_DMACCR_MASK         0x00013FFFU
#define ETH_DMACTCR_MASK        0x003F1010U
#define ETH_DMACRCR_MASK        0x803F0000U
#define ETH_MACPMTCSR_MASK      (ETH_MACPMTCSR_PD | ETH_MACPMTCSR_WFE | \
                                 ETH_MACPMTCSR_MPE | ETH_MACPMTCSR_GU)

/* Timeout values */
#define ETH_SWRESET_TIMEOUT     500U
#define ETH_MDIO_BUS_TIMEOUT    1000U

#define ETH_DMARXDESC_ERRORS_MASK ((uint32_t)(ETH_DMARXDESC_DBE | ETH_DMARXDESC_RE | \
                                              ETH_DMARXDESC_OE  | ETH_DMARXDESC_RWT |\
                                              ETH_DMARXDESC_LC | ETH_DMARXDESC_CE |\
                                              ETH_DMARXDESC_DE | ETH_DMARXDESC_IPV4HCE))

#define ETH_MAC_US_TICK         1000000U

#define ETH_MACTSCR_MASK        0x0087FF2FU

#define ETH_PTPTSHR_VALUE       0xFFFFFFFFU
#define ETH_PTPTSLR_VALUE       0xBB9ACA00U

/* Ethernet MACMIIAR register Mask */
#define ETH_MACMIIAR_CR_MASK    0xFFFFFFE3U

/* Delay to wait when writing to some Ethernet registers */
#define ETH_REG_WRITE_DELAY     0x00000001U

/* ETHERNET MACCR register Mask */
#define ETH_MACCR_CLEAR_MASK    0xFF20810FU

/* ETHERNET MACFCR register Mask */
#define ETH_MACFCR_CLEAR_MASK   0x0000FF41U

/* ETHERNET DMAOMR register Mask */
#define ETH_DMAOMR_CLEAR_MASK   0xF8DE3F23U

/* ETHERNET MAC address offsets */
#define ETH_MAC_ADDR_HBASE      (uint32_t)(ETH_MAC_BASE + 0x40U)  /* ETHERNET MAC address high offset */
#define ETH_MAC_ADDR_LBASE      (uint32_t)(ETH_MAC_BASE + 0x44U)  /* ETHERNET MAC address low offset */

/* ETHERNET DMA Rx descriptors Frame length Shift */
#define  ETH_DMARXDESC_FRAMELENGTHSHIFT            16U
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
static void ETH_FlushTransmitFIFO(ETH_HandleTypeDef *heth);
static void ETH_MACAddressConfig(ETH_HandleTypeDef *heth, uint32_t MacAddr, uint8_t *Addr);

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

  /* Select MII or RMII Mode*/
  SYSCFG->PMC &= ~(SYSCFG_PMC_MII_RMII_SEL);
  SYSCFG->PMC |= (uint32_t)heth->Init.MediaInterface;
  /* Dummy read to sync SYSCFG with ETH */
  (void)SYSCFG->PMC;

  /* Ethernet Software reset */
  /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
  /* After reset all the registers holds their respective reset values */
  SET_BIT(heth->Instance->DMABMR, ETH_DMABMR_SR);

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Wait for software reset */
  while (READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_SR) > 0U)
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


  /*------------------ MAC, MTL and DMA default Configuration ----------------*/
  ETH_MACDMAConfig(heth);


  /*------------------ DMA Tx Descriptors Configuration ----------------------*/
  ETH_DMATxDescListInit(heth);

  /*------------------ DMA Rx Descriptors Configuration ----------------------*/
  ETH_DMARxDescListInit(heth);

  /*--------------------- ETHERNET MAC Address Configuration ------------------*/
  ETH_MACAddressConfig(heth, ETH_MAC_ADDRESS0, heth->Init.MACAddr);

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
  uint32_t tmpreg1;

  if (heth->gState == HAL_ETH_STATE_READY)
  {
    heth->gState = HAL_ETH_STATE_BUSY;

    /* Set nombre of descriptors to build */
    heth->RxDescList.RxBuildDescCnt = ETH_RX_DESC_CNT;

    /* Build all descriptors */
    ETH_UpdateDescriptor(heth);

    /* Enable the MAC transmission */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Enable the MAC reception */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Flush Transmit FIFO */
    ETH_FlushTransmitFIFO(heth);

    /* Enable the DMA transmission */
    SET_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_ST);

    /* Enable the DMA reception */
    SET_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_SR);

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
  uint32_t tmpreg1;

  if (heth->gState == HAL_ETH_STATE_READY)
  {
    heth->gState = HAL_ETH_STATE_BUSY;

    /* save IT mode to ETH Handle */
    heth->RxDescList.ItMode = 1U;
    /* Disable MMC Interrupts */
    SET_BIT(heth->Instance->MACIMR, ETH_MACIMR_TSTIM | ETH_MACIMR_PMTIM);

    /* Disable Rx MMC Interrupts */
    SET_BIT(heth->Instance->MMCRIMR, ETH_MMCRIMR_RGUFM | ETH_MMCRIMR_RFAEM | \
            ETH_MMCRIMR_RFCEM);

    /* Disable Tx MMC Interrupts */
    SET_BIT(heth->Instance->MMCTIMR, ETH_MMCTIMR_TGFM | ETH_MMCTIMR_TGFMSCM | \
            ETH_MMCTIMR_TGFSCM);

    /* Set nombre of descriptors to build */
    heth->RxDescList.RxBuildDescCnt = ETH_RX_DESC_CNT;

    /* Build all descriptors */
    ETH_UpdateDescriptor(heth);

    /* Enable the MAC transmission */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Enable the MAC reception */
    SET_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Flush Transmit FIFO */
    ETH_FlushTransmitFIFO(heth);

    /* Enable the DMA transmission */
    SET_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_ST);

    /* Enable the DMA reception */
    SET_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_SR);

    /* Enable ETH DMA interrupts:
    - Tx complete interrupt
    - Rx complete interrupt
    - Fatal bus interrupt
    */
    __HAL_ETH_DMA_ENABLE_IT(heth, (ETH_DMAIER_NISE | ETH_DMAIER_RIE | ETH_DMAIER_TIE  |
                                   ETH_DMAIER_FBEIE | ETH_DMAIER_AISE | ETH_DMAIER_RBUIE));

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
  uint32_t tmpreg1;

  if (heth->gState == HAL_ETH_STATE_STARTED)
  {
    /* Set the ETH peripheral state to BUSY */
    heth->gState = HAL_ETH_STATE_BUSY;
    /* Disable the DMA transmission */
    CLEAR_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_ST);

    /* Disable the DMA reception */
    CLEAR_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_SR);

    /* Disable the MAC reception */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Flush Transmit FIFO */
    ETH_FlushTransmitFIFO(heth);

    /* Disable the MAC transmission */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

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
  uint32_t tmpreg1;

  if (heth->gState == HAL_ETH_STATE_STARTED)
  {
    /* Set the ETH peripheral state to BUSY */
    heth->gState = HAL_ETH_STATE_BUSY;

    __HAL_ETH_DMA_DISABLE_IT(heth, (ETH_DMAIER_NISE | ETH_DMAIER_RIE | ETH_DMAIER_TIE  |
                                    ETH_DMAIER_FBEIE | ETH_DMAIER_AISE | ETH_DMAIER_RBUIE));

    /* Disable the DMA transmission */
    CLEAR_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_ST);

    /* Disable the DMA reception */
    CLEAR_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_SR);

    /* Disable the MAC reception */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_RE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Flush Transmit FIFO */
    ETH_FlushTransmitFIFO(heth);

    /* Disable the MAC transmission */
    CLEAR_BIT(heth->Instance->MACCR, ETH_MACCR_TE);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACCR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACCR = tmpreg1;

    /* Clear IOC bit to all Rx descriptors */
    for (descindex = 0; descindex < (uint32_t)ETH_RX_DESC_CNT; descindex++)
    {
      dmarxdesc = (ETH_DMADescTypeDef *)heth->RxDescList.RxDesc[descindex];
      SET_BIT(dmarxdesc->DESC1, ETH_DMARXDESC_DIC);
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
    WRITE_REG(heth->Instance->DMATPDR, (uint32_t)(heth->TxDescList.TxDesc[heth->TxDescList.CurTxDesc]));

    tickstart = HAL_GetTick();

    /* Wait for data to be transmitted or timeout occurred */
    while ((dmatxdesc->DESC0 & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
    {
      if ((heth->Instance->DMASR & ETH_DMASR_FBES) != (uint32_t)RESET)
      {
        heth->ErrorCode |= HAL_ETH_ERROR_DMA;
        heth->DMAErrorCode = heth->Instance->DMASR;
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
          dmatxdesc->DESC0 = (ETH_DMATXDESC_FS | ETH_DMATXDESC_LS);
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
    if (((heth->Instance)->DMASR & ETH_DMASR_TBUS) != (uint32_t)RESET)
    {
      /* Clear TBUS ETHERNET DMA flag */
      (heth->Instance)->DMASR = ETH_DMASR_TBUS;
      /* Resume DMA transmission*/
      (heth->Instance)->DMATPDR = 0U;
    }

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
  while ((READ_BIT(dmarxdesc->DESC0, ETH_DMARXDESC_OWN) == (uint32_t)RESET) && (desccnt < desccntmax)
         && (rxdataready == 0U))
  {
    if (READ_BIT(dmarxdesc->DESC0,  ETH_DMARXDESC_LS)  != (uint32_t)RESET)
    {
      /* Get timestamp high */
      heth->RxDescList.TimeStamp.TimeStampHigh = dmarxdesc->DESC6;
      /* Get timestamp low */
      heth->RxDescList.TimeStamp.TimeStampLow  = dmarxdesc->DESC7;
    }
    if ((READ_BIT(dmarxdesc->DESC0, ETH_DMARXDESC_FS) != (uint32_t)RESET) || (heth->RxDescList.pRxStart != NULL))
    {
      /* Check first descriptor */
      if (READ_BIT(dmarxdesc->DESC0, ETH_DMARXDESC_FS) != (uint32_t)RESET)
      {
        heth->RxDescList.RxDescCnt = 0;
        heth->RxDescList.RxDataLength = 0;
      }

      /* Check if last descriptor */
      bufflength = heth->Init.RxBuffLen;
      if (READ_BIT(dmarxdesc->DESC0, ETH_DMARXDESC_LS) != (uint32_t)RESET)
      {
        /* Get the Frame Length of the received packet: substruct 4 bytes of the CRC */
        bufflength = ((dmarxdesc->DESC0 & ETH_DMARXDESC_FL) >> ETH_DMARXDESC_FRAMELENGTHSHIFT) - 4U;

        /* Save Last descriptor index */
        heth->RxDescList.pRxLastRxDesc = dmarxdesc->DESC0;

        /* Packet ready */
        rxdataready = 1;
      }

      /* Link data */
      WRITE_REG(dmarxdesc->BackupAddr0, dmarxdesc->DESC2);
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
        WRITE_REG(dmarxdesc->DESC2, (uint32_t)buff);
      }
    }

    if (allocStatus != 0U)
    {
      if (heth->RxDescList.ItMode == 0U)
      {
        WRITE_REG(dmarxdesc->DESC1, ETH_DMARXDESC_DIC | ETH_RX_BUF_SIZE | ETH_DMARXDESC_RCH);
      }
      else
      {
        WRITE_REG(dmarxdesc->DESC1, ETH_RX_BUF_SIZE | ETH_DMARXDESC_RCH);
      }

      /* Before transferring the ownership to DMA, make sure that the RX descriptors bits writing
         is fully performed.
         The __DMB() instruction is added to avoid any potential compiler optimization that
         may lead to abnormal behavior. */
      __DMB();

      SET_BIT(dmarxdesc->DESC0, ETH_DMARXDESC_OWN);

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
    WRITE_REG(heth->Instance->DMARPDR, 0);

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
  *pErrorCode = READ_BIT(heth->RxDescList.pRxLastRxDesc, ETH_DMARXDESC_ERRORS_MASK);

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
      if ((heth->Init.TxDesc[idx].DESC0 & ETH_DMATXDESC_OWN) == 0U)
      {
#ifdef HAL_ETH_USE_PTP
        /* Get timestamp low */
        timestamp->TimeStampLow = heth->Init.TxDesc[idx].DESC6;
        /* Get timestamp high */
        timestamp->TimeStampHigh = heth->Init.TxDesc[idx].DESC7;
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
            ((uint32_t)ptpconfig->TimestampUpdate << ETH_PTPTSCR_TSFCU_Pos) |
            ((uint32_t)ptpconfig->TimestampAll << ETH_PTPTSCR_TSSARFE_Pos) |
            ((uint32_t)ptpconfig->TimestampRolloverMode << ETH_PTPTSCR_TSSSR_Pos) |
            ((uint32_t)ptpconfig->TimestampV2 << ETH_PTPTSCR_TSPTPPSV2E_Pos) |
            ((uint32_t)ptpconfig->TimestampEthernet << ETH_PTPTSCR_TSSPTPOEFE_Pos) |
            ((uint32_t)ptpconfig->TimestampIPv6 << ETH_PTPTSCR_TSSIPV6FE_Pos) |
            ((uint32_t)ptpconfig->TimestampIPv4 << ETH_PTPTSCR_TSSIPV4FE_Pos) |
            ((uint32_t)ptpconfig->TimestampEvent << ETH_PTPTSCR_TSSEME_Pos) |
            ((uint32_t)ptpconfig->TimestampMaster << ETH_PTPTSCR_TSSMRME_Pos) |
            ((uint32_t)ptpconfig->TimestampFilter << ETH_PTPTSCR_TSPFFMAE_Pos) |
            ((uint32_t)ptpconfig->TimestampClockType << ETH_PTPTSCR_TSCNT_Pos);

  /* Write to MACTSCR */
  MODIFY_REG(heth->Instance->PTPTSCR, ETH_MACTSCR_MASK, tmpTSCR);

  /* Enable Timestamp */
  SET_BIT(heth->Instance->PTPTSCR, ETH_PTPTSCR_TSE);
  WRITE_REG(heth->Instance->PTPSSIR, ptpconfig->TimestampSubsecondInc);
  WRITE_REG(heth->Instance->PTPTSAR, ptpconfig->TimestampAddend);

  /* Enable Timestamp */
  if (ptpconfig->TimestampAddendUpdate == ENABLE)
  {
    SET_BIT(heth->Instance->PTPTSCR, ETH_PTPTSCR_TSARU);
    while ((heth->Instance->PTPTSCR & ETH_PTPTSCR_TSARU) != 0) {}
  }

  /* Enable Update mode */
  if (ptpconfig->TimestampUpdateMode == ENABLE)
  {
    SET_BIT(heth->Instance->PTPTSCR, ETH_PTPTSCR_TSFCU);
  }

  /* Initialize Time */
  time.Seconds = 0;
  time.NanoSeconds = 0;
  HAL_ETH_PTP_SetTime(heth, &time);

  /* Ptp Init */
  SET_BIT(heth->Instance->PTPTSCR, ETH_PTPTSCR_TSSTI);

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
  ptpconfig->Timestamp = READ_BIT(heth->Instance->PTPTSCR, ETH_PTPTSCR_TSE);
  ptpconfig->TimestampUpdate = ((READ_BIT(heth->Instance->PTPTSCR,
                                          ETH_PTPTSCR_TSFCU) >> ETH_PTPTSCR_TSFCU_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampAll = ((READ_BIT(heth->Instance->PTPTSCR,
                                       ETH_PTPTSCR_TSSARFE) >> ETH_PTPTSCR_TSSARFE_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampRolloverMode = ((READ_BIT(heth->Instance->PTPTSCR,
                                                ETH_PTPTSCR_TSSSR) >> ETH_PTPTSCR_TSSSR_Pos) > 0U)
                                     ? ENABLE : DISABLE;
  ptpconfig->TimestampV2 = ((READ_BIT(heth->Instance->PTPTSCR,
                                      ETH_PTPTSCR_TSPTPPSV2E) >> ETH_PTPTSCR_TSPTPPSV2E_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampEthernet = ((READ_BIT(heth->Instance->PTPTSCR,
                                            ETH_PTPTSCR_TSSPTPOEFE) >> ETH_PTPTSCR_TSSPTPOEFE_Pos) > 0U)
                                 ? ENABLE : DISABLE;
  ptpconfig->TimestampIPv6 = ((READ_BIT(heth->Instance->PTPTSCR,
                                        ETH_PTPTSCR_TSSIPV6FE) >> ETH_PTPTSCR_TSSIPV6FE_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampIPv4 = ((READ_BIT(heth->Instance->PTPTSCR,
                                        ETH_PTPTSCR_TSSIPV4FE) >> ETH_PTPTSCR_TSSIPV4FE_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampEvent = ((READ_BIT(heth->Instance->PTPTSCR,
                                         ETH_PTPTSCR_TSSEME) >> ETH_PTPTSCR_TSSEME_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampMaster = ((READ_BIT(heth->Instance->PTPTSCR,
                                          ETH_PTPTSCR_TSSMRME) >> ETH_PTPTSCR_TSSMRME_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampFilter = ((READ_BIT(heth->Instance->PTPTSCR,
                                          ETH_PTPTSCR_TSPFFMAE) >> ETH_PTPTSCR_TSPFFMAE_Pos) > 0U) ? ENABLE : DISABLE;
  ptpconfig->TimestampClockType = ((READ_BIT(heth->Instance->PTPTSCR,
                                             ETH_PTPTSCR_TSCNT) >> ETH_PTPTSCR_TSCNT_Pos) > 0U) ? ENABLE : DISABLE;

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
    heth->Instance->PTPTSHUR = time->Seconds;

    /* Set NanoSeconds */
    heth->Instance->PTPTSLUR = time->NanoSeconds;

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
    time->Seconds = heth->Instance->PTPTSHR;

    /* Get NanoSeconds */
    time->NanoSeconds = heth->Instance->PTPTSLR;

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
      heth->Instance->PTPTSHUR = ETH_PTPTSHR_VALUE - timeoffset->Seconds + 1U;

      if (READ_BIT(heth->Instance->PTPTSCR, ETH_PTPTSCR_TSSSR) == ETH_PTPTSCR_TSSSR)
      {
        /* Set nanoSeconds update */
        heth->Instance->PTPTSLUR = ETH_PTPTSLR_VALUE - timeoffset->NanoSeconds;
      }
      else
      {
        heth->Instance->PTPTSLUR = ETH_PTPTSHR_VALUE - timeoffset->NanoSeconds + 1U;
      }
    }
    else
    {
      /* Set Seconds update */
      heth->Instance->PTPTSHUR = timeoffset->Seconds;
      /* Set nanoSeconds update */
      heth->Instance->PTPTSLUR = timeoffset->NanoSeconds;
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
    SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_TTSE);

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
  /* Packet received */
  if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMASR_RS))
  {
    if (__HAL_ETH_DMA_GET_IT_SOURCE(heth, ETH_DMAIER_RIE))
    {
      /* Clear the Eth DMA Rx IT pending bits */
      __HAL_ETH_DMA_CLEAR_IT(heth, ETH_DMASR_RS | ETH_DMASR_NIS);

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
  if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMASR_TS))
  {
    if (__HAL_ETH_DMA_GET_IT_SOURCE(heth, ETH_DMAIER_TIE))
    {
      /* Clear the Eth DMA Tx IT pending bits */
      __HAL_ETH_DMA_CLEAR_IT(heth, ETH_DMASR_TS | ETH_DMASR_NIS);

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
  if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMASR_AIS))
  {
    if (__HAL_ETH_DMA_GET_IT_SOURCE(heth, ETH_DMAIER_AISE))
    {
      heth->ErrorCode |= HAL_ETH_ERROR_DMA;

      /* if fatal bus error occurred */
      if (__HAL_ETH_DMA_GET_IT(heth, ETH_DMASR_FBES))
      {
        /* Get DMA error code  */
        heth->DMAErrorCode = READ_BIT(heth->Instance->DMASR, (ETH_DMASR_FBES | ETH_DMASR_TPS | ETH_DMASR_RPS));

        /* Disable all interrupts */
        __HAL_ETH_DMA_DISABLE_IT(heth, ETH_DMAIER_NISE | ETH_DMAIER_AISE);

        /* Set HAL state to ERROR */
        heth->gState = HAL_ETH_STATE_ERROR;
      }
      else
      {
        /* Get DMA error status  */
        heth->DMAErrorCode = READ_BIT(heth->Instance->DMASR, (ETH_DMASR_ETS | ETH_DMASR_RWTS |
                                                              ETH_DMASR_RBUS | ETH_DMASR_AIS));

        /* Clear the interrupt summary flag */
        __HAL_ETH_DMA_CLEAR_IT(heth, (ETH_DMASR_ETS | ETH_DMASR_RWTS |
                                      ETH_DMASR_RBUS | ETH_DMASR_AIS));
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


  /* ETH PMT IT */
  if (__HAL_ETH_MAC_GET_IT(heth, ETH_MAC_PMT_IT))
  {
    /* Get MAC Wake-up source and clear the status register pending bit */
    heth->MACWakeUpEvent = READ_BIT(heth->Instance->MACPMTCSR, (ETH_MACPMTCSR_WFR | ETH_MACPMTCSR_MPR));

#if (USE_HAL_ETH_REGISTER_CALLBACKS == 1)
    /* Call registered PMT callback*/
    heth->PMTCallback(heth);
#else
    /* Ethernet PMT callback */
    HAL_ETH_PMTCallback(heth);
#endif  /* USE_HAL_ETH_REGISTER_CALLBACKS */

    heth->MACWakeUpEvent = (uint32_t)(0x0U);
  }


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
  uint32_t tmpreg1;
  uint32_t tickstart;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg1 = heth->Instance->MACMIIAR;

  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg1 &= ~ETH_MACMIIAR_CR_MASK;

  /* Prepare the MII address register value */
  tmpreg1 |= ((PHYAddr << 11U) & ETH_MACMIIAR_PA);                        /* Set the PHY device address   */
  tmpreg1 |= (((uint32_t)PHYReg << 6U) & ETH_MACMIIAR_MR);                /* Set the PHY register address */
  tmpreg1 &= ~ETH_MACMIIAR_MW;                                            /* Set the read mode            */
  tmpreg1 |= ETH_MACMIIAR_MB;                                             /* Set the MII Busy bit         */

  /* Write the result value into the MII Address register */
  heth->Instance->MACMIIAR = tmpreg1;


  tickstart = HAL_GetTick();

  /* Check for the Busy flag */
  while ((tmpreg1 & ETH_MACMIIAR_MB) == ETH_MACMIIAR_MB)
  {
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > PHY_READ_TO)
    {
      return HAL_ERROR;
    }

    tmpreg1 = heth->Instance->MACMIIAR;
  }

  /* Get MACMIIDR value */
  *pRegValue = (uint16_t)(heth->Instance->MACMIIDR);

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
  uint32_t tmpreg1;
  uint32_t tickstart;

  /* Get the ETHERNET MACMIIAR value */
  tmpreg1 = heth->Instance->MACMIIAR;

  /* Keep only the CSR Clock Range CR[2:0] bits value */
  tmpreg1 &= ~ETH_MACMIIAR_CR_MASK;

  /* Prepare the MII register address value */
  tmpreg1 |= ((PHYAddr << 11U) & ETH_MACMIIAR_PA);                      /* Set the PHY device address */
  tmpreg1 |= (((uint32_t)PHYReg << 6U) & ETH_MACMIIAR_MR);              /* Set the PHY register address */
  tmpreg1 |= ETH_MACMIIAR_MW;                                           /* Set the write mode */
  tmpreg1 |= ETH_MACMIIAR_MB;                                           /* Set the MII Busy bit */

  /* Give the value to the MII data register */
  heth->Instance->MACMIIDR = (uint16_t)RegValue;

  /* Write the result value into the MII Address register */
  heth->Instance->MACMIIAR = tmpreg1;

  /* Get tick */
  tickstart = HAL_GetTick();

  /* Check for the Busy flag */
  while ((tmpreg1 & ETH_MACMIIAR_MB) == ETH_MACMIIAR_MB)
  {
    /* Check for the Timeout */
    if ((HAL_GetTick() - tickstart) > PHY_WRITE_TO)
    {
      return HAL_ERROR;
    }

    tmpreg1 = heth->Instance->MACMIIAR;
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
  macconf->DeferralCheck = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_DC) >> 4) > 0U) ? ENABLE : DISABLE;
  macconf->BackOffLimit = READ_BIT(heth->Instance->MACCR, ETH_MACCR_BL);
  macconf->RetryTransmission = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_RD) >> 9) == 0U) ? ENABLE : DISABLE;
  macconf->CarrierSenseDuringTransmit = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_CSD) >> 16) > 0U)
                                        ? ENABLE : DISABLE;
  macconf->ReceiveOwn = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_ROD) >> 13) == 0U) ? ENABLE : DISABLE;
  macconf->LoopbackMode = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_LM) >> 12) > 0U) ? ENABLE : DISABLE;
  macconf->DuplexMode = READ_BIT(heth->Instance->MACCR, ETH_MACCR_DM);
  macconf->Speed = READ_BIT(heth->Instance->MACCR, ETH_MACCR_FES);
  macconf->Jabber = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_JD) >> 22) == 0U) ? ENABLE : DISABLE;
  macconf->Watchdog = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_WD) >> 23) == 0U) ? ENABLE : DISABLE;
  macconf->AutomaticPadCRCStrip = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_APCS) >> 7) > 0U) ? ENABLE : DISABLE;
  macconf->InterPacketGapVal = READ_BIT(heth->Instance->MACCR, ETH_MACCR_IFG);
  macconf->ChecksumOffload = ((READ_BIT(heth->Instance->MACCR, ETH_MACCR_IPCO) >> 10U) > 0U) ? ENABLE : DISABLE;


  macconf->TransmitFlowControl = ((READ_BIT(heth->Instance->MACFCR, ETH_MACFCR_TFCE) >> 1) > 0U) ? ENABLE : DISABLE;
  macconf->ZeroQuantaPause = ((READ_BIT(heth->Instance->MACFCR, ETH_MACFCR_ZQPD) >> 7) == 0U) ? ENABLE : DISABLE;
  macconf->PauseLowThreshold = READ_BIT(heth->Instance->MACFCR, ETH_MACFCR_PLT);
  macconf->PauseTime = (READ_BIT(heth->Instance->MACFCR, ETH_MACFCR_PT) >> 16);
  macconf->ReceiveFlowControl = ((READ_BIT(heth->Instance->MACFCR, ETH_MACFCR_RFCE) >> 2U) > 0U) ? ENABLE : DISABLE;
  macconf->UnicastPausePacketDetect = ((READ_BIT(heth->Instance->MACFCR, ETH_MACFCR_UPFD) >> 3U) > 0U)
                                      ? ENABLE : DISABLE;

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

  dmaconf->DMAArbitration = READ_BIT(heth->Instance->DMABMR,
                                    (ETH_DMAARBITRATION_RXPRIORTX | ETH_DMAARBITRATION_ROUNDROBIN_RXTX_4_1));
  dmaconf->AddressAlignedBeats = ((READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_AAB) >> 25U) > 0U) ? ENABLE : DISABLE;
  dmaconf->BurstMode = READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_FB | ETH_DMABMR_MB);
  dmaconf->RxDMABurstLength = READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_RDP);
  dmaconf->TxDMABurstLength = READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_PBL);
  dmaconf->EnhancedDescriptorFormat = ((READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_EDE) >> 7) > 0U) ? ENABLE : DISABLE;
  dmaconf->DescriptorSkipLength = READ_BIT(heth->Instance->DMABMR, ETH_DMABMR_DSL) >> 2;

  dmaconf->DropTCPIPChecksumErrorFrame = ((READ_BIT(heth->Instance->DMAOMR,
                                                    ETH_DMAOMR_DTCEFD) >> 26) > 0U) ? DISABLE : ENABLE;
  dmaconf->ReceiveStoreForward = ((READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_RSF) >> 25) > 0U) ? ENABLE : DISABLE;
  dmaconf->FlushRxPacket = ((READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_FTF) >> 20) > 0U) ? DISABLE : ENABLE;
  dmaconf->TransmitStoreForward = ((READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_TSF) >> 21) > 0U) ? ENABLE : DISABLE;
  dmaconf->TransmitThresholdControl = READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_TTC);
  dmaconf->ForwardErrorFrames = ((READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_FEF) >> 7) > 0U) ? ENABLE : DISABLE;
  dmaconf->ForwardUndersizedGoodFrames = ((READ_BIT(heth->Instance->DMAOMR,
                                                    ETH_DMAOMR_FUGF) >> 6) > 0U) ? ENABLE : DISABLE;
  dmaconf->ReceiveThresholdControl = READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_RTC);
  dmaconf->SecondFrameOperate = ((READ_BIT(heth->Instance->DMAOMR, ETH_DMAOMR_OSF) >> 2) > 0U) ? ENABLE : DISABLE;
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

  /* Get the ETHERNET MACMIIAR value */
  tmpreg = (heth->Instance)->MACMIIAR;
  /* Clear CSR Clock Range CR[2:0] bits */
  tmpreg &= ETH_MACMIIAR_CR_MASK;

  /* Get hclk frequency value */
  hclk = HAL_RCC_GetHCLKFreq();

  /* Set CR bits depending on hclk value */
  if ((hclk >= 20000000U) && (hclk < 35000000U))
  {
    /* CSR Clock Range between 20-35 MHz */
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div16;
  }
  else if ((hclk >= 35000000U) && (hclk < 60000000U))
  {
    /* CSR Clock Range between 35-60 MHz */
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div26;
  }
  else if ((hclk >= 60000000U) && (hclk < 100000000U))
  {
    /* CSR Clock Range between 60-100 MHz */
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div42;
  }
  else if ((hclk >= 100000000U) && (hclk < 150000000U))
  {
    /* CSR Clock Range between 100-150 MHz */
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div62;
  }
  else /* ((hclk >= 150000000)&&(hclk <= 183000000))*/
  {
    /* CSR Clock Range between 150-183 MHz */
    tmpreg |= (uint32_t)ETH_MACMIIAR_CR_Div102;
  }

  /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
  (heth->Instance)->MACMIIAR = (uint32_t)tmpreg;
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
  uint32_t tmpreg1;

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

  MODIFY_REG(heth->Instance->MACFFR, ETH_MACFFR_MASK, filterconfig);

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACFFR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACFFR = tmpreg1;

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

  pFilterConfig->PromiscuousMode = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_PM)) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->HashUnicast = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_HU) >> 1) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->HashMulticast = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_HM) >> 2) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->DestAddrInverseFiltering = ((READ_BIT(heth->Instance->MACFFR,
                                                       ETH_MACFFR_DAIF) >> 3) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->PassAllMulticast = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_PAM) >> 4) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->BroadcastFilter = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_BFD) >> 5) == 0U) ? ENABLE : DISABLE;
  pFilterConfig->ControlPacketsFilter = READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_PCF);
  pFilterConfig->SrcAddrInverseFiltering = ((READ_BIT(heth->Instance->MACFFR,
                                                      ETH_MACFFR_SAIF) >> 8) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->SrcAddrFiltering = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_SAF) >> 9) > 0U) ? ENABLE : DISABLE;
  pFilterConfig->HachOrPerfectFilter = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_HPF) >> 10) > 0U)
                                       ? ENABLE : DISABLE;
  pFilterConfig->ReceiveAllMode = ((READ_BIT(heth->Instance->MACFFR, ETH_MACFFR_RA) >> 31) > 0U) ? ENABLE : DISABLE;

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
  (*(__IO uint32_t *)macaddrhr) |= (ETH_MACA1HR_AE | ETH_MACA1HR_SA);

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
  uint32_t tmpreg1;
  if (pHashTable == NULL)
  {
    return HAL_ERROR;
  }

  heth->Instance->MACHTHR = pHashTable[0];

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACHTHR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACHTHR = tmpreg1;

  heth->Instance->MACHTLR = pHashTable[1];

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACHTLR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACHTLR = tmpreg1;

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
  uint32_t tmpreg1;
  MODIFY_REG(heth->Instance->MACVLANTR, ETH_MACVLANTR_VLANTI, VLANIdentifier);
  if (ComparisonBits == ETH_VLANTAGCOMPARISON_16BIT)
  {
    CLEAR_BIT(heth->Instance->MACVLANTR, ETH_MACVLANTR_VLANTC);
  }
  else
  {
    SET_BIT(heth->Instance->MACVLANTR, ETH_MACVLANTR_VLANTC);
  }

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACVLANTR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACVLANTR = tmpreg1;
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

  powerdownconfig = (((uint32_t)pPowerDownConfig->MagicPacket << ETH_MACPMTCSR_MPE_Pos) |
                     ((uint32_t)pPowerDownConfig->WakeUpPacket << ETH_MACPMTCSR_WFE_Pos) |
                     ((uint32_t)pPowerDownConfig->GlobalUnicast << ETH_MACPMTCSR_GU_Pos) |
                     ETH_MACPMTCSR_PD);

  MODIFY_REG(heth->Instance->MACPMTCSR, ETH_MACPMTCSR_MASK, powerdownconfig);
}

/**
  * @brief  Exits from the Power down mode.
  * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None.
  */
void HAL_ETH_ExitPowerDownMode(ETH_HandleTypeDef *heth)
{
  uint32_t tmpreg1;

  /* clear wake up sources */
  CLEAR_BIT(heth->Instance->MACPMTCSR, ETH_MACPMTCSR_WFE | ETH_MACPMTCSR_MPE | ETH_MACPMTCSR_GU);

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACPMTCSR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACPMTCSR = tmpreg1;

  if (READ_BIT(heth->Instance->MACPMTCSR, ETH_MACPMTCSR_PD) != 0U)
  {
    /* Exit power down mode */
    CLEAR_BIT(heth->Instance->MACPMTCSR, ETH_MACPMTCSR_PD);

    /* Wait until the write operation will be taken into account :
    at least four TX_CLK/RX_CLK clock cycles */
    tmpreg1 = (heth->Instance)->MACPMTCSR;
    HAL_Delay(ETH_REG_WRITE_DELAY);
    (heth->Instance)->MACPMTCSR = tmpreg1;
  }

  /* Disable PMT interrupt */
  SET_BIT(heth->Instance->MACIMR, ETH_MACIMR_PMTIM);
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
  SET_BIT(heth->Instance->MACPMTCSR, ETH_MACPMTCSR_WFFRPR);

  /* Wake up packet filter config */
  for (regindex = 0; regindex < Count; regindex++)
  {
    /* Write filter regs */
    WRITE_REG(heth->Instance->MACRWUFFR, pFilter[regindex]);
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

/**
  * @brief  Clears the ETHERNET transmit FIFO.
  * @param  heth pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @retval None
  */
static void ETH_FlushTransmitFIFO(ETH_HandleTypeDef *heth)
{
  __IO uint32_t tmpreg = 0;

  /* Set the Flush Transmit FIFO bit */
  (heth->Instance)->DMAOMR |= ETH_DMAOMR_FTF;

  /* Wait until the write operation will be taken into account:
     at least four TX_CLK/RX_CLK clock cycles */
  tmpreg = (heth->Instance)->DMAOMR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->DMAOMR = tmpreg;
}

static void ETH_SetMACConfig(ETH_HandleTypeDef *heth,  ETH_MACConfigTypeDef *macconf)
{
  uint32_t tmpreg1;

  /*------------------------ ETHERNET MACCR Configuration --------------------*/
  /* Get the ETHERNET MACCR value */
  tmpreg1 = (heth->Instance)->MACCR;
  /* Clear WD, PCE, PS, TE and RE bits */
  tmpreg1 &= ETH_MACCR_CLEAR_MASK;

  tmpreg1 |= (uint32_t)(((uint32_t)((macconf->Watchdog == DISABLE) ? 1U : 0U) << 23U) |
                        ((uint32_t)((macconf->Jabber == DISABLE) ? 1U : 0U) << 22U) |
                        (uint32_t)macconf->InterPacketGapVal |
                        ((uint32_t)macconf->CarrierSenseDuringTransmit << 16U) |
                        macconf->Speed |
                        ((uint32_t)((macconf->ReceiveOwn == DISABLE) ? 1U : 0U) << 13U) |
                        ((uint32_t)macconf->LoopbackMode << 12U) |
                        macconf->DuplexMode |
                        ((uint32_t)macconf->ChecksumOffload << 10U) |
                        ((uint32_t)((macconf->RetryTransmission == DISABLE) ? 1U : 0U) << 9U) |
                        ((uint32_t)macconf->AutomaticPadCRCStrip << 7U) |
                        macconf->BackOffLimit |
                        ((uint32_t)macconf->DeferralCheck << 4U));

  /* Write to ETHERNET MACCR */
  (heth->Instance)->MACCR = (uint32_t)tmpreg1;

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACCR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACCR = tmpreg1;

  /*----------------------- ETHERNET MACFCR Configuration --------------------*/

  /* Get the ETHERNET MACFCR value */
  tmpreg1 = (heth->Instance)->MACFCR;
  /* Clear xx bits */
  tmpreg1 &= ETH_MACFCR_CLEAR_MASK;

  tmpreg1 |= (uint32_t)((macconf->PauseTime << 16U) |
                        ((uint32_t)((macconf->ZeroQuantaPause == DISABLE) ? 1U : 0U) << 7U) |
                        macconf->PauseLowThreshold |
                        ((uint32_t)((macconf->UnicastPausePacketDetect == ENABLE) ? 1U : 0U) << 3U) |
                        ((uint32_t)((macconf->ReceiveFlowControl == ENABLE) ? 1U : 0U) << 2U) |
                        ((uint32_t)((macconf->TransmitFlowControl == ENABLE) ? 1U : 0U) << 1U));

  /* Write to ETHERNET MACFCR */
  (heth->Instance)->MACFCR = (uint32_t)tmpreg1;

  /* Wait until the write operation will be taken into account :
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->MACFCR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->MACFCR = tmpreg1;
}

static void ETH_SetDMAConfig(ETH_HandleTypeDef *heth,  ETH_DMAConfigTypeDef *dmaconf)
{
  uint32_t tmpreg1;

  /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
  /* Get the ETHERNET DMAOMR value */
  tmpreg1 = (heth->Instance)->DMAOMR;
  /* Clear xx bits */
  tmpreg1 &= ETH_DMAOMR_CLEAR_MASK;

  tmpreg1 |= (uint32_t)(((uint32_t)((dmaconf->DropTCPIPChecksumErrorFrame == DISABLE) ? 1U : 0U) << 26U) |
                        ((uint32_t)dmaconf->ReceiveStoreForward << 25U) |
                        ((uint32_t)((dmaconf->FlushRxPacket == DISABLE) ? 1U : 0U) << 20U) |
                        ((uint32_t)dmaconf->TransmitStoreForward << 21U) |
                        dmaconf->TransmitThresholdControl |
                        ((uint32_t)dmaconf->ForwardErrorFrames << 7U) |
                        ((uint32_t)dmaconf->ForwardUndersizedGoodFrames << 6U) |
                        dmaconf->ReceiveThresholdControl |
                        ((uint32_t)dmaconf->SecondFrameOperate << 2U));

  /* Write to ETHERNET DMAOMR */
  (heth->Instance)->DMAOMR = (uint32_t)tmpreg1;

  /* Wait until the write operation will be taken into account:
  at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->DMAOMR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->DMAOMR = tmpreg1;

  /*----------------------- ETHERNET DMABMR Configuration --------------------*/
  (heth->Instance)->DMABMR = (uint32_t)(((uint32_t)dmaconf->AddressAlignedBeats << 25U) |
                                        dmaconf->BurstMode |
                                        dmaconf->RxDMABurstLength | /* !! if 4xPBL is selected for Tx or
                                                                       Rx it is applied for the other */
                                        dmaconf->TxDMABurstLength |
                                        ((uint32_t)dmaconf->EnhancedDescriptorFormat << 7U) |
                                        (dmaconf->DescriptorSkipLength << 2U) |
                                        dmaconf->DMAArbitration |
                                        ETH_DMABMR_USP); /* Enable use of separate PBL for Rx and Tx */

  /* Wait until the write operation will be taken into account:
     at least four TX_CLK/RX_CLK clock cycles */
  tmpreg1 = (heth->Instance)->DMABMR;
  HAL_Delay(ETH_REG_WRITE_DELAY);
  (heth->Instance)->DMABMR = tmpreg1;
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
  macDefaultConf.Watchdog = ENABLE;
  macDefaultConf.Jabber = ENABLE;
  macDefaultConf.InterPacketGapVal = ETH_INTERFRAMEGAP_96BIT;
  macDefaultConf.CarrierSenseDuringTransmit = DISABLE;
  macDefaultConf.ReceiveOwn = ENABLE;
  macDefaultConf.LoopbackMode = DISABLE;
  macDefaultConf.ChecksumOffload = ENABLE;
  macDefaultConf.RetryTransmission = DISABLE;
  macDefaultConf.AutomaticPadCRCStrip = DISABLE;
  macDefaultConf.BackOffLimit = ETH_BACKOFFLIMIT_10;
  macDefaultConf.DeferralCheck = DISABLE;
  macDefaultConf.PauseTime = 0x0U;
  macDefaultConf.ZeroQuantaPause = DISABLE;
  macDefaultConf.PauseLowThreshold = ETH_PAUSELOWTHRESHOLD_MINUS4;
  macDefaultConf.ReceiveFlowControl = DISABLE;
  macDefaultConf.TransmitFlowControl = DISABLE;
  macDefaultConf.Speed = ETH_SPEED_100M;
  macDefaultConf.DuplexMode = ETH_FULLDUPLEX_MODE;
  macDefaultConf.UnicastPausePacketDetect = DISABLE;

  /* MAC default configuration */
  ETH_SetMACConfig(heth, &macDefaultConf);

  /*--------------- ETHERNET DMA registers default Configuration --------------*/
  dmaDefaultConf.DropTCPIPChecksumErrorFrame = ENABLE;
  dmaDefaultConf.ReceiveStoreForward = ENABLE;
  dmaDefaultConf.FlushRxPacket = ENABLE;
  dmaDefaultConf.TransmitStoreForward = ENABLE;
  dmaDefaultConf.TransmitThresholdControl = ETH_TRANSMITTHRESHOLDCONTROL_64BYTES;
  dmaDefaultConf.ForwardErrorFrames = DISABLE;
  dmaDefaultConf.ForwardUndersizedGoodFrames = DISABLE;
  dmaDefaultConf.ReceiveThresholdControl = ETH_RECEIVEDTHRESHOLDCONTROL_64BYTES;
  dmaDefaultConf.SecondFrameOperate = ENABLE;
  dmaDefaultConf.AddressAlignedBeats = ENABLE;
  dmaDefaultConf.BurstMode = ETH_BURSTLENGTH_FIXED;
  dmaDefaultConf.RxDMABurstLength = ETH_RXDMABURSTLENGTH_32BEAT;
  dmaDefaultConf.TxDMABurstLength = ETH_TXDMABURSTLENGTH_32BEAT;
  dmaDefaultConf.EnhancedDescriptorFormat = ENABLE;
  dmaDefaultConf.DescriptorSkipLength = 0x0U;
  dmaDefaultConf.DMAArbitration = ETH_DMAARBITRATION_ROUNDROBIN_RXTX_1_1;

  /* DMA default configuration */
  ETH_SetDMAConfig(heth, &dmaDefaultConf);
}

/**
  * @brief  Configures the selected MAC address.
  * @param  heth pointer to a ETH_HandleTypeDef structure that contains
  *         the configuration information for ETHERNET module
  * @param  MacAddr The MAC address to configure
  *          This parameter can be one of the following values:
  *             @arg ETH_MAC_Address0: MAC Address0
  *             @arg ETH_MAC_Address1: MAC Address1
  *             @arg ETH_MAC_Address2: MAC Address2
  *             @arg ETH_MAC_Address3: MAC Address3
  * @param  Addr Pointer to MAC address buffer data (6 bytes)
  * @retval HAL status
  */
static void ETH_MACAddressConfig(ETH_HandleTypeDef *heth, uint32_t MacAddr, uint8_t *Addr)
{
  uint32_t tmpreg1;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(heth);

  /* Calculate the selected MAC address high register */
  tmpreg1 = ((uint32_t)Addr[5U] << 8U) | (uint32_t)Addr[4U];
  /* Load the selected MAC address high register */
  (*(__IO uint32_t *)((uint32_t)(ETH_MAC_ADDR_HBASE + MacAddr))) = tmpreg1;
  /* Calculate the selected MAC address low register */
  tmpreg1 = ((uint32_t)Addr[3U] << 24U) | ((uint32_t)Addr[2U] << 16U) | ((uint32_t)Addr[1U] << 8U) | Addr[0U];

  /* Load the selected MAC address low register */
  (*(__IO uint32_t *)((uint32_t)(ETH_MAC_ADDR_LBASE + MacAddr))) = tmpreg1;
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

    /* Set Second Address Chained bit */
    SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_TCH);

    if (i < ((uint32_t)ETH_TX_DESC_CNT - 1U))
    {
      WRITE_REG(dmatxdesc->DESC3, (uint32_t)(heth->Init.TxDesc + i + 1U));
    }
    else
    {
      WRITE_REG(dmatxdesc->DESC3, (uint32_t)(heth->Init.TxDesc));
    }

    /* Set the DMA Tx descriptors checksum insertion */
    SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_CHECKSUMTCPUDPICMPFULL);
  }

  heth->TxDescList.CurTxDesc = 0;

  /* Set Transmit Descriptor List Address */
  WRITE_REG(heth->Instance->DMATDLAR, (uint32_t) heth->Init.TxDesc);
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

    /* Set Own bit of the Rx descriptor Status */
    dmarxdesc->DESC0 = ETH_DMARXDESC_OWN;

    /* Set Buffer1 size and Second Address Chained bit */
    dmarxdesc->DESC1 = ETH_DMARXDESC_RCH | ETH_RX_BUF_SIZE;

    /* Enable Ethernet DMA Rx Descriptor interrupt */
    dmarxdesc->DESC1 &= ~ETH_DMARXDESC_DIC;

    /* Set Rx descritors addresses */
    WRITE_REG(heth->RxDescList.RxDesc[i], (uint32_t)dmarxdesc);

    if (i < ((uint32_t)ETH_RX_DESC_CNT - 1U))
    {
      WRITE_REG(dmarxdesc->DESC3, (uint32_t)(heth->Init.RxDesc + i + 1U));
    }
    else
    {
      WRITE_REG(dmarxdesc->DESC3, (uint32_t)(heth->Init.RxDesc));
    }
  }

  WRITE_REG(heth->RxDescList.RxDescIdx, 0);
  WRITE_REG(heth->RxDescList.RxDescCnt, 0);
  WRITE_REG(heth->RxDescList.RxBuildDescIdx, 0);
  WRITE_REG(heth->RxDescList.RxBuildDescCnt, 0);
  WRITE_REG(heth->RxDescList.ItMode, 0);

  /* Set Receive Descriptor List Address */
  WRITE_REG(heth->Instance->DMARDLAR, (uint32_t) heth->Init.RxDesc);
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
  if ((READ_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_OWN) == ETH_DMATXDESC_OWN)
      || (dmatxdesclist->PacketAddress[descidx] != NULL))
  {
    return HAL_ETH_ERROR_BUSY;
  }


  descnbr += 1U;

  /* Set header or buffer 1 address */
  WRITE_REG(dmatxdesc->DESC2, (uint32_t)txbuffer->buffer);

  /* Set header or buffer 1 Length */
  MODIFY_REG(dmatxdesc->DESC1, ETH_DMATXDESC_TBS1, txbuffer->len);

  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_CSUM) != 0U)
  {
    MODIFY_REG(dmatxdesc->DESC0, ETH_DMATXDESC_CIC, pTxConfig->ChecksumCtrl);
  }

  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_CRCPAD) != 0U)
  {
    MODIFY_REG(dmatxdesc->DESC0, ETH_CRC_PAD_DISABLE, pTxConfig->CRCPadCtrl);
  }


  if (READ_BIT(pTxConfig->Attributes, ETH_TX_PACKETS_FEATURES_VLANTAG) != 0U)
  {
    /* Set Vlan Type */
    SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_VF);
  }

  /* Mark it as First Descriptor */
  SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_FS);

  /* Ensure rest of descriptor is written to RAM before the OWN bit */
  __DMB();
  /* set OWN bit of FIRST descriptor */
  SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_OWN);

  /* only if the packet is split into more than one descriptors > 1 */
  while (txbuffer->next != NULL)
  {
    /* Clear the LD bit of previous descriptor */
    CLEAR_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_LS);
    if (ItMode != ((uint32_t)RESET))
    {
      /* Set Interrupt on completion bit */
      SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_IC);
    }
    else
    {
      /* Clear Interrupt on completion bit */
      CLEAR_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_IC);
    }
    /* Increment current tx descriptor index */
    INCR_TX_DESC_INDEX(descidx, 1U);
    /* Get current descriptor address */
    dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

    /* Clear the FD bit of new Descriptor */
    CLEAR_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_FS);

    /* Current Tx Descriptor Owned by DMA: cannot be used by the application  */
    if ((READ_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_OWN) == ETH_DMATXDESC_OWN)
        || (dmatxdesclist->PacketAddress[descidx] != NULL))
    {
      descidx = firstdescidx;
      dmatxdesc = (ETH_DMADescTypeDef *)dmatxdesclist->TxDesc[descidx];

      /* clear previous desc own bit */
      for (idx = 0; idx < descnbr; idx ++)
      {
        /* Ensure rest of descriptor is written to RAM before the OWN bit */
        __DMB();

        CLEAR_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_OWN);

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
    WRITE_REG(dmatxdesc->DESC2, (uint32_t)txbuffer->buffer);

    /* Set header or buffer 1 Length */
    MODIFY_REG(dmatxdesc->DESC1, ETH_DMATXDESC_TBS1, txbuffer->len);

    bd_count += 1U;

    /* Ensure rest of descriptor is written to RAM before the OWN bit */
    __DMB();
    /* Set Own bit */
    SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_OWN);
  }

  if (ItMode != ((uint32_t)RESET))
  {
    /* Set Interrupt on completion bit */
    SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_IC);
  }
  else
  {
    /* Clear Interrupt on completion bit */
    CLEAR_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_IC);
  }

  /* Mark it as LAST descriptor */
  SET_BIT(dmatxdesc->DESC0, ETH_DMATXDESC_LS);
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

