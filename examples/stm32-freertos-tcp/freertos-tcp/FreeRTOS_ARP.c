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
 * @file FreeRTOS_ARP.c
 * @brief Implements the Address Resolution Protocol for the FreeRTOS+TCP network stack.
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
#include "FreeRTOS_ARP.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_DHCP.h"
#if ( ipconfigUSE_LLMNR == 1 )
    #include "FreeRTOS_DNS.h"
#endif /* ipconfigUSE_LLMNR */
#include "NetworkBufferManagement.h"
#include "NetworkInterface.h"

/** @brief When the age of an entry in the ARP table reaches this value (it counts down
 * to zero, so this is an old entry) an ARP request will be sent to see if the
 * entry is still valid and can therefore be refreshed. */
#define arpMAX_ARP_AGE_BEFORE_NEW_ARP_REQUEST    ( 3 )

/** @brief The time between gratuitous ARPs. */
#ifndef arpGRATUITOUS_ARP_PERIOD
    #define arpGRATUITOUS_ARP_PERIOD    ( pdMS_TO_TICKS( 20000U ) )
#endif

/*-----------------------------------------------------------*/

/*
 * Lookup an MAC address in the ARP cache from the IP address.
 */
static eARPLookupResult_t prvCacheLookup( uint32_t ulAddressToLookup,
                                          MACAddress_t * const pxMACAddress );

/*-----------------------------------------------------------*/

/** @brief The ARP cache. */
_static ARPCacheRow_t xARPCache[ ipconfigARP_CACHE_ENTRIES ];

/** @brief  The time at which the last gratuitous ARP was sent.  Gratuitous ARPs are used
 * to ensure ARP tables are up to date and to detect IP address conflicts. */
static TickType_t xLastGratuitousARPTime = ( TickType_t ) 0;

/*
 * IP-clash detection is currently only used internally. When DHCP doesn't respond, the
 * driver can try out a random LinkLayer IP address (169.254.x.x).  It will send out a
 * gratuitous ARP message and, after a period of time, check the variables here below:
 */
#if ( ipconfigARP_USE_CLASH_DETECTION != 0 )
    /* Becomes non-zero if another device responded to a gratuitous ARP message. */
    BaseType_t xARPHadIPClash;
    /* MAC-address of the other device containing the same IP-address. */
    MACAddress_t xARPClashMacAddress;
#endif /* ipconfigARP_USE_CLASH_DETECTION */

/*-----------------------------------------------------------*/

/**
 * @brief Process the ARP packets.
 *
 * @param[in] pxARPFrame: The ARP Frame (the ARP packet).
 *
 * @return An enum which says whether to return the frame or to release it.
 */
