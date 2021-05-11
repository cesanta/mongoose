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
 * @file FreeRTOS_Sockets.c
 * @brief Implements the Sockets API based on Berkeley sockets for the FreeRTOS+TCP network stack.
 *        Sockets are used by the application processes to interact with the IP-task which in turn
 *        interacts with the hardware.
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
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_DNS.h"
#include "NetworkBufferManagement.h"

/* The ItemValue of the sockets xBoundSocketListItem member holds the socket's
 * port number. */
/** @brief Set the port number for the socket in the xBoundSocketListItem. */
#define socketSET_SOCKET_PORT( pxSocket, usPort )    listSET_LIST_ITEM_VALUE( ( &( ( pxSocket )->xBoundSocketListItem ) ), ( usPort ) )
/** @brief Get the port number for the socket in the xBoundSocketListItem. */
#define socketGET_SOCKET_PORT( pxSocket )            listGET_LIST_ITEM_VALUE( ( &( ( pxSocket )->xBoundSocketListItem ) ) )

/** @brief Test if a socket it bound which means it is either included in
 *         xBoundUDPSocketsList or xBoundTCPSocketsList
 */
#define socketSOCKET_IS_BOUND( pxSocket )            ( listLIST_ITEM_CONTAINER( &( pxSocket )->xBoundSocketListItem ) != NULL )

/** @brief If FreeRTOS_sendto() is called on a socket that is not bound to a port
 *         number then, depending on the FreeRTOSIPConfig.h settings, it might be
 *         that a port number is automatically generated for the socket.
 *         Automatically generated port numbers will be between
 *         socketAUTO_PORT_ALLOCATION_START_NUMBER and 0xffff.
 *
 * @note Per https://tools.ietf.org/html/rfc6056, "the dynamic ports consist of
 *       the range 49152-65535. However, ephemeral port selection algorithms should
 *       use the whole range 1024-65535" excluding those already in use (inbound
 *       or outbound).
 */
#if !defined( socketAUTO_PORT_ALLOCATION_START_NUMBER )
    #define socketAUTO_PORT_ALLOCATION_START_NUMBER    ( ( uint16_t ) 0x0400 )
#endif

/** @brief Maximum value of port number which can be auto assigned. */
#define socketAUTO_PORT_ALLOCATION_MAX_NUMBER    ( ( uint16_t ) 0xffff )

/** @brief The number of octets that make up an IP address. */
#define socketMAX_IP_ADDRESS_OCTETS              4U

/** @brief A block time of 0 simply means "don't block". */
#define socketDONT_BLOCK                         ( ( TickType_t ) 0 )

/** @brief TCP timer period in milliseconds. */
#if ( ( ipconfigUSE_TCP == 1 ) && !defined( ipTCP_TIMER_PERIOD_MS ) )
    #define ipTCP_TIMER_PERIOD_MS    ( 1000U )
#endif

/* Some helper macro's for defining the 20/80 % limits of uxLittleSpace / uxEnoughSpace. */
#define sock20_PERCENT     20U  /**< 20% of the defined limit. */
#define sock80_PERCENT     80U  /**< 80% of the defined limit. */
#define sock100_PERCENT    100U /**< 100% of the defined limit. */

#if ( ipconfigUSE_CALLBACKS != 0 )
    static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( F_TCP_UDP_Handler_t )
    {
        return ( F_TCP_UDP_Handler_t * ) pvArgument;
    }
    static portINLINE ipDECL_CAST_CONST_PTR_FUNC_FOR_TYPE( F_TCP_UDP_Handler_t )
    {
        return ( const F_TCP_UDP_Handler_t * ) pvArgument;
    }
#endif


/**
 * @brief Utility function to cast pointer of a type to pointer of type NetworkBufferDescriptor_t.
 *
 * @return The casted pointer.
 */
static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( NetworkBufferDescriptor_t )
{
    return ( NetworkBufferDescriptor_t * ) pvArgument;
}


/**
 * @brief Utility function to cast pointer of a type to pointer of type StreamBuffer_t.
 *
 * @return The casted pointer.
 */
static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( StreamBuffer_t )
{
    return ( StreamBuffer_t * ) pvArgument;
}
/*-----------------------------------------------------------*/

/*
 * Allocate the next port number from the private allocation range.
 * TCP and UDP each have their own series of port numbers
 * ulProtocol is either ipPROTOCOL_UDP or ipPROTOCOL_TCP
 */
static uint16_t prvGetPrivatePortNumber( BaseType_t xProtocol );

/*
 * Return the list item from within pxList that has an item value of
 * xWantedItemValue.  If there is no such list item return NULL.
 */
static const ListItem_t * pxListFindListItemWithValue( const List_t * pxList,
                                                       TickType_t xWantedItemValue );

/*
 * Return pdTRUE only if pxSocket is valid and bound, as far as can be
 * determined.
 */
static BaseType_t prvValidSocket( const FreeRTOS_Socket_t * pxSocket,
                                  BaseType_t xProtocol,
                                  BaseType_t xIsBound );

#if ( ipconfigUSE_TCP == 1 )

/*
 * Internal function prvSockopt_so_buffer(): sets FREERTOS_SO_SNDBUF or
 * FREERTOS_SO_RCVBUF properties of a socket.
 */
    static BaseType_t prvSockopt_so_buffer( FreeRTOS_Socket_t * pxSocket,
                                            int32_t lOptionName,
                                            const void * pvOptionValue );
#endif /* ipconfigUSE_TCP == 1 */

/*
 * Before creating a socket, check the validity of the parameters used
 * and find the size of the socket space, which is different for UDP and TCP
 */
static BaseType_t prvDetermineSocketSize( BaseType_t xDomain,
                                          BaseType_t xType,
                                          BaseType_t xProtocol,
                                          size_t * pxSocketSize );

#if ( ipconfigUSE_TCP == 1 )

/*
 * Create a txStream or a rxStream, depending on the parameter 'xIsInputStream'
 */
    static StreamBuffer_t * prvTCPCreateStream( FreeRTOS_Socket_t * pxSocket,
                                                BaseType_t xIsInputStream );
#endif /* ipconfigUSE_TCP == 1 */

#if ( ipconfigUSE_TCP == 1 )

/*
 * Called from FreeRTOS_send(): some checks which will be done before
 * sending a TCP packed.
 */
    static int32_t prvTCPSendCheck( FreeRTOS_Socket_t * pxSocket,
                                    size_t uxDataLength );
#endif /* ipconfigUSE_TCP */

#if ( ipconfigUSE_TCP == 1 )

/*
 * When a child socket gets closed, make sure to update the child-count of the parent
 */
    static void prvTCPSetSocketCount( FreeRTOS_Socket_t const * pxSocketToDelete );
#endif /* ipconfigUSE_TCP == 1 */

#if ( ipconfigUSE_TCP == 1 )

/*
 * Called from FreeRTOS_connect(): make some checks and if allowed, send a
 * message to the IP-task to start connecting to a remote socket
 */
    static BaseType_t prvTCPConnectStart( FreeRTOS_Socket_t * pxSocket,
                                          struct freertos_sockaddr const * pxAddress );
#endif /* ipconfigUSE_TCP */

#if ( ipconfigUSE_TCP == 1 )

/*
 * Check if it makes any sense to wait for a connect event.
 * It may return: -EINPROGRESS, -EAGAIN, or 0 for OK.
 */
    static BaseType_t bMayConnect( FreeRTOS_Socket_t const * pxSocket );
#endif /* ipconfigUSE_TCP */

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/* Executed by the IP-task, it will check all sockets belonging to a set */
    static void prvFindSelectedSocket( SocketSelect_t * pxSocketSet );

#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

/** @brief The list that contains mappings between sockets and port numbers.
 *         Accesses to this list must be protected by critical sections of
 *         some kind.
 */
List_t xBoundUDPSocketsList;

#if ipconfigUSE_TCP == 1

/** @brief The list that contains mappings between sockets and port numbers.
 *         Accesses to this list must be protected by critical sections of
 *         some kind.
 */
    List_t xBoundTCPSocketsList;

#endif /* ipconfigUSE_TCP == 1 */

/*-----------------------------------------------------------*/

/**
 * @brief Check whether the socket is valid or not.
 *
 * @param[in] pxSocket: The socket being checked.
 * @param[in] xProtocol: The protocol for which the socket was created.
 * @param[in] xIsBound: pdTRUE when the socket should be bound, otherwise pdFALSE.
 *
 * @return If the socket is valid, then pdPASS is returned or else, pdFAIL
 *         is returned.
 */
