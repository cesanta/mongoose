/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FSL_USART_H_
#define FSL_USART_H_

#include "fsl_common.h"

/*!
 * @addtogroup usart_driver
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*! @{ */
/*! @brief USART driver version. */
#define FSL_USART_DRIVER_VERSION (MAKE_VERSION(2, 8, 4))
/*! @} */

#define USART_FIFOTRIG_TXLVL_GET(base) (((base)->FIFOTRIG & USART_FIFOTRIG_TXLVL_MASK) >> USART_FIFOTRIG_TXLVL_SHIFT)
#define USART_FIFOTRIG_RXLVL_GET(base) (((base)->FIFOTRIG & USART_FIFOTRIG_RXLVL_MASK) >> USART_FIFOTRIG_RXLVL_SHIFT)

/*! @brief Retry times for waiting flag.
 *
 * Defining to zero means to keep waiting for the flag until it is assert/deassert in blocking transfer,
 * otherwise the program will wait until the UART_RETRY_TIMES counts down to 0,
 * if the flag still remains unchanged then program will return kStatus_USART_Timeout.
 * It is not advised to use this macro in formal application to prevent any hardware error
 * because the actual wait period is affected by the compiler and optimization.
 */
#ifndef UART_RETRY_TIMES
#define UART_RETRY_TIMES 0U
#endif

/*! @brief Error codes for the USART driver. */
enum
{
    kStatus_USART_TxBusy              = MAKE_STATUS(kStatusGroup_LPC_USART, 0),  /*!< Transmitter is busy. */
    kStatus_USART_RxBusy              = MAKE_STATUS(kStatusGroup_LPC_USART, 1),  /*!< Receiver is busy. */
    kStatus_USART_TxIdle              = MAKE_STATUS(kStatusGroup_LPC_USART, 2),  /*!< USART transmitter is idle. */
    kStatus_USART_RxIdle              = MAKE_STATUS(kStatusGroup_LPC_USART, 3),  /*!< USART receiver is idle. */
    kStatus_USART_TxError             = MAKE_STATUS(kStatusGroup_LPC_USART, 7),  /*!< Error happens on txFIFO. */
    kStatus_USART_RxError             = MAKE_STATUS(kStatusGroup_LPC_USART, 9),  /*!< Error happens on rxFIFO. */
    kStatus_USART_RxRingBufferOverrun = MAKE_STATUS(kStatusGroup_LPC_USART, 8),  /*!< Error happens on rx ring buffer */
    kStatus_USART_NoiseError          = MAKE_STATUS(kStatusGroup_LPC_USART, 10), /*!< USART noise error. */
    kStatus_USART_FramingError        = MAKE_STATUS(kStatusGroup_LPC_USART, 11), /*!< USART framing error. */
    kStatus_USART_ParityError         = MAKE_STATUS(kStatusGroup_LPC_USART, 12), /*!< USART parity error. */
    kStatus_USART_BaudrateNotSupport =
        MAKE_STATUS(kStatusGroup_LPC_USART, 13), /*!< Baudrate is not support in current clock source */
#if UART_RETRY_TIMES
    kStatus_USART_Timeout = MAKE_STATUS(kStatusGroup_LPC_USART, 14), /*!< USART time out. */
#endif
};

/*! @brief USART synchronous mode. */
typedef enum _usart_sync_mode
{
    kUSART_SyncModeDisabled = 0x0U, /*!< Asynchronous mode.       */
    kUSART_SyncModeSlave    = 0x2U, /*!< Synchronous slave mode.  */
    kUSART_SyncModeMaster   = 0x3U, /*!< Synchronous master mode. */
} usart_sync_mode_t;

/*! @brief USART parity mode. */
typedef enum _usart_parity_mode
{
    kUSART_ParityDisabled = 0x0U, /*!< Parity disabled */
    kUSART_ParityEven     = 0x2U, /*!< Parity enabled, type even, bit setting: PE|PT = 10 */
    kUSART_ParityOdd      = 0x3U, /*!< Parity enabled, type odd,  bit setting: PE|PT = 11 */
} usart_parity_mode_t;

/*! @brief USART stop bit count. */
typedef enum _usart_stop_bit_count
{
    kUSART_OneStopBit = 0U, /*!< One stop bit */
    kUSART_TwoStopBit = 1U, /*!< Two stop bits */
} usart_stop_bit_count_t;

/*! @brief USART data size. */
typedef enum _usart_data_len
{
    kUSART_7BitsPerChar = 0U, /*!< Seven bit mode */
    kUSART_8BitsPerChar = 1U, /*!< Eight bit mode */
} usart_data_len_t;

/*! @brief USART clock polarity configuration, used in sync mode.*/
typedef enum _usart_clock_polarity
{
    kUSART_RxSampleOnFallingEdge = 0x0U, /*!< Un_RXD is sampled on the falling edge of SCLK. */
    kUSART_RxSampleOnRisingEdge  = 0x1U, /*!< Un_RXD is sampled on the rising edge of SCLK. */
} usart_clock_polarity_t;