eFrameProcessingResult_t eARPProcessPacket( ARPPacket_t * const pxARPFrame )
{
    eFrameProcessingResult_t eReturn = eReleaseBuffer;
    ARPHeader_t * pxARPHeader;
    uint32_t ulTargetProtocolAddress, ulSenderProtocolAddress;
/* memcpy() helper variables for MISRA Rule 21.15 compliance*/
    const void * pvCopySource;
    void * pvCopyDest;

    pxARPHeader = &( pxARPFrame->xARPHeader );

    /* The field ulSenderProtocolAddress is badly aligned, copy byte-by-byte. */

    /*
     * Use helper variables for memcpy() to remain
     * compliant with MISRA Rule 21.15.  These should be
     * optimized away.
     */
    pvCopySource = pxARPHeader->ucSenderProtocolAddress;
    pvCopyDest = &ulSenderProtocolAddress;
    ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( ulSenderProtocolAddress ) );
    /* The field ulTargetProtocolAddress is well-aligned, a 32-bits copy. */
    ulTargetProtocolAddress = pxARPHeader->ulTargetProtocolAddress;

    traceARP_PACKET_RECEIVED();

    /* Don't do anything if the local IP address is zero because
     * that means a DHCP request has not completed. */
    if( *ipLOCAL_IP_ADDRESS_POINTER != 0UL )
    {
        switch( pxARPHeader->usOperation )
        {
            case ipARP_REQUEST:

                /* The packet contained an ARP request.  Was it for the IP
                 * address of the node running this code? */
                if( ulTargetProtocolAddress == *ipLOCAL_IP_ADDRESS_POINTER )
                {
                    iptraceSENDING_ARP_REPLY( ulSenderProtocolAddress );

                    /* The request is for the address of this node.  Add the
                     * entry into the ARP cache, or refresh the entry if it
                     * already exists. */
                    vARPRefreshCacheEntry( &( pxARPHeader->xSenderHardwareAddress ), ulSenderProtocolAddress );

                    /* Generate a reply payload in the same buffer. */
                    pxARPHeader->usOperation = ( uint16_t ) ipARP_REPLY;

                    if( ulTargetProtocolAddress == ulSenderProtocolAddress )
                    {
                        /* A double IP address is detected! */
                        /* Give the sources MAC address the value of the broadcast address, will be swapped later */

                        /*
                         * Use helper variables for memcpy() to remain
                         * compliant with MISRA Rule 21.15.  These should be
                         * optimized away.
                         */
                        pvCopySource = xBroadcastMACAddress.ucBytes;
                        pvCopyDest = pxARPFrame->xEthernetHeader.xSourceAddress.ucBytes;
                        ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( xBroadcastMACAddress ) );

                        ( void ) memset( pxARPHeader->xTargetHardwareAddress.ucBytes, 0, sizeof( MACAddress_t ) );
                        pxARPHeader->ulTargetProtocolAddress = 0UL;
                    }
                    else
                    {
                        /*
                         * Use helper variables for memcpy() to remain
                         * compliant with MISRA Rule 21.15.  These should be
                         * optimized away.
                         */
                        pvCopySource = pxARPHeader->xSenderHardwareAddress.ucBytes;
                        pvCopyDest = pxARPHeader->xTargetHardwareAddress.ucBytes;
                        ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( MACAddress_t ) );
                        pxARPHeader->ulTargetProtocolAddress = ulSenderProtocolAddress;
                    }

                    /*
                     * Use helper variables for memcpy() to remain
                     * compliant with MISRA Rule 21.15.  These should be
                     * optimized away.
                     */
                    pvCopySource = ipLOCAL_MAC_ADDRESS;
                    pvCopyDest = pxARPHeader->xSenderHardwareAddress.ucBytes;
                    ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( MACAddress_t ) );
                    pvCopySource = ipLOCAL_IP_ADDRESS_POINTER;
                    pvCopyDest = pxARPHeader->ucSenderProtocolAddress;
                    ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( pxARPHeader->ucSenderProtocolAddress ) );

                    eReturn = eReturnEthernetFrame;
                }

                break;

            case ipARP_REPLY:
                iptracePROCESSING_RECEIVED_ARP_REPLY( ulTargetProtocolAddress );
                vARPRefreshCacheEntry( &( pxARPHeader->xSenderHardwareAddress ), ulSenderProtocolAddress );
                /* Process received ARP frame to see if there is a clash. */
                #if ( ipconfigARP_USE_CLASH_DETECTION != 0 )
                    {
                        if( ulSenderProtocolAddress == *ipLOCAL_IP_ADDRESS_POINTER )
                        {
                            xARPHadIPClash = pdTRUE;
                            /* Remember the MAC-address of the other device which has the same IP-address. */
                            ( void ) memcpy( xARPClashMacAddress.ucBytes, pxARPHeader->xSenderHardwareAddress.ucBytes, sizeof( xARPClashMacAddress.ucBytes ) );
                        }
                    }
                #endif /* ipconfigARP_USE_CLASH_DETECTION */
                break;

            default:
                /* Invalid. */
                break;
        }
    }

    return eReturn;
}
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_ARP_REMOVE_ENTRY != 0 )

/**
 * @brief Remove an ARP cache entry that matches with .pxMACAddress.
 *
 * @param[in] pxMACAddress: Pointer to the MAC address whose entry shall
 *                          be removed..
 * @return When the entry was found and remove: the IP-address, otherwise zero.
 */
    uint32_t ulARPRemoveCacheEntryByMac( const MACAddress_t * pxMACAddress )
    {
        BaseType_t x;
        uint32_t lResult = 0;

        configASSERT( pxMACAddress != NULL );

        /* For each entry in the ARP cache table. */
        for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
        {
            if( ( memcmp( xARPCache[ x ].xMACAddress.ucBytes, pxMACAddress->ucBytes, sizeof( pxMACAddress->ucBytes ) ) == 0 ) )
            {
                lResult = xARPCache[ x ].ulIPAddress;
                ( void ) memset( &xARPCache[ x ], 0, sizeof( xARPCache[ x ] ) );
                break;
            }
        }

        return lResult;
    }

#endif /* ipconfigUSE_ARP_REMOVE_ENTRY != 0 */
/*-----------------------------------------------------------*/

/**
 * @brief Add/update the ARP cache entry MAC-address to IP-address mapping.
 *
 * @param[in] pxMACAddress: Pointer to the MAC address whose mapping is being
 *                          updated.
 * @param[in] ulIPAddress: 32-bit representation of the IP-address whose mapping
 *                         is being updated.
 */