static BaseType_t prvValidSocket( const FreeRTOS_Socket_t * pxSocket,
                                  BaseType_t xProtocol,
                                  BaseType_t xIsBound )
{
    BaseType_t xReturn;

    if( ( pxSocket == NULL ) || ( pxSocket == FREERTOS_INVALID_SOCKET ) )
    {
        xReturn = pdFALSE;
    }
    else if( ( xIsBound != pdFALSE ) && !socketSOCKET_IS_BOUND( pxSocket ) )
    {
        /* The caller expects the socket to be bound, but it isn't. */
        xReturn = pdFALSE;
    }
    else if( pxSocket->ucProtocol != ( uint8_t ) xProtocol )
    {
        /* Socket has a wrong type (UDP != TCP). */
        xReturn = pdFALSE;
    }
    else
    {
        xReturn = pdTRUE;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Initialise the bound TCP/UDP socket lists.
 */
void vNetworkSocketsInit( void )
{
    vListInitialise( &xBoundUDPSocketsList );

    #if ( ipconfigUSE_TCP == 1 )
        {
            vListInitialise( &xBoundTCPSocketsList );
        }
    #endif /* ipconfigUSE_TCP == 1 */
}
/*-----------------------------------------------------------*/

/**
 * @brief Determine the socket size for the given protocol.
 *
 * @param[in] xDomain: The domain for which the size of socket is being determined.
 * @param[in] xType: Is this a datagram socket or a stream socket.
 * @param[in] xProtocol: The protocol being used.
 * @param[out] pxSocketSize: Pointer to a variable in which the size shall be returned
 *                           if all checks pass.
 *
 * @return pdPASS if socket size was determined and put in the parameter pxSocketSize
 *         correctly, else pdFAIL.
 */
static BaseType_t prvDetermineSocketSize( BaseType_t xDomain,
                                          BaseType_t xType,
                                          BaseType_t xProtocol,
                                          size_t * pxSocketSize )
{
    BaseType_t xReturn = pdPASS;
    FreeRTOS_Socket_t const * pxSocket = NULL;

    /* Asserts must not appear before it has been determined that the network
     * task is ready - otherwise the asserts will fail. */
    if( xIPIsNetworkTaskReady() == pdFALSE )
    {
        xReturn = pdFAIL;
    }
    else
    {
        /* Only Ethernet is currently supported. */
        configASSERT( xDomain == FREERTOS_AF_INET );

        /* Check if the UDP socket-list has been initialised. */
        configASSERT( listLIST_IS_INITIALISED( &xBoundUDPSocketsList ) );
        #if ( ipconfigUSE_TCP == 1 )
            {
                /* Check if the TCP socket-list has been initialised. */
                configASSERT( listLIST_IS_INITIALISED( &xBoundTCPSocketsList ) );
            }
        #endif /* ipconfigUSE_TCP == 1 */

        if( xProtocol == FREERTOS_IPPROTO_UDP )
        {
            if( xType != FREERTOS_SOCK_DGRAM )
            {
                xReturn = pdFAIL;
                configASSERT( xReturn == pdPASS );
            }

            /* In case a UDP socket is created, do not allocate space for TCP data. */
            *pxSocketSize = ( sizeof( *pxSocket ) - sizeof( pxSocket->u ) ) + sizeof( pxSocket->u.xUDP );
        }

        #if ( ipconfigUSE_TCP == 1 )
            else if( xProtocol == FREERTOS_IPPROTO_TCP )
            {
                if( xType != FREERTOS_SOCK_STREAM )
                {
                    xReturn = pdFAIL;
                    configASSERT( xReturn == pdPASS );
                }

                *pxSocketSize = ( sizeof( *pxSocket ) - sizeof( pxSocket->u ) ) + sizeof( pxSocket->u.xTCP );
            }
        #endif /* ipconfigUSE_TCP == 1 */
        else
        {
            xReturn = pdFAIL;
            configASSERT( xReturn == pdPASS );
        }
    }

    /* In case configASSERT() is not used */
    ( void ) xDomain;
    ( void ) pxSocket; /* Was only used for sizeof. */
    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief allocate and initialise a socket.
 *
 * @param[in] xDomain: The domain in which the socket should be created.
 * @param[in] xType: The type of the socket.
 * @param[in] xProtocol: The protocol of the socket.
 *
 * @return FREERTOS_INVALID_SOCKET if the allocation failed, or if there was
 *         a parameter error, otherwise a valid socket.
 */
Socket_t FreeRTOS_socket( BaseType_t xDomain,
                          BaseType_t xType,
                          BaseType_t xProtocol )
{
    FreeRTOS_Socket_t * pxSocket;

/* Note that this value will be over-written by the call to prvDetermineSocketSize. */
    size_t uxSocketSize = 1;
    EventGroupHandle_t xEventGroup;
    Socket_t xReturn;

    if( prvDetermineSocketSize( xDomain, xType, xProtocol, &uxSocketSize ) == pdFAIL )
    {
        xReturn = FREERTOS_INVALID_SOCKET;
    }
    else
    {
        /* Allocate the structure that will hold the socket information. The
        * size depends on the type of socket: UDP sockets need less space. A
        * define 'pvPortMallocSocket' will used to allocate the necessary space.
        * By default it points to the FreeRTOS function 'pvPortMalloc()'. */
        pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, pvPortMallocSocket( uxSocketSize ) );

        if( pxSocket == NULL )
        {
            xReturn = FREERTOS_INVALID_SOCKET;
            iptraceFAILED_TO_CREATE_SOCKET();
        }
        else
        {
            xEventGroup = xEventGroupCreate();

            if( xEventGroup == NULL )
            {
                vPortFreeSocket( pxSocket );
                xReturn = FREERTOS_INVALID_SOCKET;
                iptraceFAILED_TO_CREATE_EVENT_GROUP();
            }
            else
            {
                if( xProtocol == FREERTOS_IPPROTO_UDP )
                {
                    iptraceMEM_STATS_CREATE( tcpSOCKET_UDP, pxSocket, uxSocketSize + sizeof( StaticEventGroup_t ) );
                }
                else
                {
                    /* Lint wants at least a comment, in case the macro is empty. */
                    iptraceMEM_STATS_CREATE( tcpSOCKET_TCP, pxSocket, uxSocketSize + sizeof( StaticEventGroup_t ) );
                }

                /* Clear the entire space to avoid nulling individual entries. */
                ( void ) memset( pxSocket, 0, uxSocketSize );

                pxSocket->xEventGroup = xEventGroup;

                /* Initialise the socket's members.  The semaphore will be created
                 * if the socket is bound to an address, for now the pointer to the
                 * semaphore is just set to NULL to show it has not been created. */
                if( xProtocol == FREERTOS_IPPROTO_UDP )
                {
                    vListInitialise( &( pxSocket->u.xUDP.xWaitingPacketsList ) );

                    #if ( ipconfigUDP_MAX_RX_PACKETS > 0U )
                        {
                            pxSocket->u.xUDP.uxMaxPackets = ( UBaseType_t ) ipconfigUDP_MAX_RX_PACKETS;
                        }
                    #endif /* ipconfigUDP_MAX_RX_PACKETS > 0 */
                }

                vListInitialiseItem( &( pxSocket->xBoundSocketListItem ) );
                listSET_LIST_ITEM_OWNER( &( pxSocket->xBoundSocketListItem ), ipPOINTER_CAST( void *, pxSocket ) );

                pxSocket->xReceiveBlockTime = ipconfigSOCK_DEFAULT_RECEIVE_BLOCK_TIME;
                pxSocket->xSendBlockTime = ipconfigSOCK_DEFAULT_SEND_BLOCK_TIME;
                pxSocket->ucSocketOptions = ( uint8_t ) FREERTOS_SO_UDPCKSUM_OUT;
                pxSocket->ucProtocol = ( uint8_t ) xProtocol; /* protocol: UDP or TCP */

                #if ( ipconfigUSE_TCP == 1 )
                    {
                        if( xProtocol == FREERTOS_IPPROTO_TCP )
                        {
                            /* StreamSize is expressed in number of bytes */
                            /* Round up buffer sizes to nearest multiple of MSS */
                            pxSocket->u.xTCP.usCurMSS = ( uint16_t ) ipconfigTCP_MSS;
                            pxSocket->u.xTCP.usInitMSS = ( uint16_t ) ipconfigTCP_MSS;
                            pxSocket->u.xTCP.uxRxStreamSize = ( size_t ) ipconfigTCP_RX_BUFFER_LENGTH;
                            pxSocket->u.xTCP.uxTxStreamSize = ( size_t ) FreeRTOS_round_up( ipconfigTCP_TX_BUFFER_LENGTH, ipconfigTCP_MSS );
                            /* Use half of the buffer size of the TCP windows */
                            #if ( ipconfigUSE_TCP_WIN == 1 )
                                {
                                    pxSocket->u.xTCP.uxRxWinSize = FreeRTOS_max_uint32( 1UL, ( uint32_t ) ( pxSocket->u.xTCP.uxRxStreamSize / 2U ) / ipconfigTCP_MSS );
                                    pxSocket->u.xTCP.uxTxWinSize = FreeRTOS_max_uint32( 1UL, ( uint32_t ) ( pxSocket->u.xTCP.uxTxStreamSize / 2U ) / ipconfigTCP_MSS );
                                }
                            #else
                                {
                                    pxSocket->u.xTCP.uxRxWinSize = 1U;
                                    pxSocket->u.xTCP.uxTxWinSize = 1U;
                                }
                            #endif

                            /* The above values are just defaults, and can be overridden by
                             * calling FreeRTOS_setsockopt().  No buffers will be allocated until a
                             * socket is connected and data is exchanged. */
                        }
                    }
                #endif /* ipconfigUSE_TCP == 1 */
                xReturn = pxSocket;
            }
        }
    }

    /* Remove compiler warnings in the case the configASSERT() is not defined. */
    ( void ) xDomain;

    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief Create a socket set.
 *
 * @return The new socket set which was created, or NULL when allocation has failed.
 */
    SocketSet_t FreeRTOS_CreateSocketSet( void )
    {
        SocketSelect_t * pxSocketSet;

        pxSocketSet = ipCAST_PTR_TO_TYPE_PTR( SocketSelect_t, pvPortMalloc( sizeof( *pxSocketSet ) ) );

        if( pxSocketSet != NULL )
        {
            ( void ) memset( pxSocketSet, 0, sizeof( *pxSocketSet ) );
            pxSocketSet->xSelectGroup = xEventGroupCreate();

            if( pxSocketSet->xSelectGroup == NULL )
            {
                vPortFree( pxSocketSet );
                pxSocketSet = NULL;
            }
            else
            {
                /* Lint wants at least a comment, in case the macro is empty. */
                iptraceMEM_STATS_CREATE( tcpSOCKET_SET, pxSocketSet, sizeof( *pxSocketSet ) + sizeof( StaticEventGroup_t ) );
            }
        }

        return ( SocketSet_t ) pxSocketSet;
    }

#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief Delete a given socket set.
 *
 * @param[in] xSocketSet: The socket set being deleted.
 */
    void FreeRTOS_DeleteSocketSet( SocketSet_t xSocketSet )
    {
        SocketSelect_t * pxSocketSet = ( SocketSelect_t * ) xSocketSet;


        iptraceMEM_STATS_DELETE( pxSocketSet );

        vEventGroupDelete( pxSocketSet->xSelectGroup );
        vPortFree( pxSocketSet );
    }

#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief Add a socket to a set.
 *
 * @param[in] xSocket: The socket being added.
 * @param[in] xSocketSet: The socket set being added to.
 * @param[in] xBitsToSet: The event bits to set, a combination of the values defined
 *                        in 'eSelectEvent_t', for read, write, exception, etc.
 */
    void FreeRTOS_FD_SET( Socket_t xSocket,
                          SocketSet_t xSocketSet,
                          EventBits_t xBitsToSet )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
        SocketSelect_t * pxSocketSet = ( SocketSelect_t * ) xSocketSet;


        configASSERT( pxSocket != NULL );
        configASSERT( xSocketSet != NULL );

        /* Make sure we're not adding bits which are reserved for internal use,
         * such as eSELECT_CALL_IP */
        pxSocket->xSelectBits |= xBitsToSet & ( ( EventBits_t ) eSELECT_ALL );

        if( ( pxSocket->xSelectBits & ( ( EventBits_t ) eSELECT_ALL ) ) != ( EventBits_t ) 0U )
        {
            /* Adding a socket to a socket set. */
            pxSocket->pxSocketSet = ( SocketSelect_t * ) xSocketSet;

            /* Now have the IP-task call vSocketSelect() to see if the set contains
             * any sockets which are 'ready' and set the proper bits. */
            prvFindSelectedSocket( pxSocketSet );
        }
    }

#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief Clear select bits for a socket. If the mask becomes 0,
 *        remove the socket from the set.
 *
 * @param[in] xSocket: The socket whose select bits are being cleared.
 * @param[in] xSocketSet: The socket set of the socket.
 * @param[in] xBitsToClear: The bits to be cleared. Every '1' means that the
 *                corresponding bit will be cleared. See 'eSelectEvent_t' for
 *                the possible values.
 */
    void FreeRTOS_FD_CLR( Socket_t xSocket,
                          SocketSet_t xSocketSet,
                          EventBits_t xBitsToClear )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;

        configASSERT( pxSocket != NULL );
        configASSERT( xSocketSet != NULL );

        pxSocket->xSelectBits &= ~( xBitsToClear & ( ( EventBits_t ) eSELECT_ALL ) );

        if( ( pxSocket->xSelectBits & ( ( EventBits_t ) eSELECT_ALL ) ) != ( EventBits_t ) 0U )
        {
            pxSocket->pxSocketSet = ( SocketSelect_t * ) xSocketSet;
        }
        else
        {
            /* disconnect it from the socket set */
            pxSocket->pxSocketSet = NULL;
        }
    }


#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/


#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief Test if a socket belongs to a socket-set and if so, which event bit(s)
 *        are set.
 *
 * @param[in] xSocket: The socket of interest.
 * @param[in] xSocketSet: The socket set to which the socket belongs.
 *
 * @return If the socket belongs to the socket set: the event bits, otherwise zero.
 */
    EventBits_t FreeRTOS_FD_ISSET( Socket_t xSocket,
                                   SocketSet_t xSocketSet )
    {
        EventBits_t xReturn;
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;

        configASSERT( pxSocket != NULL );
        configASSERT( xSocketSet != NULL );

        if( xSocketSet == ( SocketSet_t ) pxSocket->pxSocketSet )
        {
            /* Make sure we're not adding bits which are reserved for internal
             * use. */
            xReturn = pxSocket->xSocketBits & ( ( EventBits_t ) eSELECT_ALL );
        }
        else
        {
            xReturn = 0;
        }

        return xReturn;
    }


#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief The select() statement: wait for an event to occur on any of the sockets
 *        included in a socket set.
 *
 * @param[in] xSocketSet: The socket set including the sockets on which we are
 *                        waiting for an event to occur.
 * @param[in] xBlockTimeTicks: Maximum time ticks to wait for an event to occur.
 *                   If the value is 'portMAX_DELAY' then the function will wait
 *                   indefinitely for an event to occur.
 *
 * @return The socket which might have triggered the event bit.
 */
    BaseType_t FreeRTOS_select( SocketSet_t xSocketSet,
                                TickType_t xBlockTimeTicks )
    {
        TimeOut_t xTimeOut;
        TickType_t xRemainingTime;
        SocketSelect_t * pxSocketSet = ( SocketSelect_t * ) xSocketSet;
        EventBits_t uxResult;

        configASSERT( xSocketSet != NULL );

        /* Only in the first round, check for non-blocking */
        xRemainingTime = xBlockTimeTicks;

        /* Fetch the current time */
        vTaskSetTimeOutState( &xTimeOut );

        for( ; ; )
        {
            /* Find a socket which might have triggered the bit
             * This function might return immediately or block for a limited time */
            uxResult = xEventGroupWaitBits( pxSocketSet->xSelectGroup, ( ( EventBits_t ) eSELECT_ALL ), pdFALSE, pdFALSE, xRemainingTime );

            #if ( ipconfigSUPPORT_SIGNALS != 0 )
                {
                    if( ( uxResult & ( ( EventBits_t ) eSELECT_INTR ) ) != 0U )
                    {
                        ( void ) xEventGroupClearBits( pxSocketSet->xSelectGroup, ( EventBits_t ) eSELECT_INTR );
                        FreeRTOS_debug_printf( ( "FreeRTOS_select: interrupted\n" ) );
                        break;
                    }
                }
            #endif /* ipconfigSUPPORT_SIGNALS */

            /* Have the IP-task find the socket which had an event */
            prvFindSelectedSocket( pxSocketSet );

            uxResult = xEventGroupGetBits( pxSocketSet->xSelectGroup );

            if( uxResult != 0U )
            {
                break;
            }

            /* Has the timeout been reached? */
            if( xTaskCheckForTimeOut( &xTimeOut, &xRemainingTime ) != pdFALSE )
            {
                break;
            }
        }

        return ( BaseType_t ) uxResult;
    }


#endif /* ipconfigSUPPORT_SELECT_FUNCTION */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief Send a message to the IP-task to have it check all sockets belonging to
 *        'pxSocketSet'
 *
 * @param[in] pxSocketSet: The socket set being asked to check.
 */
    static void prvFindSelectedSocket( SocketSelect_t * pxSocketSet )
    {
        IPStackEvent_t xSelectEvent;

        #if ( ipconfigSELECT_USES_NOTIFY != 0 )
            SocketSelectMessage_t xSelectMessage;
        #endif

        xSelectEvent.eEventType = eSocketSelectEvent;
        #if ( ipconfigSELECT_USES_NOTIFY != 0 )
            {
                xSelectMessage.pxSocketSet = pxSocketSet;
                xSelectMessage.xTaskhandle = xTaskGetCurrentTaskHandle();
                xSelectEvent.pvData = &( xSelectMessage );
            }
        #else
            {
                xSelectEvent.pvData = pxSocketSet;

                /* while the IP-task works on the request, the API will block on
                 * 'eSELECT_CALL_IP'.  So clear it first. */
                ( void ) xEventGroupClearBits( pxSocketSet->xSelectGroup, ( BaseType_t ) eSELECT_CALL_IP );
            }
        #endif /* if ( ipconfigSELECT_USES_NOTIFY != 0 ) */

        /* Now send the socket select event */
        if( xSendEventStructToIPTask( &xSelectEvent, ( TickType_t ) portMAX_DELAY ) == pdFAIL )
        {
            /* Oops, we failed to wake-up the IP task. No use to wait for it. */
            FreeRTOS_debug_printf( ( "prvFindSelectedSocket: failed\n" ) );
        }
        else
        {
            /* As soon as the IP-task is ready, it will set 'eSELECT_CALL_IP' to
             * wakeup the calling API */
            #if ( ipconfigSELECT_USES_NOTIFY != 0 )
                {
                    ( void ) ulTaskNotifyTake( pdFALSE, portMAX_DELAY );
                }
            #else
                {
                    ( void ) xEventGroupWaitBits( pxSocketSet->xSelectGroup, ( BaseType_t ) eSELECT_CALL_IP, pdTRUE, pdFALSE, portMAX_DELAY );
                }
            #endif
        }
    }


#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

/**
 * @brief Receive data from a bound socket. In this library, the function
 *        can only be used with connection-less sockets (UDP). For TCP sockets,
 *        please use FreeRTOS_recv().
 *
 * @param[in] xSocket: The socket to which the data is sent i.e. the
 *                     listening socket.
 * @param[out] pvBuffer: The buffer in which the data being received is to
 *                      be stored.
 * @param[in] uxBufferLength: The length of the buffer.
 * @param[in] xFlags: The flags to indicate preferences while calling this function.
 * @param[out] pxSourceAddress: The source address from which the data is being sent.
 * @param[out] pxSourceAddressLength: This parameter is used only to adhere to Berkeley
 *                              sockets standard. It is not used internally.
 *
 * @return The number of bytes received. Or else, an error code is returned. When it
 *         returns a negative value, the cause can be looked-up in
 *         'FreeRTOS_errno_TCP.h'.
 */
int32_t FreeRTOS_recvfrom( Socket_t xSocket,
                           void * pvBuffer,
                           size_t uxBufferLength,
                           BaseType_t xFlags,
                           struct freertos_sockaddr * pxSourceAddress,
                           socklen_t * pxSourceAddressLength )
{
    BaseType_t lPacketCount;
    NetworkBufferDescriptor_t * pxNetworkBuffer;
    const void * pvCopySource;
    FreeRTOS_Socket_t const * pxSocket = xSocket;
    TickType_t xRemainingTime = ( TickType_t ) 0; /* Obsolete assignment, but some compilers output a warning if its not done. */
    BaseType_t xTimed = pdFALSE;
    TimeOut_t xTimeOut;
    int32_t lReturn;
    EventBits_t xEventBits = ( EventBits_t ) 0;
    size_t uxPayloadLength;

    if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_UDP, pdTRUE ) == pdFALSE )
    {
        lReturn = -pdFREERTOS_ERRNO_EINVAL;
    }
    else
    {
        lPacketCount = ( BaseType_t ) listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) );

        /* The function prototype is designed to maintain the expected Berkeley
         * sockets standard, but this implementation does not use all the parameters. */
        ( void ) pxSourceAddressLength;

        while( lPacketCount == 0 )
        {
            if( xTimed == pdFALSE )
            {
                /* Check to see if the socket is non blocking on the first
                 * iteration.  */
                xRemainingTime = pxSocket->xReceiveBlockTime;

                if( xRemainingTime == ( TickType_t ) 0 )
                {
                    #if ( ipconfigSUPPORT_SIGNALS != 0 )
                        {
                            /* Just check for the interrupt flag. */
                            xEventBits = xEventGroupWaitBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_INTR,
                                                              pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, socketDONT_BLOCK );
                        }
                    #endif /* ipconfigSUPPORT_SIGNALS */
                    break;
                }

                if( ( ( ( UBaseType_t ) xFlags ) & ( ( UBaseType_t ) FREERTOS_MSG_DONTWAIT ) ) != 0U )
                {
                    break;
                }

                /* To ensure this part only executes once. */
                xTimed = pdTRUE;

                /* Fetch the current time. */
                vTaskSetTimeOutState( &xTimeOut );
            }

            /* Wait for arrival of data.  While waiting, the IP-task may set the
             * 'eSOCKET_RECEIVE' bit in 'xEventGroup', if it receives data for this
             * socket, thus unblocking this API call. */
            xEventBits = xEventGroupWaitBits( pxSocket->xEventGroup, ( ( EventBits_t ) eSOCKET_RECEIVE ) | ( ( EventBits_t ) eSOCKET_INTR ),
                                              pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, xRemainingTime );

            #if ( ipconfigSUPPORT_SIGNALS != 0 )
                {
                    if( ( xEventBits & ( EventBits_t ) eSOCKET_INTR ) != 0U )
                    {
                        if( ( xEventBits & ( EventBits_t ) eSOCKET_RECEIVE ) != 0U )
                        {
                            /* Shouldn't have cleared the eSOCKET_RECEIVE flag. */
                            ( void ) xEventGroupSetBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_RECEIVE );
                        }

                        break;
                    }
                }
            #else /* if ( ipconfigSUPPORT_SIGNALS != 0 ) */
                {
                    ( void ) xEventBits;
                }
            #endif /* ipconfigSUPPORT_SIGNALS */

            lPacketCount = ( BaseType_t ) listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) );

            if( lPacketCount != 0 )
            {
                break;
            }

            /* Has the timeout been reached ? */
            if( xTaskCheckForTimeOut( &xTimeOut, &xRemainingTime ) != pdFALSE )
            {
                break;
            }
        } /* while( lPacketCount == 0 ) */

        if( lPacketCount != 0 )
        {
            taskENTER_CRITICAL();
            {
                /* The owner of the list item is the network buffer. */
                pxNetworkBuffer = ipCAST_PTR_TO_TYPE_PTR( NetworkBufferDescriptor_t, listGET_OWNER_OF_HEAD_ENTRY( &( pxSocket->u.xUDP.xWaitingPacketsList ) ) );

                if( ( ( UBaseType_t ) xFlags & ( UBaseType_t ) FREERTOS_MSG_PEEK ) == 0U )
                {
                    /* Remove the network buffer from the list of buffers waiting to
                     * be processed by the socket. */
                    ( void ) uxListRemove( &( pxNetworkBuffer->xBufferListItem ) );
                }
            }
            taskEXIT_CRITICAL();

            /* The returned value is the length of the payload data, which is
             * calculated at the total packet size minus the headers.
             * The validity of `xDataLength` prvProcessIPPacket has been confirmed
             * in 'prvProcessIPPacket()'. */
            uxPayloadLength = pxNetworkBuffer->xDataLength - sizeof( UDPPacket_t );
            lReturn = ( int32_t ) uxPayloadLength;

            if( pxSourceAddress != NULL )
            {
                pxSourceAddress->sin_port = pxNetworkBuffer->usPort;
                pxSourceAddress->sin_addr = pxNetworkBuffer->ulIPAddress;
            }

            if( ( ( UBaseType_t ) xFlags & ( UBaseType_t ) FREERTOS_ZERO_COPY ) == 0U )
            {
                /* The zero copy flag is not set.  Truncate the length if it won't
                 * fit in the provided buffer. */
                if( lReturn > ( int32_t ) uxBufferLength )
                {
                    iptraceRECVFROM_DISCARDING_BYTES( ( uxBufferLength - lReturn ) );
                    lReturn = ( int32_t ) uxBufferLength;
                }

                /* Copy the received data into the provided buffer, then release the
                 * network buffer. */
                pvCopySource = ( const void * ) &pxNetworkBuffer->pucEthernetBuffer[ ipUDP_PAYLOAD_OFFSET_IPv4 ];
                ( void ) memcpy( pvBuffer, pvCopySource, ( size_t ) lReturn );

                if( ( ( UBaseType_t ) xFlags & ( UBaseType_t ) FREERTOS_MSG_PEEK ) == 0U )
                {
                    vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                }
            }
            else
            {
                /* The zero copy flag was set.  pvBuffer is not a buffer into which
                 * the received data can be copied, but a pointer that must be set to
                 * point to the buffer in which the received data has already been
                 * placed. */
                *( ( void ** ) pvBuffer ) = ipPOINTER_CAST( void *, &( pxNetworkBuffer->pucEthernetBuffer[ ipUDP_PAYLOAD_OFFSET_IPv4 ] ) );
            }
        }

        #if ( ipconfigSUPPORT_SIGNALS != 0 )
            else if( ( xEventBits & ( EventBits_t ) eSOCKET_INTR ) != 0U )
            {
                lReturn = -pdFREERTOS_ERRNO_EINTR;
                iptraceRECVFROM_INTERRUPTED();
            }
        #endif /* ipconfigSUPPORT_SIGNALS */
        else
        {
            lReturn = -pdFREERTOS_ERRNO_EWOULDBLOCK;
            iptraceRECVFROM_TIMEOUT();
        }
    }

    return lReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Send data to a socket. The socket must have already been created by a
 *        successful call to FreeRTOS_socket(). It works for UDP-sockets only.
 *
 * @param[in] xSocket: The socket being sent to.
 * @param[in] pvBuffer: Pointer to the data being sent.
 * @param[in] uxTotalDataLength: Length (in bytes) of the data being sent.
 * @param[in] xFlags: Flags used to communicate preferences to the function.
 *                    Possibly FREERTOS_MSG_DONTWAIT and/or FREERTOS_ZERO_COPY.
 * @param[in] pxDestinationAddress: The address to which the data is to be sent.
 * @param[in] xDestinationAddressLength: This parameter is present to adhere to the
 *                  Berkeley sockets standard. Else, it is not used.
 *
 * @return When positive: the total number of bytes sent, when negative an error
 *         has occurred: it can be looked-up in 'FreeRTOS_errno_TCP.h'.
 */
int32_t FreeRTOS_sendto( Socket_t xSocket,
                         const void * pvBuffer,
                         size_t uxTotalDataLength,
                         BaseType_t xFlags,
                         const struct freertos_sockaddr * pxDestinationAddress,
                         socklen_t xDestinationAddressLength )
{
    NetworkBufferDescriptor_t * pxNetworkBuffer;
    void * pvCopyDest;
    IPStackEvent_t xStackTxEvent = { eStackTxEvent, NULL };
    TimeOut_t xTimeOut;
    TickType_t xTicksToWait;
    int32_t lReturn = 0;
    FreeRTOS_Socket_t const * pxSocket;
    const size_t uxMaxPayloadLength = ipMAX_UDP_PAYLOAD_LENGTH;
    const size_t uxPayloadOffset = ipUDP_PAYLOAD_OFFSET_IPv4;


    pxSocket = ( FreeRTOS_Socket_t * ) xSocket;

    /* The function prototype is designed to maintain the expected Berkeley
     * sockets standard, but this implementation does not use all the
     * parameters. */
    ( void ) xDestinationAddressLength;
    configASSERT( pvBuffer != NULL );

    if( uxTotalDataLength <= ( size_t ) uxMaxPayloadLength )
    {
        /* If the socket is not already bound to an address, bind it now.
         * Passing NULL as the address parameter tells FreeRTOS_bind() to select
         * the address to bind to. */
        if( socketSOCKET_IS_BOUND( pxSocket ) ||
            ( FreeRTOS_bind( xSocket, NULL, 0U ) == 0 ) )
        {
            xTicksToWait = pxSocket->xSendBlockTime;

            #if ( ipconfigUSE_CALLBACKS != 0 )
                {
                    if( xIsCallingFromIPTask() != pdFALSE )
                    {
                        /* If this send function is called from within a call-back
                         * handler it may not block, otherwise chances would be big to
                         * get a deadlock: the IP-task waiting for itself. */
                        xTicksToWait = ( TickType_t ) 0;
                    }
                }
            #endif /* ipconfigUSE_CALLBACKS */

            if( ( ( UBaseType_t ) xFlags & ( UBaseType_t ) FREERTOS_MSG_DONTWAIT ) != 0U )
            {
                xTicksToWait = ( TickType_t ) 0;
            }

            if( ( ( UBaseType_t ) xFlags & ( UBaseType_t ) FREERTOS_ZERO_COPY ) == 0U )
            {
                /* Zero copy is not set, so obtain a network buffer into
                 * which the payload will be copied. */
                vTaskSetTimeOutState( &xTimeOut );

                /* Block until a buffer becomes available, or until a
                 * timeout has been reached */
                pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( uxPayloadOffset + uxTotalDataLength, xTicksToWait );

                if( pxNetworkBuffer != NULL )
                {
                    pvCopyDest = ( void * ) &pxNetworkBuffer->pucEthernetBuffer[ uxPayloadOffset ];
                    ( void ) memcpy( pvCopyDest, pvBuffer, uxTotalDataLength );

                    if( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdTRUE )
                    {
                        /* The entire block time has been used up. */
                        xTicksToWait = ( TickType_t ) 0;
                    }
                }
            }
            else
            {
                /* When zero copy is used, pvBuffer is a pointer to the
                 * payload of a buffer that has already been obtained from the
                 * stack.  Obtain the network buffer pointer from the buffer. */
                pxNetworkBuffer = pxUDPPayloadBuffer_to_NetworkBuffer( pvBuffer );
            }

            if( pxNetworkBuffer != NULL )
            {
                /* xDataLength is the size of the total packet, including the Ethernet header. */
                pxNetworkBuffer->xDataLength = uxTotalDataLength + sizeof( UDPPacket_t );
                pxNetworkBuffer->usPort = pxDestinationAddress->sin_port;
                pxNetworkBuffer->usBoundPort = ( uint16_t ) socketGET_SOCKET_PORT( pxSocket );
                pxNetworkBuffer->ulIPAddress = pxDestinationAddress->sin_addr;

                /* The socket options are passed to the IP layer in the
                 * space that will eventually get used by the Ethernet header. */
                pxNetworkBuffer->pucEthernetBuffer[ ipSOCKET_OPTIONS_OFFSET ] = pxSocket->ucSocketOptions;

                /* Tell the networking task that the packet needs sending. */
                xStackTxEvent.pvData = pxNetworkBuffer;

                /* Ask the IP-task to send this packet */
                if( xSendEventStructToIPTask( &xStackTxEvent, xTicksToWait ) == pdPASS )
                {
                    /* The packet was successfully sent to the IP task. */
                    lReturn = ( int32_t ) uxTotalDataLength;
                    #if ( ipconfigUSE_CALLBACKS == 1 )
                        {
                            if( ipconfigIS_VALID_PROG_ADDRESS( pxSocket->u.xUDP.pxHandleSent ) )
                            {
                                pxSocket->u.xUDP.pxHandleSent( xSocket, uxTotalDataLength );
                            }
                        }
                    #endif /* ipconfigUSE_CALLBACKS */
                }
                else
                {
                    /* If the buffer was allocated in this function, release
                     * it. */
                    if( ( ( UBaseType_t ) xFlags & ( UBaseType_t ) FREERTOS_ZERO_COPY ) == 0U )
                    {
                        vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                    }

                    iptraceSTACK_TX_EVENT_LOST( ipSTACK_TX_EVENT );
                }
            }
            else
            {
                /* If errno was available, errno would be set to
                 * FREERTOS_ENOPKTS.  As it is, the function must return the
                 * number of transmitted bytes, so the calling function knows
                 * how much data was actually sent. */
                iptraceNO_BUFFER_FOR_SENDTO();
            }
        }
        else
        {
            /* No comment. */
            iptraceSENDTO_SOCKET_NOT_BOUND();
        }
    }
    else
    {
        /* The data is longer than the available buffer space. */
        iptraceSENDTO_DATA_TOO_LONG();
    }

    return lReturn;
} /* Tested */
/*-----------------------------------------------------------*/

