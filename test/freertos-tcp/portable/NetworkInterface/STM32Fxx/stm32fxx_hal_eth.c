/**
 ******************************************************************************
 * @file    stm32fxx_hal_eth.c
 * @author  MCD Application Team
 * @version V1.3.2
 * @date    26-June-2015
 * @brief   ETH HAL module driver.
 *          This file provides firmware functions to manage the following
 *          functionalities of the Ethernet (ETH) peripheral:
 *           + Initialization and de-initialization functions
 *           + IO operation functions
 *           + Peripheral Control functions
 *           + Peripheral State and Errors functions
 *
 * @verbatim
 * ==============================================================================
 ##### How to use this driver #####
 #####==============================================================================
 #####[..]
 #####(#)Declare a ETH_HandleTypeDef handle structure, for example:
 #####   ETH_HandleTypeDef  heth;
 #####
 #####(#)Fill parameters of Init structure in heth handle
 #####
 #####(#)Call HAL_ETH_Init() API to initialize the Ethernet peripheral (MAC, DMA, ...)
 #####
 #####(#)Initialize the ETH low level resources through the HAL_ETH_MspInit() API:
 #####    (##) Enable the Ethernet interface clock using
 #####         (+++) __HAL_RCC_ETHMAC_CLK_ENABLE();
 #####         (+++) __HAL_RCC_ETHMACTX_CLK_ENABLE();
 #####         (+++) __HAL_RCC_ETHMACRX_CLK_ENABLE();
 #####
 #####    (##) Initialize the related GPIO clocks
 #####    (##) Configure Ethernet pin-out
 #####    (##) Configure Ethernet NVIC interrupt (IT mode)
 #####
 #####(#)Initialize Ethernet DMA Descriptors in chain mode and point to allocated buffers:
 #####    (##) HAL_ETH_DMATxDescListInit(); for Transmission process
 #####    (##) HAL_ETH_DMARxDescListInit(); for Reception process
 #####
 #####(#)Enable MAC and DMA transmission and reception:
 #####    (##) HAL_ETH_Start();
 #####
 #####(#)Prepare ETH DMA TX Descriptors and give the hand to ETH DMA to transfer
 #####   the frame to MAC TX FIFO:
 #####   (##) HAL_ETH_TransmitFrame();
 #####
 #####(#)Poll for a received frame in ETH RX DMA Descriptors and get received
 #####   frame parameters
 #####   (##) HAL_ETH_GetReceivedFrame(); (should be called into an infinite loop)
 #####
 #####(#) Get a received frame when an ETH RX interrupt occurs:
 #####   (##) HAL_ETH_GetReceivedFrame_IT(); (called in IT mode only)
 #####
 #####(#) Communicate with external PHY device:
 #####   (##) Read a specific register from the PHY
 #####        HAL_ETH_ReadPHYRegister();
 #####   (##) Write data to a specific RHY register:
 #####        HAL_ETH_WritePHYRegister();
 #####
 #####(#) Configure the Ethernet MAC after ETH peripheral initialization
 #####    HAL_ETH_ConfigMAC(); all MAC parameters should be filled.
 #####
 #####(#) Configure the Ethernet DMA after ETH peripheral initialization
 #####    HAL_ETH_ConfigDMA(); all DMA parameters should be filled.
 #####
 #####-@- The PTP protocol and the DMA descriptors ring mode are not supported
 #####    in this driver
 #####
 #####@endverbatim
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#if defined( STM32F7xx )
    #include "stm32f7xx_hal.h"
    #define stm_is_F7    1
#elif defined( STM32F407xx ) || defined( STM32F417xx ) || defined( STM32F427xx ) || defined( STM32F437xx ) || defined( STM32F429xx ) || defined( STM32F439xx )
    #include "stm32f4xx_hal.h"
    #define stm_is_F4    1
#elif defined( STM32F2xx )
    #include "stm32f2xx_hal.h"
    #define stm_is_F2    1
#else
    #error For what part should this be compiled?
#endif

#include "stm32fxx_hal_eth.h"

/** @addtogroup STM32F4xx_HAL_Driver
 * @{
 */

/** @defgroup ETH ETH
 * @brief ETH HAL module driver
 * @{
 */

#if !defined( ARRAY_SIZE )
    #define ARRAY_SIZE( x )    ( sizeof( x ) / sizeof( x )[ 0 ] )
#endif

#ifdef HAL_ETH_MODULE_ENABLED

    #if ( stm_is_F2 != 0 || stm_is_F4 != 0 || stm_is_F7 )

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/** @defgroup ETH_Private_Constants ETH Private Constants
 * @{
 */

/**
 * @}
 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/** @defgroup ETH_Private_Functions ETH Private Functions
 * @{
 */
        static void ETH_MACDMAConfig( ETH_HandleTypeDef * heth,
                                      uint32_t err );
        static void ETH_MACAddressConfig( ETH_HandleTypeDef * heth,
                                          uint32_t MacAddr,
                                          uint8_t * Addr );
        static void ETH_MACReceptionEnable( ETH_HandleTypeDef * heth );
        static void ETH_MACReceptionDisable( ETH_HandleTypeDef * heth );
        static void ETH_MACTransmissionEnable( ETH_HandleTypeDef * heth );
        static void ETH_MACTransmissionDisable( ETH_HandleTypeDef * heth );
        static void ETH_DMATransmissionEnable( ETH_HandleTypeDef * heth );
        static void ETH_DMATransmissionDisable( ETH_HandleTypeDef * heth );
        static void ETH_DMAReceptionEnable( ETH_HandleTypeDef * heth );
        static void ETH_DMAReceptionDisable( ETH_HandleTypeDef * heth );
        static void ETH_FlushTransmitFIFO( ETH_HandleTypeDef * heth );

/**
 * @}
 */
/* Private functions ---------------------------------------------------------*/

/** @defgroup ETH_Exported_Functions ETH Exported Functions
 * @{
 */

/** @defgroup ETH_Exported_Functions_Group1 Initialization and de-initialization functions
 *  @brief   Initialization and Configuration functions
 *
 * @verbatim
 * ===============================================================================
 ##### Initialization and de-initialization functions #####
 #####===============================================================================
 #####[..]  This section provides functions allowing to:
 #####(+) Initialize and configure the Ethernet peripheral
 #####(+) De-initialize the Ethernet peripheral
 #####
 #####@endverbatim
 * @{
 */
        extern void vMACBProbePhy( void );

