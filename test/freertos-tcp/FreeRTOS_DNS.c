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
 * @file FreeRTOS_DNS.c
 * @brief Implements the Domain Name System for the FreeRTOS+TCP network stack.
 */

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_UDP_IP.h"
#include "FreeRTOS_DNS.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkBufferManagement.h"
#include "NetworkInterface.h"

/* Exclude the entire file if DNS is not enabled. */
#if ( ipconfigUSE_DNS != 0 )

    #if ( ipconfigBYTE_ORDER == pdFREERTOS_LITTLE_ENDIAN )
        #define dnsDNS_PORT             0x3500U  /**< Little endian: Port used for DNS. */
        #define dnsONE_QUESTION         0x0100U  /**< Little endian representation of a DNS question.*/
        #define dnsOUTGOING_FLAGS       0x0001U  /**< Little endian representation of standard query. */
        #define dnsRX_FLAGS_MASK        0x0f80U  /**< Little endian:  The bits of interest in the flags field of incoming DNS messages. */
        #define dnsEXPECTED_RX_FLAGS    0x0080U  /**< Little Endian: Should be a response, without any errors. */
    #else
        #define dnsDNS_PORT             0x0035U  /**< Big endian: Port used for DNS. */
        #define dnsONE_QUESTION         0x0001U  /**< Big endian representation of a DNS question.*/
        #define dnsOUTGOING_FLAGS       0x0100U  /**< Big endian representation of standard query. */
        #define dnsRX_FLAGS_MASK        0x800fU  /**< Big endian: The bits of interest in the flags field of incoming DNS messages. */
        #define dnsEXPECTED_RX_FLAGS    0x8000U  /**< Big endian: Should be a response, without any errors. */

    #endif /* ipconfigBYTE_ORDER */

/** @brief The maximum number of times a DNS request should be sent out if a response
 * is not received, before giving up. */
    #ifndef ipconfigDNS_REQUEST_ATTEMPTS
        #define ipconfigDNS_REQUEST_ATTEMPTS    5
    #endif

/** @brief If the top two bits in the first character of a name field are set then the
 * name field is an offset to the string, rather than the string itself. */
    #define dnsNAME_IS_OFFSET    ( ( uint8_t ) 0xc0 )

/* NBNS flags. */
    #if ( ipconfigUSE_NBNS == 1 )
        #define dnsNBNS_FLAGS_RESPONSE        0x8000U /**< NBNS response flag. */
        #define dnsNBNS_FLAGS_OPCODE_MASK     0x7800U /**< NBNS opcode bitmask. */
        #define dnsNBNS_FLAGS_OPCODE_QUERY    0x0000U /**< NBNS opcode query. */
    #endif /* ( ipconfigUSE_NBNS == 1 ) */

/* Host types. */
    #define dnsTYPE_A_HOST    0x01U /**< DNS type A host. */
    #define dnsCLASS_IN       0x01U /**< DNS class IN (Internet). */

    #ifndef _lint
        /* LLMNR constants. */
        #define dnsLLMNR_TTL_VALUE           300000UL /**< LLMNR time to live value. */
        #define dnsLLMNR_FLAGS_IS_REPONSE    0x8000U  /**< LLMNR flag value for response. */
    #endif /* _lint */

/* NBNS constants. */
    #if ( ipconfigUSE_NBNS != 0 )
        #define dnsNBNS_TTL_VALUE               3600UL  /**< NBNS TTL: 1 hour valid. */
        #define dnsNBNS_TYPE_NET_BIOS           0x0020U /**< NBNS Type: NetBIOS. */
        #define dnsNBNS_CLASS_IN                0x01U   /**< NBNS Class: IN (Internet). */
        #define dnsNBNS_NAME_FLAGS              0x6000U /**< NBNS name flags. */
        #define dnsNBNS_ENCODED_NAME_LENGTH     32      /**< NBNS encoded name length. */

/** @brief If the queried NBNS name matches with the device's name,
 * the query will be responded to with these flags: */
        #define dnsNBNS_QUERY_RESPONSE_FLAGS    ( 0x8500U )
    #endif /* ( ipconfigUSE_NBNS != 0 ) */

/** @brief Flag DNS parsing errors in situations where an IPv4 address is the return
 * type. */
    #define dnsPARSE_ERROR    0UL

    #ifndef _lint
        #if ( ipconfigUSE_DNS_CACHE == 0 )
            #if ( ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY != 1 )
                #error When DNS caching is disabled, please make ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY equal to 1.
            #endif
        #endif
    #endif

/** @brief Define the ASCII value of '.' (Period/Full-stop). */
    #define ASCII_BASELINE_DOT    46U

/*
 * Create a socket and bind it to the standard DNS port number.  Return the
 * the created socket - or NULL if the socket could not be created or bound.
 */
    static Socket_t prvCreateDNSSocket( void );

/*
 * Create the DNS message in the zero copy buffer passed in the first parameter.
 */
    _static size_t prvCreateDNSMessage( uint8_t * pucUDPPayloadBuffer,
                                        const char * pcHostName,
                                        TickType_t uxIdentifier );

/*
 * Simple routine that jumps over the NAME field of a resource record.
 * It returns the number of bytes read.
 */
    _static size_t prvSkipNameField( const uint8_t * pucByte,
                                     size_t uxLength );

/*
 * Process a response packet from a DNS server.
 * The parameter 'xExpected' indicates whether the identifier in the reply
 * was expected, and thus if the DNS cache may be updated with the reply.
 */
    _static uint32_t prvParseDNSReply( uint8_t * pucUDPPayloadBuffer,
                                       size_t uxBufferLength,
                                       BaseType_t xExpected );

/*
 * Check if hostname is already known. If not, call prvGetHostByName() to send a DNS request.
 */
    #if ( ipconfigDNS_USE_CALLBACKS == 1 )
        static uint32_t prvPrepareLookup( const char * pcHostName,
                                          FOnDNSEvent pCallback,
                                          void * pvSearchID,
                                          TickType_t uxTimeout );
    #else
        static uint32_t prvPrepareLookup( const char * pcHostName );
    #endif

/*
 * Prepare and send a message to a DNS server.  'uxReadTimeOut_ticks' will be passed as
 * zero, in case the user has supplied a call-back function.
 */
    static uint32_t prvGetHostByName( const char * pcHostName,
                                      TickType_t uxIdentifier,
                                      TickType_t uxReadTimeOut_ticks );

    #if ( ipconfigDNS_USE_CALLBACKS != 0 )
        static void vDNSSetCallBack( const char * pcHostName,
                                     void * pvSearchID,
                                     FOnDNSEvent pCallbackFunction,
                                     TickType_t uxTimeout,
                                     TickType_t uxIdentifier );
    #endif /* ipconfigDNS_USE_CALLBACKS */

    #if ( ipconfigDNS_USE_CALLBACKS != 0 )
        static BaseType_t xDNSDoCallback( TickType_t uxIdentifier,
                                          const char * pcName,
                                          uint32_t ulIPAddress );
    #endif /* ipconfigDNS_USE_CALLBACKS */

/*
 * The NBNS and the LLMNR protocol share this reply function.
 */
    #if ( ( ipconfigUSE_NBNS == 1 ) || ( ipconfigUSE_LLMNR == 1 ) )
        static void prvReplyDNSMessage( NetworkBufferDescriptor_t * pxNetworkBuffer,
                                        BaseType_t lNetLength );
    #endif

    #if ( ipconfigUSE_NBNS == 1 )
        static portINLINE void prvTreatNBNS( uint8_t * pucPayload,
                                             size_t uxBufferLength,
                                             uint32_t ulIPAddress );
    #endif /* ipconfigUSE_NBNS */


    #if ( ipconfigUSE_DNS_CACHE == 1 ) || ( ipconfigDNS_USE_CALLBACKS == 1 )
        _static size_t prvReadNameField( const uint8_t * pucByte,
                                         size_t uxRemainingBytes,
                                         char * pcName,
                                         size_t uxDestLen );
    #endif /* ipconfigUSE_DNS_CACHE || ipconfigDNS_USE_CALLBACKS */

    #if ( ipconfigUSE_DNS_CACHE == 1 )
        static BaseType_t prvProcessDNSCache( const char * pcName,
                                              uint32_t * pulIP,
                                              uint32_t ulTTL,
                                              BaseType_t xLookUp );

        typedef struct xDNS_CACHE_TABLE_ROW
        {
            uint32_t ulIPAddresses[ ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY ]; /* The IP address(es) of an ARP cache entry. */
            char pcName[ ipconfigDNS_CACHE_NAME_LENGTH ];                    /* The name of the host */
            uint32_t ulTTL;                                                  /* Time-to-Live (in seconds) from the DNS server. */
            uint32_t ulTimeWhenAddedInSeconds;
            #if ( ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY > 1 )
                uint8_t ucNumIPAddresses;
                uint8_t ucCurrentIPAddress;
            #endif
        } DNSCacheRow_t;

        static DNSCacheRow_t xDNSCache[ ipconfigDNS_CACHE_ENTRIES ];

/* Utility function: Clear DNS cache by calling this function. */
        void FreeRTOS_dnsclear( void )
        {
            ( void ) memset( xDNSCache, 0x0, sizeof( xDNSCache ) );
        }
    #endif /* ipconfigUSE_DNS_CACHE == 1 */

    #if ( ipconfigUSE_LLMNR == 1 )
        /** @brief The MAC address used for LLMNR. */
        const MACAddress_t xLLMNR_MacAdress = { { 0x01, 0x00, 0x5e, 0x00, 0x00, 0xfc } };
    #endif /* ipconfigUSE_LLMNR == 1 */

/*-----------------------------------------------------------*/

/* Below #include just tells the compiler to pack the structure.
 * It is included in to make the code more readable */
    #include "pack_struct_start.h"
    struct xDNSMessage
    {
        uint16_t usIdentifier;    /**< Query identifier. Used to match up replies to outstanding queries. */
        uint16_t usFlags;         /**< Flags. */
        uint16_t usQuestions;     /**< Number of questions asked in this query. */
        uint16_t usAnswers;       /**< Number of answers being provided in this query. */
        uint16_t usAuthorityRRs;  /**< Authoritative name server resource records. */
        uint16_t usAdditionalRRs; /**< Additional resource records.*/
    }
    #include "pack_struct_end.h"
    typedef struct xDNSMessage DNSMessage_t;

/**
 * @brief Utility function to cast pointer of a type to pointer of type DNSMessage_t.
 *
 * @return The casted pointer.
 */
    static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( DNSMessage_t )
    {
        return ( DNSMessage_t * ) pvArgument;
    }