/**
 * @brief binds a socket to a local port number. If port 0 is provided,
 *        a system provided port number will be assigned. This function
 *        can be used for both UDP and TCP sockets. The actual binding
 *        will be performed by the IP-task to avoid mutual access to the
 *        bound-socket-lists (xBoundUDPSocketsList or xBoundTCPSocketsList).
 *
 * @param[in] xSocket: The socket being bound.
 * @param[in] pxAddress: The address struct carrying the port number to which
 *                       this socket is to be bound.
 * @param[in] xAddressLength: This parameter is not used internally. The
 *                       function signature is used to adhere to standard
 *                       Berkeley sockets API.
 *
 * @return The return value is 0 if the bind is successful.
 *         If some error occurred, then a negative value is returned.
 */
BaseType_t FreeRTOS_bind( Socket_t xSocket,
                          struct freertos_sockaddr const * pxAddress,
                          socklen_t xAddressLength )
{
    IPStackEvent_t xBindEvent;
    FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
    BaseType_t xReturn = 0;

    ( void ) xAddressLength;

    configASSERT( xIsCallingFromIPTask() == pdFALSE );

    if( ( pxSocket == NULL ) || ( pxSocket == FREERTOS_INVALID_SOCKET ) )
    {
        xReturn = -pdFREERTOS_ERRNO_EINVAL;
    }

    /* Once a socket is bound to a port, it can not be bound to a different
     * port number */
    else if( socketSOCKET_IS_BOUND( pxSocket ) )
    {
        /* The socket is already bound. */
        FreeRTOS_debug_printf( ( "vSocketBind: Socket already bound to %d\n", pxSocket->usLocalPort ) );
        xReturn = -pdFREERTOS_ERRNO_EINVAL;
    }
    else
    {
        /* Prepare a messages to the IP-task in order to perform the binding.
         * The desired port number will be passed in usLocalPort. */
        xBindEvent.eEventType = eSocketBindEvent;
        xBindEvent.pvData = xSocket;

        if( pxAddress != NULL )
        {
            pxSocket->usLocalPort = FreeRTOS_ntohs( pxAddress->sin_port );
        }
        else
        {
            /* Caller wants to bind to a random port number. */
            pxSocket->usLocalPort = 0U;
        }

        /* portMAX_DELAY is used as a the time-out parameter, as binding *must*
         * succeed before the socket can be used.  _RB_ The use of an infinite
         * block time needs be changed as it could result in the task hanging. */
        if( xSendEventStructToIPTask( &xBindEvent, ( TickType_t ) portMAX_DELAY ) == pdFAIL )
        {
            /* Failed to wake-up the IP-task, no use to wait for it */
            FreeRTOS_debug_printf( ( "FreeRTOS_bind: send event failed\n" ) );
            xReturn = -pdFREERTOS_ERRNO_ECANCELED;
        }
        else
        {
            /* The IP-task will set the 'eSOCKET_BOUND' bit when it has done its
             * job. */
            ( void ) xEventGroupWaitBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_BOUND, pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, portMAX_DELAY );

            if( !socketSOCKET_IS_BOUND( pxSocket ) )
            {
                xReturn = -pdFREERTOS_ERRNO_EINVAL;
            }
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Internal version of bind() that should not be called directly.
 *        'xInternal' is used for TCP sockets only: it allows to have several
 *        (connected) child sockets bound to the same server port.
 *
 * @param[in] pxSocket: The socket is to be bound.
 * @param[in] pxBindAddress: The port to which this socket should be bound.
 * @param[in] uxAddressLength: The address length.
 * @param[in] xInternal: pdTRUE is calling internally, else pdFALSE.
 *
 * @return If the socket was bound to a port successfully, then a 0 is returned.
 *         Or else, an error code is returned.
 */
BaseType_t vSocketBind( FreeRTOS_Socket_t * pxSocket,
                        struct freertos_sockaddr * pxBindAddress,
                        size_t uxAddressLength,
                        BaseType_t xInternal )
{
    BaseType_t xReturn = 0; /* In Berkeley sockets, 0 means pass for bind(). */
    List_t * pxSocketList;
    struct freertos_sockaddr * pxAddress = pxBindAddress;

    #if ( ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND == 1 )
        struct freertos_sockaddr xAddress;
    #endif /* ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND */

    #if ( ipconfigUSE_TCP == 1 )
        if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            pxSocketList = &xBoundTCPSocketsList;
        }
        else
    #endif /* ipconfigUSE_TCP == 1 */
    {
        pxSocketList = &xBoundUDPSocketsList;
    }

    /* The function prototype is designed to maintain the expected Berkeley
     * sockets standard, but this implementation does not use all the parameters. */
    ( void ) uxAddressLength;

    configASSERT( pxSocket != NULL );
    configASSERT( pxSocket != FREERTOS_INVALID_SOCKET );

    #if ( ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND == 1 )
        {
            /* pxAddress will be NULL if sendto() was called on a socket without the
             * socket being bound to an address. In this case, automatically allocate
             * an address to the socket.  There is a small chance that the allocated
             * port will already be in use - if that is the case, then the check below
             * [pxListFindListItemWithValue()] will result in an error being returned. */
            if( pxAddress == NULL )
            {
                pxAddress = &xAddress;
                /* Put the port to zero to be assigned later. */
                pxAddress->sin_port = 0U;
            }
        }
    #endif /* ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND == 1 */

    /* Sockets must be bound before calling FreeRTOS_sendto() if
    * ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND is not set to 1. */
    configASSERT( pxAddress != NULL );

    #if ( ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND == 1 )
        /* pxAddress is not NULL, no testing needed. */
    #else
        if( pxAddress != NULL )
    #endif
    {
        /* Add a do-while loop to facilitate use of 'break' statements. */
        do
        {
            if( pxAddress->sin_port == 0U )
            {
                pxAddress->sin_port = prvGetPrivatePortNumber( ( BaseType_t ) pxSocket->ucProtocol );

                if( pxAddress->sin_port == ( uint16_t ) 0U )
                {
                    xReturn = -pdFREERTOS_ERRNO_EADDRNOTAVAIL;
                    break;
                }
            }

            /* If vSocketBind() is called from the API FreeRTOS_bind() it has been
             * confirmed that the socket was not yet bound to a port.  If it is called
             * from the IP-task, no such check is necessary. */

            /* Check to ensure the port is not already in use.  If the bind is
             * called internally, a port MAY be used by more than one socket. */
            if( ( ( xInternal == pdFALSE ) || ( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP ) ) &&
                ( pxListFindListItemWithValue( pxSocketList, ( TickType_t ) pxAddress->sin_port ) != NULL ) )
            {
                FreeRTOS_debug_printf( ( "vSocketBind: %sP port %d in use\n",
                                         ( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP ) ? "TC" : "UD",
                                         FreeRTOS_ntohs( pxAddress->sin_port ) ) );
                xReturn = -pdFREERTOS_ERRNO_EADDRINUSE;
            }
            else
            {
                /* Allocate the port number to the socket.
                 * This macro will set 'xBoundSocketListItem->xItemValue' */
                socketSET_SOCKET_PORT( pxSocket, pxAddress->sin_port );

                /* And also store it in a socket field 'usLocalPort' in host-byte-order,
                 * mostly used for logging and debugging purposes */
                pxSocket->usLocalPort = FreeRTOS_ntohs( pxAddress->sin_port );

                /* Add the socket to the list of bound ports. */
                {
                    /* If the network driver can iterate through 'xBoundUDPSocketsList',
                     * by calling xPortHasUDPSocket() then the IP-task must temporarily
                     * suspend the scheduler to keep the list in a consistent state. */
                    #if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 1 )
                        {
                            vTaskSuspendAll();
                        }
                    #endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */

                    /* Add the socket to 'xBoundUDPSocketsList' or 'xBoundTCPSocketsList' */
                    vListInsertEnd( pxSocketList, &( pxSocket->xBoundSocketListItem ) );

                    #if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 1 )
                        {
                            ( void ) xTaskResumeAll();
                        }
                    #endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */
                }
            }
        } while( ipFALSE_BOOL );
    }

    #if ( ipconfigALLOW_SOCKET_SEND_WITHOUT_BIND == 0 )
        else
        {
            xReturn = -pdFREERTOS_ERRNO_EADDRNOTAVAIL;
            FreeRTOS_debug_printf( ( "vSocketBind: Socket no addr\n" ) );
        }
    #endif

    if( xReturn != 0 )
    {
        iptraceBIND_FAILED( xSocket, ( FreeRTOS_ntohs( pxAddress->sin_port ) ) );
    }

    return xReturn;
} /* Tested */
/*-----------------------------------------------------------*/

/**
 * @brief Close a socket and free the allocated space. In case of a TCP socket:
 *        the connection will not be closed automatically. Subsequent messages
 *        for the closed socket will be responded to with a RST. The IP-task
 *        will actually close the socket, after receiving a 'eSocketCloseEvent'
 *        message.
 *
 * @param[in] xSocket: the socket being closed.
 *
 * @return There are three distinct values which can be returned:
 *         0: If the xSocket is NULL/invalid.
 *         1: If the socket was successfully closed (read the brief above).
 *        -1: If the socket was valid but could not be closed because the message
 *            could not be delivered to the IP-task. Try again later.
 */
BaseType_t FreeRTOS_closesocket( Socket_t xSocket )
{
    BaseType_t xResult;

    #if ( ipconfigUSE_TCP == 1 ) && ( ipconfigUSE_CALLBACKS == 1 )
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
    #endif
    IPStackEvent_t xCloseEvent;
    xCloseEvent.eEventType = eSocketCloseEvent;
    xCloseEvent.pvData = xSocket;

    if( ( xSocket == NULL ) || ( xSocket == FREERTOS_INVALID_SOCKET ) )
    {
        xResult = 0;
    }
    else
    {
        #if ( ( ipconfigUSE_TCP == 1 ) && ( ipconfigUSE_CALLBACKS == 1 ) )
            {
                if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP )
                {
                    /* Make sure that IP-task won't call the user callback's anymore */
                    pxSocket->u.xTCP.pxHandleConnected = NULL;
                    pxSocket->u.xTCP.pxHandleReceive = NULL;
                    pxSocket->u.xTCP.pxHandleSent = NULL;
                }
            }
        #endif /* ( ( ipconfigUSE_TCP == 1 ) && ( ipconfigUSE_CALLBACKS == 1 ) ) */

        /* Let the IP task close the socket to keep it synchronised with the
         * packet handling. */

        /* Note when changing the time-out value below, it must be checked who is calling
         * this function. If it is called by the IP-task, a deadlock could occur.
         * The IP-task would only call it in case of a user call-back */
        if( xSendEventStructToIPTask( &xCloseEvent, ( TickType_t ) 0 ) == pdFAIL )
        {
            FreeRTOS_debug_printf( ( "FreeRTOS_closesocket: failed\n" ) );
            xResult = -1;
        }
        else
        {
            xResult = 1;
        }
    }

    return xResult;
}

/**
 * @brief This is the internal version of FreeRTOS_closesocket(). It will
 *        be called by the IPtask only to avoid problems with synchronicity.
 *
 * @param[in] pxSocket: The socket descriptor of the socket being closed.
 *
 * @return Returns NULL, always.
 */
void * vSocketClose( FreeRTOS_Socket_t * pxSocket )
{
    NetworkBufferDescriptor_t * pxNetworkBuffer;

    #if ( ipconfigUSE_TCP == 1 )
        {
            /* For TCP: clean up a little more. */
            if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP )
            {
                #if ( ipconfigUSE_TCP_WIN == 1 )
                    {
                        if( pxSocket->u.xTCP.pxAckMessage != NULL )
                        {
                            vReleaseNetworkBufferAndDescriptor( pxSocket->u.xTCP.pxAckMessage );
                        }

                        /* Free the resources which were claimed by the tcpWin member */
                        vTCPWindowDestroy( &pxSocket->u.xTCP.xTCPWindow );
                    }
                #endif /* ipconfigUSE_TCP_WIN */

                /* Free the input and output streams */
                if( pxSocket->u.xTCP.rxStream != NULL )
                {
                    iptraceMEM_STATS_DELETE( pxSocket->u.xTCP.rxStream );
                    vPortFreeLarge( pxSocket->u.xTCP.rxStream );
                }

                if( pxSocket->u.xTCP.txStream != NULL )
                {
                    iptraceMEM_STATS_DELETE( pxSocket->u.xTCP.txStream );
                    vPortFreeLarge( pxSocket->u.xTCP.txStream );
                }

                /* In case this is a child socket, make sure the child-count of the
                 * parent socket is decreased. */
                prvTCPSetSocketCount( pxSocket );
            }
        }
    #endif /* ipconfigUSE_TCP == 1 */

    /* Socket must be unbound first, to ensure no more packets are queued on
     * it. */
    if( socketSOCKET_IS_BOUND( pxSocket ) )
    {
        /* If the network driver can iterate through 'xBoundUDPSocketsList',
         * by calling xPortHasUDPSocket(), then the IP-task must temporarily
         * suspend the scheduler to keep the list in a consistent state. */
        #if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 1 )
            {
                vTaskSuspendAll();
            }
        #endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */

        ( void ) uxListRemove( &( pxSocket->xBoundSocketListItem ) );

        #if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 1 )
            {
                ( void ) xTaskResumeAll();
            }
        #endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */
    }

    /* Now the socket is not bound the list of waiting packets can be
     * drained. */
    if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_UDP )
    {
        while( listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) ) > 0U )
        {
            pxNetworkBuffer = ipCAST_PTR_TO_TYPE_PTR( NetworkBufferDescriptor_t, listGET_OWNER_OF_HEAD_ENTRY( &( pxSocket->u.xUDP.xWaitingPacketsList ) ) );
            ( void ) uxListRemove( &( pxNetworkBuffer->xBufferListItem ) );
            vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
        }
    }

    if( pxSocket->xEventGroup != NULL )
    {
        vEventGroupDelete( pxSocket->xEventGroup );
    }

    #if ( ipconfigUSE_TCP == 1 ) && ( ipconfigHAS_DEBUG_PRINTF != 0 )
        {
            if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP )
            {
                FreeRTOS_debug_printf( ( "FreeRTOS_closesocket[%u to %lxip:%u]: buffers %lu socks %lu\n",
                                         pxSocket->usLocalPort,
                                         pxSocket->u.xTCP.ulRemoteIP,
                                         pxSocket->u.xTCP.usRemotePort,
                                         uxGetNumberOfFreeNetworkBuffers(),
                                         listCURRENT_LIST_LENGTH( &xBoundTCPSocketsList ) ) );
            }
        }
    #endif /* ( ipconfigUSE_TCP == 1 ) && ( ipconfigHAS_DEBUG_PRINTF != 0 ) */

    /* And finally, after all resources have been freed, free the socket space */
    iptraceMEM_STATS_DELETE( pxSocket );
    vPortFreeSocket( pxSocket );

    return NULL;
} /* Tested */

/*-----------------------------------------------------------*/

#if ipconfigUSE_TCP == 1

/**
 * @brief When a child socket gets closed, make sure to update the child-count of the
 *        parent. When a listening parent socket is closed, make sure no child-sockets
 *        keep a pointer to it.
 *
 * @param[in] pxSocketToDelete: The socket being closed.
 */
    static void prvTCPSetSocketCount( FreeRTOS_Socket_t const * pxSocketToDelete )
    {
        const ListItem_t * pxIterator;
        const ListItem_t * pxEnd = listGET_END_MARKER( &xBoundTCPSocketsList );
        FreeRTOS_Socket_t * pxOtherSocket;
        uint16_t usLocalPort = pxSocketToDelete->usLocalPort;

        for( pxIterator = listGET_NEXT( pxEnd );
             pxIterator != pxEnd;
             pxIterator = listGET_NEXT( pxIterator ) )
        {
            pxOtherSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxIterator ) );

            if( ( pxOtherSocket->u.xTCP.ucTCPState == ( uint8_t ) eTCP_LISTEN ) &&
                ( pxOtherSocket->usLocalPort == usLocalPort ) &&
                ( pxOtherSocket->u.xTCP.usChildCount != 0U ) )
            {
                pxOtherSocket->u.xTCP.usChildCount--;
                FreeRTOS_debug_printf( ( "Lost: Socket %u now has %u / %u child%s\n",
                                         pxOtherSocket->usLocalPort,
                                         pxOtherSocket->u.xTCP.usChildCount,
                                         pxOtherSocket->u.xTCP.usBacklog,
                                         ( pxOtherSocket->u.xTCP.usChildCount == 1U ) ? "" : "ren" ) );
                break;
            }
        }
    }


#endif /* ipconfigUSE_TCP == 1 */

/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Set the value of receive/send buffer after some preliminary checks.
 *
 * @param[in] pxSocket: The socket whose options are being set.
 * @param[in] lOptionName: The option name: either FREERTOS_SO_SNDBUF or
 *                         FREERTOS_SO_SNDBUF.
 * @param[in] pvOptionValue: The value of the option being set.
 *
 * @return If there is no error, then 0 is returned. Or a negative errno
 *         value is returned.
 */
    static BaseType_t prvSockopt_so_buffer( FreeRTOS_Socket_t * pxSocket,
                                            int32_t lOptionName,
                                            const void * pvOptionValue )
    {
        uint32_t ulNewValue;
        BaseType_t xReturn;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            FreeRTOS_debug_printf( ( "Set SO_%sBUF: wrong socket type\n",
                                     ( lOptionName == FREERTOS_SO_SNDBUF ) ? "SND" : "RCV" ) );
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else if( ( ( lOptionName == FREERTOS_SO_SNDBUF ) && ( pxSocket->u.xTCP.txStream != NULL ) ) ||
                 ( ( lOptionName == FREERTOS_SO_RCVBUF ) && ( pxSocket->u.xTCP.rxStream != NULL ) ) )
        {
            FreeRTOS_debug_printf( ( "Set SO_%sBUF: buffer already created\n",
                                     ( lOptionName == FREERTOS_SO_SNDBUF ) ? "SND" : "RCV" ) );
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            ulNewValue = *( ipPOINTER_CAST( const uint32_t *, pvOptionValue ) );

            if( lOptionName == FREERTOS_SO_SNDBUF )
            {
                /* Round up to nearest MSS size */
                ulNewValue = FreeRTOS_round_up( ulNewValue, ( uint32_t ) pxSocket->u.xTCP.usInitMSS );
                pxSocket->u.xTCP.uxTxStreamSize = ulNewValue;
            }
            else
            {
                pxSocket->u.xTCP.uxRxStreamSize = ulNewValue;
            }

            xReturn = 0;
        }

        return xReturn;
    }
#endif /* ipconfigUSE_TCP == 1 */
/*-----------------------------------------------------------*/

/* FreeRTOS_setsockopt calls itself, but in a very limited way,
 * only when FREERTOS_SO_WIN_PROPERTIES is being set. */

/**
 * @brief Set the socket options for the given socket.
 *
 * @param[in] xSocket: The socket for which the options are to be set.
 * @param[in] lLevel: Not used. Parameter is used to maintain the Berkeley sockets
 *                    standard.
 * @param[in] lOptionName: The name of the option to be set.
 * @param[in] pvOptionValue: The value of the option to be set.
 * @param[in] uxOptionLength: Not used. Parameter is used to maintain the Berkeley
 *                            sockets standard.
 *
 * @return If the option can be set with the given value, then 0 is returned. Else,
 *         an error code is returned.
 */