/**
 * @brief  Initializes the Ethernet MAC and DMA according to default
 *         parameters.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_Init( ETH_HandleTypeDef * heth )
        {
            uint32_t tmpreg = 0uL;
            uint32_t hclk = 60000000uL;
            uint32_t err = ETH_SUCCESS;

            /* Check the ETH peripheral state */
            if( heth == NULL )
            {
                return HAL_ERROR;
            }

            /* Check parameters */
            assert_param( IS_ETH_AUTONEGOTIATION( heth->Init.AutoNegotiation ) );
            assert_param( IS_ETH_RX_MODE( heth->Init.RxMode ) );
            assert_param( IS_ETH_CHECKSUM_MODE( heth->Init.ChecksumMode ) );
            assert_param( IS_ETH_MEDIA_INTERFACE( heth->Init.MediaInterface ) );

            if( heth->State == HAL_ETH_STATE_RESET )
            {
                /* Init the low level hardware : GPIO, CLOCK, NVIC. */
                HAL_ETH_MspInit( heth );
            }

            /* Enable SYSCFG Clock */
            __HAL_RCC_SYSCFG_CLK_ENABLE();

            /* Select MII or RMII Mode*/
            SYSCFG->PMC &= ~( SYSCFG_PMC_MII_RMII_SEL );
            SYSCFG->PMC |= ( uint32_t ) heth->Init.MediaInterface;

            /* Ethernet Software reset */
            /* Set the SWR bit: resets all MAC subsystem internal registers and logic */
            /* After reset all the registers holds their respective reset values */
            /* Also enable EDFE: Enhanced descriptor format enable. */
            heth->Instance->DMABMR |= ETH_DMABMR_SR | ETH_DMABMR_EDE;

            /* Wait for software reset */
            while( ( heth->Instance->DMABMR & ETH_DMABMR_SR ) != ( uint32_t ) RESET )
            {
                /* If your program hangs here, please check the value of 'ipconfigUSE_RMII'. */
            }

            /*-------------------------------- MAC Initialization ----------------------*/
            /* Get the ETHERNET MACMIIAR value */
            tmpreg = heth->Instance->MACMIIAR;
            /* Clear CSR Clock Range CR[2:0] bits */
            tmpreg &= ETH_MACMIIAR_CR_MASK;

            /* Get hclk frequency value (e.g. 168,000,000) */
            hclk = HAL_RCC_GetHCLKFreq();

            /* Set CR bits depending on hclk value */
            if( ( hclk >= 20000000uL ) && ( hclk < 35000000uL ) )
            {
                /* CSR Clock Range between 20-35 MHz */
                tmpreg |= ( uint32_t ) ETH_MACMIIAR_CR_Div16;
            }
            else if( ( hclk >= 35000000uL ) && ( hclk < 60000000uL ) )
            {
                /* CSR Clock Range between 35-60 MHz */
                tmpreg |= ( uint32_t ) ETH_MACMIIAR_CR_Div26;
            }
            else if( ( hclk >= 60000000uL ) && ( hclk < 100000000uL ) )
            {
                /* CSR Clock Range between 60-100 MHz */
                tmpreg |= ( uint32_t ) ETH_MACMIIAR_CR_Div42;
            }
            else if( ( hclk >= 100000000uL ) && ( hclk < 150000000uL ) )
            {
                /* CSR Clock Range between 100-150 MHz */
                tmpreg |= ( uint32_t ) ETH_MACMIIAR_CR_Div62;
            }
            else /* ( ( hclk >= 150000000uL ) && ( hclk <= 183000000uL ) ) */
            {
                /* CSR Clock Range between 150-183 MHz */
                tmpreg |= ( uint32_t ) ETH_MACMIIAR_CR_Div102;
            }

            /* Write to ETHERNET MAC MIIAR: Configure the ETHERNET CSR Clock Range */
            heth->Instance->MACMIIAR = ( uint32_t ) tmpreg;

            /* Initialise the MACB and set all PHY properties */
            vMACBProbePhy();

            /* Config MAC and DMA */
            ETH_MACDMAConfig( heth, err );

            /* Set ETH HAL State to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Return function status */
            return HAL_OK;
        }

/**
 * @brief  De-Initializes the ETH peripheral.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_DeInit( ETH_HandleTypeDef * heth )
        {
            /* Set the ETH peripheral state to BUSY */
            heth->State = HAL_ETH_STATE_BUSY;

            /* De-Init the low level hardware : GPIO, CLOCK, NVIC. */
            HAL_ETH_MspDeInit( heth );

            /* Set ETH HAL state to Disabled */
            heth->State = HAL_ETH_STATE_RESET;

            /* Release Lock */
            __HAL_UNLOCK( heth );

            /* Return function status */
            return HAL_OK;
        }

/**
 * @brief  Initializes the ETH MSP.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        __weak void HAL_ETH_MspInit( ETH_HandleTypeDef * heth )
        {
            /* NOTE : This function Should not be modified, when the callback is needed,
             * the HAL_ETH_MspInit could be implemented in the user file
             */
            ( void ) heth;
        }

/**
 * @brief  DeInitializes ETH MSP.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        __weak void HAL_ETH_MspDeInit( ETH_HandleTypeDef * heth )
        {
            /* NOTE : This function Should not be modified, when the callback is needed,
             * the HAL_ETH_MspDeInit could be implemented in the user file
             */
            ( void ) heth;
        }

/**
 * @}
 */

/** @defgroup ETH_Exported_Functions_Group2 IO operation functions
 *  @brief   Data transfers functions
 *
 * @verbatim
 * ==============================================================================
 ##### IO operation functions #####
 #####==============================================================================
 #####[..]  This section provides functions allowing to:
 #####  (+) Transmit a frame
 #####      HAL_ETH_TransmitFrame();
 #####  (+) Receive a frame
 #####      HAL_ETH_GetReceivedFrame();
 #####      HAL_ETH_GetReceivedFrame_IT();
 #####  (+) Read from an External PHY register
 #####      HAL_ETH_ReadPHYRegister();
 #####  (+) Write to an External PHY register
 #####      HAL_ETH_WritePHYRegister();
 #####
 #####@endverbatim
 #####
 * @{
 */

        #define ETH_DMA_ALL_INTS                                                                           \
    ( ETH_DMA_IT_TST | ETH_DMA_IT_PMT | ETH_DMA_IT_MMC | ETH_DMA_IT_NIS | ETH_DMA_IT_AIS | ETH_DMA_IT_ER | \
      ETH_DMA_IT_FBE | ETH_DMA_IT_ET | ETH_DMA_IT_RWT | ETH_DMA_IT_RPS | ETH_DMA_IT_RBU | ETH_DMA_IT_R |   \
      ETH_DMA_IT_TU | ETH_DMA_IT_RO | ETH_DMA_IT_TJT | ETH_DMA_IT_TPS | ETH_DMA_IT_T )

/*#define ETH_DMA_ALL_INTS		ETH_DMA_IT_RBU | ETH_DMA_FLAG_T | ETH_DMA_FLAG_AIS */

        #define INT_MASK    ( ( uint32_t ) ~( ETH_DMA_IT_TBU ) )
        void HAL_ETH_IRQHandler( ETH_HandleTypeDef * heth )
        {
            uint32_t dmasr;

            dmasr = heth->Instance->DMASR & ETH_DMA_ALL_INTS;
            heth->Instance->DMASR = dmasr;

            /* Frame received */
            if( ( dmasr & ( ETH_DMA_FLAG_R | ETH_DMA_IT_RBU ) ) != 0 )
            {
                /* Receive complete callback */
                HAL_ETH_RxCpltCallback( heth );
            }

            /* Frame transmitted */
            if( ( dmasr & ( ETH_DMA_FLAG_T ) ) != 0 )
            {
                /* Transfer complete callback */
                HAL_ETH_TxCpltCallback( heth );
            }

            /* ETH DMA Error */
            if( ( dmasr & ( ETH_DMA_FLAG_AIS ) ) != 0 )
            {
                /* Ethernet Error callback */
                HAL_ETH_ErrorCallback( heth );
            }
        }

