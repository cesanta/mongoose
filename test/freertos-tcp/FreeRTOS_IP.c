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
 * @file FreeRTOS_IP.c
 * @brief Implements the basic functionality for the FreeRTOS+TCP network stack.
 */

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_ARP.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "NetworkBufferManagement.h"
#include "FreeRTOS_DNS.h"


/* Used to ensure the structure packing is having the desired effect.  The
 * 'volatile' is used to prevent compiler warnings about comparing a constant with
 * a constant. */
#ifndef _lint
    #define ipEXPECTED_EthernetHeader_t_SIZE    ( ( size_t ) 14 ) /**< Ethernet Header size in bytes. */
    #define ipEXPECTED_ARPHeader_t_SIZE         ( ( size_t ) 28 ) /**< ARP header size in bytes. */
    #define ipEXPECTED_IPHeader_t_SIZE          ( ( size_t ) 20 ) /**< IP header size in bytes. */
    #define ipEXPECTED_IGMPHeader_t_SIZE        ( ( size_t ) 8 )  /**< IGMP header size in bytes. */
    #define ipEXPECTED_ICMPHeader_t_SIZE        ( ( size_t ) 8 )  /**< ICMP header size in bytes. */
    #define ipEXPECTED_UDPHeader_t_SIZE         ( ( size_t ) 8 )  /**< UDP header size in bytes. */
    #define ipEXPECTED_TCPHeader_t_SIZE         ( ( size_t ) 20 ) /**< TCP header size in bytes. */
#endif

/* ICMP protocol definitions. */
#define ipICMP_ECHO_REQUEST                 ( ( uint8_t ) 8 ) /**< ICMP echo request. */
#define ipICMP_ECHO_REPLY                   ( ( uint8_t ) 0 ) /**< ICMP echo reply. */

/* IPv4 multi-cast addresses range from 224.0.0.0.0 to 240.0.0.0. */
#define ipFIRST_MULTI_CAST_IPv4             0xE0000000UL /**< Lower bound of the IPv4 multicast address. */
#define ipLAST_MULTI_CAST_IPv4              0xF0000000UL /**< Higher bound of the IPv4 multicast address. */

/* The first byte in the IPv4 header combines the IP version (4) with
 * with the length of the IP header. */
#define ipIPV4_VERSION_HEADER_LENGTH_MIN    0x45U /**< Minimum IPv4 header length. */
#define ipIPV4_VERSION_HEADER_LENGTH_MAX    0x4FU /**< Maximum IPv4 header length. */

/** @brief Time delay between repeated attempts to initialise the network hardware. */
#ifndef ipINITIALISATION_RETRY_DELAY
    #define ipINITIALISATION_RETRY_DELAY    ( pdMS_TO_TICKS( 3000U ) )
#endif

/** @brief Defines how often the ARP timer callback function is executed.  The time is
 * shorter in the Windows simulator as simulated time is not real time. */
#ifndef ipARP_TIMER_PERIOD_MS
    #ifdef _WINDOWS_
        #define ipARP_TIMER_PERIOD_MS    ( 500U ) /* For windows simulator builds. */
    #else
        #define ipARP_TIMER_PERIOD_MS    ( 10000U )
    #endif
#endif

#ifndef iptraceIP_TASK_STARTING
    #define iptraceIP_TASK_STARTING()    do {} while( ipFALSE_BOOL ) /**< Empty definition in case iptraceIP_TASK_STARTING is not defined. */
#endif

#if ( ( ipconfigUSE_TCP == 1 ) && !defined( ipTCP_TIMER_PERIOD_MS ) )
    /** @brief When initialising the TCP timer, give it an initial time-out of 1 second. */
    #define ipTCP_TIMER_PERIOD_MS    ( 1000U )
#endif

/** @brief If ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES is set to 1, then the Ethernet
 * driver will filter incoming packets and only pass the stack those packets it
 * considers need processing.  In this case ipCONSIDER_FRAME_FOR_PROCESSING() can
 * be #-defined away.  If ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES is set to 0
 * then the Ethernet driver will pass all received packets to the stack, and the
 * stack must do the filtering itself.  In this case ipCONSIDER_FRAME_FOR_PROCESSING
 * needs to call eConsiderFrameForProcessing.
 */
#if ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES == 0
    #define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer )    eConsiderFrameForProcessing( ( pucEthernetBuffer ) )
#else
    #define ipCONSIDER_FRAME_FOR_PROCESSING( pucEthernetBuffer )    eProcessBuffer
#endif

#if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 0 )
    #if ( ipconfigBYTE_ORDER == pdFREERTOS_LITTLE_ENDIAN )
        /** @brief The bits in the two byte IP header field that make up the fragment offset value. */
        #define ipFRAGMENT_OFFSET_BIT_MASK    ( ( uint16_t ) 0xff0f )
    #else
        /** @brief The bits in the two byte IP header field that make up the fragment offset value. */
        #define ipFRAGMENT_OFFSET_BIT_MASK    ( ( uint16_t ) 0x0fff )
    #endif /* ipconfigBYTE_ORDER */
#endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */

/** @brief The maximum time the IP task is allowed to remain in the Blocked state if no
 * events are posted to the network event queue. */
#ifndef ipconfigMAX_IP_TASK_SLEEP_TIME
    #define ipconfigMAX_IP_TASK_SLEEP_TIME    ( pdMS_TO_TICKS( 10000UL ) )
#endif

/** @brief Returned as the (invalid) checksum when the protocol being checked is not
 * handled.  The value is chosen simply to be easy to spot when debugging. */
#define ipUNHANDLED_PROTOCOL    0x4321U

/** @brief Returned to indicate a valid checksum. */
#define ipCORRECT_CRC           0xffffU

/** @brief Returned to indicate incorrect checksum. */
#define ipWRONG_CRC             0x0000U

/** @brief Returned as the (invalid) checksum when the length of the data being checked
 * had an invalid length. */
#define ipINVALID_LENGTH        0x1234U

/* Trace macros to aid in debugging, disabled if ipconfigHAS_PRINTF != 1 */
#if ( ipconfigHAS_PRINTF == 1 )
    #define DEBUG_DECLARE_TRACE_VARIABLE( type, var, init )    type var = ( init ) /**< Trace macro to set "type var = init". */
    #define DEBUG_SET_TRACE_VARIABLE( var, value )             var = ( value )     /**< Trace macro to set var = value. */
#else
    #define DEBUG_DECLARE_TRACE_VARIABLE( type, var, init )                        /**< Empty definition since ipconfigHAS_PRINTF != 1. */
    #define DEBUG_SET_TRACE_VARIABLE( var, value )                                 /**< Empty definition since ipconfigHAS_PRINTF != 1. */
#endif

/*-----------------------------------------------------------*/

/**
 * Used in checksum calculation.
 */
typedef union _xUnion32
{
    uint32_t u32;      /**< The 32-bit member of the union. */
    uint16_t u16[ 2 ]; /**< The array of 2 16-bit members of the union. */
    uint8_t u8[ 4 ];   /**< The array of 4 8-bit members of the union. */
} xUnion32;

/**
 * Used in checksum calculation.
 */
typedef union _xUnionPtr
{
    uint32_t * u32ptr; /**< The pointer member to a 32-bit variable. */
    uint16_t * u16ptr; /**< The pointer member to a 16-bit variable. */
    uint8_t * u8ptr;   /**< The pointer member to an 8-bit variable. */
} xUnionPtr;


/**
 * @brief Utility function to cast pointer of a type to pointer of type NetworkBufferDescriptor_t.
 *
 * @return The casted pointer.
 */
static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( NetworkBufferDescriptor_t )
{
    return ( NetworkBufferDescriptor_t * ) pvArgument;
}

/*-----------------------------------------------------------*/

/*
 * The main TCP/IP stack processing task.  This task receives commands/events
 * from the network hardware drivers and tasks that are using sockets.  It also
 * maintains a set of protocol timers.
 */
static void prvIPTask( void * pvParameters );

/*
 * Called when new data is available from the network interface.
 */
static void prvProcessEthernetPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer );

/*
 * Process incoming IP packets.
 */
static eFrameProcessingResult_t prvProcessIPPacket( IPPacket_t * pxIPPacket,
                                                    NetworkBufferDescriptor_t * const pxNetworkBuffer );

#if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )

/*
 * Process incoming ICMP packets.
 */
    static eFrameProcessingResult_t prvProcessICMPPacket( ICMPPacket_t * const pxICMPPacket );
#endif /* ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 ) */

/*
 * Turns around an incoming ping request to convert it into a ping reply.
 */
#if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 )
    static eFrameProcessingResult_t prvProcessICMPEchoRequest( ICMPPacket_t * const pxICMPPacket );
#endif /* ipconfigREPLY_TO_INCOMING_PINGS */

/*
 * Processes incoming ping replies.  The application callback function
 * vApplicationPingReplyHook() is called with the results.
 */
#if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )
    static void prvProcessICMPEchoReply( ICMPPacket_t * const pxICMPPacket );
#endif /* ipconfigSUPPORT_OUTGOING_PINGS */

/*
 * Called to create a network connection when the stack is first started, or
 * when the network connection is lost.
 */
static void prvProcessNetworkDownEvent( void );

/*
 * Checks the ARP, DHCP and TCP timers to see if any periodic or timeout
 * processing is required.
 */
static void prvCheckNetworkTimers( void );

/*
 * Determine how long the IP task can sleep for, which depends on when the next
 * periodic or timeout processing must be performed.
 */
static TickType_t prvCalculateSleepTime( void );

/*
 * The network card driver has received a packet.  In the case that it is part
 * of a linked packet chain, walk through it to handle every message.
 */
static void prvHandleEthernetPacket( NetworkBufferDescriptor_t * pxBuffer );

/*
 * Utility functions for the light weight IP timers.
 */
static void prvIPTimerStart( IPTimer_t * pxTimer,
                             TickType_t xTime );
static BaseType_t prvIPTimerCheck( IPTimer_t * pxTimer );
static void prvIPTimerReload( IPTimer_t * pxTimer,
                              TickType_t xTime );

/* The function 'prvAllowIPPacket()' checks if a packets should be processed. */
static eFrameProcessingResult_t prvAllowIPPacket( const IPPacket_t * const pxIPPacket,
                                                  const NetworkBufferDescriptor_t * const pxNetworkBuffer,
                                                  UBaseType_t uxHeaderLength );

#if ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 1 )

/* Even when the driver takes care of checksum calculations,
 *  the IP-task will still check if the length fields are OK. */
    static BaseType_t xCheckSizeFields( const uint8_t * const pucEthernetBuffer,
                                        size_t uxBufferLength );
#endif /* ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 1 ) */

/*
 * Returns the network buffer descriptor that owns a given packet buffer.
 */
static NetworkBufferDescriptor_t * prvPacketBuffer_to_NetworkBuffer( const void * pvBuffer,
                                                                     size_t uxOffset );

/*-----------------------------------------------------------*/

/** @brief The queue used to pass events into the IP-task for processing. */
QueueHandle_t xNetworkEventQueue = NULL;

/** @brief The IP packet ID. */
uint16_t usPacketIdentifier = 0U;

/** @brief For convenience, a MAC address of all 0xffs is defined const for quick
 * reference. */
const MACAddress_t xBroadcastMACAddress = { { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } };

/** @brief Structure that stores the netmask, gateway address and DNS server addresses. */
NetworkAddressingParameters_t xNetworkAddressing = { 0, 0, 0, 0, 0 };

/** @brief Default values for the above struct in case DHCP
 * does not lead to a confirmed request. */
NetworkAddressingParameters_t xDefaultAddressing = { 0, 0, 0, 0, 0 };

/** @brief Used to ensure network down events cannot be missed when they cannot be
 * posted to the network event queue because the network event queue is already
 * full. */
static volatile BaseType_t xNetworkDownEventPending = pdFALSE;

/** @brief Stores the handle of the task that handles the stack.  The handle is used
 * (indirectly) by some utility function to determine if the utility function is
 * being called by a task (in which case it is ok to block) or by the IP task
 * itself (in which case it is not ok to block). */

static TaskHandle_t xIPTaskHandle = NULL;

#if ( ipconfigUSE_TCP != 0 )

/** @brief Set to a non-zero value if one or more TCP message have been processed
 * within the last round. */
    static BaseType_t xProcessedTCPMessage;
#endif

/** @brief Simple set to pdTRUE or pdFALSE depending on whether the network is up or
 * down (connected, not connected) respectively. */
static BaseType_t xNetworkUp = pdFALSE;

/*
 * A timer for each of the following processes, all of which need attention on a
 * regular basis
 */

/** @brief ARP timer, to check its table entries. */
static IPTimer_t xARPTimer;
#if ( ipconfigUSE_DHCP != 0 )
    /** @brief DHCP timer, to send requests and to renew a reservation.  */
    static IPTimer_t xDHCPTimer;
#endif
#if ( ipconfigUSE_TCP != 0 )
    /** @brief TCP timer, to check for timeouts, resends. */
    static IPTimer_t xTCPTimer;
#endif
#if ( ipconfigDNS_USE_CALLBACKS != 0 )
    /** @brief DNS timer, to check for timeouts when looking-up a domain. */
    static IPTimer_t xDNSTimer;
#endif

/** @brief Set to pdTRUE when the IP task is ready to start processing packets. */
static BaseType_t xIPTaskInitialised = pdFALSE;

#if ( ipconfigCHECK_IP_QUEUE_SPACE != 0 )
    /** @brief Keep track of the lowest amount of space in 'xNetworkEventQueue'. */
    static UBaseType_t uxQueueMinimumSpace = ipconfigEVENT_QUEUE_LENGTH;
#endif

/*-----------------------------------------------------------*/

/* Coverity wants to make pvParameters const, which would make it incompatible. Leave the
 * function signature as is. */

/**
 * @brief The IP task handles all requests from the user application and the
 *        network interface. It receives messages through a FreeRTOS queue called
 *        'xNetworkEventQueue'. prvIPTask() is the only task which has access to
 *        the data of the IP-stack, and so it has no need of using mutexes.
 *
 * @param[in] pvParameters: Not used.
 */
