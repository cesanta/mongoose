/* clang-format off */
/*******************************************************************************
  SPI Driver Interface Implementation

  Company:
    Microchip Technology Inc.

  File Name:
   drv_spi_api.i

  Summary:
 SPI Driver implementation for functions that the client API uses that change
 depending on what compile time options there are.

  Description:
    The SPI Driver provides a interface to access the SPI hardware on the PIC32
    microcontroller.  This file implements the SPI Driver. This file
    should be included in the project if SPI driver functionality is needed.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
//DOM-IGNORE-END

#include "driver/spi/src/dynamic/drv_spi_internal.h"

#define _SPI_DRV_VTABLE_POLLED      0x0000
#define _SPI_DRV_VTABLE_ISR         0x0001

#define _SPI_DRV_VTABLE_SLAVE       0x0000
#define _SPI_DRV_VTABLE_MASTER      0x0002

#define _SPI_DRV_VTABLE_RM          0x0000
#define _SPI_DRV_VTABLE_EBM         0x0004

#define _SPI_DRV_VTABLE_8BIT        0x0008
#define _SPI_DRV_VTABLE_16BIT       0x0010
#define _SPI_DRV_VTABLE_32BIT       0x0020

#define _SPI_DRV_VTABLE_P_S_R_8 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_P_S_R_16 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_P_S_R_32 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_P_S_E_8 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_P_S_E_16 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_P_S_E_32 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_I_S_R_8 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_I_S_R_16 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_I_S_R_32 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_I_S_E_8 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_I_S_E_16 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_I_S_E_32 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_SLAVE | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_P_M_R_8 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_P_M_R_16 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_P_M_R_32 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_P_M_E_8 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_P_M_E_16 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_P_M_E_32 _SPI_DRV_VTABLE_POLLED | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_I_M_R_8 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_I_M_R_16 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_I_M_R_32 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_RM | _SPI_DRV_VTABLE_32BIT

#define _SPI_DRV_VTABLE_I_M_E_8 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_8BIT
#define _SPI_DRV_VTABLE_I_M_E_16 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_16BIT
#define _SPI_DRV_VTABLE_I_M_E_32 _SPI_DRV_VTABLE_ISR | _SPI_DRV_VTABLE_MASTER | _SPI_DRV_VTABLE_EBM | _SPI_DRV_VTABLE_32BIT

int32_t DRV_SPI_SetVTable(struct DRV_SPI_DRIVER_OBJECT * driverObject, const DRV_SPI_INIT * const pInit)
{
    uint8_t mode = 0;
    if (pInit->spiMode == DRV_SPI_MODE_MASTER)
    {
        mode = _SPI_DRV_VTABLE_MASTER;
    }
    else
    {
        SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
        return -1;
    }

    if (pInit->taskMode == DRV_SPI_TASK_MODE_ISR)
    {
        mode |= DRV_SPI_TASK_MODE_ISR;
    }
    else
    {
        SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
        return -1;
    }

    if (pInit->bufferType == DRV_SPI_BUFFER_TYPE_ENHANCED)
    {
        mode |= _SPI_DRV_VTABLE_EBM;
    }
    else
    {
        SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
        return -1;
    }

    if (pInit->commWidth == SPI_COMMUNICATION_WIDTH_8BITS)
    {
        mode |= _SPI_DRV_VTABLE_8BIT;
    }
    else
    {
        SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
        return -1;
    }
    switch (mode)
    {
    case _SPI_DRV_VTABLE_I_M_E_8:
        driverObject->vfMainTask = DRV_SPI_ISRMasterEBM8BitTasks;
        break;
    default:
        SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
        return -1;
    }
    return 0;
}

int32_t DRV_SPI_SetupHardware(struct DRV_SPI_DRIVER_OBJECT * driverObject, const DRV_SPI_INIT * const init)
{
    const register SPI_MODULE_ID spiId = init->spiId;

/* disable the SPI*/
    PLIB_SPI_Disable(spiId);

    /* Set up Master or Slave Mode*/
    PLIB_SPI_MasterEnable ( spiId  );

    /* Set up if the SPI is allowed to run while the rest of the CPU is in idle mode*/
    if (init->allowIdleRun)
    {
        PLIB_SPI_StopInIdleDisable( spiId  );
    }
    else
    {
        PLIB_SPI_StopInIdleEnable( spiId  );
    }

    /* Set up close Polarity and output data phase*/
    switch(init->clockMode)
    {
        case DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_RISE:
            PLIB_SPI_ClockPolaritySelect( spiId, SPI_CLOCK_POLARITY_IDLE_LOW );
            PLIB_SPI_OutputDataPhaseSelect( spiId, SPI_OUTPUT_DATA_PHASE_ON_IDLE_TO_ACTIVE_CLOCK );
            break;
        case DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_FALL:
            PLIB_SPI_ClockPolaritySelect( spiId, SPI_CLOCK_POLARITY_IDLE_LOW );
            PLIB_SPI_OutputDataPhaseSelect( spiId, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK );
            break;
        case DRV_SPI_CLOCK_MODE_IDLE_HIGH_EDGE_FALL: //TODO: Make sure these are right
            PLIB_SPI_ClockPolaritySelect( spiId, SPI_CLOCK_POLARITY_IDLE_HIGH );
            PLIB_SPI_OutputDataPhaseSelect( spiId, SPI_OUTPUT_DATA_PHASE_ON_IDLE_TO_ACTIVE_CLOCK );
            break;
        case DRV_SPI_CLOCK_MODE_IDLE_HIGH_EDGE_RISE:
            PLIB_SPI_ClockPolaritySelect( spiId, SPI_CLOCK_POLARITY_IDLE_HIGH );
            PLIB_SPI_OutputDataPhaseSelect( spiId, SPI_OUTPUT_DATA_PHASE_ON_ACTIVE_TO_IDLE_CLOCK );
            break;
        default:
             SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
            return -1;
    }

    // Set up the  Input Sample Phase
    PLIB_SPI_InputSamplePhaseSelect ( spiId, init->inputSamplePhase);

    //Enable the SSx Pin on slave side if needed
    if(init->spiMode == DRV_SPI_MODE_SLAVE && (init->spiProtocolType == DRV_SPI_PROTOCOL_TYPE_FRAMED || init->spiSlaveSSPin))
    {
        PLIB_SPI_PinEnable(spiId, SPI_PIN_SLAVE_SELECT);
    }
    else
    {
        PLIB_SPI_PinDisable(spiId, SPI_PIN_SLAVE_SELECT);
    }

    /* Communication Width Selection */
    PLIB_SPI_CommunicationWidthSelect ( spiId, init->commWidth );

    /* Baudrate selection */
    PLIB_SPI_BaudRateSet( spiId , SYS_CLK_PeripheralFrequencyGet(init->spiClk), init->baudRate );
    driverObject->currentBaudRate = init->baudRate;
    driverObject->baudRate = init->baudRate;

    switch (init->spiProtocolType)
    {
        case DRV_SPI_PROTOCOL_TYPE_STANDARD:
             PLIB_SPI_FramedCommunicationDisable( spiId  );
             break;

        case DRV_SPI_PROTOCOL_TYPE_FRAMED:

            #if defined (PLIB_SPI_ExistsFrameSyncPulseDirection)
                if (PLIB_SPI_ExistsFrameSyncPulseDirection(spiId))
                {
                    PLIB_SPI_FrameSyncPulseDirectionSelect(spiId, init->framePulseDirection);
                }
            #endif
            #if defined (PLIB_SPI_ExistsFrameSyncPulsePolarity)
                if (PLIB_SPI_ExistsFrameSyncPulsePolarity(spiId))
                {
                    PLIB_SPI_FrameSyncPulsePolaritySelect(spiId, init->framePulsePolarity);
                }
            #endif
            #if defined (PLIB_SPI_ExistsFrameSyncPulseEdge)
                if (PLIB_SPI_ExistsFrameSyncPulseEdge(spiId))
                {
                    PLIB_SPI_FrameSyncPulseEdgeSelect(spiId, init->framePulseEdge);
                }
            #endif
            #if defined (PLIB_SPI_ExistsFrameSyncPulseWidth)
                if (PLIB_SPI_ExistsFrameSyncPulseWidth(spiId))
                {
                    PLIB_SPI_FrameSyncPulseWidthSelect(spiId, init->framePulseWidth);
                }
            #endif
            #if defined (PLIB_SPI_ExistsFrameSyncPulseCounter)
                if (PLIB_SPI_ExistsFrameSyncPulseCounter(spiId))
                {
                    PLIB_SPI_FrameSyncPulseCounterSelect(spiId, init->frameSyncPulse);
                }
            #endif


            PLIB_SPI_FramedCommunicationEnable( spiId  );
            break;

        case DRV_SPI_PROTOCOL_TYPE_AUDIO:
             PLIB_SPI_FramedCommunicationDisable( spiId  );

             {
                 SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
                return -1;
             }
             break;
        default:
             SYS_ASSERT(false, "\r\nInvalid SPI Configuration.");
            return -1;
    }

    PLIB_SPI_FIFOEnable( spiId  );
    PLIB_SPI_FIFOInterruptModeSelect(spiId, SPI_FIFO_INTERRUPT_WHEN_TRANSMIT_BUFFER_IS_COMPLETELY_EMPTY);
    PLIB_SPI_FIFOInterruptModeSelect(spiId, SPI_FIFO_INTERRUPT_WHEN_RECEIVE_BUFFER_IS_NOT_EMPTY);

    PLIB_SPI_BufferClear( spiId );
    PLIB_SPI_ReceiverOverflowClear ( spiId );

    // Note: We do not enable the SPI here, that will be done by the first client.
    return 0;
}