/**
 * @brief Utility function to cast a const pointer of a type to a const pointer of type DNSMessage_t.
 *
 * @return The casted pointer.
 */
    static portINLINE ipDECL_CAST_CONST_PTR_FUNC_FOR_TYPE( DNSMessage_t )
    {
        return ( const DNSMessage_t * ) pvArgument;
    }

/* A DNS query consists of a header, as described in 'struct xDNSMessage'
 * It is followed by 1 or more queries, each one consisting of a name and a tail,
 * with two fields: type and class
 */
    #include "pack_struct_start.h"
    struct xDNSTail
    {
        uint16_t usType;  /**< Type of DNS message. */
        uint16_t usClass; /**< Class of DNS message. */
    }
    #include "pack_struct_end.h"
    typedef struct xDNSTail DNSTail_t;

/**
 * @brief Utility function to cast pointer of a type to pointer of type DNSTail_t.
 *
 * @return The casted pointer.
 */
    static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( DNSTail_t )
    {
        return ( DNSTail_t * ) pvArgument;
    }

/* DNS answer record header. */
    #include "pack_struct_start.h"
    struct xDNSAnswerRecord
    {
        uint16_t usType;       /**< Type of DNS answer record. */
        uint16_t usClass;      /**< Class of DNS answer record. */
        uint32_t ulTTL;        /**< Number of seconds the result can be cached. */
        uint16_t usDataLength; /**< Length of the data field. */
    }
    #include "pack_struct_end.h"
    typedef struct xDNSAnswerRecord DNSAnswerRecord_t;

/**
 * @brief Utility function to cast pointer of a type to pointer of type DNSAnswerRecord_t.
 *
 * @return The casted pointer.
 */
    static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( DNSAnswerRecord_t )
    {
        return ( DNSAnswerRecord_t * ) pvArgument;
    }

    #if ( ipconfigUSE_LLMNR == 1 )

        #include "pack_struct_start.h"
        struct xLLMNRAnswer
        {
            uint8_t ucNameCode;    /**< Name type. */
            uint8_t ucNameOffset;  /**< The name is not repeated in the answer, only the offset is given with "0xc0 <offs>" */
            uint16_t usType;       /**< Type of the Resource record. */
            uint16_t usClass;      /**< Class of the Resource record. */
            uint32_t ulTTL;        /**< Seconds till this entry can be cached. */
            uint16_t usDataLength; /**< Length of the address in this record. */
            uint32_t ulIPAddress;  /**< The IP-address. */
        }
        #include "pack_struct_end.h"
        typedef struct xLLMNRAnswer LLMNRAnswer_t;

/**
 * @brief Utility function to cast pointer of a type to pointer of type LLMNRAnswer_t.
 *
 * @return The casted pointer.
 */
        static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( LLMNRAnswer_t )
        {
            return ( LLMNRAnswer_t * ) pvArgument;
        }


    #endif /* ipconfigUSE_LLMNR == 1 */

    #if ( ipconfigUSE_NBNS == 1 )

        #include "pack_struct_start.h"
        struct xNBNSRequest
        {
            uint16_t usRequestId;                          /**< NBNS request ID. */
            uint16_t usFlags;                              /**< Flags of the DNS message. */
            uint16_t ulRequestCount;                       /**< The number of requests/questions in this query. */
            uint16_t usAnswerRSS;                          /**< The number of answers in this query. */
            uint16_t usAuthRSS;                            /**< Number of authoritative resource records. */
            uint16_t usAdditionalRSS;                      /**< Number of additional resource records. */
            uint8_t ucNameSpace;                           /**< Length of name. */
            uint8_t ucName[ dnsNBNS_ENCODED_NAME_LENGTH ]; /**< The domain name. */
            uint8_t ucNameZero;                            /**< Terminator of the name. */
            uint16_t usType;                               /**< Type of NBNS record. */
            uint16_t usClass;                              /**< Class of NBNS request. */
        }
        #include "pack_struct_end.h"
        typedef struct xNBNSRequest NBNSRequest_t;

        #include "pack_struct_start.h"
        struct xNBNSAnswer
        {
            uint16_t usType;       /**< Type of NBNS answer. */
            uint16_t usClass;      /**< Class of NBNS answer. */
            uint32_t ulTTL;        /**< Time in seconds for which the answer can be cached. */
            uint16_t usDataLength; /**< Data length. */
            uint16_t usNbFlags;    /**< NetBIOS flags 0x6000 : IP-address, big-endian. */
            uint32_t ulIPAddress;  /**< The IPv4 address. */
        }
        #include "pack_struct_end.h"
        typedef struct xNBNSAnswer NBNSAnswer_t;

/**
 * @brief Utility function to cast pointer of a type to pointer of type NBNSAnswer_t.
 *
 * @return The casted pointer.
 */
        static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( NBNSAnswer_t )
        {
            return ( NBNSAnswer_t * ) pvArgument;
        }

    #endif /* ipconfigUSE_NBNS == 1 */

/*-----------------------------------------------------------*/

    #if ( ipconfigUSE_DNS_CACHE == 1 )
        uint32_t FreeRTOS_dnslookup( const char * pcHostName )
        {
            uint32_t ulIPAddress = 0UL;

            ( void ) prvProcessDNSCache( pcHostName, &ulIPAddress, 0, pdTRUE );
            return ulIPAddress;
        }
    #endif /* ipconfigUSE_DNS_CACHE == 1 */
    /*-----------------------------------------------------------*/

    #if ( ipconfigDNS_USE_CALLBACKS == 1 )

/** @brief The structure to hold information for a DNS callback. */
        typedef struct xDNS_Callback
        {
            TickType_t uxRemaningTime;     /**< Timeout in ms */
            FOnDNSEvent pCallbackFunction; /**< Function to be called when the address has been found or when a timeout has been reached */
            TimeOut_t uxTimeoutState;      /**< Timeout state. */
            void * pvSearchID;             /**< Search ID of the callback function. */
            struct xLIST_ITEM xListItem;   /**< List struct. */
            char pcName[ 1 ];              /**< 1 character name. */
        } DNSCallback_t;


/**
 * @brief Utility function to cast pointer of a type to pointer of type DNSCallback_t.
 *
 * @return The casted pointer.
 */
        static portINLINE ipDECL_CAST_PTR_FUNC_FOR_TYPE( DNSCallback_t )
        {
            return ( DNSCallback_t * ) pvArgument;
        }

/** @brief The list of all callback structures. */
        static List_t xCallbackList;

/**
 * @brief Define FreeRTOS_gethostbyname() as a normal blocking call.
 *
 * @param[in] pcHostName: The hostname whose IP address is being searched for.
 *
 * @return The IP-address of the hostname.
 */
        uint32_t FreeRTOS_gethostbyname( const char * pcHostName )
        {
            return FreeRTOS_gethostbyname_a( pcHostName, NULL, ( void * ) NULL, 0U );
        }
        /*-----------------------------------------------------------*/

/** @brief Initialise the list of call-back structures.
 */
        void vDNSInitialise( void )
        {
            vListInitialise( &xCallbackList );
        }
        /*-----------------------------------------------------------*/

/**
 * @brief Iterate through the list of call-back structures and remove
 *        old entries which have reached a timeout.
 *        As soon as the list has become empty, the DNS timer will be stopped.
 *        In case pvSearchID is supplied, the user wants to cancel a DNS request.
 *
 * @param[in] pvSearchID: The search ID of callback function whose associated
 *                 DNS request is being cancelled. If non-ID specific checking of
 *                 all requests is required, then this field should be kept as NULL.
 */
        void vDNSCheckCallBack( void * pvSearchID )
        {
            const ListItem_t * pxIterator;
            const ListItem_t * xEnd = listGET_END_MARKER( &xCallbackList );

            vTaskSuspendAll();
            {
                for( pxIterator = ( const ListItem_t * ) listGET_NEXT( xEnd );
                     pxIterator != xEnd;
                     )
                {
                    DNSCallback_t * pxCallback = ipCAST_PTR_TO_TYPE_PTR( DNSCallback_t, listGET_LIST_ITEM_OWNER( pxIterator ) );
                    /* Move to the next item because we might remove this item */
                    pxIterator = ( const ListItem_t * ) listGET_NEXT( pxIterator );

                    if( ( pvSearchID != NULL ) && ( pvSearchID == pxCallback->pvSearchID ) )
                    {
                        ( void ) uxListRemove( &( pxCallback->xListItem ) );
                        vPortFree( pxCallback );
                    }
                    else if( xTaskCheckForTimeOut( &pxCallback->uxTimeoutState, &pxCallback->uxRemaningTime ) != pdFALSE )
                    {
                        pxCallback->pCallbackFunction( pxCallback->pcName, pxCallback->pvSearchID, 0 );
                        ( void ) uxListRemove( &( pxCallback->xListItem ) );
                        vPortFree( pxCallback );
                    }
                    else
                    {
                        /* This call-back is still waiting for a reply or a time-out. */
                    }
                }
            }
            ( void ) xTaskResumeAll();

            if( listLIST_IS_EMPTY( &xCallbackList ) != pdFALSE )
            {
                vIPSetDnsTimerEnableState( pdFALSE );
            }
        }
        /*-----------------------------------------------------------*/

/**
 * @brief Remove the entry defined by the search ID to cancel a DNS request.
 *
 * @param[in] pvSearchID: The search ID of the callback function associated with
 *                        the DNS request being cancelled. Note that the value of
 *                        the pointer matters, not the pointee.
 */
        void FreeRTOS_gethostbyname_cancel( void * pvSearchID )
        {
            /* _HT_ Should better become a new API call to have the IP-task remove the callback */
            vDNSCheckCallBack( pvSearchID );
        }
        /*-----------------------------------------------------------*/