BaseType_t FreeRTOS_setsockopt( Socket_t xSocket,
                                int32_t lLevel,
                                int32_t lOptionName,
                                const void * pvOptionValue,
                                size_t uxOptionLength )
{
/* The standard Berkeley function returns 0 for success. */
    BaseType_t xReturn = -pdFREERTOS_ERRNO_EINVAL;
    FreeRTOS_Socket_t * pxSocket;

    pxSocket = ( FreeRTOS_Socket_t * ) xSocket;

    /* The function prototype is designed to maintain the expected Berkeley
     * sockets standard, but this implementation does not use all the parameters. */
    ( void ) lLevel;
    ( void ) uxOptionLength;

    if( ( pxSocket == NULL ) || ( pxSocket == FREERTOS_INVALID_SOCKET ) )
    {
        xReturn = -pdFREERTOS_ERRNO_EINVAL;
        return xReturn;
    }

    switch( lOptionName )
    {
        case FREERTOS_SO_RCVTIMEO:
            /* Receive time out. */
            pxSocket->xReceiveBlockTime = *( ( const TickType_t * ) pvOptionValue );
            xReturn = 0;
            break;

        case FREERTOS_SO_SNDTIMEO:
            pxSocket->xSendBlockTime = *( ( const TickType_t * ) pvOptionValue );

            if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_UDP )
            {
                /* The send time out is capped for the reason stated in the
                 * comments where ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS is defined
                 * in FreeRTOSIPConfig.h (assuming an official configuration file
                 * is being used. */
                if( pxSocket->xSendBlockTime > ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS )
                {
                    pxSocket->xSendBlockTime = ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS;
                }
            }
            else
            {
                /* For TCP socket, it isn't necessary to limit the blocking time
                 * because the FreeRTOS_send() function does not wait for a network
                 * buffer to become available. */
            }

            xReturn = 0;
            break;

            #if ( ipconfigUDP_MAX_RX_PACKETS > 0U )
                case FREERTOS_SO_UDP_MAX_RX_PACKETS:

                    if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_UDP )
                    {
                        break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                    }

                    pxSocket->u.xUDP.uxMaxPackets = *( ( const UBaseType_t * ) pvOptionValue );
                    xReturn = 0;
                    break;
            #endif /* ipconfigUDP_MAX_RX_PACKETS */

        case FREERTOS_SO_UDPCKSUM_OUT:

            /* Turn calculating of the UDP checksum on/off for this socket. If pvOptionValue
             * is anything else than NULL, the checksum generation will be turned on. */

            if( pvOptionValue == NULL )
            {
                pxSocket->ucSocketOptions &= ~( ( uint8_t ) FREERTOS_SO_UDPCKSUM_OUT );
            }
            else
            {
                pxSocket->ucSocketOptions |= ( uint8_t ) FREERTOS_SO_UDPCKSUM_OUT;
            }

            xReturn = 0;
            break;

            #if ( ipconfigUSE_CALLBACKS == 1 )
                #if ( ipconfigUSE_TCP == 1 )
                    case FREERTOS_SO_TCP_CONN_HANDLER: /* Set a callback for (dis)connection events */
                    case FREERTOS_SO_TCP_RECV_HANDLER: /* Install a callback for receiving TCP data. Supply pointer to 'F_TCP_UDP_Handler_t' (see below) */
                    case FREERTOS_SO_TCP_SENT_HANDLER: /* Install a callback for sending TCP data. Supply pointer to 'F_TCP_UDP_Handler_t' (see below) */
                #endif /* ipconfigUSE_TCP */
                case FREERTOS_SO_UDP_RECV_HANDLER:     /* Install a callback for receiving UDP data. Supply pointer to 'F_TCP_UDP_Handler_t' (see below) */
                case FREERTOS_SO_UDP_SENT_HANDLER:     /* Install a callback for sending UDP data. Supply pointer to 'F_TCP_UDP_Handler_t' (see below) */
                   {
                       #if ( ipconfigUSE_TCP == 1 )
                           {
                               UBaseType_t uxProtocol;

                               if( ( lOptionName == FREERTOS_SO_UDP_RECV_HANDLER ) ||
                                   ( lOptionName == FREERTOS_SO_UDP_SENT_HANDLER ) )
                               {
                                   uxProtocol = ( UBaseType_t ) FREERTOS_IPPROTO_UDP;
                               }
                               else
                               {
                                   uxProtocol = ( UBaseType_t ) FREERTOS_IPPROTO_TCP;
                               }

                               if( pxSocket->ucProtocol != ( uint8_t ) uxProtocol )
                               {
                                   break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                               }
                           }
                       #else /* if ( ipconfigUSE_TCP == 1 ) */
                           {
                               /* No need to check if the socket has the right
                                * protocol, because only UDP socket can be created. */
                           }
                       #endif /* ipconfigUSE_TCP */

                       switch( lOptionName )
                       {
                           #if ipconfigUSE_TCP == 1
                               case FREERTOS_SO_TCP_CONN_HANDLER:
                                   pxSocket->u.xTCP.pxHandleConnected = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( F_TCP_UDP_Handler_t, pvOptionValue )->pxOnTCPConnected;
                                   break;

                               case FREERTOS_SO_TCP_RECV_HANDLER:
                                   pxSocket->u.xTCP.pxHandleReceive = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( F_TCP_UDP_Handler_t, pvOptionValue )->pxOnTCPReceive;
                                   break;

                               case FREERTOS_SO_TCP_SENT_HANDLER:
                                   pxSocket->u.xTCP.pxHandleSent = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( F_TCP_UDP_Handler_t, pvOptionValue )->pxOnTCPSent;
                                   break;
                           #endif /* ipconfigUSE_TCP */
                           case FREERTOS_SO_UDP_RECV_HANDLER:
                               pxSocket->u.xUDP.pxHandleReceive = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( F_TCP_UDP_Handler_t, pvOptionValue )->pxOnUDPReceive;
                               break;

                           case FREERTOS_SO_UDP_SENT_HANDLER:
                               pxSocket->u.xUDP.pxHandleSent = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( F_TCP_UDP_Handler_t, pvOptionValue )->pxOnUDPSent;
                               break;

                           default:
                               /* Should it throw an error here? */
                               break;
                       }
                   }

                    xReturn = 0;
                    break;
            #endif /* ipconfigUSE_CALLBACKS */

            #if ( ipconfigUSE_TCP != 0 )
                #if ( ipconfigSOCKET_HAS_USER_SEMAPHORE != 0 )

                    /* Each socket has a semaphore on which the using task normally
                     * sleeps. */
                    case FREERTOS_SO_SET_SEMAPHORE:
                       {
                           pxSocket->pxUserSemaphore = *( ipPOINTER_CAST( SemaphoreHandle_t *, pvOptionValue ) );
                       }
                        xReturn = 0;
                        break;
                #endif /* ipconfigSOCKET_HAS_USER_SEMAPHORE */

                #if ( ipconfigSOCKET_HAS_USER_WAKE_CALLBACK != 0 )
                    case FREERTOS_SO_WAKEUP_CALLBACK:

                        /* Each socket can have a callback function that is executed
                         * when there is an event the socket's owner might want to
                         * process. */
                        /* The type cast of the pointer expression "A" to type "B" removes const qualifier from the pointed to type. */
                        pxSocket->pxUserWakeCallback = ( const SocketWakeupCallback_t ) pvOptionValue;
                        xReturn = 0;
                        break;
                #endif /* ipconfigSOCKET_HAS_USER_WAKE_CALLBACK */

                case FREERTOS_SO_SET_LOW_HIGH_WATER:
                   {
                       const LowHighWater_t * pxLowHighWater = ipPOINTER_CAST( const LowHighWater_t *, pvOptionValue );

                       if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
                       {
                           /* It is not allowed to access 'pxSocket->u.xTCP'. */
                           FreeRTOS_debug_printf( ( "FREERTOS_SO_SET_LOW_HIGH_WATER: wrong socket type\n" ) );
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       if( ( pxLowHighWater->uxLittleSpace >= pxLowHighWater->uxEnoughSpace ) ||
                           ( pxLowHighWater->uxEnoughSpace > pxSocket->u.xTCP.uxRxStreamSize ) )
                       {
                           /* Impossible values. */
                           FreeRTOS_debug_printf( ( "FREERTOS_SO_SET_LOW_HIGH_WATER: bad values\n" ) );
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       /* Send a STOP when buffer space drops below 'uxLittleSpace' bytes. */
                       pxSocket->u.xTCP.uxLittleSpace = pxLowHighWater->uxLittleSpace;
                       /* Send a GO when buffer space grows above 'uxEnoughSpace' bytes. */
                       pxSocket->u.xTCP.uxEnoughSpace = pxLowHighWater->uxEnoughSpace;
                       xReturn = 0;
                   }
                   break;

                case FREERTOS_SO_SNDBUF: /* Set the size of the send buffer, in units of MSS (TCP only) */
                case FREERTOS_SO_RCVBUF: /* Set the size of the receive buffer, in units of MSS (TCP only) */
                    xReturn = prvSockopt_so_buffer( pxSocket, lOptionName, pvOptionValue );
                    break;

                case FREERTOS_SO_WIN_PROPERTIES: /* Set all buffer and window properties in one call, parameter is pointer to WinProperties_t */
                   {
                       const WinProperties_t * pxProps;

                       if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
                       {
                           FreeRTOS_debug_printf( ( "Set SO_WIN_PROP: wrong socket type\n" ) );
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       if( ( pxSocket->u.xTCP.txStream != NULL ) || ( pxSocket->u.xTCP.rxStream != NULL ) )
                       {
                           FreeRTOS_debug_printf( ( "Set SO_WIN_PROP: buffer already created\n" ) );
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       pxProps = ipPOINTER_CAST( const WinProperties_t *, pvOptionValue );

                       xReturn = prvSockopt_so_buffer( pxSocket, FREERTOS_SO_SNDBUF, &( pxProps->lTxBufSize ) );

                       if( xReturn != 0 )
                       {
                           break; /* will return an error. */
                       }

                       xReturn = prvSockopt_so_buffer( pxSocket, FREERTOS_SO_RCVBUF, &( pxProps->lRxBufSize ) );

                       if( xReturn != 0 )
                       {
                           break; /* will return an error. */
                       }

                       #if ( ipconfigUSE_TCP_WIN == 1 )
                           {
                               pxSocket->u.xTCP.uxRxWinSize = ( uint32_t ) pxProps->lRxWinSize; /* Fixed value: size of the TCP reception window */
                               pxSocket->u.xTCP.uxTxWinSize = ( uint32_t ) pxProps->lTxWinSize; /* Fixed value: size of the TCP transmit window */
                           }
                       #else
                           {
                               pxSocket->u.xTCP.uxRxWinSize = 1U;
                               pxSocket->u.xTCP.uxTxWinSize = 1U;
                           }
                       #endif

                       /* In case the socket has already initialised its tcpWin,
                        * adapt the window size parameters */
                       if( pxSocket->u.xTCP.xTCPWindow.u.bits.bHasInit != pdFALSE_UNSIGNED )
                       {
                           pxSocket->u.xTCP.xTCPWindow.xSize.ulRxWindowLength = pxSocket->u.xTCP.uxRxWinSize * pxSocket->u.xTCP.usInitMSS;
                           pxSocket->u.xTCP.xTCPWindow.xSize.ulTxWindowLength = pxSocket->u.xTCP.uxTxWinSize * pxSocket->u.xTCP.usInitMSS;
                       }
                   }

                    xReturn = 0;
                    break;

                case FREERTOS_SO_REUSE_LISTEN_SOCKET: /* If true, the server-socket will turn into a connected socket */
                   {
                       if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
                       {
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       if( *( ( const BaseType_t * ) pvOptionValue ) != 0 )
                       {
                           pxSocket->u.xTCP.bits.bReuseSocket = pdTRUE;
                       }
                       else
                       {
                           pxSocket->u.xTCP.bits.bReuseSocket = pdFALSE;
                       }
                   }
                    xReturn = 0;
                    break;

                case FREERTOS_SO_CLOSE_AFTER_SEND: /* As soon as the last byte has been transmitted, finalise the connection */
                   {
                       if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
                       {
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       if( *( ( const BaseType_t * ) pvOptionValue ) != 0 )
                       {
                           pxSocket->u.xTCP.bits.bCloseAfterSend = pdTRUE;
                       }
                       else
                       {
                           pxSocket->u.xTCP.bits.bCloseAfterSend = pdFALSE;
                       }
                   }
                    xReturn = 0;
                    break;

                case FREERTOS_SO_SET_FULL_SIZE: /* Refuse to send packets smaller than MSS  */
                   {
                       if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
                       {
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       if( *( ( const BaseType_t * ) pvOptionValue ) != 0 )
                       {
                           pxSocket->u.xTCP.xTCPWindow.u.bits.bSendFullSize = pdTRUE;
                       }
                       else
                       {
                           pxSocket->u.xTCP.xTCPWindow.u.bits.bSendFullSize = pdFALSE;
                       }

                       if( ( pxSocket->u.xTCP.xTCPWindow.u.bits.bSendFullSize == pdFALSE_UNSIGNED ) &&
                           ( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED ) &&
                           ( FreeRTOS_outstanding( pxSocket ) != 0 ) )
                       {
                           pxSocket->u.xTCP.usTimeout = 1U; /* to set/clear bSendFullSize */
                           ( void ) xSendEventToIPTask( eTCPTimerEvent );
                       }
                   }
                    xReturn = 0;
                    break;

                case FREERTOS_SO_STOP_RX: /* Refuse to receive more packets. */
                   {
                       if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
                       {
                           break; /* will return -pdFREERTOS_ERRNO_EINVAL */
                       }

                       if( *( ( const BaseType_t * ) pvOptionValue ) != 0 )
                       {
                           pxSocket->u.xTCP.bits.bRxStopped = pdTRUE;
                       }
                       else
                       {
                           pxSocket->u.xTCP.bits.bRxStopped = pdFALSE;
                       }

                       pxSocket->u.xTCP.bits.bWinChange = pdTRUE;
                       pxSocket->u.xTCP.usTimeout = 1U; /* to set/clear bRxStopped */
                       ( void ) xSendEventToIPTask( eTCPTimerEvent );
                   }
                    xReturn = 0;
                    break;
            #endif /* ipconfigUSE_TCP == 1 */

        default:
            /* No other options are handled. */
            xReturn = -pdFREERTOS_ERRNO_ENOPROTOOPT;
            break;
    }

    return xReturn;
} /* Tested */

/*-----------------------------------------------------------*/

/**
 * @brief Find an available port number per https://tools.ietf.org/html/rfc6056.
 *
 * @param[in] xProtocol: FREERTOS_IPPROTO_TCP/FREERTOS_IPPROTO_UDP.
 *
 * @return If an available protocol port is found then that port number is returned.
 *         Or else, 0 is returned.
 */
static uint16_t prvGetPrivatePortNumber( BaseType_t xProtocol )
{
    const uint16_t usEphemeralPortCount =
        socketAUTO_PORT_ALLOCATION_MAX_NUMBER - ( socketAUTO_PORT_ALLOCATION_START_NUMBER - 1U );
    uint16_t usIterations = usEphemeralPortCount;
    uint32_t ulRandomSeed = 0;
    uint16_t usResult = 0;
    const List_t * pxList;

    #if ipconfigUSE_TCP == 1
        if( xProtocol == ( BaseType_t ) FREERTOS_IPPROTO_TCP )
        {
            pxList = &xBoundTCPSocketsList;
        }
        else
    #endif
    {
        pxList = &xBoundUDPSocketsList;
    }

    /* Avoid compiler warnings if ipconfigUSE_TCP is not defined. */
    ( void ) xProtocol;

    /* Find the next available port using the random seed as a starting
     * point. */
    do
    {
        /* Only proceed if the random number generator succeeded. */
        if( xApplicationGetRandomNumber( &( ulRandomSeed ) ) == pdFALSE )
        {
            break;
        }

        /* Map the random to a candidate port. */
        usResult =
            socketAUTO_PORT_ALLOCATION_START_NUMBER +
            ( ( ( uint16_t ) ulRandomSeed ) % usEphemeralPortCount );

        /* Check if there's already an open socket with the same protocol
         * and port. */
        if( NULL == pxListFindListItemWithValue(
                pxList,
                ( TickType_t ) FreeRTOS_htons( usResult ) ) )
        {
            usResult = FreeRTOS_htons( usResult );
            break;
        }
        else
        {
            usResult = 0;
        }

        usIterations--;
    }
    while( usIterations > 0U );

    return usResult;
}
/*-----------------------------------------------------------*/

/**
 * @brief Find a list item associated with the wanted-item.
 *
 * @param[in] pxList: The list through which the search is to be conducted.
 * @param[in] xWantedItemValue: The wanted item whose association is to be found.
 *
 * @return The list item holding the value being searched for. If nothing is found,
 *         then a NULL is returned.
 */
static const ListItem_t * pxListFindListItemWithValue( const List_t * pxList,
                                                       TickType_t xWantedItemValue )
{
    const ListItem_t * pxResult = NULL;

    if( ( xIPIsNetworkTaskReady() != pdFALSE ) && ( pxList != NULL ) )
    {
        const ListItem_t * pxIterator;
        const ListItem_t * pxEnd = listGET_END_MARKER( pxList );

        for( pxIterator = listGET_NEXT( pxEnd );
             pxIterator != pxEnd;
             pxIterator = listGET_NEXT( pxIterator ) )
        {
            if( listGET_LIST_ITEM_VALUE( pxIterator ) == xWantedItemValue )
            {
                pxResult = pxIterator;
                break;
            }
        }
    }

    return pxResult;
} /* Tested */

/*-----------------------------------------------------------*/

/**
 * @brief Find the UDP socket corresponding to the port number.
 *
 * @param[in] uxLocalPort: The port whose corresponding bound UDP socket
 *                         is to be found.
 *
 * @return The socket owning the port if found or else NULL.
 */
FreeRTOS_Socket_t * pxUDPSocketLookup( UBaseType_t uxLocalPort )
{
    const ListItem_t * pxListItem;
    FreeRTOS_Socket_t * pxSocket = NULL;

    /* Looking up a socket is quite simple, find a match with the local port.
     *
     * See if there is a list item associated with the port number on the
     * list of bound sockets. */
    pxListItem = pxListFindListItemWithValue( &xBoundUDPSocketsList, ( TickType_t ) uxLocalPort );

    if( pxListItem != NULL )
    {
        /* The owner of the list item is the socket itself. */
        pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxListItem ) );
        configASSERT( pxSocket != NULL );
    }

    return pxSocket;
}

/*-----------------------------------------------------------*/

#define sockDIGIT_COUNT    ( 3U ) /**< Each nibble is expressed in at most 3 digits such as "192". */

/**
 * @brief Convert the 32-bit representation of the IP-address to the dotted decimal
 *        notation after some checks.
 *        A safe alternative is FreeRTOS_inet_ntop4().
 *
 * @param[in] ulIPAddress: 32-bit representation of the IP-address.
 * @param[out] pcBuffer: The buffer where the dotted decimal representation will be
 *                      stored if all checks pass. The buffer must be at least 16
 *                      bytes long.
 *
 * @return If all checks pass, then the pointer returned will be same as pcBuffer
 *         and will have the address stored in the location. Else, NULL is returned.
 */
const char * FreeRTOS_inet_ntoa( uint32_t ulIPAddress,
                                 char * pcBuffer )
{
    socklen_t uxNibble;
    socklen_t uxIndex = 0;
    const uint8_t * pucAddress = ( const uint8_t * ) &( ulIPAddress );
    const char * pcResult = pcBuffer;
    const socklen_t uxSize = 16;

    for( uxNibble = 0; uxNibble < ipSIZE_OF_IPv4_ADDRESS; uxNibble++ )
    {
        uint8_t pucDigits[ sockDIGIT_COUNT ];
        uint8_t ucValue = pucAddress[ uxNibble ];
        socklen_t uxSource = ( socklen_t ) sockDIGIT_COUNT - ( socklen_t ) 1U;
        socklen_t uxNeeded;

        for( ; ; )
        {
            pucDigits[ uxSource ] = ucValue % ( uint8_t ) 10U;
            ucValue /= ( uint8_t ) 10U;

            if( uxSource == 1U )
            {
                break;
            }

            uxSource--;
        }

        pucDigits[ 0 ] = ucValue;

        /* Skip leading zeros. */
        for( uxSource = 0; uxSource < ( ( socklen_t ) sockDIGIT_COUNT - ( socklen_t ) 1U ); uxSource++ )
        {
            if( pucDigits[ uxSource ] != 0U )
            {
                break;
            }
        }

        /* Write e.g. "192.", which is 3 digits and a dot. */
        uxNeeded = ( ( socklen_t ) sockDIGIT_COUNT - uxSource ) + 1U;

        if( ( uxIndex + uxNeeded ) > uxSize )
        {
            /* The result won't fit. */
            pcResult = NULL;
            break;
        }

        for( ; uxSource < ( socklen_t ) sockDIGIT_COUNT; uxSource++ )
        {
            pcBuffer[ uxIndex ] = ( char ) ( pucDigits[ uxSource ] + ( char ) '0' );
            uxIndex++;
        }

        if( uxNibble < ( ipSIZE_OF_IPv4_ADDRESS - 1U ) )
        {
            pcBuffer[ uxIndex ] = '.';
        }
        else
        {
            pcBuffer[ uxIndex ] = '\0';
        }

        uxIndex++;
    }

    return pcResult;
}
/*-----------------------------------------------------------*/

/**
 * @brief Convert the dotted decimal format of the IP-address to the 32-bit representation.
 *
 * @param[in] xAddressFamily: The Address family to which the IP-address belongs to. Only
 *                            FREERTOS_AF_INET (IPv4) is supported.
 * @param[in] pcSource: Pointer to the string holding the dotted decimal representation of
 *                      the IP-address.
 * @param[out] pvDestination: The pointer to the address struct/variable where the converted
 *                            IP-address will be stored. The buffer must be 4 bytes long
 *                            in case of a IPv4 address.
 *
 * @return If all checks pass, then pdPASS is returned or else pdFAIL is returned.
 */
BaseType_t FreeRTOS_inet_pton( BaseType_t xAddressFamily,
                               const char * pcSource,
                               void * pvDestination )
{
    BaseType_t xResult;

    /* Printable string to struct sockaddr. */
    switch( xAddressFamily )
    {
        case FREERTOS_AF_INET:
            xResult = FreeRTOS_inet_pton4( pcSource, pvDestination );
            break;

        default:
            xResult = -pdFREERTOS_ERRNO_EAFNOSUPPORT;
            break;
    }

    return xResult;
}
/*-----------------------------------------------------------*/

/**
 * @brief Convert the 32-bit representation of the IP-address to the dotted
 *        decimal format based on the Address Family. (Only FREERTOS_AF_INET
 *        is allowed).
 *
 * @param[in] xAddressFamily: The address family of the IP-address.
 * @param[in] pvSource: Pointer to the 32-bit representation of IP-address.
 * @param[out] pcDestination: The pointer to the character array where the dotted
 *                            decimal address will be stored if every check does pass.
 * @param[in] uxSize: Size of the character array. This value makes sure that the code
 *                    doesn't write beyond it's bounds.
 *
 * @return If every check does pass, then the pointer to the pcDestination is returned
 *         holding the dotted decimal format of IP-address. Else, a NULL is returned.
 */
const char * FreeRTOS_inet_ntop( BaseType_t xAddressFamily,
                                 const void * pvSource,
                                 char * pcDestination,
                                 socklen_t uxSize )
{
    const char * pcResult;

    /* Printable struct sockaddr to string. */
    switch( xAddressFamily )
    {
        case FREERTOS_AF_INET:
            pcResult = FreeRTOS_inet_ntop4( pvSource, pcDestination, uxSize );
            break;

        default:
            /* errno should be set to pdFREERTOS_ERRNO_EAFNOSUPPORT. */
            pcResult = NULL;
            break;
    }

    return pcResult;
}
/*-----------------------------------------------------------*/

/**
 * @brief Convert the 32-bit representation of the IP-address to the dotted decimal format.
 *
 * @param[in] pvSource: The pointer to the 32-bit representation of the IP-address.
 * @param[out] pcDestination: The pointer to a character array where the string of the
 *                           dotted decimal IP format.
 * @param[in] uxSize: Size of the character array. This value makes sure that the code
 *                    doesn't write beyond it's bounds.
 *
 * @return The pointer to the string holding the dotted decimal format of the IP-address. If
 *         everything passes correctly, then the pointer being returned is the same as
 *         pcDestination, else a NULL is returned.
 */
const char * FreeRTOS_inet_ntop4( const void * pvSource,
                                  char * pcDestination,
                                  socklen_t uxSize )
{
    uint32_t ulIPAddress;
    void * pvCopyDest;
    const char * pcReturn;

    if( uxSize < 16U )
    {
        /* There must be space for "255.255.255.255". */
        pcReturn = NULL;
    }
    else
    {
        pvCopyDest = ( void * ) &ulIPAddress;
        ( void ) memcpy( pvCopyDest, pvSource, sizeof( ulIPAddress ) );
        ( void ) FreeRTOS_inet_ntoa( ulIPAddress, pcDestination );
        pcReturn = pcDestination;
    }

    return pcReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief This function converts the character string pcSource into a network address
 *        structure, then copies the network address structure to pvDestination.
 *        pvDestination is written in network byte order.
 *
 * @param[in] pcSource: The character string in holding the IP address.
 * @param[out] pvDestination: The returned network address in 32-bit network-endian format.
 *
 * @return pdPASS if the translation was successful or else pdFAIL.
 */
BaseType_t FreeRTOS_inet_pton4( const char * pcSource,
                                void * pvDestination )
{
    const uint32_t ulDecimalBase = 10U;
    uint8_t ucOctet[ socketMAX_IP_ADDRESS_OCTETS ];
    uint32_t ulReturn = 0UL, ulValue;
    UBaseType_t uxOctetNumber;
    BaseType_t xResult = pdPASS;
    const char * pcIPAddress = pcSource;
    const void * pvCopySource;

    /* Translate "192.168.2.100" to a 32-bit number, network-endian. */
    for( uxOctetNumber = 0U; uxOctetNumber < socketMAX_IP_ADDRESS_OCTETS; uxOctetNumber++ )
    {
        ulValue = 0UL;

        while( ( *pcIPAddress >= '0' ) && ( *pcIPAddress <= '9' ) )
        {
            BaseType_t xChar;

            /* Move previous read characters into the next decimal
             * position. */
            ulValue *= ulDecimalBase;

            /* Add the binary value of the ascii character. */
            xChar = ( BaseType_t ) pcIPAddress[ 0 ];
            xChar = xChar - ( BaseType_t ) '0';
            ulValue += ( uint32_t ) xChar;

            /* Move to next character in the string. */
            pcIPAddress++;
        }

        /* Check characters were read. */
        if( pcIPAddress == pcSource )
        {
            xResult = pdFAIL;
        }

        /* Check the value fits in an 8-bit number. */
        if( ulValue > 0xffUL )
        {
            xResult = pdFAIL;
        }
        else
        {
            ucOctet[ uxOctetNumber ] = ( uint8_t ) ulValue;

            /* Check the next character is as expected. */
            if( uxOctetNumber < ( socketMAX_IP_ADDRESS_OCTETS - 1U ) )
            {
                if( *pcIPAddress != '.' )
                {
                    xResult = pdFAIL;
                }
                else
                {
                    /* Move past the dot. */
                    pcIPAddress++;
                }
            }
        }

        if( xResult == pdFAIL )
        {
            /* No point going on. */
            break;
        }
    }

    if( *pcIPAddress != ( char ) 0 )
    {
        /* Expected the end of the string. */
        xResult = pdFAIL;
    }

    if( uxOctetNumber != socketMAX_IP_ADDRESS_OCTETS )
    {
        /* Didn't read enough octets. */
        xResult = pdFAIL;
    }

    if( xResult == pdPASS )
    {
        /* lint: ucOctet has been set because xResult == pdPASS. */
        ulReturn = FreeRTOS_inet_addr_quick( ucOctet[ 0 ], ucOctet[ 1 ], ucOctet[ 2 ], ucOctet[ 3 ] );
    }
    else
    {
        ulReturn = 0UL;
    }

    pvCopySource = ( const void * ) &ulReturn;
    ( void ) memcpy( pvDestination, pvCopySource, sizeof( ulReturn ) );

    return xResult;
}
/*-----------------------------------------------------------*/

/**
 * @brief Convert the IP address from "w.x.y.z" (dotted decimal) format to the 32-bit format.
 *
 * @param[in] pcIPAddress: The character string pointer holding the IP-address in the "W.X.Y.Z"
 *                         (dotted decimal) format.
 *
 * @return The 32-bit representation of IP(v4) address.
 */
uint32_t FreeRTOS_inet_addr( const char * pcIPAddress )
{
    uint32_t ulReturn = 0UL;

    /* inet_pton AF_INET target is a 4-byte 'struct in_addr'. */
    ( void ) FreeRTOS_inet_pton4( pcIPAddress, &( ulReturn ) );

    return ulReturn;
}
/*-----------------------------------------------------------*/


/**
 * @brief Function to get the local address and IP port of the given socket.
 *
 * @param[in] xSocket: Socket whose port is to be added to the pxAddress.
 * @param[out] pxAddress: Structure in which the IP address and the port number
 *                        is returned.
 *
 * @return Size of the freertos_sockaddr structure.
 */
size_t FreeRTOS_GetLocalAddress( ConstSocket_t xSocket,
                                 struct freertos_sockaddr * pxAddress )
{
    const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;

    /* IP address of local machine. */
    pxAddress->sin_addr = *ipLOCAL_IP_ADDRESS_POINTER;

    /* Local port on this machine. */
    pxAddress->sin_port = FreeRTOS_htons( pxSocket->usLocalPort );

    return sizeof( *pxAddress );
}

/*-----------------------------------------------------------*/

/**
 * @brief Wake up the user of the given socket through event-groups.
 *
 * @param[in] pxSocket: The socket whose user is to be woken up.
 */
void vSocketWakeUpUser( FreeRTOS_Socket_t * pxSocket )
{
/* _HT_ must work this out, now vSocketWakeUpUser will be called for any important
 * event or transition */
    #if ( ipconfigSOCKET_HAS_USER_SEMAPHORE == 1 )
        {
            if( pxSocket->pxUserSemaphore != NULL )
            {
                ( void ) xSemaphoreGive( pxSocket->pxUserSemaphore );
            }
        }
    #endif /* ipconfigSOCKET_HAS_USER_SEMAPHORE */

    #if ( ipconfigSOCKET_HAS_USER_WAKE_CALLBACK == 1 )
        {
            if( pxSocket->pxUserWakeCallback != NULL )
            {
                pxSocket->pxUserWakeCallback( pxSocket );
            }
        }
    #endif /* ipconfigSOCKET_HAS_USER_WAKE_CALLBACK */

    #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
        {
            if( pxSocket->pxSocketSet != NULL )
            {
                EventBits_t xSelectBits = ( pxSocket->xEventBits >> SOCKET_EVENT_BIT_COUNT ) & ( ( EventBits_t ) eSELECT_ALL );

                if( xSelectBits != 0UL )
                {
                    pxSocket->xSocketBits |= xSelectBits;
                    ( void ) xEventGroupSetBits( pxSocket->pxSocketSet->xSelectGroup, xSelectBits );
                }
            }

            pxSocket->xEventBits &= ( EventBits_t ) eSOCKET_ALL;
        }
    #endif /* ipconfigSUPPORT_SELECT_FUNCTION */

    if( ( pxSocket->xEventGroup != NULL ) && ( pxSocket->xEventBits != 0U ) )
    {
        ( void ) xEventGroupSetBits( pxSocket->xEventGroup, pxSocket->xEventBits );
    }

    pxSocket->xEventBits = 0UL;
}

/*-----------------------------------------------------------*/

#if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 1 )

/**
 * @brief This define makes it possible for network interfaces to inspect
 *        UDP messages and see if there is any UDP socket bound to a given port
 *        number.  This is probably only useful in systems with a minimum of
 *        RAM and when lots of anonymous broadcast messages come in.
 *
 * @param[in] usPortNr: the port number to look for.
 *
 * @return xFound if a socket with the port number is found.
 */
    BaseType_t xPortHasUDPSocket( uint16_t usPortNr )
    {
        BaseType_t xFound = pdFALSE;

        vTaskSuspendAll();
        {
            if( ( pxListFindListItemWithValue( &xBoundUDPSocketsList, ( TickType_t ) usPortNr ) != NULL ) )
            {
                xFound = pdTRUE;
            }
        }
        ( void ) xTaskResumeAll();

        return xFound;
    }

#endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */

/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Check if it makes any sense to wait for a connect event.
 *
 * @param[in] pxSocket: The socket trying to connect.
 *
 * @return It may return: -EINPROGRESS, -EAGAIN, or 0 for OK.
 */
    static BaseType_t bMayConnect( FreeRTOS_Socket_t const * pxSocket )
    {
        BaseType_t xResult;
        eIPTCPState_t eState = ipNUMERIC_CAST( eIPTCPState_t, pxSocket->u.xTCP.ucTCPState );

        switch( eState )
        {
            case eCLOSED:
            case eCLOSE_WAIT:
                xResult = 0;
                break;

            case eCONNECT_SYN:
                xResult = -pdFREERTOS_ERRNO_EINPROGRESS;
                break;

            case eTCP_LISTEN:
            case eSYN_FIRST:
            case eSYN_RECEIVED:
            case eESTABLISHED:
            case eFIN_WAIT_1:
            case eFIN_WAIT_2:
            case eCLOSING:
            case eLAST_ACK:
            case eTIME_WAIT:
            default:
                xResult = -pdFREERTOS_ERRNO_EAGAIN;
                break;
        }

        return xResult;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Called from #FreeRTOS_connect(): make some checks and if allowed,
 *        send a message to the IP-task to start connecting to a remote socket.
 *
 * @param[in] pxSocket: The socket attempting to connect to a remote port.
 * @param[in] pxAddress: The address the socket is trying to connect to.
 *
 * @return 0 on successful checks or a negative error code.
 */
    static BaseType_t prvTCPConnectStart( FreeRTOS_Socket_t * pxSocket,
                                          struct freertos_sockaddr const * pxAddress )
    {
        BaseType_t xResult = 0;

        if( pxAddress == NULL )
        {
            /* NULL address passed to the function. Invalid value. */
            xResult = -pdFREERTOS_ERRNO_EINVAL;
        }
        else if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdFALSE ) == pdFALSE )
        {
            /* Not a valid socket or wrong type */
            xResult = -pdFREERTOS_ERRNO_EBADF;
        }
        else if( FreeRTOS_issocketconnected( pxSocket ) > 0 )
        {
            /* The socket is already connected. */
            xResult = -pdFREERTOS_ERRNO_EISCONN;
        }
        else if( !socketSOCKET_IS_BOUND( pxSocket ) )
        {
            /* Bind the socket to the port that the client task will send from.
             * Non-standard, so the error returned is that returned by bind(). */
            xResult = FreeRTOS_bind( pxSocket, NULL, 0U );
        }
        else
        {
            /* The socket is valid, not yet connected, and already bound to a port number. */
        }

        if( xResult == 0 )
        {
            /* Check if it makes any sense to wait for a connect event, this condition
             * might change while sleeping, so it must be checked within each loop */
            xResult = bMayConnect( pxSocket ); /* -EINPROGRESS, -EAGAIN, or 0 for OK */

            /* Start the connect procedure, kernel will start working on it */
            if( xResult == 0 )
            {
                pxSocket->u.xTCP.bits.bConnPrepared = pdFALSE;
                pxSocket->u.xTCP.ucRepCount = 0U;

                FreeRTOS_debug_printf( ( "FreeRTOS_connect: %u to %lxip:%u\n",
                                         pxSocket->usLocalPort, FreeRTOS_ntohl( pxAddress->sin_addr ), FreeRTOS_ntohs( pxAddress->sin_port ) ) );

                /* Port on remote machine. */
                pxSocket->u.xTCP.usRemotePort = FreeRTOS_ntohs( pxAddress->sin_port );

                /* IP address of remote machine. */
                pxSocket->u.xTCP.ulRemoteIP = FreeRTOS_ntohl( pxAddress->sin_addr );

                /* (client) internal state: socket wants to send a connect. */
                vTCPStateChange( pxSocket, eCONNECT_SYN );

                /* To start an active connect. */
                pxSocket->u.xTCP.usTimeout = 1U;

                if( xSendEventToIPTask( eTCPTimerEvent ) != pdPASS )
                {
                    xResult = -pdFREERTOS_ERRNO_ECANCELED;
                }
            }
        }

        return xResult;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Connect to a remote port.
 *
 * @param[in] xClientSocket: The socket initiating the connection.
 * @param[in] pxAddress: The address of the remote socket.
 * @param[in] xAddressLength: This parameter is not used. It is kept in
 *                   the function signature to adhere to the Berkeley
 *                   sockets standard.
 *
 * @return 0 is returned on a successful connection, else a negative
 *         error code is returned.
 */
    BaseType_t FreeRTOS_connect( Socket_t xClientSocket,
                                 struct freertos_sockaddr * pxAddress,
                                 socklen_t xAddressLength )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xClientSocket;
        TickType_t xRemainingTime;
        BaseType_t xTimed = pdFALSE;
        BaseType_t xResult = -pdFREERTOS_ERRNO_EINVAL;
        TimeOut_t xTimeOut;

        ( void ) xAddressLength;

        xResult = prvTCPConnectStart( pxSocket, pxAddress );

        if( xResult == 0 )
        {
            /* And wait for the result */
            for( ; ; )
            {
                if( xTimed == pdFALSE )
                {
                    /* Only in the first round, check for non-blocking */
                    xRemainingTime = pxSocket->xReceiveBlockTime;

                    if( xRemainingTime == ( TickType_t ) 0 )
                    {
                        /* Not yet connected, correct state, non-blocking. */
                        xResult = -pdFREERTOS_ERRNO_EWOULDBLOCK;
                        break;
                    }

                    /* Don't get here a second time. */
                    xTimed = pdTRUE;

                    /* Fetch the current time */
                    vTaskSetTimeOutState( &xTimeOut );
                }

                /* Did it get connected while sleeping ? */
                xResult = FreeRTOS_issocketconnected( pxSocket );

                /* Returns positive when connected, negative means an error */
                if( xResult < 0 )
                {
                    /* Return the error */
                    break;
                }

                if( xResult > 0 )
                {
                    /* Socket now connected, return a zero */
                    xResult = 0;
                    break;
                }

                /* Is it allowed to sleep more? */
                if( xTaskCheckForTimeOut( &xTimeOut, &xRemainingTime ) != pdFALSE )
                {
                    xResult = -pdFREERTOS_ERRNO_ETIMEDOUT;
                    break;
                }

                /* Go sleeping until we get any down-stream event */
                ( void ) xEventGroupWaitBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_CONNECT, pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, xRemainingTime );
            }
        }

        return xResult;
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Accept a connection on an listening socket.
 *
 * @param[in] xServerSocket: The socket in listening mode.
 * @param[out] pxAddress: The address of the machine trying to connect to this node
 *                        is returned in this pointer.
 * @param[out] pxAddressLength: The length of the address of the remote machine.
 *
 * @return FreeRTOS_accept: can return a new connected socket if the server socket
 *         is in listen mode and receives a connection request. The new socket will
 *         be bound already to the same port number as the listening socket.
 */
    Socket_t FreeRTOS_accept( Socket_t xServerSocket,
                              struct freertos_sockaddr * pxAddress,
                              socklen_t * pxAddressLength )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xServerSocket;
        FreeRTOS_Socket_t * pxClientSocket = NULL;
        TickType_t xRemainingTime;
        BaseType_t xTimed = pdFALSE, xAsk = pdFALSE;
        TimeOut_t xTimeOut;
        IPStackEvent_t xAskEvent;

        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdTRUE ) == pdFALSE )
        {
            /* Not a valid socket or wrong type */
            pxClientSocket = FREERTOS_INVALID_SOCKET;
        }
        else if( ( pxSocket->u.xTCP.bits.bReuseSocket == pdFALSE_UNSIGNED ) &&
                 ( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eTCP_LISTEN ) )
        {
            /* Parent socket is not in listening mode */
            pxClientSocket = FREERTOS_INVALID_SOCKET;
        }
        else
        {
            /* Loop will stop with breaks. */
            for( ; ; )
            {
                /* Is there a new client? */
                vTaskSuspendAll();
                {
                    if( pxSocket->u.xTCP.bits.bReuseSocket == pdFALSE_UNSIGNED )
                    {
                        pxClientSocket = pxSocket->u.xTCP.pxPeerSocket;
                    }
                    else
                    {
                        pxClientSocket = pxSocket;
                    }

                    if( pxClientSocket != NULL )
                    {
                        pxSocket->u.xTCP.pxPeerSocket = NULL;

                        /* Is it still not taken ? */
                        if( pxClientSocket->u.xTCP.bits.bPassAccept != pdFALSE_UNSIGNED )
                        {
                            pxClientSocket->u.xTCP.bits.bPassAccept = pdFALSE;
                        }
                        else
                        {
                            pxClientSocket = NULL;
                        }
                    }
                }
                ( void ) xTaskResumeAll();

                if( pxClientSocket != NULL )
                {
                    if( pxAddress != NULL )
                    {
                        /* IP address of remote machine. */
                        pxAddress->sin_addr = FreeRTOS_ntohl( pxClientSocket->u.xTCP.ulRemoteIP );

                        /* Port on remote machine. */
                        pxAddress->sin_port = FreeRTOS_ntohs( pxClientSocket->u.xTCP.usRemotePort );
                    }

                    if( pxAddressLength != NULL )
                    {
                        *pxAddressLength = sizeof( *pxAddress );
                    }

                    if( pxSocket->u.xTCP.bits.bReuseSocket == pdFALSE_UNSIGNED )
                    {
                        xAsk = pdTRUE;
                    }
                }

                if( xAsk != pdFALSE )
                {
                    /* Ask to set an event in 'xEventGroup' as soon as a new
                     * client gets connected for this listening socket. */
                    xAskEvent.eEventType = eTCPAcceptEvent;
                    xAskEvent.pvData = pxSocket;
                    ( void ) xSendEventStructToIPTask( &xAskEvent, portMAX_DELAY );
                }

                if( pxClientSocket != NULL )
                {
                    break;
                }

                if( xTimed == pdFALSE )
                {
                    /* Only in the first round, check for non-blocking */
                    xRemainingTime = pxSocket->xReceiveBlockTime;

                    if( xRemainingTime == ( TickType_t ) 0 )
                    {
                        break;
                    }

                    /* Don't get here a second time */
                    xTimed = pdTRUE;

                    /* Fetch the current time */
                    vTaskSetTimeOutState( &xTimeOut );
                }

                /* Has the timeout been reached? */
                if( xTaskCheckForTimeOut( &xTimeOut, &xRemainingTime ) != pdFALSE )
                {
                    break;
                }

                /* Go sleeping until we get any down-stream event */
                ( void ) xEventGroupWaitBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_ACCEPT, pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, xRemainingTime );
            }
        }

        return pxClientSocket;
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Read incoming data from a TCP socket. Only after the last
 *        byte has been read, a close error might be returned.
 *
 * @param[in] xSocket: The socket owning the connection.
 * @param[out] pvBuffer: The buffer to store the incoming data in.
 * @param[in] uxBufferLength: The length of the buffer so that the function
 *                            does not do out of bound access.
 * @param[in] xFlags: The flags for conveying preference. The values
 *                    FREERTOS_MSG_DONTWAIT, FREERTOS_ZERO_COPY and/or
 *                    FREERTOS_MSG_PEEK can be used.
 *
 * @return The number of bytes actually received and stored in the pvBuffer.
 */
    BaseType_t FreeRTOS_recv( Socket_t xSocket,
                              void * pvBuffer,
                              size_t uxBufferLength,
                              BaseType_t xFlags )
    {
        BaseType_t xByteCount;
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
        TickType_t xRemainingTime;
        BaseType_t xTimed = pdFALSE;
        TimeOut_t xTimeOut;
        EventBits_t xEventBits = ( EventBits_t ) 0;

        /* Check if the socket is valid, has type TCP and if it is bound to a
         * port. */
        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdTRUE ) == pdFALSE )
        {
            xByteCount = -pdFREERTOS_ERRNO_EINVAL;
        }
        else if( ( ( ( uint32_t ) xFlags & ( uint32_t ) FREERTOS_ZERO_COPY ) != 0U ) &&
                 ( pvBuffer == NULL ) )
        {
            /* In zero-copy mode, pvBuffer is a pointer to a pointer ( not NULL ). */
            xByteCount = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            if( pxSocket->u.xTCP.rxStream != NULL )
            {
                xByteCount = ( BaseType_t ) uxStreamBufferGetSize( pxSocket->u.xTCP.rxStream );
            }
            else
            {
                xByteCount = 0;
            }

            while( xByteCount == 0 )
            {
                switch( ipNUMERIC_CAST( eIPTCPState_t, pxSocket->u.xTCP.ucTCPState ) )
                {
                    case eCLOSED:
                    case eCLOSE_WAIT: /* (server + client) waiting for a connection termination request from the local user. */
                    case eCLOSING:    /* (server + client) waiting for a connection termination request acknowledgement from the remote TCP. */

                        if( pxSocket->u.xTCP.bits.bMallocError != pdFALSE_UNSIGNED )
                        {
                            /* The no-memory error has priority above the non-connected error.
                             * Both are fatal and will lead to closing the socket. */
                            xByteCount = -pdFREERTOS_ERRNO_ENOMEM;
                        }
                        else
                        {
                            xByteCount = -pdFREERTOS_ERRNO_ENOTCONN;
                        }

                        break;

                    case eTCP_LISTEN:
                    case eCONNECT_SYN:
                    case eSYN_FIRST:
                    case eSYN_RECEIVED:
                    case eESTABLISHED:
                    case eFIN_WAIT_1:
                    case eFIN_WAIT_2:
                    case eLAST_ACK:
                    case eTIME_WAIT:
                    default:
                        /* Nothing. */
                        break;
                }

                if( xByteCount < 0 )
                {
                    break;
                }

                if( xTimed == pdFALSE )
                {
                    /* Only in the first round, check for non-blocking. */
                    xRemainingTime = pxSocket->xReceiveBlockTime;

                    if( xRemainingTime == ( TickType_t ) 0 )
                    {
                        #if ( ipconfigSUPPORT_SIGNALS != 0 )
                            {
                                /* Just check for the interrupt flag. */
                                xEventBits = xEventGroupWaitBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_INTR,
                                                                  pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, socketDONT_BLOCK );
                            }
                        #endif /* ipconfigSUPPORT_SIGNALS */
                        break;
                    }

                    if( ( ( uint32_t ) xFlags & ( uint32_t ) FREERTOS_MSG_DONTWAIT ) != 0U )
                    {
                        break;
                    }

                    /* Don't get here a second time. */
                    xTimed = pdTRUE;

                    /* Fetch the current time. */
                    vTaskSetTimeOutState( &xTimeOut );
                }

                /* Has the timeout been reached? */
                if( xTaskCheckForTimeOut( &xTimeOut, &xRemainingTime ) != pdFALSE )
                {
                    break;
                }

                /* Block until there is a down-stream event. */
                xEventBits = xEventGroupWaitBits( pxSocket->xEventGroup,
                                                  ( EventBits_t ) eSOCKET_RECEIVE | ( EventBits_t ) eSOCKET_CLOSED | ( EventBits_t ) eSOCKET_INTR,
                                                  pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, xRemainingTime );
                #if ( ipconfigSUPPORT_SIGNALS != 0 )
                    {
                        if( ( xEventBits & ( EventBits_t ) eSOCKET_INTR ) != 0U )
                        {
                            break;
                        }
                    }
                #else
                    {
                        ( void ) xEventBits;
                    }
                #endif /* ipconfigSUPPORT_SIGNALS */

                if( pxSocket->u.xTCP.rxStream != NULL )
                {
                    xByteCount = ( BaseType_t ) uxStreamBufferGetSize( pxSocket->u.xTCP.rxStream );
                }
                else
                {
                    xByteCount = 0;
                }
            }

            #if ( ipconfigSUPPORT_SIGNALS != 0 )
                if( ( xEventBits & ( EventBits_t ) eSOCKET_INTR ) != 0U )
                {
                    if( ( xEventBits & ( ( EventBits_t ) eSOCKET_RECEIVE | ( EventBits_t ) eSOCKET_CLOSED ) ) != 0U )
                    {
                        /* Shouldn't have cleared other flags. */
                        xEventBits &= ~( ( EventBits_t ) eSOCKET_INTR );
                        ( void ) xEventGroupSetBits( pxSocket->xEventGroup, xEventBits );
                    }

                    xByteCount = -pdFREERTOS_ERRNO_EINTR;
                }
                else
            #endif /* ipconfigSUPPORT_SIGNALS */

            if( xByteCount > 0 )
            {
                if( ( ( uint32_t ) xFlags & ( uint32_t ) FREERTOS_ZERO_COPY ) == 0U )
                {
                    BaseType_t xIsPeek = ( ( ( uint32_t ) xFlags & ( uint32_t ) FREERTOS_MSG_PEEK ) != 0U ) ? 1L : 0L;

                    xByteCount = ( BaseType_t )
                                 uxStreamBufferGet( pxSocket->u.xTCP.rxStream,
                                                    0UL,
                                                    ipPOINTER_CAST( uint8_t *, pvBuffer ),
                                                    ( size_t ) uxBufferLength,
                                                    xIsPeek );

                    if( pxSocket->u.xTCP.bits.bLowWater != pdFALSE_UNSIGNED )
                    {
                        /* We had reached the low-water mark, now see if the flag
                         * can be cleared */
                        size_t uxFrontSpace = uxStreamBufferFrontSpace( pxSocket->u.xTCP.rxStream );

                        if( uxFrontSpace >= pxSocket->u.xTCP.uxEnoughSpace )
                        {
                            pxSocket->u.xTCP.bits.bLowWater = pdFALSE;
                            pxSocket->u.xTCP.bits.bWinChange = pdTRUE;
                            pxSocket->u.xTCP.usTimeout = 1U; /* because bLowWater is cleared. */
                            ( void ) xSendEventToIPTask( eTCPTimerEvent );
                        }
                    }
                }
                else
                {
                    /* Zero-copy reception of data: pvBuffer is a pointer to a pointer. */
                    xByteCount = ( BaseType_t ) uxStreamBufferGetPtr( pxSocket->u.xTCP.rxStream, ipPOINTER_CAST( uint8_t * *, pvBuffer ) );
                }
            }
            else
            {
                /* Nothing. */
            }
        } /* prvValidSocket() */

        return xByteCount;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Called from FreeRTOS_send(): some checks which will be done before
 *        sending a TCP packed.
 *
 * @param[in] pxSocket: The socket owning the connection.
 * @param[in] uxDataLength: The length of the data to be sent.
 *
 * @return 0: representing OK, else a negative error code will be returned.
 */
    static int32_t prvTCPSendCheck( FreeRTOS_Socket_t * pxSocket,
                                    size_t uxDataLength )
    {
        int32_t xResult = 1;

        /* Is this a socket of type TCP and is it already bound to a port number ? */
        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdTRUE ) == pdFALSE )
        {
            xResult = -pdFREERTOS_ERRNO_EINVAL;
        }
        else if( pxSocket->u.xTCP.bits.bMallocError != pdFALSE_UNSIGNED )
        {
            xResult = -pdFREERTOS_ERRNO_ENOMEM;
        }
        else if( ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCLOSED ) ||
                 ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCLOSE_WAIT ) ||
                 ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCLOSING ) )
        {
            xResult = -pdFREERTOS_ERRNO_ENOTCONN;
        }
        else if( pxSocket->u.xTCP.bits.bFinSent != pdFALSE_UNSIGNED )
        {
            /* This TCP connection is closing already, the FIN flag has been sent.
             * Maybe it is still delivering or receiving data.
             * Return OK in order not to get closed/deleted too quickly */
            xResult = 0;
        }
        else if( uxDataLength == 0UL )
        {
            /* send() is being called to send zero bytes */
            xResult = 0;
        }
        else if( pxSocket->u.xTCP.txStream == NULL )
        {
            /* Create the outgoing stream only when it is needed */
            ( void ) prvTCPCreateStream( pxSocket, pdFALSE );

            if( pxSocket->u.xTCP.txStream == NULL )
            {
                xResult = -pdFREERTOS_ERRNO_ENOMEM;
            }
        }
        else
        {
            /* Nothing. */
        }

        return xResult;
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Get a direct pointer to the circular transmit buffer.
 *
 * @param[in] xSocket: The socket owning the buffer.
 * @param[in] pxLength: This will contain the number of bytes that may be written.
 *
 * @return Head of the circular transmit buffer if all checks pass. Or else, NULL
 *         is returned.
 */
    uint8_t * FreeRTOS_get_tx_head( ConstSocket_t xSocket,
                                    BaseType_t * pxLength )
    {
        uint8_t * pucReturn = NULL;
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        StreamBuffer_t * pxBuffer = NULL;

        *pxLength = 0;

        /* Confirm that this is a TCP socket before dereferencing structure
         * member pointers. */
        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdFALSE ) == pdTRUE )
        {
            pxBuffer = pxSocket->u.xTCP.txStream;

            if( pxBuffer != NULL )
            {
                BaseType_t xSpace = ( BaseType_t ) uxStreamBufferGetSpace( pxBuffer );
                BaseType_t xRemain = ( BaseType_t ) pxBuffer->LENGTH - ( BaseType_t ) pxBuffer->uxHead;

                *pxLength = FreeRTOS_min_BaseType( xSpace, xRemain );
                pucReturn = &( pxBuffer->ucArray[ pxBuffer->uxHead ] );
            }
        }

        return pucReturn;
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Send data using a TCP socket. It is not necessary to have the socket
 *        connected already. Outgoing data will be stored and delivered as soon as
 *        the socket gets connected.
 *
 * @param[in] xSocket: The socket owning the connection.
 * @param[in] pvBuffer: The buffer containing the data.
 * @param[in] uxDataLength: The length of the data to be added.
 * @param[in] xFlags: This parameter is not used. (zero or FREERTOS_MSG_DONTWAIT).
 *
 * @return The number of bytes actually sent. Zero when nothing could be sent
 *         or a negative error code in case an error occurred.
 */
    BaseType_t FreeRTOS_send( Socket_t xSocket,
                              const void * pvBuffer,
                              size_t uxDataLength,
                              BaseType_t xFlags )
    {
        BaseType_t xByteCount = -pdFREERTOS_ERRNO_EINVAL;
        BaseType_t xBytesLeft;
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
        TickType_t xRemainingTime;
        BaseType_t xTimed = pdFALSE;
        TimeOut_t xTimeOut;
        BaseType_t xCloseAfterSend;
        const uint8_t * pucSource = ipPOINTER_CAST( const uint8_t *, pvBuffer );

        /* Prevent compiler warnings about unused parameters.  The parameter
         * may be used in future versions. */
        ( void ) xFlags;

        if( pvBuffer != NULL )
        {
            xByteCount = ( BaseType_t ) prvTCPSendCheck( pxSocket, uxDataLength );
        }

        if( xByteCount > 0 )
        {
            /* xBytesLeft is number of bytes to send, will count to zero. */
            xBytesLeft = ( BaseType_t ) uxDataLength;

            /* xByteCount is number of bytes that can be sent now. */
            xByteCount = ( BaseType_t ) uxStreamBufferGetSpace( pxSocket->u.xTCP.txStream );

            /* While there are still bytes to be sent. */
            while( xBytesLeft > 0 )
            {
                /* If txStream has space. */
                if( xByteCount > 0 )
                {
                    /* Don't send more than necessary. */
                    if( xByteCount > xBytesLeft )
                    {
                        xByteCount = xBytesLeft;
                    }

                    /* Is the close-after-send flag set and is this really the
                     * last transmission? */
                    if( ( pxSocket->u.xTCP.bits.bCloseAfterSend != pdFALSE_UNSIGNED ) && ( xByteCount == xBytesLeft ) )
                    {
                        xCloseAfterSend = pdTRUE;
                    }
                    else
                    {
                        xCloseAfterSend = pdFALSE;
                    }

                    /* The flag 'bCloseAfterSend' can be set before sending data
                     * using setsockopt()
                     *
                     * When the last data packet is being sent out, a FIN flag will
                     * be included to let the peer know that no more data is to be
                     * expected.  The use of 'bCloseAfterSend' is not mandatory, it
                     * is just a faster way of transferring files (e.g. when using
                     * FTP). */
                    if( xCloseAfterSend != pdFALSE )
                    {
                        /* Now suspend the scheduler: sending the last data and
                         * setting bCloseRequested must be done together */
                        vTaskSuspendAll();
                        pxSocket->u.xTCP.bits.bCloseRequested = pdTRUE;
                    }

                    xByteCount = ( BaseType_t ) uxStreamBufferAdd( pxSocket->u.xTCP.txStream, 0UL, pucSource, ( size_t ) xByteCount );

                    if( xCloseAfterSend != pdFALSE )
                    {
                        /* Now when the IP-task transmits the data, it will also
                         * see that bCloseRequested is true and include the FIN
                         * flag to start closure of the connection. */
                        ( void ) xTaskResumeAll();
                    }

                    /* Send a message to the IP-task so it can work on this
                    * socket.  Data is sent, let the IP-task work on it. */
                    pxSocket->u.xTCP.usTimeout = 1U;

                    if( xIsCallingFromIPTask() == pdFALSE )
                    {
                        /* Only send a TCP timer event when not called from the
                         * IP-task. */
                        ( void ) xSendEventToIPTask( eTCPTimerEvent );
                    }

                    xBytesLeft -= xByteCount;

                    if( xBytesLeft == 0 )
                    {
                        break;
                    }

                    /* As there are still bytes left to be sent, increase the
                     * data pointer. */
                    pucSource = &( pucSource[ xByteCount ] );
                }

                /* Not all bytes have been sent. In case the socket is marked as
                 * blocking sleep for a while. */
                if( xTimed == pdFALSE )
                {
                    /* Only in the first round, check for non-blocking. */
                    xRemainingTime = pxSocket->xSendBlockTime;

                    #if ( ipconfigUSE_CALLBACKS != 0 )
                        {
                            if( xIsCallingFromIPTask() != pdFALSE )
                            {
                                /* If this send function is called from within a
                                 * call-back handler it may not block, otherwise
                                 * chances would be big to get a deadlock: the IP-task
                                 * waiting for itself. */
                                xRemainingTime = ( TickType_t ) 0;
                            }
                        }
                    #endif /* ipconfigUSE_CALLBACKS */

                    if( xRemainingTime == ( TickType_t ) 0 )
                    {
                        break;
                    }

                    if( ( ( uint32_t ) xFlags & ( uint32_t ) FREERTOS_MSG_DONTWAIT ) != 0U )
                    {
                        break;
                    }

                    /* Don't get here a second time. */
                    xTimed = pdTRUE;

                    /* Fetch the current time. */
                    vTaskSetTimeOutState( &xTimeOut );
                }
                else
                {
                    /* Has the timeout been reached? */
                    if( xTaskCheckForTimeOut( &xTimeOut, &xRemainingTime ) != pdFALSE )
                    {
                        break;
                    }
                }

                /* Go sleeping until down-stream events are received. */
                ( void ) xEventGroupWaitBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_SEND | ( EventBits_t ) eSOCKET_CLOSED,
                                              pdTRUE /*xClearOnExit*/, pdFALSE /*xWaitAllBits*/, xRemainingTime );

                xByteCount = ( BaseType_t ) uxStreamBufferGetSpace( pxSocket->u.xTCP.txStream );
            }

            /* How much was actually sent? */
            xByteCount = ( ( BaseType_t ) uxDataLength ) - xBytesLeft;

            if( xByteCount == 0 )
            {
                if( pxSocket->u.xTCP.ucTCPState > ( uint8_t ) eESTABLISHED )
                {
                    xByteCount = ( BaseType_t ) -pdFREERTOS_ERRNO_ENOTCONN;
                }
                else
                {
                    if( ipconfigTCP_MAY_LOG_PORT( pxSocket->usLocalPort ) )
                    {
                        FreeRTOS_debug_printf( ( "FreeRTOS_send: %u -> %lxip:%d: no space\n",
                                                 pxSocket->usLocalPort,
                                                 pxSocket->u.xTCP.ulRemoteIP,
                                                 pxSocket->u.xTCP.usRemotePort ) );
                    }

                    xByteCount = ( BaseType_t ) -pdFREERTOS_ERRNO_ENOSPC;
                }
            }
        }

        return xByteCount;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Request to put a socket in listen mode.
 *
 * @param[in] xSocket: the socket to be put in listening mode.
 * @param[in] xBacklog: Maximum number of child sockets.
 *
 * @return 0 in case of success, or else a negative error code is
 *         returned.
 */
    BaseType_t FreeRTOS_listen( Socket_t xSocket,
                                BaseType_t xBacklog )
    {
        FreeRTOS_Socket_t * pxSocket;
        BaseType_t xResult = 0;

        pxSocket = ( FreeRTOS_Socket_t * ) xSocket;

        /* listen() is allowed for a valid TCP socket in Closed state and already
         * bound. */
        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdTRUE ) == pdFALSE )
        {
            xResult = -pdFREERTOS_ERRNO_EOPNOTSUPP;
        }
        else if( ( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eCLOSED ) && ( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eCLOSE_WAIT ) )
        {
            /* Socket is in a wrong state. */
            xResult = -pdFREERTOS_ERRNO_EOPNOTSUPP;
        }
        else
        {
            /* Backlog is interpreted here as "the maximum number of child
             * sockets. */
            pxSocket->u.xTCP.usBacklog = ( uint16_t ) FreeRTOS_min_int32( ( int32_t ) 0xffff, ( int32_t ) xBacklog );

            /* This cleaning is necessary only if a listening socket is being
             * reused as it might have had a previous connection. */
            if( pxSocket->u.xTCP.bits.bReuseSocket != pdFALSE_UNSIGNED )
            {
                if( pxSocket->u.xTCP.rxStream != NULL )
                {
                    vStreamBufferClear( pxSocket->u.xTCP.rxStream );
                }

                if( pxSocket->u.xTCP.txStream != NULL )
                {
                    vStreamBufferClear( pxSocket->u.xTCP.txStream );
                }

                ( void ) memset( pxSocket->u.xTCP.xPacket.u.ucLastPacket, 0, sizeof( pxSocket->u.xTCP.xPacket.u.ucLastPacket ) );
                ( void ) memset( &pxSocket->u.xTCP.xTCPWindow, 0, sizeof( pxSocket->u.xTCP.xTCPWindow ) );
                ( void ) memset( &pxSocket->u.xTCP.bits, 0, sizeof( pxSocket->u.xTCP.bits ) );

                /* Now set the bReuseSocket flag again, because the bits have
                 * just been cleared. */
                pxSocket->u.xTCP.bits.bReuseSocket = pdTRUE;
            }

            vTCPStateChange( pxSocket, eTCP_LISTEN );
        }

        return xResult;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Shutdown - This function will shut down the connection in both
 *        directions. However, it will first deliver all data queued for
 *        transmission, and also it will first wait to receive any missing
 *        packets from the peer.
 *
 * @param[in] xSocket: The socket owning the connection.
 * @param[in] xHow: Not used. Just present to stick to Berkeley standard.
 *
 * @return 0 on successful shutdown or else a negative error code.
 */
    BaseType_t FreeRTOS_shutdown( Socket_t xSocket,
                                  BaseType_t xHow )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xResult;

        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdTRUE ) == pdFALSE )
        {
            /*_RB_ Is this comment correct?  The socket is not of a type that
             * supports the listen() operation. */
            xResult = -pdFREERTOS_ERRNO_EOPNOTSUPP;
        }
        else if( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eESTABLISHED )
        {
            /*_RB_ Is this comment correct?  The socket is not of a type that
             * supports the listen() operation. */
            xResult = -pdFREERTOS_ERRNO_EOPNOTSUPP;
        }
        else
        {
            pxSocket->u.xTCP.bits.bUserShutdown = pdTRUE_UNSIGNED;

            /* Let the IP-task perform the shutdown of the connection. */
            pxSocket->u.xTCP.usTimeout = 1U;
            ( void ) xSendEventToIPTask( eTCPTimerEvent );
            xResult = 0;
        }

        ( void ) xHow;

        return xResult;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief A TCP timer has expired, now check all TCP sockets for:
 *        - Active connect
 *        - Send a delayed ACK
 *        - Send new data
 *        - Send a keep-alive packet
 *        - Check for timeout (in non-connected states only)
 *
 * @param[in] xWillSleep: Whether the calling task is going to sleep.
 *
 * @return Minimum amount of time before the timer shall expire.
 */
    TickType_t xTCPTimerCheck( BaseType_t xWillSleep )
    {
        FreeRTOS_Socket_t * pxSocket;
        TickType_t xShortest = pdMS_TO_TICKS( ( TickType_t ) ipTCP_TIMER_PERIOD_MS );
        TickType_t xNow = xTaskGetTickCount();
        static TickType_t xLastTime = 0U;
        TickType_t xDelta = xNow - xLastTime;
        const ListItem_t * pxEnd = listGET_END_MARKER( &xBoundTCPSocketsList );
        const ListItem_t * pxIterator = ( const ListItem_t * ) listGET_HEAD_ENTRY( &xBoundTCPSocketsList );

        xLastTime = xNow;

        if( xDelta == 0U )
        {
            xDelta = 1U;
        }

        while( pxIterator != pxEnd )
        {
            pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxIterator ) );
            pxIterator = ( ListItem_t * ) listGET_NEXT( pxIterator );

            /* Sockets with 'timeout == 0' do not need any regular attention. */
            if( pxSocket->u.xTCP.usTimeout == 0U )
            {
                continue;
            }

            if( xDelta < ( TickType_t ) pxSocket->u.xTCP.usTimeout )
            {
                pxSocket->u.xTCP.usTimeout = ( uint16_t ) ( ( ( TickType_t ) pxSocket->u.xTCP.usTimeout ) - xDelta );
            }
            else
            {
                BaseType_t xRc;

                pxSocket->u.xTCP.usTimeout = 0U;
                xRc = xTCPSocketCheck( pxSocket );

                /* Within this function, the socket might want to send a delayed
                 * ack or send out data or whatever it needs to do. */
                if( xRc < 0 )
                {
                    /* Continue because the socket was deleted. */
                    continue;
                }
            }

            /* In xEventBits the driver may indicate that the socket has
             * important events for the user.  These are only done just before the
             * IP-task goes to sleep. */
            if( pxSocket->xEventBits != 0U )
            {
                if( xWillSleep != pdFALSE )
                {
                    /* The IP-task is about to go to sleep, so messages can be
                     * sent to the socket owners. */
                    vSocketWakeUpUser( pxSocket );
                }
                else
                {
                    /* Or else make sure this will be called again to wake-up
                     * the sockets' owner. */
                    xShortest = ( TickType_t ) 0;
                }
            }

            if( ( pxSocket->u.xTCP.usTimeout != 0U ) && ( xShortest > ( TickType_t ) pxSocket->u.xTCP.usTimeout ) )
            {
                xShortest = ( TickType_t ) pxSocket->u.xTCP.usTimeout;
            }
        }

        return xShortest;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief As multiple sockets may be bound to the same local port number
 *        looking up a socket is a little more complex: Both a local port,
 *        and a remote port and IP address are being used to find a match.
 *        For a socket in listening mode, the remote port and IP address
 *        are both 0.
 *
 * @param[in] ulLocalIP: Local IP address. Ignored for now.
 * @param[in] uxLocalPort: Local port number.
 * @param[in] ulRemoteIP: Remote (peer) IP address.
 * @param[in] uxRemotePort: Remote (peer) port.
 *
 * @return The socket which was found.
 */
    FreeRTOS_Socket_t * pxTCPSocketLookup( uint32_t ulLocalIP,
                                           UBaseType_t uxLocalPort,
                                           uint32_t ulRemoteIP,
                                           UBaseType_t uxRemotePort )
    {
        const ListItem_t * pxIterator;
        FreeRTOS_Socket_t * pxResult = NULL, * pxListenSocket = NULL;
        const ListItem_t * pxEnd = listGET_END_MARKER( &xBoundTCPSocketsList );

        /* Parameter not yet supported. */
        ( void ) ulLocalIP;

        for( pxIterator = listGET_NEXT( pxEnd );
             pxIterator != pxEnd;
             pxIterator = listGET_NEXT( pxIterator ) )
        {
            FreeRTOS_Socket_t * pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxIterator ) );

            if( pxSocket->usLocalPort == ( uint16_t ) uxLocalPort )
            {
                if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eTCP_LISTEN )
                {
                    /* If this is a socket listening to uxLocalPort, remember it
                     * in case there is no perfect match. */
                    pxListenSocket = pxSocket;
                }
                else if( ( pxSocket->u.xTCP.usRemotePort == ( uint16_t ) uxRemotePort ) && ( pxSocket->u.xTCP.ulRemoteIP == ulRemoteIP ) )
                {
                    /* For sockets not in listening mode, find a match with
                     * xLocalPort, ulRemoteIP AND xRemotePort. */
                    pxResult = pxSocket;
                    break;
                }
                else
                {
                    /* This 'pxSocket' doesn't match. */
                }
            }
        }

        if( pxResult == NULL )
        {
            /* An exact match was not found, maybe a listening socket was
             * found. */
            pxResult = pxListenSocket;
        }

        return pxResult;
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief For the web server: borrow the circular Rx buffer for inspection.
 *        HTML driver wants to see if a sequence of 13/10/13/10 is available.
 *
 * @param[in] xSocket: The socket whose Rx stream is to be returned.
 *
 * @return The Rx stream of the socket if all checks pass, else NULL.
 */
    const struct xSTREAM_BUFFER * FreeRTOS_get_rx_buf( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        const struct xSTREAM_BUFFER * pxReturn = NULL;


        /* Confirm that this is a TCP socket before dereferencing structure
         * member pointers. */
        if( prvValidSocket( pxSocket, FREERTOS_IPPROTO_TCP, pdFALSE ) == pdTRUE )
        {
            pxReturn = pxSocket->u.xTCP.rxStream;
        }

        return pxReturn;
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Create the stream buffer for the given socket.
 *
 * @param[in] pxSocket: the socket to create the stream for.
 * @param[in] xIsInputStream: Is this input stream? pdTRUE/pdFALSE?
 *
 * @return The stream buffer.
 */
    static StreamBuffer_t * prvTCPCreateStream( FreeRTOS_Socket_t * pxSocket,
                                                BaseType_t xIsInputStream )
    {
        StreamBuffer_t * pxBuffer;
        size_t uxLength;
        size_t uxSize;

        /* Now that a stream is created, the maximum size is fixed before
         * creation, it could still be changed with setsockopt(). */
        if( xIsInputStream != pdFALSE )
        {
            uxLength = pxSocket->u.xTCP.uxRxStreamSize;

            if( pxSocket->u.xTCP.uxLittleSpace == 0UL )
            {
                pxSocket->u.xTCP.uxLittleSpace = ( sock20_PERCENT * pxSocket->u.xTCP.uxRxStreamSize ) / sock100_PERCENT;
            }

            if( pxSocket->u.xTCP.uxEnoughSpace == 0UL )
            {
                pxSocket->u.xTCP.uxEnoughSpace = ( sock80_PERCENT * pxSocket->u.xTCP.uxRxStreamSize ) / sock100_PERCENT;
            }
        }
        else
        {
            uxLength = pxSocket->u.xTCP.uxTxStreamSize;
        }

        /* Add an extra 4 (or 8) bytes. */
        uxLength += sizeof( size_t );

        /* And make the length a multiple of sizeof( size_t ). */
        uxLength &= ~( sizeof( size_t ) - 1U );

        uxSize = ( sizeof( *pxBuffer ) + uxLength ) - sizeof( pxBuffer->ucArray );

        pxBuffer = ipCAST_PTR_TO_TYPE_PTR( StreamBuffer_t, pvPortMallocLarge( uxSize ) );

        if( pxBuffer == NULL )
        {
            FreeRTOS_debug_printf( ( "prvTCPCreateStream: malloc failed\n" ) );
            pxSocket->u.xTCP.bits.bMallocError = pdTRUE;
            vTCPStateChange( pxSocket, eCLOSE_WAIT );
        }
        else
        {
            /* Clear the markers of the stream */
            ( void ) memset( pxBuffer, 0, sizeof( *pxBuffer ) - sizeof( pxBuffer->ucArray ) );
            pxBuffer->LENGTH = ( size_t ) uxLength;

            if( xTCPWindowLoggingLevel != 0 )
            {
                FreeRTOS_debug_printf( ( "prvTCPCreateStream: %cxStream created %u bytes (total %u)\n", ( xIsInputStream != 0 ) ? 'R' : 'T', uxLength, uxSize ) );
            }

            if( xIsInputStream != 0 )
            {
                iptraceMEM_STATS_CREATE( tcpRX_STREAM_BUFFER, pxBuffer, uxSize );
                pxSocket->u.xTCP.rxStream = pxBuffer;
            }
            else
            {
                iptraceMEM_STATS_CREATE( tcpTX_STREAM_BUFFER, pxBuffer, uxSize );
                pxSocket->u.xTCP.txStream = pxBuffer;
            }
        }

        return pxBuffer;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Add data to the RxStream. When uxOffset > 0, data has come in out-of-order
 *        and will be put in front of the head so it can not be popped by the user.
 *
 * @param[in] pxSocket: The socket to whose RxStream data is to be added.
 * @param[in] uxOffset: Offset of the packet.
 * @param[in] pcData: The data to be added to the RxStream.
 * @param[in] ulByteCount: Number of bytes in the data.
 *
 * @return The number of bytes actually added to the RxStream. Or else, a negative
 *         error code is returned.
 */
    int32_t lTCPAddRxdata( FreeRTOS_Socket_t * pxSocket,
                           size_t uxOffset,
                           const uint8_t * pcData,
                           uint32_t ulByteCount )
    {
        StreamBuffer_t * pxStream = pxSocket->u.xTCP.rxStream;
        int32_t xResult = 0;

        #if ( ipconfigUSE_CALLBACKS == 1 )
            BaseType_t bHasHandler = ipconfigIS_VALID_PROG_ADDRESS( pxSocket->u.xTCP.pxHandleReceive ) ? pdTRUE : pdFALSE;
            const uint8_t * pucBuffer = NULL;
        #endif /* ipconfigUSE_CALLBACKS */

        /* int32_t uxStreamBufferAdd( pxBuffer, uxOffset, pucData, aCount )
         * if( pucData != NULL ) copy data the the buffer
         * if( pucData == NULL ) no copying, just advance rxHead
         * if( uxOffset != 0 ) Just store data which has come out-of-order
         * if( uxOffset == 0 ) Also advance rxHead */
        if( pxStream == NULL )
        {
            pxStream = prvTCPCreateStream( pxSocket, pdTRUE );

            if( pxStream == NULL )
            {
                xResult = -1;
            }
        }

        if( xResult >= 0 )
        {
            #if ( ipconfigUSE_CALLBACKS == 1 )
                {
                    if( ( bHasHandler != pdFALSE ) && ( uxStreamBufferGetSize( pxStream ) == 0U ) && ( uxOffset == 0UL ) && ( pcData != NULL ) )
                    {
                        /* Data can be passed directly to the user */
                        pucBuffer = pcData;

                        pcData = NULL;
                    }
                }
            #endif /* ipconfigUSE_CALLBACKS */

            xResult = ( int32_t ) uxStreamBufferAdd( pxStream, uxOffset, pcData, ( size_t ) ulByteCount );

            #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                {
                    if( xResult != ( int32_t ) ulByteCount )
                    {
                        FreeRTOS_debug_printf( ( "lTCPAddRxdata: at %u: %d/%u bytes (tail %u head %u space %u front %u)\n",
                                                 ( UBaseType_t ) uxOffset,
                                                 ( BaseType_t ) xResult,
                                                 ( UBaseType_t ) ulByteCount,
                                                 ( UBaseType_t ) pxStream->uxTail,
                                                 ( UBaseType_t ) pxStream->uxHead,
                                                 ( UBaseType_t ) uxStreamBufferFrontSpace( pxStream ),
                                                 ( UBaseType_t ) pxStream->uxFront ) );
                    }
                }
            #endif /* ipconfigHAS_DEBUG_PRINTF */

            if( uxOffset == 0U )
            {
                /* Data is being added to rxStream at the head (offs = 0) */
                #if ( ipconfigUSE_CALLBACKS == 1 )
                    if( bHasHandler != pdFALSE )
                    {
                        /* The socket owner has installed an OnReceive handler. Pass the
                         * Rx data, without copying from the rxStream, to the user. */
                        for( ; ; )
                        {
                            uint8_t * ucReadPtr = NULL;
                            uint32_t ulCount;

                            if( pucBuffer != NULL )
                            {
                                ucReadPtr = ipPOINTER_CAST( uint8_t *, pucBuffer );
                                ulCount = ulByteCount;
                                pucBuffer = NULL;
                            }
                            else
                            {
                                ulCount = ( uint32_t ) uxStreamBufferGetPtr( pxStream, &( ucReadPtr ) );
                            }

                            if( ulCount == 0UL )
                            {
                                break;
                            }

                            ( void ) pxSocket->u.xTCP.pxHandleReceive( pxSocket, ucReadPtr, ( size_t ) ulCount );
                            ( void ) uxStreamBufferGet( pxStream, 0UL, NULL, ( size_t ) ulCount, pdFALSE );
                        }
                    }
                    else
                #endif /* ipconfigUSE_CALLBACKS */
                {
                    /* See if running out of space. */
                    if( pxSocket->u.xTCP.bits.bLowWater == pdFALSE_UNSIGNED )
                    {
                        size_t uxFrontSpace = uxStreamBufferFrontSpace( pxSocket->u.xTCP.rxStream );

                        if( uxFrontSpace <= pxSocket->u.xTCP.uxLittleSpace )
                        {
                            pxSocket->u.xTCP.bits.bLowWater = pdTRUE;
                            pxSocket->u.xTCP.bits.bWinChange = pdTRUE;

                            /* bLowWater was reached, send the changed window size. */
                            pxSocket->u.xTCP.usTimeout = 1U;
                            ( void ) xSendEventToIPTask( eTCPTimerEvent );
                        }
                    }

                    /* New incoming data is available, wake up the user.   User's
                     * semaphores will be set just before the IP-task goes asleep. */
                    pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_RECEIVE;

                    #if ipconfigSUPPORT_SELECT_FUNCTION == 1
                        {
                            if( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_READ ) != 0U )
                            {
                                pxSocket->xEventBits |= ( ( ( EventBits_t ) eSELECT_READ ) << SOCKET_EVENT_BIT_COUNT );
                            }
                        }
                    #endif
                }
            }
        }

        return xResult;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Function to get the remote IP-address and port number.
 *
 * @param[in] xSocket: Socket owning the connection.
 * @param[out] pxAddress: The address pointer to which the address
 *                        is to be added.
 *
 * @return The size of the address being returned. Or else a negative
 *         error code will be returned.
 */
    BaseType_t FreeRTOS_GetRemoteAddress( ConstSocket_t xSocket,
                                          struct freertos_sockaddr * pxAddress )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xResult;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xResult = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            /* BSD style sockets communicate IP and port addresses in network
             * byte order.
             * IP address of remote machine. */
            pxAddress->sin_addr = FreeRTOS_htonl( pxSocket->u.xTCP.ulRemoteIP );

            /* Port on remote machine. */
            pxAddress->sin_port = FreeRTOS_htons( pxSocket->u.xTCP.usRemotePort );

            xResult = ( BaseType_t ) sizeof( *pxAddress );
        }

        return xResult;
    }


