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
 * @file FreeRTOS_UDP_IP.c
 * @brief This file has the source code for the UDP-IP functionality of the FreeRTOS+TCP
 *        network stack.
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
#include "FreeRTOS_ARP.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "NetworkBufferManagement.h"

#if ( ipconfigUSE_DNS == 1 )
    #include "FreeRTOS_DNS.h"
#endif

/** @brief The expected IP version and header length coded into the IP header itself. */
#define ipIP_VERSION_AND_HEADER_LENGTH_BYTE    ( ( uint8_t ) 0x45 )

/** @brief Part of the Ethernet and IP headers are always constant when sending an IPv4
 * UDP packet.  This array defines the constant parts, allowing this part of the
 * packet to be filled in using a simple memcpy() instead of individual writes. */
/*lint -e708 (Info -- union initialization). */
UDPPacketHeader_t xDefaultPartUDPPacketHeader =
{
    /* .ucBytes : */
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* Ethernet source MAC address. */
        0x08, 0x00,                          /* Ethernet frame type. */
        ipIP_VERSION_AND_HEADER_LENGTH_BYTE, /* ucVersionHeaderLength. */
        0x00,                                /* ucDifferentiatedServicesCode. */
        0x00, 0x00,                          /* usLength. */
        0x00, 0x00,                          /* usIdentification. */
        0x00, 0x00,                          /* usFragmentOffset. */
        ipconfigUDP_TIME_TO_LIVE,            /* ucTimeToLive */
        ipPROTOCOL_UDP,                      /* ucProtocol. */
        0x00, 0x00,                          /* usHeaderChecksum. */
        0x00, 0x00, 0x00, 0x00               /* Source IP address. */
    }
};
/*-----------------------------------------------------------*/

/**
 * @brief Process the generated UDP packet and do other checks before sending the
 *        packet such as ARP cache check and address resolution.
 *
 * @param[in] pxNetworkBuffer: The network buffer carrying the packet.
 */
void vProcessGeneratedUDPPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
    UDPPacket_t * pxUDPPacket;
    IPHeader_t * pxIPHeader;
    eARPLookupResult_t eReturned;
    uint32_t ulIPAddress = pxNetworkBuffer->ulIPAddress;
    size_t uxPayloadSize;
    /* memcpy() helper variables for MISRA Rule 21.15 compliance*/
    const void * pvCopySource;
    void * pvCopyDest;

    /* Map the UDP packet onto the start of the frame. */
    pxUDPPacket = ipCAST_PTR_TO_TYPE_PTR( UDPPacket_t, pxNetworkBuffer->pucEthernetBuffer );

    #if ipconfigSUPPORT_OUTGOING_PINGS == 1
        if( pxNetworkBuffer->usPort == ( uint16_t ) ipPACKET_CONTAINS_ICMP_DATA )
        {
            uxPayloadSize = pxNetworkBuffer->xDataLength - sizeof( ICMPPacket_t );
        }
        else
    #endif
    {
        uxPayloadSize = pxNetworkBuffer->xDataLength - sizeof( UDPPacket_t );
    }

    /* Determine the ARP cache status for the requested IP address. */
    eReturned = eARPGetCacheEntry( &( ulIPAddress ), &( pxUDPPacket->xEthernetHeader.xDestinationAddress ) );

    if( eReturned != eCantSendPacket )
    {
        if( eReturned == eARPCacheHit )
        {
            #if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
                uint8_t ucSocketOptions;
            #endif
            iptraceSENDING_UDP_PACKET( pxNetworkBuffer->ulIPAddress );

            /* Create short cuts to the data within the packet. */
            pxIPHeader = &( pxUDPPacket->xIPHeader );

            #if ( ipconfigSUPPORT_OUTGOING_PINGS == 1 )

                /* Is it possible that the packet is not actually a UDP packet
                 * after all, but an ICMP packet. */
                if( pxNetworkBuffer->usPort != ( uint16_t ) ipPACKET_CONTAINS_ICMP_DATA )
            #endif /* ipconfigSUPPORT_OUTGOING_PINGS */
            {
                UDPHeader_t * pxUDPHeader;

                pxUDPHeader = &( pxUDPPacket->xUDPHeader );

                pxUDPHeader->usDestinationPort = pxNetworkBuffer->usPort;
                pxUDPHeader->usSourcePort = pxNetworkBuffer->usBoundPort;
                pxUDPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( UDPHeader_t ) );
                pxUDPHeader->usLength = FreeRTOS_htons( pxUDPHeader->usLength );
                pxUDPHeader->usChecksum = 0U;
            }

            /* memcpy() the constant parts of the header information into
             * the correct location within the packet.  This fills in:
             *  xEthernetHeader.xSourceAddress
             *  xEthernetHeader.usFrameType
             *  xIPHeader.ucVersionHeaderLength
             *  xIPHeader.ucDifferentiatedServicesCode
             *  xIPHeader.usLength
             *  xIPHeader.usIdentification
             *  xIPHeader.usFragmentOffset
             *  xIPHeader.ucTimeToLive
             *  xIPHeader.ucProtocol
             * and
             *  xIPHeader.usHeaderChecksum
             */

            /* Save options now, as they will be overwritten by memcpy */
            #if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
                {
                    ucSocketOptions = pxNetworkBuffer->pucEthernetBuffer[ ipSOCKET_OPTIONS_OFFSET ];
                }
            #endif

            /*
             * Offset the memcpy by the size of a MAC address to start at the packet's
             * Ethernet header 'source' MAC address; the preceding 'destination' should not be altered.
             */

            /*
             * Use helper variables for memcpy() to remain
             * compliant with MISRA Rule 21.15.  These should be
             * optimized away.
             */
            pvCopySource = xDefaultPartUDPPacketHeader.ucBytes;
            /* The Ethernet source address is at offset 6. */
            pvCopyDest = &pxNetworkBuffer->pucEthernetBuffer[ sizeof( MACAddress_t ) ];
            ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( xDefaultPartUDPPacketHeader ) );

            #if ipconfigSUPPORT_OUTGOING_PINGS == 1
                if( pxNetworkBuffer->usPort == ( uint16_t ) ipPACKET_CONTAINS_ICMP_DATA )
                {
                    pxIPHeader->ucProtocol = ipPROTOCOL_ICMP;
                    pxIPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( IPHeader_t ) + sizeof( ICMPHeader_t ) );
                }
                else
            #endif /* ipconfigSUPPORT_OUTGOING_PINGS */
            {
                pxIPHeader->usLength = ( uint16_t ) ( uxPayloadSize + sizeof( IPHeader_t ) + sizeof( UDPHeader_t ) );
            }

            pxIPHeader->usLength = FreeRTOS_htons( pxIPHeader->usLength );
            pxIPHeader->ulDestinationIPAddress = pxNetworkBuffer->ulIPAddress;

            #if ( ipconfigUSE_LLMNR == 1 )
                {
                    /* LLMNR messages are typically used on a LAN and they're
                     * not supposed to cross routers */
                    if( pxNetworkBuffer->ulIPAddress == ipLLMNR_IP_ADDR )
                    {
                        pxIPHeader->ucTimeToLive = 0x01;
                    }
                }
            #endif

            #if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
                {
                    pxIPHeader->usHeaderChecksum = 0U;
                    pxIPHeader->usHeaderChecksum = usGenerateChecksum( 0U, ( uint8_t * ) &( pxIPHeader->ucVersionHeaderLength ), ipSIZE_OF_IPv4_HEADER );
                    pxIPHeader->usHeaderChecksum = ~FreeRTOS_htons( pxIPHeader->usHeaderChecksum );

                    if( ( ucSocketOptions & ( uint8_t ) FREERTOS_SO_UDPCKSUM_OUT ) != 0U )
                    {
                        ( void ) usGenerateProtocolChecksum( ( uint8_t * ) pxUDPPacket, pxNetworkBuffer->xDataLength, pdTRUE );
                    }
                    else
                    {
                        pxUDPPacket->xUDPHeader.usChecksum = 0U;
                    }
                }
            #endif /* if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 ) */
        }
        else if( eReturned == eARPCacheMiss )
        {
            /* Add an entry to the ARP table with a null hardware address.
             * This allows the ARP timer to know that an ARP reply is
             * outstanding, and perform retransmissions if necessary. */
            vARPRefreshCacheEntry( NULL, ulIPAddress );

            /* Generate an ARP for the required IP address. */
            iptracePACKET_DROPPED_TO_GENERATE_ARP( pxNetworkBuffer->ulIPAddress );
            pxNetworkBuffer->ulIPAddress = ulIPAddress;
            vARPGenerateRequestPacket( pxNetworkBuffer );
        }
        else
        {
            /* The lookup indicated that an ARP request has already been
             * sent out for the queried IP address. */
            eReturned = eCantSendPacket;
        }
    }

    if( eReturned != eCantSendPacket )
    {
        /* The network driver is responsible for freeing the network buffer
         * after the packet has been sent. */

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
        iptraceNETWORK_INTERFACE_OUTPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );
        ( void ) xNetworkInterfaceOutput( pxNetworkBuffer, pdTRUE );
    }
    else
    {
        /* The packet can't be sent (DHCP not completed?).  Just drop the
         * packet. */
        vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Process the received UDP packet.
 *
 * @param[in] pxNetworkBuffer: The network buffer carrying the UDP packet.
 * @param[in] usPort: The port number on which this packet was received.
 *
 * @return pdPASS in case the UDP packet could be processed. Else pdFAIL is returned.
 */
BaseType_t xProcessReceivedUDPPacket( NetworkBufferDescriptor_t * pxNetworkBuffer,
                                      uint16_t usPort )
{
    BaseType_t xReturn = pdPASS;
    FreeRTOS_Socket_t * pxSocket;

    configASSERT( pxNetworkBuffer != NULL );
    configASSERT( pxNetworkBuffer->pucEthernetBuffer != NULL );

    /* Map the ethernet buffer to the UDPPacket_t struct for easy access to the fields. */
    const UDPPacket_t * pxUDPPacket = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( UDPPacket_t, pxNetworkBuffer->pucEthernetBuffer );

    /* Caller must check for minimum packet size. */
    pxSocket = pxUDPSocketLookup( usPort );

    if( pxSocket != NULL )
    {
        /* When refreshing the ARP cache with received UDP packets we must be
         * careful;  hundreds of broadcast messages may pass and if we're not
         * handling them, no use to fill the ARP cache with those IP addresses. */
        vARPRefreshCacheEntry( &( pxUDPPacket->xEthernetHeader.xSourceAddress ), pxUDPPacket->xIPHeader.ulSourceIPAddress );

        #if ( ipconfigUSE_CALLBACKS == 1 )
            {
                /* Did the owner of this socket register a reception handler ? */
                if( ipconfigIS_VALID_PROG_ADDRESS( pxSocket->u.xUDP.pxHandleReceive ) )
                {
                    struct freertos_sockaddr xSourceAddress, destinationAddress;
                    void * pcData = &( pxNetworkBuffer->pucEthernetBuffer[ ipUDP_PAYLOAD_OFFSET_IPv4 ] );
                    FOnUDPReceive_t xHandler = ( FOnUDPReceive_t ) pxSocket->u.xUDP.pxHandleReceive;
                    xSourceAddress.sin_port = pxNetworkBuffer->usPort;
                    xSourceAddress.sin_addr = pxNetworkBuffer->ulIPAddress;
                    destinationAddress.sin_port = usPort;
                    destinationAddress.sin_addr = pxUDPPacket->xIPHeader.ulDestinationIPAddress;

                    /* The value of 'xDataLength' was proven to be at least the size of a UDP packet in prvProcessIPPacket(). */
                    if( xHandler( ( Socket_t ) pxSocket,
                                  ( void * ) pcData,
                                  ( size_t ) ( pxNetworkBuffer->xDataLength - ipUDP_PAYLOAD_OFFSET_IPv4 ),
                                  &( xSourceAddress ),
                                  &( destinationAddress ) ) != 0 )
                    {
                        xReturn = pdFAIL; /* xHandler has consumed the data, do not add it to .xWaitingPacketsList'. */
                    }
                }
            }
        #endif /* ipconfigUSE_CALLBACKS */

        #if ( ipconfigUDP_MAX_RX_PACKETS > 0U )
            {
                if( xReturn == pdPASS )
                {
                    if( listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) ) >= pxSocket->u.xUDP.uxMaxPackets )
                    {
                        FreeRTOS_debug_printf( ( "xProcessReceivedUDPPacket: buffer full %ld >= %ld port %u\n",
                                                 listCURRENT_LIST_LENGTH( &( pxSocket->u.xUDP.xWaitingPacketsList ) ),
                                                 pxSocket->u.xUDP.uxMaxPackets, pxSocket->usLocalPort ) );
                        xReturn = pdFAIL; /* we did not consume or release the buffer */
                    }
                }
            }
        #endif /* if ( ipconfigUDP_MAX_RX_PACKETS > 0U ) */

        #if ( ipconfigUSE_CALLBACKS == 1 ) || ( ipconfigUDP_MAX_RX_PACKETS > 0U )
            if( xReturn == pdPASS ) /*lint !e774: Boolean within 'if' always evaluates to True, depending on configuration. [MISRA 2012 Rule 14.3, required. */
        #else
            /* xReturn is still pdPASS. */
        #endif
        {
            vTaskSuspendAll();
            {
                taskENTER_CRITICAL();
                {
                    /* Add the network packet to the list of packets to be
                     * processed by the socket. */
                    vListInsertEnd( &( pxSocket->u.xUDP.xWaitingPacketsList ), &( pxNetworkBuffer->xBufferListItem ) );
                }
                taskEXIT_CRITICAL();
            }
            ( void ) xTaskResumeAll();

            /* Set the socket's receive event */
            if( pxSocket->xEventGroup != NULL )
            {
                ( void ) xEventGroupSetBits( pxSocket->xEventGroup, ( EventBits_t ) eSOCKET_RECEIVE );
            }

            #if ( ipconfigSUPPORT_SELECT_FUNCTION == 1 )
                {
                    if( ( pxSocket->pxSocketSet != NULL ) && ( ( pxSocket->xSelectBits & ( ( EventBits_t ) eSELECT_READ ) ) != 0U ) )
                    {
                        ( void ) xEventGroupSetBits( pxSocket->pxSocketSet->xSelectGroup, ( EventBits_t ) eSELECT_READ );
                    }
                }
            #endif

            #if ( ipconfigSOCKET_HAS_USER_SEMAPHORE == 1 )
                {
                    if( pxSocket->pxUserSemaphore != NULL )
                    {
                        ( void ) xSemaphoreGive( pxSocket->pxUserSemaphore );
                    }
                }
            #endif

            #if ( ipconfigUSE_DHCP == 1 )
                {
                    if( xIsDHCPSocket( pxSocket ) != 0 )
                    {
                        ( void ) xSendDHCPEvent();
                    }
                }
            #endif
        }
    }
    else
    {
        /* There is no socket listening to the target port, but still it might
         * be for this node. */

        #if ( ipconfigUSE_DNS == 1 ) && ( ipconfigDNS_USE_CALLBACKS == 1 )

            /* A DNS reply, check for the source port.  Although the DNS client
             * does open a UDP socket to send a messages, this socket will be
             * closed after a short timeout.  Messages that come late (after the
             * socket is closed) will be treated here. */
            if( FreeRTOS_ntohs( pxUDPPacket->xUDPHeader.usSourcePort ) == ( uint16_t ) ipDNS_PORT )
            {
                vARPRefreshCacheEntry( &( pxUDPPacket->xEthernetHeader.xSourceAddress ), pxUDPPacket->xIPHeader.ulSourceIPAddress );
                xReturn = ( BaseType_t ) ulDNSHandlePacket( pxNetworkBuffer );
            }
            else
        #endif

        #if ( ipconfigUSE_LLMNR == 1 )
            /* A LLMNR request, check for the destination port. */
            if( ( usPort == FreeRTOS_ntohs( ipLLMNR_PORT ) ) ||
                ( pxUDPPacket->xUDPHeader.usSourcePort == FreeRTOS_ntohs( ipLLMNR_PORT ) ) )
            {
                vARPRefreshCacheEntry( &( pxUDPPacket->xEthernetHeader.xSourceAddress ), pxUDPPacket->xIPHeader.ulSourceIPAddress );
                xReturn = ( BaseType_t ) ulDNSHandlePacket( pxNetworkBuffer );
            }
            else
        #endif /* ipconfigUSE_LLMNR */

        #if ( ipconfigUSE_NBNS == 1 )
            /* a NetBIOS request, check for the destination port */
            if( ( usPort == FreeRTOS_ntohs( ipNBNS_PORT ) ) ||
                ( pxUDPPacket->xUDPHeader.usSourcePort == FreeRTOS_ntohs( ipNBNS_PORT ) ) )
            {
                vARPRefreshCacheEntry( &( pxUDPPacket->xEthernetHeader.xSourceAddress ), pxUDPPacket->xIPHeader.ulSourceIPAddress );
                xReturn = ( BaseType_t ) ulNBNSHandlePacket( pxNetworkBuffer );
            }
            else
        #endif /* ipconfigUSE_NBNS */
        {
            xReturn = pdFAIL;
        }
    }

    return xReturn;
}
/*-----------------------------------------------------------*/