/**
 * @brief FreeRTOS_gethostbyname_a() was called along with callback parameters.
 *        Store them in a list for later reference.
 *
 * @param[in] pcHostName: The hostname whose IP address is being searched for.
 * @param[in] pvSearchID: The search ID of the DNS callback function to set.
 * @param[in] pCallbackFunction: The callback function pointer.
 * @param[in] uxTimeout: Timeout of the callback function.
 * @param[in] uxIdentifier: Random number used as ID in the DNS message.
 */
        static void vDNSSetCallBack( const char * pcHostName,
                                     void * pvSearchID,
                                     FOnDNSEvent pCallbackFunction,
                                     TickType_t uxTimeout,
                                     TickType_t uxIdentifier )
        {
            size_t lLength = strlen( pcHostName );
            DNSCallback_t * pxCallback = ipCAST_PTR_TO_TYPE_PTR( DNSCallback_t, pvPortMalloc( sizeof( *pxCallback ) + lLength ) );

            /* Translate from ms to number of clock ticks. */
            uxTimeout /= portTICK_PERIOD_MS;

            if( pxCallback != NULL )
            {
                if( listLIST_IS_EMPTY( &xCallbackList ) != pdFALSE )
                {
                    /* This is the first one, start the DNS timer to check for timeouts */
                    vIPReloadDNSTimer( FreeRTOS_min_uint32( 1000U, uxTimeout ) );
                }

                ( void ) strcpy( pxCallback->pcName, pcHostName );
                pxCallback->pCallbackFunction = pCallbackFunction;
                pxCallback->pvSearchID = pvSearchID;
                pxCallback->uxRemaningTime = uxTimeout;
                vTaskSetTimeOutState( &pxCallback->uxTimeoutState );
                listSET_LIST_ITEM_OWNER( &( pxCallback->xListItem ), ( void * ) pxCallback );
                listSET_LIST_ITEM_VALUE( &( pxCallback->xListItem ), uxIdentifier );
                vTaskSuspendAll();
                {
                    vListInsertEnd( &xCallbackList, &pxCallback->xListItem );
                }
                ( void ) xTaskResumeAll();
            }
        }
        /*-----------------------------------------------------------*/

/**
 * @brief A DNS reply was received, see if there is any matching entry and
 *        call the handler.
 *
 * @param[in] uxIdentifier: Identifier associated with the callback function.
 * @param[in] pcName: The name associated with the callback function.
 * @param[in] ulIPAddress: IP-address obtained from the DNS server.
 *
 * @return Returns pdTRUE if uxIdentifier was recognized.
 */
        static BaseType_t xDNSDoCallback( TickType_t uxIdentifier,
                                          const char * pcName,
                                          uint32_t ulIPAddress )
        {
            BaseType_t xResult = pdFALSE;
            const ListItem_t * pxIterator;
            const ListItem_t * xEnd = listGET_END_MARKER( &xCallbackList );

            vTaskSuspendAll();
            {
                for( pxIterator = ( const ListItem_t * ) listGET_NEXT( xEnd );
                     pxIterator != ( const ListItem_t * ) xEnd;
                     pxIterator = ( const ListItem_t * ) listGET_NEXT( pxIterator ) )
                {
                    if( listGET_LIST_ITEM_VALUE( pxIterator ) == uxIdentifier )
                    {
                        DNSCallback_t * pxCallback = ipCAST_PTR_TO_TYPE_PTR( DNSCallback_t, listGET_LIST_ITEM_OWNER( pxIterator ) );

                        pxCallback->pCallbackFunction( pcName, pxCallback->pvSearchID, ulIPAddress );
                        ( void ) uxListRemove( &pxCallback->xListItem );
                        vPortFree( pxCallback );

                        if( listLIST_IS_EMPTY( &xCallbackList ) != pdFALSE )
                        {
                            /* The list of outstanding requests is empty. No need for periodic polling. */
                            vIPSetDnsTimerEnableState( pdFALSE );
                        }

                        xResult = pdTRUE;
                        break;
                    }
                }
            }
            ( void ) xTaskResumeAll();
            return xResult;
        }

    #endif /* ipconfigDNS_USE_CALLBACKS == 1 */
    /*-----------------------------------------------------------*/

    #if ( ipconfigDNS_USE_CALLBACKS == 0 )

/**
 * @brief Get the IP-address corresponding to the given hostname.
 *
 * @param[in] pcHostName: The hostname whose IP address is being queried.
 *
 * @return The IP-address corresponding to the hostname.
 */
        uint32_t FreeRTOS_gethostbyname( const char * pcHostName )
        {
            return prvPrepareLookup( pcHostName );
        }
    #else

/**
 * @brief Get the IP-address corresponding to the given hostname.
 *
 * @param[in] pcHostName: The hostname whose IP address is being queried.
 * @param[in] pCallback: The callback function which will be called upon DNS response.
 * @param[in] pvSearchID: Search ID for the callback function.
 * @param[in] uxTimeout: Timeout for the callback function.
 *
 * @return The IP-address corresponding to the hostname.
 */
        uint32_t FreeRTOS_gethostbyname_a( const char * pcHostName,
                                           FOnDNSEvent pCallback,
                                           void * pvSearchID,
                                           TickType_t uxTimeout )
        {
            return prvPrepareLookup( pcHostName, pCallback, pvSearchID, uxTimeout );
        }
    #endif /* if ( ipconfigDNS_USE_CALLBACKS == 0 ) */

    #if ( ipconfigDNS_USE_CALLBACKS == 1 )

/**
 * @brief Check if hostname is already known. If not, call prvGetHostByName() to send a DNS request.
 *
 * @param[in] pcHostName: The hostname whose IP address is being queried.
 * @param[in] pCallback: The callback function which will be called upon DNS response.
 * @param[in] pvSearchID: Search ID for the callback function.
 * @param[in] uxTimeout: Timeout for the callback function.
 *
 * @return The IP-address corresponding to the hostname.
 */
        static uint32_t prvPrepareLookup( const char * pcHostName,
                                          FOnDNSEvent pCallback,
                                          void * pvSearchID,
                                          TickType_t uxTimeout )
    #else