/*! @brief txFIFO watermark values */
typedef enum _usart_txfifo_watermark
{
    kUSART_TxFifo0 = 0, /*!< USART tx watermark is empty */
    kUSART_TxFifo1 = 1, /*!< USART tx watermark at 1 item */
    kUSART_TxFifo2 = 2, /*!< USART tx watermark at 2 items */
    kUSART_TxFifo3 = 3, /*!< USART tx watermark at 3 items */
    kUSART_TxFifo4 = 4, /*!< USART tx watermark at 4 items */
    kUSART_TxFifo5 = 5, /*!< USART tx watermark at 5 items */
    kUSART_TxFifo6 = 6, /*!< USART tx watermark at 6 items */
    kUSART_TxFifo7 = 7, /*!< USART tx watermark at 7 items */
} usart_txfifo_watermark_t;

/*! @brief rxFIFO watermark values */
typedef enum _usart_rxfifo_watermark
{
    kUSART_RxFifo1 = 0, /*!< USART rx watermark at 1 item */
    kUSART_RxFifo2 = 1, /*!< USART rx watermark at 2 items */
    kUSART_RxFifo3 = 2, /*!< USART rx watermark at 3 items */
    kUSART_RxFifo4 = 3, /*!< USART rx watermark at 4 items */
    kUSART_RxFifo5 = 4, /*!< USART rx watermark at 5 items */
    kUSART_RxFifo6 = 5, /*!< USART rx watermark at 6 items */
    kUSART_RxFifo7 = 6, /*!< USART rx watermark at 7 items */
    kUSART_RxFifo8 = 7, /*!< USART rx watermark at 8 items */
} usart_rxfifo_watermark_t;

/*!
 * @brief USART interrupt configuration structure, default settings all disabled.
 */
enum _usart_interrupt_enable
{
    kUSART_TxErrorInterruptEnable = (USART_FIFOINTENSET_TXERR_MASK),
    kUSART_RxErrorInterruptEnable = (USART_FIFOINTENSET_RXERR_MASK),
    kUSART_TxLevelInterruptEnable = (USART_FIFOINTENSET_TXLVL_MASK),
    kUSART_RxLevelInterruptEnable = (USART_FIFOINTENSET_RXLVL_MASK),
    kUSART_TxIdleInterruptEnable  = (USART_INTENSET_TXIDLEEN_MASK << 16U), /*!< Transmitter idle. */
    kUSART_CtsChangeInterruptEnable =
        (USART_INTENSET_DELTACTSEN_MASK << 16U), /*!< Change in the state of the CTS input. */
    kUSART_RxBreakChangeInterruptEnable =
        (USART_INTENSET_DELTARXBRKEN_MASK),                              /*!< Break condition asserted or deasserted. */
    kUSART_RxStartInterruptEnable       = (USART_INTENSET_STARTEN_MASK), /*!< Rx start bit detected. */
    kUSART_FramingErrorInterruptEnable  = (USART_INTENSET_FRAMERREN_MASK),   /*!< Framing error detected. */
    kUSART_ParityErrorInterruptEnable   = (USART_INTENSET_PARITYERREN_MASK), /*!< Parity error detected. */
    kUSART_NoiseErrorInterruptEnable    = (USART_INTENSET_RXNOISEEN_MASK),   /*!< Noise error detected. */
    kUSART_AutoBaudErrorInterruptEnable = (USART_INTENSET_ABERREN_MASK),     /*!< Auto baudrate error detected. */
#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
    kUSART_RxTimeoutInterruptEnable = (USART_FIFOINTENSET_RXTIMEOUT_MASK), /*!< Receive timeout detected. */
#endif
    kUSART_AllInterruptEnables =
        kUSART_TxErrorInterruptEnable | kUSART_RxErrorInterruptEnable | kUSART_TxLevelInterruptEnable |
        kUSART_RxLevelInterruptEnable | kUSART_TxIdleInterruptEnable | kUSART_CtsChangeInterruptEnable |
        kUSART_RxBreakChangeInterruptEnable | kUSART_RxStartInterruptEnable | kUSART_FramingErrorInterruptEnable |
        kUSART_ParityErrorInterruptEnable | kUSART_NoiseErrorInterruptEnable |
#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
        kUSART_RxTimeoutInterruptEnable |
#endif
        kUSART_AutoBaudErrorInterruptEnable,
};

/*!
 * @brief USART status flags.
 *
 * This provides constants for the USART status flags for use in the USART functions.
 */