/**
 * @brief  Tx Transfer completed callbacks.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        __weak void HAL_ETH_TxCpltCallback( ETH_HandleTypeDef * heth )
        {
            /* NOTE : This function Should not be modified, when the callback is needed,
             * the HAL_ETH_TxCpltCallback could be implemented in the user file
             */
            ( void ) heth;
        }

/**
 * @brief  Rx Transfer completed callbacks.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        __weak void HAL_ETH_RxCpltCallback( ETH_HandleTypeDef * heth )
        {
            /* NOTE : This function Should not be modified, when the callback is needed,
             * the HAL_ETH_TxCpltCallback could be implemented in the user file
             */
            ( void ) heth;
        }

/**
 * @brief  Ethernet transfer error callbacks
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        __weak void HAL_ETH_ErrorCallback( ETH_HandleTypeDef * heth )
        {
            /* NOTE : This function Should not be modified, when the callback is needed,
             * the HAL_ETH_TxCpltCallback could be implemented in the user file
             */
            ( void ) heth;
        }

/**
 * @brief  Reads a PHY register
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @param PHYReg: PHY register address, is the index of one of the 32 PHY register.
 *                This parameter can be one of the following values:
 *                   PHY_BCR: Transceiver Basic Control Register,
 *                   PHY_BSR: Transceiver Basic Status Register.
 *                   More PHY register could be read depending on the used PHY
 * @param RegValue: PHY register value
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_ReadPHYRegister( ETH_HandleTypeDef * heth,
                                                   uint16_t PHYReg,
                                                   uint32_t * RegValue )
        {
            uint32_t tmpreg = 0uL;
            uint32_t tickstart = 0uL;
            HAL_StatusTypeDef xResult;

            /* Check parameters */
            assert_param( IS_ETH_PHY_ADDRESS( heth->Init.PhyAddress ) );

            /* Check the ETH peripheral state */
            if( heth->State == HAL_ETH_STATE_BUSY_RD )
            {
                xResult = HAL_BUSY;
            }
            else
            {
                __HAL_LOCK( heth );

                /* Set ETH HAL State to BUSY_RD */
                heth->State = HAL_ETH_STATE_BUSY_RD;

                /* Get the ETHERNET MACMIIAR value */
                tmpreg = heth->Instance->MACMIIAR;

                /* Keep only the CSR Clock Range CR[2:0] bits value */
                tmpreg &= ~ETH_MACMIIAR_CR_MASK;

                /* Prepare the MII address register value */
                tmpreg |= ( ( ( uint32_t ) heth->Init.PhyAddress << 11 ) & ETH_MACMIIAR_PA ); /* Set the PHY device address   */
                tmpreg |= ( ( ( uint32_t ) PHYReg << 6 ) & ETH_MACMIIAR_MR );                 /* Set the PHY register address */
                tmpreg &= ~ETH_MACMIIAR_MW;                                                   /* Set the read mode            */
                tmpreg |= ETH_MACMIIAR_MB;                                                    /* Set the MII Busy bit         */

                /* Write the result value into the MII Address register */
                heth->Instance->MACMIIAR = tmpreg;

                /* Get tick */
                tickstart = HAL_GetTick();

                /* Check for the Busy flag */
                while( 1 )
                {
                    tmpreg = heth->Instance->MACMIIAR;

                    if( ( tmpreg & ETH_MACMIIAR_MB ) == 0uL )
                    {
                        /* Get MACMIIDR value */
                        *RegValue = ( uint32_t ) heth->Instance->MACMIIDR;
                        xResult = HAL_OK;
                        break;
                    }

                    /* Check for the Timeout */
                    if( ( HAL_GetTick() - tickstart ) > PHY_READ_TO )
                    {
                        xResult = HAL_TIMEOUT;
                        break;
                    }
                }

                /* Set ETH HAL State to READY */
                heth->State = HAL_ETH_STATE_READY;

                /* Process Unlocked */
                __HAL_UNLOCK( heth );
            }

            /* Return function status */
            return xResult;
        }

/**
 * @brief  Writes to a PHY register.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @param  PHYReg: PHY register address, is the index of one of the 32 PHY register.
 *          This parameter can be one of the following values:
 *             PHY_BCR: Transceiver Control Register.
 *             More PHY register could be written depending on the used PHY
 * @param  RegValue: the value to write
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_WritePHYRegister( ETH_HandleTypeDef * heth,
                                                    uint16_t PHYReg,
                                                    uint32_t RegValue )
        {
            uint32_t tmpreg = 0;
            uint32_t tickstart = 0;
            HAL_StatusTypeDef xResult;

            /* Check parameters */
            assert_param( IS_ETH_PHY_ADDRESS( heth->Init.PhyAddress ) );

            /* Check the ETH peripheral state */
            if( heth->State == HAL_ETH_STATE_BUSY_WR )
            {
                xResult = HAL_BUSY;
            }
            else
            {
                __HAL_LOCK( heth );

                /* Set ETH HAL State to BUSY_WR */
                heth->State = HAL_ETH_STATE_BUSY_WR;

                /* Get the ETHERNET MACMIIAR value */
                tmpreg = heth->Instance->MACMIIAR;

                /* Keep only the CSR Clock Range CR[2:0] bits value */
                tmpreg &= ~ETH_MACMIIAR_CR_MASK;

                /* Prepare the MII register address value */
                tmpreg |= ( ( ( uint32_t ) heth->Init.PhyAddress << 11 ) & ETH_MACMIIAR_PA ); /* Set the PHY device address */
                tmpreg |= ( ( ( uint32_t ) PHYReg << 6 ) & ETH_MACMIIAR_MR );                 /* Set the PHY register address */
                tmpreg |= ETH_MACMIIAR_MW;                                                    /* Set the write mode */
                tmpreg |= ETH_MACMIIAR_MB;                                                    /* Set the MII Busy bit */

                /* Give the value to the MII data register */
                heth->Instance->MACMIIDR = ( uint16_t ) RegValue;

                /* Write the result value into the MII Address register */
                heth->Instance->MACMIIAR = tmpreg;

                /* Get tick */
                tickstart = HAL_GetTick();

                /* Check for the Busy flag */
                while( 1 )
                {
                    tmpreg = heth->Instance->MACMIIAR;

                    if( ( tmpreg & ETH_MACMIIAR_MB ) == 0ul )
                    {
                        xResult = HAL_OK;
                        break;
                    }

                    /* Check for the Timeout */
                    if( ( HAL_GetTick() - tickstart ) > PHY_WRITE_TO )
                    {
                        xResult = HAL_TIMEOUT;
                        break;
                    }
                }

                /* Set ETH HAL State to READY */
                heth->State = HAL_ETH_STATE_READY;
                /* Process Unlocked */
                __HAL_UNLOCK( heth );
            }

            /* Return function status */
            return xResult;
        }

/**
 * @}
 */

/** @defgroup ETH_Exported_Functions_Group3 Peripheral Control functions
 *  @brief    Peripheral Control functions
 *
 * @verbatim
 * ===============================================================================
 ##### Peripheral Control functions #####
 #####===============================================================================
 #####[..]  This section provides functions allowing to:
 #####(+) Enable MAC and DMA transmission and reception.
 #####    HAL_ETH_Start();
 #####(+) Disable MAC and DMA transmission and reception.
 #####    HAL_ETH_Stop();
 #####(+) Set the MAC configuration in runtime mode
 #####    HAL_ETH_ConfigMAC();
 #####(+) Set the DMA configuration in runtime mode
 #####    HAL_ETH_ConfigDMA();
 #####
 #####@endverbatim
 * @{
 */