/**
 * @brief Check if hostname is already known. If not, call prvGetHostByName() to send a DNS request.
 *
 * @param[in] pcHostName: The hostname whose IP address is being queried.
 *
 * @return The IP-address corresponding to the hostname.
 */
        static uint32_t prvPrepareLookup( const char * pcHostName )
    #endif
    {
        uint32_t ulIPAddress = 0UL;
        TickType_t uxReadTimeOut_ticks = ipconfigDNS_RECEIVE_BLOCK_TIME_TICKS;

        /* Generate a unique identifier for this query. Keep it in a local variable
         * as gethostbyname() may be called from different threads */
        BaseType_t xHasRandom = pdFALSE;
        TickType_t uxIdentifier = 0U;

        #if ( ipconfigUSE_DNS_CACHE != 0 )
            BaseType_t xLengthOk = pdFALSE;
        #endif

        #if ( ipconfigUSE_DNS_CACHE != 0 )
            {
                if( pcHostName != NULL )
                {
                    size_t xLength = strlen( pcHostName ) + 1U;

                    if( xLength <= ipconfigDNS_CACHE_NAME_LENGTH )
                    {
                        /* The name is not too long. */
                        xLengthOk = pdTRUE;
                    }
                    else
                    {
                        FreeRTOS_printf( ( "prvPrepareLookup: name is too long ( %lu > %lu )\n",
                                           ( uint32_t ) xLength,
                                           ( uint32_t ) ipconfigDNS_CACHE_NAME_LENGTH ) );
                    }
                }
            }

            if( ( pcHostName != NULL ) && ( xLengthOk != pdFALSE ) )
        #else /* if ( ipconfigUSE_DNS_CACHE != 0 ) */
            if( pcHostName != NULL )
        #endif /* ( ipconfigUSE_DNS_CACHE != 0 ) */
        {
            /* If the supplied hostname is IP address, convert it to uint32_t
             * and return. */
            #if ( ipconfigINCLUDE_FULL_INET_ADDR == 1 )
                {
                    ulIPAddress = FreeRTOS_inet_addr( pcHostName );
                }
            #endif /* ipconfigINCLUDE_FULL_INET_ADDR == 1 */

            /* If a DNS cache is used then check the cache before issuing another DNS
             * request. */
            #if ( ipconfigUSE_DNS_CACHE == 1 )
                {
                    if( ulIPAddress == 0UL )
                    {
                        ulIPAddress = FreeRTOS_dnslookup( pcHostName );

                        if( ulIPAddress != 0UL )
                        {
                            FreeRTOS_debug_printf( ( "FreeRTOS_gethostbyname: found '%s' in cache: %lxip\n", pcHostName, ulIPAddress ) );
                        }
                        else
                        {
                            /* prvGetHostByName will be called to start a DNS lookup. */
                        }
                    }
                }
            #endif /* ipconfigUSE_DNS_CACHE == 1 */

            /* Generate a unique identifier. */
            if( ulIPAddress == 0UL )
            {
                uint32_t ulNumber;

                xHasRandom = xApplicationGetRandomNumber( &( ulNumber ) );
                /* DNS identifiers are 16-bit. */
                uxIdentifier = ( TickType_t ) ( ulNumber & 0xffffU );
            }

            #if ( ipconfigDNS_USE_CALLBACKS == 1 )
                {
                    if( pCallback != NULL )
                    {
                        if( ulIPAddress == 0UL )
                        {
                            /* The user has provided a callback function, so do not block on recvfrom() */
                            if( xHasRandom != pdFALSE )
                            {
                                uxReadTimeOut_ticks = 0U;
                                vDNSSetCallBack( pcHostName, pvSearchID, pCallback, uxTimeout, uxIdentifier );
                            }
                        }
                        else
                        {
                            /* The IP address is known, do the call-back now. */
                            pCallback( pcHostName, pvSearchID, ulIPAddress );
                        }
                    }
                }
            #endif /* if ( ipconfigDNS_USE_CALLBACKS == 1 ) */

            if( ( ulIPAddress == 0UL ) && ( xHasRandom != pdFALSE ) )
            {
                ulIPAddress = prvGetHostByName( pcHostName, uxIdentifier, uxReadTimeOut_ticks );
            }
        }

        return ulIPAddress;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Prepare and send a message to a DNS server.  'uxReadTimeOut_ticks' will be passed as
 * zero, in case the user has supplied a call-back function.
 *
 * @param[in] pcHostName: The hostname for which an IP address is required.
 * @param[in] uxIdentifier: Identifier to send in the DNS message.
 * @param[in] uxReadTimeOut_ticks: The timeout in ticks for waiting. In case the user has supplied
 *                                 a call-back function, this value should be zero.
 *
 * @return The IPv4 IP address for the hostname being queried. It will be zero if there is no reply.
 */
    static uint32_t prvGetHostByName( const char * pcHostName,
                                      TickType_t uxIdentifier,
                                      TickType_t uxReadTimeOut_ticks )
    {
        struct freertos_sockaddr xAddress;
        Socket_t xDNSSocket;
        uint32_t ulIPAddress = 0UL;
        uint32_t ulAddressLength = sizeof( struct freertos_sockaddr );
        BaseType_t xAttempt;
        int32_t lBytes;
        size_t uxPayloadLength, uxExpectedPayloadLength;
        TickType_t uxWriteTimeOut_ticks = ipconfigDNS_SEND_BLOCK_TIME_TICKS;

        #if ( ipconfigUSE_LLMNR == 1 )
            BaseType_t bHasDot = pdFALSE;
        #endif /* ipconfigUSE_LLMNR == 1 */

        /* If LLMNR is being used then determine if the host name includes a '.' -
         * if not then LLMNR can be used as the lookup method. */
        #if ( ipconfigUSE_LLMNR == 1 )
            {
                const char * pucPtr;

                for( pucPtr = pcHostName; *pucPtr != ( char ) 0; pucPtr++ )
                {
                    if( *pucPtr == '.' )
                    {
                        bHasDot = pdTRUE;
                        break;
                    }
                }
            }
        #endif /* ipconfigUSE_LLMNR == 1 */

        /* Two is added at the end for the count of characters in the first
         * subdomain part and the string end byte. */
        uxExpectedPayloadLength = sizeof( DNSMessage_t ) + strlen( pcHostName ) + sizeof( uint16_t ) + sizeof( uint16_t ) + 2U;

        xDNSSocket = prvCreateDNSSocket();

        if( xDNSSocket != NULL )
        {
            /* Ideally we should check for the return value. But since we are passing
             * correct parameters, and xDNSSocket is != NULL, the return value is
             * going to be '0' i.e. success. Thus, return value is discarded */
            ( void ) FreeRTOS_setsockopt( xDNSSocket, 0, FREERTOS_SO_SNDTIMEO, &( uxWriteTimeOut_ticks ), sizeof( TickType_t ) );
            ( void ) FreeRTOS_setsockopt( xDNSSocket, 0, FREERTOS_SO_RCVTIMEO, &( uxReadTimeOut_ticks ), sizeof( TickType_t ) );

            for( xAttempt = 0; xAttempt < ipconfigDNS_REQUEST_ATTEMPTS; xAttempt++ )
            {
                size_t uxHeaderBytes;
                NetworkBufferDescriptor_t * pxNetworkBuffer;
                uint8_t * pucUDPPayloadBuffer = NULL, * pucReceiveBuffer;

                /* Get a buffer.  This uses a maximum delay, but the delay will be
                 * capped to ipconfigUDP_MAX_SEND_BLOCK_TIME_TICKS so the return value
                 * still needs to be tested. */

                uxHeaderBytes = ipSIZE_OF_ETH_HEADER + ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_UDP_HEADER;

                pxNetworkBuffer = pxGetNetworkBufferWithDescriptor( uxHeaderBytes + uxExpectedPayloadLength, 0UL );

                if( pxNetworkBuffer != NULL )
                {
                    pucUDPPayloadBuffer = &( pxNetworkBuffer->pucEthernetBuffer[ uxHeaderBytes ] );
                }

                if( pucUDPPayloadBuffer != NULL )
                {
                    /* Create the message in the obtained buffer. */
                    uxPayloadLength = prvCreateDNSMessage( pucUDPPayloadBuffer, pcHostName, uxIdentifier );

                    iptraceSENDING_DNS_REQUEST();

                    /* Obtain the DNS server address. */
                    FreeRTOS_GetAddressConfiguration( NULL, NULL, NULL, &ulIPAddress );

                    /* Send the DNS message. */
                    #if ( ipconfigUSE_LLMNR == 1 )
                        if( bHasDot == pdFALSE )
                        {
                            /* Use LLMNR addressing. */
                            ( ipCAST_PTR_TO_TYPE_PTR( DNSMessage_t, pucUDPPayloadBuffer ) )->usFlags = 0;
                            xAddress.sin_addr = ipLLMNR_IP_ADDR; /* Is in network byte order. */
                            xAddress.sin_port = ipLLMNR_PORT;
                            xAddress.sin_port = FreeRTOS_ntohs( xAddress.sin_port );
                        }
                        else
                    #endif
                    {
                        /* Use DNS server. */
                        xAddress.sin_addr = ulIPAddress;
                        xAddress.sin_port = dnsDNS_PORT;
                    }

                    ulIPAddress = 0UL;

                    if( FreeRTOS_sendto( xDNSSocket, pucUDPPayloadBuffer, uxPayloadLength, FREERTOS_ZERO_COPY, &xAddress, sizeof( xAddress ) ) != 0 )
                    {
                        /* Wait for the reply. */
                        lBytes = FreeRTOS_recvfrom( xDNSSocket, &pucReceiveBuffer, 0, FREERTOS_ZERO_COPY, &xAddress, &ulAddressLength );

                        if( lBytes > 0 )
                        {
                            BaseType_t xExpected;
                            const DNSMessage_t * pxDNSMessageHeader = ipCAST_CONST_PTR_TO_CONST_TYPE_PTR( DNSMessage_t, pucReceiveBuffer );

                            /* See if the identifiers match. */
                            if( uxIdentifier == ( TickType_t ) pxDNSMessageHeader->usIdentifier )
                            {
                                xExpected = pdTRUE;
                            }
                            else
                            {
                                /* The reply was not expected. */
                                xExpected = pdFALSE;
                            }

                            /* The reply was received.  Process it. */
                            #if ( ipconfigDNS_USE_CALLBACKS == 0 )

                                /* It is useless to analyse the unexpected reply
                                 * unless asynchronous look-ups are enabled. */
                                if( xExpected != pdFALSE )
                            #endif /* ipconfigDNS_USE_CALLBACKS == 0 */
                            {
                                ulIPAddress = prvParseDNSReply( pucReceiveBuffer, ( size_t ) lBytes, xExpected );
                            }

                            /* Finished with the buffer.  The zero copy interface
                             * is being used, so the buffer must be freed by the
                             * task. */
                            FreeRTOS_ReleaseUDPPayloadBuffer( pucReceiveBuffer );

                            if( ulIPAddress != 0UL )
                            {
                                /* All done. */
                                /* coverity[break_stmt] : Break statement terminating the loop */
                                break;
                            }
                        }
                    }
                    else
                    {
                        /* The message was not sent so the stack will not be
                         * releasing the zero copy - it must be released here. */
                        vReleaseNetworkBufferAndDescriptor( pxNetworkBuffer );
                    }
                }

                if( uxReadTimeOut_ticks == 0U )
                {
                    /* This DNS lookup is asynchronous, using a call-back:
                     * send the request only once. */
                    break;
                }
            }

            /* Finished with the socket. */
            ( void ) FreeRTOS_closesocket( xDNSSocket );
        }

        return ulIPAddress;
    }
    /*-----------------------------------------------------------*/

/**
 * @brief Create the DNS message in the zero copy buffer passed in the first parameter.
 *
 * @param[in,out] pucUDPPayloadBuffer: The zero copy buffer where the DNS message will be created.
 * @param[in] pcHostName: Hostname to be looked up.
 * @param[in] uxIdentifier: The identifier to be added to the DNS message.
 *
 * @return Total size of the generated message, which is the space from the last written byte
 *         to the beginning of the buffer.
 */
    _static size_t prvCreateDNSMessage( uint8_t * pucUDPPayloadBuffer,
                                        const char * pcHostName,
                                        TickType_t uxIdentifier )
    {
        DNSMessage_t * pxDNSMessageHeader;
        size_t uxStart, uxIndex;
        DNSTail_t const * pxTail;
        static const DNSMessage_t xDefaultPartDNSHeader =
        {
            0,                 /* The identifier will be overwritten. */
            dnsOUTGOING_FLAGS, /* Flags set for standard query. */
            dnsONE_QUESTION,   /* One question is being asked. */
            0,                 /* No replies are included. */
            0,                 /* No authorities. */
            0                  /* No additional authorities. */
        };

        /* memcpy() helper variables for MISRA Rule 21.15 compliance*/
        const void * pvCopySource;
        void * pvCopyDest;

        /* Copy in the const part of the header. Intentionally using different
         * pointers with memcpy() to put the information in to correct place. */

        /*
         * Use helper variables for memcpy() to remain
         * compliant with MISRA Rule 21.15.  These should be
         * optimized away.
         */
        pvCopySource = &xDefaultPartDNSHeader;
        pvCopyDest = pucUDPPayloadBuffer;
        ( void ) memcpy( pvCopyDest, pvCopySource, sizeof( xDefaultPartDNSHeader ) );

        /* Write in a unique identifier. Cast the Payload Buffer to DNSMessage_t
         * to easily access fields of the DNS Message. */
        pxDNSMessageHeader = ipCAST_PTR_TO_TYPE_PTR( DNSMessage_t, pucUDPPayloadBuffer );
        pxDNSMessageHeader->usIdentifier = ( uint16_t ) uxIdentifier;

        /* Create the resource record at the end of the header.  First
         * find the end of the header. */
        uxStart = sizeof( xDefaultPartDNSHeader );

        /* Leave a gap for the first length byte. */
        uxIndex = uxStart + 1U;

        /* Copy in the host name. */
        ( void ) strcpy( ( char * ) &( pucUDPPayloadBuffer[ uxIndex ] ), pcHostName );

        /* Walk through the string to replace the '.' characters with byte
         * counts.  pucStart holds the address of the byte count.  Walking the
         * string starts after the byte count position. */
        uxIndex = uxStart;

        do
        {
            size_t uxLength;

            /* Skip the length byte. */
            uxIndex++;

            while( ( pucUDPPayloadBuffer[ uxIndex ] != ( uint8_t ) 0U ) &&
                   ( pucUDPPayloadBuffer[ uxIndex ] != ( uint8_t ) ASCII_BASELINE_DOT ) )
            {
                uxIndex++;
            }

            /* Fill in the byte count, then move the pucStart pointer up to
             * the found byte position. */
            uxLength = uxIndex - ( uxStart + 1U );
            pucUDPPayloadBuffer[ uxStart ] = ( uint8_t ) uxLength;

            uxStart = uxIndex;
        } while( pucUDPPayloadBuffer[ uxIndex ] != ( uint8_t ) 0U );

        /* Finish off the record. Cast the record onto DNSTail_t structure to easily
         * access the fields of the DNS Message. */
        pxTail = ipCAST_PTR_TO_TYPE_PTR( DNSTail_t, &( pucUDPPayloadBuffer[ uxStart + 1U ] ) );

        #if defined( _lint ) || defined( __COVERITY__ )
            ( void ) pxTail;
        #else
            vSetField16( pxTail, DNSTail_t, usType, dnsTYPE_A_HOST );
            vSetField16( pxTail, DNSTail_t, usClass, dnsCLASS_IN );
        #endif

        /* Return the total size of the generated message, which is the space from
         * the last written byte to the beginning of the buffer. */
        return uxIndex + sizeof( DNSTail_t ) + 1U;
    }
    /*-----------------------------------------------------------*/

    #if ( ipconfigUSE_DNS_CACHE == 1 ) || ( ipconfigDNS_USE_CALLBACKS == 1 )

/**
 * @brief Read the Name field out of a DNS response packet.
 *
 * @param[in] pucByte: Pointer to the DNS response.
 * @param[in] uxRemainingBytes: Length of the DNS response.
 * @param[out] pcName: The pointer in which the name in the DNS response will be returned.
 * @param[in] uxDestLen: Size of the pcName array.
 *
 * @return If a fully formed name was found, then return the number of bytes processed in pucByte.
 */
        _static size_t prvReadNameField( const uint8_t * pucByte,
                                         size_t uxRemainingBytes,
                                         char * pcName,
                                         size_t uxDestLen )
        {
            size_t uxNameLen = 0U;
            size_t uxIndex = 0U;
            size_t uxSourceLen = uxRemainingBytes;

            /* uxCount gets the values from pucByte and counts down to 0.
             * No need to have a different type than that of pucByte */
            size_t uxCount;

            if( uxSourceLen == ( size_t ) 0U )
            {
                /* Return 0 value in case of error. */
                uxIndex = 0U;
            }

            /* Determine if the name is the fully coded name, or an offset to the name
             * elsewhere in the message. */
            else if( ( pucByte[ uxIndex ] & dnsNAME_IS_OFFSET ) == dnsNAME_IS_OFFSET )
            {
                /* Jump over the two byte offset. */
                if( uxSourceLen > sizeof( uint16_t ) )
                {
                    uxIndex += sizeof( uint16_t );
                }
                else
                {
                    uxIndex = 0U;
                }
            }
            else
            {
                /* 'uxIndex' points to the full name. Walk over the string. */
                while( ( uxIndex < uxSourceLen ) && ( pucByte[ uxIndex ] != ( uint8_t ) 0x00U ) )
                {
                    /* If this is not the first time through the loop, then add a
                     * separator in the output. */
                    if( ( uxNameLen > 0U ) )
                    {
                        if( uxNameLen >= uxDestLen )
                        {
                            uxIndex = 0U;
                            /* coverity[break_stmt] : Break statement terminating the loop */
                            break;
                        }

                        pcName[ uxNameLen ] = '.';
                        uxNameLen++;
                    }

                    /* Process the first/next sub-string. */
                    uxCount = ( size_t ) pucByte[ uxIndex ];
                    uxIndex++;

                    if( ( uxIndex + uxCount ) > uxSourceLen )
                    {
                        uxIndex = 0U;
                        break;
                    }

                    while( ( uxCount-- != 0U ) && ( uxIndex < uxSourceLen ) )
                    {
                        if( uxNameLen >= uxDestLen )
                        {
                            uxIndex = 0U;
                            break;

                            /* break out of inner loop here
                             * break out of outer loop at the test uxNameLen >= uxDestLen. */
                        }

                        pcName[ uxNameLen ] = ( char ) pucByte[ uxIndex ];
                        uxNameLen++;
                        uxIndex++;
                    }
                }

                /* Confirm that a fully formed name was found. */
                if( uxIndex > 0U )
                {
                    if( ( uxNameLen < uxDestLen ) && ( uxIndex < uxSourceLen ) && ( pucByte[ uxIndex ] == 0U ) )
                    {
                        pcName[ uxNameLen ] = '\0';
                        uxIndex++;
                    }
                    else
                    {
                        uxIndex = 0U;
                    }
                }
            }

            return uxIndex;
        }
    #endif /* ipconfigUSE_DNS_CACHE || ipconfigDNS_USE_CALLBACKS */
    /*-----------------------------------------------------------*/

/**
 * @brief Simple routine that jumps over the NAME field of a resource record.
 *
 * @param[in] pucByte: The pointer to the resource record.
 * @param[in] uxLength: Length of the resource record.
 *
 * @return It returns the number of bytes read, or zero when an error has occurred.
 */
    _static size_t prvSkipNameField( const uint8_t * pucByte,
                                     size_t uxLength )
    {
        size_t uxChunkLength;
        size_t uxSourceLenCpy = uxLength;
        size_t uxIndex = 0U;

        if( uxSourceLenCpy == 0U )
        {
            uxIndex = 0U;
        }

        /* Determine if the name is the fully coded name, or an offset to the name
         * elsewhere in the message. */
        else if( ( pucByte[ uxIndex ] & dnsNAME_IS_OFFSET ) == dnsNAME_IS_OFFSET )
        {
            /* Jump over the two byte offset. */
            if( uxSourceLenCpy > sizeof( uint16_t ) )
            {
                uxIndex += sizeof( uint16_t );
            }
            else
            {
                uxIndex = 0U;
            }
        }
        else
        {
            /* pucByte points to the full name. Walk over the string. */
            while( ( pucByte[ uxIndex ] != 0U ) && ( uxSourceLenCpy > 1U ) )
            {
                /* Conversion to size_t causes addition to be done
                 * in size_t */
                uxChunkLength = ( ( size_t ) pucByte[ uxIndex ] ) + 1U;

                if( uxSourceLenCpy > uxChunkLength )
                {
                    uxSourceLenCpy -= uxChunkLength;
                    uxIndex += uxChunkLength;
                }
                else
                {
                    uxIndex = 0U;
                    break;
                }
            }

            /* Confirm that a fully formed name was found. */
            if( uxIndex > 0U )
            {
                if( pucByte[ uxIndex ] == 0U )
                {
                    uxIndex++;
                }
                else
                {
                    uxIndex = 0U;
                }
            }
        }

        return uxIndex;
    }
    /*-----------------------------------------------------------*/

/* The function below will only be called :
 * when ipconfigDNS_USE_CALLBACKS == 1
 * when ipconfigUSE_LLMNR == 1
 * for testing purposes, by the module test_freertos_tcp.c
 */

/**
 * @brief Perform some preliminary checks and then parse the DNS packet.
 *
 * @param[in] pxNetworkBuffer: The network buffer to be parsed.
 *
 * @return Always pdFAIL to indicate that the packet was not consumed and must
 *         be released by the caller.
 */
    uint32_t ulDNSHandlePacket( const NetworkBufferDescriptor_t * pxNetworkBuffer )
    {
        uint8_t * pucPayLoadBuffer;
        size_t uxPayloadSize;

        /* Only proceed if the payload length indicated in the header
         * appears to be valid. */
        if( pxNetworkBuffer->xDataLength >= sizeof( UDPPacket_t ) )
        {
            uxPayloadSize = pxNetworkBuffer->xDataLength - sizeof( UDPPacket_t );

            if( uxPayloadSize >= sizeof( DNSMessage_t ) )
            {
                pucPayLoadBuffer = &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( UDPPacket_t ) ] );

                /* The parameter pdFALSE indicates that the reply was not expected. */
                ( void ) prvParseDNSReply( pucPayLoadBuffer,
                                           uxPayloadSize,
                                           pdFALSE );
            }
        }

        /* The packet was not consumed. */
        return pdFAIL;
    }
    /*-----------------------------------------------------------*/


    #if ( ipconfigUSE_NBNS == 1 )