#endif /* ipconfigUSE_TCP */

/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Check the number of bytes that may be added to txStream.
 *
 * @param[in] xSocket: The socket to be checked.
 *
 * @return the number of bytes that may be added to txStream or
 *         else a negative error code.
 */
    BaseType_t FreeRTOS_maywrite( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xResult;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xResult = -pdFREERTOS_ERRNO_EINVAL;
        }
        else if( pxSocket->u.xTCP.ucTCPState != ( uint8_t ) eESTABLISHED )
        {
            if( ( pxSocket->u.xTCP.ucTCPState < ( uint8_t ) eCONNECT_SYN ) || ( pxSocket->u.xTCP.ucTCPState > ( EventBits_t ) eESTABLISHED ) )
            {
                xResult = -1;
            }
            else
            {
                xResult = 0;
            }
        }
        else if( pxSocket->u.xTCP.txStream == NULL )
        {
            xResult = ( BaseType_t ) pxSocket->u.xTCP.uxTxStreamSize;
        }
        else
        {
            xResult = ( BaseType_t ) uxStreamBufferGetSpace( pxSocket->u.xTCP.txStream );
        }

        return xResult;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Get the number of bytes that can be written in the Tx buffer
 *        of the given socket.
 *
 * @param[in] xSocket: the socket to be checked.
 *
 * @return The bytes that can be written. Or else an error code.
 */
    BaseType_t FreeRTOS_tx_space( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            if( pxSocket->u.xTCP.txStream != NULL )
            {
                xReturn = ( BaseType_t ) uxStreamBufferGetSpace( pxSocket->u.xTCP.txStream );
            }
            else
            {
                xReturn = ( BaseType_t ) pxSocket->u.xTCP.uxTxStreamSize;
            }
        }

        return xReturn;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Returns the number of bytes stored in the Tx buffer.
 *
 * @param[in] xSocket: The socket to be checked.
 *
 * @return The number of bytes stored in the Tx buffer of the socket.
 *         Or an error code.
 */
    BaseType_t FreeRTOS_tx_size( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            if( pxSocket->u.xTCP.txStream != NULL )
            {
                xReturn = ( BaseType_t ) uxStreamBufferGetSize( pxSocket->u.xTCP.txStream );
            }
            else
            {
                xReturn = 0;
            }
        }

        return xReturn;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Is the socket connected.
 *
 * @param[in] xSocket: The socket being checked.
 *
 * @return pdTRUE if TCP socket is connected.
 */
    BaseType_t FreeRTOS_issocketconnected( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn = pdFALSE;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            if( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED )
            {
                if( pxSocket->u.xTCP.ucTCPState < ( uint8_t ) eCLOSE_WAIT )
                {
                    xReturn = pdTRUE;
                }
            }
        }

        return xReturn;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Get the actual value of Maximum Segment Size ( MSS ) being used.
 *
 * @param[in] xSocket: The socket whose MSS is to be returned.
 *
 * @return the actual size of MSS being used or an error code.
 */
    BaseType_t FreeRTOS_mss( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            /* usCurMSS is declared as uint16_t to save space.  FreeRTOS_mss()
             * will often be used in signed native-size expressions cast it to
             * BaseType_t. */
            xReturn = ( BaseType_t ) ( pxSocket->u.xTCP.usCurMSS );
        }

        return xReturn;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Get the connection status. The values correspond to the members
 *        of the enum 'eIPTCPState_t'.
 *
 * @param[in] xSocket: Socket to get the connection status from.
 *
 * @return The connection status or an error code.
 *
 * @note For internal use only.
 */
    BaseType_t FreeRTOS_connstatus( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        {
            /* Cast it to BaseType_t. */
            xReturn = ( BaseType_t ) ( pxSocket->u.xTCP.ucTCPState );
        }

        return xReturn;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Returns the number of bytes which can be read from the RX stream buffer.
 *
 * @param[in] xSocket: the socket to get the number of bytes from.
 *
 * @return Returns the number of bytes which can be read. Or an error
 *         code is returned.
 */
    BaseType_t FreeRTOS_rx_size( ConstSocket_t xSocket )
    {
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;

        if( pxSocket->ucProtocol != ( uint8_t ) FREERTOS_IPPROTO_TCP )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else if( pxSocket->u.xTCP.rxStream != NULL )
        {
            xReturn = ( BaseType_t ) uxStreamBufferGetSize( pxSocket->u.xTCP.rxStream );
        }
        else
        {
            xReturn = 0;
        }

        return xReturn;
    }


#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if 0

/**
 * @brief Returns the number of packets that are stored in a UDP socket.
 *
 * @param[in] xSocket: the socket to get the number of bytes from.
 *
 * @return Returns the number of packets that are stored.  Use FreeRTOS_recvfrom()
 *         to retrieve those packets.
 */
    BaseType_t FreeRTOS_udp_rx_size( Socket_t xSocket )
    {
        BaseType_t xReturn = 0;
        const FreeRTOS_Socket_t * pxSocket = ( const FreeRTOS_Socket_t * ) xSocket;

        if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_UDP )
        {
            xReturn = ( BaseType_t ) listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) );
        }
        else
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }

        return xReturn;
    }
