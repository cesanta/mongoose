/*
 * Handling of Ethernet PHY's
 * PHY's communicate with an EMAC either through
 * a Media-Independent Interface (MII), or a Reduced Media-Independent Interface (RMII).
 * The EMAC can poll for PHY ports on 32 different addresses. Each of the PHY ports
 * shall be treated independently.
 *
 */

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "phyHandling.h"

#define phyMIN_PHY_ADDRESS    0
#define phyMAX_PHY_ADDRESS    31

#if defined( PHY_LS_HIGH_CHECK_TIME_MS ) || defined( PHY_LS_LOW_CHECK_TIME_MS )
    #warning please use the new defines with 'ipconfig' prefix
#endif

#ifndef ipconfigPHY_LS_HIGH_CHECK_TIME_MS

/* Check if the LinkStatus in the PHY is still high after 15 seconds of not
 * receiving packets. */
    #define ipconfigPHY_LS_HIGH_CHECK_TIME_MS    15000UL
#endif

#ifndef ipconfigPHY_LS_LOW_CHECK_TIME_MS
    /* Check if the LinkStatus in the PHY is still low every second. */
    #define ipconfigPHY_LS_LOW_CHECK_TIME_MS    1000UL
#endif

/* As the following 3 macro's are OK in most situations, and so they're not
 * included in 'FreeRTOSIPConfigDefaults.h'.
 * Users can change their values in the project's 'FreeRTOSIPConfig.h'. */
#ifndef phyPHY_MAX_RESET_TIME_MS
    #define phyPHY_MAX_RESET_TIME_MS    1000UL
#endif

#ifndef phyPHY_MAX_NEGOTIATE_TIME_MS
    #define phyPHY_MAX_NEGOTIATE_TIME_MS    3000UL
#endif

#ifndef phySHORT_DELAY_MS
    #define phySHORT_DELAY_MS    50UL
#endif

/* Naming and numbering of basic PHY registers. */
#define phyREG_00_BMCR             0x00U    /* Basic Mode Control Register. */
#define phyREG_01_BMSR             0x01U    /* Basic Mode Status Register. */
#define phyREG_02_PHYSID1          0x02U    /* PHYS ID 1 */
#define phyREG_03_PHYSID2          0x03U    /* PHYS ID 2 */
#define phyREG_04_ADVERTISE        0x04U    /* Advertisement control reg */

/* Naming and numbering of extended PHY registers. */
#define PHYREG_10_PHYSTS           0x10U    /* 16 PHY status register Offset */
#define phyREG_19_PHYCR            0x19U    /* 25 RW PHY Control Register */
#define phyREG_1F_PHYSPCS          0x1FU    /* 31 RW PHY Special Control Status */

/* Bit fields for 'phyREG_00_BMCR', the 'Basic Mode Control Register'. */
#define phyBMCR_FULL_DUPLEX        0x0100U  /* Full duplex. */
#define phyBMCR_AN_RESTART         0x0200U  /* Auto negotiation restart. */
#define phyBMCR_ISOLATE            0x0400U  /* 1 = Isolates 0 = Normal operation. */
#define phyBMCR_AN_ENABLE          0x1000U  /* Enable auto negotiation. */
#define phyBMCR_SPEED_100          0x2000U  /* Select 100Mbps. */
#define phyBMCR_RESET              0x8000U  /* Reset the PHY. */

/* Bit fields for 'phyREG_19_PHYCR', the 'PHY Control Register'. */
#define PHYCR_MDIX_EN              0x8000U  /* Enable Auto MDIX. */
#define PHYCR_MDIX_FORCE           0x4000U  /* Force MDIX crossed. */

#define phyBMSR_AN_COMPLETE        0x0020U  /* Auto-Negotiation process completed */

#define phyBMSR_LINK_STATUS        0x0004U

#define phyPHYSTS_LINK_STATUS      0x0001U  /* PHY Link mask */
#define phyPHYSTS_SPEED_STATUS     0x0002U  /* PHY Speed mask */
#define phyPHYSTS_DUPLEX_STATUS    0x0004U  /* PHY Duplex mask */

/* Bit fields for 'phyREG_1F_PHYSPCS
 *  001 = 10BASE-T half-duplex
 *  101 = 10BASE-T full-duplex
 *  010 = 100BASE-TX half-duplex
 *  110 = 100BASE-TX full-duplex
 */