/**
 * @brief Handle an NBNS packet.
 *
 * @param[in] pxNetworkBuffer: The network buffer holding the NBNS packet.
 *
 * @return pdFAIL to show that the packet was not consumed.
 */
        uint32_t ulNBNSHandlePacket( NetworkBufferDescriptor_t * pxNetworkBuffer )
        {
            UDPPacket_t * pxUDPPacket = ipCAST_PTR_TO_TYPE_PTR( UDPPacket_t, pxNetworkBuffer->pucEthernetBuffer );
            uint8_t * pucUDPPayloadBuffer = &( pxNetworkBuffer->pucEthernetBuffer[ sizeof( *pxUDPPacket ) ] );

            prvTreatNBNS( pucUDPPayloadBuffer,
                          pxNetworkBuffer->xDataLength,
                          pxUDPPacket->xIPHeader.ulSourceIPAddress );

            /* The packet was not consumed. */
            return pdFAIL;
        }

    #endif /* ipconfigUSE_NBNS */
    /*-----------------------------------------------------------*/

/**
 * @brief Process a response packet from a DNS server, or an LLMNR reply.
 *
 * @param[in] pucUDPPayloadBuffer: The DNS response received as a UDP
 *                                 payload.
 * @param[in] uxBufferLength: Length of the UDP payload buffer.
 * @param[in] xExpected: indicates whether the identifier in the reply
 *                       was expected, and thus if the DNS cache may be
 *                       updated with the reply.
 *
 * @return The IP address in the DNS response if present and if xExpected is set to pdTRUE.
 *         An error code (dnsPARSE_ERROR) if there was an error in the DNS response.
 *         0 if xExpected set to pdFALSE.
 */
    _static uint32_t prvParseDNSReply( uint8_t * pucUDPPayloadBuffer,
                                       size_t uxBufferLength,
                                       BaseType_t xExpected )
    {
        DNSMessage_t * pxDNSMessageHeader;
        /* This pointer is not used to modify anything */
        const DNSAnswerRecord_t * pxDNSAnswerRecord;
        uint32_t ulIPAddress = 0UL;

        #if ( ipconfigUSE_LLMNR == 1 )
            char * pcRequestedName = NULL;
        #endif
        uint8_t * pucByte;
        size_t uxSourceBytesRemaining;
        uint16_t x, usDataLength, usQuestions;
        uint16_t usType = 0U;
        BaseType_t xReturn = pdTRUE;
        /* memcpy() helper variables for MISRA Rule 21.15 compliance*/
        const void * pvCopySource;
        void * pvCopyDest;

        #if ( ipconfigUSE_LLMNR == 1 )
            uint16_t usClass = 0U;
        #endif
        #if ( ipconfigUSE_DNS_CACHE == 1 ) || ( ipconfigDNS_USE_CALLBACKS == 1 )
            BaseType_t xDoStore = xExpected;
            char pcName[ ipconfigDNS_CACHE_NAME_LENGTH ] = "";
        #endif
        const size_t uxAddressLength = ipSIZE_OF_IPv4_ADDRESS;

        /* Ensure that the buffer is of at least minimal DNS message length. */
        if( uxBufferLength < sizeof( DNSMessage_t ) )
        {
            xReturn = pdFALSE;
        }
        else
        {
            uxSourceBytesRemaining = uxBufferLength;

            /* Parse the DNS message header. Map the byte stream onto a structure
             * for easier access. */
            pxDNSMessageHeader = ipCAST_PTR_TO_TYPE_PTR( DNSMessage_t, pucUDPPayloadBuffer );

            /* Introduce a do {} while (0) to allow the use of breaks. */
            do
            {
                size_t uxBytesRead = 0U;
                size_t uxResult;

                /* Start at the first byte after the header. */
                pucByte = &( pucUDPPayloadBuffer[ sizeof( DNSMessage_t ) ] );
                uxSourceBytesRemaining -= sizeof( DNSMessage_t );

                /* Skip any question records. */
                usQuestions = FreeRTOS_ntohs( pxDNSMessageHeader->usQuestions );

                for( x = 0U; x < usQuestions; x++ )
                {
                    #if ( ipconfigUSE_LLMNR == 1 )
                        {
                            if( x == 0U )
                            {
                                pcRequestedName = ( char * ) pucByte;
                            }
                        }
                    #endif

                    #if ( ipconfigUSE_DNS_CACHE == 1 ) || ( ipconfigDNS_USE_CALLBACKS == 1 )
                        if( x == 0U )
                        {
                            uxResult = prvReadNameField( pucByte,
                                                         uxSourceBytesRemaining,
                                                         pcName,
                                                         sizeof( pcName ) );

                            /* Check for a malformed response. */
                            if( uxResult == 0U )
                            {
                                xReturn = pdFALSE;
                                break;
                            }

                            uxBytesRead += uxResult;
                            pucByte = &( pucByte[ uxResult ] );
                            uxSourceBytesRemaining -= uxResult;
                        }
                        else
                    #endif /* ipconfigUSE_DNS_CACHE || ipconfigDNS_USE_CALLBACKS */
                    {
                        /* Skip the variable length pcName field. */
                        uxResult = prvSkipNameField( pucByte,
                                                     uxSourceBytesRemaining );

                        /* Check for a malformed response. */
                        if( uxResult == 0U )
                        {
                            xReturn = pdFALSE;
                            break;
                        }

                        uxBytesRead += uxResult;
                        pucByte = &( pucByte[ uxResult ] );
                        uxSourceBytesRemaining -= uxResult;
                    }

                    /* Check the remaining buffer size. */
                    if( uxSourceBytesRemaining >= sizeof( uint32_t ) )
                    {
                        #if ( ipconfigUSE_LLMNR == 1 )
                            {
                                /* usChar2u16 returns value in host endianness. */
                                usType = usChar2u16( pucByte );
                                usClass = usChar2u16( &( pucByte[ 2 ] ) );
                            }
                        #endif /* ipconfigUSE_LLMNR */

                        /* Skip the type and class fields. */
                        pucByte = &( pucByte[ sizeof( uint32_t ) ] );
                        uxSourceBytesRemaining -= sizeof( uint32_t );
                    }
                    else
                    {
                        xReturn = pdFALSE;
                        break;
                    }
                }

                if( xReturn == pdFALSE )
                {
                    /* No need to proceed. Break out of the do-while loop. */
                    break;
                }

                /* Search through the answer records. */
                pxDNSMessageHeader->usAnswers = FreeRTOS_ntohs( pxDNSMessageHeader->usAnswers );

                if( ( pxDNSMessageHeader->usFlags & dnsRX_FLAGS_MASK ) == dnsEXPECTED_RX_FLAGS )
                {
                    const uint16_t usCount = ( uint16_t ) ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY;
                    uint16_t usNumARecordsStored = 0;

                    for( x = 0U; x < pxDNSMessageHeader->usAnswers; x++ )
                    {
                        BaseType_t xDoAccept;

                        if( usNumARecordsStored >= usCount )
                        {
                            /* Only count ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY number of records. */
                            break;
                        }

                        uxResult = prvSkipNameField( pucByte,
                                                     uxSourceBytesRemaining );

                        /* Check for a malformed response. */
                        if( uxResult == 0U )
                        {
                            xReturn = pdFALSE;
                            break;
                        }

                        uxBytesRead += uxResult;
                        pucByte = &( pucByte[ uxResult ] );
                        uxSourceBytesRemaining -= uxResult;

                        /* Is there enough data for an IPv4 A record answer and, if so,
                         * is this an A record? */
                        if( uxSourceBytesRemaining < sizeof( uint16_t ) )
                        {
                            xReturn = pdFALSE;
                            break;
                        }

                        usType = usChar2u16( pucByte );

                        if( usType == ( uint16_t ) dnsTYPE_A_HOST )
                        {
                            if( uxSourceBytesRemaining >= ( sizeof( DNSAnswerRecord_t ) + uxAddressLength ) )
                            {
                                xDoAccept = pdTRUE;
                            }
                            else
                            {
                                xDoAccept = pdFALSE;
                            }
                        }
                        else
                        {
                            /* Unknown host type. */
                            xDoAccept = pdFALSE;
                        }

                        if( xDoAccept != pdFALSE )
                        {
                            /* This is the required record type and is of sufficient size. */

                            /* Mapping pucByte to a DNSAnswerRecord allows easy access of the
                             * fields of the structure. */
                            pxDNSAnswerRecord = ipCAST_PTR_TO_TYPE_PTR( DNSAnswerRecord_t, pucByte );

                            /* Sanity check the data length of an IPv4 answer. */
                            if( FreeRTOS_ntohs( pxDNSAnswerRecord->usDataLength ) == ( uint16_t ) uxAddressLength )
                            {
                                /* Copy the IP address out of the record. Using different pointers
                                 * to copy only the portion we want is intentional here. */

                                /*
                                 * Use helper variables for memcpy() to remain
                                 * compliant with MISRA Rule 21.15.  These should be
                                 * optimized away.
                                 */
                                pvCopySource = &pucByte[ sizeof( DNSAnswerRecord_t ) ];
                                pvCopyDest = &ulIPAddress;
                                ( void ) memcpy( pvCopyDest, pvCopySource, uxAddressLength );

                                #if ( ipconfigDNS_USE_CALLBACKS == 1 )
                                    {
                                        /* See if any asynchronous call was made to FreeRTOS_gethostbyname_a() */
                                        if( xDNSDoCallback( ( TickType_t ) pxDNSMessageHeader->usIdentifier, pcName, ulIPAddress ) != pdFALSE )
                                        {
                                            /* This device has requested this DNS look-up.
                                             * The result may be stored in the DNS cache. */
                                            xDoStore = pdTRUE;
                                        }
                                    }
                                #endif /* ipconfigDNS_USE_CALLBACKS == 1 */
                                #if ( ipconfigUSE_DNS_CACHE == 1 )
                                    {
                                        char cBuffer[ 16 ];

                                        /* The reply will only be stored in the DNS cache when the
                                         * request was issued by this device. */
                                        if( xDoStore != pdFALSE )
                                        {
                                            ( void ) prvProcessDNSCache( pcName, &ulIPAddress, pxDNSAnswerRecord->ulTTL, pdFALSE );
                                            usNumARecordsStored++; /* Track # of A records stored */
                                        }

                                        ( void ) FreeRTOS_inet_ntop( FREERTOS_AF_INET, ( const void * ) &( ulIPAddress ), cBuffer, sizeof( cBuffer ) );
                                        /* Show what has happened. */
                                        FreeRTOS_printf( ( "DNS[0x%04lX]: The answer to '%s' (%s) will%s be stored\n",
                                                           ( UBaseType_t ) pxDNSMessageHeader->usIdentifier,
                                                           pcName,
                                                           cBuffer,
                                                           ( xDoStore != 0 ) ? "" : " NOT" ) );
                                    }
                                #endif /* ipconfigUSE_DNS_CACHE */
                            }

                            pucByte = &( pucByte[ sizeof( DNSAnswerRecord_t ) + uxAddressLength ] );
                            uxSourceBytesRemaining -= ( sizeof( DNSAnswerRecord_t ) + uxAddressLength );
                        }
                        else if( uxSourceBytesRemaining >= sizeof( DNSAnswerRecord_t ) )
                        {
                            /* It's not an A record, so skip it. Get the header location
                             * and then jump over the header. */
                            /* Cast the response to DNSAnswerRecord for easy access to fields of the DNS response. */
                            pxDNSAnswerRecord = ipCAST_PTR_TO_TYPE_PTR( DNSAnswerRecord_t, pucByte );

                            pucByte = &( pucByte[ sizeof( DNSAnswerRecord_t ) ] );
                            uxSourceBytesRemaining -= sizeof( DNSAnswerRecord_t );

                            /* Determine the length of the answer data from the header. */
                            usDataLength = FreeRTOS_ntohs( pxDNSAnswerRecord->usDataLength );

                            /* Jump over the answer. */
                            if( uxSourceBytesRemaining >= usDataLength )
                            {
                                pucByte = &( pucByte[ usDataLength ] );
                                uxSourceBytesRemaining -= usDataLength;
                            }
                            else
                            {
                                /* Malformed response. */
                                xReturn = pdFALSE;
                                break;
                            }
                        }
                        else
                        {
                            /* Do nothing */
                        }
                    }
                }

                #if ( ipconfigUSE_LLMNR == 1 )
                    else if( ( usQuestions != ( uint16_t ) 0U ) && ( usType == dnsTYPE_A_HOST ) && ( usClass == dnsCLASS_IN ) && ( pcRequestedName != NULL ) )
                    {
                        /* If this is not a reply to our DNS request, it might an LLMNR
                         * request. */
                        if( xApplicationDNSQueryHook( &( pcRequestedName[ 1 ] ) ) != pdFALSE )
                        {
                            int16_t usLength;
                            NetworkBufferDescriptor_t * pxNewBuffer = NULL;
                            NetworkBufferDescriptor_t * pxNetworkBuffer = pxUDPPayloadBuffer_to_NetworkBuffer( pucUDPPayloadBuffer );
                            LLMNRAnswer_t * pxAnswer;
                            uint8_t * pucNewBuffer = NULL;

                            if( ( xBufferAllocFixedSize == pdFALSE ) && ( pxNetworkBuffer != NULL ) )
                            {
                                size_t uxDataLength = uxBufferLength + sizeof( UDPHeader_t ) + sizeof( EthernetHeader_t ) + sizeof( IPHeader_t );

                                /* Set the size of the outgoing packet. */
                                pxNetworkBuffer->xDataLength = uxDataLength;
                                pxNewBuffer = pxDuplicateNetworkBufferWithDescriptor( pxNetworkBuffer, uxDataLength + sizeof( LLMNRAnswer_t ) );

                                if( pxNewBuffer != NULL )
                                {
                                    BaseType_t xOffset1, xOffset2;

                                    xOffset1 = ( BaseType_t ) ( pucByte - pucUDPPayloadBuffer );
                                    xOffset2 = ( BaseType_t ) ( ( ( uint8_t * ) pcRequestedName ) - pucUDPPayloadBuffer );

                                    pxNetworkBuffer = pxNewBuffer;
                                    pucNewBuffer = &( pxNetworkBuffer->pucEthernetBuffer[ ipUDP_PAYLOAD_OFFSET_IPv4 ] );

                                    pucByte = &( pucNewBuffer[ xOffset1 ] );
                                    pcRequestedName = ( char * ) &( pucNewBuffer[ xOffset2 ] );
                                    pxDNSMessageHeader = ipCAST_PTR_TO_TYPE_PTR( DNSMessage_t, pucNewBuffer );
                                }
                                else
                                {
                                    /* Just to indicate that the message may not be answered. */
                                    pxNetworkBuffer = NULL;
                                }
                            }

                            /* The test on 'pucNewBuffer' is only to satisfy lint. */
                            if( ( pxNetworkBuffer != NULL ) && ( pucNewBuffer != NULL ) )
                            {
                                pxAnswer = ipCAST_PTR_TO_TYPE_PTR( LLMNRAnswer_t, pucByte );

                                /* We leave 'usIdentifier' and 'usQuestions' untouched */
                                #ifndef _lint
                                    vSetField16( pxDNSMessageHeader, DNSMessage_t, usFlags, dnsLLMNR_FLAGS_IS_REPONSE ); /* Set the response flag */
                                    vSetField16( pxDNSMessageHeader, DNSMessage_t, usAnswers, 1 );                       /* Provide a single answer */
                                    vSetField16( pxDNSMessageHeader, DNSMessage_t, usAuthorityRRs, 0 );                  /* No authority */
                                    vSetField16( pxDNSMessageHeader, DNSMessage_t, usAdditionalRRs, 0 );                 /* No additional info */
                                #endif /* lint */

                                pxAnswer->ucNameCode = dnsNAME_IS_OFFSET;
                                pxAnswer->ucNameOffset = ( uint8_t ) ( pcRequestedName - ( char * ) pucNewBuffer );

                                #ifndef _lint
                                    vSetField16( pxAnswer, LLMNRAnswer_t, usType, dnsTYPE_A_HOST ); /* Type A: host */
                                    vSetField16( pxAnswer, LLMNRAnswer_t, usClass, dnsCLASS_IN );   /* 1: Class IN */
                                    vSetField32( pxAnswer, LLMNRAnswer_t, ulTTL, dnsLLMNR_TTL_VALUE );
                                    vSetField16( pxAnswer, LLMNRAnswer_t, usDataLength, 4 );
                                    vSetField32( pxAnswer, LLMNRAnswer_t, ulIPAddress, FreeRTOS_ntohl( *ipLOCAL_IP_ADDRESS_POINTER ) );
                                #endif /* lint */
                                usLength = ( int16_t ) ( sizeof( *pxAnswer ) + ( size_t ) ( pucByte - pucNewBuffer ) );

                                prvReplyDNSMessage( pxNetworkBuffer, usLength );

                                if( pxNewBuffer != NULL )
                                {
                                    vReleaseNetworkBufferAndDescriptor( pxNewBuffer );
                                }
                            }
                        }
                    }
                    else
                    {
                        /* Not an expected reply. */
                    }
                #endif /* ipconfigUSE_LLMNR == 1 */
                ( void ) uxBytesRead;
            } while( ipFALSE_BOOL );
        }

        if( xReturn == pdFALSE )
        {
            /* There was an error while parsing the DNS response. Return error code. */
            ulIPAddress = dnsPARSE_ERROR;
        }
        else if( xExpected == pdFALSE )
        {
            /* Do not return a valid IP-address in case the reply was not expected. */
            ulIPAddress = 0UL;
        }
        else
        {
            /* The IP-address found will be returned. */
        }

        #if ( ipconfigUSE_DNS_CACHE == 1 ) || ( ipconfigDNS_USE_CALLBACKS == 1 )
            ( void ) xDoStore;
        #endif

        return ulIPAddress;
    }
    /*-----------------------------------------------------------*/

    #if ( ipconfigUSE_NBNS == 1 )