enum _usart_flags
{
    kUSART_TxError            = (USART_FIFOSTAT_TXERR_MASK),       /*!< TXERR bit, sets if TX buffer is error */
    kUSART_RxError            = (USART_FIFOSTAT_RXERR_MASK),       /*!< RXERR bit, sets if RX buffer is error */
    kUSART_TxFifoEmptyFlag    = (USART_FIFOSTAT_TXEMPTY_MASK),     /*!< TXEMPTY bit, sets if TX buffer is empty */
    kUSART_TxFifoNotFullFlag  = (USART_FIFOSTAT_TXNOTFULL_MASK),   /*!< TXNOTFULL bit, sets if TX buffer is not full */
    kUSART_RxFifoNotEmptyFlag = (USART_FIFOSTAT_RXNOTEMPTY_MASK),  /*!< RXNOEMPTY bit, sets if RX buffer is not empty */
    kUSART_RxFifoFullFlag     = (USART_FIFOSTAT_RXFULL_MASK),      /*!< RXFULL bit, sets if RX buffer is full */
    kUSART_RxIdleFlag         = (USART_STAT_RXIDLE_MASK << 16U),   /*!< Receiver idle. */
    kUSART_TxIdleFlag         = (USART_STAT_TXIDLE_MASK << 16U),   /*!< Transmitter idle. */
    kUSART_CtsAssertFlag      = (USART_STAT_CTS_MASK << 16U),      /*!< CTS signal high. */
    kUSART_CtsChangeFlag      = (USART_STAT_DELTACTS_MASK << 16U), /*!< CTS signal changed interrupt status. */
    kUSART_BreakDetectFlag = (USART_STAT_RXBRK_MASK), /*!< Break detected. Self cleared when rx pin goes high again. */
    kUSART_BreakDetectChangeFlag = (USART_STAT_DELTARXBRK_MASK), /*!< Break detect change interrupt flag. A change in
                                                                    the state of receiver break detection. */
    kUSART_RxStartFlag       = (USART_STAT_START_MASK),          /*!< Rx start bit detected interrupt flag. */
    kUSART_FramingErrorFlag  = (USART_STAT_FRAMERRINT_MASK),     /*!< Framing error interrupt flag. */
    kUSART_ParityErrorFlag   = (USART_STAT_PARITYERRINT_MASK),   /*!< parity error interrupt flag. */
    kUSART_NoiseErrorFlag    = (USART_STAT_RXNOISEINT_MASK),     /*!< Noise error interrupt flag. */
    kUSART_AutobaudErrorFlag = (USART_STAT_ABERR_MASK), /*!< Auto baudrate error interrupt flag, caused by the baudrate
                                                           counter timeout before the end of start bit. */
#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
    kUSART_RxTimeoutFlag = (USART_FIFOSTAT_RXTIMEOUT_MASK), /*!< RXTIMEOUT bit, sets if RX FIFO Timeout. */
#endif
    kUSART_AllClearFlags = kUSART_TxError | kUSART_RxError | kUSART_CtsChangeFlag | kUSART_BreakDetectChangeFlag |
                           kUSART_RxStartFlag | kUSART_FramingErrorFlag | kUSART_ParityErrorFlag |
#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
                           kUSART_RxTimeoutFlag |
#endif
                           kUSART_NoiseErrorFlag | kUSART_AutobaudErrorFlag,
};

#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
/*! @brief USART receive timeout configuration structure. */
typedef struct _usart_rx_timeout_config
{
    bool enable;                /*!< Enable RX timeout */
    bool resetCounterOnEmpty;   /*!< Enable RX timeout counter reset when RX FIFO becames empty. */
    bool resetCounterOnReceive; /*!< Enable RX timeout counter reset when RX FIFO receives data from the transmitter
                                   side. */
    uint32_t counter;           /*!< RX timeout counter*/
    uint8_t prescaler;          /*!< RX timeout prescaler*/
} usart_rx_timeout_config;
#endif

/*! @brief USART configuration structure. */
typedef struct _usart_config
{
    uint32_t baudRate_Bps;                /*!< USART baud rate  */
    usart_parity_mode_t parityMode;       /*!< Parity mode, disabled (default), even, odd */
    usart_stop_bit_count_t stopBitCount;  /*!< Number of stop bits, 1 stop bit (default) or 2 stop bits  */
    usart_data_len_t bitCountPerChar;     /*!< Data length - 7 bit, 8 bit  */
    bool loopback;                        /*!< Enable peripheral loopback */
    bool enableRx;                        /*!< Enable RX */
    bool enableTx;                        /*!< Enable TX */
    bool enableContinuousSCLK;            /*!< USART continuous Clock generation enable in synchronous master mode. */
    bool enableMode32k;                   /*!< USART uses 32 kHz clock from the RTC oscillator as the clock source. */
    bool enableHardwareFlowControl;       /*!< Enable hardware control RTS/CTS */
    usart_txfifo_watermark_t txWatermark; /*!< txFIFO watermark */
    usart_rxfifo_watermark_t rxWatermark; /*!< rxFIFO watermark */
    usart_sync_mode_t syncMode; /*!< Transfer mode select - asynchronous, synchronous master, synchronous slave. */
    usart_clock_polarity_t clockPolarity; /*!< Selects the clock polarity and sampling edge in synchronous mode. */
#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
    usart_rx_timeout_config rxTimeout; /*!< rx timeout configuration */
#endif
} usart_config_t;

/*! @brief USART transfer structure. */
typedef struct _usart_transfer
{
    /*
     * Use separate TX and RX data pointer, because TX data is const data.
     * The member data is kept for backward compatibility.
     */
    union
    {
        uint8_t *data;         /*!< The buffer of data to be transfer.*/
        uint8_t *rxData;       /*!< The buffer to receive data. */
        const uint8_t *txData; /*!< The buffer of data to be sent. */
    };
    size_t dataSize; /*!< The byte count to be transfer. */
} usart_transfer_t;

/* Forward declaration of the handle typedef. */
typedef struct _usart_handle usart_handle_t;

/*! @brief USART transfer callback function. */
typedef void (*usart_transfer_callback_t)(USART_Type *base, usart_handle_t *handle, status_t status, void *userData);