#define phyPHYSPCS_SPEED_MASK      0x000CU
#define phyPHYSPCS_SPEED_10        0x0004U
#define phyPHYSPCS_FULL_DUPLEX     0x0010U

/*
 * Description of all capabilities that can be advertised to
 * the peer (usually a switch or router).
 */

#define phyADVERTISE_CSMA       0x0001U     /* Supports IEEE 802.3u: Fast Ethernet at 100 Mbit/s */
#define phyADVERTISE_10HALF     0x0020U     /* Try for 10mbps half-duplex. */
#define phyADVERTISE_10FULL     0x0040U     /* Try for 10mbps full-duplex. */
#define phyADVERTISE_100HALF    0x0080U     /* Try for 100mbps half-duplex. */
#define phyADVERTISE_100FULL    0x0100U     /* Try for 100mbps full-duplex. */

#define phyADVERTISE_ALL                            \
    ( phyADVERTISE_10HALF | phyADVERTISE_10FULL |   \
      phyADVERTISE_100HALF | phyADVERTISE_100FULL | \
      phyADVERTISE_CSMA )

/* Send a reset command to a set of PHY-ports. */
static uint32_t xPhyReset( EthernetPhy_t * pxPhyObject,
                           uint32_t ulPhyMask );

static BaseType_t xHas_1F_PHYSPCS( uint32_t ulPhyID )
{
    BaseType_t xResult;

    switch( ulPhyID )
    {
        case PHY_ID_LAN8720:
        case PHY_ID_LAN8742A:
        case PHY_ID_KSZ8041:

        /*
         *      case PHY_ID_KSZ8051: // same ID as 8041
         *      case PHY_ID_KSZ8081: // same ID as 8041
         */
        case PHY_ID_KSZ8081MNXIA:

        case PHY_ID_KSZ8863:
        default:
            /* Most PHY's have a 1F_PHYSPCS */
            xResult = pdTRUE;
            break;

        case PHY_ID_DP83848I:
            xResult = pdFALSE;
            break;
    }

    return xResult;
}
/*-----------------------------------------------------------*/

static BaseType_t xHas_19_PHYCR( uint32_t ulPhyID )
{
    BaseType_t xResult;

    switch( ulPhyID )
    {
        case PHY_ID_LAN8742A:
        case PHY_ID_DP83848I:
            xResult = pdTRUE;
            break;

        default:
            /* Most PHY's do not have a 19_PHYCR */
            xResult = pdFALSE;
            break;
    }

    return xResult;
}
/*-----------------------------------------------------------*/

/* Initialise the struct and assign a PHY-read and -write function. */
void vPhyInitialise( EthernetPhy_t * pxPhyObject,
                     xApplicationPhyReadHook_t fnPhyRead,
                     xApplicationPhyWriteHook_t fnPhyWrite )
{
    memset( ( void * ) pxPhyObject, 0, sizeof( *pxPhyObject ) );

    pxPhyObject->fnPhyRead = fnPhyRead;
    pxPhyObject->fnPhyWrite = fnPhyWrite;
}
/*-----------------------------------------------------------*/

/* Discover all PHY's connected by polling 32 indexes ( zero-based ) */
BaseType_t xPhyDiscover( EthernetPhy_t * pxPhyObject )
{
    BaseType_t xPhyAddress;

    pxPhyObject->xPortCount = 0;

    for( xPhyAddress = phyMIN_PHY_ADDRESS; xPhyAddress <= phyMAX_PHY_ADDRESS; xPhyAddress++ )
    {
        uint32_t ulLowerID;

        pxPhyObject->fnPhyRead( xPhyAddress, phyREG_03_PHYSID2, &ulLowerID );

        /* A valid PHY id can not be all zeros or all ones. */
        if( ( ulLowerID != ( uint16_t ) ~0U ) && ( ulLowerID != ( uint16_t ) 0U ) )
        {
            uint32_t ulUpperID;
            uint32_t ulPhyID;

            pxPhyObject->fnPhyRead( xPhyAddress, phyREG_02_PHYSID1, &ulUpperID );
            ulPhyID = ( ( ( uint32_t ) ulUpperID ) << 16 ) | ( ulLowerID & 0xFFF0 );

            pxPhyObject->ucPhyIndexes[ pxPhyObject->xPortCount ] = xPhyAddress;
            pxPhyObject->ulPhyIDs[ pxPhyObject->xPortCount ] = ulPhyID;

            pxPhyObject->xPortCount++;

            /* See if there is more storage space. */
            if( pxPhyObject->xPortCount == ipconfigPHY_MAX_PORTS )
            {
                break;
            }
        }
    }

    if( pxPhyObject->xPortCount > 0 )
    {
        FreeRTOS_printf( ( "PHY ID %lX\n", pxPhyObject->ulPhyIDs[ 0 ] ) );
    }

    return pxPhyObject->xPortCount;
}
/*-----------------------------------------------------------*/

