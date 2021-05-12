/*
 * FreeRTOS+TCP V2.3.2
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file FreeRTOS_TCP_IP.c
 * @brief Module which handles the TCP connections for FreeRTOS+TCP.
 * It depends on  FreeRTOS_TCP_WIN.c, which handles the TCP windowing
 * schemes.
 *
 * Endianness: in this module all ports and IP addresses are stored in
 * host byte-order, except fields in the IP-packets
 */

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "NetworkBufferManagement.h"
#include "FreeRTOS_ARP.h"


/* Just make sure the contents doesn't get compiled if TCP is not enabled. */
#if ipconfigUSE_TCP == 1

/*lint -e750  local macro not referenced [MISRA 2012 Rule 2.5, advisory] */

/*
 * The meaning of the TCP flags:
 */
    #define tcpTCP_FLAG_FIN     ( ( uint8_t ) 0x01U )                   /**< No more data from sender. */
    #define tcpTCP_FLAG_SYN     ( ( uint8_t ) 0x02U )                   /**< Synchronize sequence numbers. */
    #define tcpTCP_FLAG_RST     ( ( uint8_t ) 0x04U )                   /**< Reset the connection. */
    #define tcpTCP_FLAG_PSH     ( ( uint8_t ) 0x08U )                   /**< Push function: please push buffered data to the recv application. */
    #define tcpTCP_FLAG_ACK     ( ( uint8_t ) 0x10U )                   /**< Acknowledgment field is significant. */
    #define tcpTCP_FLAG_URG     ( ( uint8_t ) 0x20U )                   /**< Urgent pointer field is significant. */
    #define tcpTCP_FLAG_ECN     ( ( uint8_t ) 0x40U )                   /**< ECN-Echo. */
    #define tcpTCP_FLAG_CWR     ( ( uint8_t ) 0x80U )                   /**< Congestion Window Reduced. */

    #define tcpTCP_FLAG_CTRL    ( ( uint8_t ) 0x1FU )                   /**< A mask to filter all protocol flags. */


/*
 * A few values of the TCP options:
 */
    #define tcpTCP_OPT_END                        0U /**< End of TCP options list. */
    #define tcpTCP_OPT_NOOP                       1U /**< "No-operation" TCP option. */
    #define tcpTCP_OPT_MSS                        2U /**< Maximum segment size TCP option. */
    #define tcpTCP_OPT_WSOPT                      3U /**< TCP Window Scale Option (3-byte long). */
    #define tcpTCP_OPT_SACK_P                     4U /**< Advertise that SACK is permitted. */
    #define tcpTCP_OPT_SACK_A                     5U /**< SACK option with first/last. */
    #define tcpTCP_OPT_TIMESTAMP                  8U /**< Time-stamp option. */


    #define tcpTCP_OPT_MSS_LEN                    4U /**< Length of TCP MSS option. */
    #define tcpTCP_OPT_WSOPT_LEN                  3U /**< Length of TCP WSOPT option. */

    #define tcpTCP_OPT_TIMESTAMP_LEN              10 /**< fixed length of the time-stamp option. */

    #ifndef ipconfigTCP_ACK_EARLIER_PACKET
        #define ipconfigTCP_ACK_EARLIER_PACKET    1   /**< Acknowledge an earlier packet. */
    #endif


/** @brief
 * The macro tcpNOW_CONNECTED() is use to determine if the connection makes a
 * transition from connected to non-connected and vice versa.
 * tcpNOW_CONNECTED() returns true when the status has one of these values:
 * eESTABLISHED, eFIN_WAIT_1, eFIN_WAIT_2, eCLOSING, eLAST_ACK, eTIME_WAIT
 * Technically the connection status is closed earlier, but the library wants
 * to prevent that the socket will be deleted before the last ACK has been
 * and thus causing a 'RST' packet on either side.
 */
    #define tcpNOW_CONNECTED( status ) \
    ( ( ( ( status ) >= ( BaseType_t ) eESTABLISHED ) && ( ( status ) != ( BaseType_t ) eCLOSE_WAIT ) ) ? 1 : 0 )

/** @brief
 * The highest 4 bits in the TCP offset byte indicate the total length of the
 * TCP header, divided by 4.
 */
    #define tcpVALID_BITS_IN_TCP_OFFSET_BYTE    ( 0xF0U )

/*
 * Acknowledgements to TCP data packets may be delayed as long as more is being expected.
 * A normal delay would be 200ms. Here a much shorter delay of 20 ms is being used to
 * gain performance.
 */
    #define tcpDELAYED_ACK_SHORT_DELAY_MS       ( 2 )   /**< Should not become smaller than 1. */
    #define tcpDELAYED_ACK_LONGER_DELAY_MS      ( 20 )  /**< Longer delay for ACK. */


/** @brief
 * The MSS (Maximum Segment Size) will be taken as large as possible. However, packets with
 * an MSS of 1460 bytes won't be transported through the internet.  The MSS will be reduced
 * to 1400 bytes.
 */
    #define tcpREDUCED_MSS_THROUGH_INTERNET    ( 1400 )

/** @brief
 * When there are no TCP options, the TCP offset equals 20 bytes, which is stored as
 * the number 5 (words) in the higher nibble of the TCP-offset byte.
 */
    #define tcpTCP_OFFSET_LENGTH_BITS          ( 0xf0U )
    #define tcpTCP_OFFSET_STANDARD_LENGTH      ( 0x50U )  /**< Standard TCP packet offset. */


/** @brief
 * Each TCP socket is checked regularly to see if it can send data packets.
 * By default, the maximum number of packets sent during one check is limited to 8.
 * This amount may be further limited by setting the socket's TX window size.
 */
    #if ( !defined( SEND_REPEATED_COUNT ) )
        #define SEND_REPEATED_COUNT    ( 8 )
    #endif /* !defined( SEND_REPEATED_COUNT ) */

/** @brief
 * Define a maximum period of time (ms) to leave a TCP-socket unattended.
 * When a TCP timer expires, retries and keep-alive messages will be checked.
 */
    #ifndef tcpMAXIMUM_TCP_WAKEUP_TIME_MS
        #define tcpMAXIMUM_TCP_WAKEUP_TIME_MS    20000U
    #endif

/* Two macro's that were introduced to work with both IPv4 and IPv6. */
    #define xIPHeaderSize( pxNetworkBuffer )    ( ipSIZE_OF_IPv4_HEADER )  /**< Size of IP Header. */
    #define uxIPHeaderSizeSocket( pxSocket )    ( ipSIZE_OF_IPv4_HEADER )  /**< Size of IP Header socket. */


/*
 * Returns true if the socket must be checked.  Non-active sockets are waiting
 * for user action, either connect() or close().
 */
    static BaseType_t prvTCPSocketIsActive( eIPTCPState_t xStatus );

/*
 * Either sends a SYN or calls prvTCPSendRepeated (for regular messages).
 */
    static int32_t prvTCPSendPacket( FreeRTOS_Socket_t * pxSocket );

/*
 * Try to send a series of messages.
 */
    static int32_t prvTCPSendRepeated( FreeRTOS_Socket_t * pxSocket,
                                       NetworkBufferDescriptor_t ** ppxNetworkBuffer );

/*
 * Return or send a packet to the other party.
 */
    static void prvTCPReturnPacket( FreeRTOS_Socket_t * pxSocket,
                                    NetworkBufferDescriptor_t * pxDescriptor,
                                    uint32_t ulLen,
                                    BaseType_t xReleaseAfterSend );

/*
 * Initialise the data structures which keep track of the TCP windowing system.
 */
    static void prvTCPCreateWindow( FreeRTOS_Socket_t * pxSocket );

/*
 * Let ARP look-up the MAC-address of the peer and initialise the first SYN
 * packet.
 */
    static BaseType_t prvTCPPrepareConnect( FreeRTOS_Socket_t * pxSocket );

    #if ( ipconfigHAS_DEBUG_PRINTF != 0 )

/*
 * For logging and debugging: make a string showing the TCP flags.
 */
        static const char * prvTCPFlagMeaning( UBaseType_t xFlags );
    #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */

/*
 * Parse the TCP option(s) received, if present.
 */
    _static void prvCheckOptions( FreeRTOS_Socket_t * pxSocket,
                                  const NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * Identify and deal with a single TCP header option, advancing the pointer to
 * the header. This function returns pdTRUE or pdFALSE depending on whether the
 * caller should continue to parse more header options or break the loop.
 */
    _static size_t prvSingleStepTCPHeaderOptions( const uint8_t * const pucPtr,
                                                  size_t uxTotalLength,
                                                  FreeRTOS_Socket_t * const pxSocket,
                                                  BaseType_t xHasSYNFlag );

    #if ( ipconfigUSE_TCP_WIN == 1 )

/*
 * Skip past TCP header options when doing Selective ACK, until there are no
 * more options left.
 */
        _static void prvReadSackOption( const uint8_t * const pucPtr,
                                        size_t uxIndex,
                                        FreeRTOS_Socket_t * const pxSocket );
    #endif /* ( ipconfigUSE_TCP_WIN == 1 ) */


/*
 * Set the initial properties in the options fields, like the preferred
 * value of MSS and whether SACK allowed.  Will be transmitted in the state
 * 'eCONNECT_SYN'.
 */
    static UBaseType_t prvSetSynAckOptions( FreeRTOS_Socket_t * pxSocket,
                                            TCPHeader_t * pxTCPHeader );

/*
 * For anti-hang protection and TCP keep-alive messages.  Called in two places:
 * after receiving a packet and after a state change.  The socket's alive timer
 * may be reset.
 */
    static void prvTCPTouchSocket( FreeRTOS_Socket_t * pxSocket );

/*
 * Prepare an outgoing message, if anything has to be sent.
 */
    static int32_t prvTCPPrepareSend( FreeRTOS_Socket_t * pxSocket,
                                      NetworkBufferDescriptor_t ** ppxNetworkBuffer,
                                      UBaseType_t uxOptionsLength );

/*
 * Calculate when this socket needs to be checked to do (re-)transmissions.
 */
    static TickType_t prvTCPNextTimeout( FreeRTOS_Socket_t * pxSocket );

/*
 * The API FreeRTOS_send() adds data to the TX stream.  Add
 * this data to the windowing system to it can be transmitted.
 */
    static void prvTCPAddTxData( FreeRTOS_Socket_t * pxSocket );

/*
 *  Called to handle the closure of a TCP connection.
 */
    static BaseType_t prvTCPHandleFin( FreeRTOS_Socket_t * pxSocket,
                                       const NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * Called from prvTCPHandleState().  Find the TCP payload data and check and
 * return its length.
 */
    static BaseType_t prvCheckRxData( const NetworkBufferDescriptor_t * pxNetworkBuffer,
                                      uint8_t ** ppucRecvData );

/*
 * Called from prvTCPHandleState().  Check if the payload data may be accepted.
 * If so, it will be added to the socket's reception queue.
 */
    static BaseType_t prvStoreRxData( FreeRTOS_Socket_t * pxSocket,
                                      const uint8_t * pucRecvData,
                                      NetworkBufferDescriptor_t * pxNetworkBuffer,
                                      uint32_t ulReceiveLength );

/*
 * Set the TCP options (if any) for the outgoing packet.
 */
    static UBaseType_t prvSetOptions( FreeRTOS_Socket_t * pxSocket,
                                      const NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * Called from prvTCPHandleState() as long as the TCP status is eSYN_RECEIVED to
 * eCONNECT_SYN.
 */
    static BaseType_t prvHandleSynReceived( FreeRTOS_Socket_t * pxSocket,
                                            const NetworkBufferDescriptor_t * pxNetworkBuffer,
                                            uint32_t ulReceiveLength,
                                            UBaseType_t uxOptionsLength );

/*
 * Called from prvTCPHandleState() as long as the TCP status is eESTABLISHED.
 */
    static BaseType_t prvHandleEstablished( FreeRTOS_Socket_t * pxSocket,
                                            NetworkBufferDescriptor_t ** ppxNetworkBuffer,
                                            uint32_t ulReceiveLength,
                                            UBaseType_t uxOptionsLength );

/*
 * Called from prvTCPHandleState().  There is data to be sent.
 * If ipconfigUSE_TCP_WIN is defined, and if only an ACK must be sent, it will
 * be checked if it would better be postponed for efficiency.
 */
    static BaseType_t prvSendData( FreeRTOS_Socket_t * pxSocket,
                                   NetworkBufferDescriptor_t ** ppxNetworkBuffer,
                                   uint32_t ulReceiveLength,
                                   BaseType_t xByteCount );

/*
 * The heart of all: check incoming packet for valid data and acks and do what
 * is necessary in each state.
 */
    static BaseType_t prvTCPHandleState( FreeRTOS_Socket_t * pxSocket,
                                         NetworkBufferDescriptor_t ** ppxNetworkBuffer );

/*
 * Common code for sending a TCP protocol control packet (i.e. no options, no
 * payload, just flags).
 */
    static BaseType_t prvTCPSendSpecialPacketHelper( NetworkBufferDescriptor_t * pxNetworkBuffer,
                                                     uint8_t ucTCPFlags );

/*
 * A "challenge ACK" is as per https://tools.ietf.org/html/rfc5961#section-3.2,
 * case #3. In summary, an RST was received with a sequence number that is
 * unexpected but still within the window.
 */
    static BaseType_t prvTCPSendChallengeAck( NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * Reply to a peer with the RST flag on, in case a packet can not be handled.
 */
    static BaseType_t prvTCPSendReset( NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * Set the initial value for MSS (Maximum Segment Size) to be used.
 */
    static void prvSocketSetMSS( FreeRTOS_Socket_t * pxSocket );

/*
 * Return either a newly created socket, or the current socket in a connected
 * state (depends on the 'bReuseSocket' flag).
 */
    static FreeRTOS_Socket_t * prvHandleListen( FreeRTOS_Socket_t * pxSocket,
                                                NetworkBufferDescriptor_t * pxNetworkBuffer );

/*
 * After a listening socket receives a new connection, it may duplicate itself.
 * The copying takes place in prvTCPSocketCopy.
 */
    static BaseType_t prvTCPSocketCopy( FreeRTOS_Socket_t * pxNewSocket,
                                        FreeRTOS_Socket_t * pxSocket );

/*
 * prvTCPStatusAgeCheck() will see if the socket has been in a non-connected
 * state for too long.  If so, the socket will be closed, and -1 will be
 * returned.
 */
    #if ( ipconfigTCP_HANG_PROTECTION == 1 )
        static BaseType_t prvTCPStatusAgeCheck( FreeRTOS_Socket_t * pxSocket );
    #endif

    static NetworkBufferDescriptor_t * prvTCPBufferResize( const FreeRTOS_Socket_t * pxSocket,
                                                           NetworkBufferDescriptor_t * pxNetworkBuffer,
                                                           int32_t lDataLen,
                                                           UBaseType_t uxOptionsLength );

    #if ( ipconfigUSE_TCP_WIN != 0 )
        static uint8_t prvWinScaleFactor( const FreeRTOS_Socket_t * pxSocket );
    #endif

/*-----------------------------------------------------------*/

/**
 * @brief Check whether the socket is active or not.
 *
 * @param[in] xStatus: The status of the socket.
 *
 * @return pdTRUE if the socket must be checked. Non-active sockets
 *         are waiting for user action, either connect() or close().
 */
    static BaseType_t prvTCPSocketIsActive( eIPTCPState_t xStatus )
    {
        BaseType_t xResult;

        switch( xStatus )
        {
            case eCLOSED:
            case eCLOSE_WAIT:
            case eFIN_WAIT_2:
            case eCLOSING:
            case eTIME_WAIT:
                xResult = pdFALSE;
                break;

            case eTCP_LISTEN:
            case eCONNECT_SYN:
            case eSYN_FIRST:
            case eSYN_RECEIVED:
            case eESTABLISHED:
            case eFIN_WAIT_1:
            case eLAST_ACK:
            default:
                xResult = pdTRUE;
                break;
        }

        return xResult;
    }
    /*-----------------------------------------------------------*/

    #if ( ipconfigTCP_HANG_PROTECTION == 1 )

/**
 * @brief Some of the TCP states may only last a certain amount of time.
 *        This function checks if the socket is 'hanging', i.e. staying
 *        too long in the same state.
 *
 * @param[in] The socket to be checked.
 *
 * @return pdFALSE if no checks are needed, pdTRUE if checks were done, or negative
 *         in case the socket has reached a critical time-out. The socket will go to
 *         the eCLOSE_WAIT state.
 */
        static BaseType_t prvTCPStatusAgeCheck( FreeRTOS_Socket_t * pxSocket )
        {
            BaseType_t xResult;
            eIPTCPState_t eState = ipNUMERIC_CAST( eIPTCPState_t, pxSocket->u.xTCP.ucTCPState );

            switch( eState )
            {
                case eESTABLISHED:

                    /* If the 'ipconfigTCP_KEEP_ALIVE' option is enabled, sockets in
                     *  state ESTABLISHED can be protected using keep-alive messages. */
                    xResult = pdFALSE;
                    break;

                case eCLOSED:
                case eTCP_LISTEN:
                case eCLOSE_WAIT:
                    /* These 3 states may last for ever, up to the owner. */
                    xResult = pdFALSE;
                    break;

                case eCONNECT_SYN:
                case eSYN_FIRST:
                case eSYN_RECEIVED:
                case eFIN_WAIT_1:
                case eFIN_WAIT_2:
                case eCLOSING:
                case eLAST_ACK:
                case eTIME_WAIT:
                default:

                    /* All other (non-connected) states will get anti-hanging
                     * protection. */
                    xResult = pdTRUE;
                    break;
            }

            if( xResult != pdFALSE )
            {
                /* How much time has past since the last active moment which is
                 * defined as A) a state change or B) a packet has arrived. */
                TickType_t xAge = xTaskGetTickCount() - pxSocket->u.xTCP.xLastActTime;

                /* ipconfigTCP_HANG_PROTECTION_TIME is in units of seconds. */
                if( xAge > ( ( TickType_t ) ipconfigTCP_HANG_PROTECTION_TIME * ( TickType_t ) configTICK_RATE_HZ ) )
                {
                    #if ( ipconfigHAS_DEBUG_PRINTF == 1 )
                        {
                            FreeRTOS_debug_printf( ( "Inactive socket closed: port %u rem %lxip:%u status %s\n",
                                                     pxSocket->usLocalPort,
                                                     pxSocket->u.xTCP.ulRemoteIP,
                                                     pxSocket->u.xTCP.usRemotePort,
                                                     FreeRTOS_GetTCPStateName( ( UBaseType_t ) pxSocket->u.xTCP.ucTCPState ) ) );
                        }
                    #endif /* ipconfigHAS_DEBUG_PRINTF */

                    /* Move to eCLOSE_WAIT, user may close the socket. */
                    vTCPStateChange( pxSocket, eCLOSE_WAIT );

                    /* When 'bPassQueued' true, this socket is an orphan until it
                     * gets connected. */
                    if( pxSocket->u.xTCP.bits.bPassQueued != pdFALSE_UNSIGNED )
                    {
                        /* vTCPStateChange() has called FreeRTOS_closesocket()
                         * in case the socket is not yet owned by the application.
                         * Return a negative value to inform the caller that
                         * the socket will be closed in the next cycle. */
                        xResult = -1;
                    }
                }
            }

            return xResult;
        }
        /*-----------------------------------------------------------*/

    #endif /* if ( ipconfigTCP_HANG_PROTECTION == 1 ) */

/**
 * @brief As soon as a TCP socket timer expires, this function will be called
 *       (from xTCPTimerCheck). It can send a delayed ACK or new data.
 *
 * @param[in] pxSocket: socket to be checked.
 *
 * @return 0 on success, a negative error code on failure. A negative value will be
 *         returned in case the hang-protection has put the socket in a wait-close state.
 *
 * @note Sequence of calling (normally) :
 *   IP-Task:
 *      xTCPTimerCheck()                // Check all sockets ( declared in FreeRTOS_Sockets.c )
 *      xTCPSocketCheck()               // Either send a delayed ACK or call prvTCPSendPacket()
 *      prvTCPSendPacket()              // Either send a SYN or call prvTCPSendRepeated ( regular messages )
 *      prvTCPSendRepeated()            // Send at most 8 messages on a row
 *          prvTCPReturnPacket()        // Prepare for returning
 *          xNetworkInterfaceOutput()   // Sends data to the NIC ( declared in portable/NetworkInterface/xxx )
 */
    BaseType_t xTCPSocketCheck( FreeRTOS_Socket_t * pxSocket )
    {
        BaseType_t xResult = 0;
        BaseType_t xReady = pdFALSE;

        if( ( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED ) && ( pxSocket->u.xTCP.txStream != NULL ) )
        {
            /* The API FreeRTOS_send() might have added data to the TX stream.  Add
             * this data to the windowing system so it can be transmitted. */
            prvTCPAddTxData( pxSocket );
        }

        #if ( ipconfigUSE_TCP_WIN == 1 )
            {
                if( pxSocket->u.xTCP.pxAckMessage != NULL )
                {
                    /* The first task of this regular socket check is to send-out delayed
                     * ACK's. */
                    if( pxSocket->u.xTCP.bits.bUserShutdown == pdFALSE_UNSIGNED )
                    {
                        /* Earlier data was received but not yet acknowledged.  This
                         * function is called when the TCP timer for the socket expires, the
                         * ACK may be sent now. */
                        if( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eCLOSED )
                        {
                            if( ( xTCPWindowLoggingLevel > 1 ) && ipconfigTCP_MAY_LOG_PORT( pxSocket->usLocalPort ) )
                            {
                                FreeRTOS_debug_printf( ( "Send[%u->%u] del ACK %lu SEQ %lu (len %u)\n",
                                                         pxSocket->usLocalPort,
                                                         pxSocket->u.xTCP.usRemotePort,
                                                         pxSocket->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber - pxSocket->u.xTCP.xTCPWindow.rx.ulFirstSequenceNumber,
                                                         pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber - pxSocket->u.xTCP.xTCPWindow.tx.ulFirstSequenceNumber,
                                                         ( unsigned ) ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_TCP_HEADER ) );
                            }

                            prvTCPReturnPacket( pxSocket, pxSocket->u.xTCP.pxAckMessage, ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_TCP_HEADER, ipconfigZERO_COPY_TX_DRIVER );

                            #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
                                {
                                    /* The ownership has been passed to the SEND routine,
                                     * clear the pointer to it. */
                                    pxSocket->u.xTCP.pxAckMessage = NULL;
                                }
                            #endif /* ipconfigZERO_COPY_TX_DRIVER */
                        }

                        if( prvTCPNextTimeout( pxSocket ) > 1U )
                        {
                            /* Tell the code below that this function is ready. */
                            xReady = pdTRUE;
                        }
                    }
                    else
                    {
                        /* The user wants to perform an active shutdown(), skip sending
                         * the delayed ACK.  The function prvTCPSendPacket() will send the
                         * FIN along with the ACK's. */
                    }

                    if( pxSocket->u.xTCP.pxAckMessage != NULL )
                    {
                        vReleaseNetworkBufferAndDescriptor( pxSocket->u.xTCP.pxAckMessage );
                        pxSocket->u.xTCP.pxAckMessage = NULL;
                    }
                }
            }
        #endif /* ipconfigUSE_TCP_WIN */

        if( xReady == pdFALSE )
        {
            /* The second task of this regular socket check is sending out data. */
            if( ( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED ) ||
                ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCONNECT_SYN ) )
            {
                ( void ) prvTCPSendPacket( pxSocket );
            }

            /* Set the time-out for the next wakeup for this socket. */
            ( void ) prvTCPNextTimeout( pxSocket );

            #if ( ipconfigTCP_HANG_PROTECTION == 1 )
                {
                    /* In all (non-connected) states in which keep-alive messages can not be sent
                     * the anti-hang protocol will close sockets that are 'hanging'. */
                    xResult = prvTCPStatusAgeCheck( pxSocket );
                }
            #endif
        }

        return xResult;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvTCPSendPacket() will be called when the socket time-out has been reached.
 *
 * @param[in] pxSocket: The socket owning the connection.
 *
 * @return Number of bytes to be sent.
 *
 * @note It is only called by xTCPSocketCheck().
 */
    static int32_t prvTCPSendPacket( FreeRTOS_Socket_t * pxSocket )
    {
        int32_t lResult = 0;
        UBaseType_t uxOptionsLength, uxIntermediateResult = 0;
        NetworkBufferDescriptor_t * pxNetworkBuffer;

        if( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eCONNECT_SYN )
        {
            /* The connection is in a state other than SYN. */
            pxNetworkBuffer = NULL;

            /* prvTCPSendRepeated() will only create a network buffer if necessary,
             * i.e. when data must be sent to the peer. */
            lResult = prvTCPSendRepeated( pxSocket, &pxNetworkBuffer );

            if( pxNetworkBuffer != NULL )
            {
                vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
            }
        }
        else
        {
            if( pxSocket->u.xTCP.ucRepCount >= 3U )
            {
                /* The connection is in the SYN status. The packet will be repeated
                 * to most 3 times.  When there is no response, the socket get the
                 * status 'eCLOSE_WAIT'. */
                FreeRTOS_debug_printf( ( "Connect: giving up %lxip:%u\n",
                                         pxSocket->u.xTCP.ulRemoteIP,       /* IP address of remote machine. */
                                         pxSocket->u.xTCP.usRemotePort ) ); /* Port on remote machine. */
                vTCPStateChange( pxSocket, eCLOSE_WAIT );
            }
            else if( ( pxSocket->u.xTCP.bits.bConnPrepared != pdFALSE_UNSIGNED ) || ( prvTCPPrepareConnect( pxSocket ) == pdTRUE ) )
            {
                ProtocolHeaders_t * pxProtocolHeaders;
                const UBaseType_t uxHeaderSize = ipSIZE_OF_IPv4_HEADER;

                /* Or else, if the connection has been prepared, or can be prepared
                 * now, proceed to send the packet with the SYN flag.
                 * prvTCPPrepareConnect() prepares 'xPacket' and returns pdTRUE if
                 * the Ethernet address of the peer or the gateway is found. */
                pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t, &( pxSocket->u.xTCP.xPacket.u.ucLastPacket[ ipSIZE_OF_ETH_HEADER + uxHeaderSize ] ) );

                /* About to send a SYN packet.  Call prvSetSynAckOptions() to set
                 * the proper options: The size of MSS and whether SACK's are
                 * allowed. */
                uxOptionsLength = prvSetSynAckOptions( pxSocket, &( pxProtocolHeaders->xTCPHeader ) );

                /* Return the number of bytes to be sent. */
                uxIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength;
                lResult = ( int32_t ) uxIntermediateResult;

                /* Set the TCP offset field:  ipSIZE_OF_TCP_HEADER equals 20 and
                 * uxOptionsLength is always a multiple of 4.  The complete expression
                 * would be:
                 * ucTCPOffset = ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) / 4 ) << 4 */
                pxProtocolHeaders->xTCPHeader.ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );

                /* Repeat Count is used for a connecting socket, to limit the number
                 * of tries. */
                pxSocket->u.xTCP.ucRepCount++;

                /* Send the SYN message to make a connection.  The messages is
                 * stored in the socket field 'xPacket'.  It will be wrapped in a
                 * pseudo network buffer descriptor before it will be sent. */
                prvTCPReturnPacket( pxSocket, NULL, ( uint32_t ) lResult, pdFALSE );
            }
            else
            {
                /* Nothing to do. */
            }
        }

        /* Return the total number of bytes sent. */
        return lResult;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvTCPSendRepeated will try to send a series of messages, as
 *        long as there is data to be sent and as long as the transmit
 *        window isn't full.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in,out] ppxNetworkBuffer: Pointer to pointer to the network buffer.
 *
 * @return Total number of bytes sent.
 */
    static int32_t prvTCPSendRepeated( FreeRTOS_Socket_t * pxSocket,
                                       NetworkBufferDescriptor_t ** ppxNetworkBuffer )
    {
        UBaseType_t uxIndex;
        int32_t lResult = 0;
        UBaseType_t uxOptionsLength = 0U;
        int32_t xSendLength;

        for( uxIndex = 0U; uxIndex < ( UBaseType_t ) SEND_REPEATED_COUNT; uxIndex++ )
        {
            /* prvTCPPrepareSend() might allocate a network buffer if there is data
             * to be sent. */
            xSendLength = prvTCPPrepareSend( pxSocket, ppxNetworkBuffer, uxOptionsLength );

            if( xSendLength <= 0 )
            {
                break;
            }

            /* And return the packet to the peer. */
            prvTCPReturnPacket( pxSocket, *ppxNetworkBuffer, ( uint32_t ) xSendLength, ipconfigZERO_COPY_TX_DRIVER );

            #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
                {
                    *ppxNetworkBuffer = NULL;
                }
            #endif /* ipconfigZERO_COPY_TX_DRIVER */

            lResult += xSendLength;
        }

        /* Return the total number of bytes sent. */
        return lResult;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief  Return (or send) a packet to the peer. The data is stored in pxBuffer,
 *         which may either point to a real network buffer or to a TCP socket field
 *         called 'xTCP.xPacket'. A temporary xNetworkBuffer will be used to pass
 *         the data to the NIC.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in] pxDescriptor: The network buffer descriptor carrying the packet.
 * @param[in] ulLen: Length of the packet being sent.
 * @param[in] xReleaseAfterSend: pdTRUE if the ownership of the descriptor is
 *                               transferred to the network interface.
 */
    static void prvTCPReturnPacket( FreeRTOS_Socket_t * pxSocket,
                                    NetworkBufferDescriptor_t * pxDescriptor,
                                    uint32_t ulLen,
                                    BaseType_t xReleaseAfterSend )
    {
        TCPPacket_t * pxTCPPacket;
        IPHeader_t * pxIPHeader;
        BaseType_t xDoRelease = xReleaseAfterSend;
        EthernetHeader_t * pxEthernetHeader;
        uint32_t ulFrontSpace, ulSpace, ulSourceAddress, ulWinSize;
        const TCPWindow_t * pxTCPWindow;
        NetworkBufferDescriptor_t * pxNetworkBuffer = pxDescriptor;
        NetworkBufferDescriptor_t xTempBuffer;
        /* memcpy() helper variables for MISRA Rule 21.15 compliance*/
        const void * pvCopySource;
        void * pvCopyDest;


        /* For sending, a pseudo network buffer will be used, as explained above. */

        if( pxNetworkBuffer == NULL )
        {
            pxNetworkBuffer = &xTempBuffer;

            #if ( ipconfigUSE_LINKED_RX_MESSAGES != 0 )
                {
                    pxNetworkBuffer->pxNextBuffer = NULL;
                }
            #endif
            pxNetworkBuffer->pucEthernetBuffer = pxSocket->u.xTCP.xPacket.u.ucLastPacket;
            pxNetworkBuffer->xDataLength = sizeof( pxSocket->u.xTCP.xPacket.u.ucLastPacket );
            xDoRelease = pdFALSE;
        }

        #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
            {
                if( xDoRelease == pdFALSE )
                {
                    pxNetworkBuffer = pxDuplicateNetworkBufferWithDescriptor( pxNetworkBuffer, ( size_t ) pxNetworkBuffer->xDataLength );

                    if( pxNetworkBuffer == NULL )
                    {
                        FreeRTOS_debug_printf( ( "prvTCPReturnPacket: duplicate failed\n" ) );
                    }

                    xDoRelease = pdTRUE;
                }
            }
        #endif /* ipconfigZERO_COPY_TX_DRIVER */

        #ifndef __COVERITY__
            if( pxNetworkBuffer != NULL )
        #endif
        {
            /* Map the ethernet buffer onto a TCPPacket_t struct for easy access to the fields. */
            pxTCPPacket = ipCAST_PTR_TO_TYPE_PTR( TCPPacket_t, pxNetworkBuffer->pucEthernetBuffer );
            pxIPHeader = &pxTCPPacket->xIPHeader;
            pxEthernetHeader = &pxTCPPacket->xEthernetHeader;

            /* Fill the packet, using hton translations. */
            if( pxSocket != NULL )
            {
                /* Calculate the space in the RX buffer in order to advertise the
                 * size of this socket's reception window. */
                pxTCPWindow = &( pxSocket->u.xTCP.xTCPWindow );

                if( pxSocket->u.xTCP.rxStream != NULL )
                {
                    /* An RX stream was created already, see how much space is
                     * available. */
                    ulFrontSpace = ( uint32_t ) uxStreamBufferFrontSpace( pxSocket->u.xTCP.rxStream );
                }
                else
                {
                    /* No RX stream has been created, the full stream size is
                     * available. */
                    ulFrontSpace = ( uint32_t ) pxSocket->u.xTCP.uxRxStreamSize;
                }

                /* Take the minimum of the RX buffer space and the RX window size. */
                ulSpace = FreeRTOS_min_uint32( pxTCPWindow->xSize.ulRxWindowLength, ulFrontSpace );

                if( ( pxSocket->u.xTCP.bits.bLowWater != pdFALSE_UNSIGNED ) || ( pxSocket->u.xTCP.bits.bRxStopped != pdFALSE_UNSIGNED ) )
                {
                    /* The low-water mark was reached, meaning there was little
                     * space left.  The socket will wait until the application has read
                     * or flushed the incoming data, and 'zero-window' will be
                     * advertised. */
                    ulSpace = 0U;
                }

                /* If possible, advertise an RX window size of at least 1 MSS, otherwise
                 * the peer might start 'zero window probing', i.e. sending small packets
                 * (1, 2, 4, 8... bytes). */
                if( ( ulSpace < pxSocket->u.xTCP.usCurMSS ) && ( ulFrontSpace >= pxSocket->u.xTCP.usCurMSS ) )
                {
                    ulSpace = pxSocket->u.xTCP.usCurMSS;
                }

                /* Avoid overflow of the 16-bit win field. */
                #if ( ipconfigUSE_TCP_WIN != 0 )
                    {
                        ulWinSize = ( ulSpace >> pxSocket->u.xTCP.ucMyWinScaleFactor );
                    }
                #else
                    {
                        ulWinSize = ulSpace;
                    }
                #endif

                if( ulWinSize > 0xfffcUL )
                {
                    ulWinSize = 0xfffcUL;
                }

                pxTCPPacket->xTCPHeader.usWindow = FreeRTOS_htons( ( uint16_t ) ulWinSize );

                /* The new window size has been advertised, switch off the flag. */
                pxSocket->u.xTCP.bits.bWinChange = pdFALSE_UNSIGNED;

                /* Later on, when deciding to delay an ACK, a precise estimate is needed
                 * of the free RX space.  At this moment, 'ulHighestRxAllowed' would be the
                 * highest sequence number minus 1 that the socket will accept. */
                pxSocket->u.xTCP.ulHighestRxAllowed = pxTCPWindow->rx.ulCurrentSequenceNumber + ulSpace;

                #if ( ipconfigTCP_KEEP_ALIVE == 1 )
                    if( pxSocket->u.xTCP.bits.bSendKeepAlive != pdFALSE_UNSIGNED )
                    {
                        /* Sending a keep-alive packet, send the current sequence number
                         * minus 1, which will be recognised as a keep-alive packet and
                         * responded to by acknowledging the last byte. */
                        pxSocket->u.xTCP.bits.bSendKeepAlive = pdFALSE_UNSIGNED;
                        pxSocket->u.xTCP.bits.bWaitKeepAlive = pdTRUE_UNSIGNED;

                        pxTCPPacket->xTCPHeader.ulSequenceNumber = pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber - 1UL;
                        pxTCPPacket->xTCPHeader.ulSequenceNumber = FreeRTOS_htonl( pxTCPPacket->xTCPHeader.ulSequenceNumber );
                    }
                    else
                #endif /* if ( ipconfigTCP_KEEP_ALIVE == 1 ) */
                {
                    pxTCPPacket->xTCPHeader.ulSequenceNumber = FreeRTOS_htonl( pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber );

                    if( ( pxTCPPacket->xTCPHeader.ucTCPFlags & ( uint8_t ) tcpTCP_FLAG_FIN ) != 0U )
                    {
                        /* Suppress FIN in case this packet carries earlier data to be
                         * retransmitted. */
                        uint32_t ulDataLen = ( uint32_t ) ( ulLen - ( ipSIZE_OF_TCP_HEADER + ipSIZE_OF_IPv4_HEADER ) );

                        if( ( pxTCPWindow->ulOurSequenceNumber + ulDataLen ) != pxTCPWindow->tx.ulFINSequenceNumber )
                        {
                            pxTCPPacket->xTCPHeader.ucTCPFlags &= ( ( uint8_t ) ~tcpTCP_FLAG_FIN );
                            FreeRTOS_debug_printf( ( "Suppress FIN for %lu + %lu < %lu\n",
                                                     pxTCPWindow->ulOurSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber,
                                                     ulDataLen,
                                                     pxTCPWindow->tx.ulFINSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber ) );
                        }
                    }
                }

                /* Tell which sequence number is expected next time */
                pxTCPPacket->xTCPHeader.ulAckNr = FreeRTOS_htonl( pxTCPWindow->rx.ulCurrentSequenceNumber );
            }
            else
            {
                /* Sending data without a socket, probably replying with a RST flag
                 * Just swap the two sequence numbers. */
                vFlip_32( pxTCPPacket->xTCPHeader.ulSequenceNumber, pxTCPPacket->xTCPHeader.ulAckNr );
            }

            pxIPHeader->ucTimeToLive = ( uint8_t ) ipconfigTCP_TIME_TO_LIVE;
            pxIPHeader->usLength = FreeRTOS_htons( ulLen );

            if( ( pxSocket == NULL ) || ( *ipLOCAL_IP_ADDRESS_POINTER == 0UL ) )
            {
                /* When pxSocket is NULL, this function is called by prvTCPSendReset()
                * and the IP-addresses must be swapped.
                * Also swap the IP-addresses in case the IP-tack doesn't have an
                * IP-address yet, i.e. when ( *ipLOCAL_IP_ADDRESS_POINTER == 0UL ). */
                ulSourceAddress = pxIPHeader->ulDestinationIPAddress;
            }
            else
            {
                ulSourceAddress = *ipLOCAL_IP_ADDRESS_POINTER;
            }

            pxIPHeader->ulDestinationIPAddress = pxIPHeader->ulSourceIPAddress;
            pxIPHeader->ulSourceIPAddress = ulSourceAddress;
            vFlip_16( pxTCPPacket->xTCPHeader.usSourcePort, pxTCPPacket->xTCPHeader.usDestinationPort );

            /* Just an increasing number. */
            pxIPHeader->usIdentification = FreeRTOS_htons( usPacketIdentifier );
            usPacketIdentifier++;
            pxIPHeader->usFragmentOffset = 0U;

            /* Important: tell NIC driver how many bytes must be sent. */
            pxNetworkBuffer->xDataLength = ulLen + ipSIZE_OF_ETH_HEADER;

            #if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
                {
                    /* calculate the IP header checksum, in case the driver won't do that. */
                    pxIPHeader->usHeaderChecksum = 0x00U;
                    pxIPHeader->usHeaderChecksum = usGenerateChecksum( 0U, ( uint8_t * ) &( pxIPHeader->ucVersionHeaderLength ), ipSIZE_OF_IPv4_HEADER );
                    pxIPHeader->usHeaderChecksum = ~FreeRTOS_htons( pxIPHeader->usHeaderChecksum );

                    /* calculate the TCP checksum for an outgoing packet. */
                    ( void ) usGenerateProtocolChecksum( ( uint8_t * ) pxTCPPacket, pxNetworkBuffer->xDataLength, pdTRUE );

                    /* A calculated checksum of 0 must be inverted as 0 means the checksum
                     * is disabled. */
                    if( pxTCPPacket->xTCPHeader.usChecksum == 0U )
                    {
                        pxTCPPacket->xTCPHeader.usChecksum = 0xffffU;
                    }
                }
            #endif /* if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 ) */

            #if ( ipconfigUSE_LINKED_RX_MESSAGES != 0 )
                {
                    pxNetworkBuffer->pxNextBuffer = NULL;
                }
            #endif

            /* Fill in the destination MAC addresses. */
            ( void ) memcpy( ( void * ) ( &( pxEthernetHeader->xDestinationAddress ) ),
                             ( const void * ) ( &( pxEthernetHeader->xSourceAddress ) ),
                             sizeof( pxEthernetHeader->xDestinationAddress ) );

            /*
             * Use helper variables for memcpy() to remain
             * compliant with MISRA Rule 21.15.  These should be
             * optimized away.
             */
            /* The source MAC addresses is fixed to 'ipLOCAL_MAC_ADDRESS'. */
            pvCopySource = ipLOCAL_MAC_ADDRESS;
            pvCopyDest = &pxEthernetHeader->xSourceAddress;
            ( void ) memcpy( pvCopyDest, pvCopySource, ( size_t ) ipMAC_ADDRESS_LENGTH_BYTES );

            #if defined( ipconfigETHERNET_MINIMUM_PACKET_BYTES )
                {
                    if( pxNetworkBuffer->xDataLength < ( size_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES )
                    {
                        BaseType_t xIndex;

                        for( xIndex = ( BaseType_t ) pxNetworkBuffer->xDataLength; xIndex < ( BaseType_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES; xIndex++ )
                        {
                            pxNetworkBuffer->pucEthernetBuffer[ xIndex ] = 0U;
                        }

                        pxNetworkBuffer->xDataLength = ( size_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES;
                    }
                }
            #endif /* if defined( ipconfigETHERNET_MINIMUM_PACKET_BYTES ) */

            /* Send! */
            iptraceNETWORK_INTERFACE_OUTPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );
            ( void ) xNetworkInterfaceOutput( pxNetworkBuffer, xDoRelease );

            if( xDoRelease == pdFALSE )
            {
                /* Swap-back some fields, as pxBuffer probably points to a socket field
                 * containing the packet header. */
                vFlip_16( pxTCPPacket->xTCPHeader.usSourcePort, pxTCPPacket->xTCPHeader.usDestinationPort );
                pxTCPPacket->xIPHeader.ulSourceIPAddress = pxTCPPacket->xIPHeader.ulDestinationIPAddress;
                ( void ) memcpy( ( void * ) ( pxEthernetHeader->xSourceAddress.ucBytes ), ( const void * ) ( pxEthernetHeader->xDestinationAddress.ucBytes ), ( size_t ) ipMAC_ADDRESS_LENGTH_BYTES );
            }
            else
            {
                /* Nothing to do: the buffer has been passed to DMA and will be released after use */
            }
        } /* if( pxNetworkBuffer != NULL ) */
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Create the TCP window for the given socket.
 *
 * @param[in] pxSocket: The socket for which the window is being created.
 *
 * @note The SYN event is very important: the sequence numbers, which have a kind of
 *       random starting value, are being synchronized. The sliding window manager
 *       (in FreeRTOS_TCP_WIN.c) needs to know them, along with the Maximum Segment
 *       Size (MSS).
 */
    static void prvTCPCreateWindow( FreeRTOS_Socket_t * pxSocket )
    {
        if( xTCPWindowLoggingLevel != 0 )
        {
            FreeRTOS_debug_printf( ( "Limits (using): TCP Win size %u Water %u <= %u <= %u\n",
                                     ( unsigned ) pxSocket->u.xTCP.uxRxWinSize * ipconfigTCP_MSS,
                                     ( unsigned ) pxSocket->u.xTCP.uxLittleSpace,
                                     ( unsigned ) pxSocket->u.xTCP.uxEnoughSpace,
                                     ( unsigned ) pxSocket->u.xTCP.uxRxStreamSize ) );
        }

        vTCPWindowCreate(
            &pxSocket->u.xTCP.xTCPWindow,
            ipconfigTCP_MSS * pxSocket->u.xTCP.uxRxWinSize,
            ipconfigTCP_MSS * pxSocket->u.xTCP.uxTxWinSize,
            pxSocket->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber,
            pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber,
            ( uint32_t ) pxSocket->u.xTCP.usInitMSS );
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Let ARP look-up the MAC-address of the peer and initialise the first SYN
 *        packet.
 *
 * @param[in] pxSocket: The socket owning the TCP connection. The first packet shall
 *               be created in this socket.
 *
 * @return pdTRUE: if the packet was successfully created and the first SYN can be sent.
 *         Else pdFALSE.
 *
 * @note Connecting sockets have a special state: eCONNECT_SYN. In this phase,
 *       the Ethernet address of the target will be found using ARP. In case the
 *       target IP address is not within the netmask, the hardware address of the
 *       gateway will be used.
 */
    static BaseType_t prvTCPPrepareConnect( FreeRTOS_Socket_t * pxSocket )
    {
        TCPPacket_t * pxTCPPacket;
        IPHeader_t * pxIPHeader;
        eARPLookupResult_t eReturned;
        uint32_t ulRemoteIP;
        MACAddress_t xEthAddress;
        BaseType_t xReturn = pdTRUE;
        uint32_t ulInitialSequenceNumber = 0;

        #if ( ipconfigHAS_PRINTF != 0 )
            {
                /* Only necessary for nicer logging. */
                ( void ) memset( xEthAddress.ucBytes, 0, sizeof( xEthAddress.ucBytes ) );
            }
        #endif /* ipconfigHAS_PRINTF != 0 */

        ulRemoteIP = FreeRTOS_htonl( pxSocket->u.xTCP.ulRemoteIP );

        /* Determine the ARP cache status for the requested IP address. */
        eReturned = eARPGetCacheEntry( &( ulRemoteIP ), &( xEthAddress ) );

        switch( eReturned )
        {
            case eARPCacheHit:    /* An ARP table lookup found a valid entry. */
                break;            /* We can now prepare the SYN packet. */

            case eARPCacheMiss:   /* An ARP table lookup did not find a valid entry. */
            case eCantSendPacket: /* There is no IP address, or an ARP is still in progress. */
            default:
                /* Count the number of times it could not find the ARP address. */
                pxSocket->u.xTCP.ucRepCount++;

                FreeRTOS_debug_printf( ( "ARP for %lxip (using %lxip): rc=%d %02X:%02X:%02X %02X:%02X:%02X\n",
                                         pxSocket->u.xTCP.ulRemoteIP,
                                         FreeRTOS_htonl( ulRemoteIP ),
                                         eReturned,
                                         xEthAddress.ucBytes[ 0 ],
                                         xEthAddress.ucBytes[ 1 ],
                                         xEthAddress.ucBytes[ 2 ],
                                         xEthAddress.ucBytes[ 3 ],
                                         xEthAddress.ucBytes[ 4 ],
                                         xEthAddress.ucBytes[ 5 ] ) );

                /* And issue a (new) ARP request */
                FreeRTOS_OutputARPRequest( ulRemoteIP );
                xReturn = pdFALSE;
                break;
        }

        if( xReturn != pdFALSE )
        {
            /* Get a difficult-to-predict initial sequence number for this 4-tuple. */
            ulInitialSequenceNumber = ulApplicationGetNextSequenceNumber( *ipLOCAL_IP_ADDRESS_POINTER,
                                                                          pxSocket->usLocalPort,
                                                                          pxSocket->u.xTCP.ulRemoteIP,
                                                                          pxSocket->u.xTCP.usRemotePort );

            /* Check for a random number generation error. */
            if( ulInitialSequenceNumber == 0UL )
            {
                xReturn = pdFALSE;
            }
        }

        if( xReturn != pdFALSE )
        {
            uint16_t usLength;

            /* The MAC-address of the peer (or gateway) has been found,
             * now prepare the initial TCP packet and some fields in the socket. Map
             * the buffer onto the TCPPacket_t struct to easily access it's field. */
            pxTCPPacket = ipCAST_PTR_TO_TYPE_PTR( TCPPacket_t, pxSocket->u.xTCP.xPacket.u.ucLastPacket );
            pxIPHeader = &pxTCPPacket->xIPHeader;

            /* reset the retry counter to zero. */
            pxSocket->u.xTCP.ucRepCount = 0U;

            /* And remember that the connect/SYN data are prepared. */
            pxSocket->u.xTCP.bits.bConnPrepared = pdTRUE_UNSIGNED;

            /* Now that the Ethernet address is known, the initial packet can be
             * prepared. */
            ( void ) memset( pxSocket->u.xTCP.xPacket.u.ucLastPacket, 0, sizeof( pxSocket->u.xTCP.xPacket.u.ucLastPacket ) );

            /* Write the Ethernet address in Source, because it will be swapped by
             * prvTCPReturnPacket(). */
            ( void ) memcpy( ( void * ) ( &pxTCPPacket->xEthernetHeader.xSourceAddress ), ( const void * ) ( &xEthAddress ), sizeof( xEthAddress ) );

            /* 'ipIPv4_FRAME_TYPE' is already in network-byte-order. */
            pxTCPPacket->xEthernetHeader.usFrameType = ipIPv4_FRAME_TYPE;

            pxIPHeader->ucVersionHeaderLength = 0x45U;
            usLength = ( uint16_t ) ( sizeof( TCPPacket_t ) - sizeof( pxTCPPacket->xEthernetHeader ) );
            pxIPHeader->usLength = FreeRTOS_htons( usLength );
            pxIPHeader->ucTimeToLive = ( uint8_t ) ipconfigTCP_TIME_TO_LIVE;

            pxIPHeader->ucProtocol = ( uint8_t ) ipPROTOCOL_TCP;

            /* Addresses and ports will be stored swapped because prvTCPReturnPacket
             * will swap them back while replying. */
            pxIPHeader->ulDestinationIPAddress = *ipLOCAL_IP_ADDRESS_POINTER;
            pxIPHeader->ulSourceIPAddress = FreeRTOS_htonl( pxSocket->u.xTCP.ulRemoteIP );

            pxTCPPacket->xTCPHeader.usSourcePort = FreeRTOS_htons( pxSocket->u.xTCP.usRemotePort );
            pxTCPPacket->xTCPHeader.usDestinationPort = FreeRTOS_htons( pxSocket->usLocalPort );

            /* We are actively connecting, so the peer's Initial Sequence Number (ISN)
             * isn't known yet. */
            pxSocket->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber = 0UL;

            /* Start with ISN (Initial Sequence Number). */
            pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber = ulInitialSequenceNumber;

            /* The TCP header size is 20 bytes, divided by 4 equals 5, which is put in
             * the high nibble of the TCP offset field. */
            pxTCPPacket->xTCPHeader.ucTCPOffset = 0x50U;

            /* Only set the SYN flag. */
            pxTCPPacket->xTCPHeader.ucTCPFlags = tcpTCP_FLAG_SYN;

            /* Set the values of usInitMSS / usCurMSS for this socket. */
            prvSocketSetMSS( pxSocket );

            /* The initial sequence numbers at our side are known.  Later
             * vTCPWindowInit() will be called to fill in the peer's sequence numbers, but
             * first wait for a SYN+ACK reply. */
            prvTCPCreateWindow( pxSocket );
        }

        return xReturn;
    }
    /*-----------------------------------------------------------*/

/* For logging and debugging: make a string showing the TCP flags
 */
    #if ( ipconfigHAS_DEBUG_PRINTF != 0 )

/**
 * @brief Print out the value of flags in a human readable manner.
 *
 * @param[in] xFlags: The TCP flags.
 *
 * @return The string containing the flags.
 */
        static const char * prvTCPFlagMeaning( UBaseType_t xFlags )
        {
            static char retString[ 10 ];
            size_t uxFlags = ( size_t ) xFlags;

            ( void ) snprintf( retString,
                               sizeof( retString ), "%c%c%c%c%c%c%c%c",
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_FIN ) != 0 ) ? 'F' : '.',   /* 0x0001: No more data from sender */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_SYN ) != 0 ) ? 'S' : '.',   /* 0x0002: Synchronize sequence numbers */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_RST ) != 0 ) ? 'R' : '.',   /* 0x0004: Reset the connection */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_PSH ) != 0 ) ? 'P' : '.',   /* 0x0008: Push function: please push buffered data to the recv application */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_ACK ) != 0 ) ? 'A' : '.',   /* 0x0010: Acknowledgment field is significant */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_URG ) != 0 ) ? 'U' : '.',   /* 0x0020: Urgent pointer field is significant */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_ECN ) != 0 ) ? 'E' : '.',   /* 0x0040: ECN-Echo */
                               ( ( uxFlags & ( size_t ) tcpTCP_FLAG_CWR ) != 0 ) ? 'C' : '.' ); /* 0x0080: Congestion Window Reduced */
            return retString;
        }
        /*-----------------------------------------------------------*/

    #endif /* ipconfigHAS_DEBUG_PRINTF */