/*! @brief USART handle structure. */
struct _usart_handle
{
    const uint8_t *volatile txData; /*!< Address of remaining data to send. */
    volatile size_t txDataSize;     /*!< Size of the remaining data to send. */
    size_t txDataSizeAll;           /*!< Size of the data to send out. */
    uint8_t *volatile rxData;       /*!< Address of remaining data to receive. */
    volatile size_t rxDataSize;     /*!< Size of the remaining data to receive. */
    size_t rxDataSizeAll;           /*!< Size of the data to receive. */

    uint8_t *rxRingBuffer;              /*!< Start address of the receiver ring buffer. */
    size_t rxRingBufferSize;            /*!< Size of the ring buffer. */
    volatile uint16_t rxRingBufferHead; /*!< Index for the driver to store received data into ring buffer. */
    volatile uint16_t rxRingBufferTail; /*!< Index for the user to get data from the ring buffer. */

    usart_transfer_callback_t callback; /*!< Callback function. */
    void *userData;                     /*!< USART callback function parameter.*/

    volatile uint8_t txState; /*!< TX transfer state. */
    volatile uint8_t rxState; /*!< RX transfer state */

    uint8_t txWatermark; /*!< txFIFO watermark */
    uint8_t rxWatermark; /*!< rxFIFO watermark */
};

/*! @brief Typedef for usart interrupt handler. */
typedef void (*flexcomm_usart_irq_handler_t)(USART_Type *base, usart_handle_t *handle);

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* _cplusplus */

/*! @brief Returns instance number for USART peripheral base address. */
uint32_t USART_GetInstance(USART_Type *base);

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Initializes a USART instance with user configuration structure and peripheral clock.
 *
 * This function configures the USART module with the user-defined settings. The user can configure the configuration
 * structure and also get the default configuration by using the USART_GetDefaultConfig() function.
 * Example below shows how to use this API to configure USART.
 * @code
 *  usart_config_t usartConfig;
 *  usartConfig.baudRate_Bps = 115200U;
 *  usartConfig.parityMode = kUSART_ParityDisabled;
 *  usartConfig.stopBitCount = kUSART_OneStopBit;
 *  USART_Init(USART1, &usartConfig, 20000000U);
 * @endcode
 *
 * @param base USART peripheral base address.
 * @param config Pointer to user-defined configuration structure.
 * @param srcClock_Hz USART clock source frequency in HZ.
 * @retval kStatus_USART_BaudrateNotSupport Baudrate is not support in current clock source.
 * @retval kStatus_InvalidArgument USART base address is not valid
 * @retval kStatus_Success Status USART initialize succeed
 */
status_t USART_Init(USART_Type *base, const usart_config_t *config, uint32_t srcClock_Hz);
#if defined(FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG) && FSL_FEATURE_USART_HAS_FIFORXTIMEOUTCFG
/*!
 * @brief Calculate the USART instance RX timeout prescaler and counter.
 *
 * This function for calculate the USART RXFIFO timeout config. This function is used to calculate
 * suitable prescaler and counter for target_us.
 * @code
 *   usart_config_t config;
 *   config.rxWatermark                     = kUSART_RxFifo2;
 *   config.rxTimeout.enable                = true;
 *   config.rxTimeout.resetCounterOnEmpty   = true;
 *   config.rxTimeout.resetCounterOnReceive = true;
 *   USART_CalcTimeoutConfig(200U, &config.rxTimeout.prescaler, &config.rxTimeout.counter,
 *                                    CLOCK_GetFreq(kCLOCK_BusClk));
 * @endcode
 * @param target_us  Time for rx timeout unit us.
 * @param rxTimeoutPrescaler The prescaler to be setted after function.
 * @param rxTimeoutcounter The counter to be setted after function.
 * @param srcClock_Hz The clockSrc for rx timeout.
 */
void USART_CalcTimeoutConfig(uint32_t target_us,
                             uint8_t *rxTimeoutPrescaler,
                             uint32_t *rxTimeoutcounter,
                             uint32_t srcClock_Hz);
/*!
 * @brief Sets the USART instance RX timeout config.
 *
 * This function configures the USART RXFIFO timeout config. This function is used to config
 * the USART RXFIFO timeout config after the USART module is initialized by the USART_Init.
 *
 * @param base USART peripheral base address.
 * @param config pointer to receive timeout configuration structure.
 */
void USART_SetRxTimeoutConfig(USART_Type *base, const usart_rx_timeout_config *config);
#endif
/*!
 * @brief Deinitializes a USART instance.
 *
 * This function waits for TX complete, disables TX and RX, and disables the USART clock.
 *
 * @param base USART peripheral base address.
 */
void USART_Deinit(USART_Type *base);

/*!
 * @brief Gets the default configuration structure.
 *
 * This function initializes the USART configuration structure to a default value. The default
 * values are:
 *   usartConfig->baudRate_Bps = 115200U;
 *   usartConfig->parityMode = kUSART_ParityDisabled;
 *   usartConfig->stopBitCount = kUSART_OneStopBit;
 *   usartConfig->bitCountPerChar = kUSART_8BitsPerChar;
 *   usartConfig->loopback = false;
 *   usartConfig->enableTx = false;
 *   usartConfig->enableRx = false;
 *
 * @param config Pointer to configuration structure.
 */
void USART_GetDefaultConfig(usart_config_t *config);