/* Send a reset command to a set of PHY-ports. */
static uint32_t xPhyReset( EthernetPhy_t * pxPhyObject,
                           uint32_t ulPhyMask )
{
    uint32_t ulDoneMask, ulConfig;
    TickType_t xRemainingTime;
    TimeOut_t xTimer;
    BaseType_t xPhyIndex;

    /* A bit-mask of PHY ports that are ready. */
    ulDoneMask = 0UL;

    /* Set the RESET bits high. */
    for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++ )
    {
        BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

        /* Read Control register. */
        pxPhyObject->fnPhyRead( xPhyAddress, phyREG_00_BMCR, &ulConfig );
        pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_00_BMCR, ulConfig | phyBMCR_RESET );
    }

    xRemainingTime = ( TickType_t ) pdMS_TO_TICKS( phyPHY_MAX_RESET_TIME_MS );
    vTaskSetTimeOutState( &xTimer );

    /* The reset should last less than a second. */
    for( ; ; )
    {
        for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++ )
        {
            BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

            pxPhyObject->fnPhyRead( xPhyAddress, phyREG_00_BMCR, &ulConfig );

            if( ( ulConfig & phyBMCR_RESET ) == 0 )
            {
                FreeRTOS_printf( ( "xPhyReset: phyBMCR_RESET %d ready\n", ( int ) xPhyIndex ) );
                ulDoneMask |= ( 1UL << xPhyIndex );
            }
        }

        if( ulDoneMask == ulPhyMask )
        {
            break;
        }

        if( xTaskCheckForTimeOut( &xTimer, &xRemainingTime ) != pdFALSE )
        {
            FreeRTOS_printf( ( "xPhyReset: phyBMCR_RESET timed out ( done 0x%02lX )\n", ulDoneMask ) );
            break;
        }

        /* Block for a while */
        vTaskDelay( pdMS_TO_TICKS( phySHORT_DELAY_MS ) );
    }

    /* Clear the reset bits. */
    for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++ )
    {
        if( ( ulDoneMask & ( 1UL << xPhyIndex ) ) == 0UL )
        {
            BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

            /* The reset operation timed out, clear the bit manually. */
            pxPhyObject->fnPhyRead( xPhyAddress, phyREG_00_BMCR, &ulConfig );
            pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_00_BMCR, ulConfig & ~phyBMCR_RESET );
        }
    }

    vTaskDelay( pdMS_TO_TICKS( phySHORT_DELAY_MS ) );

    return ulDoneMask;
}
/*-----------------------------------------------------------*/