/**
 * @brief Respond to an NBNS query or an NBNS reply.
 *
 * @param[in] pucPayload: the UDP payload of the NBNS message.
 * @param[in] uxBufferLength: Length of the Buffer.
 * @param[in] ulIPAddress: IP address of the sender.
 */
        static void prvTreatNBNS( uint8_t * pucPayload,
                                  size_t uxBufferLength,
                                  uint32_t ulIPAddress )
        {
            uint16_t usFlags, usType, usClass;
            uint8_t * pucSource, * pucTarget;
            uint8_t ucByte;
            uint8_t ucNBNSName[ 17 ];
            uint8_t * pucUDPPayloadBuffer = pucPayload;
            NetworkBufferDescriptor_t * pxNetworkBuffer;
            size_t uxBytesNeeded = sizeof( UDPPacket_t ) + sizeof( NBNSRequest_t );

            /* Check for minimum buffer size. */
            if( uxBufferLength < uxBytesNeeded )
            {
                return;
            }

            /* Read the request flags in host endianness. */
            usFlags = usChar2u16( &( pucUDPPayloadBuffer[ offsetof( NBNSRequest_t, usFlags ) ] ) );

            if( ( usFlags & dnsNBNS_FLAGS_OPCODE_MASK ) == dnsNBNS_FLAGS_OPCODE_QUERY )
            {
                usType = usChar2u16( &( pucUDPPayloadBuffer[ offsetof( NBNSRequest_t, usType ) ] ) );
                usClass = usChar2u16( &( pucUDPPayloadBuffer[ offsetof( NBNSRequest_t, usClass ) ] ) );

                /* Not used for now */
                ( void ) usClass;

                /* For NBNS a name is 16 bytes long, written with capitals only.
                 * Make sure that the copy is terminated with a zero. */
                pucTarget = &( ucNBNSName[ sizeof( ucNBNSName ) - 2U ] );
                pucTarget[ 1 ] = ( uint8_t ) 0U;

                /* Start with decoding the last 2 bytes. */
                pucSource = &( pucUDPPayloadBuffer[ ( dnsNBNS_ENCODED_NAME_LENGTH - 2 ) + offsetof( NBNSRequest_t, ucName ) ] );

                for( ; ; )
                {
                    const uint8_t ucCharA = ( uint8_t ) 0x41U;

                    ucByte = ( ( uint8_t ) ( ( pucSource[ 0 ] - ucCharA ) << 4 ) ) | ( pucSource[ 1 ] - ucCharA );

                    /* Make sure there are no trailing spaces in the name. */
                    if( ( ucByte == ( uint8_t ) ' ' ) && ( pucTarget[ 1 ] == 0U ) )
                    {
                        ucByte = 0U;
                    }

                    *pucTarget = ucByte;

                    if( pucTarget == ucNBNSName )
                    {
                        break;
                    }

                    pucTarget -= 1;
                    pucSource -= 2;
                }

                #if ( ipconfigUSE_DNS_CACHE == 1 )
                    {
                        if( ( usFlags & dnsNBNS_FLAGS_RESPONSE ) != 0U )
                        {
                            /* If this is a response from another device,
                             * add the name to the DNS cache */
                            ( void ) prvProcessDNSCache( ( char * ) ucNBNSName, &( ulIPAddress ), 0, pdFALSE );
                        }
                    }
                #else
                    {
                        /* Avoid compiler warnings. */
                        ( void ) ulIPAddress;
                    }
                #endif /* ipconfigUSE_DNS_CACHE */

                if( ( ( usFlags & dnsNBNS_FLAGS_RESPONSE ) == 0U ) &&
                    ( usType == dnsNBNS_TYPE_NET_BIOS ) &&
                    ( xApplicationDNSQueryHook( ( const char * ) ucNBNSName ) != pdFALSE ) )
                {
                    uint16_t usLength;
                    DNSMessage_t * pxMessage;
                    NBNSAnswer_t * pxAnswer;

                    /* Someone is looking for a device with ucNBNSName,
                     * prepare a positive reply. */
                    pxNetworkBuffer = pxUDPPayloadBuffer_to_NetworkBuffer( pucUDPPayloadBuffer );

                    if( ( xBufferAllocFixedSize == pdFALSE ) && ( pxNetworkBuffer != NULL ) )
                    {
                        NetworkBufferDescriptor_t * pxNewBuffer;

                        /* The field xDataLength was set to the total length of the UDP packet,
                         * i.e. the payload size plus sizeof( UDPPacket_t ). */
                        pxNewBuffer = pxDuplicateNetworkBufferWithDescriptor( pxNetworkBuffer, pxNetworkBuffer->xDataLength + sizeof( NBNSAnswer_t ) );

                        if( pxNewBuffer != NULL )
                        {
                            pucUDPPayloadBuffer = &( pxNewBuffer->pucEthernetBuffer[ sizeof( UDPPacket_t ) ] );
                            pxNetworkBuffer = pxNewBuffer;
                        }
                        else
                        {
                            /* Just prevent that a reply will be sent */
                            pxNetworkBuffer = NULL;
                        }
                    }

                    /* Should not occur: pucUDPPayloadBuffer is part of a xNetworkBufferDescriptor */
                    if( pxNetworkBuffer != NULL )
                    {
                        pxMessage = ipCAST_PTR_TO_TYPE_PTR( DNSMessage_t, pucUDPPayloadBuffer );

                        /* As the fields in the structures are not word-aligned, we have to
                         * copy the values byte-by-byte using macro's vSetField16() and vSetField32() */
                        #ifndef _lint
                            vSetField16( pxMessage, DNSMessage_t, usFlags, dnsNBNS_QUERY_RESPONSE_FLAGS ); /* 0x8500 */
                            vSetField16( pxMessage, DNSMessage_t, usQuestions, 0 );
                            vSetField16( pxMessage, DNSMessage_t, usAnswers, 1 );
                            vSetField16( pxMessage, DNSMessage_t, usAuthorityRRs, 0 );
                            vSetField16( pxMessage, DNSMessage_t, usAdditionalRRs, 0 );
                        #else
                            ( void ) pxMessage;
                        #endif

                        pxAnswer = ipCAST_PTR_TO_TYPE_PTR( NBNSAnswer_t, &( pucUDPPayloadBuffer[ offsetof( NBNSRequest_t, usType ) ] ) );

                        #ifndef _lint
                            vSetField16( pxAnswer, NBNSAnswer_t, usType, usType );            /* Type */
                            vSetField16( pxAnswer, NBNSAnswer_t, usClass, dnsNBNS_CLASS_IN ); /* Class */
                            vSetField32( pxAnswer, NBNSAnswer_t, ulTTL, dnsNBNS_TTL_VALUE );
                            vSetField16( pxAnswer, NBNSAnswer_t, usDataLength, 6 );           /* 6 bytes including the length field */
                            vSetField16( pxAnswer, NBNSAnswer_t, usNbFlags, dnsNBNS_NAME_FLAGS );
                            vSetField32( pxAnswer, NBNSAnswer_t, ulIPAddress, FreeRTOS_ntohl( *ipLOCAL_IP_ADDRESS_POINTER ) );
                        #else
                            ( void ) pxAnswer;
                        #endif

                        usLength = ( uint16_t ) ( sizeof( NBNSAnswer_t ) + ( size_t ) offsetof( NBNSRequest_t, usType ) );

                        prvReplyDNSMessage( pxNetworkBuffer, ( BaseType_t ) usLength );
                    }
                }
            }
        }

    #endif /* ipconfigUSE_NBNS */
    /*-----------------------------------------------------------*/