/*!
 * @brief Sets the USART instance baud rate.
 *
 * This function configures the USART module baud rate. This function is used to update
 * the USART module baud rate after the USART module is initialized by the USART_Init.
 * @code
 *  USART_SetBaudRate(USART1, 115200U, 20000000U);
 * @endcode
 *
 * @param base USART peripheral base address.
 * @param baudrate_Bps USART baudrate to be set.
 * @param srcClock_Hz USART clock source frequency in HZ.
 * @retval kStatus_USART_BaudrateNotSupport Baudrate is not support in current clock source.
 * @retval kStatus_Success Set baudrate succeed.
 * @retval kStatus_InvalidArgument One or more arguments are invalid.
 */
status_t USART_SetBaudRate(USART_Type *base, uint32_t baudrate_Bps, uint32_t srcClock_Hz);

/*!
 * @brief Enable 32 kHz mode which USART uses clock from the RTC oscillator as the clock source
 *
 * Please note that in order to use a 32 kHz clock to operate USART properly, the RTC oscillator
 * and its 32 kHz output must be manully enabled by user, by calling RTC_Init and setting
 * SYSCON_RTCOSCCTRL_EN bit to 1.
 * And in 32kHz clocking mode the USART can only work at 9600 baudrate or at the baudrate that
 * 9600 can evenly divide, eg: 4800, 3200.
 *
 * @param base USART peripheral base address.
 * @param baudRate_Bps USART baudrate to be set..
 * @param enableMode32k true is 32k mode, false is normal mode.
 * @param srcClock_Hz USART clock source frequency in HZ.
 * @retval kStatus_USART_BaudrateNotSupport Baudrate is not support in current clock source.
 * @retval kStatus_Success Set baudrate succeed.
 * @retval kStatus_InvalidArgument One or more arguments are invalid.
 */
status_t USART_Enable32kMode(USART_Type *base, uint32_t baudRate_Bps, bool enableMode32k, uint32_t srcClock_Hz);

/*!
 * @brief Enable 9-bit data mode for USART.
 *
 * This function set the 9-bit mode for USART module. The 9th bit is not used for parity thus can be modified by user.
 *
 * @param base USART peripheral base address.
 * @param enable true to enable, false to disable.
 */
void USART_Enable9bitMode(USART_Type *base, bool enable);

/*!
 * @brief Set the USART slave address.
 *
 * This function configures the address for USART module that works as slave in 9-bit data mode. When the address
 * detection is enabled, the frame it receices with MSB being 1 is considered as an address frame, otherwise it is
 * considered as data frame. Once the address frame matches slave's own addresses, this slave is addressed. This
 * address frame and its following data frames are stored in the receive buffer, otherwise the frames will be discarded.
 * To un-address a slave, just send an address frame with unmatched address.
 *
 * @note Any USART instance joined in the multi-slave system can work as slave. The position of the address mark is the
 * same as the parity bit when parity is enabled for 8 bit and 9 bit data formats.
 *
 * @param base USART peripheral base address.
 * @param address USART slave address.
 */
static inline void USART_SetMatchAddress(USART_Type *base, uint8_t address)
{
    /* Configure match address. */
    base->ADDR = (uint32_t)address;
}

/*!
 * @brief Enable the USART match address feature.
 *
 * @param base USART peripheral base address.
 * @param match true to enable match address, false to disable.
 */
static inline void USART_EnableMatchAddress(USART_Type *base, bool match)
{
    /* Configure match address enable bit. */
    if (match)
    {
        base->CFG |= (uint32_t)USART_CFG_AUTOADDR_MASK;
        base->CTL |= (uint32_t)USART_CTL_ADDRDET_MASK;
    }
    else
    {
        base->CFG &= ~(uint32_t)USART_CFG_AUTOADDR_MASK;
        base->CTL &= ~(uint32_t)USART_CTL_ADDRDET_MASK;
    }
}

/*! @} */

/*!
 * @name Status
 * @{
 */

/*!
 * @brief Get USART status flags.
 *
 * This function get all USART status flags, the flags are returned as the logical
 * OR value of the enumerators @ref _usart_flags. To check a specific status,
 * compare the return value with enumerators in @ref _usart_flags.
 * For example, to check whether the TX is empty:
 * @code
 *     if (kUSART_TxFifoNotFullFlag & USART_GetStatusFlags(USART1))
 *     {
 *         ...
 *     }
 * @endcode
 *
 * @param base USART peripheral base address.
 * @return USART status flags which are ORed by the enumerators in the _usart_flags.
 */
static inline uint32_t USART_GetStatusFlags(USART_Type *base)
{
    return (base->FIFOSTAT & 0xFF0000FFUL) | (base->STAT & 0xFFUL) << 16U | (base->STAT & 0xFFFF00UL);
}

/*!
 * @brief Clear USART status flags.
 *
 * This function clear supported USART status flags.
 * The mask is a logical OR of enumeration members. See @ref kUSART_AllClearFlags.
 * For example:
 * @code
 *     USART_ClearStatusFlags(USART1, kUSART_TxError | kUSART_RxError)
 * @endcode
 *
 * @param base USART peripheral base address.
 * @param mask status flags to be cleared.
 */
static inline void USART_ClearStatusFlags(USART_Type *base, uint32_t mask)
{
    mask &= (uint32_t)kUSART_AllClearFlags;
    /* Clear the clearable status in STAT register. */
    base->STAT = (mask & 0xFFFF00UL) | ((mask & 0xFF0000UL) >> 16U);
    /* Only TXERR, RXERR fields support write. Remaining fields should be set to zero */
    base->FIFOSTAT = mask & (USART_FIFOSTAT_TXERR_MASK | USART_FIFOSTAT_RXERR_MASK);
}