#endif /* 0 */

#if ( ipconfigUSE_TCP == 1 )

/**
 * @brief Get the net status. The IP-task will print a summary of all sockets and
 *        their connections.
 */
    void FreeRTOS_netstat( void )
    {
        IPStackEvent_t xAskEvent;

        /* Ask the IP-task to call vTCPNetStat()
         * to avoid accessing xBoundTCPSocketsList
         */
        xAskEvent.eEventType = eTCPNetStat;
        xAskEvent.pvData = ( void * ) NULL;
        ( void ) xSendEventStructToIPTask( &xAskEvent, pdMS_TO_TICKS( 1000U ) );
    }

#endif /* ipconfigUSE_TCP */
/*-----------------------------------------------------------*/

#if ( ( ipconfigHAS_PRINTF != 0 ) && ( ipconfigUSE_TCP == 1 ) )

/**
 * @brief Print a summary of all sockets and their connections.
 */
    void vTCPNetStat( void )
    {
        /* Show a simple listing of all created sockets and their connections */
        const ListItem_t * pxIterator;
        BaseType_t count = 0;
        size_t uxMinimum = uxGetMinimumFreeNetworkBuffers();
        size_t uxCurrent = uxGetNumberOfFreeNetworkBuffers();

        if( !listLIST_IS_INITIALISED( &xBoundTCPSocketsList ) )
        {
            FreeRTOS_printf( ( "PLUS-TCP not initialized\n" ) );
        }
        else
        {
            const ListItem_t * pxEndTCP = listGET_END_MARKER( &xBoundTCPSocketsList );
            const ListItem_t * pxEndUDP = listGET_END_MARKER( &xBoundUDPSocketsList );
            FreeRTOS_printf( ( "Prot Port IP-Remote       : Port  R/T Status       Alive  tmout Child\n" ) );

            for( pxIterator = listGET_HEAD_ENTRY( &xBoundTCPSocketsList );
                 pxIterator != pxEndTCP;
                 pxIterator = listGET_NEXT( pxIterator ) )
            {
                const FreeRTOS_Socket_t * pxSocket = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxIterator ) );
                #if ( ipconfigTCP_KEEP_ALIVE == 1 )
                    TickType_t age = xTaskGetTickCount() - pxSocket->u.xTCP.xLastAliveTime;
                #else
                    TickType_t age = 0U;
                #endif

                char ucChildText[ 16 ] = "";

                if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eTCP_LISTEN )
                {
                    /* Using function "snprintf". */
                    const int32_t copied_len = snprintf( ucChildText, sizeof( ucChildText ), " %d/%d",
                                                         ( int32_t ) pxSocket->u.xTCP.usChildCount,
                                                         ( int32_t ) pxSocket->u.xTCP.usBacklog );
                    ( void ) copied_len;
                    /* These should never evaluate to false since the buffers are both shorter than 5-6 characters (<=65535) */
                    configASSERT( copied_len >= 0 );
                    configASSERT( copied_len < ( int32_t ) sizeof( ucChildText ) );
                }

                FreeRTOS_printf( ( "TCP %5d %-16lxip:%5d %d/%d %-13.13s %6lu %6u%s\n",
                                   pxSocket->usLocalPort,         /* Local port on this machine */
                                   pxSocket->u.xTCP.ulRemoteIP,   /* IP address of remote machine */
                                   pxSocket->u.xTCP.usRemotePort, /* Port on remote machine */
                                   ( pxSocket->u.xTCP.rxStream != NULL ) ? 1 : 0,
                                   ( pxSocket->u.xTCP.txStream != NULL ) ? 1 : 0,
                                   FreeRTOS_GetTCPStateName( pxSocket->u.xTCP.ucTCPState ),
                                   ( age > 999999u ) ? 999999u : age, /* Format 'age' for printing */
                                   pxSocket->u.xTCP.usTimeout,
                                   ucChildText ) );
                count++;
            }

            for( pxIterator = listGET_HEAD_ENTRY( &xBoundUDPSocketsList );
                 pxIterator != pxEndUDP;
                 pxIterator = listGET_NEXT( pxIterator ) )
            {
                /* Local port on this machine */
                FreeRTOS_printf( ( "UDP Port %5u\n",
                                   FreeRTOS_ntohs( listGET_LIST_ITEM_VALUE( pxIterator ) ) ) );
                count++;
            }

            FreeRTOS_printf( ( "FreeRTOS_netstat: %lu sockets %lu < %lu < %ld buffers free\n",
                               ( UBaseType_t ) count,
                               ( UBaseType_t ) uxMinimum,
                               ( UBaseType_t ) uxCurrent,
                               ( BaseType_t ) ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS ) );
        }
    }