/**
 * @brief  Enables Ethernet MAC and DMA reception/transmission
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_Start( ETH_HandleTypeDef * heth )
        {
            /* Process Locked */
            __HAL_LOCK( heth );

            /* Set the ETH peripheral state to BUSY */
            heth->State = HAL_ETH_STATE_BUSY;

            /* Enable transmit state machine of the MAC for transmission on the MII */
            ETH_MACTransmissionEnable( heth );

            /* Enable receive state machine of the MAC for reception from the MII */
            ETH_MACReceptionEnable( heth );

            /* Flush Transmit FIFO */
            ETH_FlushTransmitFIFO( heth );

            /* Start DMA transmission */
            ETH_DMATransmissionEnable( heth );

            /* Start DMA reception */
            ETH_DMAReceptionEnable( heth );

            /* Set the ETH state to READY*/
            heth->State = HAL_ETH_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK( heth );

            /* Return function status */
            return HAL_OK;
        }

/**
 * @brief  Stop Ethernet MAC and DMA reception/transmission
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_Stop( ETH_HandleTypeDef * heth )
        {
            /* Process Locked */
            __HAL_LOCK( heth );

            /* Set the ETH peripheral state to BUSY */
            heth->State = HAL_ETH_STATE_BUSY;

            /* Stop DMA transmission */
            ETH_DMATransmissionDisable( heth );

            /* Stop DMA reception */
            ETH_DMAReceptionDisable( heth );

            /* Disable receive state machine of the MAC for reception from the MII */
            ETH_MACReceptionDisable( heth );

            /* Flush Transmit FIFO */
            ETH_FlushTransmitFIFO( heth );

            /* Disable transmit state machine of the MAC for transmission on the MII */
            ETH_MACTransmissionDisable( heth );

            /* Set the ETH state*/
            heth->State = HAL_ETH_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK( heth );

            /* Return function status */
            return HAL_OK;
        }

        static void vRegisterDelay()
        {
            uint32_t uxCount;

            /*
             * Regarding the HAL delay functions, I noticed that HAL delay is being used to workaround the
             * "Successive write operations to the same register might not be fully taken into account" errata.
             * The workaround requires a delay of four TX_CLK/RX_CLK clock cycles. For a 10 Mbit connection,
             * these clocks are running at 2.5 MHz, so this delay would be at most 1.6 microseconds.
             * 180 Mhz = 288 loops
             * 168 Mhz = 269 loops
             * 100 Mhz = 160 loops
             *  84 Mhz = 134 loops
             */
        #define WAIT_TIME_NS    1600uL          /* 1.6 microseconds */
        #define CPU_MAX_FREQ    SystemCoreClock /* 84, 100, 168 or 180 MHz */
            uint32_t NOP_COUNT = ( WAIT_TIME_NS * ( CPU_MAX_FREQ / 1000uL ) ) / 1000000uL;

            for( uxCount = NOP_COUNT; uxCount > 0uL; uxCount-- )
            {
                __NOP();
            }
        }

        static void prvWriteMACFCR( ETH_HandleTypeDef * heth,
                                    uint32_t ulValue )
        {
            /* Enable the MAC transmission */
            heth->Instance->MACFCR = ulValue;

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles.
             * Read it back, wait a ms and */
            ( void ) heth->Instance->MACFCR;

            vRegisterDelay();

            heth->Instance->MACFCR = ulValue;
        }

        static void prvWriteDMAOMR( ETH_HandleTypeDef * heth,
                                    uint32_t ulValue )
        {
            /* Enable the MAC transmission */
            heth->Instance->DMAOMR = ulValue;

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles.
             * Read it back, wait a ms and */
            ( void ) heth->Instance->DMAOMR;

            vRegisterDelay();

            heth->Instance->DMAOMR = ulValue;
        }

        static void prvWriteMACCR( ETH_HandleTypeDef * heth,
                                   uint32_t ulValue )
        {
            /* Enable the MAC transmission */
            heth->Instance->MACCR = ulValue;

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles.
             * Read it back, wait a ms and */
            ( void ) heth->Instance->MACCR;

            vRegisterDelay();

            heth->Instance->MACCR = ulValue;
        }