/*! @} */

/*!
 * @name Interrupts
 * @{
 */
/*!
 * @brief Enables USART interrupts according to the provided mask.
 *
 * This function enables the USART interrupts according to the provided mask. The mask
 * is a logical OR of enumeration members. See @ref _usart_interrupt_enable.
 * For example, to enable TX empty interrupt and RX full interrupt:
 * @code
 *     USART_EnableInterrupts(USART1, kUSART_TxLevelInterruptEnable | kUSART_RxLevelInterruptEnable);
 * @endcode
 *
 * @param base USART peripheral base address.
 * @param mask The interrupts to enable. Logical OR of @ref _usart_interrupt_enable.
 */
static inline void USART_EnableInterrupts(USART_Type *base, uint32_t mask)
{
    mask &= (uint32_t)kUSART_AllInterruptEnables;
    base->INTENSET     = (mask & 0x1FF00UL) | ((mask & 0xFF0000UL) >> 16U);
    base->FIFOINTENSET = mask & 0xF00000FUL;
}

/*!
 * @brief Disables USART interrupts according to a provided mask.
 *
 * This function disables the USART interrupts according to a provided mask. The mask
 * is a logical OR of enumeration members. See @ref _usart_interrupt_enable.
 * This example shows how to disable the TX empty interrupt and RX full interrupt:
 * @code
 *     USART_DisableInterrupts(USART1, kUSART_TxLevelInterruptEnable | kUSART_RxLevelInterruptEnable);
 * @endcode
 *
 * @param base USART peripheral base address.
 * @param mask The interrupts to disable. Logical OR of @ref _usart_interrupt_enable.
 */
static inline void USART_DisableInterrupts(USART_Type *base, uint32_t mask)
{
    mask &= (uint32_t)kUSART_AllInterruptEnables;
    base->INTENCLR     = (mask & 0x1FF00UL) | ((mask & 0xFF0000UL) >> 16U);
    base->FIFOINTENCLR = mask & 0xFUL;
}

/*!
 * @brief Returns enabled USART interrupts.
 *
 * This function returns the enabled USART interrupts.
 *
 * @param base USART peripheral base address.
 */
static inline uint32_t USART_GetEnabledInterrupts(USART_Type *base)
{
    return (base->INTENSET & 0x1FF00UL) | ((base->INTENSET & 0xFFUL) << 16UL) | (base->FIFOINTENSET & 0xFUL);
}

/*!
 * @brief Enable DMA for Tx
 */
static inline void USART_EnableTxDMA(USART_Type *base, bool enable)
{
    if (enable)
    {
        base->FIFOCFG |= USART_FIFOCFG_DMATX_MASK;
    }
    else
    {
        base->FIFOCFG &= ~(USART_FIFOCFG_DMATX_MASK);
    }
}

/*!
 * @brief Enable DMA for Rx
 */
static inline void USART_EnableRxDMA(USART_Type *base, bool enable)
{
    if (enable)
    {
        base->FIFOCFG |= USART_FIFOCFG_DMARX_MASK;
    }
    else
    {
        base->FIFOCFG &= ~(USART_FIFOCFG_DMARX_MASK);
    }
}

/*!
 * @brief Enable CTS.
 * This function will determine whether CTS is used for flow control.
 *
 * @param base    USART peripheral base address.
 * @param enable  Enable CTS or not, true for enable and false for disable.
 */
static inline void USART_EnableCTS(USART_Type *base, bool enable)
{
    if (enable)
    {
        base->CFG |= USART_CFG_CTSEN_MASK;
    }
    else
    {
        base->CFG &= ~USART_CFG_CTSEN_MASK;
    }
}

/*!
 * @brief Continuous Clock generation.
 * By default, SCLK is only output while data is being transmitted in synchronous mode.
 * Enable this funciton, SCLK will run continuously in synchronous mode, allowing
 * characters to be received on Un_RxD independently from transmission on Un_TXD).
 *
 * @param base    USART peripheral base address.
 * @param enable  Enable Continuous Clock generation mode or not, true for enable and false for disable.
 */
static inline void USART_EnableContinuousSCLK(USART_Type *base, bool enable)
{
    if (enable)
    {
        base->CTL |= USART_CTL_CC_MASK;
    }
    else
    {
        base->CTL &= ~USART_CTL_CC_MASK;
    }
}

/*!
 * @brief Enable Continuous Clock generation bit auto clear.
 * While enable this cuntion, the Continuous Clock bit is automatically cleared when a complete
 * character has been received. This bit is cleared at the same time.
 *
 * @param base    USART peripheral base address.
 * @param enable  Enable auto clear or not, true for enable and false for disable.
 */
static inline void USART_EnableAutoClearSCLK(USART_Type *base, bool enable)
{
    if (enable)
    {
        base->CTL |= USART_CTL_CLRCCONRX_MASK;
    }
    else
    {
        base->CTL &= ~USART_CTL_CLRCCONRX_MASK;
    }
}

/*!
 * @brief Sets the rx FIFO watermark.
 *
 * @param base USART peripheral base address.
 * @param water Rx FIFO watermark.
 */