/**
 * @brief Create a socket and bind it to the standard DNS port number.
 *
 * @return The created socket - or NULL if the socket could not be created or could not be bound.
 */
    static Socket_t prvCreateDNSSocket( void )
    {
        Socket_t xSocket;
        struct freertos_sockaddr xAddress;
        BaseType_t xReturn;

        /* This must be the first time this function has been called.  Create
         * the socket. */
        xSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );

        if( xSocketValid( xSocket ) == pdFALSE )
        {
            /* There was an error, return NULL. */
            xSocket = NULL;
        }
        else
        {
            /* Auto bind the port. */
            xAddress.sin_port = 0U;
            xReturn = FreeRTOS_bind( xSocket, &xAddress, sizeof( xAddress ) );

            /* Check the bind was successful, and clean up if not. */
            if( xReturn != 0 )
            {
                ( void ) FreeRTOS_closesocket( xSocket );
                xSocket = NULL;
            }
            else
            {
                /* The send and receive timeouts will be set later on. */
            }
        }

        return xSocket;
    }
/*-----------------------------------------------------------*/

    #if ( ( ipconfigUSE_NBNS == 1 ) || ( ipconfigUSE_LLMNR == 1 ) )

/**
 * @brief Send a DNS message to be used in NBNS or LLMNR
 *
 * @param[in] pxNetworkBuffer: The network buffer descriptor with the DNS message.
 * @param[in] lNetLength: The length of the DNS message.
 */
        static void prvReplyDNSMessage( NetworkBufferDescriptor_t * pxNetworkBuffer,
                                        BaseType_t lNetLength )
        {
            UDPPacket_t * pxUDPPacket;
            IPHeader_t * pxIPHeader;
            UDPHeader_t * pxUDPHeader;
            size_t uxDataLength;

            pxUDPPacket = ipCAST_PTR_TO_TYPE_PTR( UDPPacket_t, pxNetworkBuffer->pucEthernetBuffer );
            pxIPHeader = &pxUDPPacket->xIPHeader;
            pxUDPHeader = &pxUDPPacket->xUDPHeader;
            /* HT: started using defines like 'ipSIZE_OF_xxx' */
            pxIPHeader->usLength = FreeRTOS_htons( ( uint16_t ) lNetLength + ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_UDP_HEADER );
            /* HT:endian: should not be translated, copying from packet to packet */
            pxIPHeader->ulDestinationIPAddress = pxIPHeader->ulSourceIPAddress;
            pxIPHeader->ulSourceIPAddress = *ipLOCAL_IP_ADDRESS_POINTER;
            pxIPHeader->ucTimeToLive = ipconfigUDP_TIME_TO_LIVE;
            pxIPHeader->usIdentification = FreeRTOS_htons( usPacketIdentifier );
            usPacketIdentifier++;
            pxUDPHeader->usLength = FreeRTOS_htons( ( uint32_t ) lNetLength + ipSIZE_OF_UDP_HEADER );
            vFlip_16( pxUDPHeader->usSourcePort, pxUDPHeader->usDestinationPort );

            /* Important: tell NIC driver how many bytes must be sent */
            uxDataLength = ( ( size_t ) lNetLength ) + ipSIZE_OF_IPv4_HEADER + ipSIZE_OF_UDP_HEADER + ipSIZE_OF_ETH_HEADER;

            #if ( ipconfigDRIVER_INCLUDED_TX_IP_CHECKSUM == 0 )
                {
                    /* Calculate the IP header checksum. */
                    pxIPHeader->usHeaderChecksum = 0U;
                    pxIPHeader->usHeaderChecksum = usGenerateChecksum( 0U, ( uint8_t * ) &( pxIPHeader->ucVersionHeaderLength ), ipSIZE_OF_IPv4_HEADER );
                    pxIPHeader->usHeaderChecksum = ~FreeRTOS_htons( pxIPHeader->usHeaderChecksum );

                    /* calculate the UDP checksum for outgoing package */
                    ( void ) usGenerateProtocolChecksum( ( uint8_t * ) pxUDPPacket, uxDataLength, pdTRUE );
                }
            #endif

            /* Important: tell NIC driver how many bytes must be sent */
            pxNetworkBuffer->xDataLength = uxDataLength;

            /* This function will fill in the eth addresses and send the packet */
            vReturnEthernetFrame( pxNetworkBuffer, pdFALSE );
        }

    #endif /* ipconfigUSE_NBNS == 1 || ipconfigUSE_LLMNR == 1 */