/**
 * @brief  Set ETH MAC Configuration.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @param  macconf: MAC Configuration structure
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_ConfigMAC( ETH_HandleTypeDef * heth,
                                             ETH_MACInitTypeDef * macconf )
        {
            uint32_t tmpreg = 0uL;

            /* Process Locked */
            __HAL_LOCK( heth );

            /* Set the ETH peripheral state to BUSY */
            heth->State = HAL_ETH_STATE_BUSY;

            assert_param( IS_ETH_SPEED( heth->Init.Speed ) );
            assert_param( IS_ETH_DUPLEX_MODE( heth->Init.DuplexMode ) );

            if( macconf != NULL )
            {
                /* Check the parameters */
                assert_param( IS_ETH_WATCHDOG( macconf->Watchdog ) );
                assert_param( IS_ETH_JABBER( macconf->Jabber ) );
                assert_param( IS_ETH_INTER_FRAME_GAP( macconf->InterFrameGap ) );
                assert_param( IS_ETH_CARRIER_SENSE( macconf->CarrierSense ) );
                assert_param( IS_ETH_RECEIVE_OWN( macconf->ReceiveOwn ) );
                assert_param( IS_ETH_LOOPBACK_MODE( macconf->LoopbackMode ) );
                assert_param( IS_ETH_CHECKSUM_OFFLOAD( macconf->ChecksumOffload ) );
                assert_param( IS_ETH_RETRY_TRANSMISSION( macconf->RetryTransmission ) );
                assert_param( IS_ETH_AUTOMATIC_PADCRC_STRIP( macconf->AutomaticPadCRCStrip ) );
                assert_param( IS_ETH_BACKOFF_LIMIT( macconf->BackOffLimit ) );
                assert_param( IS_ETH_DEFERRAL_CHECK( macconf->DeferralCheck ) );
                assert_param( IS_ETH_RECEIVE_ALL( macconf->ReceiveAll ) );
                assert_param( IS_ETH_SOURCE_ADDR_FILTER( macconf->SourceAddrFilter ) );
                assert_param( IS_ETH_CONTROL_FRAMES( macconf->PassControlFrames ) );
                assert_param( IS_ETH_BROADCAST_FRAMES_RECEPTION( macconf->BroadcastFramesReception ) );
                assert_param( IS_ETH_DESTINATION_ADDR_FILTER( macconf->DestinationAddrFilter ) );
                assert_param( IS_ETH_PROMISCUOUS_MODE( macconf->PromiscuousMode ) );
                assert_param( IS_ETH_MULTICAST_FRAMES_FILTER( macconf->MulticastFramesFilter ) );
                assert_param( IS_ETH_UNICAST_FRAMES_FILTER( macconf->UnicastFramesFilter ) );
                assert_param( IS_ETH_PAUSE_TIME( macconf->PauseTime ) );
                assert_param( IS_ETH_ZEROQUANTA_PAUSE( macconf->ZeroQuantaPause ) );
                assert_param( IS_ETH_PAUSE_LOW_THRESHOLD( macconf->PauseLowThreshold ) );
                assert_param( IS_ETH_UNICAST_PAUSE_FRAME_DETECT( macconf->UnicastPauseFrameDetect ) );
                assert_param( IS_ETH_RECEIVE_FLOWCONTROL( macconf->ReceiveFlowControl ) );
                assert_param( IS_ETH_TRANSMIT_FLOWCONTROL( macconf->TransmitFlowControl ) );
                assert_param( IS_ETH_VLAN_TAG_COMPARISON( macconf->VLANTagComparison ) );
                assert_param( IS_ETH_VLAN_TAG_IDENTIFIER( macconf->VLANTagIdentifier ) );

                /*------------------------ ETHERNET MACCR Configuration --------------------*/
                /* Get the ETHERNET MACCR value */
                tmpreg = heth->Instance->MACCR;
                /* Clear WD, PCE, PS, TE and RE bits */
                tmpreg &= ETH_MACCR_CLEAR_MASK;

                tmpreg |= ( uint32_t ) (
                    macconf->Watchdog |
                    macconf->Jabber |
                    macconf->InterFrameGap |
                    macconf->CarrierSense |
                    heth->Init.Speed |
                    macconf->ReceiveOwn |
                    macconf->LoopbackMode |
                    heth->Init.DuplexMode |
                    macconf->ChecksumOffload |
                    macconf->RetryTransmission |
                    macconf->AutomaticPadCRCStrip |
                    macconf->BackOffLimit |
                    macconf->DeferralCheck );

                /* Write to ETHERNET MACCR */
                prvWriteMACCR( heth, tmpreg );

                /*----------------------- ETHERNET MACFFR Configuration --------------------*/
                /* Write to ETHERNET MACFFR */
                heth->Instance->MACFFR = ( uint32_t ) (
                    macconf->ReceiveAll |
                    macconf->SourceAddrFilter |
                    macconf->PassControlFrames |
                    macconf->BroadcastFramesReception |
                    macconf->DestinationAddrFilter |
                    macconf->PromiscuousMode |
                    macconf->MulticastFramesFilter |
                    macconf->UnicastFramesFilter );

                /* Wait until the write operation will be taken into account :
                 * at least four TX_CLK/RX_CLK clock cycles */
                tmpreg = heth->Instance->MACFFR;
                vRegisterDelay();
                heth->Instance->MACFFR = tmpreg;

                /*--------------- ETHERNET MACHTHR and MACHTLR Configuration ---------------*/
                /* Write to ETHERNET MACHTHR */
                heth->Instance->MACHTHR = ( uint32_t ) macconf->HashTableHigh;

                /* Write to ETHERNET MACHTLR */
                heth->Instance->MACHTLR = ( uint32_t ) macconf->HashTableLow;
                /*----------------------- ETHERNET MACFCR Configuration --------------------*/

                /* Get the ETHERNET MACFCR value */
                tmpreg = heth->Instance->MACFCR;
                /* Clear xx bits */
                tmpreg &= ETH_MACFCR_CLEAR_MASK;

                tmpreg |= ( uint32_t ) ( (
                                             macconf->PauseTime << 16 ) |
                                         macconf->ZeroQuantaPause |
                                         macconf->PauseLowThreshold |
                                         macconf->UnicastPauseFrameDetect |
                                         macconf->ReceiveFlowControl |
                                         macconf->TransmitFlowControl );

                /* Write to ETHERNET MACFCR */
                prvWriteMACFCR( heth, tmpreg );

                /*----------------------- ETHERNET MACVLANTR Configuration -----------------*/
                heth->Instance->MACVLANTR = ( uint32_t ) ( macconf->VLANTagComparison |
                                                           macconf->VLANTagIdentifier );

                /* Wait until the write operation will be taken into account :
                 * at least four TX_CLK/RX_CLK clock cycles */
                tmpreg = heth->Instance->MACVLANTR;
                vRegisterDelay();
                heth->Instance->MACVLANTR = tmpreg;
            }
            else /* macconf == NULL : here we just configure Speed and Duplex mode */
            {
                /*------------------------ ETHERNET MACCR Configuration --------------------*/
                /* Get the ETHERNET MACCR value */
                tmpreg = heth->Instance->MACCR;

                /* Clear FES and DM bits */
                tmpreg &= ~( ( uint32_t ) 0x00004800uL );

                tmpreg |= ( uint32_t ) ( heth->Init.Speed | heth->Init.DuplexMode );

                /* Write to ETHERNET MACCR */
                prvWriteMACCR( heth, tmpreg );
            }

            /* Set the ETH state to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK( heth );

            /* Return function status */
            return HAL_OK;
        }

/**
 * @brief  Sets ETH DMA Configuration.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @param  dmaconf: DMA Configuration structure
 * @retval HAL status
 */
        HAL_StatusTypeDef HAL_ETH_ConfigDMA( ETH_HandleTypeDef * heth,
                                             ETH_DMAInitTypeDef * dmaconf )
        {
            uint32_t tmpreg = 0uL;

            /* Process Locked */
            __HAL_LOCK( heth );

            /* Set the ETH peripheral state to BUSY */
            heth->State = HAL_ETH_STATE_BUSY;

            /* Check parameters */
            assert_param( IS_ETH_DROP_TCPIP_CHECKSUM_FRAME( dmaconf->DropTCPIPChecksumErrorFrame ) );
            assert_param( IS_ETH_RECEIVE_STORE_FORWARD( dmaconf->ReceiveStoreForward ) );
            assert_param( IS_ETH_FLUSH_RECEIVE_FRAME( dmaconf->FlushReceivedFrame ) );
            assert_param( IS_ETH_TRANSMIT_STORE_FORWARD( dmaconf->TransmitStoreForward ) );
            assert_param( IS_ETH_TRANSMIT_THRESHOLD_CONTROL( dmaconf->TransmitThresholdControl ) );
            assert_param( IS_ETH_FORWARD_ERROR_FRAMES( dmaconf->ForwardErrorFrames ) );
            assert_param( IS_ETH_FORWARD_UNDERSIZED_GOOD_FRAMES( dmaconf->ForwardUndersizedGoodFrames ) );
            assert_param( IS_ETH_RECEIVE_THRESHOLD_CONTROL( dmaconf->ReceiveThresholdControl ) );
            assert_param( IS_ETH_SECOND_FRAME_OPERATE( dmaconf->SecondFrameOperate ) );
            assert_param( IS_ETH_ADDRESS_ALIGNED_BEATS( dmaconf->AddressAlignedBeats ) );
            assert_param( IS_ETH_FIXED_BURST( dmaconf->FixedBurst ) );
            assert_param( IS_ETH_RXDMA_BURST_LENGTH( dmaconf->RxDMABurstLength ) );
            assert_param( IS_ETH_TXDMA_BURST_LENGTH( dmaconf->TxDMABurstLength ) );
            assert_param( IS_ETH_ENHANCED_DESCRIPTOR_FORMAT( dmaconf->EnhancedDescriptorFormat ) );
            assert_param( IS_ETH_DMA_DESC_SKIP_LENGTH( dmaconf->DescriptorSkipLength ) );
            assert_param( IS_ETH_DMA_ARBITRATION_ROUNDROBIN_RXTX( dmaconf->DMAArbitration ) );

            /*----------------------- ETHERNET DMAOMR Configuration --------------------*/
            /* Get the ETHERNET DMAOMR value */
            tmpreg = heth->Instance->DMAOMR;
            /* Clear xx bits */
            tmpreg &= ETH_DMAOMR_CLEAR_MASK;

            tmpreg |= ( uint32_t ) (
                dmaconf->DropTCPIPChecksumErrorFrame |
                dmaconf->ReceiveStoreForward |
                dmaconf->FlushReceivedFrame |
                dmaconf->TransmitStoreForward |
                dmaconf->TransmitThresholdControl |
                dmaconf->ForwardErrorFrames |
                dmaconf->ForwardUndersizedGoodFrames |
                dmaconf->ReceiveThresholdControl |
                dmaconf->SecondFrameOperate );

            /* Write to ETHERNET DMAOMR */
            prvWriteDMAOMR( heth, tmpreg );

            /*----------------------- ETHERNET DMABMR Configuration --------------------*/
            heth->Instance->DMABMR = ( uint32_t ) ( dmaconf->AddressAlignedBeats |
                                                    dmaconf->FixedBurst |
                                                    dmaconf->RxDMABurstLength | /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                                                    dmaconf->TxDMABurstLength |
                                                    dmaconf->EnhancedDescriptorFormat |
                                                    ( dmaconf->DescriptorSkipLength << 2 ) |
                                                    dmaconf->DMAArbitration |
                                                    ETH_DMABMR_USP ); /* Enable use of separate PBL for Rx and Tx */

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles */
            tmpreg = heth->Instance->DMABMR;
            vRegisterDelay();
            heth->Instance->DMABMR = tmpreg;

            /* Set the ETH state to Ready */
            heth->State = HAL_ETH_STATE_READY;

            /* Process Unlocked */
            __HAL_UNLOCK( heth );

            /* Return function status */
            return HAL_OK;
        }