void vARPRefreshCacheEntry( const MACAddress_t * pxMACAddress,
                            const uint32_t ulIPAddress )
{
    BaseType_t x = 0;
    BaseType_t xIpEntry = -1;
    BaseType_t xMacEntry = -1;
    BaseType_t xUseEntry = 0;
    uint8_t ucMinAgeFound = 0U;

    #if ( ipconfigARP_STORES_REMOTE_ADDRESSES == 0 )

        /* Only process the IP address if it is on the local network.
         * Unless: when '*ipLOCAL_IP_ADDRESS_POINTER' equals zero, the IP-address
         * and netmask are still unknown. */
        if( ( ( ulIPAddress & xNetworkAddressing.ulNetMask ) == ( ( *ipLOCAL_IP_ADDRESS_POINTER ) & xNetworkAddressing.ulNetMask ) ) ||
            ( *ipLOCAL_IP_ADDRESS_POINTER == 0UL ) )
    #else

        /* If ipconfigARP_STORES_REMOTE_ADDRESSES is non-zero, IP addresses with
         * a different netmask will also be stored.  After when replying to a UDP
         * message from a different netmask, the IP address can be looped up and a
         * reply sent.  This option is useful for systems with multiple gateways,
         * the reply will surely arrive.  If ipconfigARP_STORES_REMOTE_ADDRESSES is
         * zero the the gateway address is the only option. */

        if( pdTRUE )
    #endif
    {
        /* Start with the maximum possible number. */
        ucMinAgeFound--;

        /* For each entry in the ARP cache table. */
        for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
        {
            BaseType_t xMatchingMAC;

            if( pxMACAddress != NULL )
            {
                if( memcmp( xARPCache[ x ].xMACAddress.ucBytes, pxMACAddress->ucBytes, sizeof( pxMACAddress->ucBytes ) ) == 0 )
                {
                    xMatchingMAC = pdTRUE;
                }
                else
                {
                    xMatchingMAC = pdFALSE;
                }
            }
            else
            {
                xMatchingMAC = pdFALSE;
            }

            /* Does this line in the cache table hold an entry for the IP
             * address being queried? */
            if( xARPCache[ x ].ulIPAddress == ulIPAddress )
            {
                if( pxMACAddress == NULL )
                {
                    /* In case the parameter pxMACAddress is NULL, an entry will be reserved to
                     * indicate that there is an outstanding ARP request, This entry will have
                     * "ucValid == pdFALSE". */
                    xIpEntry = x;
                    break;
                }

                /* See if the MAC-address also matches. */
                if( xMatchingMAC != pdFALSE )
                {
                    /* This function will be called for each received packet
                     * As this is by far the most common path the coding standard
                     * is relaxed in this case and a return is permitted as an
                     * optimisation. */
                    xARPCache[ x ].ucAge = ( uint8_t ) ipconfigMAX_ARP_AGE;
                    xARPCache[ x ].ucValid = ( uint8_t ) pdTRUE;
                    return;
                }

                /* Found an entry containing ulIPAddress, but the MAC address
                 * doesn't match.  Might be an entry with ucValid=pdFALSE, waiting
                 * for an ARP reply.  Still want to see if there is match with the
                 * given MAC address.ucBytes.  If found, either of the two entries
                 * must be cleared. */
                xIpEntry = x;
            }
            else if( xMatchingMAC != pdFALSE )
            {
                /* Found an entry with the given MAC-address, but the IP-address
                 * is different.  Continue looping to find a possible match with
                 * ulIPAddress. */
                #if ( ipconfigARP_STORES_REMOTE_ADDRESSES != 0 )

                    /* If ARP stores the MAC address of IP addresses outside the
                     * network, than the MAC address of the gateway should not be
                     * overwritten. */
                    BaseType_t bIsLocal[ 2 ];
                    bIsLocal[ 0 ] = ( ( xARPCache[ x ].ulIPAddress & xNetworkAddressing.ulNetMask ) == ( ( *ipLOCAL_IP_ADDRESS_POINTER ) & xNetworkAddressing.ulNetMask ) );
                    bIsLocal[ 1 ] = ( ( ulIPAddress & xNetworkAddressing.ulNetMask ) == ( ( *ipLOCAL_IP_ADDRESS_POINTER ) & xNetworkAddressing.ulNetMask ) );

                    if( bIsLocal[ 0 ] == bIsLocal[ 1 ] )
                    {
                        xMacEntry = x;
                    }
                #else /* if ( ipconfigARP_STORES_REMOTE_ADDRESSES != 0 ) */
                    xMacEntry = x;
                #endif /* if ( ipconfigARP_STORES_REMOTE_ADDRESSES != 0 ) */
            }

            /* _HT_
             * Shouldn't we test for xARPCache[ x ].ucValid == pdFALSE here ? */
            else if( xARPCache[ x ].ucAge < ucMinAgeFound )
            {
                /* As the table is traversed, remember the table row that
                 * contains the oldest entry (the lowest age count, as ages are
                 * decremented to zero) so the row can be re-used if this function
                 * needs to add an entry that does not already exist. */
                ucMinAgeFound = xARPCache[ x ].ucAge;
                xUseEntry = x;
            }
            else
            {
                /* Nothing happens to this cache entry for now. */
            }
        }

        if( xMacEntry >= 0 )
        {
            xUseEntry = xMacEntry;

            if( xIpEntry >= 0 )
            {
                /* Both the MAC address as well as the IP address were found in
                 * different locations: clear the entry which matches the
                 * IP-address */
                ( void ) memset( &( xARPCache[ xIpEntry ] ), 0, sizeof( ARPCacheRow_t ) );
            }
        }
        else if( xIpEntry >= 0 )
        {
            /* An entry containing the IP-address was found, but it had a different MAC address */
            xUseEntry = xIpEntry;
        }
        else
        {
            /* No matching entry found. */
        }

        /* If the entry was not found, we use the oldest entry and set the IPaddress */
        xARPCache[ xUseEntry ].ulIPAddress = ulIPAddress;

        if( pxMACAddress != NULL )
        {
            ( void ) memcpy( xARPCache[ xUseEntry ].xMACAddress.ucBytes, pxMACAddress->ucBytes, sizeof( pxMACAddress->ucBytes ) );

            iptraceARP_TABLE_ENTRY_CREATED( ulIPAddress, ( *pxMACAddress ) );
            /* And this entry does not need immediate attention */
            xARPCache[ xUseEntry ].ucAge = ( uint8_t ) ipconfigMAX_ARP_AGE;
            xARPCache[ xUseEntry ].ucValid = ( uint8_t ) pdTRUE;
        }
        else if( xIpEntry < 0 )
        {
            xARPCache[ xUseEntry ].ucAge = ( uint8_t ) ipconfigMAX_ARP_RETRANSMISSIONS;
            xARPCache[ xUseEntry ].ucValid = ( uint8_t ) pdFALSE;
        }
        else
        {
            /* Nothing will be stored. */
        }
    }
}
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_ARP_REVERSED_LOOKUP == 1 )