static void prvIPTask( void * pvParameters )
{
    IPStackEvent_t xReceivedEvent;
    TickType_t xNextIPSleep;
    FreeRTOS_Socket_t * pxSocket;
    struct freertos_sockaddr xAddress;

    /* Just to prevent compiler warnings about unused parameters. */
    ( void ) pvParameters;

    /* A possibility to set some additional task properties. */
    iptraceIP_TASK_STARTING();

    /* Generate a dummy message to say that the network connection has gone
     *  down.  This will cause this task to initialise the network interface.  After
     *  this it is the responsibility of the network interface hardware driver to
     *  send this message if a previously connected network is disconnected. */
    FreeRTOS_NetworkDown();

    #if ( ipconfigUSE_TCP == 1 )
        {
            /* Initialise the TCP timer. */
            prvIPTimerReload( &xTCPTimer, pdMS_TO_TICKS( ipTCP_TIMER_PERIOD_MS ) );
        }
    #endif

    /* Initialisation is complete and events can now be processed. */
    xIPTaskInitialised = pdTRUE;

    FreeRTOS_debug_printf( ( "prvIPTask started\n" ) );

    /* Loop, processing IP events. */
    for( ; ; )
    {
        ipconfigWATCHDOG_TIMER();

        /* Check the ARP, DHCP and TCP timers to see if there is any periodic
         * or timeout processing to perform. */
        prvCheckNetworkTimers();

        /* Calculate the acceptable maximum sleep time. */
        xNextIPSleep = prvCalculateSleepTime();

        /* Wait until there is something to do. If the following call exits
         * due to a time out rather than a message being received, set a
         * 'NoEvent' value. */
        if( xQueueReceive( xNetworkEventQueue, ( void * ) &xReceivedEvent, xNextIPSleep ) == pdFALSE )
        {
            xReceivedEvent.eEventType = eNoEvent;
        }

        #if ( ipconfigCHECK_IP_QUEUE_SPACE != 0 )
            {
                if( xReceivedEvent.eEventType != eNoEvent )
                {
                    UBaseType_t uxCount;

                    uxCount = uxQueueSpacesAvailable( xNetworkEventQueue );

                    if( uxQueueMinimumSpace > uxCount )
                    {
                        uxQueueMinimumSpace = uxCount;
                    }
                }
            }
        #endif /* ipconfigCHECK_IP_QUEUE_SPACE */

        iptraceNETWORK_EVENT_RECEIVED( xReceivedEvent.eEventType );

        switch( xReceivedEvent.eEventType )
        {
            case eNetworkDownEvent:
                /* Attempt to establish a connection. */
                xNetworkUp = pdFALSE;
                prvProcessNetworkDownEvent();
                break;

            case eNetworkRxEvent:

                /* The network hardware driver has received a new packet.  A
                 * pointer to the received buffer is located in the pvData member
                 * of the received event structure. */
                prvHandleEthernetPacket( ipCAST_PTR_TO_TYPE_PTR( NetworkBufferDescriptor_t, xReceivedEvent.pvData ) );
                break;

            case eNetworkTxEvent:

               {
                   NetworkBufferDescriptor_t * pxDescriptor = ipCAST_PTR_TO_TYPE_PTR( NetworkBufferDescriptor_t, xReceivedEvent.pvData );

                   /* Send a network packet. The ownership will  be transferred to
                    * the driver, which will release it after delivery. */
                   iptraceNETWORK_INTERFACE_OUTPUT( pxDescriptor->xDataLength, pxDescriptor->pucEthernetBuffer );
                   ( void ) xNetworkInterfaceOutput( pxDescriptor, pdTRUE );
               }

               break;

            case eARPTimerEvent:
                /* The ARP timer has expired, process the ARP cache. */
                vARPAgeCache();
                break;

            case eSocketBindEvent:

                /* FreeRTOS_bind (a user API) wants the IP-task to bind a socket
                 * to a port. The port number is communicated in the socket field
                 * usLocalPort. vSocketBind() will actually bind the socket and the
                 * API will unblock as soon as the eSOCKET_BOUND event is
                 * triggered. */
                pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, xReceivedEvent.pvData );
                xAddress.sin_addr = 0U; /* For the moment. */
                xAddress.sin_port = FreeRTOS_ntohs( pxSocket->usLocalPort );
                pxSocket->usLocalPort = 0U;
                ( void ) vSocketBind( pxSocket, &xAddress, sizeof( xAddress ), pdFALSE );

                /* Before 'eSocketBindEvent' was sent it was tested that
                 * ( xEventGroup != NULL ) so it can be used now to wake up the
                 * user. */
                pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_BOUND;
                vSocketWakeUpUser( pxSocket );
                break;

            case eSocketCloseEvent:

                /* The user API FreeRTOS_closesocket() has sent a message to the
                 * IP-task to actually close a socket. This is handled in
                 * vSocketClose().  As the socket gets closed, there is no way to
                 * report back to the API, so the API won't wait for the result */
                ( void ) vSocketClose( ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, xReceivedEvent.pvData ) );
                break;

            case eStackTxEvent:

                /* The network stack has generated a packet to send.  A
                 * pointer to the generated buffer is located in the pvData
                 * member of the received event structure. */
                vProcessGeneratedUDPPacket( ipCAST_PTR_TO_TYPE_PTR( NetworkBufferDescriptor_t, xReceivedEvent.pvData ) );
                break;

            case eDHCPEvent:
                /* The DHCP state machine needs processing. */
                #if ( ipconfigUSE_DHCP == 1 )
                    {
                        uintptr_t uxState;
                        eDHCPState_t eState;

                        /* Cast in two steps to please MISRA. */
                        uxState = ( uintptr_t ) xReceivedEvent.pvData;
                        eState = ( eDHCPState_t ) uxState;

                        /* Process DHCP messages for a given end-point. */
                        vDHCPProcess( pdFALSE, eState );
                    }
                #endif /* ipconfigUSE_DHCP */
                break;

            case eSocketSelectEvent:

                /* FreeRTOS_select() has got unblocked by a socket event,
                 * vSocketSelect() will check which sockets actually have an event
                 * and update the socket field xSocketBits. */
                #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
                    #if ( ipconfigSELECT_USES_NOTIFY != 0 )
                        {
                            SocketSelectMessage_t * pxMessage = ipCAST_PTR_TO_TYPE_PTR( SocketSelectMessage_t, xReceivedEvent.pvData );
                            vSocketSelect( pxMessage->pxSocketSet );
                            ( void ) xTaskNotifyGive( pxMessage->xTaskhandle );
                        }
                    #else
                        {
                            vSocketSelect( ipCAST_PTR_TO_TYPE_PTR( SocketSelect_t, xReceivedEvent.pvData ) );
                        }
                    #endif /* ( ipconfigSELECT_USES_NOTIFY != 0 ) */
                #endif /* ipconfigSUPPORT_SELECT_FUNCTION == 1 */
                break;

            case eSocketSignalEvent:
                #if ( ipconfigSUPPORT_SIGNALS != 0 )

                    /* Some task wants to signal the user of this socket in
                     * order to interrupt a call to recv() or a call to select(). */
                    ( void ) FreeRTOS_SignalSocket( ipPOINTER_CAST( Socket_t, xReceivedEvent.pvData ) );
                #endif /* ipconfigSUPPORT_SIGNALS */
                break;

            case eTCPTimerEvent:
                #if ( ipconfigUSE_TCP == 1 )

                    /* Simply mark the TCP timer as expired so it gets processed
                     * the next time prvCheckNetworkTimers() is called. */
                    xTCPTimer.bExpired = pdTRUE_UNSIGNED;
                #endif /* ipconfigUSE_TCP */
                break;

            case eTCPAcceptEvent:

                /* The API FreeRTOS_accept() was called, the IP-task will now
                 * check if the listening socket (communicated in pvData) actually
                 * received a new connection. */
                #if ( ipconfigUSE_TCP == 1 )
                    pxSocket = ipCAST_PTR_TO_TYPE_PTR( FreeRTOS_Socket_t, xReceivedEvent.pvData );

                    if( xTCPCheckNewClient( pxSocket ) != pdFALSE )
                    {
                        pxSocket->xEventBits |= ( EventBits_t ) eSOCKET_ACCEPT;
                        vSocketWakeUpUser( pxSocket );
                    }
                #endif /* ipconfigUSE_TCP */
                break;

            case eTCPNetStat:

                /* FreeRTOS_netstat() was called to have the IP-task print an
                 * overview of all sockets and their connections */
                #if ( ( ipconfigUSE_TCP == 1 ) && ( ipconfigHAS_PRINTF == 1 ) )
                    vTCPNetStat();
                #endif /* ipconfigUSE_TCP */
                break;

            case eNoEvent:
                /* xQueueReceive() returned because of a normal time-out. */
                break;

            default:
                /* Should not get here. */
                break;
        }

        if( xNetworkDownEventPending != pdFALSE )
        {
            /* A network down event could not be posted to the network event
             * queue because the queue was full.
             * As this code runs in the IP-task, it can be done directly by
             * calling prvProcessNetworkDownEvent(). */
            prvProcessNetworkDownEvent();
        }
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Function to check whether the current context belongs to
 *        the IP-task.
 *
 * @return If the current context belongs to the IP-task, then pdTRUE is
 *         returned. Else pdFALSE is returned.
 *
 * @note Very important: the IP-task is not allowed to call its own API's,
 *        because it would easily get into a dead-lock.
 */