/**
 * @}
 */

/** @defgroup ETH_Exported_Functions_Group4 Peripheral State functions
 *  @brief   Peripheral State functions
 *
 * @verbatim
 * ===============================================================================
 ##### Peripheral State functions #####
 #####===============================================================================
 #####[..]
 #####This subsection permits to get in run-time the status of the peripheral
 #####and the data flow.
 ##### (+) Get the ETH handle state:
 #####     HAL_ETH_GetState();
 #####
 #####
 #####@endverbatim
 * @{
 */

/**
 * @brief  Return the ETH HAL state
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval HAL state
 */
        HAL_ETH_StateTypeDef HAL_ETH_GetState( ETH_HandleTypeDef * heth )
        {
            /* Return ETH state */
            return heth->State;
        }

/**
 * @}
 */

/**
 * @}
 */

/** @addtogroup ETH_Private_Functions
 * @{
 */

/**
 * @brief  Configures Ethernet MAC and DMA with default parameters.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @param  err: Ethernet Init error
 * @retval HAL status
 */
        static void ETH_MACDMAConfig( ETH_HandleTypeDef * heth,
                                      uint32_t err )
        {
            ETH_MACInitTypeDef macinit;
            ETH_DMAInitTypeDef dmainit;
            uint32_t tmpreg = 0uL;

            if( err != ETH_SUCCESS ) /* Auto-negotiation failed */
            {
                /* Set Ethernet duplex mode to Full-duplex */
                heth->Init.DuplexMode = ETH_MODE_FULLDUPLEX;

                /* Set Ethernet speed to 100M */
                heth->Init.Speed = ETH_SPEED_100M;
            }

            /* Ethernet MAC default initialization **************************************/
            macinit.Watchdog = ETH_WATCHDOG_ENABLE;
            macinit.Jabber = ETH_JABBER_ENABLE;
            macinit.InterFrameGap = ETH_INTERFRAMEGAP_96BIT;
            macinit.CarrierSense = ETH_CARRIERSENCE_ENABLE;
            macinit.ReceiveOwn = ETH_RECEIVEOWN_ENABLE;
            macinit.LoopbackMode = ETH_LOOPBACKMODE_DISABLE;

            if( heth->Init.ChecksumMode == ETH_CHECKSUM_BY_HARDWARE )
            {
                macinit.ChecksumOffload = ETH_CHECKSUMOFFLAOD_ENABLE;
            }
            else
            {
                macinit.ChecksumOffload = ETH_CHECKSUMOFFLAOD_DISABLE;
            }

            macinit.RetryTransmission = ETH_RETRYTRANSMISSION_DISABLE;
            macinit.AutomaticPadCRCStrip = ETH_AUTOMATICPADCRCSTRIP_DISABLE;
            macinit.BackOffLimit = ETH_BACKOFFLIMIT_10;
            macinit.DeferralCheck = ETH_DEFFERRALCHECK_DISABLE;
            macinit.ReceiveAll = ETH_RECEIVEAll_DISABLE;
            macinit.SourceAddrFilter = ETH_SOURCEADDRFILTER_DISABLE;
            macinit.PassControlFrames = ETH_PASSCONTROLFRAMES_BLOCKALL;
            macinit.BroadcastFramesReception = ETH_BROADCASTFRAMESRECEPTION_ENABLE;
            macinit.DestinationAddrFilter = ETH_DESTINATIONADDRFILTER_NORMAL;
            macinit.PromiscuousMode = ETH_PROMISCUOUS_MODE_DISABLE;
            macinit.MulticastFramesFilter = ETH_MULTICASTFRAMESFILTER_PERFECT;
            macinit.UnicastFramesFilter = ETH_UNICASTFRAMESFILTER_PERFECT;
            macinit.HashTableHigh = 0x0uL;
            macinit.HashTableLow = 0x0uL;
            macinit.PauseTime = 0x0uL;
            macinit.ZeroQuantaPause = ETH_ZEROQUANTAPAUSE_DISABLE;
            macinit.PauseLowThreshold = ETH_PAUSELOWTHRESHOLD_MINUS4;
            macinit.UnicastPauseFrameDetect = ETH_UNICASTPAUSEFRAMEDETECT_DISABLE;
            macinit.ReceiveFlowControl = ETH_RECEIVEFLOWCONTROL_DISABLE;
            macinit.TransmitFlowControl = ETH_TRANSMITFLOWCONTROL_DISABLE;
            macinit.VLANTagComparison = ETH_VLANTAGCOMPARISON_16BIT;
            macinit.VLANTagIdentifier = 0x0uL;

            /*------------------------ ETHERNET MACCR Configuration --------------------*/
            /* Get the ETHERNET MACCR value */
            tmpreg = heth->Instance->MACCR;
            /* Clear WD, PCE, PS, TE and RE bits */
            tmpreg &= ETH_MACCR_CLEAR_MASK;
            /* Set the WD bit according to ETH Watchdog value */
            /* Set the JD: bit according to ETH Jabber value */
            /* Set the IFG bit according to ETH InterFrameGap value */
            /* Set the DCRS bit according to ETH CarrierSense value */
            /* Set the FES bit according to ETH Speed value */
            /* Set the DO bit according to ETH ReceiveOwn value */
            /* Set the LM bit according to ETH LoopbackMode value */
            /* Set the DM bit according to ETH Mode value */
            /* Set the IPCO bit according to ETH ChecksumOffload value */
            /* Set the DR bit according to ETH RetryTransmission value */
            /* Set the ACS bit according to ETH AutomaticPadCRCStrip value */
            /* Set the BL bit according to ETH BackOffLimit value */
            /* Set the DC bit according to ETH DeferralCheck value */
            tmpreg |= ( uint32_t ) ( macinit.Watchdog |
                                     macinit.Jabber |
                                     macinit.InterFrameGap |
                                     macinit.CarrierSense |
                                     heth->Init.Speed |
                                     macinit.ReceiveOwn |
                                     macinit.LoopbackMode |
                                     heth->Init.DuplexMode |
                                     macinit.ChecksumOffload |
                                     macinit.RetryTransmission |
                                     macinit.AutomaticPadCRCStrip |
                                     macinit.BackOffLimit |
                                     macinit.DeferralCheck );

            /* Write to ETHERNET MACCR */
            prvWriteMACCR( heth, tmpreg );

            /*----------------------- ETHERNET MACFFR Configuration --------------------*/
            /* Set the RA bit according to ETH ReceiveAll value */
            /* Set the SAF and SAIF bits according to ETH SourceAddrFilter value */
            /* Set the PCF bit according to ETH PassControlFrames value */
            /* Set the DBF bit according to ETH BroadcastFramesReception value */
            /* Set the DAIF bit according to ETH DestinationAddrFilter value */
            /* Set the PR bit according to ETH PromiscuousMode value */
            /* Set the PM, HMC and HPF bits according to ETH MulticastFramesFilter value */
            /* Set the HUC and HPF bits according to ETH UnicastFramesFilter value */
            /* Write to ETHERNET MACFFR */
            heth->Instance->MACFFR = ( uint32_t ) ( macinit.ReceiveAll |
                                                    macinit.SourceAddrFilter |
                                                    macinit.PassControlFrames |
                                                    macinit.BroadcastFramesReception |
                                                    macinit.DestinationAddrFilter |
                                                    macinit.PromiscuousMode |
                                                    macinit.MulticastFramesFilter |
                                                    macinit.UnicastFramesFilter );

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles */
            tmpreg = heth->Instance->MACFFR;
            vRegisterDelay();
            heth->Instance->MACFFR = tmpreg;

            /*--------------- ETHERNET MACHTHR and MACHTLR Configuration --------------*/
            /* Write to ETHERNET MACHTHR */
            heth->Instance->MACHTHR = ( uint32_t ) macinit.HashTableHigh;

            /* Write to ETHERNET MACHTLR */
            heth->Instance->MACHTLR = ( uint32_t ) macinit.HashTableLow;
            /*----------------------- ETHERNET MACFCR Configuration -------------------*/

            /* Get the ETHERNET MACFCR value */
            tmpreg = heth->Instance->MACFCR;
            /* Clear xx bits */
            tmpreg &= ETH_MACFCR_CLEAR_MASK;

            /* Set the PT bit according to ETH PauseTime value */
            /* Set the DZPQ bit according to ETH ZeroQuantaPause value */
            /* Set the PLT bit according to ETH PauseLowThreshold value */
            /* Set the UP bit according to ETH UnicastPauseFrameDetect value */
            /* Set the RFE bit according to ETH ReceiveFlowControl value */
            /* Set the TFE bit according to ETH TransmitFlowControl value */
            tmpreg |= ( uint32_t ) ( ( macinit.PauseTime << 16 ) |
                                     macinit.ZeroQuantaPause |
                                     macinit.PauseLowThreshold |
                                     macinit.UnicastPauseFrameDetect |
                                     macinit.ReceiveFlowControl |
                                     macinit.TransmitFlowControl );

            /* Write to ETHERNET MACFCR */
            prvWriteMACFCR( heth, tmpreg );

            /*----------------------- ETHERNET MACVLANTR Configuration ----------------*/
            /* Set the ETV bit according to ETH VLANTagComparison value */
            /* Set the VL bit according to ETH VLANTagIdentifier value */
            heth->Instance->MACVLANTR = ( uint32_t ) ( macinit.VLANTagComparison |
                                                       macinit.VLANTagIdentifier );

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles */
            tmpreg = heth->Instance->MACVLANTR;
            vRegisterDelay();
            heth->Instance->MACVLANTR = tmpreg;

            /* Ethernet DMA default initialization ************************************/
            dmainit.DropTCPIPChecksumErrorFrame = ETH_DROPTCPIPCHECKSUMERRORFRAME_ENABLE;
            dmainit.ReceiveStoreForward = ETH_RECEIVESTOREFORWARD_ENABLE;
            dmainit.FlushReceivedFrame = ETH_FLUSHRECEIVEDFRAME_ENABLE;
            dmainit.TransmitStoreForward = ETH_TRANSMITSTOREFORWARD_ENABLE;
            dmainit.TransmitThresholdControl = ETH_TRANSMITTHRESHOLDCONTROL_64BYTES;
            dmainit.ForwardErrorFrames = ETH_FORWARDERRORFRAMES_DISABLE;
            dmainit.ForwardUndersizedGoodFrames = ETH_FORWARDUNDERSIZEDGOODFRAMES_DISABLE;
            dmainit.ReceiveThresholdControl = ETH_RECEIVEDTHRESHOLDCONTROL_64BYTES;
            dmainit.SecondFrameOperate = ETH_SECONDFRAMEOPERARTE_ENABLE;
            dmainit.AddressAlignedBeats = ETH_ADDRESSALIGNEDBEATS_ENABLE;
            dmainit.FixedBurst = ETH_FIXEDBURST_ENABLE;
            dmainit.RxDMABurstLength = ETH_RXDMABURSTLENGTH_32BEAT;
            dmainit.TxDMABurstLength = ETH_TXDMABURSTLENGTH_32BEAT;
            dmainit.EnhancedDescriptorFormat = ETH_DMAENHANCEDDESCRIPTOR_ENABLE;
            dmainit.DescriptorSkipLength = 0x0uL;
            dmainit.DMAArbitration = ETH_DMAARBITRATION_ROUNDROBIN_RXTX_1_1;

            /* Get the ETHERNET DMAOMR value */
            tmpreg = heth->Instance->DMAOMR;
            /* Clear xx bits */
            tmpreg &= ETH_DMAOMR_CLEAR_MASK;

            /* Set the DT bit according to ETH DropTCPIPChecksumErrorFrame value */
            /* Set the RSF bit according to ETH ReceiveStoreForward value */
            /* Set the DFF bit according to ETH FlushReceivedFrame value */
            /* Set the TSF bit according to ETH TransmitStoreForward value */
            /* Set the TTC bit according to ETH TransmitThresholdControl value */
            /* Set the FEF bit according to ETH ForwardErrorFrames value */
            /* Set the FUF bit according to ETH ForwardUndersizedGoodFrames value */
            /* Set the RTC bit according to ETH ReceiveThresholdControl value */
            /* Set the OSF bit according to ETH SecondFrameOperate value */
            tmpreg |= ( uint32_t ) ( dmainit.DropTCPIPChecksumErrorFrame |
                                     dmainit.ReceiveStoreForward |
                                     dmainit.FlushReceivedFrame |
                                     dmainit.TransmitStoreForward |
                                     dmainit.TransmitThresholdControl |
                                     dmainit.ForwardErrorFrames |
                                     dmainit.ForwardUndersizedGoodFrames |
                                     dmainit.ReceiveThresholdControl |
                                     dmainit.SecondFrameOperate );

            /* Write to ETHERNET DMAOMR */
            prvWriteDMAOMR( heth, tmpreg );

            /*----------------------- ETHERNET DMABMR Configuration ------------------*/
            /* Set the AAL bit according to ETH AddressAlignedBeats value */
            /* Set the FB bit according to ETH FixedBurst value */
            /* Set the RPBL and 4*PBL bits according to ETH RxDMABurstLength value */
            /* Set the PBL and 4*PBL bits according to ETH TxDMABurstLength value */
            /* Set the Enhanced DMA descriptors bit according to ETH EnhancedDescriptorFormat value*/
            /* Set the DSL bit according to ETH DesciptorSkipLength value */
            /* Set the PR and DA bits according to ETH DMAArbitration value */
            heth->Instance->DMABMR = ( uint32_t ) ( dmainit.AddressAlignedBeats |
                                                    dmainit.FixedBurst |
                                                    dmainit.RxDMABurstLength | /* !! if 4xPBL is selected for Tx or Rx it is applied for the other */
                                                    dmainit.TxDMABurstLength |
                                                    dmainit.EnhancedDescriptorFormat |
                                                    ( dmainit.DescriptorSkipLength << 2 ) |
                                                    dmainit.DMAArbitration |
                                                    ETH_DMABMR_USP ); /* Enable use of separate PBL for Rx and Tx */

            /* Wait until the write operation will be taken into account:
             * at least four TX_CLK/RX_CLK clock cycles */
            tmpreg = heth->Instance->DMABMR;
            vRegisterDelay();
            heth->Instance->DMABMR = tmpreg;

            if( heth->Init.RxMode == ETH_RXINTERRUPT_MODE )
            {
                /* Enable the Ethernet Rx Interrupt */
                __HAL_ETH_DMA_ENABLE_IT( ( heth ), ETH_DMA_IT_NIS | ETH_DMA_IT_R );
            }

            /* Initialize MAC address in ethernet MAC */
            ETH_MACAddressConfig( heth, ETH_MAC_ADDRESS0, heth->Init.MACAddr );
        }