/*-----------------------------------------------------------*/

    #if ( ipconfigUSE_DNS_CACHE == 1 )

/**
 * @brief Send a DNS message to be used in NBNS or LLMNR
 *
 * @param[in] pcName: the name of the host
 * @param[in,out] pulIP: when doing a lookup, will be set, when doing an update,
 *                       will be read.
 * @param[in] ulTTL: Time To Live
 * @param[in] xLookUp: pdTRUE if a look-up is expected, pdFALSE, when the DNS cache must
 *                     be updated.
 *
 * @return
 */
        static BaseType_t prvProcessDNSCache( const char * pcName,
                                              uint32_t * pulIP,
                                              uint32_t ulTTL,
                                              BaseType_t xLookUp )
        {
            BaseType_t x;
            BaseType_t xFound = pdFALSE;
            uint32_t ulCurrentTimeSeconds = ( xTaskGetTickCount() / portTICK_PERIOD_MS ) / 1000UL;
            uint32_t ulIPAddressIndex = 0;
            static BaseType_t xFreeEntry = 0;

            configASSERT( ( pcName != NULL ) );

            /* For each entry in the DNS cache table. */
            for( x = 0; x < ipconfigDNS_CACHE_ENTRIES; x++ )
            {
                if( xDNSCache[ x ].pcName[ 0 ] == ( char ) 0 )
                {
                    continue;
                }

                if( strcmp( xDNSCache[ x ].pcName, pcName ) == 0 )
                {
                    /* Is this function called for a lookup or to add/update an IP address? */
                    if( xLookUp != pdFALSE )
                    {
                        /* Confirm that the record is still fresh. */
                        if( ulCurrentTimeSeconds < ( xDNSCache[ x ].ulTimeWhenAddedInSeconds + FreeRTOS_ntohl( xDNSCache[ x ].ulTTL ) ) )
                        {
                            #if ( ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY > 1 )
                                uint8_t ucIndex;
                                /* The ucCurrentIPAddress value increments without bound and will rollover, */
                                /*  modulo it by the number of IP addresses to keep it in range.     */
                                /*  Also perform a final modulo by the max number of IP addresses    */
                                /*  per DNS cache entry to prevent out-of-bounds access in the event */
                                /*  that ucNumIPAddresses has been corrupted.                        */
                                ucIndex = xDNSCache[ x ].ucCurrentIPAddress % xDNSCache[ x ].ucNumIPAddresses;
                                ucIndex = ucIndex % ( uint8_t ) ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY;
                                ulIPAddressIndex = ucIndex;

                                xDNSCache[ x ].ucCurrentIPAddress++;
                            #endif /* if ( ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY > 1 ) */
                            *pulIP = xDNSCache[ x ].ulIPAddresses[ ulIPAddressIndex ];
                        }
                        else
                        {
                            /* Age out the old cached record. */
                            xDNSCache[ x ].pcName[ 0 ] = ( char ) 0;
                        }
                    }
                    else
                    {
                        #if ( ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY > 1 )
                            if( xDNSCache[ x ].ucNumIPAddresses < ( uint8_t ) ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY )
                            {
                                /* If more answers exist than there are IP address storage slots */
                                /* they will overwrite entry 0 */

                                ulIPAddressIndex = xDNSCache[ x ].ucNumIPAddresses;
                                xDNSCache[ x ].ucNumIPAddresses++;
                            }
                        #endif
                        xDNSCache[ x ].ulIPAddresses[ ulIPAddressIndex ] = *pulIP;
                        xDNSCache[ x ].ulTTL = ulTTL;
                        xDNSCache[ x ].ulTimeWhenAddedInSeconds = ulCurrentTimeSeconds;
                    }

                    xFound = pdTRUE;
                    break;
                }
            }

            if( xFound == pdFALSE )
            {
                if( xLookUp != pdFALSE )
                {
                    *pulIP = 0UL;
                }
                else
                {
                    /* Add or update the item. */
                    if( strlen( pcName ) < ( size_t ) ipconfigDNS_CACHE_NAME_LENGTH )
                    {
                        ( void ) strcpy( xDNSCache[ xFreeEntry ].pcName, pcName );

                        xDNSCache[ xFreeEntry ].ulIPAddresses[ 0 ] = *pulIP;
                        xDNSCache[ xFreeEntry ].ulTTL = ulTTL;
                        xDNSCache[ xFreeEntry ].ulTimeWhenAddedInSeconds = ulCurrentTimeSeconds;
                        #if ( ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY > 1 )
                            xDNSCache[ xFreeEntry ].ucNumIPAddresses = 1;
                            xDNSCache[ xFreeEntry ].ucCurrentIPAddress = 0;

                            /* Initialize all remaining IP addresses in this entry to 0 */
                            ( void ) memset( &xDNSCache[ xFreeEntry ].ulIPAddresses[ 1 ],
                                             0,
                                             sizeof( xDNSCache[ xFreeEntry ].ulIPAddresses[ 1 ] ) *
                                             ( ( uint32_t ) ipconfigDNS_CACHE_ADDRESSES_PER_ENTRY - 1U ) );
                        #endif

                        xFreeEntry++;

                        if( xFreeEntry == ipconfigDNS_CACHE_ENTRIES )
                        {
                            xFreeEntry = 0;
                        }
                    }
                }
            }

            if( ( xLookUp == 0 ) || ( *pulIP != 0UL ) )
            {
                FreeRTOS_debug_printf( ( "prvProcessDNSCache: %s: '%s' @ %lxip\n", ( xLookUp != 0 ) ? "look-up" : "add", pcName, FreeRTOS_ntohl( *pulIP ) ) );
            }

            return xFound;
        }

    #endif /* ipconfigUSE_DNS_CACHE */

#endif /* ipconfigUSE_DNS != 0 */

/*-----------------------------------------------------------*/

/* Provide access to private members for testing. */
#ifdef FREERTOS_ENABLE_UNIT_TESTS
    #include "freertos_tcp_test_access_dns_define.h"
#endif