BaseType_t xIsCallingFromIPTask( void )
{
    BaseType_t xReturn;

    if( xTaskGetCurrentTaskHandle() == xIPTaskHandle )
    {
        xReturn = pdTRUE;
    }
    else
    {
        xReturn = pdFALSE;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Handle the incoming Ethernet packets.
 *
 * @param[in] pxBuffer: Linked/un-linked network buffer descriptor(s)
 *                      to be processed.
 */
static void prvHandleEthernetPacket( NetworkBufferDescriptor_t * pxBuffer )
{
    #if ( ipconfigUSE_LINKED_RX_MESSAGES == 0 )
        {
            /* When ipconfigUSE_LINKED_RX_MESSAGES is not set to 0 then only one
             * buffer will be sent at a time.  This is the default way for +TCP to pass
             * messages from the MAC to the TCP/IP stack. */
            prvProcessEthernetPacket( pxBuffer );
        }
    #else /* ipconfigUSE_LINKED_RX_MESSAGES */
        {
            NetworkBufferDescriptor_t * pxNextBuffer;

            /* An optimisation that is useful when there is high network traffic.
             * Instead of passing received packets into the IP task one at a time the
             * network interface can chain received packets together and pass them into
             * the IP task in one go.  The packets are chained using the pxNextBuffer
             * member.  The loop below walks through the chain processing each packet
             * in the chain in turn. */
            do
            {
                /* Store a pointer to the buffer after pxBuffer for use later on. */
                pxNextBuffer = pxBuffer->pxNextBuffer;

                /* Make it NULL to avoid using it later on. */
                pxBuffer->pxNextBuffer = NULL;

                prvProcessEthernetPacket( pxBuffer );
                pxBuffer = pxNextBuffer;

                /* While there is another packet in the chain. */
            } while( pxBuffer != NULL );
        }
    #endif /* ipconfigUSE_LINKED_RX_MESSAGES */
}
/*-----------------------------------------------------------*/

/**
 * @brief Calculate the maximum sleep time remaining. It will go through all
 *        timers to see which timer will expire first. That will be the amount
 *        of time to block in the next call to xQueueReceive().
 *
 * @return The maximum sleep time or ipconfigMAX_IP_TASK_SLEEP_TIME,
 *         whichever is smaller.
 */
static TickType_t prvCalculateSleepTime( void )
{
    TickType_t xMaximumSleepTime;

    /* Start with the maximum sleep time, then check this against the remaining
     * time in any other timers that are active. */
    xMaximumSleepTime = ipconfigMAX_IP_TASK_SLEEP_TIME;

    if( xARPTimer.bActive != pdFALSE_UNSIGNED )
    {
        if( xARPTimer.ulRemainingTime < xMaximumSleepTime )
        {
            xMaximumSleepTime = xARPTimer.ulReloadTime;
        }
    }

    #if ( ipconfigUSE_DHCP == 1 )
        {
            if( xDHCPTimer.bActive != pdFALSE_UNSIGNED )
            {
                if( xDHCPTimer.ulRemainingTime < xMaximumSleepTime )
                {
                    xMaximumSleepTime = xDHCPTimer.ulRemainingTime;
                }
            }
        }
    #endif /* ipconfigUSE_DHCP */

    #if ( ipconfigUSE_TCP == 1 )
        {
            if( xTCPTimer.ulRemainingTime < xMaximumSleepTime )
            {
                xMaximumSleepTime = xTCPTimer.ulRemainingTime;
            }
        }
    #endif

    #if ( ipconfigDNS_USE_CALLBACKS != 0 )
        {
            if( xDNSTimer.bActive != pdFALSE_UNSIGNED )
            {
                if( xDNSTimer.ulRemainingTime < xMaximumSleepTime )
                {
                    xMaximumSleepTime = xDNSTimer.ulRemainingTime;
                }
            }
        }
    #endif

    return xMaximumSleepTime;
}
/*-----------------------------------------------------------*/

/**
 * @brief Check the network timers (ARP/DHCP/DNS/TCP) and if they are
 *        expired, send an event to the IP-Task.
 */
static void prvCheckNetworkTimers( void )
{
    /* Is it time for ARP processing? */
    if( prvIPTimerCheck( &xARPTimer ) != pdFALSE )
    {
        ( void ) xSendEventToIPTask( eARPTimerEvent );
    }

    #if ( ipconfigUSE_DHCP == 1 )
        {
            /* Is it time for DHCP processing? */
            if( prvIPTimerCheck( &xDHCPTimer ) != pdFALSE )
            {
                ( void ) xSendDHCPEvent();
            }
        }
    #endif /* ipconfigUSE_DHCP */

    #if ( ipconfigDNS_USE_CALLBACKS != 0 )
        {
            /* Is it time for DNS processing? */
            if( prvIPTimerCheck( &xDNSTimer ) != pdFALSE )
            {
                vDNSCheckCallBack( NULL );
            }
        }
    #endif /* ipconfigDNS_USE_CALLBACKS */

    #if ( ipconfigUSE_TCP == 1 )
        {
            BaseType_t xWillSleep;
            TickType_t xNextTime;
            BaseType_t xCheckTCPSockets;

            /* If the IP task has messages waiting to be processed then
             * it will not sleep in any case. */
            if( uxQueueMessagesWaiting( xNetworkEventQueue ) == 0U )
            {
                xWillSleep = pdTRUE;
            }
            else
            {
                xWillSleep = pdFALSE;
            }

            /* Sockets need to be checked if the TCP timer has expired. */
            xCheckTCPSockets = prvIPTimerCheck( &xTCPTimer );

            /* Sockets will also be checked if there are TCP messages but the
            * message queue is empty (indicated by xWillSleep being true). */
            if( ( xProcessedTCPMessage != pdFALSE ) && ( xWillSleep != pdFALSE ) )
            {
                xCheckTCPSockets = pdTRUE;
            }

            if( xCheckTCPSockets != pdFALSE )
            {
                /* Attend to the sockets, returning the period after which the
                 * check must be repeated. */
                xNextTime = xTCPTimerCheck( xWillSleep );
                prvIPTimerStart( &xTCPTimer, xNextTime );
                xProcessedTCPMessage = 0;
            }
        }
    #endif /* ipconfigUSE_TCP == 1 */
}
/*-----------------------------------------------------------*/

/**
 * @brief Start an IP timer. The IP-task has its own implementation of a timer
 *        called 'IPTimer_t', which is based on the FreeRTOS 'TimeOut_t'.
 *
 * @param[in] pxTimer: Pointer to the IP timer. When zero, the timer is marked
 *                     as expired.
 * @param[in] xTime: Time to be loaded into the IP timer.
 */
static void prvIPTimerStart( IPTimer_t * pxTimer,
                             TickType_t xTime )
{
    vTaskSetTimeOutState( &pxTimer->xTimeOut );
    pxTimer->ulRemainingTime = xTime;

    if( xTime == ( TickType_t ) 0 )
    {
        pxTimer->bExpired = pdTRUE_UNSIGNED;
    }
    else
    {
        pxTimer->bExpired = pdFALSE_UNSIGNED;
    }

    pxTimer->bActive = pdTRUE_UNSIGNED;
}
/*-----------------------------------------------------------*/

/**
 * @brief Sets the reload time of an IP timer and restarts it.
 *
 * @param[in] pxTimer: Pointer to the IP timer.
 * @param[in] xTime: Time to be reloaded into the IP timer.
 */
static void prvIPTimerReload( IPTimer_t * pxTimer,
                              TickType_t xTime )
{
    pxTimer->ulReloadTime = xTime;
    prvIPTimerStart( pxTimer, xTime );
}
/*-----------------------------------------------------------*/

/**
 * @brief Check the IP timer to see whether an IP event should be processed or not.
 *
 * @param[in] pxTimer: Pointer to the IP timer.
 *
 * @return If the timer is expired then pdTRUE is returned. Else pdFALSE.
 */
static BaseType_t prvIPTimerCheck( IPTimer_t * pxTimer )
{
    BaseType_t xReturn;

    if( pxTimer->bActive == pdFALSE_UNSIGNED )
    {
        /* The timer is not enabled. */
        xReturn = pdFALSE;
    }
    else
    {
        /* The timer might have set the bExpired flag already, if not, check the
         * value of xTimeOut against ulRemainingTime. */
        if( pxTimer->bExpired == pdFALSE_UNSIGNED )
        {
            if( xTaskCheckForTimeOut( &( pxTimer->xTimeOut ), &( pxTimer->ulRemainingTime ) ) != pdFALSE )
            {
                pxTimer->bExpired = pdTRUE_UNSIGNED;
            }
        }

        if( pxTimer->bExpired != pdFALSE_UNSIGNED )
        {
            prvIPTimerStart( pxTimer, pxTimer->ulReloadTime );
            xReturn = pdTRUE;
        }
        else
        {
            xReturn = pdFALSE;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Send a network down event to the IP-task. If it fails to post a message,
 *         the failure will be noted in the variable 'xNetworkDownEventPending'
 *         and later on a 'network-down' event, it will be executed.
 */
void FreeRTOS_NetworkDown( void )
{
    static const IPStackEvent_t xNetworkDownEvent = { eNetworkDownEvent, NULL };
    const TickType_t xDontBlock = ( TickType_t ) 0;

    /* Simply send the network task the appropriate event. */
    if( xSendEventStructToIPTask( &xNetworkDownEvent, xDontBlock ) != pdPASS )
    {
        /* Could not send the message, so it is still pending. */
        xNetworkDownEventPending = pdTRUE;
    }
    else
    {
        /* Message was sent so it is not pending. */
        xNetworkDownEventPending = pdFALSE;
    }

    iptraceNETWORK_DOWN();
}
/*-----------------------------------------------------------*/

/**
 * @brief Utility function. Process Network Down event from ISR.
 *        This function is supposed to be called form an ISR. It is recommended
 * - *        use 'FreeRTOS_NetworkDown()', when calling from a normal task.
 *
 * @return If the event was processed successfully, then return pdTRUE.
 *         Else pdFALSE.
 */
BaseType_t FreeRTOS_NetworkDownFromISR( void )
{
    static const IPStackEvent_t xNetworkDownEvent = { eNetworkDownEvent, NULL };
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    /* Simply send the network task the appropriate event. */
    if( xQueueSendToBackFromISR( xNetworkEventQueue, &xNetworkDownEvent, &xHigherPriorityTaskWoken ) != pdPASS )
    {
        xNetworkDownEventPending = pdTRUE;
    }
    else
    {
        xNetworkDownEventPending = pdFALSE;
    }

    iptraceNETWORK_DOWN();

    return xHigherPriorityTaskWoken;
}
/*-----------------------------------------------------------*/

/**
 * @brief Obtain a buffer big enough for a UDP payload of given size.
 *
 * @param[in] uxRequestedSizeBytes: The size of the UDP payload.
 * @param[in] uxBlockTimeTicks: Maximum amount of time for which this call
 *            can block. This value is capped internally.
 *
 * @return If a buffer was created then the pointer to that buffer is returned,
 *         else a NULL pointer is returned.
 */
void * FreeRTOS_GetUDPPayloadBuffer( size_t uxRequestedSizeBytes,
                                     TickType_t uxBlockTimeTicks )
{
    NetworkBufferDescriptor_t * pxNetworkBuffer;
    void * pvReturn;
    TickType_t uxBlockTime = uxBlockTimeTicks;

    /* Cap the block time.  The reason for this is explained where
     * ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS is defined (assuming an official
     * FreeRTOSIPConfig.h header file is being used). */
    if( uxBlockTime > ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS )
    {
        uxBlockTime = ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS;
    }

    /* Obtain a network buffer with the required amount of storage. */
    pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( sizeof( UDPPacket_t ) + uxRequestedSizeBytes, uxBlockTime );

    if( pxNetworkBuffer != NULL )
    {
        /* Set the actual packet size in case a bigger buffer was returned. */
        pxNetworkBuffer->xDataLength = sizeof( UDPPacket_t ) + uxRequestedSizeBytes;
        /* Skip 3 headers. */
        pvReturn = &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( UDPPacket_t ) ] );
    }
    else
    {
        pvReturn = NULL;
    }

    return ( void * ) pvReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Duplicate the given network buffer descriptor with a modified length.
 *
 * @param[in] pxNetworkBuffer: The network buffer to be duplicated.
 * @param[in] uxNewLength: The length for the new buffer.
 *
 * @return If properly duplicated, then the duplicate network buffer or else, NULL.
 */
NetworkBufferDescriptor_t * pxDuplicateNetworkBufferWithDescriptor( const NetworkBufferDescriptor_t * const pxNetworkBuffer,
                                                                    size_t uxNewLength )
{
    NetworkBufferDescriptor_t * pxNewBuffer;

    /* This function is only used when 'ipconfigZERO_COPY_TX_DRIVER' is set to 1.
     * The transmit routine wants to have ownership of the network buffer
     * descriptor, because it will pass the buffer straight to DMA. */
    pxNewBuffer = pxGetNetworkBufferWithDescriptor( uxNewLength, ( TickType_t ) 0 );

    if( pxNewBuffer != NULL )
    {
        /* Set the actual packet size in case a bigger buffer than requested
         * was returned. */
        pxNewBuffer->xDataLength = uxNewLength;

        /* Copy the original packet information. */
        pxNewBuffer->ulIPAddress = pxNetworkBuffer->ulIPAddress;
        pxNewBuffer->usPort = pxNetworkBuffer->usPort;
        pxNewBuffer->usBoundPort = pxNetworkBuffer->usBoundPort;
        ( void ) memcpy( pxNewBuffer->pucEthernetBuffer, pxNetworkBuffer->pucEthernetBuffer, pxNetworkBuffer->xDataLength );
    }

    return pxNewBuffer;
}
/*-----------------------------------------------------------*/

/**
 * @brief Get the network buffer descriptor from the packet buffer.
 *
 * @param[in] pvBuffer: The pointer to packet buffer.
 * @param[in] uxOffset: Additional offset (such as the packet length of UDP packet etc.).
 *
 * @return The network buffer descriptor if the alignment is correct. Else a NULL is returned.
 */
static NetworkBufferDescriptor_t * prvPacketBuffer_to_NetworkBuffer( const void * pvBuffer,
                                                                     size_t uxOffset )
{
    uintptr_t uxBuffer;
    NetworkBufferDescriptor_t * pxResult;

    if( pvBuffer == NULL )
    {
        pxResult = NULL;
    }
    else
    {
        /* Obtain the network buffer from the zero copy pointer. */
        uxBuffer = ipPOINTER_CAST( uintptr_t, pvBuffer );

        /* The input here is a pointer to a packet buffer plus some offset.  Subtract
         * this offset, and also the size of the header in the network buffer, usually
         * 8 + 2 bytes. */
        uxBuffer -= ( uxOffset + ipBUFFER_PADDING );

        /* Here a pointer was placed to the network descriptor.  As a
         * pointer is dereferenced, make sure it is well aligned. */
        if( ( uxBuffer & ( ( ( uintptr_t ) sizeof( uxBuffer ) ) - 1U ) ) == ( uintptr_t ) 0U )
        {
            /* The following statement may trigger a:
             * warning: cast increases required alignment of target type [-Wcast-align].
             * It has been confirmed though that the alignment is suitable. */
            pxResult = *( ( NetworkBufferDescriptor_t ** ) uxBuffer );
        }
        else
        {
            pxResult = NULL;
        }
    }

    return pxResult;
}
/*-----------------------------------------------------------*/

#if ( ipconfigZERO_COPY_TX_DRIVER != 0 ) || ( ipconfigZERO_COPY_RX_DRIVER != 0 )

/**
 * @brief Get the network buffer from the packet buffer.
 *
 * @param[in] pvBuffer: Pointer to the packet buffer.
 *
 * @return The network buffer if the alignment is correct. Else a NULL is returned.
 */
    NetworkBufferDescriptor_t * pxPacketBuffer_to_NetworkBuffer( const void * pvBuffer )
    {
        return prvPacketBuffer_to_NetworkBuffer( pvBuffer, 0U );
    }

#endif /* ( ipconfigZERO_COPY_TX_DRIVER != 0 ) || ( ipconfigZERO_COPY_RX_DRIVER != 0 ) */
/*-----------------------------------------------------------*/

/**
 * @brief Get the network buffer from the UDP Payload buffer.
 *
 * @param[in] pvBuffer: Pointer to the UDP payload buffer.
 *
 * @return The network buffer if the alignment is correct. Else a NULL is returned.
 */
NetworkBufferDescriptor_t * pxUDPPayloadBuffer_to_NetworkBuffer( const void * pvBuffer )
{
    return prvPacketBuffer_to_NetworkBuffer( pvBuffer, sizeof( UDPPacket_t ) );
}
/*-----------------------------------------------------------*/

/**
 * @brief Release the UDP payload buffer.
 *
 * @param[in] pvBuffer: Pointer to the UDP buffer that is to be released.
 */
void FreeRTOS_ReleaseUDPPayloadBuffer( void const * pvBuffer )
{
    vReleaseNetworkBufferAndDescriptor( pxUDPPayloadBuffer_to_NetworkBuffer( pvBuffer ) );
}
/*-----------------------------------------------------------*/

/*_RB_ Should we add an error or assert if the task priorities are set such that the servers won't function as expected? */

/*_HT_ There was a bug in FreeRTOS_TCP_IP.c that only occurred when the applications' priority was too high.
 * As that bug has been repaired, there is not an urgent reason to warn.
 * It is better though to use the advised priority scheme. */

/**
 * @brief Initialise the FreeRTOS-Plus-TCP network stack and initialise the IP-task.
 *
 * @param[in] ucIPAddress: Local IP address.
 * @param[in] ucNetMask: Local netmask.
 * @param[in] ucGatewayAddress: Local gateway address.
 * @param[in] ucDNSServerAddress: Local DNS server address.
 * @param[in] ucMACAddress: MAC address of the node.
 *
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 */
BaseType_t FreeRTOS_IPInit( const uint8_t ucIPAddress[ ipIP_ADDRESS_LENGTH_BYTES ],
                            const uint8_t ucNetMask[ ipIP_ADDRESS_LENGTH_BYTES ],
                            const uint8_t ucGatewayAddress[ ipIP_ADDRESS_LENGTH_BYTES ],
                            const uint8_t ucDNSServerAddress[ ipIP_ADDRESS_LENGTH_BYTES ],
                            const uint8_t ucMACAddress[ ipMAC_ADDRESS_LENGTH_BYTES ] )
{
    BaseType_t xReturn = pdFALSE;

    /* This function should only be called once. */
    configASSERT( xIPIsNetworkTaskReady() == pdFALSE );
    configASSERT( xNetworkEventQueue == NULL );
    configASSERT( xIPTaskHandle == NULL );

    if( sizeof( uintptr_t ) == 8 )
    {
        /* This is a 64-bit platform, make sure there is enough space in
         * pucEthernetBuffer to store a pointer. */
        configASSERT( ipconfigBUFFER_PADDING == 14 );
    }

    #ifndef _lint
        {
            /* Check if MTU is big enough. */
            configASSERT( ( ( size_t ) ipconfigNETWORK_MTU ) >= ( ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_TCP_HEADER + ipconfigTCP_MSS ) );
            /* Check structure packing is correct. */
            configASSERT( sizeof( EthernetHeader_t ) == ipEXPECTED_EthernetHeader_t_SIZE );
            configASSERT( sizeof( ARPHeader_t ) == ipEXPECTED_ARPHeader_t_SIZE );
            configASSERT( sizeof( IPHeader_t ) == ipEXPECTED_IPHeader_t_SIZE );
            configASSERT( sizeof( ICMPHeader_t ) == ipEXPECTED_ICMPHeader_t_SIZE );
            configASSERT( sizeof( UDPHeader_t ) == ipEXPECTED_UDPHeader_t_SIZE );
        }
    #endif /* ifndef _lint */
    /* Attempt to create the queue used to communicate with the IP task. */
    xNetworkEventQueue = xQueueCreate( ipconfigEVENT_QUEUE_LENGTH, sizeof( IPStackEvent_t ) );
    configASSERT( xNetworkEventQueue != NULL );

    if( xNetworkEventQueue != NULL )
    {
        #if ( configQUEUE_REGISTRY_SIZE > 0 )
            {
                /* A queue registry is normally used to assist a kernel aware
                 * debugger.  If one is in use then it will be helpful for the debugger
                 * to show information about the network event queue. */
                vQueueAddToRegistry( xNetworkEventQueue, "NetEvnt" );
            }
        #endif /* configQUEUE_REGISTRY_SIZE */

        if( xNetworkBuffersInitialise() == pdPASS )
        {
            /* Store the local IP and MAC address. */
            xNetworkAddressing.ulDefaultIPAddress = FreeRTOS_inet_addr_quick( ucIPAddress[ 0 ], ucIPAddress[ 1 ], ucIPAddress[ 2 ], ucIPAddress[ 3 ] );
            xNetworkAddressing.ulNetMask = FreeRTOS_inet_addr_quick( ucNetMask[ 0 ], ucNetMask[ 1 ], ucNetMask[ 2 ], ucNetMask[ 3 ] );
            xNetworkAddressing.ulGatewayAddress = FreeRTOS_inet_addr_quick( ucGatewayAddress[ 0 ], ucGatewayAddress[ 1 ], ucGatewayAddress[ 2 ], ucGatewayAddress[ 3 ] );
            xNetworkAddressing.ulDNSServerAddress = FreeRTOS_inet_addr_quick( ucDNSServerAddress[ 0 ], ucDNSServerAddress[ 1 ], ucDNSServerAddress[ 2 ], ucDNSServerAddress[ 3 ] );
            xNetworkAddressing.ulBroadcastAddress = ( xNetworkAddressing.ulDefaultIPAddress & xNetworkAddressing.ulNetMask ) | ~xNetworkAddressing.ulNetMask;

            ( void ) memcpy( &xDefaultAddressing, &xNetworkAddressing, sizeof( xDefaultAddressing ) );

            #if ipconfigUSE_DHCP == 1
                {
                    /* The IP address is not set until DHCP completes. */
                    *ipLOCAL_IP_ADDRESS_POINTER = 0x00UL;
                }
            #else
                {
                    /* The IP address is set from the value passed in. */
                    *ipLOCAL_IP_ADDRESS_POINTER = xNetworkAddressing.ulDefaultIPAddress;

                    /* Added to prevent ARP flood to gateway.  Ensure the
                    * gateway is on the same subnet as the IP address. */
                    if( xNetworkAddressing.ulGatewayAddress != 0UL )
                    {
                        configASSERT( ( ( *ipLOCAL_IP_ADDRESS_POINTER ) & xNetworkAddressing.ulNetMask ) == ( xNetworkAddressing.ulGatewayAddress & xNetworkAddressing.ulNetMask ) );
                    }
                }
            #endif /* ipconfigUSE_DHCP == 1 */

            /* The MAC address is stored in the start of the default packet
             * header fragment, which is used when sending UDP packets. */
            ( void ) memcpy( ipLOCAL_MAC_ADDRESS, ucMACAddress, ( size_t ) ipMAC_ADDRESS_LENGTH_BYTES );

            /* Prepare the sockets interface. */
            vNetworkSocketsInit();

            /* Create the task that processes Ethernet and stack events. */
            xReturn = xTaskCreate( prvIPTask,
                                   "IP-task",
                                   ipconfigIP_TASK_STACK_SIZE_WORDS,
                                   NULL,
                                   ipconfigIP_TASK_PRIORITY,
                                   &( xIPTaskHandle ) );
        }
        else
        {
            FreeRTOS_debug_printf( ( "FreeRTOS_IPInit: xNetworkBuffersInitialise() failed\n" ) );

            /* Clean up. */
            vQueueDelete( xNetworkEventQueue );
            xNetworkEventQueue = NULL;
        }
    }
    else
    {
        FreeRTOS_debug_printf( ( "FreeRTOS_IPInit: Network event queue could not be created\n" ) );
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Get the current address configuration. Only non-NULL pointers will
 *        be filled in.
 *
 * @param[out] pulIPAddress: The current IP-address assigned.
 * @param[out] pulNetMask: The netmask used for current subnet.
 * @param[out] pulGatewayAddress: The gateway address.
 * @param[out] pulDNSServerAddress: The DNS server address.
 */
void FreeRTOS_GetAddressConfiguration( uint32_t * pulIPAddress,
                                       uint32_t * pulNetMask,
                                       uint32_t * pulGatewayAddress,
                                       uint32_t * pulDNSServerAddress )
{
    /* Return the address configuration to the caller. */

    if( pulIPAddress != NULL )
    {
        *pulIPAddress = *ipLOCAL_IP_ADDRESS_POINTER;
    }

    if( pulNetMask != NULL )
    {
        *pulNetMask = xNetworkAddressing.ulNetMask;
    }

    if( pulGatewayAddress != NULL )
    {
        *pulGatewayAddress = xNetworkAddressing.ulGatewayAddress;
    }

    if( pulDNSServerAddress != NULL )
    {
        *pulDNSServerAddress = xNetworkAddressing.ulDNSServerAddress;
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Set the current network address configuration. Only non-NULL pointers will
 *        be used.
 *
 * @param[in] pulIPAddress: The current IP-address assigned.
 * @param[in] pulNetMask: The netmask used for current subnet.
 * @param[in] pulGatewayAddress: The gateway address.
 * @param[in] pulDNSServerAddress: The DNS server address.
 */
void FreeRTOS_SetAddressConfiguration( const uint32_t * pulIPAddress,
                                       const uint32_t * pulNetMask,
                                       const uint32_t * pulGatewayAddress,
                                       const uint32_t * pulDNSServerAddress )
{
    /* Update the address configuration. */

    if( pulIPAddress != NULL )
    {
        *ipLOCAL_IP_ADDRESS_POINTER = *pulIPAddress;
    }

    if( pulNetMask != NULL )
    {
        xNetworkAddressing.ulNetMask = *pulNetMask;
    }

    if( pulGatewayAddress != NULL )
    {
        xNetworkAddressing.ulGatewayAddress = *pulGatewayAddress;
    }

    if( pulDNSServerAddress != NULL )
    {
        xNetworkAddressing.ulDNSServerAddress = *pulDNSServerAddress;
    }
}
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )

/**
 * @brief Send a ping request to the given IP address. After receiving a reply,
 *        IP-task will call a user-supplied function 'vApplicationPingReplyHook()'.
 *
 * @param[in] ulIPAddress: The IP address to which the ping is to be sent.
 * @param[in] uxNumberOfBytesToSend: Number of bytes in the ping request.
 * @param[in] uxBlockTimeTicks: Maximum number of ticks to wait.
 *
 * @return If successfully sent to IP task for processing then the sequence
 *         number of the ping packet or else, pdFAIL.
 */
    BaseType_t FreeRTOS_SendPingRequest( uint32_t ulIPAddress,
                                         size_t uxNumberOfBytesToSend,
                                         TickType_t uxBlockTimeTicks )
    {
        NetworkBufferDescriptor_t * pxNetworkBuffer;
        ICMPHeader_t * pxICMPHeader;
        EthernetHeader_t * pxEthernetHeader;
        BaseType_t xReturn = pdFAIL;
        static uint16_t usSequenceNumber = 0;
        uint8_t * pucChar;
        size_t uxTotalLength;
        IPStackEvent_t xStackTxEvent = { eStackTxEvent, NULL };

        uxTotalLength = uxNumberOfBytesToSend + sizeof( ICMPPacket_t );
        BaseType_t xEnoughSpace;

        if( uxNumberOfBytesToSend < ( ipconfigNETWORK_MTU - ( sizeof( IPHeader_t ) + sizeof( ICMPHeader_t ) ) ) )
        {
            xEnoughSpace = pdTRUE;
        }
        else
        {
            xEnoughSpace = pdFALSE;
        }

        if( ( uxGetNumberOfFreeNetworkBuffers() >= 4U ) && ( uxNumberOfBytesToSend >= 1U ) && ( xEnoughSpace != pdFALSE ) )
        {
            pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( uxTotalLength, uxBlockTimeTicks );

            if( pxNetworkBuffer != NULL )
            {
                pxEthernetHeader = ipCAST_PTR_TO_TYPE_PTR( EthernetHeader_t, pxNetworkBuffer->pucEthernetBuffer );
                pxEthernetHeader->usFrameType = ipIPv4_FRAME_TYPE;

                pxICMPHeader = ipCAST_PTR_TO_TYPE_PTR( ICMPHeader_t, &( pxNetworkBuffer->pucEthernetBuffer[ ipIP_PAYLOAD_OFFSET ] ) );
                usSequenceNumber++;

                /* Fill in the basic header information. */
                pxICMPHeader->ucTypeOfMessage = ipICMP_ECHO_REQUEST;
                pxICMPHeader->ucTypeOfService = 0;
                pxICMPHeader->usIdentifier = usSequenceNumber;
                pxICMPHeader->usSequenceNumber = usSequenceNumber;

                /* Find the start of the data. */
                pucChar = ( uint8_t * ) pxICMPHeader;
                pucChar = &( pucChar[ sizeof( ICMPHeader_t ) ] );

                /* Just memset the data to a fixed value. */
                ( void ) memset( pucChar, ( int ) ipECHO_DATA_FILL_BYTE, uxNumberOfBytesToSend );

                /* The message is complete, IP and checksum's are handled by
                 * vProcessGeneratedUDPPacket */
                pxNetworkBuffer->pucEthernetBuffer[ ipSOCKET_OPTIONS_OFFSET ] = FREERTOS_SO_UDPCKSUM_OUT;
                pxNetworkBuffer->ulIPAddress = ulIPAddress;
                pxNetworkBuffer->usPort = ipPACKET_CONTAINS_ICMP_DATA;
                /* xDataLength is the size of the total packet, including the Ethernet header. */
                pxNetworkBuffer->xDataLength = uxTotalLength;

                /* Send to the stack. */
                xStackTxEvent.pvData = pxNetworkBuffer;

                if( xSendEventStructToIPTask( &( xStackTxEvent ), uxBlockTimeTicks ) != pdPASS )
                {
                    vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                    iptraceSTACK_TX_EVENT_LOST( ipSTACK_TX_EVENT );
                }
                else
                {
                    xReturn = ( BaseType_t ) usSequenceNumber;
                }
            }
        }
        else
        {
            /* The requested number of bytes will not fit in the available space
             * in the network buffer. */
        }

        return xReturn;
    }

#endif /* ipconfigSUPPORT_OUTGOING_PINGS == 1 */
/*-----------------------------------------------------------*/

/**
 * @brief Send an event to the IP task. It calls 'xSendEventStructToIPTask' internally.
 *
 * @param[in] eEvent: The event to be sent.
 *
 * @return pdPASS if the event was sent (or the desired effect was achieved). Else, pdFAIL.
 */
BaseType_t xSendEventToIPTask( eIPEvent_t eEvent )
{
    IPStackEvent_t xEventMessage;
    const TickType_t xDontBlock = ( TickType_t ) 0;

    xEventMessage.eEventType = eEvent;
    xEventMessage.pvData = ( void * ) NULL;

    return xSendEventStructToIPTask( &xEventMessage, xDontBlock );
}
/*-----------------------------------------------------------*/

/**
 * @brief Send an event (in form of struct) to the IP task to be processed.
 *
 * @param[in] pxEvent: The event to be sent.
 * @param[in] uxTimeout: Timeout for waiting in case the queue is full. 0 for non-blocking calls.
 *
 * @return pdPASS if the event was sent (or the desired effect was achieved). Else, pdFAIL.
 */
BaseType_t xSendEventStructToIPTask( const IPStackEvent_t * pxEvent,
                                     TickType_t uxTimeout )
{
    BaseType_t xReturn, xSendMessage;
    TickType_t uxUseTimeout = uxTimeout;

    if( ( xIPIsNetworkTaskReady() == pdFALSE ) && ( pxEvent->eEventType != eNetworkDownEvent ) )
    {
        /* Only allow eNetworkDownEvent events if the IP task is not ready
         * yet.  Not going to attempt to send the message so the send failed. */
        xReturn = pdFAIL;
    }
    else
    {
        xSendMessage = pdTRUE;

        #if ( ipconfigUSE_TCP == 1 )
            {
                if( pxEvent->eEventType == eTCPTimerEvent )
                {
                    /* TCP timer events are sent to wake the timer task when
                     * xTCPTimer has expired, but there is no point sending them if the
                     * IP task is already awake processing other message. */
                    xTCPTimer.bExpired = pdTRUE_UNSIGNED;

                    if( uxQueueMessagesWaiting( xNetworkEventQueue ) != 0U )
                    {
                        /* Not actually going to send the message but this is not a
                         * failure as the message didn't need to be sent. */
                        xSendMessage = pdFALSE;
                    }
                }
            }
        #endif /* ipconfigUSE_TCP */

        if( xSendMessage != pdFALSE )
        {
            /* The IP task cannot block itself while waiting for itself to
             * respond. */
            if( ( xIsCallingFromIPTask() == pdTRUE ) && ( uxUseTimeout > ( TickType_t ) 0U ) )
            {
                uxUseTimeout = ( TickType_t ) 0;
            }

            xReturn = xQueueSendToBack( xNetworkEventQueue, pxEvent, uxUseTimeout );

            if( xReturn == pdFAIL )
            {
                /* A message should have been sent to the IP task, but wasn't. */
                FreeRTOS_debug_printf( ( "xSendEventStructToIPTask: CAN NOT ADD %d\n", pxEvent->eEventType ) );
                iptraceSTACK_TX_EVENT_LOST( pxEvent->eEventType );
            }
        }
        else
        {
            /* It was not necessary to send the message to process the event so
             * even though the message was not sent the call was successful. */
            xReturn = pdPASS;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_DHCP != 0 )

/**
 * @brief Create a DHCP event.
 *
 * @return pdPASS or pdFAIL, depending on whether xSendEventStructToIPTask()
 *         succeeded.
 */
    BaseType_t xSendDHCPEvent( void )
    {
        IPStackEvent_t xEventMessage;
        const TickType_t uxDontBlock = 0U;
        uintptr_t uxOption = eGetDHCPState();

        xEventMessage.eEventType = eDHCPEvent;
        xEventMessage.pvData = ( void * ) uxOption;

        return xSendEventStructToIPTask( &xEventMessage, uxDontBlock );
    }
/*-----------------------------------------------------------*/
#endif /* ( ipconfigUSE_DHCP != 0 ) */

/**
 * @brief Decide whether this packet should be processed or not based on the IP address in the packet.
 *
 * @param[in] pucEthernetBuffer: The ethernet packet under consideration.
 *
 * @return Enum saying whether to release or to process the packet.
 */
eFrameProcessingResult_t eConsiderFrameForProcessing( const uint8_t * const pucEthernetBuffer )
{
    eFrameProcessingResult_t eReturn;
    const EthernetHeader_t * pxEthernetHeader;

    /* Map the buffer onto Ethernet Header struct for easy access to fields. */
    pxEthernetHeader = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( EthernetHeader_t, pucEthernetBuffer );

    if( memcmp( ipLOCAL_MAC_ADDRESS, pxEthernetHeader->xDestinationAddress.ucBytes, sizeof( MACAddress_t ) ) == 0 )
    {
        /* The packet was directed to this node - process it. */
        eReturn = eProcessBuffer;
    }
    else if( memcmp( xBroadcastMACAddress.ucBytes, pxEthernetHeader->xDestinationAddress.ucBytes, sizeof( MACAddress_t ) ) == 0 )
    {
        /* The packet was a broadcast - process it. */
        eReturn = eProcessBuffer;
    }
    else
    #if ( ipconfigUSE_LLMNR == 1 )
        if( memcmp( xLLMNR_MacAdress.ucBytes, pxEthernetHeader->xDestinationAddress.ucBytes, sizeof( MACAddress_t ) ) == 0 )
        {
            /* The packet is a request for LLMNR - process it. */
            eReturn = eProcessBuffer;
        }
        else
    #endif /* ipconfigUSE_LLMNR */
    {
        /* The packet was not a broadcast, or for this node, just release
         * the buffer without taking any other action. */
        eReturn = eReleaseBuffer;
    }

    #if ( ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES == 1 )
        {
            uint16_t usFrameType;

            if( eReturn == eProcessBuffer )
            {
                usFrameType = pxEthernetHeader->usFrameType;
                usFrameType = FreeRTOS_ntohs( usFrameType );

                if( usFrameType <= 0x600U )
                {
                    /* Not an Ethernet II frame. */
                    eReturn = eReleaseBuffer;
                }
            }
        }
    #endif /* ipconfigFILTER_OUT_NON_ETHERNET_II_FRAMES == 1  */

    return eReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Process a 'Network down' event and complete required processing.
 */
static void prvProcessNetworkDownEvent( void )
{
    /* Stop the ARP timer while there is no network. */
    xARPTimer.bActive = pdFALSE_UNSIGNED;

    #if ipconfigUSE_NETWORK_EVENT_HOOK == 1
        {
            static BaseType_t xCallEventHook = pdFALSE;

            /* The first network down event is generated by the IP stack itself to
             * initialise the network hardware, so do not call the network down event
             * the first time through. */
            if( xCallEventHook == pdTRUE )
            {
                vApplicationIPNetworkEventHook( eNetworkDown );
            }

            xCallEventHook = pdTRUE;
        }
    #endif /* if ipconfigUSE_NETWORK_EVENT_HOOK == 1 */

    /* Per the ARP Cache Validation section of https://tools.ietf.org/html/rfc1122,
     * treat network down as a "delivery problem" and flush the ARP cache for this
     * interface. */
    FreeRTOS_ClearARP();

    /* The network has been disconnected (or is being initialised for the first
     * time).  Perform whatever hardware processing is necessary to bring it up
     * again, or wait for it to be available again.  This is hardware dependent. */
    if( xNetworkInterfaceInitialise() != pdPASS )
    {
        /* Ideally the network interface initialisation function will only
         * return when the network is available.  In case this is not the case,
         * wait a while before retrying the initialisation. */
        vTaskDelay( ipINITIALISATION_RETRY_DELAY );
        FreeRTOS_NetworkDown();
    }
    else
    {
        /* Set remaining time to 0 so it will become active immediately. */
        #if ipconfigUSE_DHCP == 1
            {
                /* The network is not up until DHCP has completed. */
                vDHCPProcess( pdTRUE, eInitialWait );
            }
        #else
            {
                /* Perform any necessary 'network up' processing. */
                vIPNetworkUpCalls();
            }
        #endif
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Perform all the required tasks when the network gets connected.
 */
void vIPNetworkUpCalls( void )
{
    xNetworkUp = pdTRUE;

    #if ( ipconfigUSE_NETWORK_EVENT_HOOK == 1 )
        {
            vApplicationIPNetworkEventHook( eNetworkUp );
        }
    #endif /* ipconfigUSE_NETWORK_EVENT_HOOK */

    #if ( ipconfigDNS_USE_CALLBACKS != 0 )
        {
            /* The following function is declared in FreeRTOS_DNS.c and 'private' to
             * this library */
            extern void vDNSInitialise( void );
            vDNSInitialise();
        }
    #endif /* ipconfigDNS_USE_CALLBACKS != 0 */

    /* Set remaining time to 0 so it will become active immediately. */
    prvIPTimerReload( &xARPTimer, pdMS_TO_TICKS( ipARP_TIMER_PERIOD_MS ) );
}
/*-----------------------------------------------------------*/

/**
 * @brief Process the Ethernet packet.
 *
 * @param[in,out] pxNetworkBuffer: the network buffer containing the ethernet packet. If the
 *                                 buffer is large enough, it may be reused to send a reply.
 */
static void prvProcessEthernetPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
    const EthernetHeader_t * pxEthernetHeader;
    eFrameProcessingResult_t eReturned = eReleaseBuffer;

    configASSERT( pxNetworkBuffer != NULL );

    iptraceNETWORK_INTERFACE_INPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );

    /* Interpret the Ethernet frame. */
    if( pxNetworkBuffer->xDataLength >= sizeof( EthernetHeader_t ) )
    {
        eReturned = ipCONSIDER_FRAME_FOR_PROCESSING( pxNetworkBuffer->pucEthernetBuffer );

        /* Map the buffer onto the Ethernet Header struct for easy access to the fields. */
        pxEthernetHeader = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( EthernetHeader_t, pxNetworkBuffer->pucEthernetBuffer );

        /* The condition "eReturned == eProcessBuffer" must be true. */
        #if ( ipconfigETHERNET_DRIVER_FILTERS_FRAME_TYPES == 0 )
            if( eReturned == eProcessBuffer )
        #endif
        {
            /* Interpret the received Ethernet packet. */
            switch( pxEthernetHeader->usFrameType )
            {
                case ipARP_FRAME_TYPE:

                    /* The Ethernet frame contains an ARP packet. */
                    if( pxNetworkBuffer->xDataLength >= sizeof( ARPPacket_t ) )
                    {
                        eReturned = eARPProcessPacket( ipCAST_PTR_TO_TYPE_PTR( ARPPacket_t, pxNetworkBuffer->pucEthernetBuffer ) );
                    }
                    else
                    {
                        eReturned = eReleaseBuffer;
                    }

                    break;

                case ipIPv4_FRAME_TYPE:

                    /* The Ethernet frame contains an IP packet. */
                    if( pxNetworkBuffer->xDataLength >= sizeof( IPPacket_t ) )
                    {
                        eReturned = prvProcessIPPacket( ipCAST_PTR_TO_TYPE_PTR( IPPacket_t, pxNetworkBuffer->pucEthernetBuffer ), pxNetworkBuffer );
                    }
                    else
                    {
                        eReturned = eReleaseBuffer;
                    }

                    break;

                default:
                    /* No other packet types are handled.  Nothing to do. */
                    eReturned = eReleaseBuffer;
                    break;
            }
        }
    }

    /* Perform any actions that resulted from processing the Ethernet frame. */
    switch( eReturned )
    {
        case eReturnEthernetFrame:

            /* The Ethernet frame will have been updated (maybe it was
             * an ARP request or a PING request?) and should be sent back to
             * its source. */
            vReturnEthernetFrame( pxNetworkBuffer, pdTRUE );

            /* parameter pdTRUE: the buffer must be released once
             * the frame has been transmitted */
            break;

        case eFrameConsumed:

            /* The frame is in use somewhere, don't release the buffer
             * yet. */
            break;

        case eReleaseBuffer:
        case eProcessBuffer:
        default:

            /* The frame is not being used anywhere, and the
             * NetworkBufferDescriptor_t structure containing the frame should
             * just be released back to the list of free buffers. */
            vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
            break;
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Is the IP address an IPv4 multicast address.
 *
 * @param[in] ulIPAddress: The IP address being checked.
 *
 * @return pdTRUE if the IP address is a multicast address or else, pdFALSE.
 */
BaseType_t xIsIPv4Multicast( uint32_t ulIPAddress )
{
    BaseType_t xReturn;
    uint32_t ulIP = FreeRTOS_ntohl( ulIPAddress );

    if( ( ulIP >= ipFIRST_MULTI_CAST_IPv4 ) && ( ulIP < ipLAST_MULTI_CAST_IPv4 ) )
    {
        xReturn = pdTRUE;
    }
    else
    {
        xReturn = pdFALSE;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Set multicast MAC address.
 *
 * @param[in] ulIPAddress: IP address.
 * @param[out] pxMACAddress: Pointer to MAC address.
 */
void vSetMultiCastIPv4MacAddress( uint32_t ulIPAddress,
                                  MACAddress_t * pxMACAddress )
{
    uint32_t ulIP = FreeRTOS_ntohl( ulIPAddress );

    pxMACAddress->ucBytes[ 0 ] = ( uint8_t ) 0x01U;
    pxMACAddress->ucBytes[ 1 ] = ( uint8_t ) 0x00U;
    pxMACAddress->ucBytes[ 2 ] = ( uint8_t ) 0x5EU;
    pxMACAddress->ucBytes[ 3 ] = ( uint8_t ) ( ( ulIP >> 16 ) & 0x7fU ); /* Use 7 bits. */
    pxMACAddress->ucBytes[ 4 ] = ( uint8_t ) ( ( ulIP >> 8 ) & 0xffU );  /* Use 8 bits. */
    pxMACAddress->ucBytes[ 5 ] = ( uint8_t ) ( ( ulIP ) & 0xffU );       /* Use 8 bits. */
}
/*-----------------------------------------------------------*/

/**
 * @brief Check whether this IP packet is to be allowed or to be dropped.
 *
 * @param[in] pxIPPacket: The IP packet under consideration.
 * @param[in] pxNetworkBuffer: The whole network buffer.
 * @param[in] uxHeaderLength: The length of the header.
 *
 * @return Whether the packet should be processed or dropped.
 */
static eFrameProcessingResult_t prvAllowIPPacket( const IPPacket_t * const pxIPPacket,
                                                  const NetworkBufferDescriptor_t * const pxNetworkBuffer,
                                                  UBaseType_t uxHeaderLength )
{
    eFrameProcessingResult_t eReturn = eProcessBuffer;

    #if ( ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 0 ) || ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 0 ) )
        const IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
    #else

        /* or else, the parameter won't be used and the function will be optimised
         * away */
        ( void ) pxIPPacket;
    #endif

    #if ( ipconfigETHERNET_DRIVER_FILTERS_PACKETS == 0 )
        {
            /* In systems with a very small amount of RAM, it might be advantageous
             * to have incoming messages checked earlier, by the network card driver.
             * This method may decrease the usage of sparse network buffers. */
            uint32_t ulDestinationIPAddress = pxIPHeader->ulDestinationIPAddress;

            /* Ensure that the incoming packet is not fragmented (only outgoing
             * packets can be fragmented) as these are the only handled IP frames
             * currently. */
            if( ( pxIPHeader->usFragmentOffset & ipFRAGMENT_OFFSET_BIT_MASK ) != 0U )
            {
                /* Can not handle, fragmented packet. */
                eReturn = eReleaseBuffer;
            }

            /* Test if the length of the IP-header is between 20 and 60 bytes,
             * and if the IP-version is 4. */
            else if( ( pxIPHeader->ucVersionHeaderLength < ipIPV4_VERSION_HEADER_LENGTH_MIN ) ||
                     ( pxIPHeader->ucVersionHeaderLength > ipIPV4_VERSION_HEADER_LENGTH_MAX ) )
            {
                /* Can not handle, unknown or invalid header version. */
                eReturn = eReleaseBuffer;
            }
            /* Is the packet for this IP address? */
            else if( ( ulDestinationIPAddress != *ipLOCAL_IP_ADDRESS_POINTER ) &&
                     /* Is it the global broadcast address 255.255.255.255 ? */
                     ( ulDestinationIPAddress != ipBROADCAST_IP_ADDRESS ) &&
                     /* Is it a specific broadcast address 192.168.1.255 ? */
                     ( ulDestinationIPAddress != xNetworkAddressing.ulBroadcastAddress ) &&
                     #if ( ipconfigUSE_LLMNR == 1 )
                         /* Is it the LLMNR multicast address? */
                         ( ulDestinationIPAddress != ipLLMNR_IP_ADDR ) &&
                     #endif
                     /* Or (during DHCP negotiation) we have no IP-address yet? */
                     ( *ipLOCAL_IP_ADDRESS_POINTER != 0UL ) )
            {
                /* Packet is not for this node, release it */
                eReturn = eReleaseBuffer;
            }
            else
            {
                /* Packet is not fragmented, destination is this device. */
            }
        }
    #endif /* ipconfigETHERNET_DRIVER_FILTERS_PACKETS */

    #if ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 0 )
        {
            /* Some drivers of NIC's with checksum-offloading will enable the above
             * define, so that the checksum won't be checked again here */
            if( eReturn == eProcessBuffer )
            {
                /* Is the IP header checksum correct? */
                if( ( pxIPHeader->ucProtocol != ( uint8_t ) ipPROTOCOL_ICMP ) &&
                    ( usGenerateChecksum( 0U, ( uint8_t * ) &( pxIPHeader->ucVersionHeaderLength ), ( size_t ) uxHeaderLength ) != ipCORRECT_CRC ) )
                {
                    /* Check sum in IP-header not correct. */
                    eReturn = eReleaseBuffer;
                }
                /* Is the upper-layer checksum (TCP/UDP/ICMP) correct? */
                else if( usGenerateProtocolChecksum( ( uint8_t * ) ( pxNetworkBuffer->pucEthernetBuffer ), pxNetworkBuffer->xDataLength, pdFALSE ) != ipCORRECT_CRC )
                {
                    /* Protocol checksum not accepted. */
                    eReturn = eReleaseBuffer;
                }
                else
                {
                    /* The checksum of the received packet is OK. */
                }
            }
        }
    #else /* if ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 0 ) */
        {
            if( eReturn == eProcessBuffer )
            {
                if( xCheckSizeFields( ( uint8_t * ) ( pxNetworkBuffer->pucEthernetBuffer ), pxNetworkBuffer->xDataLength ) != pdPASS )
                {
                    /* Some of the length checks were not successful. */
                    eReturn = eReleaseBuffer;
                }
            }

            #if ( ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS == 0 )
                {
                    /* Check if this is a UDP packet without a checksum. */
                    if( eReturn == eProcessBuffer )
                    {
                        /* ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS is defined as 0,
                         * and so UDP packets carrying a protocol checksum of 0, will
                         * be dropped. */

                        /* Identify the next protocol. */
                        if( pxIPPacket->xIPHeader.ucProtocol == ( uint8_t ) ipPROTOCOL_UDP )
                        {
                            ProtocolPacket_t * pxProtPack;
                            const uint16_t * pusChecksum;

                            /* pxProtPack will point to the offset were the protocols begin. */
                            pxProtPack = ipCAST_PTR_TO_TYPE_PTR( ProtocolPacket_t, &( pxNetworkBuffer->pucEthernetBuffer[ uxHeaderLength - ipSIZE_OF_IPv4_HEADER ] ) );
                            pusChecksum = ( const uint16_t * ) ( &( pxProtPack->xUDPPacket.xUDPHeader.usChecksum ) );

                            if( *pusChecksum == ( uint16_t ) 0U )
                            {
                                #if ( ipconfigHAS_PRINTF != 0 )
                                    {
                                        static BaseType_t xCount = 0;

                                        if( xCount < 5 )
                                        {
                                            FreeRTOS_printf( ( "prvAllowIPPacket: UDP packet from %xip without CRC dropped\n",
                                                               FreeRTOS_ntohl( pxIPPacket->xIPHeader.ulSourceIPAddress ) ) );
                                            xCount++;
                                        }
                                    }
                                #endif /* ( ipconfigHAS_PRINTF != 0 ) */

                                /* Protocol checksum not accepted. */
                                eReturn = eReleaseBuffer;
                            }
                        }
                    }
                }
            #endif /* ( ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS == 0 ) */

            /* to avoid warning unused parameters */
            ( void ) uxHeaderLength;
        }
    #endif /* ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 0 */

    return eReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief Process an IP-packet.
 *
 * @param[in] pxIPPacket: The IP packet to be processed.
 * @param[in] pxNetworkBuffer: The networkbuffer descriptor having the IP packet.
 *
 * @return An enum to show whether the packet should be released/kept/processed etc.
 */
static eFrameProcessingResult_t prvProcessIPPacket( IPPacket_t * pxIPPacket,
                                                    NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
    eFrameProcessingResult_t eReturn;
    IPHeader_t * pxIPHeader = &( pxIPPacket->xIPHeader );
    size_t uxLength = ( size_t ) pxIPHeader->ucVersionHeaderLength;
    UBaseType_t uxHeaderLength = ( UBaseType_t ) ( ( uxLength & 0x0FU ) << 2 );
    uint8_t ucProtocol;

    /* Bound the calculated header length: take away the Ethernet header size,
     * then check if the IP header is claiming to be longer than the remaining
     * total packet size. Also check for minimal header field length. */
    if( ( uxHeaderLength > ( pxNetworkBuffer->xDataLength - ipSIZE_OF_ETH_HEADER ) ) ||
        ( uxHeaderLength < ipSIZE_OF_IPv4_HEADER ) )
    {
        eReturn = eReleaseBuffer;
    }
    else
    {
        ucProtocol = pxIPPacket->xIPHeader.ucProtocol;
        /* Check if the IP headers are acceptable and if it has our destination. */
        eReturn = prvAllowIPPacket( pxIPPacket, pxNetworkBuffer, uxHeaderLength );

        if( eReturn == eProcessBuffer )
        {
            /* Are there IP-options. */
            if( uxHeaderLength > ipSIZE_OF_IPv4_HEADER )
            {
                /* The size of the IP-header is larger than 20 bytes.
                 * The extra space is used for IP-options. */
                #if ( ipconfigIP_PASS_PACKETS_WITH_IP_OPTIONS != 0 )
                    {
                        /* All structs of headers expect a IP header size of 20 bytes
                         * IP header options were included, we'll ignore them and cut them out. */
                        const size_t optlen = ( ( size_t ) uxHeaderLength ) - ipSIZE_OF_IPv4_HEADER;
                        /* From: the previous start of UDP/ICMP/TCP data. */
                        const uint8_t * pucSource = ( const uint8_t * ) &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( EthernetHeader_t ) + uxHeaderLength ] );
                        /* To: the usual start of UDP/ICMP/TCP data at offset 20 (decimal ) from IP header. */
                        uint8_t * pucTarget = ( uint8_t * ) &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( EthernetHeader_t ) + ipSIZE_OF_IPv4_HEADER ] );
                        /* How many: total length minus the options and the lower headers. */
                        const size_t xMoveLen = pxNetworkBuffer->xDataLength - ( optlen + ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_ETH_HEADER );

                        ( void ) memmove( pucTarget, pucSource, xMoveLen );
                        pxNetworkBuffer->xDataLength -= optlen;

                        /* Rewrite the Version/IHL byte to indicate that this packet has no IP options. */
                        pxIPHeader->ucVersionHeaderLength = ( pxIPHeader->ucVersionHeaderLength & 0xF0U ) | /* High nibble is the version. */
                                                            ( ( ipSIZE_OF_IPv4_HEADER >> 2 ) & 0x0FU );
                    }
                #else /* if ( ipconfigIP_PASS_PACKETS_WITH_IP_OPTIONS != 0 ) */
                    {
                        /* 'ipconfigIP_PASS_PACKETS_WITH_IP_OPTIONS' is not set, so packets carrying
                         * IP-options will be dropped. */
                        eReturn = eReleaseBuffer;
                    }
                #endif /* if ( ipconfigIP_PASS_PACKETS_WITH_IP_OPTIONS != 0 ) */
            }

            if( eReturn != eReleaseBuffer )
            {
                /* Add the IP and MAC addresses to the ARP table if they are not
                 * already there - otherwise refresh the age of the existing
                 * entry. */
                if( ucProtocol != ( uint8_t ) ipPROTOCOL_UDP )
                {
                    /* Refresh the ARP cache with the IP/MAC-address of the received
                     *  packet. For UDP packets, this will be done later in
                     *  xProcessReceivedUDPPacket(), as soon as it's know that the message
                     *  will be handled.  This will prevent the ARP cache getting
                     *  overwritten with the IP address of useless broadcast packets. */
                    vARPRefreshCacheEntry( &( pxIPPacket->xEthernetHeader.xSourceAddress ), pxIPHeader->ulSourceIPAddress );
                }

                switch( ucProtocol )
                {
                    case ipPROTOCOL_ICMP:

                        /* The IP packet contained an ICMP frame.  Don't bother checking
                         * the ICMP checksum, as if it is wrong then the wrong data will
                         * also be returned, and the source of the ping will know something
                         * went wrong because it will not be able to validate what it
                         * receives. */
                        #if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )
                            if( pxNetworkBuffer->xDataLength >= sizeof( ICMPPacket_t ) )
                            {
                                /* Map the buffer onto a ICMP-Packet struct to easily access the
                                 * fields of ICMP packet. */
                                ICMPPacket_t * pxICMPPacket = ipCAST_PTR_TO_TYPE_PTR( ICMPPacket_t, pxNetworkBuffer->pucEthernetBuffer );

                                if( pxIPHeader->ulDestinationIPAddress == *ipLOCAL_IP_ADDRESS_POINTER )
                                {
                                    eReturn = prvProcessICMPPacket( pxICMPPacket );
                                }
                            }
                            else
                            {
                                eReturn = eReleaseBuffer;
                            }
                        #endif /* ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 ) */
                        break;

                    case ipPROTOCOL_UDP:
                       {
                           /* The IP packet contained a UDP frame. */

                           /* Map the buffer onto a UDP-Packet struct to easily access the
                            * fields of UDP packet. */
                           const UDPPacket_t * pxUDPPacket = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( UDPPacket_t, pxNetworkBuffer->pucEthernetBuffer );
                           uint16_t usLength;

                           /* Note the header values required prior to the checksum
                            * generation as the checksum pseudo header may clobber some of
                            * these values. */
                           usLength = FreeRTOS_ntohs( pxUDPPacket->xUDPHeader.usLength );

                           if( ( pxNetworkBuffer->xDataLength >= sizeof( UDPPacket_t ) ) &&
                               ( ( ( size_t ) usLength ) >= sizeof( UDPHeader_t ) ) )
                           {
                               size_t uxPayloadSize_1, uxPayloadSize_2;

                               /* Ensure that downstream UDP packet handling has the lesser
                                * of: the actual network buffer Ethernet frame length, or
                                * the sender's UDP packet header payload length, minus the
                                * size of the UDP header.
                                *
                                * The size of the UDP packet structure in this implementation
                                * includes the size of the Ethernet header, the size of
                                * the IP header, and the size of the UDP header. */
                               uxPayloadSize_1 = pxNetworkBuffer->xDataLength - sizeof( UDPPacket_t );
                               uxPayloadSize_2 = ( ( size_t ) usLength ) - sizeof( UDPHeader_t );

                               if( uxPayloadSize_1 > uxPayloadSize_2 )
                               {
                                   pxNetworkBuffer->xDataLength = uxPayloadSize_2 + sizeof( UDPPacket_t );
                               }

                               /* Fields in pxNetworkBuffer (usPort, ulIPAddress) are network order. */
                               pxNetworkBuffer->usPort = pxUDPPacket->xUDPHeader.usSourcePort;
                               pxNetworkBuffer->ulIPAddress = pxUDPPacket->xIPHeader.ulSourceIPAddress;

                               /* ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM:
                                * In some cases, the upper-layer checksum has been calculated
                                * by the NIC driver. */

                               /* Pass the packet payload to the UDP sockets
                                * implementation. */
                               if( xProcessReceivedUDPPacket( pxNetworkBuffer,
                                                              pxUDPPacket->xUDPHeader.usDestinationPort ) == pdPASS )
                               {
                                   eReturn = eFrameConsumed;
                               }
                           }
                           else
                           {
                               eReturn = eReleaseBuffer;
                           }
                       }
                       break;

                        #if ipconfigUSE_TCP == 1
                            case ipPROTOCOL_TCP:

                                if( xProcessReceivedTCPPacket( pxNetworkBuffer ) == pdPASS )
                                {
                                    eReturn = eFrameConsumed;
                                }

                                /* Setting this variable will cause xTCPTimerCheck()
                                 * to be called just before the IP-task blocks. */
                                xProcessedTCPMessage++;
                                break;
                        #endif /* if ipconfigUSE_TCP == 1 */
                    default:
                        /* Not a supported frame type. */
                        break;
                }
            }
        }
    }

    return eReturn;
}
/*-----------------------------------------------------------*/

#if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )

/**
 * @brief Process an ICMP echo reply.
 *
 * @param[in] pxICMPPacket: The IP packet that contains the ICMP message.
 */
    static void prvProcessICMPEchoReply( ICMPPacket_t * const pxICMPPacket )
    {
        ePingReplyStatus_t eStatus = eSuccess;
        uint16_t usDataLength, usCount;
        uint8_t * pucByte;

        /* Find the total length of the IP packet. */
        usDataLength = pxICMPPacket->xIPHeader.usLength;
        usDataLength = FreeRTOS_ntohs( usDataLength );

        /* Remove the length of the IP headers to obtain the length of the ICMP
         * message itself. */
        usDataLength = ( uint16_t ) ( ( ( uint32_t ) usDataLength ) - ipSIZE_OF_IPv4_HEADER );

        /* Remove the length of the ICMP header, to obtain the length of
         * data contained in the ping. */
        usDataLength = ( uint16_t ) ( ( ( uint32_t ) usDataLength ) - ipSIZE_OF_ICMP_HEADER );

        /* Checksum has already been checked before in prvProcessIPPacket */

        /* Find the first byte of the data within the ICMP packet. */
        pucByte = ( uint8_t * ) pxICMPPacket;
        pucByte = &( pucByte[ sizeof( ICMPPacket_t ) ] );

        /* Check each byte. */
        for( usCount = 0; usCount < usDataLength; usCount++ )
        {
            if( *pucByte != ( uint8_t ) ipECHO_DATA_FILL_BYTE )
            {
                eStatus = eInvalidData;
                break;
            }

            pucByte++;
        }

        /* Call back into the application to pass it the result. */
        vApplicationPingReplyHook( eStatus, pxICMPPacket->xICMPHeader.usIdentifier );
    }

#endif /* if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 ) */
/*-----------------------------------------------------------*/

#if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 )

/**
 * @brief Process an ICMP echo request.
 *
 * @param[in,out] pxICMPPacket: The IP packet that contains the ICMP message.
 */
    static eFrameProcessingResult_t prvProcessICMPEchoRequest( ICMPPacket_t * const pxICMPPacket )
    {
        ICMPHeader_t * pxICMPHeader;
        IPHeader_t * pxIPHeader;
        uint16_t usRequest;

        pxICMPHeader = &( pxICMPPacket->xICMPHeader );
        pxIPHeader = &( pxICMPPacket->xIPHeader );

        /* HT:endian: changed back */
        iptraceSENDING_PING_REPLY( pxIPHeader->ulSourceIPAddress );

        /* The checksum can be checked here - but a ping reply should be
         * returned even if the checksum is incorrect so the other end can
         * tell that the ping was received - even if the ping reply contains
         * invalid data. */
        pxICMPHeader->ucTypeOfMessage = ( uint8_t ) ipICMP_ECHO_REPLY;
        pxIPHeader->ulDestinationIPAddress = pxIPHeader->ulSourceIPAddress;
        pxIPHeader->ulSourceIPAddress = *ipLOCAL_IP_ADDRESS_POINTER;

        /* Update the checksum because the ucTypeOfMessage member in the header
         * has been changed to ipICMP_ECHO_REPLY.  This is faster than calling
         * usGenerateChecksum(). */

        /* due to compiler warning "integer operation result is out of range" */

        usRequest = ( uint16_t ) ( ( uint16_t ) ipICMP_ECHO_REQUEST << 8 );

        if( pxICMPHeader->usChecksum >= FreeRTOS_htons( 0xFFFFU - usRequest ) )
        {
            pxICMPHeader->usChecksum = pxICMPHeader->usChecksum + FreeRTOS_htons( usRequest + 1U );
        }
        else
        {
            pxICMPHeader->usChecksum = pxICMPHeader->usChecksum + FreeRTOS_htons( usRequest );
        }

        return eReturnEthernetFrame;
    }

#endif /* ipconfigREPLY_TO_INCOMING_PINGS == 1 */
/*-----------------------------------------------------------*/

#if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )

/**
 * @brief Process an ICMP packet. Only echo requests and echo replies are recognised and handled.
 *
 * @param[in,out] pxICMPPacket: The IP packet that contains the ICMP message.
 *
 * @return eReleaseBuffer when the message buffer should be released, or eReturnEthernetFrame
 *                        when the packet should be returned.
 */
    static eFrameProcessingResult_t prvProcessICMPPacket( ICMPPacket_t * const pxICMPPacket )
    {
        eFrameProcessingResult_t eReturn = eReleaseBuffer;

        iptraceICMP_PACKET_RECEIVED();

        switch( pxICMPPacket->xICMPHeader.ucTypeOfMessage )
        {
            case ipICMP_ECHO_REQUEST:
                #if ( ipconfigREPLY_TO_INCOMING_PINGS == 1 )
                    eReturn = prvProcessICMPEchoRequest( pxICMPPacket );
                #endif /* ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) */
                break;

            case ipICMP_ECHO_REPLY:
                #if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )
                    prvProcessICMPEchoReply( pxICMPPacket );
                #endif /* ipconfigSUPPORT_OUTGOING_PINGS */
                break;

            default:
                /* Only ICMP echo packets are handled. */
                break;
        }

        return eReturn;
    }

#endif /* ( ipconfigREPLY_TO_INCOMING_PINGS == 1 ) || ( ipconfigSUPPORT_OUTGOING_PINGS == 1 ) */
/*-----------------------------------------------------------*/

#if ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 1 )

/**
 * @brief Although the driver will take care of checksum calculations, the IP-task
 *        will still check if the length fields are OK.
 *
 * @param[in] pucEthernetBuffer: The Ethernet packet received.
 * @param[in] uxBufferLength: The total number of bytes received.
 *
 * @return pdPASS when the length fields in the packet OK, pdFAIL when the packet
 *         should be dropped.
 */
    static BaseType_t xCheckSizeFields( const uint8_t * const pucEthernetBuffer,
                                        size_t uxBufferLength )
    {
        size_t uxLength;
        const IPPacket_t * pxIPPacket;
        UBaseType_t uxIPHeaderLength;
        const ProtocolPacket_t * pxProtPack;
        uint8_t ucProtocol;
        uint16_t usLength;
        uint16_t ucVersionHeaderLength;
        size_t uxMinimumLength;
        BaseType_t xResult = pdFAIL;

        DEBUG_DECLARE_TRACE_VARIABLE( BaseType_t, xLocation, 0 );

        do
        {
            /* Check for minimum packet size: Ethernet header and an IP-header, 34 bytes */
            if( uxBufferLength < sizeof( IPPacket_t ) )
            {
                DEBUG_SET_TRACE_VARIABLE( xLocation, 1 );
                break;
            }

            /* Map the buffer onto a IP-Packet struct to easily access the
             * fields of the IP packet. */
            pxIPPacket = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( IPPacket_t, pucEthernetBuffer );

            ucVersionHeaderLength = pxIPPacket->xIPHeader.ucVersionHeaderLength;

            /* Test if the length of the IP-header is between 20 and 60 bytes,
             * and if the IP-version is 4. */
            if( ( ucVersionHeaderLength < ipIPV4_VERSION_HEADER_LENGTH_MIN ) ||
                ( ucVersionHeaderLength > ipIPV4_VERSION_HEADER_LENGTH_MAX ) )
            {
                DEBUG_SET_TRACE_VARIABLE( xLocation, 2 );
                break;
            }

            ucVersionHeaderLength = ( ucVersionHeaderLength & ( uint8_t ) 0x0FU ) << 2;
            uxIPHeaderLength = ( UBaseType_t ) ucVersionHeaderLength;

            /* Check if the complete IP-header is transferred. */
            if( uxBufferLength < ( ipSIZE_OF_ETH_HEADER + uxIPHeaderLength ) )
            {
                DEBUG_SET_TRACE_VARIABLE( xLocation, 3 );
                break;
            }

            /* Check if the complete IP-header plus protocol data have been transferred: */
            usLength = pxIPPacket->xIPHeader.usLength;
            usLength = FreeRTOS_ntohs( usLength );

            if( uxBufferLength < ( size_t ) ( ipSIZE_OF_ETH_HEADER + ( size_t ) usLength ) )
            {
                DEBUG_SET_TRACE_VARIABLE( xLocation, 4 );
                break;
            }

            /* Identify the next protocol. */
            ucProtocol = pxIPPacket->xIPHeader.ucProtocol;

            /* If this IP packet header includes Options, then the following
             * assignment results in a pointer into the protocol packet with the Ethernet
             * and IP headers incorrectly aligned. However, either way, the "third"
             * protocol (Layer 3 or 4) header will be aligned, which is the convenience
             * of this calculation. */

            /* Map the Buffer onto the Protocol Packet struct for easy access to the
             * struct fields. */
            pxProtPack = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( ProtocolPacket_t, &( pucEthernetBuffer[ uxIPHeaderLength - ipSIZE_OF_IPv4_HEADER ] ) );

            /* Switch on the Layer 3/4 protocol. */
            if( ucProtocol == ( uint8_t ) ipPROTOCOL_UDP )
            {
                /* Expect at least a complete UDP header. */
                uxMinimumLength = uxIPHeaderLength + ipSIZE_OF_ETH_HEADER + ipSIZE_OF_UDP_HEADER;
            }
            else if( ucProtocol == ( uint8_t ) ipPROTOCOL_TCP )
            {
                uxMinimumLength = uxIPHeaderLength + ipSIZE_OF_ETH_HEADER + ipSIZE_OF_TCP_HEADER;
            }
            else if( ( ucProtocol == ( uint8_t ) ipPROTOCOL_ICMP ) ||
                     ( ucProtocol == ( uint8_t ) ipPROTOCOL_IGMP ) )
            {
                uxMinimumLength = uxIPHeaderLength + ipSIZE_OF_ETH_HEADER + ipSIZE_OF_ICMP_HEADER;
            }
            else
            {
                /* Unhandled protocol, other than ICMP, IGMP, UDP, or TCP. */
                DEBUG_SET_TRACE_VARIABLE( xLocation, 5 );
                break;
            }

            if( uxBufferLength < uxMinimumLength )
            {
                DEBUG_SET_TRACE_VARIABLE( xLocation, 6 );
                break;
            }

            uxLength = ( size_t ) usLength;
            uxLength -= ( ( uint16_t ) uxIPHeaderLength ); /* normally, minus 20. */

            if( ( uxLength < ( ( size_t ) sizeof( pxProtPack->xUDPPacket.xUDPHeader ) ) ) ||
                ( uxLength > ( ( size_t ) ipconfigNETWORK_MTU - ( size_t ) uxIPHeaderLength ) ) )
            {
                /* For incoming packets, the length is out of bound: either
                 * too short or too long. For outgoing packets, there is a
                 * serious problem with the format/length. */
                DEBUG_SET_TRACE_VARIABLE( xLocation, 7 );
                break;
            }

            xResult = pdPASS;
        } while( ipFALSE_BOOL );

        if( xResult != pdPASS )
        {
            /* NOP if ipconfigHAS_PRINTF != 1 */
            FreeRTOS_printf( ( "xCheckSizeFields: location %ld\n", xLocation ) );
        }

        return xResult;
    }
#endif /* ( ipconfigDRIVER_INCLUDED_RX_IP_CHECKSUM == 1 ) */
/*-----------------------------------------------------------*/

/**
 * @brief Generate or check the protocol checksum of the data sent in the first parameter.
 *        At the same time, the length of the packet and the length of the different layers
 *        will be checked.
 *
 * @param[in] pucEthernetBuffer: The Ethernet buffer for which the checksum is to be calculated
 *                               or checked.
 * @param[in] uxBufferLength: the total number of bytes received, or the number of bytes written
 *                            in the packet buffer.
 * @param[in] xOutgoingPacket: Whether this is an outgoing packet or not.
 *
 * @return When xOutgoingPacket is false: the error code can be either: ipINVALID_LENGTH,
 *         ipUNHANDLED_PROTOCOL, ipWRONG_CRC, or ipCORRECT_CRC.
 *         When xOutgoingPacket is true: either ipINVALID_LENGTH or ipCORRECT_CRC.
 */
uint16_t usGenerateProtocolChecksum( const uint8_t * const pucEthernetBuffer,
                                     size_t uxBufferLength,
                                     BaseType_t xOutgoingPacket )
{
    uint32_t ulLength;
    uint16_t usChecksum, * pusChecksum;
    const IPPacket_t * pxIPPacket;
    UBaseType_t uxIPHeaderLength;
    const ProtocolPacket_t * pxProtPack;
    uint8_t ucProtocol;

    #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
        const char * pcType;
    #endif
    uint16_t usLength;
    uint16_t ucVersionHeaderLength;
    DEBUG_DECLARE_TRACE_VARIABLE( BaseType_t, xLocation, 0 );

    /* Introduce a do-while loop to allow use of break statements.
     * Note: MISRA prohibits use of 'goto', thus replaced with breaks. */
    do
    {
        /* Check for minimum packet size. */
        if( uxBufferLength < sizeof( IPPacket_t ) )
        {
            usChecksum = ipINVALID_LENGTH;
            DEBUG_SET_TRACE_VARIABLE( xLocation, 1 );
            break;
        }

        /* Parse the packet length. */
        pxIPPacket = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( IPPacket_t, pucEthernetBuffer );

        /* Per https://tools.ietf.org/html/rfc791, the four-bit Internet Header
         * Length field contains the length of the internet header in 32-bit words. */
        ucVersionHeaderLength = pxIPPacket->xIPHeader.ucVersionHeaderLength;
        ucVersionHeaderLength = ( ucVersionHeaderLength & ( uint8_t ) 0x0FU ) << 2;
        uxIPHeaderLength = ( UBaseType_t ) ucVersionHeaderLength;

        /* Check for minimum packet size. */
        if( uxBufferLength < ( sizeof( IPPacket_t ) + ( uxIPHeaderLength - ipSIZE_OF_IPv4_HEADER ) ) )
        {
            usChecksum = ipINVALID_LENGTH;
            DEBUG_SET_TRACE_VARIABLE( xLocation, 2 );
            break;
        }

        usLength = pxIPPacket->xIPHeader.usLength;
        usLength = FreeRTOS_ntohs( usLength );

        if( uxBufferLength < ( size_t ) ( ipSIZE_OF_ETH_HEADER + ( size_t ) usLength ) )
        {
            usChecksum = ipINVALID_LENGTH;
            DEBUG_SET_TRACE_VARIABLE( xLocation, 3 );
            break;
        }

        /* Identify the next protocol. */
        ucProtocol = pxIPPacket->xIPHeader.ucProtocol;

        /* N.B., if this IP packet header includes Options, then the following
         * assignment results in a pointer into the protocol packet with the Ethernet
         * and IP headers incorrectly aligned. However, either way, the "third"
         * protocol (Layer 3 or 4) header will be aligned, which is the convenience
         * of this calculation. */
        pxProtPack = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( ProtocolPacket_t, &( pucEthernetBuffer[ uxIPHeaderLength - ipSIZE_OF_IPv4_HEADER ] ) );

        /* Switch on the Layer 3/4 protocol. */
        if( ucProtocol == ( uint8_t ) ipPROTOCOL_UDP )
        {
            if( uxBufferLength < ( uxIPHeaderLength + ipSIZE_OF_ETH_HEADER + ipSIZE_OF_UDP_HEADER ) )
            {
                usChecksum = ipINVALID_LENGTH;
                DEBUG_SET_TRACE_VARIABLE( xLocation, 4 );
                break;
            }

            pusChecksum = ( uint16_t * ) ( &( pxProtPack->xUDPPacket.xUDPHeader.usChecksum ) );
            #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                {
                    pcType = "UDP";
                }
            #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */
        }
        else if( ucProtocol == ( uint8_t ) ipPROTOCOL_TCP )
        {
            if( uxBufferLength < ( uxIPHeaderLength + ipSIZE_OF_ETH_HEADER + ipSIZE_OF_TCP_HEADER ) )
            {
                usChecksum = ipINVALID_LENGTH;
                DEBUG_SET_TRACE_VARIABLE( xLocation, 5 );
                break;
            }

            pusChecksum = ( uint16_t * ) ( &( pxProtPack->xTCPPacket.xTCPHeader.usChecksum ) );
            #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                {
                    pcType = "TCP";
                }
            #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */
        }
        else if( ( ucProtocol == ( uint8_t ) ipPROTOCOL_ICMP ) ||
                 ( ucProtocol == ( uint8_t ) ipPROTOCOL_IGMP ) )
        {
            if( uxBufferLength < ( uxIPHeaderLength + ipSIZE_OF_ETH_HEADER + ipSIZE_OF_ICMP_HEADER ) )
            {
                usChecksum = ipINVALID_LENGTH;
                DEBUG_SET_TRACE_VARIABLE( xLocation, 6 );
                break;
            }

            pusChecksum = ( uint16_t * ) ( &( pxProtPack->xICMPPacket.xICMPHeader.usChecksum ) );
            #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                {
                    if( ucProtocol == ( uint8_t ) ipPROTOCOL_ICMP )
                    {
                        pcType = "ICMP";
                    }
                    else
                    {
                        pcType = "IGMP";
                    }
                }
            #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */
        }
        else
        {
            /* Unhandled protocol, other than ICMP, IGMP, UDP, or TCP. */
            usChecksum = ipUNHANDLED_PROTOCOL;
            DEBUG_SET_TRACE_VARIABLE( xLocation, 7 );
            break;
        }

        /* The protocol and checksum field have been identified. Check the direction
         * of the packet. */
        if( xOutgoingPacket != pdFALSE )
        {
            /* This is an outgoing packet. Before calculating the checksum, set it
             * to zero. */
            *( pusChecksum ) = 0U;
        }
        else if( ( *pusChecksum == 0U ) && ( ucProtocol == ( uint8_t ) ipPROTOCOL_UDP ) )
        {
            #if ( ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS == 0 )
                {
                    /* Sender hasn't set the checksum, drop the packet because
                     * ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS is not set. */
                    usChecksum = ipWRONG_CRC;
                    #if ( ipconfigHAS_PRINTF != 0 )
                        {
                            static BaseType_t xCount = 0;

                            if( xCount < 5 )
                            {
                                FreeRTOS_printf( ( "usGenerateProtocolChecksum: UDP packet from %xip without CRC dropped\n",
                                                   FreeRTOS_ntohl( pxIPPacket->xIPHeader.ulSourceIPAddress ) ) );
                                xCount++;
                            }
                        }
                    #endif /* ( ipconfigHAS_PRINTF != 0 ) */
                }
            #else /* if ( ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS == 0 ) */
                {
                    /* Sender hasn't set the checksum, no use to calculate it. */
                    usChecksum = ipCORRECT_CRC;
                }
            #endif /* if ( ipconfigUDP_PASS_ZERO_CHECKSUM_PACKETS == 0 ) */
            DEBUG_SET_TRACE_VARIABLE( xLocation, 8 );
            break;
        }
        else
        {
            /* Other incoming packet than UDP. */
        }

        usLength = pxIPPacket->xIPHeader.usLength;
        usLength = FreeRTOS_ntohs( usLength );
        ulLength = ( uint32_t ) usLength;
        ulLength -= ( ( uint16_t ) uxIPHeaderLength ); /* normally minus 20 */

        if( ( ulLength < ( ( uint32_t ) sizeof( pxProtPack->xUDPPacket.xUDPHeader ) ) ) ||
            ( ulLength > ( ( uint32_t ) ipconfigNETWORK_MTU - ( uint32_t ) uxIPHeaderLength ) ) )
        {
            #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                {
                    FreeRTOS_debug_printf( ( "usGenerateProtocolChecksum[%s]: len invalid: %lu\n", pcType, ulLength ) );
                }
            #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */

            /* Again, in a 16-bit return value there is no space to indicate an
             * error.  For incoming packets, 0x1234 will cause dropping of the packet.
             * For outgoing packets, there is a serious problem with the
             * format/length */
            usChecksum = ipINVALID_LENGTH;
            DEBUG_SET_TRACE_VARIABLE( xLocation, 9 );
            break;
        }

        if( ucProtocol <= ( uint8_t ) ipPROTOCOL_IGMP )
        {
            /* ICMP/IGMP do not have a pseudo header for CRC-calculation. */
            usChecksum = ( uint16_t )
                         ( ~usGenerateChecksum( 0U,
                                                ( const uint8_t * ) &( pxProtPack->xTCPPacket.xTCPHeader ), ( size_t ) ulLength ) );
        }
        else
        {
            /* For UDP and TCP, sum the pseudo header, i.e. IP protocol + length
             * fields */
            usChecksum = ( uint16_t ) ( ulLength + ( ( uint16_t ) ucProtocol ) );

            /* And then continue at the IPv4 source and destination addresses. */
            usChecksum = ( uint16_t )
                         ( ~usGenerateChecksum( usChecksum,
                                                ipPOINTER_CAST( const uint8_t *, &( pxIPPacket->xIPHeader.ulSourceIPAddress ) ),
                                                ( size_t ) ( ( 2U * ipSIZE_OF_IPv4_ADDRESS ) + ulLength ) ) );
            /* Sum TCP header and data. */
        }

        if( xOutgoingPacket == pdFALSE )
        {
            /* This is in incoming packet. If the CRC is correct, it should be zero. */
            if( usChecksum == 0U )
            {
                usChecksum = ( uint16_t ) ipCORRECT_CRC;
            }
        }
        else
        {
            if( ( usChecksum == 0U ) && ( ucProtocol == ( uint8_t ) ipPROTOCOL_UDP ) )
            {
                /* In case of UDP, a calculated checksum of 0x0000 is transmitted
                 * as 0xffff. A value of zero would mean that the checksum is not used. */
                #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
                    {
                        if( xOutgoingPacket != pdFALSE )
                        {
                            FreeRTOS_debug_printf( ( "usGenerateProtocolChecksum[%s]: crc swap: %04X\n", pcType, usChecksum ) );
                        }
                    }
                #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */

                usChecksum = ( uint16_t ) 0xffffu;
            }
        }

        usChecksum = FreeRTOS_htons( usChecksum );

        if( xOutgoingPacket != pdFALSE )
        {
            *( pusChecksum ) = usChecksum;
        }

        #if ( ipconfigHAS_DEBUG_PRINTF != 0 )
            else if( ( xOutgoingPacket == pdFALSE ) && ( usChecksum != ipCORRECT_CRC ) )
            {
                FreeRTOS_debug_printf( ( "usGenerateProtocolChecksum[%s]: ID %04X: from %lxip to %lxip bad crc: %04X\n",
                                         pcType,
                                         FreeRTOS_ntohs( pxIPPacket->xIPHeader.usIdentification ),
                                         FreeRTOS_ntohl( pxIPPacket->xIPHeader.ulSourceIPAddress ),
                                         FreeRTOS_ntohl( pxIPPacket->xIPHeader.ulDestinationIPAddress ),
                                         FreeRTOS_ntohs( *pusChecksum ) ) );
            }
            else
            {
                /* Nothing. */
            }
        #endif /* ipconfigHAS_DEBUG_PRINTF != 0 */
    } while( ipFALSE_BOOL );

    if( ( usChecksum == ipUNHANDLED_PROTOCOL ) ||
        ( usChecksum == ipINVALID_LENGTH ) )
    {
        /* NOP if ipconfigHAS_PRINTF != 0 */
        FreeRTOS_printf( ( "CRC error: %04x location %ld\n", usChecksum, xLocation ) );
    }

    return usChecksum;
}
/*-----------------------------------------------------------*/

/**
 * This method generates a checksum for a given IPv4 header, per RFC791 (page 14).
 * The checksum algorithm is described as:
 *   "[T]he 16 bit one's complement of the one's complement sum of all 16 bit words in the
 *   header.  For purposes of computing the checksum, the value of the checksum field is zero."
 *
 * In a nutshell, that means that each 16-bit 'word' must be summed, after which
 * the number of 'carries' (overflows) is added to the result. If that addition
 * produces an overflow, that 'carry' must also be added to the final result. The final checksum
 * should be the bitwise 'not' (ones-complement) of the result if the packet is
 * meant to be transmitted, but this method simply returns the raw value, probably
 * because when a packet is received, the checksum is verified by checking that
 * ((received & calculated) == 0) without applying a bitwise 'not' to the 'calculated' checksum.
 *
 * This logic is optimized for microcontrollers which have limited resources, so the logic looks odd.
 * It iterates over the full range of 16-bit words, but it does so by processing several 32-bit
 * words at once whenever possible. Its first step is to align the memory pointer to a 32-bit boundary,
 * after which it runs a fast loop to process multiple 32-bit words at once and adding their 'carries'.
 * Finally, it finishes up by processing any remaining 16-bit words, and adding up all of the 'carries'.
 * With 32-bit arithmetic, the number of 16-bit 'carries' produced by sequential additions can be found
 * by looking at the 16 most-significant bits of the 32-bit integer, since a 32-bit int will continue
 * counting up instead of overflowing after 16 bits. That is why the actual checksum calculations look like:
 *   union.u32 = ( uint32_t ) union.u16[ 0 ] + union.u16[ 1 ];
 *
 * Arguments:
 *   ulSum: This argument provides a value to initialise the progressive summation
 *   of the header's values to. It is often 0, but protocols like TCP or UDP
 *   can have pseudo-header fields which need to be included in the checksum.
 *   pucNextData: This argument contains the address of the first byte which this
 *   method should process. The method's memory iterator is initialised to this value.
 *   uxDataLengthBytes: This argument contains the number of bytes that this method
 *   should process.
 */

/**
 * @brief Calculates the 16-bit checksum of an array of bytes
 *
 * @param[in] usSum: The initial sum, obtained from earlier data.
 * @param[in] pucNextData: The actual data.
 * @param[in] uxByteCount: The number of bytes.
 *
 * @return The 16-bit one's complement of the one's complement sum of all 16-bit
 *         words in the header
 */
uint16_t usGenerateChecksum( uint16_t usSum,
                             const uint8_t * pucNextData,
                             size_t uxByteCount )
{
/* MISRA/PC-lint doesn't like the use of unions. Here, they are a great
 * aid though to optimise the calculations. */
    xUnion32 xSum2, xSum, xTerm;
    xUnionPtr xSource;
    xUnionPtr xLastSource;
    uintptr_t uxAlignBits;
    uint32_t ulCarry = 0UL;
    uint16_t usTemp;
    size_t uxDataLengthBytes = uxByteCount;

    /* Small MCUs often spend up to 30% of the time doing checksum calculations
    * This function is optimised for 32-bit CPUs; Each time it will try to fetch
    * 32-bits, sums it with an accumulator and counts the number of carries. */

    /* Swap the input (little endian platform only). */
    usTemp = FreeRTOS_ntohs( usSum );
    xSum.u32 = ( uint32_t ) usTemp;
    xTerm.u32 = 0UL;

    xSource.u8ptr = ipPOINTER_CAST( uint8_t *, pucNextData );
    uxAlignBits = ( ( ( uintptr_t ) pucNextData ) & 0x03U );

    /*
     * If pucNextData is non-aligned then the checksum is starting at an
     * odd position and we need to make sure the usSum value now in xSum is
     * as if it had been "aligned" in the same way.
     */
    if( ( uxAlignBits & 1UL ) != 0U )
    {
        xSum.u32 = ( ( xSum.u32 & 0xffU ) << 8 ) | ( ( xSum.u32 & 0xff00U ) >> 8 );
    }

    /* If byte (8-bit) aligned... */
    if( ( ( uxAlignBits & 1UL ) != 0UL ) && ( uxDataLengthBytes >= ( size_t ) 1 ) )
    {
        xTerm.u8[ 1 ] = *( xSource.u8ptr );
        xSource.u8ptr++;
        uxDataLengthBytes--;
        /* Now xSource is word (16-bit) aligned. */
    }

    /* If half-word (16-bit) aligned... */
    if( ( ( uxAlignBits == 1U ) || ( uxAlignBits == 2U ) ) && ( uxDataLengthBytes >= 2U ) )
    {
        xSum.u32 += *( xSource.u16ptr );
        xSource.u16ptr++;
        uxDataLengthBytes -= 2U;
        /* Now xSource is word (32-bit) aligned. */
    }

    /* Word (32-bit) aligned, do the most part. */
    xLastSource.u32ptr = ( xSource.u32ptr + ( uxDataLengthBytes / 4U ) ) - 3U;

    /* In this loop, four 32-bit additions will be done, in total 16 bytes.
     * Indexing with constants (0,1,2,3) gives faster code than using
     * post-increments. */
    while( xSource.u32ptr < xLastSource.u32ptr )
    {
        /* Use a secondary Sum2, just to see if the addition produced an
         * overflow. */
        xSum2.u32 = xSum.u32 + xSource.u32ptr[ 0 ];

        if( xSum2.u32 < xSum.u32 )
        {
            ulCarry++;
        }

        /* Now add the secondary sum to the major sum, and remember if there was
         * a carry. */
        xSum.u32 = xSum2.u32 + xSource.u32ptr[ 1 ];

        if( xSum2.u32 > xSum.u32 )
        {
            ulCarry++;
        }

        /* And do the same trick once again for indexes 2 and 3 */
        xSum2.u32 = xSum.u32 + xSource.u32ptr[ 2 ];

        if( xSum2.u32 < xSum.u32 )
        {
            ulCarry++;
        }

        xSum.u32 = xSum2.u32 + xSource.u32ptr[ 3 ];

        if( xSum2.u32 > xSum.u32 )
        {
            ulCarry++;
        }

        /* And finally advance the pointer 4 * 4 = 16 bytes. */
        xSource.u32ptr = &( xSource.u32ptr[ 4 ] );
    }

    /* Now add all carries. */
    xSum.u32 = ( uint32_t ) xSum.u16[ 0 ] + xSum.u16[ 1 ] + ulCarry;

    uxDataLengthBytes %= 16U;
    xLastSource.u8ptr = ( uint8_t * ) ( xSource.u8ptr + ( uxDataLengthBytes & ~( ( size_t ) 1 ) ) );

    /* Half-word aligned. */

    /* Coverity does not like Unions. Warning issued here: "The operator "<"
     * is being applied to the pointers "xSource.u16ptr" and "xLastSource.u16ptr",
     * which do not point into the same object." */
    while( xSource.u16ptr < xLastSource.u16ptr )
    {
        /* At least one more short. */
        xSum.u32 += xSource.u16ptr[ 0 ];
        xSource.u16ptr++;
    }

    if( ( uxDataLengthBytes & ( size_t ) 1 ) != 0U ) /* Maybe one more ? */
    {
        xTerm.u8[ 0 ] = xSource.u8ptr[ 0 ];
    }

    xSum.u32 += xTerm.u32;

    /* Now add all carries again. */

    /* Assigning value from "xTerm.u32" to "xSum.u32" here, but that stored value is overwritten before it can be used.
     * Coverity doesn't understand about union variables. */
    xSum.u32 = ( uint32_t ) xSum.u16[ 0 ] + xSum.u16[ 1 ];

    /* coverity[value_overwrite] */
    xSum.u32 = ( uint32_t ) xSum.u16[ 0 ] + xSum.u16[ 1 ];

    if( ( uxAlignBits & 1U ) != 0U )
    {
        /* Quite unlikely, but pucNextData might be non-aligned, which would
        * mean that a checksum is calculated starting at an odd position. */
        xSum.u32 = ( ( xSum.u32 & 0xffU ) << 8 ) | ( ( xSum.u32 & 0xff00U ) >> 8 );
    }

    /* swap the output (little endian platform only). */
    return FreeRTOS_htons( ( ( uint16_t ) xSum.u32 ) );
}
/*-----------------------------------------------------------*/

/* This function is used in other files, has external linkage e.g. in
 * FreeRTOS_DNS.c. Not to be made static. */

/**
 * @brief Send the Ethernet frame after checking for some conditions.
 *
 * @param[in,out] pxNetworkBuffer: The network buffer which is to be sent.
 * @param[in] xReleaseAfterSend: Whether this network buffer is to be released or not.
 */
void vReturnEthernetFrame( NetworkBufferDescriptor_t * pxNetworkBuffer,
                           BaseType_t xReleaseAfterSend )
{
    EthernetHeader_t * pxEthernetHeader;
/* memcpy() helper variables for MISRA Rule 21.15 compliance*/
    const void * pvCopySource;
    void * pvCopyDest;

    #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
        NetworkBufferDescriptor_t * pxNewBuffer;
    #endif

    #if defined( ipconfigETHERNET_MINIMUM_PACKET_BYTES )
        {
            if( pxNetworkBuffer->xDataLength < ( size_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES )
            {
                BaseType_t xIndex;

                FreeRTOS_printf( ( "vReturnEthernetFrame: length %u\n", ( unsigned ) pxNetworkBuffer->xDataLength ) );

                for( xIndex = ( BaseType_t ) pxNetworkBuffer->xDataLength; xIndex < ( BaseType_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES; xIndex++ )
                {
                    pxNetworkBuffer->pucEthernetBuffer[ xIndex ] = 0U;
                }

                pxNetworkBuffer->xDataLength = ( size_t ) ipconfigETHERNET_MINIMUM_PACKET_BYTES;
            }
        }
    #endif /* if defined( ipconfigETHERNET_MINIMUM_PACKET_BYTES ) */

    #if ( ipconfigZERO_COPY_TX_DRIVER != 0 )
        if( xReleaseAfterSend == pdFALSE )
        {
            pxNewBuffer = pxDuplicateNetworkBufferWithDescriptor( pxNetworkBuffer, pxNetworkBuffer->xDataLength );

            if( pxNewBuffer != NULL )
            {
                xReleaseAfterSend = pdTRUE;
                /* Want no rounding up. */
                pxNewBuffer->xDataLength = pxNetworkBuffer->xDataLength;
            }

            pxNetworkBuffer = pxNewBuffer;
        }

        if( pxNetworkBuffer != NULL )
    #endif /* if ( ipconfigZERO_COPY_TX_DRIVER != 0 ) */
    {
        /* Map the Buffer to Ethernet Header struct for easy access to fields. */
        pxEthernetHeader = ipCAST_PTR_TO_TYPE_PTR( EthernetHeader_t, pxNetworkBuffer->pucEthernetBuffer );

        /*
         * Use helper variables for memcpy() to remain
         * compliant with MISRA Rule 21.15.  These should be
         * optimized away.
         */
        /* Swap source and destination MAC addresses. */
        pvCopySource = &pxEthernetHeader->xSourceAddress;
        pvCopyDest = &pxEthernetHeader->xDestinationAddress;
        ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( pxEthernetHeader->xDestinationAddress ) );

        pvCopySource = ipLOCAL_MAC_ADDRESS;
        pvCopyDest = &pxEthernetHeader->xSourceAddress;
        ( void ) memcpy( pvCopyDest, pvCopySource, ( size_t ) ipMAC_ADDRESS_LENGTH_BYTES );

        /* Send! */
        iptraceNETWORK_INTERFACE_OUTPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );
        ( void ) xNetworkInterfaceOutput( pxNetworkBuffer, xReleaseAfterSend );
    }
}
/*-----------------------------------------------------------*/


#if ( ipconfigHAS_PRINTF != 0 )

    #ifndef ipMONITOR_MAX_HEAP

/* As long as the heap has more space than e.g. 1 MB, there
 * will be no messages. */
        #define ipMONITOR_MAX_HEAP    ( 1024U * 1024U )
    #endif /* ipMONITOR_MAX_HEAP */

    #ifndef ipMONITOR_PERCENTAGE_90
        /* Make this number lower to get less logging messages. */
        #define ipMONITOR_PERCENTAGE_90    ( 90U )
    #endif

    #define ipMONITOR_PERCENTAGE_100       ( 100U )

/**
 * @brief A function that monitors a three resources: the heap, the space in the message
 *        queue of the IP-task, the number of available network buffer descriptors.
 */
    void vPrintResourceStats( void )
    {
        static UBaseType_t uxLastMinBufferCount = ipconfigNUM_NETWORK_BUFFER_DESCRIPTORS;
        static size_t uxMinLastSize = 0u;
        UBaseType_t uxCurrentBufferCount;
        size_t uxMinSize;

        /* When setting up and testing a project with FreeRTOS+TCP, it is
         * can be helpful to monitor a few resources: the number of network
         * buffers and the amount of available heap.
         * This function will issue some logging when a minimum value has
         * changed. */
        uxCurrentBufferCount = uxGetMinimumFreeNetworkBuffers();

        if( uxLastMinBufferCount > uxCurrentBufferCount )
        {
            /* The logging produced below may be helpful
             * while tuning +TCP: see how many buffers are in use. */
            uxLastMinBufferCount = uxCurrentBufferCount;
            FreeRTOS_printf( ( "Network buffers: %lu lowest %lu\n",
                               uxGetNumberOfFreeNetworkBuffers(),
                               uxCurrentBufferCount ) );
        }

        uxMinSize = xPortGetMinimumEverFreeHeapSize();

        if( uxMinLastSize == 0U )
        {
            /* Probably the first time this function is called. */
            uxMinLastSize = uxMinSize;
        }
        else if( uxMinSize >= ipMONITOR_MAX_HEAP )
        {
            /* There is more than enough heap space. No need for logging. */
        }
        /* Write logging if there is a 10% decrease since the last time logging was written. */
        else if( ( uxMinLastSize * ipMONITOR_PERCENTAGE_90 ) > ( uxMinSize * ipMONITOR_PERCENTAGE_100 ) )
        {
            uxMinLastSize = uxMinSize;
            FreeRTOS_printf( ( "Heap: current %lu lowest %lu\n", xPortGetFreeHeapSize(), uxMinSize ) );
        }
        else
        {
            /* Nothing to log. */
        }

        #if ( ipconfigCHECK_IP_QUEUE_SPACE != 0 )
            {
                static UBaseType_t uxLastMinQueueSpace = 0;
                UBaseType_t uxCurrentCount = 0u;

                uxCurrentCount = uxGetMinimumIPQueueSpace();

                if( uxLastMinQueueSpace != uxCurrentCount )
                {
                    /* The logging produced below may be helpful
                     * while tuning +TCP: see how many buffers are in use. */
                    uxLastMinQueueSpace = uxCurrentCount;
                    FreeRTOS_printf( ( "Queue space: lowest %lu\n", uxCurrentCount ) );
                }
            }
        #endif /* ipconfigCHECK_IP_QUEUE_SPACE */
    }
#endif /* ( ipconfigHAS_PRINTF != 0 ) */
/*-----------------------------------------------------------*/

/**
 * @brief Returns the IP address of the NIC.
 *
 * @return The IP address of the NIC.
 */
uint32_t FreeRTOS_GetIPAddress( void )
{
    return *ipLOCAL_IP_ADDRESS_POINTER;
}
/*-----------------------------------------------------------*/

/**
 * @brief Sets the IP address of the NIC.
 *
 * @param[in] ulIPAddress: IP address of the NIC to be set.
 */
void FreeRTOS_SetIPAddress( uint32_t ulIPAddress )
{
    *ipLOCAL_IP_ADDRESS_POINTER = ulIPAddress;
}
/*-----------------------------------------------------------*/

/**
 * @brief Get the gateway address of the subnet.
 *
 * @return The IP-address of the gateway, zero if a gateway is
 *         not used/defined.
 */
uint32_t FreeRTOS_GetGatewayAddress( void )
{
    return xNetworkAddressing.ulGatewayAddress;
}
/*-----------------------------------------------------------*/

/**
 * @brief Get the DNS server address.
 *
 * @return The IP address of the DNS server.
 */
uint32_t FreeRTOS_GetDNSServerAddress( void )
{
    return xNetworkAddressing.ulDNSServerAddress;
}
/*-----------------------------------------------------------*/

/**
 * @brief Get the netmask for the subnet.
 *
 * @return The 32 bit netmask for the subnet.
 */
uint32_t FreeRTOS_GetNetmask( void )
{
    return xNetworkAddressing.ulNetMask;
}
/*-----------------------------------------------------------*/

/**
 * @brief Update the MAC address.
 *
 * @param[in] ucMACAddress: the MAC address to be set.
 */
void FreeRTOS_UpdateMACAddress( const uint8_t ucMACAddress[ ipMAC_ADDRESS_LENGTH_BYTES ] )
{
    /* Copy the MAC address at the start of the default packet header fragment. */
    ( void ) memcpy( ipLOCAL_MAC_ADDRESS, ucMACAddress, ( size_t ) ipMAC_ADDRESS_LENGTH_BYTES );
}
/*-----------------------------------------------------------*/

/**
 * @brief Get the MAC address.
 *
 * @return The pointer to MAC address.
 */
const uint8_t * FreeRTOS_GetMACAddress( void )
{
    return ipLOCAL_MAC_ADDRESS;
}
/*-----------------------------------------------------------*/

/**
 * @brief Set the netmask for the subnet.
 *
 * @param[in] ulNetmask: The 32 bit netmask of the subnet.
 */
void FreeRTOS_SetNetmask( uint32_t ulNetmask )
{
    xNetworkAddressing.ulNetMask = ulNetmask;
}
/*-----------------------------------------------------------*/

/**
 * @brief Set the gateway address.
 *
 * @param[in] ulGatewayAddress: The gateway address.
 */
void FreeRTOS_SetGatewayAddress( uint32_t ulGatewayAddress )
{
    xNetworkAddressing.ulGatewayAddress = ulGatewayAddress;
}
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_DHCP == 1 )

/**
 * @brief Enable/disable the DHCP timer.
 *
 * @param[in] xEnableState: pdTRUE - enable timer; pdFALSE - disable timer.
 */
    void vIPSetDHCPTimerEnableState( BaseType_t xEnableState )
    {
        if( xEnableState != pdFALSE )
        {
            xDHCPTimer.bActive = pdTRUE_UNSIGNED;
        }
        else
        {
            xDHCPTimer.bActive = pdFALSE_UNSIGNED;
        }
    }
#endif /* ipconfigUSE_DHCP */
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_DHCP == 1 )

/**
 * @brief Reload the DHCP timer.
 *
 * @param[in] ulLeaseTime: The reload value.
 */
    void vIPReloadDHCPTimer( uint32_t ulLeaseTime )
    {
        prvIPTimerReload( &xDHCPTimer, ulLeaseTime );
    }
#endif /* ipconfigUSE_DHCP */
/*-----------------------------------------------------------*/

#if ( ipconfigDNS_USE_CALLBACKS == 1 )

/**
 * @brief Enable/disable the DNS timer.
 *
 * @param[in] xEnableState: pdTRUE - enable timer; pdFALSE - disable timer.
 */
    void vIPSetDnsTimerEnableState( BaseType_t xEnableState )
    {
        if( xEnableState != 0 )
        {
            xDNSTimer.bActive = pdTRUE;
        }
        else
        {
            xDNSTimer.bActive = pdFALSE;
        }
    }

#endif /* ipconfigUSE_DHCP */
/*-----------------------------------------------------------*/

#if ( ipconfigDNS_USE_CALLBACKS != 0 )

/**
 * @brief Reload the DNS timer.
 *
 * @param[in] ulCheckTime: The reload value.
 */
    void vIPReloadDNSTimer( uint32_t ulCheckTime )
    {
        prvIPTimerReload( &xDNSTimer, ulCheckTime );
    }
#endif /* ipconfigDNS_USE_CALLBACKS != 0 */
/*-----------------------------------------------------------*/

/**
 * @brief Returns whether the IP task is ready.
 *
 * @return pdTRUE if IP task is ready, else pdFALSE.
 */
BaseType_t xIPIsNetworkTaskReady( void )
{
    return xIPTaskInitialised;
}
/*-----------------------------------------------------------*/

/**
 * @brief Returns whether this node is connected to network or not.
 *
 * @return pdTRUE if network is connected, else pdFALSE.
 */
BaseType_t FreeRTOS_IsNetworkUp( void )
{
    return xNetworkUp;
}
/*-----------------------------------------------------------*/

#if ( ipconfigCHECK_IP_QUEUE_SPACE != 0 )

/**
 * @brief Get the minimum space in the IP task queue.
 *
 * @return The minimum possible space in the IP task queue.
 */
    UBaseType_t uxGetMinimumIPQueueSpace( void )
    {
        return uxQueueMinimumSpace;
    }
#endif
/*-----------------------------------------------------------*/

/**
 * @brief Utility function: Convert error number to a human readable
 *        string. Declaration in FreeRTOS_errno_TCP.h.
 *
 * @param[in] xErrnum: The error number.
 * @param[in] pcBuffer: Buffer big enough to be filled with the human readable message.
 * @param[in] uxLength: Maximum length of the buffer.
 *
 * @return The buffer filled with human readable error string.
 */
const char * FreeRTOS_strerror_r( BaseType_t xErrnum,
                                  char * pcBuffer,
                                  size_t uxLength )
{
    const char * pcName;

    switch( xErrnum )
    {
        case pdFREERTOS_ERRNO_EADDRINUSE:
            pcName = "EADDRINUSE";
            break;

        case pdFREERTOS_ERRNO_ENOMEM:
            pcName = "ENOMEM";
            break;

        case pdFREERTOS_ERRNO_EADDRNOTAVAIL:
            pcName = "EADDRNOTAVAIL";
            break;

        case pdFREERTOS_ERRNO_ENOPROTOOPT:
            pcName = "ENOPROTOOPT";
            break;

        case pdFREERTOS_ERRNO_EBADF:
            pcName = "EBADF";
            break;

        case pdFREERTOS_ERRNO_ENOSPC:
            pcName = "ENOSPC";
            break;

        case pdFREERTOS_ERRNO_ECANCELED:
            pcName = "ECANCELED";
            break;

        case pdFREERTOS_ERRNO_ENOTCONN:
            pcName = "ENOTCONN";
            break;

        case pdFREERTOS_ERRNO_EINPROGRESS:
            pcName = "EINPROGRESS";
            break;

        case pdFREERTOS_ERRNO_EOPNOTSUPP:
            pcName = "EOPNOTSUPP";
            break;

        case pdFREERTOS_ERRNO_EINTR:
            pcName = "EINTR";
            break;

        case pdFREERTOS_ERRNO_ETIMEDOUT:
            pcName = "ETIMEDOUT";
            break;

        case pdFREERTOS_ERRNO_EINVAL:
            pcName = "EINVAL";
            break;

        case pdFREERTOS_ERRNO_EWOULDBLOCK:
            pcName = "EWOULDBLOCK";
            break; /* same as EAGAIN */

        case pdFREERTOS_ERRNO_EISCONN:
            pcName = "EISCONN";
            break;

        default:
            /* Using function "snprintf". */
            ( void ) snprintf( pcBuffer, uxLength, "Errno %d", ( int32_t ) xErrnum );
            pcName = NULL;
            break;
    }

    if( pcName != NULL )
    {
        /* Using function "snprintf". */
        ( void ) snprintf( pcBuffer, uxLength, "%s", pcName );
    }

    if( uxLength > 0U )
    {
        pcBuffer[ uxLength - 1U ] = '\0';
    }

    return pcBuffer;
}
/*-----------------------------------------------------------*/

/* Provide access to private members for verification. */
#ifdef FREERTOS_TCP_ENABLE_VERIFICATION
    #include "aws_freertos_ip_verification_access_ip_define.h"
#endif