/**
 * @brief  Configures the selected MAC address.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @param  MacAddr: The MAC address to configure
 *          This parameter can be one of the following values:
 *             @arg ETH_MAC_Address0: MAC Address0
 *             @arg ETH_MAC_Address1: MAC Address1
 *             @arg ETH_MAC_Address2: MAC Address2
 *             @arg ETH_MAC_Address3: MAC Address3
 * @param  Addr: Pointer to MAC address buffer data (6 bytes)
 * @retval HAL status
 */
        static void ETH_MACAddressConfig( ETH_HandleTypeDef * heth,
                                          uint32_t MacAddr,
                                          uint8_t * Addr )
        {
            uint32_t tmpreg;

            ( void ) heth;

            /* Check the parameters */
            assert_param( IS_ETH_MAC_ADDRESS0123( MacAddr ) );

            /* Calculate the selected MAC address high register */
            /* Register ETH_MACA0HR: Bit 31 MO: Always 1. */
            tmpreg = 0x80000000uL | ( ( uint32_t ) Addr[ 5 ] << 8 ) | ( uint32_t ) Addr[ 4 ];
            /* Load the selected MAC address high register */
            ( *( __IO uint32_t * ) ( ( uint32_t ) ( ETH_MAC_ADDR_HBASE + MacAddr ) ) ) = tmpreg;
            /* Calculate the selected MAC address low register */
            tmpreg = ( ( uint32_t ) Addr[ 3 ] << 24 ) | ( ( uint32_t ) Addr[ 2 ] << 16 ) | ( ( uint32_t ) Addr[ 1 ] << 8 ) | Addr[ 0 ];

            /* Load the selected MAC address low register */
            ( *( __IO uint32_t * ) ( ( uint32_t ) ( ETH_MAC_ADDR_LBASE + MacAddr ) ) ) = tmpreg;
        }