BaseType_t xPhyConfigure( EthernetPhy_t * pxPhyObject,
                          const PhyProperties_t * pxPhyProperties )
{
    uint32_t ulConfig, ulAdvertise;
    BaseType_t xPhyIndex;

    if( pxPhyObject->xPortCount < 1 )
    {
        FreeRTOS_printf( ( "xPhyConfigure: No PHY's detected.\n" ) );
        return -1;
    }

    /* The expected ID for the 'LAN8742A'  is 0x0007c130. */
    /* The expected ID for the 'LAN8720'   is 0x0007c0f0. */
    /* The expected ID for the 'DP83848I'  is 0x20005C90. */

    /* Set advertise register. */
    if( ( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_AUTO ) && ( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_AUTO ) )
    {
        ulAdvertise = phyADVERTISE_ALL;
        /* Reset auto-negotiation capability. */
    }
    else
    {
        /* Always select protocol 802.3u. */
        ulAdvertise = phyADVERTISE_CSMA;

        if( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_AUTO )
        {
            if( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_FULL )
            {
                ulAdvertise |= phyADVERTISE_10FULL | phyADVERTISE_100FULL;
            }
            else
            {
                ulAdvertise |= phyADVERTISE_10HALF | phyADVERTISE_100HALF;
            }
        }
        else if( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_AUTO )
        {
            if( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_10 )
            {
                ulAdvertise |= phyADVERTISE_10FULL | phyADVERTISE_10HALF;
            }
            else
            {
                ulAdvertise |= phyADVERTISE_100FULL | phyADVERTISE_100HALF;
            }
        }
        else if( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_100 )
        {
            if( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_FULL )
            {
                ulAdvertise |= phyADVERTISE_100FULL;
            }
            else
            {
                ulAdvertise |= phyADVERTISE_100HALF;
            }
        }
        else
        {
            if( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_FULL )
            {
                ulAdvertise |= phyADVERTISE_10FULL;
            }
            else
            {
                ulAdvertise |= phyADVERTISE_10HALF;
            }
        }
    }

    /* Send a reset command to a set of PHY-ports. */
    xPhyReset( pxPhyObject, xPhyGetMask( pxPhyObject ) );

    for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++ )
    {
        BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];
        uint32_t ulPhyID = pxPhyObject->ulPhyIDs[ xPhyIndex ];

        /* Write advertise register. */
        pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_04_ADVERTISE, ulAdvertise );

        /*
         *      AN_EN        AN1         AN0       Forced Mode
         *        0           0           0        10BASE-T, Half-Duplex
         *        0           0           1        10BASE-T, Full-Duplex
         *        0           1           0        100BASE-TX, Half-Duplex
         *        0           1           1        100BASE-TX, Full-Duplex
         *      AN_EN        AN1         AN0       Advertised Mode
         *        1           0           0        10BASE-T, Half/Full-Duplex
         *        1           0           1        100BASE-TX, Half/Full-Duplex
         *        1           1           0        10BASE-T Half-Duplex
         *                                         100BASE-TX, Half-Duplex
         *        1           1           1        10BASE-T, Half/Full-Duplex
         *                                         100BASE-TX, Half/Full-Duplex
         */

        /* Read Control register. */
        pxPhyObject->fnPhyRead( xPhyAddress, phyREG_00_BMCR, &ulConfig );

        ulConfig &= ~( phyBMCR_SPEED_100 | phyBMCR_FULL_DUPLEX );

        ulConfig |= phyBMCR_AN_ENABLE;

        if( ( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_100 ) || ( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_AUTO ) )
        {
            ulConfig |= phyBMCR_SPEED_100;
        }
        else if( pxPhyProperties->ucSpeed == ( uint8_t ) PHY_SPEED_10 )
        {
            ulConfig &= ~phyBMCR_SPEED_100;
        }

        if( ( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_FULL ) || ( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_AUTO ) )
        {
            ulConfig |= phyBMCR_FULL_DUPLEX;
        }
        else if( pxPhyProperties->ucDuplex == ( uint8_t ) PHY_DUPLEX_HALF )
        {
            ulConfig &= ~phyBMCR_FULL_DUPLEX;
        }

        if( xHas_19_PHYCR( ulPhyID ) )
        {
            uint32_t ulPhyControl;
            /* Read PHY Control register. */
            pxPhyObject->fnPhyRead( xPhyAddress, phyREG_19_PHYCR, &ulPhyControl );

            /* Clear bits which might get set: */
            ulPhyControl &= ~( PHYCR_MDIX_EN | PHYCR_MDIX_FORCE );

            if( pxPhyProperties->ucMDI_X == PHY_MDIX_AUTO )
            {
                ulPhyControl |= PHYCR_MDIX_EN;
            }
            else if( pxPhyProperties->ucMDI_X == PHY_MDIX_CROSSED )
            {
                /* Force direct link = Use crossed RJ45 cable. */
                ulPhyControl &= ~PHYCR_MDIX_FORCE;
            }
            else
            {
                /* Force crossed link = Use direct RJ45 cable. */
                ulPhyControl |= PHYCR_MDIX_FORCE;
            }

            /* update PHY Control Register. */
            pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_19_PHYCR, ulPhyControl );
        }

        FreeRTOS_printf( ( "+TCP: advertise: %04lX config %04lX\n", ulAdvertise, ulConfig ) );
    }

    /* Keep these values for later use. */
    pxPhyObject->ulBCRValue = ulConfig & ~phyBMCR_ISOLATE;
    pxPhyObject->ulACRValue = ulAdvertise;

    return 0;
}
/*-----------------------------------------------------------*/