/**
 * @brief Parse the TCP option(s) received, if present.
 *
 * @param[in] pxSocket: The socket handling the connection.
 * @param[in] pxNetworkBuffer: The network buffer containing the TCP
 *                             packet.
 *
 * @note It has already been verified that:
 *       ((pxTCPHeader->ucTCPOffset & 0xf0) > 0x50), meaning that
 *       the TP header is longer than the usual 20 (5 x 4) bytes.
 */
    _static void prvCheckOptions( FreeRTOS_Socket_t * pxSocket,
                                  const NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        size_t uxTCPHeaderOffset = ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer );
        const ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                              &( pxNetworkBuffer->pucEthernetBuffer[ uxTCPHeaderOffset ] ) );
        const TCPHeader_t * pxTCPHeader;
        const uint8_t * pucPtr;
        BaseType_t xHasSYNFlag;
        /* Offset in the network packet where the first option byte is stored. */
        size_t uxOptionOffset = uxTCPHeaderOffset + ( sizeof( TCPHeader_t ) - sizeof( pxTCPHeader->ucOptdata ) );
        size_t uxOptionsLength;
        size_t uxResult;
        uint8_t ucLength;

        pxTCPHeader = &( pxProtocolHeaders->xTCPHeader );


        /* A character pointer to iterate through the option data */
        pucPtr = pxTCPHeader->ucOptdata;

        if( pxTCPHeader->ucTCPOffset <= ( 5U << 4U ) )
        {
            /* Avoid integer underflow in computation of ucLength. */
        }
        else
        {
            ucLength = ( ( ( pxTCPHeader->ucTCPOffset >> 4U ) - 5U ) << 2U );
            uxOptionsLength = ( size_t ) ucLength;

            if( pxNetworkBuffer->xDataLength > uxOptionOffset )
            {
                /* Validate options size calculation. */
                if( ( pxNetworkBuffer->xDataLength > uxOptionOffset ) &&
                    ( uxOptionsLength <= ( pxNetworkBuffer->xDataLength - uxOptionOffset ) ) )
                {
                    if( ( pxTCPHeader->ucTCPFlags & tcpTCP_FLAG_SYN ) != ( uint8_t ) 0U )
                    {
                        xHasSYNFlag = pdTRUE;
                    }
                    else
                    {
                        xHasSYNFlag = pdFALSE;
                    }

                    /* The length check is only necessary in case the option data are
                     *  corrupted, we don't like to run into invalid memory and crash. */
                    for( ; ; )
                    {
                        if( uxOptionsLength == 0U )
                        {
                            /* coverity[break_stmt] : Break statement terminating the loop */
                            break;
                        }

                        uxResult = prvSingleStepTCPHeaderOptions( pucPtr, uxOptionsLength, pxSocket, xHasSYNFlag );

                        if( uxResult == 0UL )
                        {
                            break;
                        }

                        uxOptionsLength -= uxResult;
                        pucPtr = &( pucPtr[ uxResult ] );
                    }
                }
            }
        }
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Identify and deal with a single TCP header option, advancing the pointer to
 *        the header.
 *
 * @param[in] pucPtr: Pointer to the TCP packet options.
 * @param[in] uxTotalLength: Length of the TCP packet options.
 * @param[in] pxSocket: Socket handling the connection.
 * @param[in] xHasSYNFlag: Whether the header has SYN flag or not.
 *
 * @return This function returns pdTRUE or pdFALSE depending on whether the caller
 *         should continue to parse more header options or break the loop.
 */
    _static size_t prvSingleStepTCPHeaderOptions( const uint8_t * const pucPtr,
                                                  size_t uxTotalLength,
                                                  FreeRTOS_Socket_t * const pxSocket,
                                                  BaseType_t xHasSYNFlag )
    {
        UBaseType_t uxNewMSS;
        size_t uxRemainingOptionsBytes = uxTotalLength;
        uint8_t ucLen;
        size_t uxIndex;
        TCPWindow_t * pxTCPWindow = &( pxSocket->u.xTCP.xTCPWindow );
        BaseType_t xReturn = pdFALSE;

        if( pucPtr[ 0U ] == tcpTCP_OPT_END )
        {
            /* End of options. */
            uxIndex = 0U;
        }
        else if( pucPtr[ 0U ] == tcpTCP_OPT_NOOP )
        {
            /* NOP option, inserted to make the length a multiple of 4. */
            uxIndex = 1U;
        }
        else if( uxRemainingOptionsBytes < 2U )
        {
            /* Any other well-formed option must be at least two bytes: the option
             * type byte followed by a length byte. */
            uxIndex = 0U;
        }

        #if ( ipconfigUSE_TCP_WIN != 0 )
            else if( pucPtr[ 0 ] == tcpTCP_OPT_WSOPT )
            {
                /* The TCP Window Scale Option. */
                /* Confirm that the option fits in the remaining buffer space. */
                if( ( uxRemainingOptionsBytes < tcpTCP_OPT_WSOPT_LEN ) || ( pucPtr[ 1 ] != tcpTCP_OPT_WSOPT_LEN ) )
                {
                    uxIndex = 0U;
                }
                else
                {
                    /* Option is only valid in SYN phase. */
                    if( xHasSYNFlag != 0 )
                    {
                        pxSocket->u.xTCP.ucPeerWinScaleFactor = pucPtr[ 2 ];
                        pxSocket->u.xTCP.bits.bWinScaling = pdTRUE_UNSIGNED;
                    }

                    uxIndex = tcpTCP_OPT_WSOPT_LEN;
                }
            }
        #endif /* ipconfigUSE_TCP_WIN */
        else if( pucPtr[ 0 ] == tcpTCP_OPT_MSS )
        {
            /* Confirm that the option fits in the remaining buffer space. */
            if( ( uxRemainingOptionsBytes < tcpTCP_OPT_MSS_LEN ) || ( pucPtr[ 1 ] != tcpTCP_OPT_MSS_LEN ) )
            {
                uxIndex = 0U;
            }
            else
            {
                /* An MSS option with the correct option length.  FreeRTOS_htons()
                 * is not needed here because usChar2u16() already returns a host
                 * endian number. */
                uxNewMSS = usChar2u16( &( pucPtr[ 2 ] ) );

                if( pxSocket->u.xTCP.usInitMSS != uxNewMSS )
                {
                    /* Perform a basic check on the the new MSS. */
                    if( uxNewMSS == 0U )
                    {
                        uxIndex = 0U;

                        /* Return Condition found. */
                        xReturn = pdTRUE;
                    }
                    else
                    {
                        FreeRTOS_debug_printf( ( "MSS change %u -> %lu\n", pxSocket->u.xTCP.usInitMSS, uxNewMSS ) );
                    }
                }

                /* If a 'return' condition has not been found. */
                if( xReturn == pdFALSE )
                {
                    if( pxSocket->u.xTCP.usInitMSS > uxNewMSS )
                    {
                        /* our MSS was bigger than the MSS of the other party: adapt it. */
                        pxSocket->u.xTCP.bits.bMssChange = pdTRUE_UNSIGNED;

                        if( pxSocket->u.xTCP.usCurMSS > uxNewMSS )
                        {
                            /* The peer advertises a smaller MSS than this socket was
                             * using.  Use that as well. */
                            FreeRTOS_debug_printf( ( "Change mss %d => %lu\n", pxSocket->u.xTCP.usCurMSS, uxNewMSS ) );
                            pxSocket->u.xTCP.usCurMSS = ( uint16_t ) uxNewMSS;
                        }

                        pxTCPWindow->xSize.ulRxWindowLength = ( ( uint32_t ) uxNewMSS ) * ( pxTCPWindow->xSize.ulRxWindowLength / ( ( uint32_t ) uxNewMSS ) );
                        pxTCPWindow->usMSSInit = ( uint16_t ) uxNewMSS;
                        pxTCPWindow->usMSS = ( uint16_t ) uxNewMSS;
                        pxSocket->u.xTCP.usInitMSS = ( uint16_t ) uxNewMSS;
                        pxSocket->u.xTCP.usCurMSS = ( uint16_t ) uxNewMSS;
                    }

                    uxIndex = tcpTCP_OPT_MSS_LEN;
                }
            }
        }
        else
        {
            /* All other options have a length field, so that we easily
             * can skip past them. */
            ucLen = pucPtr[ 1 ];
            uxIndex = 0U;

            if( ( ucLen < ( uint8_t ) 2U ) || ( uxRemainingOptionsBytes < ( size_t ) ucLen ) )
            {
                /* If the length field is too small or too big, the options are
                 * malformed, don't process them further.
                 */
            }
            else
            {
                #if ( ipconfigUSE_TCP_WIN == 1 )
                    {
                        /* Selective ACK: the peer has received a packet but it is missing
                         * earlier packets. At least this packet does not need retransmission
                         * anymore. ulTCPWindowTxSack( ) takes care of this administration.
                         */
                        if( pucPtr[ 0U ] == tcpTCP_OPT_SACK_A )
                        {
                            ucLen -= 2U;
                            uxIndex += 2U;

                            while( ucLen >= ( uint8_t ) 8U )
                            {
                                prvReadSackOption( pucPtr, uxIndex, pxSocket );
                                uxIndex += 8U;
                                ucLen -= 8U;
                            }

                            /* ucLen should be 0 by now. */
                        }
                    }
                #endif /* ipconfigUSE_TCP_WIN == 1 */

                uxIndex += ( size_t ) ucLen;
            }
        }
        return uxIndex;
    }
    /*-----------------------------------------------------------*/

    #if ( ipconfigUSE_TCP_WIN == 1 )

/**
 * @brief Skip past TCP header options when doing Selective ACK, until there are no
 *        more options left.
 *
 * @param[in] pucPtr: Pointer to the TCP packet options.
 * @param[in] uxIndex: Index of options in the TCP packet options.
 * @param[in] pxSocket: Socket handling the TCP connection.
 */
        _static void prvReadSackOption( const uint8_t * const pucPtr,
                                        size_t uxIndex,
                                        FreeRTOS_Socket_t * const pxSocket )
        {
            uint32_t ulFirst = ulChar2u32( &( pucPtr[ uxIndex ] ) );
            uint32_t ulLast = ulChar2u32( &( pucPtr[ uxIndex + 4U ] ) );
            uint32_t ulCount = ulTCPWindowTxSack( &( pxSocket->u.xTCP.xTCPWindow ), ulFirst, ulLast );

            /* ulTCPWindowTxSack( ) returns the number of bytes which have been acked
             * starting from the head position.  Advance the tail pointer in txStream.
             */
            if( ( pxSocket->u.xTCP.txStream != NULL ) && ( ulCount > 0U ) )
            {
                /* Just advancing the tail index, 'ulCount' bytes have been confirmed. */
                ( void ) uxStreamBufferGet( pxSocket->u.xTCP.txStream, 0, NULL, ( size_t ) ulCount, pdFALSE );
                pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_SEND;

                #if ipconfigSUPPORT_SELECT_FUNCTION == 1
                    {
                        if( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_WRITE ) != 0U )
                        {
                            /* The field 'xEventBits' is used to store regular socket events
                             * (at most 8), as well as 'select events', which will be left-shifted.
                             */
                            pxSocket->xEventBits |= ( ( EventBits_t ) eSELECT_WRITE ) << SOCKET_EVENT_BIT_COUNT;
                        }
                    }
                #endif

                /* In case the socket owner has installed an OnSent handler,
                 * call it now. */
                #if ( ipconfigUSE_CALLBACKS == 1 )
                    {
                        if( ipconfigIS_VALID_PROG_ADDRESS( pxSocket->u.xTCP.pxHandleSent ) )
                        {
                            pxSocket->u.xTCP.pxHandleSent( pxSocket, ulCount );
                        }
                    }
                #endif /* ipconfigUSE_CALLBACKS == 1  */
            }
        }

    #endif /* ( ipconfigUSE_TCP_WIN != 0 ) */
    /*-----------------------------------------------------------*/


    #if ( ipconfigUSE_TCP_WIN != 0 )