/**
 * @brief Retrieve an entry from the cache table
 *
 * @param[in] pxMACAddress: The MAC-address of the entry of interest.
 * @param[out] pulIPAddress: set to the IP-address found, or unchanged when not found.
 *
 * @return Either eARPCacheMiss or eARPCacheHit.
 */
    eARPLookupResult_t eARPGetCacheEntryByMac( MACAddress_t * const pxMACAddress,
                                               uint32_t * pulIPAddress )
    {
        BaseType_t x;
        eARPLookupResult_t eReturn = eARPCacheMiss;

        configASSERT( pxMACAddress != NULL );
        configASSERT( pulIPAddress != NULL );

        /* Loop through each entry in the ARP cache. */
        for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
        {
            /* Does this row in the ARP cache table hold an entry for the MAC
             * address being searched? */
            if( memcmp( pxMACAddress->ucBytes, xARPCache[ x ].xMACAddress.ucBytes, sizeof( MACAddress_t ) ) == 0 )
            {
                *pulIPAddress = xARPCache[ x ].ulIPAddress;
                eReturn = eARPCacheHit;
                break;
            }
        }

        return eReturn;
    }
#endif /* ipconfigUSE_ARP_REVERSED_LOOKUP */

/*-----------------------------------------------------------*/

/**
 * @brief Look for ulIPAddress in the ARP cache.
 *
 * @param[in,out] pulIPAddress: Pointer to the IP-address to be queried to the ARP cache.
 * @param[in,out] pxMACAddress: Pointer to a MACAddress_t variable where the MAC address
 *                          will be stored, if found.
 *
 * @return If the IP address exists, copy the associated MAC address into pxMACAddress,
 *         refresh the ARP cache entry's age, and return eARPCacheHit. If the IP
 *         address does not exist in the ARP cache return eARPCacheMiss. If the packet
 *         cannot be sent for any reason (maybe DHCP is still in process, or the
 *         addressing needs a gateway but there isn't a gateway defined) then return
 *         eCantSendPacket.
 */