/* xPhyFixedValue(): this function is called in case auto-negotiation is disabled.
 * The caller has set the values in 'xPhyPreferences' (ucDuplex and ucSpeed).
 * The PHY register phyREG_00_BMCR will be set for every connected PHY that matches
 * with ulPhyMask. */
BaseType_t xPhyFixedValue( EthernetPhy_t * pxPhyObject,
                           uint32_t ulPhyMask )
{
    BaseType_t xPhyIndex;
    uint32_t ulValue, ulBitMask = ( uint32_t ) 1U;

    ulValue = ( uint32_t ) 0U;

    if( pxPhyObject->xPhyPreferences.ucDuplex == PHY_DUPLEX_FULL )
    {
        ulValue |= phyBMCR_FULL_DUPLEX;
    }

    if( pxPhyObject->xPhyPreferences.ucSpeed == PHY_SPEED_100 )
    {
        ulValue |= phyBMCR_SPEED_100;
    }

    for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++, ulBitMask <<= 1 )
    {
        if( ( ulPhyMask & ulBitMask ) != 0lu )
        {
            BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

            /* Enable Auto-Negotiation. */
            pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_00_BMCR, ulValue );
        }
    }

    return 0;
}
/*-----------------------------------------------------------*/

/* xPhyStartAutoNegotiation() is the alternative xPhyFixedValue():
 * It sets the BMCR_AN_RESTART bit and waits for the auto-negotiation completion
 * ( phyBMSR_AN_COMPLETE ). */