/**
 * @brief Get the window scaling factor for the TCP connection.
 *
 * @param[in] pxSocket: The socket owning the TCP connection.
 *
 * @return The scaling factor.
 */
        static uint8_t prvWinScaleFactor( const FreeRTOS_Socket_t * pxSocket )
        {
            size_t uxWinSize;
            uint8_t ucFactor;


            /* 'xTCP.uxRxWinSize' is the size of the reception window in units of MSS. */
            uxWinSize = pxSocket->u.xTCP.uxRxWinSize * ( size_t ) pxSocket->u.xTCP.usInitMSS;
            ucFactor = 0U;

            while( uxWinSize > 0xffffUL )
            {
                /* Divide by two and increase the binary factor by 1. */
                uxWinSize >>= 1;
                ucFactor++;
            }

            FreeRTOS_debug_printf( ( "prvWinScaleFactor: uxRxWinSize %u MSS %u Factor %u\n",
                                     ( unsigned ) pxSocket->u.xTCP.uxRxWinSize,
                                     ( unsigned ) pxSocket->u.xTCP.usInitMSS,
                                     ucFactor ) );

            return ucFactor;
        }

    #endif /* if ( ipconfigUSE_TCP_WIN != 0 ) */
    /*-----------------------------------------------------------*/

/**
 * @brief When opening a TCP connection, while SYN's are being sent, the  parties may
 *        communicate what MSS (Maximum Segment Size) they intend to use, whether Selective
 *        ACK's ( SACK ) are supported, and the size of the reception window ( WSOPT ).
 *
 * @param[in] pxSocket: The socket being used for communication. It is used to set
 *                      the MSS.
 * @param[in,out] pxTCPHeader: The TCP packet header being used in the SYN transmission.
 *                             The MSS and corresponding options shall be set in this
 *                             header itself.
 *
 * @return The option length after the TCP header was updated.
 *
 * @note MSS is the net size of the payload, an is always smaller than MTU.
 */
    static UBaseType_t prvSetSynAckOptions( FreeRTOS_Socket_t * pxSocket,
                                            TCPHeader_t * pxTCPHeader )
    {
        uint16_t usMSS = pxSocket->u.xTCP.usInitMSS;
        UBaseType_t uxOptionsLength;

        /* We send out the TCP Maximum Segment Size option with our SYN[+ACK]. */

        pxTCPHeader->ucOptdata[ 0 ] = ( uint8_t ) tcpTCP_OPT_MSS;
        pxTCPHeader->ucOptdata[ 1 ] = ( uint8_t ) tcpTCP_OPT_MSS_LEN;
        pxTCPHeader->ucOptdata[ 2 ] = ( uint8_t ) ( usMSS >> 8 );
        pxTCPHeader->ucOptdata[ 3 ] = ( uint8_t ) ( usMSS & 0xffU );

        #if ( ipconfigUSE_TCP_WIN != 0 )
            {
                pxSocket->u.xTCP.ucMyWinScaleFactor = prvWinScaleFactor( pxSocket );

                pxTCPHeader->ucOptdata[ 4 ] = tcpTCP_OPT_NOOP;
                pxTCPHeader->ucOptdata[ 5 ] = ( uint8_t ) ( tcpTCP_OPT_WSOPT );
                pxTCPHeader->ucOptdata[ 6 ] = ( uint8_t ) ( tcpTCP_OPT_WSOPT_LEN );
                pxTCPHeader->ucOptdata[ 7 ] = ( uint8_t ) pxSocket->u.xTCP.ucMyWinScaleFactor;
                uxOptionsLength = 8U;
            }
        #else
            {
                uxOptionsLength = 4U;
            }
        #endif /* if ( ipconfigUSE_TCP_WIN != 0 ) */

        #if ( ipconfigUSE_TCP_WIN != 0 )
            {
                pxTCPHeader->ucOptdata[ uxOptionsLength ] = tcpTCP_OPT_NOOP;
                pxTCPHeader->ucOptdata[ uxOptionsLength + 1U ] = tcpTCP_OPT_NOOP;
                pxTCPHeader->ucOptdata[ uxOptionsLength + 2U ] = tcpTCP_OPT_SACK_P; /* 4: Sack-Permitted Option. */
                pxTCPHeader->ucOptdata[ uxOptionsLength + 3U ] = 2U;                /* 2: length of this option. */
                uxOptionsLength += 4U;
            }
        #endif /* ipconfigUSE_TCP_WIN == 0 */
        return uxOptionsLength; /* bytes, not words. */
    }