static inline void USART_SetRxFifoWatermark(USART_Type *base, uint8_t water)
{
    assert(water <= (USART_FIFOTRIG_RXLVL_MASK >> USART_FIFOTRIG_RXLVL_SHIFT));
    base->FIFOTRIG = (base->FIFOTRIG & ~USART_FIFOTRIG_RXLVL_MASK) | USART_FIFOTRIG_RXLVL(water);
}

/*!
 * @brief Sets the tx FIFO watermark.
 *
 * @param base USART peripheral base address.
 * @param water Tx FIFO watermark.
 */
static inline void USART_SetTxFifoWatermark(USART_Type *base, uint8_t water)
{
    assert(water <= (USART_FIFOTRIG_TXLVL_MASK >> USART_FIFOTRIG_TXLVL_SHIFT));
    base->FIFOTRIG = (base->FIFOTRIG & ~USART_FIFOTRIG_TXLVL_MASK) | USART_FIFOTRIG_TXLVL(water);
}
/*! @} */

/*!
 * @name Bus Operations
 * @{
 */

/*!
 * @brief Writes to the FIFOWR register.
 *
 * This function writes data to the txFIFO directly. The upper layer must ensure
 * that txFIFO has space for data to write before calling this function.
 *
 * @param base USART peripheral base address.
 * @param data The byte to write.
 */
static inline void USART_WriteByte(USART_Type *base, uint8_t data)
{
    base->FIFOWR = data;
}

/*!
 * @brief Reads the FIFORD register directly.
 *
 * This function reads data from the rxFIFO directly. The upper layer must
 * ensure that the rxFIFO is not empty before calling this function.
 *
 * @param base USART peripheral base address.
 * @return The byte read from USART data register.
 */
static inline uint8_t USART_ReadByte(USART_Type *base)
{
    return (uint8_t)base->FIFORD;
}

/*!
 * @brief Gets the rx FIFO data count.
 *
 * @param base USART peripheral base address.
 * @return rx FIFO data count.
 */
static inline uint8_t USART_GetRxFifoCount(USART_Type *base)
{
    return (uint8_t)((base->FIFOSTAT & USART_FIFOSTAT_RXLVL_MASK) >> USART_FIFOSTAT_RXLVL_SHIFT);
}

/*!
 * @brief Gets the tx FIFO data count.
 *
 * @param base USART peripheral base address.
 * @return tx FIFO data count.
 */
static inline uint8_t USART_GetTxFifoCount(USART_Type *base)
{
    return (uint8_t)((base->FIFOSTAT & USART_FIFOSTAT_TXLVL_MASK) >> USART_FIFOSTAT_TXLVL_SHIFT);
}

/*!
 * @brief Transmit an address frame in 9-bit data mode.
 *
 * @param base USART peripheral base address.
 * @param address USART slave address.
 */
void USART_SendAddress(USART_Type *base, uint8_t address);

/*!
 * @brief Writes to the TX register using a blocking method.
 *
 * This function polls the TX register, waits for the TX register to be empty or for the TX FIFO
 * to have room and writes data to the TX buffer.
 *
 * @param base USART peripheral base address.
 * @param data Start address of the data to write.
 * @param length Size of the data to write.
 * @retval kStatus_USART_Timeout Transmission timed out and was aborted.
 * @retval kStatus_InvalidArgument Invalid argument.
 * @retval kStatus_Success Successfully wrote all data.
 */
status_t USART_WriteBlocking(USART_Type *base, const uint8_t *data, size_t length);

/*!
 * @brief Read RX data register using a blocking method.
 *
 * This function polls the RX register, waits for the RX register to be full or for RX FIFO to
 * have data and read data from the TX register.
 *
 * @param base USART peripheral base address.
 * @param data Start address of the buffer to store the received data.
 * @param length Size of the buffer.
 * @retval kStatus_USART_FramingError Receiver overrun happened while receiving data.
 * @retval kStatus_USART_ParityError Noise error happened while receiving data.
 * @retval kStatus_USART_NoiseError Framing error happened while receiving data.
 * @retval kStatus_USART_RxError Overflow or underflow rxFIFO happened.
 * @retval kStatus_USART_Timeout Transmission timed out and was aborted.
 * @retval kStatus_Success Successfully received all data.
 */
status_t USART_ReadBlocking(USART_Type *base, uint8_t *data, size_t length);

/*! @} */

/*!
 * @name Transactional
 * @{
 */

/*!
 * @brief Initializes the USART handle.
 *
 * This function initializes the USART handle which can be used for other USART
 * transactional APIs. Usually, for a specified USART instance,
 * call this API once to get the initialized handle.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 * @param callback The callback function.
 * @param userData The parameter of the callback function.
 */
status_t USART_TransferCreateHandle(USART_Type *base,
                                    usart_handle_t *handle,
                                    usart_transfer_callback_t callback,
                                    void *userData);

/*!
 * @brief Transmits a buffer of data using the interrupt method.
 *
 * This function sends data using an interrupt method. This is a non-blocking function, which
 * returns directly without waiting for all data to be written to the TX register. When
 * all data is written to the TX register in the IRQ handler, the USART driver calls the callback
 * function and passes the @ref kStatus_USART_TxIdle as status parameter.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 * @param xfer USART transfer structure. See  #usart_transfer_t.
 * @retval kStatus_Success Successfully start the data transmission.
 * @retval kStatus_USART_TxBusy Previous transmission still not finished, data not all written to TX register yet.
 * @retval kStatus_InvalidArgument Invalid argument.
 */