BaseType_t xPhyStartAutoNegotiation( EthernetPhy_t * pxPhyObject,
                                     uint32_t ulPhyMask )
{
    uint32_t xPhyIndex, ulDoneMask, ulBitMask;
    uint32_t ulPHYLinkStatus, ulRegValue;
    TickType_t xRemainingTime;
    TimeOut_t xTimer;

    if( ulPhyMask == ( uint32_t ) 0U )
    {
        return 0;
    }

    for( xPhyIndex = 0; xPhyIndex < ( uint32_t ) pxPhyObject->xPortCount; xPhyIndex++ )
    {
        if( ( ulPhyMask & ( 1lu << xPhyIndex ) ) != 0lu )
        {
            BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

            /* Enable Auto-Negotiation. */
            pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_04_ADVERTISE, pxPhyObject->ulACRValue );
            pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_00_BMCR, pxPhyObject->ulBCRValue | phyBMCR_AN_RESTART );
        }
    }

    xRemainingTime = ( TickType_t ) pdMS_TO_TICKS( phyPHY_MAX_NEGOTIATE_TIME_MS );
    vTaskSetTimeOutState( &xTimer );
    ulDoneMask = 0;

    /* Wait until the auto-negotiation will be completed */
    for( ; ; )
    {
        ulBitMask = ( uint32_t ) 1U;

        for( xPhyIndex = 0; xPhyIndex < ( uint32_t ) pxPhyObject->xPortCount; xPhyIndex++, ulBitMask <<= 1 )
        {
            if( ( ulPhyMask & ulBitMask ) != 0lu )
            {
                if( ( ulDoneMask & ulBitMask ) == 0lu )
                {
                    BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

                    pxPhyObject->fnPhyRead( xPhyAddress, phyREG_01_BMSR, &ulRegValue );

                    if( ( ulRegValue & phyBMSR_AN_COMPLETE ) != 0 )
                    {
                        ulDoneMask |= ulBitMask;
                    }
                }
            }
        }

        if( ulPhyMask == ulDoneMask )
        {
            break;
        }

        if( xTaskCheckForTimeOut( &xTimer, &xRemainingTime ) != pdFALSE )
        {
            FreeRTOS_printf( ( "xPhyStartAutoNegotiation: phyBMCR_RESET timed out ( done 0x%02lX )\n", ulDoneMask ) );
            break;
        }

        vTaskDelay( pdMS_TO_TICKS( phySHORT_DELAY_MS ) );
    }

    if( ulDoneMask != ( uint32_t ) 0U )
    {
        ulBitMask = ( uint32_t ) 1U;
        pxPhyObject->ulLinkStatusMask &= ~( ulDoneMask );

        for( xPhyIndex = 0; xPhyIndex < ( uint32_t ) pxPhyObject->xPortCount; xPhyIndex++, ulBitMask <<= 1 )
        {
            BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];
            uint32_t ulPhyID = pxPhyObject->ulPhyIDs[ xPhyIndex ];

            if( ( ulDoneMask & ulBitMask ) == ( uint32_t ) 0U )
            {
                continue;
            }

            /* Clear the 'phyBMCR_AN_RESTART'  bit. */
            pxPhyObject->fnPhyWrite( xPhyAddress, phyREG_00_BMCR, pxPhyObject->ulBCRValue );

            pxPhyObject->fnPhyRead( xPhyAddress, phyREG_01_BMSR, &ulRegValue );

            if( ( ulRegValue & phyBMSR_LINK_STATUS ) != 0 )
            {
                ulPHYLinkStatus |= phyBMSR_LINK_STATUS;
                pxPhyObject->ulLinkStatusMask |= ulBitMask;
            }
            else
            {
                ulPHYLinkStatus &= ~( phyBMSR_LINK_STATUS );
            }

            if( ulPhyID == PHY_ID_KSZ8081MNXIA )
            {
                uint32_t ulControlStatus;

                pxPhyObject->fnPhyRead( xPhyAddress, 0x1E, &ulControlStatus );

                switch( ulControlStatus & 0x07 )
                {
                    case 0x01:
                    case 0x05:
/*	[001] = 10BASE-T half-duplex */
/*	[101] = 10BASE-T full-duplex */
                        /* 10 Mbps. */
                        ulRegValue |= phyPHYSTS_SPEED_STATUS;
                        break;

                    case 0x02:
                    case 0x06:
/*	[010] = 100BASE-TX half-duplex */
/*	[110] = 100BASE-TX full-duplex */
                        break;
                }

                switch( ulControlStatus & 0x07 )
                {
                    case 0x05:
                    case 0x06:
/*	[101] = 10BASE-T full-duplex */
/*	[110] = 100BASE-TX full-duplex */
                        /* Full duplex. */
                        ulRegValue |= phyPHYSTS_DUPLEX_STATUS;
                        break;

                    case 0x01:
                    case 0x02:
/*	[001] = 10BASE-T half-duplex */
/*	[010] = 100BASE-TX half-duplex */
                        break;
                }
            }
            else if( xHas_1F_PHYSPCS( ulPhyID ) )
            {
                /* 31 RW PHY Special Control Status */
                uint32_t ulControlStatus;

                pxPhyObject->fnPhyRead( xPhyAddress, phyREG_1F_PHYSPCS, &ulControlStatus );
                ulRegValue = 0;

                if( ( ulControlStatus & phyPHYSPCS_FULL_DUPLEX ) != 0 )
                {
                    ulRegValue |= phyPHYSTS_DUPLEX_STATUS;
                }

                if( ( ulControlStatus & phyPHYSPCS_SPEED_MASK ) == phyPHYSPCS_SPEED_10 )
                {
                    ulRegValue |= phyPHYSTS_SPEED_STATUS;
                }
            }
            else
            {
                /* Read the result of the auto-negotiation. */
                pxPhyObject->fnPhyRead( xPhyAddress, PHYREG_10_PHYSTS, &ulRegValue );
            }

            FreeRTOS_printf( ( "Autonego ready: %08lx: %s duplex %u mbit %s status\n",
                               ulRegValue,
                               ( ulRegValue & phyPHYSTS_DUPLEX_STATUS ) ? "full" : "half",
                               ( ulRegValue & phyPHYSTS_SPEED_STATUS ) ? 10 : 100,
                               ( ( ulPHYLinkStatus |= phyBMSR_LINK_STATUS ) != 0 ) ? "high" : "low" ) );

            if( ( ulRegValue & phyPHYSTS_DUPLEX_STATUS ) != ( uint32_t ) 0U )
            {
                pxPhyObject->xPhyProperties.ucDuplex = PHY_DUPLEX_FULL;
            }
            else
            {
                pxPhyObject->xPhyProperties.ucDuplex = PHY_DUPLEX_HALF;
            }

            if( ( ulRegValue & phyPHYSTS_SPEED_STATUS ) != 0 )
            {
                pxPhyObject->xPhyProperties.ucSpeed = PHY_SPEED_10;
            }
            else
            {
                pxPhyObject->xPhyProperties.ucSpeed = PHY_SPEED_100;
            }
        }
    } /* if( ulDoneMask != ( uint32_t) 0U ) */

    return 0;
}
/*-----------------------------------------------------------*/