eARPLookupResult_t eARPGetCacheEntry( uint32_t * pulIPAddress,
                                      MACAddress_t * const pxMACAddress )
{
    eARPLookupResult_t eReturn;
    uint32_t ulAddressToLookup;

    ulAddressToLookup = *pulIPAddress;

    #if ( ipconfigUSE_LLMNR == 1 )
        if( ulAddressToLookup == ipLLMNR_IP_ADDR ) /* Is in network byte order. */
        {
            /* The LLMNR IP-address has a fixed virtual MAC address. */
            ( void ) memcpy( pxMACAddress->ucBytes, xLLMNR_MacAdress.ucBytes, sizeof( MACAddress_t ) );
            eReturn = eARPCacheHit;
        }
        else
    #endif

    if( xIsIPv4Multicast( ulAddressToLookup ) != 0 )
    {
        /* Get the lowest 23 bits of the IP-address. */
        vSetMultiCastIPv4MacAddress( ulAddressToLookup, pxMACAddress );

        eReturn = eARPCacheHit;
    }
    else if( ( *pulIPAddress == ipBROADCAST_IP_ADDRESS ) ||               /* Is it the general broadcast address 255.255.255.255? */
             ( *pulIPAddress == xNetworkAddressing.ulBroadcastAddress ) ) /* Or a local broadcast address, eg 192.168.1.255? */
    {
        /* This is a broadcast so it uses the broadcast MAC address. */
        ( void ) memcpy( pxMACAddress->ucBytes, xBroadcastMACAddress.ucBytes, sizeof( MACAddress_t ) );
        eReturn = eARPCacheHit;
    }
    else if( *ipLOCAL_IP_ADDRESS_POINTER == 0UL )
    {
        /* The IP address has not yet been assigned, so there is nothing that
         * can be done. */
        eReturn = eCantSendPacket;
    }
    else
    {
        eReturn = eARPCacheMiss;

        if( ( *pulIPAddress & xNetworkAddressing.ulNetMask ) != ( ( *ipLOCAL_IP_ADDRESS_POINTER ) & xNetworkAddressing.ulNetMask ) )
        {
            /* No matching end-point is found, look for a gateway. */
            #if ( ipconfigARP_STORES_REMOTE_ADDRESSES == 1 )
                eReturn = prvCacheLookup( *pulIPAddress, pxMACAddress );

                if( eReturn == eARPCacheHit )
                {
                    /* The stack is configured to store 'remote IP addresses', i.e. addresses
                     * belonging to a different the netmask.  prvCacheLookup() returned a hit, so
                     * the MAC address is known. */
                }
                else
            #endif
            {
                /* The IP address is off the local network, so look up the
                 * hardware address of the router, if any. */
                if( xNetworkAddressing.ulGatewayAddress != ( uint32_t ) 0U )
                {
                    ulAddressToLookup = xNetworkAddressing.ulGatewayAddress;
                }
                else
                {
                    ulAddressToLookup = *pulIPAddress;
                }
            }
        }
        else
        {
            /* The IP address is on the local network, so lookup the requested
             * IP address directly. */
            ulAddressToLookup = *pulIPAddress;
        }

        #if ( ipconfigARP_STORES_REMOTE_ADDRESSES == 1 )
            if( eReturn == eARPCacheMiss ) /*lint !e774: (Info -- Boolean within 'if' always evaluates to True, depending on configuration. */
        #else
            /* No cache look-up was done, so the result is still 'eARPCacheMiss'. */
        #endif
        {
            if( ulAddressToLookup == 0UL )
            {
                /* The address is not on the local network, and there is not a
                 * router. */
                eReturn = eCantSendPacket;
            }
            else
            {
                eReturn = prvCacheLookup( ulAddressToLookup, pxMACAddress );

                if( eReturn == eARPCacheMiss )
                {
                    /* It might be that the ARP has to go to the gateway. */
                    *pulIPAddress = ulAddressToLookup;
                }
            }
        }
    }

    return eReturn;
}

/*-----------------------------------------------------------*/

/**
 * @brief Lookup an IP address in the ARP cache.
 *
 * @param[in] ulAddressToLookup: The 32-bit representation of an IP address to
 *                               lookup.
 * @param[out] pxMACAddress: A pointer to MACAddress_t variable where, if there
 *                          is an ARP cache hit, the MAC address corresponding to
 *                          the IP address will be stored.
 *
 * @return When the IP-address is found: eARPCacheHit, when not found: eARPCacheMiss,
 *         and when waiting for a ARP reply: eCantSendPacket.
 */