status_t USART_TransferSendNonBlocking(USART_Type *base, usart_handle_t *handle, usart_transfer_t *xfer);

/*!
 * @brief Sets up the RX ring buffer.
 *
 * This function sets up the RX ring buffer to a specific USART handle.
 *
 * When the RX ring buffer is used, data received are stored into the ring buffer even when the
 * user doesn't call the USART_TransferReceiveNonBlocking() API. If there is already data received
 * in the ring buffer, the user can get the received data from the ring buffer directly.
 *
 * @note When using the RX ring buffer, one byte is reserved for internal use. In other
 * words, if @p ringBufferSize is 32, then only 31 bytes are used for saving data.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 * @param ringBuffer Start address of the ring buffer for background receiving. Pass NULL to disable the ring buffer.
 * @param ringBufferSize size of the ring buffer.
 */
void USART_TransferStartRingBuffer(USART_Type *base,
                                   usart_handle_t *handle,
                                   uint8_t *ringBuffer,
                                   size_t ringBufferSize);

/*!
 * @brief Aborts the background transfer and uninstalls the ring buffer.
 *
 * This function aborts the background transfer and uninstalls the ring buffer.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 */
void USART_TransferStopRingBuffer(USART_Type *base, usart_handle_t *handle);

/*!
 * @brief Get the length of received data in RX ring buffer.
 *
 * @param handle USART handle pointer.
 * @return Length of received data in RX ring buffer.
 */
size_t USART_TransferGetRxRingBufferLength(usart_handle_t *handle);

/*!
 * @brief Aborts the interrupt-driven data transmit.
 *
 * This function aborts the interrupt driven data sending. The user can get the remainBtyes to find out
 * how many bytes are still not sent out.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 */
void USART_TransferAbortSend(USART_Type *base, usart_handle_t *handle);

/*!
 * @brief Get the number of bytes that have been sent out to bus.
 *
 * This function gets the number of bytes that have been sent out to bus by interrupt method.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 * @param count Send bytes count.
 * @retval kStatus_NoTransferInProgress No send in progress.
 * @retval kStatus_InvalidArgument Parameter is invalid.
 * @retval kStatus_Success Get successfully through the parameter \p count;
 */
status_t USART_TransferGetSendCount(USART_Type *base, usart_handle_t *handle, uint32_t *count);

/*!
 * @brief Receives a buffer of data using an interrupt method.
 *
 * This function receives data using an interrupt method. This is a non-blocking function, which
 *  returns without waiting for all data to be received.
 * If the RX ring buffer is used and not empty, the data in the ring buffer is copied and
 * the parameter @p receivedBytes shows how many bytes are copied from the ring buffer.
 * After copying, if the data in the ring buffer is not enough to read, the receive
 * request is saved by the USART driver. When the new data arrives, the receive request
 * is serviced first. When all data is received, the USART driver notifies the upper layer
 * through a callback function and passes the status parameter @ref kStatus_USART_RxIdle.
 * For example, the upper layer needs 10 bytes but there are only 5 bytes in the ring buffer.
 * The 5 bytes are copied to the xfer->data and this function returns with the
 * parameter @p receivedBytes set to 5. For the left 5 bytes, newly arrived data is
 * saved from the xfer->data[5]. When 5 bytes are received, the USART driver notifies the upper layer.
 * If the RX ring buffer is not enabled, this function enables the RX and RX interrupt
 * to receive data to the xfer->data. When all data is received, the upper layer is notified.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 * @param xfer USART transfer structure, see #usart_transfer_t.
 * @param receivedBytes Bytes received from the ring buffer directly.
 * @retval kStatus_Success Successfully queue the transfer into transmit queue.
 * @retval kStatus_USART_RxBusy Previous receive request is not finished.
 * @retval kStatus_InvalidArgument Invalid argument.
 */
status_t USART_TransferReceiveNonBlocking(USART_Type *base,
                                          usart_handle_t *handle,
                                          usart_transfer_t *xfer,
                                          size_t *receivedBytes);

/*!
 * @brief Aborts the interrupt-driven data receiving.
 *
 * This function aborts the interrupt-driven data receiving. The user can get the remainBytes to find out
 * how many bytes not received yet.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 */
void USART_TransferAbortReceive(USART_Type *base, usart_handle_t *handle);

/*!
 * @brief Get the number of bytes that have been received.
 *
 * This function gets the number of bytes that have been received.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 * @param count Receive bytes count.
 * @retval kStatus_NoTransferInProgress No receive in progress.
 * @retval kStatus_InvalidArgument Parameter is invalid.
 * @retval kStatus_Success Get successfully through the parameter \p count;
 */
status_t USART_TransferGetReceiveCount(USART_Type *base, usart_handle_t *handle, uint32_t *count);

/*!
 * @brief USART IRQ handle function.
 *
 * This function handles the USART transmit and receive IRQ request.
 *
 * @param base USART peripheral base address.
 * @param handle USART handle pointer.
 */
void USART_TransferHandleIRQ(USART_Type *base, usart_handle_t *handle);

/*! @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* FSL_USART_H_ */