/**
 * @brief 'Touch' the socket to keep it alive/updated.
 *
 * @param[in] pxSocket: The socket to be updated.
 *
 * @note This is used for anti-hanging protection and TCP keep-alive messages.
 *       Called in two places: after receiving a packet and after a state change.
 *       The socket's alive timer may be reset.
 */
    static void prvTCPTouchSocket( FreeRTOS_Socket_t * pxSocket )
    {
        #if ( ipconfigTCP_HANG_PROTECTION == 1 )
            {
                pxSocket->u.xTCP.xLastActTime = xTaskGetTickCount();
            }
        #endif

        #if ( ipconfigTCP_KEEP_ALIVE == 1 )
            {
                pxSocket->u.xTCP.bits.bWaitKeepAlive = pdFALSE_UNSIGNED;
                pxSocket->u.xTCP.bits.bSendKeepAlive = pdFALSE_UNSIGNED;
                pxSocket->u.xTCP.ucKeepRepCount = 0U;
                pxSocket->u.xTCP.xLastAliveTime = xTaskGetTickCount();
            }
        #endif

        ( void ) pxSocket;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Changing to a new state. Centralised here to do specific actions such as
 *        resetting the alive timer, calling the user's OnConnect handler to notify
 *        that a socket has got (dis)connected, and setting bit to unblock a call to
 *        FreeRTOS_select().
 *
 * @param[in] pxSocket: The socket whose state we are trying to change.
 * @param[in] eTCPState: The state to which we want to change to.
 */
    void vTCPStateChange( FreeRTOS_Socket_t * pxSocket,
                          enum eTCP_STATE eTCPState )
    {
        FreeRTOS_Socket_t * xParent = NULL;
        BaseType_t bBefore = ipNUMERIC_CAST( BaseType_t, tcpNOW_CONNECTED( ( BaseType_t ) pxSocket->u.xTCP.ucTCPState ) ); /* Was it connected ? */
        BaseType_t bAfter = ipNUMERIC_CAST( BaseType_t, tcpNOW_CONNECTED( ( BaseType_t ) eTCPState ) );                    /* Is it connected now ? */

        #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
            BaseType_t xPreviousState = ( BaseType_t ) pxSocket->u.xTCP.ucTCPState;
        #endif
        #if ( ipconfigUSE_CALLBACKS == 1 )
            FreeRTOS_Socket_t * xConnected = NULL;
        #endif

        /* Has the connected status changed? */
        if( bBefore != bAfter )
        {
            /* Is the socket connected now ? */
            if( bAfter != pdFALSE )
            {
                /* if bPassQueued is true, this socket is an orphan until it gets connected. */
                if( pxSocket->u.xTCP.bits.bPassQueued != pdFALSE_UNSIGNED )
                {
                    /* Now that it is connected, find it's parent. */
                    if( pxSocket->u.xTCP.bits.bReuseSocket != pdFALSE_UNSIGNED )
                    {
                        xParent = pxSocket;
                    }
                    else
                    {
                        xParent = pxSocket->u.xTCP.pxPeerSocket;
                        configASSERT( xParent != NULL );
                    }

                    if( xParent != NULL )
                    {
                        if( xParent->u.xTCP.pxPeerSocket == NULL )
                        {
                            xParent->u.xTCP.pxPeerSocket = pxSocket;
                        }

                        xParent->xEventBits |= ( EventBits_t ) eSOCKET_ACCEPT;

                        #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
                            {
                                /* Library support FreeRTOS_select().  Receiving a new
                                 * connection is being translated as a READ event. */
                                if( ( xParent->xSelectBits & ( ( EventBits_t ) eSELECT_READ ) ) != 0U )
                                {
                                    xParent->xEventBits |= ( ( EventBits_t ) eSELECT_READ ) << SOCKET_EVENT_BIT_COUNT;
                                }
                            }
                        #endif

                        #if ( ipconfigUSE_CALLBACKS == 1 )
                            {
                                if( ( ipconfigIS_VALID_PROG_ADDRESS( xParent->u.xTCP.pxHandleConnected ) ) &&
                                    ( xParent->u.xTCP.bits.bReuseSocket == pdFALSE_UNSIGNED ) )
                                {
                                    /* The listening socket does not become connected itself, in stead
                                     * a child socket is created.
                                     * Postpone a call the OnConnect event until the end of this function. */
                                    xConnected = xParent;
                                }
                            }
                        #endif
                    }

                    /* Don't need to access the parent socket anymore, so the
                     * reference 'pxPeerSocket' may be cleared. */
                    pxSocket->u.xTCP.pxPeerSocket = NULL;
                    pxSocket->u.xTCP.bits.bPassQueued = pdFALSE_UNSIGNED;

                    /* When true, this socket may be returned in a call to accept(). */
                    pxSocket->u.xTCP.bits.bPassAccept = pdTRUE_UNSIGNED;
                }
                else
                {
                    pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_CONNECT;

                    #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
                        {
                            if( ( pxSocket->xSelectBits & ( ( EventBits_t ) eSELECT_WRITE ) ) != 0U )
                            {
                                pxSocket->xEventBits |= ( ( EventBits_t ) eSELECT_WRITE ) << SOCKET_EVENT_BIT_COUNT;
                            }
                        }
                    #endif
                }
            }
            else /* bAfter == pdFALSE, connection is closed. */
            {
                /* Notify/wake-up the socket-owner by setting a semaphore. */
                pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_CLOSED;

                #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
                    {
                        if( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_EXCEPT ) != 0U )
                        {
                            pxSocket->xEventBits |= ( ( EventBits_t ) eSELECT_EXCEPT ) << SOCKET_EVENT_BIT_COUNT;
                        }
                    }
                #endif
            }

            #if ( ipconfigUSE_CALLBACKS == 1 )
                {
                    if( ( ipconfigIS_VALID_PROG_ADDRESS( pxSocket->u.xTCP.pxHandleConnected ) ) && ( xConnected == NULL ) )
                    {
                        /* The 'connected' state has changed, call the user handler. */
                        xConnected = pxSocket;
                    }
                }
            #endif /* ipconfigUSE_CALLBACKS */

            if( prvTCPSocketIsActive( ipNUMERIC_CAST( eIPTCPState_t, pxSocket->u.xTCP.ucTCPState ) ) == 0 )
            {
                /* Now the socket isn't in an active state anymore so it
                 * won't need further attention of the IP-task.
                 * Setting time-out to zero means that the socket won't get checked during
                 * timer events. */
                pxSocket->u.xTCP.usTimeout = 0U;
            }
        }
        else
        {
            if( ( eTCPState == eCLOSED ) ||
                ( eTCPState == eCLOSE_WAIT ) )
            {
                /* Socket goes to status eCLOSED because of a RST.
                 * When nobody owns the socket yet, delete it. */
                if( ( pxSocket->u.xTCP.bits.bPassQueued != pdFALSE_UNSIGNED ) ||
                    ( pxSocket->u.xTCP.bits.bPassAccept != pdFALSE_UNSIGNED ) )
                {
                    FreeRTOS_debug_printf( ( "vTCPStateChange: Closing socket\n" ) );

                    if( pxSocket->u.xTCP.bits.bReuseSocket == pdFALSE_UNSIGNED )
                    {
                        ( void ) FreeRTOS_closesocket( pxSocket );
                    }
                }
            }
        }

        /* Fill in the new state. */
        pxSocket->u.xTCP.ucTCPState = ( uint8_t ) eTCPState;

        /* Touch the alive timers because moving to another state. */
        prvTCPTouchSocket( pxSocket );

        #if ( ipconfigHAS_DEBUG_PRINTF == 1 )
            {
                if( ( xTCPWindowLoggingLevel >= 0 ) && ( ipconfigTCP_MAY_LOG_PORT( pxSocket->usLocalPort ) ) )
                {
                    FreeRTOS_debug_printf( ( "Socket %d -> %lxip:%u State %s->%s\n",
                                             pxSocket->usLocalPort,
                                             pxSocket->u.xTCP.ulRemoteIP,
                                             pxSocket->u.xTCP.usRemotePort,
                                             FreeRTOS_GetTCPStateName( ( UBaseType_t ) xPreviousState ),
                                             FreeRTOS_GetTCPStateName( ( UBaseType_t ) eTCPState ) ) );
                }
            }
        #endif /* ipconfigHAS_DEBUG_PRINTF */

        #if ( ipconfigUSE_CALLBACKS == 1 )
            {
                if( xConnected != NULL )
                {
                    /* The 'connected' state has changed, call the OnConnect handler of the parent. */
                    xConnected->u.xTCP.pxHandleConnected( ( Socket_t ) xConnected, bAfter );
                }
            }
        #endif

        if( xParent != NULL )
        {
            vSocketWakeUpUser( xParent );
        }
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Check if the size of a network buffer is big enough to hold the outgoing message.
 *        Allocate a new bigger network buffer when necessary.
 *
 * @param[in] pxSocket: Socket whose buffer is being resized.
 * @param[in] pxNetworkBuffer: The network buffer whose size is being increased.
 * @param[in] lDataLen: Length of the data to be put in the buffer.
 * @param[in] uxOptionsLength: Length of options.
 *
 * @return If the resizing is successful: The new buffer with the size being asked for
 *                with old data copied in it.
 *         Else, NULL.
 *
 * @note The old network buffer will be released if the resizing is successful and
 *       cannot be used any longer.
 */
    static NetworkBufferDescriptor_t * prvTCPBufferResize( const FreeRTOS_Socket_t * pxSocket,
                                                           NetworkBufferDescriptor_t * pxNetworkBuffer,
                                                           int32_t lDataLen,
                                                           UBaseType_t uxOptionsLength )
    {
        NetworkBufferDescriptor_t * pxReturn;
        size_t uxNeeded;
        BaseType_t xResize;

        if( xBufferAllocFixedSize != pdFALSE )
        {
            /* Network buffers are created with a fixed size and can hold the largest
             * MTU. */
            uxNeeded = ( size_t ) ipTOTAL_ETHERNET_FRAME_SIZE;

            /* and therefore, the buffer won't be too small.
             * Only ask for a new network buffer in case none was supplied. */
            if( pxNetworkBuffer == NULL )
            {
                xResize = pdTRUE;
            }
            else
            {
                xResize = pdFALSE;
            }
        }
        else
        {
            /* Network buffers are created with a variable size. See if it must
             * grow. */
            uxNeeded = ipNUMERIC_CAST( size_t, ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength ) + lDataLen;

            if( uxNeeded < sizeof( pxSocket->u.xTCP.xPacket.u.ucLastPacket ) )
            {
                uxNeeded = sizeof( pxSocket->u.xTCP.xPacket.u.ucLastPacket );
            }

            /* In case we were called from a TCP timer event, a buffer must be
             *  created.  Otherwise, test 'xDataLength' of the provided buffer. */
            if( ( pxNetworkBuffer == NULL ) || ( pxNetworkBuffer->xDataLength < uxNeeded ) )
            {
                xResize = pdTRUE;
            }
            else
            {
                xResize = pdFALSE;
            }
        }

        if( xResize != pdFALSE )
        {
            /* The caller didn't provide a network buffer or the provided buffer is
             * too small.  As we must send-out a data packet, a buffer will be created
             * here. */
            pxReturn = pxGetNetworkBufferWithDescriptor( uxNeeded, 0U );

            if( pxReturn != NULL )
            {
                /* Set the actual packet size, in case the returned buffer is larger. */
                pxReturn->xDataLength = uxNeeded;

                /* Copy the existing data to the new created buffer. */
                if( pxNetworkBuffer != NULL )
                {
                    /* Either from the previous buffer... */
                    ( void ) memcpy( pxReturn->pucEthernetBuffer, pxNetworkBuffer->pucEthernetBuffer, pxNetworkBuffer->xDataLength );

                    /* ...and release it. */
                    vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                }
                else
                {
                    /* Or from the socket field 'xTCP.xPacket'. */
                    ( void ) memcpy( pxReturn->pucEthernetBuffer, pxSocket->u.xTCP.xPacket.u.ucLastPacket, sizeof( pxSocket->u.xTCP.xPacket.u.ucLastPacket ) );
                }
            }
        }
        else
        {
            /* xResize is false, the network buffer provided was big enough. */
            configASSERT( pxNetworkBuffer != NULL ); /* to tell lint: when xResize is false, pxNetworkBuffer is not NULL. */
            pxReturn = pxNetworkBuffer;

            pxNetworkBuffer->xDataLength = ( size_t ) ( ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength ) + ( size_t ) lDataLen;
        }

        return pxReturn;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Prepare an outgoing message, in case anything has to be sent.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in,out] ppxNetworkBuffer: Pointer to the pointer to the network buffer.
 * @param[in] uxOptionsLength: The length of the TCP options.
 *
 * @return Length of the data to be sent if everything is correct. Else, -1
 *         is returned in case of any error.
 */
    static int32_t prvTCPPrepareSend( FreeRTOS_Socket_t * pxSocket,
                                      NetworkBufferDescriptor_t ** ppxNetworkBuffer,
                                      UBaseType_t uxOptionsLength )
    {
        int32_t lDataLen;
        uint8_t * pucEthernetBuffer, * pucSendData;
        ProtocolHeaders_t * pxProtocolHeaders;
        size_t uxOffset;
        uint32_t ulDataGot, ulDistance;
        TCPWindow_t * pxTCPWindow;
        NetworkBufferDescriptor_t * pxNewBuffer;
        int32_t lStreamPos;
        UBaseType_t uxIntermediateResult = 0;

        if( ( *ppxNetworkBuffer ) != NULL )
        {
            /* A network buffer descriptor was already supplied */
            pucEthernetBuffer = ( *ppxNetworkBuffer )->pucEthernetBuffer;
        }
        else
        {
            /* For now let it point to the last packet header */
            pucEthernetBuffer = pxSocket->u.xTCP.xPacket.u.ucLastPacket;
        }

        /* Map the ethernet buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t, &( pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) ] ) );
        pxTCPWindow = &( pxSocket->u.xTCP.xTCPWindow );
        lDataLen = 0;
        lStreamPos = 0;
        pxProtocolHeaders->xTCPHeader.ucTCPFlags |= tcpTCP_FLAG_ACK;

        if( pxSocket->u.xTCP.txStream != NULL )
        {
            /* ulTCPWindowTxGet will return the amount of data which may be sent
             * along with the position in the txStream.
             * Why check for MSS > 1 ?
             * Because some TCP-stacks (like uIP) use it for flow-control. */
            if( pxSocket->u.xTCP.usCurMSS > 1U )
            {
                lDataLen = ( int32_t ) ulTCPWindowTxGet( pxTCPWindow, pxSocket->u.xTCP.ulWindowSize, &lStreamPos );
            }

            if( lDataLen > 0 )
            {
                /* Check if the current network buffer is big enough, if not,
                 * resize it. */
                pxNewBuffer = prvTCPBufferResize( pxSocket, *ppxNetworkBuffer, lDataLen, uxOptionsLength );

                if( pxNewBuffer != NULL )
                {
                    *ppxNetworkBuffer = pxNewBuffer;
                    pucEthernetBuffer = pxNewBuffer->pucEthernetBuffer;

                    /* Map the byte stream onto ProtocolHeaders_t struct for easy
                     * access to the fields. */
                    pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t, &( pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) ] ) );

                    pucSendData = &( pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength ] );

                    /* Translate the position in txStream to an offset from the tail
                     * marker. */
                    uxOffset = uxStreamBufferDistance( pxSocket->u.xTCP.txStream, pxSocket->u.xTCP.txStream->uxTail, ( size_t ) lStreamPos );

                    /* Here data is copied from the txStream in 'peek' mode.  Only
                     * when the packets are acked, the tail marker will be updated. */
                    ulDataGot = ( uint32_t ) uxStreamBufferGet( pxSocket->u.xTCP.txStream, uxOffset, pucSendData, ( size_t ) lDataLen, pdTRUE );

                    #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                        {
                            if( ulDataGot != ( uint32_t ) lDataLen )
                            {
                                FreeRTOS_debug_printf( ( "uxStreamBufferGet: pos %d offs %u only %u != %d\n",
                                                         ( int ) lStreamPos, ( unsigned ) uxOffset, ( unsigned ) ulDataGot, ( int ) lDataLen ) );
                            }
                        }
                    #endif

                    /* If the owner of the socket requests a closure, add the FIN
                     * flag to the last packet. */
                    if( ( pxSocket->u.xTCP.bits.bCloseRequested != pdFALSE_UNSIGNED ) && ( pxSocket->u.xTCP.bits.bFinSent == pdFALSE_UNSIGNED ) )
                    {
                        ulDistance = ( uint32_t ) uxStreamBufferDistance( pxSocket->u.xTCP.txStream, ( size_t ) lStreamPos, pxSocket->u.xTCP.txStream->uxHead );

                        if( ulDistance == ulDataGot )
                        {
                            #if ( ipconfigHAS_DEBUG_PRINTF == 1 )
                                {
                                    /* the order of volatile accesses is undefined
                                     *  so such workaround */
                                    size_t uxHead = pxSocket->u.xTCP.txStream->uxHead;
                                    size_t uxMid = pxSocket->u.xTCP.txStream->uxMid;
                                    size_t uxTail = pxSocket->u.xTCP.txStream->uxTail;

                                    FreeRTOS_debug_printf( ( "CheckClose %u <= %u (%u <= %u <= %u)\n",
                                                             ( unsigned ) ulDataGot, ( unsigned ) ulDistance,
                                                             ( unsigned ) uxTail, ( unsigned ) uxMid, ( unsigned ) uxHead ) );
                                }
                            #endif /* if ( ipconfigHAS_DEBUG_PRINTF == 1 ) */

                            /* Although the socket sends a FIN, it will stay in
                             * ESTABLISHED until all current data has been received or
                             * delivered. */
                            pxProtocolHeaders->xTCPHeader.ucTCPFlags |= tcpTCP_FLAG_FIN;
                            pxTCPWindow->tx.ulFINSequenceNumber = pxTCPWindow->ulOurSequenceNumber + ( uint32_t ) lDataLen;
                            pxSocket->u.xTCP.bits.bFinSent = pdTRUE_UNSIGNED;
                        }
                    }
                }
                else
                {
                    lDataLen = -1;
                }
            }
        }

        if( ( lDataLen >= 0 ) && ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eESTABLISHED ) )
        {
            /* See if the socket owner wants to shutdown this connection. */
            if( ( pxSocket->u.xTCP.bits.bUserShutdown != pdFALSE_UNSIGNED ) &&
                ( xTCPWindowTxDone( pxTCPWindow ) != pdFALSE ) )
            {
                pxSocket->u.xTCP.bits.bUserShutdown = pdFALSE_UNSIGNED;
                pxProtocolHeaders->xTCPHeader.ucTCPFlags |= tcpTCP_FLAG_FIN;
                pxSocket->u.xTCP.bits.bFinSent = pdTRUE_UNSIGNED;
                pxSocket->u.xTCP.bits.bWinChange = pdTRUE_UNSIGNED;
                pxTCPWindow->tx.ulFINSequenceNumber = pxTCPWindow->tx.ulCurrentSequenceNumber;
                vTCPStateChange( pxSocket, eFIN_WAIT_1 );
            }

            #if ( ipconfigTCP_KEEP_ALIVE != 0 )
                {
                    if( pxSocket->u.xTCP.ucKeepRepCount > 3U ) /*_RB_ Magic number. */
                    {
                        FreeRTOS_debug_printf( ( "keep-alive: giving up %lxip:%u\n",
                                                 pxSocket->u.xTCP.ulRemoteIP,       /* IP address of remote machine. */
                                                 pxSocket->u.xTCP.usRemotePort ) ); /* Port on remote machine. */
                        vTCPStateChange( pxSocket, eCLOSE_WAIT );
                        lDataLen = -1;
                    }

                    if( ( lDataLen == 0 ) && ( pxSocket->u.xTCP.bits.bWinChange == pdFALSE_UNSIGNED ) )
                    {
                        /* If there is no data to be sent, and no window-update message,
                         * we might want to send a keep-alive message. */
                        TickType_t xAge = xTaskGetTickCount() - pxSocket->u.xTCP.xLastAliveTime;
                        TickType_t xMax;
                        xMax = ( ( TickType_t ) ipconfigTCP_KEEP_ALIVE_INTERVAL * ( TickType_t ) configTICK_RATE_HZ );

                        if( pxSocket->u.xTCP.ucKeepRepCount != ( uint8_t ) 0U )
                        {
                            xMax = ( TickType_t ) ( 3U * configTICK_RATE_HZ );
                        }

                        if( xAge > xMax )
                        {
                            pxSocket->u.xTCP.xLastAliveTime = xTaskGetTickCount();

                            if( xTCPWindowLoggingLevel != 0 )
                            {
                                FreeRTOS_debug_printf( ( "keep-alive: %lxip:%u count %u\n",
                                                         pxSocket->u.xTCP.ulRemoteIP,
                                                         pxSocket->u.xTCP.usRemotePort,
                                                         pxSocket->u.xTCP.ucKeepRepCount ) );
                            }

                            pxSocket->u.xTCP.bits.bSendKeepAlive = pdTRUE_UNSIGNED;
                            pxSocket->u.xTCP.usTimeout = ( ( uint16_t ) pdMS_TO_TICKS( 2500U ) );
                            pxSocket->u.xTCP.ucKeepRepCount++;
                        }
                    }
                }
            #endif /* ipconfigTCP_KEEP_ALIVE */
        }

        /* Anything to send, a change of the advertised window size, or maybe send a
         * keep-alive message? */
        if( ( lDataLen > 0 ) ||
            ( pxSocket->u.xTCP.bits.bWinChange != pdFALSE_UNSIGNED ) ||
            ( pxSocket->u.xTCP.bits.bSendKeepAlive != pdFALSE_UNSIGNED ) )
        {
            pxProtocolHeaders->xTCPHeader.ucTCPFlags &= ( ( uint8_t ) ~tcpTCP_FLAG_PSH );
            pxProtocolHeaders->xTCPHeader.ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 ); /*_RB_ "2" needs comment. */

            pxProtocolHeaders->xTCPHeader.ucTCPFlags |= ( uint8_t ) tcpTCP_FLAG_ACK;

            if( lDataLen != 0L )
            {
                pxProtocolHeaders->xTCPHeader.ucTCPFlags |= ( uint8_t ) tcpTCP_FLAG_PSH;
            }

            uxIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength;
            lDataLen += ( int32_t ) uxIntermediateResult;
        }

        return lDataLen;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Calculate after how much time this socket needs to be checked again.
 *
 * @param[in] pxSocket: The socket to be checked.
 *
 * @return The number of clock ticks before the timer expires.
 */
    static TickType_t prvTCPNextTimeout( FreeRTOS_Socket_t * pxSocket )
    {
        TickType_t ulDelayMs = ( TickType_t ) tcpMAXIMUM_TCP_WAKEUP_TIME_MS;

        if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCONNECT_SYN )
        {
            /* The socket is actively connecting to a peer. */
            if( pxSocket->u.xTCP.bits.bConnPrepared != pdFALSE_UNSIGNED )
            {
                /* Ethernet address has been found, use progressive timeout for
                 * active connect(). */
                if( pxSocket->u.xTCP.ucRepCount < 3U )
                {
                    ulDelayMs = ( 3000UL << ( pxSocket->u.xTCP.ucRepCount - 1U ) );
                }
                else
                {
                    ulDelayMs = 11000UL;
                }
            }
            else
            {
                /* Still in the ARP phase: check every half second. */
                ulDelayMs = 500UL;
            }

            FreeRTOS_debug_printf( ( "Connect[%lxip:%u]: next timeout %u: %lu ms\n",
                                     pxSocket->u.xTCP.ulRemoteIP, pxSocket->u.xTCP.usRemotePort,
                                     pxSocket->u.xTCP.ucRepCount, ulDelayMs ) );
            pxSocket->u.xTCP.usTimeout = ( uint16_t ) ipMS_TO_MIN_TICKS( ulDelayMs );
        }
        else if( pxSocket->u.xTCP.usTimeout == 0U )
        {
            /* Let the sliding window mechanism decide what time-out is appropriate. */
            BaseType_t xResult = xTCPWindowTxHasData( &pxSocket->u.xTCP.xTCPWindow, pxSocket->u.xTCP.ulWindowSize, &ulDelayMs );

            if( ulDelayMs == 0U )
            {
                if( xResult != ( BaseType_t ) 0 )
                {
                    ulDelayMs = 1UL;
                }
                else
                {
                    ulDelayMs = tcpMAXIMUM_TCP_WAKEUP_TIME_MS;
                }
            }
            else
            {
                /* ulDelayMs contains the time to wait before a re-transmission. */
            }

            pxSocket->u.xTCP.usTimeout = ( uint16_t ) ipMS_TO_MIN_TICKS( ulDelayMs );
        }
        else
        {
            /* field '.usTimeout' has already been set (by the
             * keep-alive/delayed-ACK mechanism). */
        }

        /* Return the number of clock ticks before the timer expires. */
        return ( TickType_t ) pxSocket->u.xTCP.usTimeout;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief The API FreeRTOS_send() adds data to the TX stream. Add
 *        this data to the windowing system to it can be transmitted.
 *
 * @param[in] pxSocket: The socket owning the connection.
 */
    static void prvTCPAddTxData( FreeRTOS_Socket_t * pxSocket )
    {
        int32_t lCount, lLength;

        /* A txStream has been created already, see if the socket has new data for
         * the sliding window.
         *
         * uxStreamBufferMidSpace() returns the distance between rxHead and rxMid.  It
         * contains new Tx data which has not been passed to the sliding window yet.
         * The oldest data not-yet-confirmed can be found at rxTail. */
        lLength = ( int32_t ) uxStreamBufferMidSpace( pxSocket->u.xTCP.txStream );

        if( lLength > 0 )
        {
            /* All data between txMid and rxHead will now be passed to the sliding
             * window manager, so it can start transmitting them.
             *
             * Hand over the new data to the sliding window handler.  It will be
             * split-up in chunks of 1460 bytes each (or less, depending on
             * ipconfigTCP_MSS). */
            lCount = lTCPWindowTxAdd( &pxSocket->u.xTCP.xTCPWindow,
                                      ( uint32_t ) lLength,
                                      ( int32_t ) pxSocket->u.xTCP.txStream->uxMid,
                                      ( int32_t ) pxSocket->u.xTCP.txStream->LENGTH );

            /* Move the rxMid pointer forward up to rxHead. */
            if( lCount > 0 )
            {
                vStreamBufferMoveMid( pxSocket->u.xTCP.txStream, ( size_t ) lCount );
            }
        }
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvTCPHandleFin() will be called to handle connection closure. The
 *        closure starts when either a FIN has been received and accepted,
 *        or when the socket has sent a FIN flag to the peer. Before being
 *        called, it has been checked that both reception and transmission
 *        are complete.
 *
 * @param[in] pxSocket: Socket owning the the connection.
 * @param[in] pxNetworkBuffer: The network buffer carrying the TCP packet.
 *
 * @return Length of the packet to be sent.
 */
    static BaseType_t prvTCPHandleFin( FreeRTOS_Socket_t * pxSocket,
                                       const NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        /* Map the ethernet buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                        &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) ] ) );
        TCPHeader_t * pxTCPHeader = &( pxProtocolHeaders->xTCPHeader );
        uint8_t ucIntermediateResult = 0, ucTCPFlags = pxTCPHeader->ucTCPFlags;
        TCPWindow_t * pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
        BaseType_t xSendLength = 0;
        uint32_t ulAckNr = FreeRTOS_ntohl( pxTCPHeader->ulAckNr );

        if( ( ucTCPFlags & tcpTCP_FLAG_FIN ) != 0U )
        {
            pxTCPWindow->rx.ulCurrentSequenceNumber = pxTCPWindow->rx.ulFINSequenceNumber + 1U;
        }

        if( pxSocket->u.xTCP.bits.bFinSent == pdFALSE_UNSIGNED )
        {
            /* We haven't yet replied with a FIN, do so now. */
            pxTCPWindow->tx.ulFINSequenceNumber = pxTCPWindow->tx.ulCurrentSequenceNumber;
            pxSocket->u.xTCP.bits.bFinSent = pdTRUE_UNSIGNED;
        }
        else
        {
            /* We did send a FIN already, see if it's ACK'd. */
            if( ulAckNr == ( pxTCPWindow->tx.ulFINSequenceNumber + 1UL ) )
            {
                pxSocket->u.xTCP.bits.bFinAcked = pdTRUE_UNSIGNED;
            }
        }

        if( pxSocket->u.xTCP.bits.bFinAcked == pdFALSE_UNSIGNED )
        {
            pxTCPWindow->tx.ulCurrentSequenceNumber = pxTCPWindow->tx.ulFINSequenceNumber;
            pxTCPHeader->ucTCPFlags = ( uint8_t ) tcpTCP_FLAG_ACK | ( uint8_t ) tcpTCP_FLAG_FIN;

            /* And wait for the final ACK. */
            vTCPStateChange( pxSocket, eLAST_ACK );
        }
        else
        {
            /* Our FIN has been ACK'd, the outgoing sequence number is now fixed. */
            pxTCPWindow->tx.ulCurrentSequenceNumber = pxTCPWindow->tx.ulFINSequenceNumber + 1U;

            if( pxSocket->u.xTCP.bits.bFinRecv == pdFALSE_UNSIGNED )
            {
                /* We have sent out a FIN but the peer hasn't replied with a FIN
                 * yet. Do nothing for the moment. */
                pxTCPHeader->ucTCPFlags = 0U;
            }
            else
            {
                if( pxSocket->u.xTCP.bits.bFinLast == pdFALSE_UNSIGNED )
                {
                    /* This is the third of the three-way hand shake: the last
                     * ACK. */
                    pxTCPHeader->ucTCPFlags = tcpTCP_FLAG_ACK;
                }
                else
                {
                    /* The other party started the closure, so we just wait for the
                     * last ACK. */
                    pxTCPHeader->ucTCPFlags = 0U;
                }

                /* And wait for the user to close this socket. */
                vTCPStateChange( pxSocket, eCLOSE_WAIT );
            }
        }

        pxTCPWindow->ulOurSequenceNumber = pxTCPWindow->tx.ulCurrentSequenceNumber;

        if( pxTCPHeader->ucTCPFlags != 0U )
        {
            ucIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + pxTCPWindow->ucOptionLength;
            xSendLength = ( BaseType_t ) ucIntermediateResult;
        }

        pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + pxTCPWindow->ucOptionLength ) << 2 );

        if( xTCPWindowLoggingLevel != 0 )
        {
            FreeRTOS_debug_printf( ( "TCP: send FIN+ACK (ack %lu, cur/nxt %lu/%lu) ourSeqNr %lu | Rx %lu\n",
                                     ulAckNr - pxTCPWindow->tx.ulFirstSequenceNumber,
                                     pxTCPWindow->tx.ulCurrentSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber,
                                     pxTCPWindow->ulNextTxSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber,
                                     pxTCPWindow->ulOurSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber,
                                     pxTCPWindow->rx.ulCurrentSequenceNumber - pxTCPWindow->rx.ulFirstSequenceNumber ) );
        }

        return xSendLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvCheckRxData(): called from prvTCPHandleState(). The
 *        first thing that will be done is find the TCP payload data
 *        and check the length of this data.
 *
 * @param[in] pxNetworkBuffer: The network buffer holding the received data.
 * @param[out] ppucRecvData: It will point to first byte of the TCP payload.
 *
 * @return Length of the received buffer.
 */
    static BaseType_t prvCheckRxData( const NetworkBufferDescriptor_t * pxNetworkBuffer,
                                      uint8_t ** ppucRecvData )
    {
        /* Map the ethernet buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        const ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                              &( pxNetworkBuffer->pucEthernetBuffer[ ( size_t ) ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) ] ) );
        const TCPHeader_t * pxTCPHeader = &( pxProtocolHeaders->xTCPHeader );
        int32_t lLength, lTCPHeaderLength, lReceiveLength, lUrgentLength;

        /* Map the buffer onto an IPHeader_t struct for easy access to fields. */
        const IPHeader_t * pxIPHeader = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( IPHeader_t, &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER ] ) );
        const size_t xIPHeaderLength = ipSIZE_OF_IPv4_HEADER;
        uint16_t usLength;
        uint8_t ucIntermediateResult = 0;

        /* Determine the length and the offset of the user-data sent to this
         * node.
         *
         * The size of the TCP header is given in a multiple of 4-byte words (single
         * byte, needs no ntoh() translation).  A shift-right 2: is the same as
         * (offset >> 4) * 4. */
        ucIntermediateResult = ( pxTCPHeader->ucTCPOffset & tcpVALID_BITS_IN_TCP_OFFSET_BYTE ) >> 2;
        lTCPHeaderLength = ( int32_t ) ucIntermediateResult;

        /* Let pucRecvData point to the first byte received. */
        *ppucRecvData = &( pxNetworkBuffer->pucEthernetBuffer[ ( size_t ) ipSIZE_OF_ETH_HEADER + xIPHeaderLength + ( size_t ) lTCPHeaderLength ] );

        /* Calculate lReceiveLength - the length of the TCP data received.  This is
         * equal to the total packet length minus:
         * ( LinkLayer length (14) + IP header length (20) + size of TCP header(20 +) ).*/
        lReceiveLength = ipNUMERIC_CAST( int32_t, pxNetworkBuffer->xDataLength ) - ( int32_t ) ipSIZE_OF_ETH_HEADER;

        usLength = FreeRTOS_htons( pxIPHeader->usLength );
        lLength = ( int32_t ) usLength;

        if( lReceiveLength > lLength )
        {
            /* More bytes were received than the reported length, often because of
             * padding bytes at the end. */
            lReceiveLength = lLength;
        }

        /* Subtract the size of the TCP and IP headers and the actual data size is
         * known. */
        if( lReceiveLength > ( lTCPHeaderLength + ( int32_t ) xIPHeaderLength ) )
        {
            lReceiveLength -= ( lTCPHeaderLength + ( int32_t ) xIPHeaderLength );
        }
        else
        {
            lReceiveLength = 0;
        }

        /* Urgent Pointer:
         * This field communicates the current value of the urgent pointer as a
         * positive offset from the sequence number in this segment.  The urgent
         * pointer points to the sequence number of the octet following the urgent
         * data.  This field is only be interpreted in segments with the URG control
         * bit set. */
        if( ( pxTCPHeader->ucTCPFlags & tcpTCP_FLAG_URG ) != 0U )
        {
            /* Although we ignore the urgent data, we have to skip it. */
            lUrgentLength = ( int32_t ) FreeRTOS_htons( pxTCPHeader->usUrgent );
            *ppucRecvData += lUrgentLength;
            lReceiveLength -= FreeRTOS_min_int32( lReceiveLength, lUrgentLength );
        }

        return ( BaseType_t ) lReceiveLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvStoreRxData(): called from prvTCPHandleState().
 *        The second thing is to do is check if the payload data may
 *        be accepted. If so, they will be added to the reception queue.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in] pucRecvData: Pointer to received data.
 * @param[in] pxNetworkBuffer: The network buffer descriptor.
 * @param[in] ulReceiveLength: The length of the received data.
 *
 * @return 0 on success, -1 on failure of storing data.
 */
    static BaseType_t prvStoreRxData( FreeRTOS_Socket_t * pxSocket,
                                      const uint8_t * pucRecvData,
                                      NetworkBufferDescriptor_t * pxNetworkBuffer,
                                      uint32_t ulReceiveLength )
    {
        /* Map the ethernet buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        const ProtocolHeaders_t * pxProtocolHeaders = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( ProtocolHeaders_t,
                                                                                          &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) ] ) );
        const TCPHeader_t * pxTCPHeader = &pxProtocolHeaders->xTCPHeader;
        TCPWindow_t * pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
        uint32_t ulSequenceNumber, ulSpace;
        int32_t lOffset, lStored;
        BaseType_t xResult = 0;

        ulSequenceNumber = FreeRTOS_ntohl( pxTCPHeader->ulSequenceNumber );

        if( ( ulReceiveLength > 0U ) && ( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eSYN_RECEIVED ) )
        {
            /* See if way may accept the data contents and forward it to the socket
             * owner.
             *
             * If it can't be "accept"ed it may have to be stored and send a selective
             * ack (SACK) option to confirm it.  In that case, lTCPAddRxdata() will be
             * called later to store an out-of-order packet (in case lOffset is
             * negative). */
            if( pxSocket->u.xTCP.rxStream != NULL )
            {
                ulSpace = ( uint32_t ) uxStreamBufferGetSpace( pxSocket->u.xTCP.rxStream );
            }
            else
            {
                ulSpace = ( uint32_t ) pxSocket->u.xTCP.uxRxStreamSize;
            }

            lOffset = lTCPWindowRxCheck( pxTCPWindow, ulSequenceNumber, ulReceiveLength, ulSpace );

            if( lOffset >= 0 )
            {
                /* New data has arrived and may be made available to the user.  See
                 * if the head marker in rxStream may be advanced, only if lOffset == 0.
                 * In case the low-water mark is reached, bLowWater will be set
                 * "low-water" here stands for "little space". */
                lStored = lTCPAddRxdata( pxSocket, ( uint32_t ) lOffset, pucRecvData, ulReceiveLength );

                if( lStored != ( int32_t ) ulReceiveLength )
                {
                    FreeRTOS_debug_printf( ( "lTCPAddRxdata: stored %ld / %lu bytes? ?\n", lStored, ulReceiveLength ) );

                    /* Received data could not be stored.  The socket's flag
                     * bMallocError has been set.  The socket now has the status
                     * eCLOSE_WAIT and a RST packet will be sent back. */
                    ( void ) prvTCPSendReset( pxNetworkBuffer );
                    xResult = -1;
                }
            }

            /* After a missing packet has come in, higher packets may be passed to
             * the user. */
            #if ( ipconfigUSE_TCP_WIN == 1 )
                {
                    /* Now lTCPAddRxdata() will move the rxHead pointer forward
                     * so data becomes available to the user immediately
                     * In case the low-water mark is reached, bLowWater will be set. */
                    if( ( xResult == 0 ) && ( pxTCPWindow->ulUserDataLength > 0UL ) )
                    {
                        ( void ) lTCPAddRxdata( pxSocket, 0UL, NULL, pxTCPWindow->ulUserDataLength );
                        pxTCPWindow->ulUserDataLength = 0;
                    }
                }
            #endif /* ipconfigUSE_TCP_WIN */
        }
        else
        {
            pxTCPWindow->ucOptionLength = 0U;
        }

        return xResult;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Set the TCP options (if any) for the outgoing packet.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in] pxNetworkBuffer: The network buffer holding the packet.
 *
 * @return Length of the TCP options after they are set.
 */
    static UBaseType_t prvSetOptions( FreeRTOS_Socket_t * pxSocket,
                                      const NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        /* Map the ethernet buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                        &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) ] ) );
        TCPHeader_t * pxTCPHeader = &pxProtocolHeaders->xTCPHeader;
        const TCPWindow_t * pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
        UBaseType_t uxOptionsLength = pxTCPWindow->ucOptionLength;
        /* memcpy() helper variables for MISRA Rule 21.15 compliance*/
        const void * pvCopySource;
        void * pvCopyDest;

        #if ( ipconfigUSE_TCP_WIN == 1 )
            if( uxOptionsLength != 0U )
            {
                /* TCP options must be sent because a packet which is out-of-order
                 * was received. */
                if( xTCPWindowLoggingLevel >= 0 )
                {
                    FreeRTOS_debug_printf( ( "SACK[%d,%d]: optlen %lu sending %lu - %lu\n",
                                             pxSocket->usLocalPort,
                                             pxSocket->u.xTCP.usRemotePort,
                                             uxOptionsLength,
                                             FreeRTOS_ntohl( pxTCPWindow->ulOptionsData[ 1 ] ) - pxSocket->u.xTCP.xTCPWindow.rx.ulFirstSequenceNumber,
                                             FreeRTOS_ntohl( pxTCPWindow->ulOptionsData[ 2 ] ) - pxSocket->u.xTCP.xTCPWindow.rx.ulFirstSequenceNumber ) );
                }

                /*
                 * Use helper variables for memcpy() source & dest to remain
                 * compliant with MISRA Rule 21.15.  These should be
                 * optimized away.
                 */
                pvCopySource = pxTCPWindow->ulOptionsData;
                pvCopyDest = pxTCPHeader->ucOptdata;
                ( void ) memcpy( pvCopyDest, pvCopySource, ( size_t ) uxOptionsLength );

                /* The header length divided by 4, goes into the higher nibble,
                 * effectively a shift-left 2. */
                pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );
            }
            else
        #endif /* ipconfigUSE_TCP_WIN */

        if( ( pxSocket->u.xTCP.ucTCPState >= ( EventBits_t ) eESTABLISHED ) && ( pxSocket->u.xTCP.bits.bMssChange != pdFALSE_UNSIGNED ) )
        {
            /* TCP options must be sent because the MSS has changed. */
            pxSocket->u.xTCP.bits.bMssChange = pdFALSE_UNSIGNED;

            if( xTCPWindowLoggingLevel >= 0 )
            {
                FreeRTOS_debug_printf( ( "MSS: sending %d\n", pxSocket->u.xTCP.usCurMSS ) );
            }

            pxTCPHeader->ucOptdata[ 0 ] = tcpTCP_OPT_MSS;
            pxTCPHeader->ucOptdata[ 1 ] = tcpTCP_OPT_MSS_LEN;
            pxTCPHeader->ucOptdata[ 2 ] = ( uint8_t ) ( ( pxSocket->u.xTCP.usCurMSS ) >> 8 );
            pxTCPHeader->ucOptdata[ 3 ] = ( uint8_t ) ( ( pxSocket->u.xTCP.usCurMSS ) & 0xffU );
            uxOptionsLength = 4U;
            pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );
        }
        else
        {
            /* Nothing. */
        }

        return uxOptionsLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvHandleSynReceived(): called from prvTCPHandleState(). Called
 *        from the states: eSYN_RECEIVED and eCONNECT_SYN. If the flags
 *        received are correct, the socket will move to eESTABLISHED.
 *
 * @param[in] pxSocket: The socket handling the connection.
 * @param[in] pxNetworkBuffer: The pointer to the network buffer carrying
 *                             the packet.
 * @param[in] ulReceiveLength: Length in bytes of the data received.
 * @param[in] uxOptionsLength: Length of the TCP options in bytes.
 *
 * @return Length of the data to be sent.
 */
    static BaseType_t prvHandleSynReceived( FreeRTOS_Socket_t * pxSocket,
                                            const NetworkBufferDescriptor_t * pxNetworkBuffer,
                                            uint32_t ulReceiveLength,
                                            UBaseType_t uxOptionsLength )
    {
        /* Map the ethernet buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                        &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) ] ) );
        TCPHeader_t * pxTCPHeader = &pxProtocolHeaders->xTCPHeader;
        TCPWindow_t * pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
        uint8_t ucTCPFlags = pxTCPHeader->ucTCPFlags;
        uint32_t ulSequenceNumber = FreeRTOS_ntohl( pxTCPHeader->ulSequenceNumber );
        BaseType_t xSendLength = 0;
        UBaseType_t uxIntermediateResult = 0U;

        /* Either expect a ACK or a SYN+ACK. */
        uint8_t ucExpect = tcpTCP_FLAG_ACK;
        const uint8_t ucFlagsMask = tcpTCP_FLAG_ACK | tcpTCP_FLAG_RST | tcpTCP_FLAG_SYN | tcpTCP_FLAG_FIN;

        if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCONNECT_SYN )
        {
            ucExpect |= tcpTCP_FLAG_SYN;
        }

        if( ( ucTCPFlags & ucFlagsMask ) != ucExpect )
        {
            /* eSYN_RECEIVED: flags 0010 expected, not 0002. */
            /* eSYN_RECEIVED: flags ACK  expected, not SYN. */
            FreeRTOS_debug_printf( ( "%s: flags %04X expected, not %04X\n",
                                     ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eSYN_RECEIVED ) ? "eSYN_RECEIVED" : "eCONNECT_SYN",
                                     ucExpect, ucTCPFlags ) );

            /* In case pxSocket is not yet owned by the application, a closure
             * of the socket will be scheduled for the next cycle. */
            vTCPStateChange( pxSocket, eCLOSE_WAIT );

            /* Send RST with the expected sequence and ACK numbers,
             * otherwise the packet will be ignored. */
            pxTCPWindow->ulOurSequenceNumber = FreeRTOS_htonl( pxTCPHeader->ulAckNr );
            pxTCPWindow->rx.ulCurrentSequenceNumber = ulSequenceNumber;

            pxTCPHeader->ucTCPFlags |= tcpTCP_FLAG_RST;

            uxIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength;
            xSendLength = ( BaseType_t ) uxIntermediateResult;

            pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );
        }
        else
        {
            pxTCPWindow->usPeerPortNumber = pxSocket->u.xTCP.usRemotePort;
            pxTCPWindow->usOurPortNumber = pxSocket->usLocalPort;

            if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCONNECT_SYN )
            {
                /* Map the Last packet onto the ProtocolHeader_t struct for easy access to the fields. */
                ProtocolHeaders_t * pxLastHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                            &( pxSocket->u.xTCP.xPacket.u.ucLastPacket[ ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) ] ) );

                /* Clear the SYN flag in lastPacket. */
                pxLastHeaders->xTCPHeader.ucTCPFlags = tcpTCP_FLAG_ACK;
                pxProtocolHeaders->xTCPHeader.ucTCPFlags = tcpTCP_FLAG_ACK;

                /* This socket was the one connecting actively so now perform the
                 * synchronisation. */
                vTCPWindowInit( &pxSocket->u.xTCP.xTCPWindow,
                                ulSequenceNumber, pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber, ( uint32_t ) pxSocket->u.xTCP.usCurMSS );
                pxTCPWindow->rx.ulHighestSequenceNumber = ulSequenceNumber + 1U;
                pxTCPWindow->rx.ulCurrentSequenceNumber = ulSequenceNumber + 1U;
                pxTCPWindow->tx.ulCurrentSequenceNumber++; /* because we send a TCP_SYN [ | TCP_ACK ]; */
                pxTCPWindow->ulNextTxSequenceNumber++;
            }
            else if( ulReceiveLength == 0U )
            {
                pxTCPWindow->rx.ulCurrentSequenceNumber = ulSequenceNumber;
            }
            else
            {
                /* Nothing. */
            }

            /* The SYN+ACK has been confirmed, increase the next sequence number by
             * 1. */
            pxTCPWindow->ulOurSequenceNumber = pxTCPWindow->tx.ulFirstSequenceNumber + 1U;

            #if ( ipconfigUSE_TCP_WIN == 1 )
                {
                    FreeRTOS_debug_printf( ( "TCP: %s %d => %lxip:%d set ESTAB (scaling %u)\n",
                                             ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCONNECT_SYN ) ? "active" : "passive",
                                             pxSocket->usLocalPort,
                                             pxSocket->u.xTCP.ulRemoteIP,
                                             pxSocket->u.xTCP.usRemotePort,
                                             ( unsigned ) pxSocket->u.xTCP.bits.bWinScaling ) );
                }
            #endif /* ipconfigUSE_TCP_WIN */

            if( ( pxSocket->u.xTCP.ucTCPState == ( EventBits_t ) eCONNECT_SYN ) || ( ulReceiveLength != 0UL ) )
            {
                pxTCPHeader->ucTCPFlags = tcpTCP_FLAG_ACK;

                uxIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ( size_t ) ipSIZE_OF_TCP_HEADER + uxOptionsLength;
                xSendLength = ( BaseType_t ) uxIntermediateResult;
                pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );
            }

            #if ( ipconfigUSE_TCP_WIN != 0 )
                {
                    if( pxSocket->u.xTCP.bits.bWinScaling == pdFALSE_UNSIGNED )
                    {
                        /* The other party did not send a scaling factor.
                         * A shifting factor in this side must be canceled. */
                        pxSocket->u.xTCP.ucMyWinScaleFactor = 0;
                        pxSocket->u.xTCP.ucPeerWinScaleFactor = 0;
                    }
                }
            #endif /* ipconfigUSE_TCP_WIN */

            /* This was the third step of connecting: SYN, SYN+ACK, ACK so now the
             * connection is established. */
            vTCPStateChange( pxSocket, eESTABLISHED );
        }

        return xSendLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief prvHandleEstablished(): called from prvTCPHandleState()
 *        Called if the status is eESTABLISHED. Data reception has been handled
 *        earlier. Here the ACK's from peer will be checked, and if a FIN is received,
 *        the code will check if it may be accepted, i.e. if all expected data has been
 *        completely received.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in,out] ppxNetworkBuffer: Pointer to pointer to the network buffer.
 * @param[in] ulReceiveLength: The length of the received packet.
 * @param[in] uxOptionsLength: Length of TCP options.
 *
 * @return The send length of the packet to be sent.
 */
    static BaseType_t prvHandleEstablished( FreeRTOS_Socket_t * pxSocket,
                                            NetworkBufferDescriptor_t ** ppxNetworkBuffer,
                                            uint32_t ulReceiveLength,
                                            UBaseType_t uxOptionsLength )
    {
        /* Map the buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                        &( ( *ppxNetworkBuffer )->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket ) ] ) );
        TCPHeader_t * pxTCPHeader = &pxProtocolHeaders->xTCPHeader;
        TCPWindow_t * pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
        uint8_t ucTCPFlags = pxTCPHeader->ucTCPFlags;
        uint32_t ulSequenceNumber = FreeRTOS_ntohl( pxTCPHeader->ulSequenceNumber ), ulCount, ulIntermediateResult = 0;
        BaseType_t xSendLength = 0, xMayClose = pdFALSE, bRxComplete, bTxDone;
        int32_t lDistance, lSendResult;
        uint16_t usWindow;
        UBaseType_t uxIntermediateResult = 0;

        /* Remember the window size the peer is advertising. */
        usWindow = FreeRTOS_ntohs( pxTCPHeader->usWindow );
        pxSocket->u.xTCP.ulWindowSize = ( uint32_t ) usWindow;
        #if ( ipconfigUSE_TCP_WIN != 0 )
            {
                pxSocket->u.xTCP.ulWindowSize =
                    ( pxSocket->u.xTCP.ulWindowSize << pxSocket->u.xTCP.ucPeerWinScaleFactor );
            }
        #endif /* ipconfigUSE_TCP_WIN */

        if( ( ucTCPFlags & ( uint8_t ) tcpTCP_FLAG_ACK ) != 0U )
        {
            ulCount = ulTCPWindowTxAck( pxTCPWindow, FreeRTOS_ntohl( pxTCPHeader->ulAckNr ) );

            /* ulTCPWindowTxAck() returns the number of bytes which have been acked,
             * starting at 'tx.ulCurrentSequenceNumber'.  Advance the tail pointer in
             * txStream. */
            if( ( pxSocket->u.xTCP.txStream != NULL ) && ( ulCount > 0U ) )
            {
                /* Just advancing the tail index, 'ulCount' bytes have been
                 * confirmed, and because there is new space in the txStream, the
                 * user/owner should be woken up. */
                /* _HT_ : only in case the socket's waiting? */
                if( uxStreamBufferGet( pxSocket->u.xTCP.txStream, 0U, NULL, ( size_t ) ulCount, pdFALSE ) != 0U )
                {
                    pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_SEND;

                    #if ipconfigSUPPORT_SELECT_FUNCTION == 1
                        {
                            if( ( pxSocket->xSelectBits & ( ( EventBits_t ) eSELECT_WRITE ) ) != 0U )
                            {
                                pxSocket->xEventBits |= ( ( EventBits_t ) eSELECT_WRITE ) << SOCKET_EVENT_BIT_COUNT;
                            }
                        }
                    #endif

                    /* In case the socket owner has installed an OnSent handler,
                     * call it now. */
                    #if ( ipconfigUSE_CALLBACKS == 1 )
                        {
                            if( ipconfigIS_VALID_PROG_ADDRESS( pxSocket->u.xTCP.pxHandleSent ) )
                            {
                                pxSocket->u.xTCP.pxHandleSent( ( Socket_t ) pxSocket, ulCount );
                            }
                        }
                    #endif /* ipconfigUSE_CALLBACKS == 1  */
                }
            }
        }

        /* If this socket has a stream for transmission, add the data to the
         * outgoing segment(s). */
        if( pxSocket->u.xTCP.txStream != NULL )
        {
            prvTCPAddTxData( pxSocket );
        }

        pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber = pxTCPWindow->tx.ulCurrentSequenceNumber;

        if( ( pxSocket->u.xTCP.bits.bFinAccepted != pdFALSE_UNSIGNED ) || ( ( ucTCPFlags & ( uint8_t ) tcpTCP_FLAG_FIN ) != 0U ) )
        {
            /* Peer is requesting to stop, see if we're really finished. */
            xMayClose = pdTRUE;

            /* Checks are only necessary if we haven't sent a FIN yet. */
            if( pxSocket->u.xTCP.bits.bFinSent == pdFALSE_UNSIGNED )
            {
                /* xTCPWindowTxDone returns true when all Tx queues are empty. */
                bRxComplete = xTCPWindowRxEmpty( pxTCPWindow );
                bTxDone = xTCPWindowTxDone( pxTCPWindow );

                if( ( bRxComplete == 0 ) || ( bTxDone == 0 ) )
                {
                    /* Refusing FIN: Rx incomplete 1 optlen 4 tx done 1. */
                    FreeRTOS_debug_printf( ( "Refusing FIN[%u,%u]: RxCompl %lu tx done %ld\n",
                                             pxSocket->usLocalPort,
                                             pxSocket->u.xTCP.usRemotePort,
                                             bRxComplete, bTxDone ) );
                    xMayClose = pdFALSE;
                }
                else
                {
                    ulIntermediateResult = ulSequenceNumber + ulReceiveLength - pxTCPWindow->rx.ulCurrentSequenceNumber;
                    lDistance = ( int32_t ) ulIntermediateResult;

                    if( lDistance > 1 )
                    {
                        FreeRTOS_debug_printf( ( "Refusing FIN: Rx not complete %ld (cur %lu high %lu)\n",
                                                 lDistance, pxTCPWindow->rx.ulCurrentSequenceNumber - pxTCPWindow->rx.ulFirstSequenceNumber,
                                                 pxTCPWindow->rx.ulHighestSequenceNumber - pxTCPWindow->rx.ulFirstSequenceNumber ) );

                        xMayClose = pdFALSE;
                    }
                }
            }

            if( xTCPWindowLoggingLevel > 0 )
            {
                FreeRTOS_debug_printf( ( "TCP: FIN received, mayClose = %ld (Rx %lu Len %ld, Tx %lu)\n",
                                         xMayClose, ulSequenceNumber - pxSocket->u.xTCP.xTCPWindow.rx.ulFirstSequenceNumber, ulReceiveLength,
                                         pxTCPWindow->tx.ulCurrentSequenceNumber - pxSocket->u.xTCP.xTCPWindow.tx.ulFirstSequenceNumber ) );
            }

            if( xMayClose != pdFALSE )
            {
                pxSocket->u.xTCP.bits.bFinAccepted = pdTRUE_UNSIGNED;
                xSendLength = prvTCPHandleFin( pxSocket, *ppxNetworkBuffer );
            }
        }

        if( xMayClose == pdFALSE )
        {
            pxTCPHeader->ucTCPFlags = tcpTCP_FLAG_ACK;

            if( ulReceiveLength != 0U )
            {
                uxIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength;
                xSendLength = ( BaseType_t ) uxIntermediateResult;
                /* TCP-offset equals '( ( length / 4 ) << 4 )', resulting in a shift-left 2 */
                pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );

                if( pxSocket->u.xTCP.bits.bFinSent != pdFALSE_UNSIGNED )
                {
                    pxTCPWindow->tx.ulCurrentSequenceNumber = pxTCPWindow->tx.ulFINSequenceNumber;
                }
            }

            /* Now get data to be transmitted. */

            /* _HT_ patch: since the MTU has be fixed at 1500 in stead of 1526, TCP
             * can not send-out both TCP options and also a full packet. Sending
             * options (SACK) is always more urgent than sending data, which can be
             * sent later. */
            if( uxOptionsLength == 0U )
            {
                /* prvTCPPrepareSend might allocate a bigger network buffer, if
                 * necessary. */
                lSendResult = prvTCPPrepareSend( pxSocket, ppxNetworkBuffer, uxOptionsLength );

                if( lSendResult > 0 )
                {
                    xSendLength = ( BaseType_t ) lSendResult;
                }
            }
        }

        return xSendLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Called from prvTCPHandleState(). There is data to be sent. If
 *        ipconfigUSE_TCP_WIN is defined, and if only an ACK must be sent, it will be
 *        checked if it would better be postponed for efficiency.
 *
 * @param[in] pxSocket: The socket owning the TCP connection.
 * @param[in] ppxNetworkBuffer: Pointer to pointer to the network buffer.
 * @param[in] ulReceiveLength: The length of the received buffer.
 * @param[in] xByteCount: Length of the data to be sent.
 *
 * @return The number of bytes actually sent.
 */
    static BaseType_t prvSendData( FreeRTOS_Socket_t * pxSocket,
                                   NetworkBufferDescriptor_t ** ppxNetworkBuffer,
                                   uint32_t ulReceiveLength,
                                   BaseType_t xByteCount )
    {
        /* Map the buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        const ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                              &( ( *ppxNetworkBuffer )->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( *ppxNetworkBuffer ) ] ) );
        const TCPHeader_t * pxTCPHeader = &pxProtocolHeaders->xTCPHeader;
        const TCPWindow_t * pxTCPWindow = &pxSocket->u.xTCP.xTCPWindow;
        /* Find out what window size we may advertised. */
        int32_t lRxSpace;
        BaseType_t xSendLength = xByteCount;
        uint32_t ulRxBufferSpace;

        #if ( ipconfigUSE_TCP_WIN == 1 )
            #if ( ipconfigTCP_ACK_EARLIER_PACKET == 0 )
                const int32_t lMinLength = 0;
            #else
                int32_t lMinLength;
            #endif
        #endif

        /* Set the time-out field, so that we'll be called by the IP-task in case no
         * next message will be received. */
        ulRxBufferSpace = pxSocket->u.xTCP.ulHighestRxAllowed - pxTCPWindow->rx.ulCurrentSequenceNumber;
        lRxSpace = ( int32_t ) ulRxBufferSpace;

        #if ipconfigUSE_TCP_WIN == 1
            {
                #if ( ipconfigTCP_ACK_EARLIER_PACKET != 0 )
                    {
                        lMinLength = ( ( int32_t ) 2 ) * ( ( int32_t ) pxSocket->u.xTCP.usCurMSS );
                    }
                #endif /* ipconfigTCP_ACK_EARLIER_PACKET */

                /* In case we're receiving data continuously, we might postpone sending
                 * an ACK to gain performance. */
                /* lint e9007 is OK because 'uxIPHeaderSizeSocket()' has no side-effects. */
                if( ( ulReceiveLength > 0U ) &&                                                   /* Data was sent to this socket. */
                    ( lRxSpace >= lMinLength ) &&                                                 /* There is Rx space for more data. */
                    ( pxSocket->u.xTCP.bits.bFinSent == pdFALSE_UNSIGNED ) &&                     /* Not in a closure phase. */
                    ( xSendLength == uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER ) && /* No Tx data or options to be sent. */
                    ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eESTABLISHED ) &&                /* Connection established. */
                    ( pxTCPHeader->ucTCPFlags == tcpTCP_FLAG_ACK ) )                              /* There are no other flags than an ACK. */
                {
                    if( pxSocket->u.xTCP.pxAckMessage != *ppxNetworkBuffer )
                    {
                        /* There was still a delayed in queue, delete it. */
                        if( pxSocket->u.xTCP.pxAckMessage != NULL )
                        {
                            vReleaseNetworkBufferAndDescriptor( pxSocket->u.xTCP.pxAckMessage );
                        }

                        pxSocket->u.xTCP.pxAckMessage = *ppxNetworkBuffer;
                    }

                    if( ( ulReceiveLength < ( uint32_t ) pxSocket->u.xTCP.usCurMSS ) ||            /* Received a small message. */
                        ( lRxSpace < ipNUMERIC_CAST( int32_t, 2U * pxSocket->u.xTCP.usCurMSS ) ) ) /* There are less than 2 x MSS space in the Rx buffer. */
                    {
                        pxSocket->u.xTCP.usTimeout = ( uint16_t ) tcpDELAYED_ACK_SHORT_DELAY_MS;
                    }
                    else
                    {
                        /* Normally a delayed ACK should wait 200 ms for a next incoming
                         * packet.  Only wait 20 ms here to gain performance.  A slow ACK
                         * for full-size message. */
                        pxSocket->u.xTCP.usTimeout = ( uint16_t ) ipMS_TO_MIN_TICKS( tcpDELAYED_ACK_LONGER_DELAY_MS );
                    }

                    if( ( xTCPWindowLoggingLevel > 1 ) && ( ipconfigTCP_MAY_LOG_PORT( pxSocket->usLocalPort ) ) )
                    {
                        FreeRTOS_debug_printf( ( "Send[%u->%u] del ACK %lu SEQ %lu (len %lu) tmout %u d %lu\n",
                                                 pxSocket->usLocalPort,
                                                 pxSocket->u.xTCP.usRemotePort,
                                                 pxTCPWindow->rx.ulCurrentSequenceNumber - pxTCPWindow->rx.ulFirstSequenceNumber,
                                                 pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber,
                                                 xSendLength,
                                                 pxSocket->u.xTCP.usTimeout, lRxSpace ) );
                    }

                    *ppxNetworkBuffer = NULL;
                    xSendLength = 0;
                }
                else if( pxSocket->u.xTCP.pxAckMessage != NULL )
                {
                    /* As an ACK is not being delayed, remove any earlier delayed ACK
                     * message. */
                    if( pxSocket->u.xTCP.pxAckMessage != *ppxNetworkBuffer )
                    {
                        vReleaseNetworkBufferAndDescriptor( pxSocket->u.xTCP.pxAckMessage );
                    }

                    pxSocket->u.xTCP.pxAckMessage = NULL;
                }
                else
                {
                    /* The ack will not be postponed, and there was no stored ack ( in 'pxAckMessage' ). */
                }
            }
        #else /* if ipconfigUSE_TCP_WIN == 1 */
            {
                /* Remove compiler warnings. */
                ( void ) ulReceiveLength;
                ( void ) pxTCPHeader;
                ( void ) lRxSpace;
            }
        #endif /* ipconfigUSE_TCP_WIN */

        if( xSendLength != 0 )
        {
            if( ( xTCPWindowLoggingLevel > 1 ) && ( ipconfigTCP_MAY_LOG_PORT( pxSocket->usLocalPort ) ) )
            {
                FreeRTOS_debug_printf( ( "Send[%u->%u] imm ACK %lu SEQ %lu (len %lu)\n",
                                         pxSocket->usLocalPort,
                                         pxSocket->u.xTCP.usRemotePort,
                                         pxTCPWindow->rx.ulCurrentSequenceNumber - pxTCPWindow->rx.ulFirstSequenceNumber,
                                         pxTCPWindow->ulOurSequenceNumber - pxTCPWindow->tx.ulFirstSequenceNumber,
                                         xSendLength ) );
            }

            /* Set the parameter 'xReleaseAfterSend' to the value of
             * ipconfigZERO_COPY_TX_DRIVER. */
            prvTCPReturnPacket( pxSocket, *ppxNetworkBuffer, ( uint32_t ) xSendLength, ipconfigZERO_COPY_TX_DRIVER );
            #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
                {
                    /* The driver has taken ownership of the Network Buffer. */
                    *ppxNetworkBuffer = NULL;
                }
            #endif
        }

        return xSendLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Check incoming packets for valid data and handle the state of the
 *        TCP connection and respond according to the situation.
 *
 * @param[in] pxSocket: The socket whose connection state is being handled.
 * @param[in] ppxNetworkBuffer: The network buffer descriptor holding the
 *            packet received from the peer.
 *
 * @return If the data is correct and some packet was sent to the peer, then
 *         the number of bytes sent is returned, or else a negative value is
 *         returned indicating an error.
 *
 * @note prvTCPHandleState() is the most important function of this TCP stack
 * We've tried to keep it (relatively short) by putting a lot of code in
 * the static functions above:
 *
 *      prvCheckRxData()
 *      prvStoreRxData()
 *      prvSetOptions()
 *      prvHandleSynReceived()
 *      prvHandleEstablished()
 *      prvSendData()
 *
 * As these functions are declared static, and they're called from one location
 * only, most compilers will inline them, thus avoiding a call and return.
 */
    static BaseType_t prvTCPHandleState( FreeRTOS_Socket_t * pxSocket,
                                         NetworkBufferDescriptor_t ** ppxNetworkBuffer )
    {
        /* Map the buffer onto the ProtocolHeader_t struct for easy access to the fields. */
        ProtocolHeaders_t * pxProtocolHeaders = ipCAST_PTR_TO_TYPE_PTR( ProtocolHeaders_t,
                                                                        &( ( *ppxNetworkBuffer )->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( *ppxNetworkBuffer ) ] ) );
        TCPHeader_t * pxTCPHeader = &( pxProtocolHeaders->xTCPHeader );
        BaseType_t xSendLength = 0;
        uint32_t ulReceiveLength; /* Number of bytes contained in the TCP message. */
        uint8_t * pucRecvData;
        uint32_t ulSequenceNumber = FreeRTOS_ntohl( pxTCPHeader->ulSequenceNumber );

        /* uxOptionsLength: the size of the options to be sent (always a multiple of
         * 4 bytes)
         * 1. in the SYN phase, we shall communicate the MSS
         * 2. in case of a SACK, Selective ACK, ack a segment which comes in
         * out-of-order. */
        UBaseType_t uxOptionsLength = 0U;
        uint8_t ucTCPFlags = pxTCPHeader->ucTCPFlags;
        TCPWindow_t * pxTCPWindow = &( pxSocket->u.xTCP.xTCPWindow );
        UBaseType_t uxIntermediateResult = 0;

        /* First get the length and the position of the received data, if any.
         * pucRecvData will point to the first byte of the TCP payload. */
        ulReceiveLength = ( uint32_t ) prvCheckRxData( *ppxNetworkBuffer, &pucRecvData );

        if( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED )
        {
            if( pxTCPWindow->rx.ulCurrentSequenceNumber == ( ulSequenceNumber + 1UL ) )
            {
                /* This is most probably a keep-alive message from peer.  Setting
                 * 'bWinChange' doesn't cause a window-size-change, the flag is used
                 * here to force sending an immediate ACK. */
                pxSocket->u.xTCP.bits.bWinChange = pdTRUE_UNSIGNED;
            }
        }

        /* Keep track of the highest sequence number that might be expected within
         * this connection. */
        if( ( ( int32_t ) ( ulSequenceNumber + ulReceiveLength - pxTCPWindow->rx.ulHighestSequenceNumber ) ) > 0L )
        {
            pxTCPWindow->rx.ulHighestSequenceNumber = ulSequenceNumber + ulReceiveLength;
        }

        /* Storing data may result in a fatal error if malloc() fails. */
        if( prvStoreRxData( pxSocket, pucRecvData, *ppxNetworkBuffer, ulReceiveLength ) < 0 )
        {
            xSendLength = -1;
        }
        else
        {
            uxOptionsLength = prvSetOptions( pxSocket, *ppxNetworkBuffer );

            if( ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eSYN_RECEIVED ) && ( ( ucTCPFlags & ( uint8_t ) tcpTCP_FLAG_CTRL ) == ( uint8_t ) tcpTCP_FLAG_SYN ) )
            {
                FreeRTOS_debug_printf( ( "eSYN_RECEIVED: ACK expected, not SYN: peer missed our SYN+ACK\n" ) );

                /* In eSYN_RECEIVED a simple ACK is expected, but apparently the
                 * 'SYN+ACK' didn't arrive.  Step back to the previous state in which
                 * a first incoming SYN is handled.  The SYN was counted already so
                 * decrease it first. */
                vTCPStateChange( pxSocket, eSYN_FIRST );
            }

            if( ( ( ucTCPFlags & tcpTCP_FLAG_FIN ) != 0U ) && ( pxSocket->u.xTCP.bits.bFinRecv == pdFALSE_UNSIGNED ) )
            {
                /* It's the first time a FIN has been received, remember its
                 * sequence number. */
                pxTCPWindow->rx.ulFINSequenceNumber = ulSequenceNumber + ulReceiveLength;
                pxSocket->u.xTCP.bits.bFinRecv = pdTRUE_UNSIGNED;

                /* Was peer the first one to send a FIN? */
                if( pxSocket->u.xTCP.bits.bFinSent == pdFALSE_UNSIGNED )
                {
                    /* If so, don't send the-last-ACK. */
                    pxSocket->u.xTCP.bits.bFinLast = pdTRUE_UNSIGNED;
                }
            }

            switch( ipNUMERIC_CAST( eIPTCPState_t, pxSocket->u.xTCP.ucTCPState ) )
            {
                case eCLOSED: /* (server + client) no connection state at all. */

                    /* Nothing to do for a closed socket, except waiting for the
                     * owner. */
                    break;

                case eTCP_LISTEN: /* (server) waiting for a connection request from
                                   * any remote TCP and port. */

                    /* The listen state was handled in xProcessReceivedTCPPacket().
                     * Should not come here. */
                    break;

                case eSYN_FIRST: /* (server) Just received a SYN request for a server
                                  * socket. */

                    /* A new socket has been created, reply with a SYN+ACK.
                     * Acknowledge with seq+1 because the SYN is seen as pseudo data
                     * with len = 1. */
                    uxOptionsLength = prvSetSynAckOptions( pxSocket, pxTCPHeader );
                    pxTCPHeader->ucTCPFlags = ( uint8_t ) tcpTCP_FLAG_SYN | ( uint8_t ) tcpTCP_FLAG_ACK;

                    uxIntermediateResult = uxIPHeaderSizeSocket( pxSocket ) + ipSIZE_OF_TCP_HEADER + uxOptionsLength;
                    xSendLength = ( BaseType_t ) uxIntermediateResult;

                    /* Set the TCP offset field:  ipSIZE_OF_TCP_HEADER equals 20 and
                     * uxOptionsLength is a multiple of 4.  The complete expression is:
                     * ucTCPOffset = ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) / 4 ) << 4 */
                    pxTCPHeader->ucTCPOffset = ( uint8_t ) ( ( ipSIZE_OF_TCP_HEADER + uxOptionsLength ) << 2 );
                    vTCPStateChange( pxSocket, eSYN_RECEIVED );

                    pxTCPWindow->rx.ulHighestSequenceNumber = ulSequenceNumber + 1UL;
                    pxTCPWindow->rx.ulCurrentSequenceNumber = ulSequenceNumber + 1UL;
                    pxTCPWindow->ulNextTxSequenceNumber = pxTCPWindow->tx.ulFirstSequenceNumber + 1UL;
                    pxTCPWindow->tx.ulCurrentSequenceNumber = pxTCPWindow->tx.ulFirstSequenceNumber + 1UL; /* because we send a TCP_SYN. */
                    break;

                case eCONNECT_SYN:  /* (client) also called SYN_SENT: we've just send a
                                     * SYN, expect a SYN+ACK and send a ACK now. */
                /* Fall through */
                case eSYN_RECEIVED: /* (server) we've had a SYN, replied with SYN+SCK
                                     * expect a ACK and do nothing. */
                    xSendLength = prvHandleSynReceived( pxSocket, *( ppxNetworkBuffer ), ulReceiveLength, uxOptionsLength );
                    break;

                case eESTABLISHED: /* (server + client) an open connection, data
                                    * received can be delivered to the user. The normal
                                    * state for the data transfer phase of the connection
                                    * The closing states are also handled here with the
                                    * use of some flags. */
                    xSendLength = prvHandleEstablished( pxSocket, ppxNetworkBuffer, ulReceiveLength, uxOptionsLength );
                    break;

                case eLAST_ACK:   /* (server + client) waiting for an acknowledgement
                                   * of the connection termination request previously
                                   * sent to the remote TCP (which includes an
                                   * acknowledgement of its connection termination
                                   * request). */
                /* Fall through */
                case eFIN_WAIT_1: /* (server + client) waiting for a connection termination request from the remote TCP,
                                   * or an acknowledgement of the connection termination request previously sent. */
                /* Fall through */
                case eFIN_WAIT_2: /* (server + client) waiting for a connection termination request from the remote TCP. */
                    xSendLength = prvTCPHandleFin( pxSocket, *ppxNetworkBuffer );
                    break;

                case eCLOSE_WAIT: /* (server + client) waiting for a connection
                                   * termination request from the local user.  Nothing to
                                   * do, connection is closed, wait for owner to close
                                   * this socket. */
                    break;

                case eCLOSING: /* (server + client) waiting for a connection
                                * termination request acknowledgement from the remote
                                * TCP. */
                    break;

                case eTIME_WAIT: /* (either server or client) waiting for enough time
                                  * to pass to be sure the remote TCP received the
                                  * acknowledgement of its connection termination
                                  * request. [According to RFC 793 a connection can stay
                                  * in TIME-WAIT for a maximum of four minutes known as
                                  * a MSL (maximum segment lifetime).]  These states are
                                  * implemented implicitly by settings flags like
                                  * 'bFinSent', 'bFinRecv', and 'bFinAcked'. */
                    break;

                default:
                    /* No more known states. */
                    break;
            }
        }

        if( xSendLength > 0 )
        {
            xSendLength = prvSendData( pxSocket, ppxNetworkBuffer, ulReceiveLength, xSendLength );
        }

        return xSendLength;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Common code for sending a TCP protocol control packet (i.e. no options, no
 *        payload, just flags).
 *
 * @param[in] pxNetworkBuffer: The network buffer received from the peer.
 * @param[in] ucTCPFlags: The flags to determine what kind of packet this is.
 *
 * @return pdFAIL always indicating that the packet was not consumed.
 */
    static BaseType_t prvTCPSendSpecialPacketHelper( NetworkBufferDescriptor_t * pxNetworkBuffer,
                                                     uint8_t ucTCPFlags )
    {
        #if ( ipconfigIGNORE_UNKNOWN_PACKETS == 1 )
            /* Configured to ignore unknown packets just suppress a compiler warning. */
            ( void ) pxNetworkBuffer;
            ( void ) ucTCPFlags;
        #else
            {
                /* Map the ethernet buffer onto the TCPPacket_t struct for easy access to the fields. */
                TCPPacket_t * pxTCPPacket = ipCAST_PTR_TO_TYPE_PTR( TCPPacket_t, pxNetworkBuffer->pucEthernetBuffer );
                const uint32_t ulSendLength =
                    ( ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_TCP_HEADER ); /* Plus 0 options. */

                pxTCPPacket->xTCPHeader.ucTCPFlags = ucTCPFlags;
                pxTCPPacket->xTCPHeader.ucTCPOffset = ( ipSIZE_OF_TCP_HEADER ) << 2;

                prvTCPReturnPacket( NULL, pxNetworkBuffer, ulSendLength, pdFALSE );
            }
        #endif /* !ipconfigIGNORE_UNKNOWN_PACKETS */

        /* The packet was not consumed. */
        return pdFAIL;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief A "challenge ACK" is as per https://tools.ietf.org/html/rfc5961#section-3.2,
 *        case #3. In summary, an RST was received with a sequence number that is
 *        unexpected but still within the window.
 *
 * @param[in] pxNetworkBuffer: The network buffer descriptor with the packet.
 *
 * @return Returns the value back from #prvTCPSendSpecialPacketHelper.
 */
    static BaseType_t prvTCPSendChallengeAck( NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        return prvTCPSendSpecialPacketHelper( pxNetworkBuffer, tcpTCP_FLAG_ACK );
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Send a RST (Reset) to peer in case the packet cannot be handled.
 *
 * @param[in] pxNetworkBuffer: The network buffer descriptor with the packet.
 *
 * @return Returns the value back from #prvTCPSendSpecialPacketHelper.
 */
    static BaseType_t prvTCPSendReset( NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        return prvTCPSendSpecialPacketHelper( pxNetworkBuffer,
                                              ( uint8_t ) tcpTCP_FLAG_ACK | ( uint8_t ) tcpTCP_FLAG_RST );
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Set the MSS (Maximum segment size) associated with the given socket.
 *
 * @param[in] pxSocket: The socket whose MSS is to be set.
 */
    static void prvSocketSetMSS( FreeRTOS_Socket_t * pxSocket )
    {
        uint32_t ulMSS = ipconfigTCP_MSS;

        if( ( ( FreeRTOS_ntohl( pxSocket->u.xTCP.ulRemoteIP ) ^ *ipLOCAL_IP_ADDRESS_POINTER ) & xNetworkAddressing.ulNetMask ) != 0UL )
        {
            /* Data for this peer will pass through a router, and maybe through
             * the internet.  Limit the MSS to 1400 bytes or less. */
            ulMSS = FreeRTOS_min_uint32( ( uint32_t ) tcpREDUCED_MSS_THROUGH_INTERNET, ulMSS );
        }

        FreeRTOS_debug_printf( ( "prvSocketSetMSS: %lu bytes for %lxip:%u\n", ulMSS, pxSocket->u.xTCP.ulRemoteIP, pxSocket->u.xTCP.usRemotePort ) );

        pxSocket->u.xTCP.usInitMSS = ( uint16_t ) ulMSS;
        pxSocket->u.xTCP.usCurMSS = ( uint16_t ) ulMSS;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Process the received TCP packet.
 *
 * @param[in] pxDescriptor: The descriptor in which the TCP packet is held.
 *
 * @return If the processing of the packet was successful, then pdPASS is returned
 *         or else pdFAIL.
 *
 * @note FreeRTOS_TCP_IP has only 2 public functions, this is the second one:
 *  xProcessReceivedTCPPacket()
 *      prvTCPHandleState()
 *          prvTCPPrepareSend()
 *              prvTCPReturnPacket()
 *              xNetworkInterfaceOutput()  // Sends data to the NIC
 *      prvTCPSendRepeated()
 *          prvTCPReturnPacket()        // Prepare for returning
 *          xNetworkInterfaceOutput()   // Sends data to the NIC
 */
    BaseType_t xProcessReceivedTCPPacket( NetworkBufferDescriptor_t * pxDescriptor )
    {
        /* Function might modify the parameter. */
        NetworkBufferDescriptor_t * pxNetworkBuffer = pxDescriptor;

        /* Map the buffer onto a ProtocolHeaders_t struct for easy access to the fields. */
        const ProtocolHeaders_t * pxProtocolHeaders = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( ProtocolHeaders_t,
                                                                                          &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) ] ) );
        FreeRTOS_Socket_t * pxSocket;
        uint16_t ucTCPFlags = pxProtocolHeaders->xTCPHeader.ucTCPFlags;
        uint32_t ulLocalIP;
        uint16_t xLocalPort = FreeRTOS_htons( pxProtocolHeaders->xTCPHeader.usDestinationPort );
        uint16_t xRemotePort = FreeRTOS_htons( pxProtocolHeaders->xTCPHeader.usSourcePort );
        uint32_t ulRemoteIP;
        uint32_t ulSequenceNumber = FreeRTOS_ntohl( pxProtocolHeaders->xTCPHeader.ulSequenceNumber );
        uint32_t ulAckNumber = FreeRTOS_ntohl( pxProtocolHeaders->xTCPHeader.ulAckNr );
        BaseType_t xResult = pdPASS;

        configASSERT( pxNetworkBuffer != NULL );
        configASSERT( pxNetworkBuffer->pucEthernetBuffer != NULL );
        const IPHeader_t * pxIPHeader;

        /* Check for a minimum packet size. */
        if( pxNetworkBuffer->xDataLength < ( ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) + ipSIZE_OF_TCP_HEADER ) )
        {
            xResult = pdFAIL;
        }
        else
        {
            /* Map the ethernet buffer onto the IPHeader_t struct for easy access to the fields. */
            pxIPHeader = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( IPHeader_t, &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER ] ) );
            ulLocalIP = FreeRTOS_htonl( pxIPHeader->ulDestinationIPAddress );
            ulRemoteIP = FreeRTOS_htonl( pxIPHeader->ulSourceIPAddress );

            /* Find the destination socket, and if not found: return a socket listing to
             * the destination PORT. */
            pxSocket = ( FreeRTOS_Socket_t * ) pxTCPSocketLookup( ulLocalIP, xLocalPort, ulRemoteIP, xRemotePort );

            if( ( pxSocket == NULL ) || ( prvTCPSocketIsActive( ipNUMERIC_CAST( eIPTCPState_t, pxSocket->u.xTCP.ucTCPState ) ) == pdFALSE ) )
            {
                /* A TCP messages is received but either there is no socket with the
                 * given port number or the there is a socket, but it is in one of these
                 * non-active states:  eCLOSED, eCLOSE_WAIT, eFIN_WAIT_2, eCLOSING, or
                 * eTIME_WAIT. */

                FreeRTOS_debug_printf( ( "TCP: No active socket on port %d (%lxip:%d)\n", xLocalPort, ulRemoteIP, xRemotePort ) );

                /* Send a RST to all packets that can not be handled.  As a result
                 * the other party will get a ECONN error.  There are two exceptions:
                 * 1) A packet that already has the RST flag set.
                 * 2) A packet that only has the ACK flag set.
                 * A packet with only the ACK flag set might be the last ACK in
                 * a three-way hand-shake that closes a connection. */
                if( ( ( ucTCPFlags & tcpTCP_FLAG_CTRL ) != tcpTCP_FLAG_ACK ) &&
                    ( ( ucTCPFlags & tcpTCP_FLAG_RST ) == 0U ) )
                {
                    ( void ) prvTCPSendReset( pxNetworkBuffer );
                }

                /* The packet can't be handled. */
                xResult = pdFAIL;
            }
            else
            {
                pxSocket->u.xTCP.ucRepCount = 0U;

                if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eTCP_LISTEN )
                {
                    /* The matching socket is in a listening state.  Test if the peer
                     * has set the SYN flag. */
                    if( ( ucTCPFlags & tcpTCP_FLAG_CTRL ) != tcpTCP_FLAG_SYN )
                    {
                        /* What happens: maybe after a reboot, a client doesn't know the
                         * connection had gone.  Send a RST in order to get a new connect
                         * request. */
                        #if ( ipconfigHAS_DEBUG_PRINTF == 1 )
                            {
                                FreeRTOS_debug_printf( ( "TCP: Server can't handle flags: %s from %lxip:%u to port %u\n",
                                                         prvTCPFlagMeaning( ( UBaseType_t ) ucTCPFlags ), ulRemoteIP, xRemotePort, xLocalPort ) );
                            }
                        #endif /* ipconfigHAS_DEBUG_PRINTF */

                        if( ( ucTCPFlags & tcpTCP_FLAG_RST ) == 0U )
                        {
                            ( void ) prvTCPSendReset( pxNetworkBuffer );
                        }

                        xResult = pdFAIL;
                    }
                    else
                    {
                        /* prvHandleListen() will either return a newly created socket
                         * (if bReuseSocket is false), otherwise it returns the current
                         * socket which will later get connected. */
                        pxSocket = prvHandleListen( pxSocket, pxNetworkBuffer );

                        if( pxSocket == NULL )
                        {
                            xResult = pdFAIL;
                        }
                    }
                } /* if( pxSocket->u.xTCP.ucTCPState == eTCP_LISTEN ). */
                else
                {
                    /* This is not a socket in listening mode. Check for the RST
                     * flag. */
                    if( ( ucTCPFlags & tcpTCP_FLAG_RST ) != 0U )
                    {
                        FreeRTOS_debug_printf( ( "TCP: RST received from %lxip:%u for %u\n", ulRemoteIP, xRemotePort, xLocalPort ) );

                        /* Implement https://tools.ietf.org/html/rfc5961#section-3.2. */
                        if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCONNECT_SYN )
                        {
                            /* Per the above RFC, "In the SYN-SENT state ... the RST is
                             * acceptable if the ACK field acknowledges the SYN." */
                            if( ulAckNumber == ( pxSocket->u.xTCP.xTCPWindow.ulOurSequenceNumber + 1UL ) )
                            {
                                vTCPStateChange( pxSocket, eCLOSED );
                            }
                        }
                        else
                        {
                            /* Check whether the packet matches the next expected sequence number. */
                            if( ulSequenceNumber == pxSocket->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber )
                            {
                                vTCPStateChange( pxSocket, eCLOSED );
                            }
                            /* Otherwise, check whether the packet is within the receive window. */
                            else if( ( ulSequenceNumber > pxSocket->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber ) &&
                                     ( ulSequenceNumber < ( pxSocket->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber +
                                                            pxSocket->u.xTCP.xTCPWindow.xSize.ulRxWindowLength ) ) )
                            {
                                /* Send a challenge ACK. */
                                ( void ) prvTCPSendChallengeAck( pxNetworkBuffer );
                            }
                            else
                            {
                                /* Nothing. */
                            }
                        }

                        /* Otherwise, do nothing. In any case, the packet cannot be handled. */
                        xResult = pdFAIL;
                    }
                    else if( ( ( ucTCPFlags & tcpTCP_FLAG_CTRL ) == tcpTCP_FLAG_SYN ) && ( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED ) )
                    {
                        /* SYN flag while this socket is already connected. */
                        FreeRTOS_debug_printf( ( "TCP: SYN unexpected from %lxip:%u\n", ulRemoteIP, xRemotePort ) );

                        /* The packet cannot be handled. */
                        xResult = pdFAIL;
                    }
                    else
                    {
                        /* Update the copy of the TCP header only (skipping eth and IP
                         * headers).  It might be used later on, whenever data must be sent
                         * to the peer. */
                        const size_t uxOffset = ipSIZE_OF_ETH_HEADER + uxIPHeaderSizeSocket( pxSocket );
                        ( void ) memcpy( ( void * ) ( &( pxSocket->u.xTCP.xPacket.u.ucLastPacket[ uxOffset ] ) ),
                                         ( const void * ) ( &( pxNetworkBuffer->pucEthernetBuffer[ uxOffset ] ) ),
                                         ipSIZE_OF_TCP_HEADER );
                    }
                }
            }

            if( xResult != pdFAIL )
            {
                uint16_t usWindow;

                /* pxSocket is not NULL when xResult != pdFAIL. */
                configASSERT( pxSocket != NULL );

                /* Touch the alive timers because we received a message for this
                 * socket. */
                prvTCPTouchSocket( pxSocket );

                /* Parse the TCP option(s), if present. */

                /* _HT_ : if we're in the SYN phase, and peer does not send a MSS option,
                 * then we MUST assume an MSS size of 536 bytes for backward compatibility. */

                /* When there are no TCP options, the TCP offset equals 20 bytes, which is stored as
                 * the number 5 (words) in the higher nibble of the TCP-offset byte. */
                if( ( pxProtocolHeaders->xTCPHeader.ucTCPOffset & tcpTCP_OFFSET_LENGTH_BITS ) > tcpTCP_OFFSET_STANDARD_LENGTH )
                {
                    prvCheckOptions( pxSocket, pxNetworkBuffer );
                }

                usWindow = FreeRTOS_ntohs( pxProtocolHeaders->xTCPHeader.usWindow );
                pxSocket->u.xTCP.ulWindowSize = ( uint32_t ) usWindow;
                #if ( ipconfigUSE_TCP_WIN == 1 )
                    {
                        /* rfc1323 : The Window field in a SYN (i.e., a <SYN> or <SYN,ACK>)
                         * segment itself is never scaled. */
                        if( ( ucTCPFlags & ( uint8_t ) tcpTCP_FLAG_SYN ) == 0U )
                        {
                            pxSocket->u.xTCP.ulWindowSize =
                                ( pxSocket->u.xTCP.ulWindowSize << pxSocket->u.xTCP.ucPeerWinScaleFactor );
                        }
                    }
                #endif /* ipconfigUSE_TCP_WIN */

                /* In prvTCPHandleState() the incoming messages will be handled
                 * depending on the current state of the connection. */
                if( prvTCPHandleState( pxSocket, &pxNetworkBuffer ) > 0 )
                {
                    /* prvTCPHandleState() has sent a message, see if there are more to
                     * be transmitted. */
                    #if ( ipconfigUSE_TCP_WIN == 1 )
                        {
                            ( void ) prvTCPSendRepeated( pxSocket, &pxNetworkBuffer );
                        }
                    #endif /* ipconfigUSE_TCP_WIN */
                }

                if( pxNetworkBuffer != NULL )
                {
                    /* We must check if the buffer is unequal to NULL, because the
                     * socket might keep a reference to it in case a delayed ACK must be
                     * sent. */
                    vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                    #ifndef _lint
                        /* Clear pointers that are freed. */
                        pxNetworkBuffer = NULL;
                    #endif
                }

                /* And finally, calculate when this socket wants to be woken up. */
                ( void ) prvTCPNextTimeout( pxSocket );
                /* Return pdPASS to tell that the network buffer is 'consumed'. */
                xResult = pdPASS;
            }
        }

        /* pdPASS being returned means the buffer has been consumed. */
        return xResult;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Handle 'listen' event on the given socket.
 *
 * @param[in] pxSocket: The socket on which the listen occurred.
 * @param[in] pxNetworkBuffer: The network buffer carrying the packet.
 *
 * @return If a new socket/duplicate socket is created, then the pointer to
 *         that socket is returned or else, a NULL pointer is returned.
 */
    static FreeRTOS_Socket_t * prvHandleListen( FreeRTOS_Socket_t * pxSocket,
                                                NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        /* Map the ethernet buffer onto a TCPPacket_t struct for easy access to the fields. */
        const TCPPacket_t * pxTCPPacket = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( TCPPacket_t, pxNetworkBuffer->pucEthernetBuffer );
        FreeRTOS_Socket_t * pxReturn = NULL;
        uint32_t ulInitialSequenceNumber;

        /* Assume that a new Initial Sequence Number will be required. Request
         * it now in order to fail out if necessary. */
        ulInitialSequenceNumber = ulApplicationGetNextSequenceNumber( *ipLOCAL_IP_ADDRESS_POINTER,
                                                                      pxSocket->usLocalPort,
                                                                      pxTCPPacket->xIPHeader.ulSourceIPAddress,
                                                                      pxTCPPacket->xTCPHeader.usSourcePort );

        /* A pure SYN (without ACK) has come in, create a new socket to answer
         * it. */
        if( ulInitialSequenceNumber != 0UL )
        {
            if( pxSocket->u.xTCP.bits.bReuseSocket != pdFALSE_UNSIGNED )
            {
                /* The flag bReuseSocket indicates that the same instance of the
                 * listening socket should be used for the connection. */
                pxReturn = pxSocket;
                pxSocket->u.xTCP.bits.bPassQueued = pdTRUE_UNSIGNED;
                pxSocket->u.xTCP.pxPeerSocket = pxSocket;
            }
            else
            {
                /* The socket does not have the bReuseSocket flag set meaning create a
                 * new socket when a connection comes in. */
                pxReturn = NULL;

                if( pxSocket->u.xTCP.usChildCount >= pxSocket->u.xTCP.usBacklog )
                {
                    FreeRTOS_printf( ( "Check: Socket %u already has %u / %u child%s\n",
                                       pxSocket->usLocalPort,
                                       pxSocket->u.xTCP.usChildCount,
                                       pxSocket->u.xTCP.usBacklog,
                                       ( pxSocket->u.xTCP.usChildCount == 1U ) ? "" : "ren" ) );
                    ( void ) prvTCPSendReset( pxNetworkBuffer );
                }
                else
                {
                    FreeRTOS_Socket_t * pxNewSocket = ( FreeRTOS_Socket_t * )
                                                      FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );

                    if( ( pxNewSocket == NULL ) || ( pxNewSocket == FREERTOS_INVALID_SOCKET ) )
                    {
                        FreeRTOS_debug_printf( ( "TCP: Listen: new socket failed\n" ) );
                        ( void ) prvTCPSendReset( pxNetworkBuffer );
                    }
                    else if( prvTCPSocketCopy( pxNewSocket, pxSocket ) != pdFALSE )
                    {
                        /* The socket will be connected immediately, no time for the
                         * owner to setsockopt's, therefore copy properties of the server
                         * socket to the new socket.  Only the binding might fail (due to
                         * lack of resources). */
                        pxReturn = pxNewSocket;
                    }
                    else
                    {
                        /* Copying failed somehow. */
                    }
                }
            }
        }

        if( ( ulInitialSequenceNumber != 0U ) && ( pxReturn != NULL ) )
        {
            /* Map the byte stream onto the ProtocolHeaders_t for easy access to the fields. */
            const ProtocolHeaders_t * pxProtocolHeaders = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( ProtocolHeaders_t,
                                                                                              &( pxNetworkBuffer->pucEthernetBuffer[ ipSIZE_OF_ETH_HEADER + xIPHeaderSize( pxNetworkBuffer ) ] ) );

            pxReturn->u.xTCP.usRemotePort = FreeRTOS_htons( pxTCPPacket->xTCPHeader.usSourcePort );
            pxReturn->u.xTCP.ulRemoteIP = FreeRTOS_htonl( pxTCPPacket->xIPHeader.ulSourceIPAddress );
            pxReturn->u.xTCP.xTCPWindow.ulOurSequenceNumber = ulInitialSequenceNumber;

            /* Here is the SYN action. */
            pxReturn->u.xTCP.xTCPWindow.rx.ulCurrentSequenceNumber = FreeRTOS_ntohl( pxProtocolHeaders->xTCPHeader.ulSequenceNumber );
            prvSocketSetMSS( pxReturn );

            prvTCPCreateWindow( pxReturn );

            vTCPStateChange( pxReturn, eSYN_FIRST );

            /* Make a copy of the header up to the TCP header.  It is needed later
             * on, whenever data must be sent to the peer. */
            ( void ) memcpy( ( void * ) pxReturn->u.xTCP.xPacket.u.ucLastPacket,
                             ( const void * ) pxNetworkBuffer->pucEthernetBuffer,
                             sizeof( pxReturn->u.xTCP.xPacket.u.ucLastPacket ) );
        }

        return pxReturn;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Duplicates a socket after a listening socket receives a connection and bind
 *        the new socket to the same port as the listening socket.
 *        Also, let the new socket inherit all properties from the listening socket.
 *
 * @param[in] pxNewSocket: Pointer to the new socket.
 * @param[in] pxSocket: Pointer to the socket being duplicated.
 *
 * @return If all steps all successful, then pdTRUE is returned. Else, pdFALSE.
 */
    static BaseType_t prvTCPSocketCopy( FreeRTOS_Socket_t * pxNewSocket,
                                        FreeRTOS_Socket_t * pxSocket )
    {
        struct freertos_sockaddr xAddress;
        BaseType_t xResult;

        pxNewSocket->xReceiveBlockTime = pxSocket->xReceiveBlockTime;
        pxNewSocket->xSendBlockTime = pxSocket->xSendBlockTime;
        pxNewSocket->ucSocketOptions = pxSocket->ucSocketOptions;
        pxNewSocket->u.xTCP.uxRxStreamSize = pxSocket->u.xTCP.uxRxStreamSize;
        pxNewSocket->u.xTCP.uxTxStreamSize = pxSocket->u.xTCP.uxTxStreamSize;
        pxNewSocket->u.xTCP.uxLittleSpace = pxSocket->u.xTCP.uxLittleSpace;
        pxNewSocket->u.xTCP.uxEnoughSpace = pxSocket->u.xTCP.uxEnoughSpace;
        pxNewSocket->u.xTCP.uxRxWinSize = pxSocket->u.xTCP.uxRxWinSize;
        pxNewSocket->u.xTCP.uxTxWinSize = pxSocket->u.xTCP.uxTxWinSize;

        #if ( ipconfigSOCKET_HAS_USER_SEMAPHORE == 1 )
            {
                pxNewSocket->pxUserSemaphore = pxSocket->pxUserSemaphore;
            }
        #endif /* ipconfigSOCKET_HAS_USER_SEMAPHORE */

        #if ( ipconfigUSE_CALLBACKS == 1 )
            {
                /* In case call-backs are used, copy them from parent to child. */
                pxNewSocket->u.xTCP.pxHandleConnected = pxSocket->u.xTCP.pxHandleConnected;
                pxNewSocket->u.xTCP.pxHandleReceive = pxSocket->u.xTCP.pxHandleReceive;
                pxNewSocket->u.xTCP.pxHandleSent = pxSocket->u.xTCP.pxHandleSent;
            }
        #endif /* ipconfigUSE_CALLBACKS */

        #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
            {
                /* Child socket of listening sockets will inherit the Socket Set
                 * Otherwise the owner has no chance of including it into the set. */
                if( pxSocket->pxSocketSet != NULL )
                {
                    pxNewSocket->pxSocketSet = pxSocket->pxSocketSet;
                    pxNewSocket->xSelectBits = pxSocket->xSelectBits | ( ( EventBits_t ) eSELECT_READ ) | ( ( EventBits_t ) eSELECT_EXCEPT );
                }
            }
        #endif /* ipconfigSUPPORT_SELECT_FUNCTION */

        /* And bind it to the same local port as its parent. */
        xAddress.sin_addr = *ipLOCAL_IP_ADDRESS_POINTER;
        xAddress.sin_port = FreeRTOS_htons( pxSocket->usLocalPort );

        #if ( ipconfigTCP_HANG_PROTECTION == 1 )
            {
                /* Only when there is anti-hanging protection, a socket may become an
                 * orphan temporarily.  Once this socket is really connected, the owner of
                 * the server socket will be notified. */

                /* When bPassQueued is true, the socket is an orphan until it gets
                 * connected. */
                pxNewSocket->u.xTCP.bits.bPassQueued = pdTRUE_UNSIGNED;
                pxNewSocket->u.xTCP.pxPeerSocket = pxSocket;
            }
        #else
            {
                /* A reference to the new socket may be stored and the socket is marked
                 * as 'passable'. */

                /* When bPassAccept is true, this socket may be returned in a call to
                 * accept(). */
                pxNewSocket->u.xTCP.bits.bPassAccept = pdTRUE_UNSIGNED;

                if( pxSocket->u.xTCP.pxPeerSocket == NULL )
                {
                    pxSocket->u.xTCP.pxPeerSocket = pxNewSocket;
                }
            }
        #endif /* if ( ipconfigTCP_HANG_PROTECTION == 1 ) */

        pxSocket->u.xTCP.usChildCount++;

        FreeRTOS_debug_printf( ( "Gain: Socket %u now has %u / %u child%s\n",
                                 pxSocket->usLocalPort,
                                 pxSocket->u.xTCP.usChildCount,
                                 pxSocket->u.xTCP.usBacklog,
                                 ( pxSocket->u.xTCP.usChildCount == 1U ) ? "" : "ren" ) );

        /* Now bind the child socket to the same port as the listening socket. */
        if( vSocketBind( pxNewSocket, &xAddress, sizeof( xAddress ), pdTRUE ) != 0 )
        {
            FreeRTOS_debug_printf( ( "TCP: Listen: new socket bind error\n" ) );
            ( void ) FreeRTOS_closesocket( pxNewSocket );
            xResult = pdFALSE;
        }
        else
        {
            xResult = pdTRUE;
        }

        return xResult;
    }
    /*-----------------------------------------------------------*/

    #if ( ( ipconfigHAS_DEBUG_PRINTF != 0 ) || ( ipconfigHAS_PRINTF != 0 ) )

        const char * FreeRTOS_GetTCPStateName( UBaseType_t ulState )
        {
            static const char * const pcStateNames[] =
            {
                "eCLOSED",
                "eTCP_LISTEN",
                "eCONNECT_SYN",
                "eSYN_FIRST",
                "eSYN_RECEIVED",
                "eESTABLISHED",
                "eFIN_WAIT_1",
                "eFIN_WAIT_2",
                "eCLOSE_WAIT",
                "eCLOSING",
                "eLAST_ACK",
                "eTIME_WAIT",
                "eUNKNOWN",
            };
            BaseType_t xIndex = ( BaseType_t ) ulState;

            if( ( xIndex < 0 ) || ( xIndex >= ARRAY_SIZE( pcStateNames ) ) )
            {
                /* The last item is called 'eUNKNOWN' */
                xIndex = ARRAY_SIZE( pcStateNames );
                xIndex--;
            }

            return pcStateNames[ xIndex ];
        }

    #endif /* ( ( ipconfigHAS_DEBUG_PRINTF != 0 ) || ( ipconfigHAS_PRINTF != 0 ) ) */
    /*-----------------------------------------------------------*/