static eARPLookupResult_t prvCacheLookup( uint32_t ulAddressToLookup,
                                          MACAddress_t * const pxMACAddress )
{
    BaseType_t x;
    eARPLookupResult_t eReturn = eARPCacheMiss;

    /* Loop through each entry in the ARP cache. */
    for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
    {
        /* Does this row in the ARP cache table hold an entry for the IP address
         * being queried? */
        if( xARPCache[ x ].ulIPAddress == ulAddressToLookup )
        {
            /* A matching valid entry was found. */
            if( xARPCache[ x ].ucValid == ( uint8_t ) pdFALSE )
            {
                /* This entry is waiting an ARP reply, so is not valid. */
                eReturn = eCantSendPacket;
            }
            else
            {
                /* A valid entry was found. */
                ( void ) memcpy( pxMACAddress->ucBytes, xARPCache[ x ].xMACAddress.ucBytes, sizeof( MACAddress_t ) );
                eReturn = eARPCacheHit;
            }

            break;
        }
    }

    return eReturn;
}
/*-----------------------------------------------------------*/

/**
 * @brief A call to this function will update (or 'Age') the ARP cache entries.
 *        The function will also try to prevent a removal of entry by sending
 *        an ARP query. It will also check whether we are waiting on an ARP
 *        reply - if we are, then an ARP request will be re-sent.
 *        In case an ARP entry has 'Aged' to 0, it will be removed from the ARP
 *        cache.
 */