/**
 * @brief  Enables the MAC transmission.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_MACTransmissionEnable( ETH_HandleTypeDef * heth )
        {
            uint32_t tmpreg = heth->Instance->MACCR | ETH_MACCR_TE;

            prvWriteMACCR( heth, tmpreg );
        }

/**
 * @brief  Disables the MAC transmission.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_MACTransmissionDisable( ETH_HandleTypeDef * heth )
        {
            uint32_t tmpreg = heth->Instance->MACCR & ~( ETH_MACCR_TE );

            prvWriteMACCR( heth, tmpreg );
        }

/**
 * @brief  Enables the MAC reception.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_MACReceptionEnable( ETH_HandleTypeDef * heth )
        {
            __IO uint32_t tmpreg = heth->Instance->MACCR | ETH_MACCR_RE;

            prvWriteMACCR( heth, tmpreg );
        }

/**
 * @brief  Disables the MAC reception.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_MACReceptionDisable( ETH_HandleTypeDef * heth )
        {
            __IO uint32_t tmpreg = heth->Instance->MACCR & ~( ETH_MACCR_RE );

            prvWriteMACCR( heth, tmpreg );
        }

/**
 * @brief  Enables the DMA transmission.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_DMATransmissionEnable( ETH_HandleTypeDef * heth )
        {
            /* Enable the DMA transmission */
            __IO uint32_t tmpreg = heth->Instance->DMAOMR | ETH_DMAOMR_ST;

            prvWriteDMAOMR( heth, tmpreg );
        }

/**
 * @brief  Disables the DMA transmission.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_DMATransmissionDisable( ETH_HandleTypeDef * heth )
        {
            /* Disable the DMA transmission */
            __IO uint32_t tmpreg = heth->Instance->DMAOMR & ~( ETH_DMAOMR_ST );

            prvWriteDMAOMR( heth, tmpreg );
        }

/**
 * @brief  Enables the DMA reception.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_DMAReceptionEnable( ETH_HandleTypeDef * heth )
        {
            /* Enable the DMA reception */
            __IO uint32_t tmpreg = heth->Instance->DMAOMR | ETH_DMAOMR_SR;

            prvWriteDMAOMR( heth, tmpreg );
        }

/**
 * @brief  Disables the DMA reception.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_DMAReceptionDisable( ETH_HandleTypeDef * heth )
        {
            /* Disable the DMA reception */
            __IO uint32_t tmpreg = heth->Instance->DMAOMR & ~( ETH_DMAOMR_SR );

            prvWriteDMAOMR( heth, tmpreg );
        }

/**
 * @brief  Clears the ETHERNET transmit FIFO.
 * @param  heth: pointer to a ETH_HandleTypeDef structure that contains
 *         the configuration information for ETHERNET module
 * @retval None
 */
        static void ETH_FlushTransmitFIFO( ETH_HandleTypeDef * heth )
        {
            /* Set the Flush Transmit FIFO bit */
            __IO uint32_t tmpreg = heth->Instance->DMAOMR | ETH_DMAOMR_FTF;

            prvWriteDMAOMR( heth, tmpreg );
        }

/**
 * @}
 */
    #endif /* stm_is_F2 != 0 || stm_is_F4 != 0 || stm_is_F7 */

#endif /* HAL_ETH_MODULE_ENABLED */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