/**
 * @brief In the API accept(), the user asks is there is a new client? As API's can
 *        not walk through the xBoundTCPSocketsList the IP-task will do this.
 *
 * @param[in] pxSocket: The socket for which the bound socket list will be iterated.
 *
 * @return if there is a new client, then pdTRUE is returned or else, pdFALSE.
 */
    BaseType_t xTCPCheckNewClient( FreeRTOS_Socket_t * pxSocket )
    {
        TickType_t uxLocalPort = ( TickType_t ) FreeRTOS_htons( pxSocket->usLocalPort );
        const ListItem_t * pxIterator;
        FreeRTOS_Socket_t * pxFound;
        BaseType_t xResult = pdFALSE;
        const ListItem_t * pxEndTCP = listGET_END_MARKER( &xBoundTCPSocketsList );

        /* Here xBoundTCPSocketsList can be accessed safely IP-task is the only one
         * who has access. */
        for( pxIterator = ( const ListItem_t * ) listGET_HEAD_ENTRY( &xBoundTCPSocketsList );
             pxIterator != pxEndTCP;
             pxIterator = ( const ListItem_t * ) listGET_NEXT( pxIterator ) )
        {
            if( listGET_LIST_ITEM_VALUE( pxIterator ) == ( configLIST_VOLATILE TickType_t ) uxLocalPort )
            {
                pxFound = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxIterator ) );

                if( ( pxFound->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP ) && ( pxFound->u.xTCP.bits.bPassAccept != pdFALSE_UNSIGNED ) )
                {
                    pxSocket->u.xTCP.pxPeerSocket = pxFound;
                    FreeRTOS_debug_printf( ( "xTCPCheckNewClient[0]: client on port %u\n", pxSocket->usLocalPort ) );
                    xResult = pdTRUE;
                    break;
                }
            }
        }

        return xResult;
    }
    /*-----------------------------------------------------------*/


#endif /* ipconfigUSE_TCP == 1 */

/* Provide access to private members for testing. */
#ifdef FREERTOS_ENABLE_UNIT_TESTS
    #include "freertos_tcp_test_access_tcp_define.h"
#endif

/* Provide access to private members for verification. */
#ifdef FREERTOS_TCP_ENABLE_VERIFICATION
    #include "aws_freertos_tcp_verification_access_tcp_define.h"
#endif