void vARPAgeCache( void )
{
    BaseType_t x;
    TickType_t xTimeNow;

    /* Loop through each entry in the ARP cache. */
    for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
    {
        /* If the entry is valid (its age is greater than zero). */
        if( xARPCache[ x ].ucAge > 0U )
        {
            /* Decrement the age value of the entry in this ARP cache table row.
             * When the age reaches zero it is no longer considered valid. */
            ( xARPCache[ x ].ucAge )--;

            /* If the entry is not yet valid, then it is waiting an ARP
             * reply, and the ARP request should be retransmitted. */
            if( xARPCache[ x ].ucValid == ( uint8_t ) pdFALSE )
            {
                FreeRTOS_OutputARPRequest( xARPCache[ x ].ulIPAddress );
            }
            else if( xARPCache[ x ].ucAge <= ( uint8_t ) arpMAX_ARP_AGE_BEFORE_NEW_ARP_REQUEST )
            {
                /* This entry will get removed soon.  See if the MAC address is
                 * still valid to prevent this happening. */
                iptraceARP_TABLE_ENTRY_WILL_EXPIRE( xARPCache[ x ].ulIPAddress );
                FreeRTOS_OutputARPRequest( xARPCache[ x ].ulIPAddress );
            }
            else
            {
                /* The age has just ticked down, with nothing to do. */
            }

            if( xARPCache[ x ].ucAge == 0U )
            {
                /* The entry is no longer valid.  Wipe it out. */
                iptraceARP_TABLE_ENTRY_EXPIRED( xARPCache[ x ].ulIPAddress );
                xARPCache[ x ].ulIPAddress = 0UL;
            }
        }
    }

    xTimeNow = xTaskGetTickCount();

    if( ( xLastGratuitousARPTime == ( TickType_t ) 0 ) || ( ( xTimeNow - xLastGratuitousARPTime ) > ( TickType_t ) arpGRATUITOUS_ARP_PERIOD ) )
    {
        FreeRTOS_OutputARPRequest( *ipLOCAL_IP_ADDRESS_POINTER );
        xLastGratuitousARPTime = xTimeNow;
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Send a Gratuitous ARP packet to allow this node to announce the IP-MAC
 *        mapping to the entire network.
 */
void vARPSendGratuitous( void )
{
    /* Setting xLastGratuitousARPTime to 0 will force a gratuitous ARP the next
     * time vARPAgeCache() is called. */
    xLastGratuitousARPTime = ( TickType_t ) 0;

    /* Let the IP-task call vARPAgeCache(). */
    ( void ) xSendEventToIPTask( eARPTimerEvent );
}

/*-----------------------------------------------------------*/

/**
 * @brief Create and send an ARP request packet.
 *
 * @param[in] ulIPAddress: A 32-bit representation of the IP-address whose
 *                         physical (MAC) address is required.
 */
void FreeRTOS_OutputARPRequest( uint32_t ulIPAddress )
{
    NetworkBufferDescriptor_t * pxNetworkBuffer;

    /* This is called from the context of the IP event task, so a block time
     * must not be used. */
    pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( sizeof( ARPPacket_t ), ( TickType_t ) 0U );

    if( pxNetworkBuffer != NULL )
    {
        pxNetworkBuffer->ulIPAddress = ulIPAddress;
        vARPGenerateRequestPacket( pxNetworkBuffer );

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

        if( xIsCallingFromIPTask() != 0 )
        {
            iptraceNETWORK_INTERFACE_OUTPUT( pxNetworkBuffer->xDataLength, pxNetworkBuffer->pucEthernetBuffer );
            /* Only the IP-task is allowed to call this function directly. */
            ( void ) xNetworkInterfaceOutput( pxNetworkBuffer, pdTRUE );
        }
        else
        {
            IPStackEvent_t xSendEvent;

            /* Send a message to the IP-task to send this ARP packet. */
            xSendEvent.eEventType = eNetworkTxEvent;
            xSendEvent.pvData = pxNetworkBuffer;

            if( xSendEventStructToIPTask( &xSendEvent, ( TickType_t ) portMAX_DELAY ) == pdFAIL )
            {
                /* Failed to send the message, so release the network buffer. */
                vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
            }
        }
    }
}
/*--------------------------------------*/

/**
 * @brief Generate an ARP request packet by copying various constant details to
 *        the buffer.
 *
 * @param[in,out] pxNetworkBuffer: Pointer to the buffer which has to be filled with
 *                             the ARP request packet details.
 */
void vARPGenerateRequestPacket( NetworkBufferDescriptor_t * const pxNetworkBuffer )
{
/* Part of the Ethernet and ARP headers are always constant when sending an IPv4
 * ARP packet.  This array defines the constant parts, allowing this part of the
 * packet to be filled in using a simple memcpy() instead of individual writes. */
    static const uint8_t xDefaultPartARPPacketHeader[] =
    {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, /* Ethernet destination address. */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* Ethernet source address. */
        0x08, 0x06,                         /* Ethernet frame type (ipARP_FRAME_TYPE). */
        0x00, 0x01,                         /* usHardwareType (ipARP_HARDWARE_TYPE_ETHERNET). */
        0x08, 0x00,                         /* usProtocolType. */
        ipMAC_ADDRESS_LENGTH_BYTES,         /* ucHardwareAddressLength. */
        ipIP_ADDRESS_LENGTH_BYTES,          /* ucProtocolAddressLength. */
        0x00, 0x01,                         /* usOperation (ipARP_REQUEST). */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* xSenderHardwareAddress. */
        0x00, 0x00, 0x00, 0x00,             /* ulSenderProtocolAddress. */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /* xTargetHardwareAddress. */
    };

    ARPPacket_t * pxARPPacket;

/* memcpy() helper variables for MISRA Rule 21.15 compliance*/
    const void * pvCopySource;
    void * pvCopyDest;

    /* Buffer allocation ensures that buffers always have space
     * for an ARP packet. See buffer allocation implementations 1
     * and 2 under portable/BufferManagement. */
    configASSERT( pxNetworkBuffer != NULL );
    configASSERT( pxNetworkBuffer->xDataLength >= sizeof( ARPPacket_t ) );

    pxARPPacket = ipCAST_PTR_TO_TYPE_PTR( ARPPacket_t, pxNetworkBuffer->pucEthernetBuffer );

    /* memcpy the const part of the header information into the correct
     * location in the packet.  This copies:
     *  xEthernetHeader.ulDestinationAddress
     *  xEthernetHeader.usFrameType;
     *  xARPHeader.usHardwareType;
     *  xARPHeader.usProtocolType;
     *  xARPHeader.ucHardwareAddressLength;
     *  xARPHeader.ucProtocolAddressLength;
     *  xARPHeader.usOperation;
     *  xARPHeader.xTargetHardwareAddress;
     */

    /*
     * Use helper variables for memcpy() to remain
     * compliant with MISRA Rule 21.15.  These should be
     * optimized away.
     */
    pvCopySource = xDefaultPartARPPacketHeader;
    pvCopyDest = pxARPPacket;
    ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( xDefaultPartARPPacketHeader ) );

    pvCopySource = ipLOCAL_MAC_ADDRESS;
    pvCopyDest = pxARPPacket->xEthernetHeader.xSourceAddress.ucBytes;
    ( void ) memcpy( pvCopyDest, pvCopySource, ipMAC_ADDRESS_LENGTH_BYTES );

    pvCopySource = ipLOCAL_MAC_ADDRESS;
    pvCopyDest = pxARPPacket->xARPHeader.xSenderHardwareAddress.ucBytes;
    ( void ) memcpy( pvCopyDest, pvCopySource, ipMAC_ADDRESS_LENGTH_BYTES );

    pvCopySource = ipLOCAL_IP_ADDRESS_POINTER;
    pvCopyDest = pxARPPacket->xARPHeader.ucSenderProtocolAddress;
    ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( pxARPPacket->xARPHeader.ucSenderProtocolAddress ) );
    pxARPPacket->xARPHeader.ulTargetProtocolAddress = pxNetworkBuffer->ulIPAddress;

    pxNetworkBuffer->xDataLength = sizeof( ARPPacket_t );

    iptraceCREATING_ARP_REQUEST( pxNetworkBuffer->ulIPAddress );
}
/*-----------------------------------------------------------*/