#endif /* ( ( ipconfigHAS_PRINTF != 0 ) && ( ipconfigUSE_TCP == 1 ) ) */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )

/**
 * @brief This internal non-blocking function will check all sockets that belong
 *        to a select set.  The events bits of each socket will be updated, and it
 *        will check if an ongoing select() call must be interrupted because of an
 *        event has occurred.
 *
 * @param[in] pxSocketSet: The socket-set which is to be waited on for change.
 */
    void vSocketSelect( SocketSelect_t * pxSocketSet )
    {
        BaseType_t xRound;
        EventBits_t xSocketBits, xBitsToClear;

        #if ipconfigUSE_TCP == 1
            BaseType_t xLastRound = 1;
        #else
            BaseType_t xLastRound = 0;
        #endif

        /* These flags will be switched on after checking the socket status. */
        EventBits_t xGroupBits = 0;

        for( xRound = 0; xRound <= xLastRound; xRound++ )
        {
            const ListItem_t * pxIterator;
            const ListItem_t * pxEnd;

            if( xRound == 0 )
            {
                pxEnd = listGET_END_MARKER( &xBoundUDPSocketsList );
            }

            #if ipconfigUSE_TCP == 1
                else
                {
                    pxEnd = listGET_END_MARKER( &xBoundTCPSocketsList );
                }
            #endif /* ipconfigUSE_TCP == 1 */

            for( pxIterator = listGET_NEXT( pxEnd );
                 pxIterator != pxEnd;
                 pxIterator = listGET_NEXT( pxIterator ) )
            {
                FreeRTOS_Socket_t * pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, listGET_LIST_ITEM_OWNER( pxIterator ) );

                if( pxSocket->pxSocketSet != pxSocketSet )
                {
                    /* Socket does not belong to this select group. */
                    continue;
                }

                xSocketBits = 0;

                #if ( ipconfigUSE_TCP == 1 )
                    if( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP )
                    {
                        /* Check if the socket has already been accepted by the
                         * owner.  If not, it is useless to return it from a
                         * select(). */
                        BaseType_t bAccepted = pdFALSE;

                        if( pxSocket->u.xTCP.bits.bPassQueued == pdFALSE_UNSIGNED )
                        {
                            if( pxSocket->u.xTCP.bits.bPassAccept == pdFALSE_UNSIGNED )
                            {
                                bAccepted = pdTRUE;
                            }
                        }

                        /* Is the set owner interested in READ events? */
                        if( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_READ ) != ( EventBits_t ) 0U )
                        {
                            if( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eTCP_LISTEN )
                            {
                                if( ( pxSocket->u.xTCP.pxPeerSocket != NULL ) && ( pxSocket->u.xTCP.pxPeerSocket->u.xTCP.bits.bPassAccept != pdFALSE_UNSIGNED ) )
                                {
                                    xSocketBits |= ( EventBits_t ) eSELECT_READ;
                                }
                            }
                            else if( ( pxSocket->u.xTCP.bits.bReuseSocket != pdFALSE_UNSIGNED ) && ( pxSocket->u.xTCP.bits.bPassAccept != pdFALSE_UNSIGNED ) )
                            {
                                /* This socket has the re-use flag. After connecting it turns into
                                 * a connected socket. Set the READ event, so that accept() will be called. */
                                xSocketBits |= ( EventBits_t ) eSELECT_READ;
                            }
                            else if( ( bAccepted != 0 ) && ( FreeRTOS_recvcount( pxSocket ) > 0 ) )
                            {
                                xSocketBits |= ( EventBits_t ) eSELECT_READ;
                            }
                            else
                            {
                                /* Nothing. */
                            }
                        }

                        /* Is the set owner interested in EXCEPTION events? */
                        if( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_EXCEPT ) != 0U )
                        {
                            if( ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCLOSE_WAIT ) || ( pxSocket->u.xTCP.ucTCPState == ( uint8_t ) eCLOSED ) )
                            {
                                xSocketBits |= ( EventBits_t ) eSELECT_EXCEPT;
                            }
                        }

                        /* Is the set owner interested in WRITE events? */
                        if( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_WRITE ) != 0U )
                        {
                            BaseType_t bMatch = pdFALSE;

                            if( bAccepted != 0 )
                            {
                                if( FreeRTOS_tx_space( pxSocket ) > 0 )
                                {
                                    bMatch = pdTRUE;
                                }
                            }

                            if( bMatch == pdFALSE )
                            {
                                if( ( pxSocket->u.xTCP.bits.bConnPrepared != pdFALSE_UNSIGNED ) &&
                                    ( pxSocket->u.xTCP.ucTCPState >= ( uint8_t ) eESTABLISHED ) &&
                                    ( pxSocket->u.xTCP.bits.bConnPassed == pdFALSE_UNSIGNED ) )
                                {
                                    pxSocket->u.xTCP.bits.bConnPassed = pdTRUE;
                                    bMatch = pdTRUE;
                                }
                            }

                            if( bMatch != pdFALSE )
                            {
                                xSocketBits |= ( EventBits_t ) eSELECT_WRITE;
                            }
                        }
                    }
                    else
                #endif /* ipconfigUSE_TCP == 1 */
                {
                    /* Select events for UDP are simpler. */
                    if( ( ( pxSocket->xSelectBits & ( EventBits_t ) eSELECT_READ ) != 0U ) &&
                        ( listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) ) > 0U ) )
                    {
                        xSocketBits |= ( EventBits_t ) eSELECT_READ;
                    }

                    /* The WRITE and EXCEPT bits are not used for UDP */
                } /* if( pxSocket->ucProtocol == FREERTOS_IPPROTO_TCP ) */

                /* Each socket keeps its own event flags, which are looked-up
                 * by FreeRTOS_FD_ISSSET() */
                pxSocket->xSocketBits = xSocketBits;

                /* The ORed value will be used to set the bits in the event
                 * group. */
                xGroupBits |= xSocketBits;
            } /* for( pxIterator ... ) */
        }     /* for( xRound = 0; xRound <= xLastRound; xRound++ ) */

        xBitsToClear = xEventGroupGetBits( pxSocketSet->xSelectGroup );

        /* Now set the necessary bits. */
        xBitsToClear = ( xBitsToClear & ~xGroupBits ) & ( ( EventBits_t ) eSELECT_ALL );

        #if ( ipconfigSUPPORT_SIGNALS != 0 )
            {
                /* Maybe the socketset was signalled, but don't
                 * clear the 'eSELECT_INTR' bit here, as it will be used
                 * and cleared in FreeRTOS_select(). */
                xBitsToClear &= ~( ( EventBits_t ) eSELECT_INTR );
            }
        #endif /* ipconfigSUPPORT_SIGNALS */

        if( xBitsToClear != 0U )
        {
            ( void ) xEventGroupClearBits( pxSocketSet->xSelectGroup, xBitsToClear );
        }

        /* Now include eSELECT_CALL_IP to wakeup the caller. */
        ( void ) xEventGroupSetBits( pxSocketSet->xSelectGroup, xGroupBits | ( EventBits_t ) eSELECT_CALL_IP );
    }


#endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SIGNALS != 0 )

/**
 * @brief Send a signal to the task which reads from this socket.
 *        The socket will receive an event of the type 'eSOCKET_INTR'.
 *        Any ongoing blocking API ( e.g. FreeRTOS_recv() ) will be terminated
 *        and return the value -pdFREERTOS_ERRNO_EINTR ( -4 ).
 *
 * @param[in] xSocket: The socket that will be signalled.
 */
    BaseType_t FreeRTOS_SignalSocket( Socket_t xSocket )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;

        if( pxSocket == NULL )
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }
        else
        #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
            if( ( pxSocket->pxSocketSet != NULL ) && ( pxSocket->pxSocketSet->xSelectGroup != NULL ) )
            {
                ( void ) xEventGroupSetBits( pxSocket->pxSocketSet->xSelectGroup, ( EventBits_t ) eSELECT_INTR );
                xReturn = 0;
            }
            else
        #endif /* ipconfigSUPPORT_SELECT_FUNCTION */
        if( pxSocket->xEventGroup != NULL )
        {
            ( void ) xEventGroupSetBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_INTR );
            xReturn = 0;
        }
        else
        {
            xReturn = -pdFREERTOS_ERRNO_EINVAL;
        }

        return xReturn;
    }

#endif /* ipconfigSUPPORT_SIGNALS */
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_SIGNALS != 0 )

/**
 * @brief The same as 'FreeRTOS_SignalSocket()', except that this function should
 *        be called from an ISR context.
 *
 * @param[in] xSocket: The socket that will be signalled.
 * @param[in,out] pxHigherPriorityTaskWoken: will be set to non-zero in case a higher-
 *                priority task has become runnable.
 */
    BaseType_t FreeRTOS_SignalSocketFromISR( Socket_t xSocket,
                                             BaseType_t * pxHigherPriorityTaskWoken )
    {
        FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) xSocket;
        BaseType_t xReturn;
        IPStackEvent_t xEvent;

        configASSERT( pxSocket != NULL );
        configASSERT( pxSocket->ucProtocol == ( uint8_t ) FREERTOS_IPPROTO_TCP );
        configASSERT( pxSocket->xEventGroup != NULL );

        xEvent.eEventType = eSocketSignalEvent;
        xEvent.pvData = pxSocket;

        /* The IP-task will call FreeRTOS_SignalSocket for this socket. */
        xReturn = xQueueSendToBackFromISR( xNetworkEventQueue, &xEvent, pxHigherPriorityTaskWoken );

        return xReturn;
    }

#endif /* ipconfigSUPPORT_SIGNALS */
/*-----------------------------------------------------------*/

#if 0
    #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
        struct pollfd
        {
            Socket_t fd;         /* file descriptor */
            EventBits_t events;  /* requested events */
            EventBits_t revents; /* returned events */
        };

        typedef BaseType_t nfds_t;

        BaseType_t poll( struct pollfd * fds,
                         nfds_t nfds,
                         BaseType_t timeout );
        BaseType_t poll( struct pollfd * fds,
                         nfds_t nfds,
                         BaseType_t timeout )
        {
            BaseType_t index;
            SocketSelect_t * pxSocketSet = NULL;
            BaseType_t xReturn = 0;

            /* See which socket-sets have been created and bound to the sockets involved. */
            for( index = 0; index < nfds; index++ )
            {
                FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) fds[ index ].fd;

                if( pxSocket->pxSocketSet != NULL )
                {
                    if( pxSocketSet == NULL )
                    {
                        /* Use this socket-set. */
                        pxSocketSet = pxSocket->pxSocketSet;
                        xReturn = 1;
                    }
                    else if( pxSocketSet == pxSocket->pxSocketSet )
                    {
                        /* Good: associated with the same socket-set. */
                    }
                    else
                    {
                        /* More than one socket-set is found: can not do a select on 2 sets. */
                        xReturn = -1;
                        break;
                    }
                }
            }

            if( xReturn == 0 )
            {
                /* Create a new socket-set, and attach all sockets to it. */
                pxSocketSet = FreeRTOS_CreateSocketSet();

                if( pxSocketSet != NULL )
                {
                    xReturn = 1;
                }
                else
                {
                    xReturn = -2;
                }

                /* Memory leak: when the last socket closes, there is no more reference to
                 * this socket-set.  It should be marked as an automatic or anonymous socket-set,
                 * so when closing the last member, its memory will be freed. */
            }

            if( xReturn > 0 )
            {
                /* Only one socket-set is found.  Connect all sockets to this socket-set. */
                for( index = 0; index < nfds; index++ )
                {
                    FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) fds[ index ].fd;
                    EventBits_t xEventBits = fds[ index ].events;

                    FreeRTOS_FD_SET( pxSocket, pxSocketSet, xEventBits );
                    FreeRTOS_FD_CLR( pxSocket, pxSocketSet, ( EventBits_t ) ~xEventBits );
                }

                /* And sleep until an event happens or a time-out. */
                xReturn = FreeRTOS_select( pxSocketSet, timeout );

                /* Now set the return events, copying from the socked field 'xSocketBits'. */
                for( index = 0; index < nfds; index++ )
                {
                    FreeRTOS_Socket_t * pxSocket = ( FreeRTOS_Socket_t * ) fds[ index ].fd;

                    fds[ index ].revents = pxSocket->xSocketBits & ( ( EventBits_t ) eSELECT_ALL );
                }
            }
            else
            {
                /* -1: Sockets are connected to different socket sets. */
                /* -2: FreeRTOS_CreateSocketSet() failed. */
            }

            return xReturn;
        }

    #endif /* ipconfigSUPPORT_SELECT_FUNCTION */
#endif /* 0 */