BaseType_t xPhyCheckLinkStatus( EthernetPhy_t * pxPhyObject,
                                BaseType_t xHadReception )
{
    uint32_t ulStatus, ulBitMask = 1U;
    BaseType_t xPhyIndex;
    BaseType_t xNeedCheck = pdFALSE;

    if( xHadReception > 0 )
    {
        /* A packet was received. No need to check for the PHY status now,
         * but set a timer to check it later on. */
        vTaskSetTimeOutState( &( pxPhyObject->xLinkStatusTimer ) );
        pxPhyObject->xLinkStatusRemaining = pdMS_TO_TICKS( ipconfigPHY_LS_HIGH_CHECK_TIME_MS );

        for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++, ulBitMask <<= 1 )
        {
            if( ( pxPhyObject->ulLinkStatusMask & ulBitMask ) == 0UL )
            {
                pxPhyObject->ulLinkStatusMask |= ulBitMask;
                FreeRTOS_printf( ( "xPhyCheckLinkStatus: PHY LS now %02lX\n", pxPhyObject->ulLinkStatusMask ) );
                xNeedCheck = pdTRUE;
            }
        }
    }
    else if( xTaskCheckForTimeOut( &( pxPhyObject->xLinkStatusTimer ), &( pxPhyObject->xLinkStatusRemaining ) ) != pdFALSE )
    {
        /* Frequent checking the PHY Link Status can affect for the performance of Ethernet controller.
         * As long as packets are received, no polling is needed.
         * Otherwise, polling will be done when the 'xLinkStatusTimer' expires. */
        for( xPhyIndex = 0; xPhyIndex < pxPhyObject->xPortCount; xPhyIndex++, ulBitMask <<= 1 )
        {
            BaseType_t xPhyAddress = pxPhyObject->ucPhyIndexes[ xPhyIndex ];

            if( pxPhyObject->fnPhyRead( xPhyAddress, phyREG_01_BMSR, &ulStatus ) == 0 )
            {
                if( !!( pxPhyObject->ulLinkStatusMask & ulBitMask ) != !!( ulStatus & phyBMSR_LINK_STATUS ) )
                {
                    if( ( ulStatus & phyBMSR_LINK_STATUS ) != 0 )
                    {
                        pxPhyObject->ulLinkStatusMask |= ulBitMask;
                    }
                    else
                    {
                        pxPhyObject->ulLinkStatusMask &= ~( ulBitMask );
                    }

                    FreeRTOS_printf( ( "xPhyCheckLinkStatus: PHY LS now %02lX\n", pxPhyObject->ulLinkStatusMask ) );
                    xNeedCheck = pdTRUE;
                }
            }
        }

        vTaskSetTimeOutState( &( pxPhyObject->xLinkStatusTimer ) );

        if( ( pxPhyObject->ulLinkStatusMask & ( ulBitMask >> 1 ) ) != 0 )
        {
            /* The link status is high, so don't poll the PHY too often. */
            pxPhyObject->xLinkStatusRemaining = pdMS_TO_TICKS( ipconfigPHY_LS_HIGH_CHECK_TIME_MS );
        }
        else
        {
            /* The link status is low, polling may be done more frequently. */
            pxPhyObject->xLinkStatusRemaining = pdMS_TO_TICKS( ipconfigPHY_LS_LOW_CHECK_TIME_MS );
        }
    }

    return xNeedCheck;
}
/*-----------------------------------------------------------*/