/**
 * @brief A call to this function will clear the ARP cache.
 */
void FreeRTOS_ClearARP( void )
{
    ( void ) memset( xARPCache, 0, sizeof( xARPCache ) );
}
/*-----------------------------------------------------------*/

#if 1

/**
 * @brief  This function will check if the target IP-address belongs to this device.
 *         If so, the packet will be passed to the IP-stack, who will answer it.
 *         The function is to be called within the function xNetworkInterfaceOutput().
 *
 * @param[in] pxDescriptor: The network buffer which is to be checked for loop-back.
 * @param[in] bReleaseAfterSend: pdTRUE: Driver is allowed to transfer ownership of descriptor.
 *                              pdFALSE: Driver is not allowed to take ownership of descriptor,
 *                                       make a copy of it.
 *
 * @return pdTRUE/pdFALSE: There is/isn't a loopback address in the packet.
 */
    BaseType_t xCheckLoopback( NetworkBufferDescriptor_t * const pxDescriptor,
                               BaseType_t bReleaseAfterSend )
    {
        BaseType_t xResult = pdFALSE;
        NetworkBufferDescriptor_t * pxUseDescriptor = pxDescriptor;
        const IPPacket_t * pxIPPacket = ipCAST_PTR_TO_TYPE_PTR( IPPacket_t, pxUseDescriptor->pucEthernetBuffer );

        if( pxIPPacket->xEthernetHeader.usFrameType == ipIPv4_FRAME_TYPE )
        {
            if( memcmp( pxIPPacket->xEthernetHeader.xDestinationAddress.ucBytes, ipLOCAL_MAC_ADDRESS, ipMAC_ADDRESS_LENGTH_BYTES ) == 0 )
            {
                xResult = pdTRUE;

                if( bReleaseAfterSend == pdFALSE )
                {
                    /* Driver is not allowed to transfer the ownership
                     * of descriptor,  so make a copy of it */
                    pxUseDescriptor =
                        pxDuplicateNetworkBufferWithDescriptor( pxDescriptor, pxDescriptor->xDataLength );
                }

                if( pxUseDescriptor != NULL )
                {
                    IPStackEvent_t xRxEvent;

                    xRxEvent.eEventType = eNetworkRxEvent;
                    xRxEvent.pvData = pxUseDescriptor;

                    if( xSendEventStructToIPTask( &xRxEvent, 0U ) != pdTRUE )
                    {
                        vReleaseNetworkBufferAndDescriptor( pxUseDescriptor );
                        iptraceETHERNET_RX_EVENT_LOST();
                        FreeRTOS_printf( ( "prvEMACRxPoll: Can not queue return packet!\n" ) );
                    }
                }
            }
        }

        return xResult;
    }

#endif /* 0 */
/*-----------------------------------------------------------*/

#if ( ipconfigHAS_PRINTF != 0 ) || ( ipconfigHAS_DEBUG_PRINTF != 0 )

    void FreeRTOS_PrintARPCache( void )
    {
        BaseType_t x, xCount = 0;

        /* Loop through each entry in the ARP cache. */
        for( x = 0; x < ipconfigARP_CACHE_ENTRIES; x++ )
        {
            if( ( xARPCache[ x ].ulIPAddress != 0UL ) && ( xARPCache[ x ].ucAge > ( uint8_t ) 0U ) )
            {
                /* See if the MAC-address also matches, and we're all happy */
                FreeRTOS_printf( ( "Arp %2ld: %3u - %16lxip : %02x:%02x:%02x : %02x:%02x:%02x\n",
                                   x,
                                   xARPCache[ x ].ucAge,
                                   xARPCache[ x ].ulIPAddress,
                                   xARPCache[ x ].xMACAddress.ucBytes[ 0 ],
                                   xARPCache[ x ].xMACAddress.ucBytes[ 1 ],
                                   xARPCache[ x ].xMACAddress.ucBytes[ 2 ],
                                   xARPCache[ x ].xMACAddress.ucBytes[ 3 ],
                                   xARPCache[ x ].xMACAddress.ucBytes[ 4 ],
                                   xARPCache[ x ].xMACAddress.ucBytes[ 5 ] ) );
                xCount++;
            }
        }

        FreeRTOS_printf( ( "Arp has %ld entries\n", xCount ) );
    }

#endif /* ( ipconfigHAS_PRINTF != 0 ) || ( ipconfigHAS_DEBUG_PRINTF != 0 ) */
