/* clang-format off */
/*******************************************************************************
  SYS PORTS Static Functions for PORTS System Service

  Company:
    Microchip Technology Inc.

  File Name:
    sys_ports_static.c

  Summary:
    SYS PORTS static function implementations for the Ports System Service.

  Description:
    The Ports System Service provides a simple interface to manage the ports
    on Microchip microcontrollers. This file defines the static implementation for the
    Ports System Service.

  Remarks:
    Static functions incorporate all system ports configuration settings as
    determined by the user via the Microchip Harmony Configurator GUI.  It provides
    static version of the routines, eliminating the need for an object ID or
    object handle.

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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

#include "system_config.h"
#include "system/ports/sys_ports.h"
#include "peripheral/devcon/plib_devcon.h"
#include "peripheral/ports/plib_ports.h"
#include "peripheral/int/plib_int.h"

/******************************************************************************
  Function:
    SYS_PORTS_Initialize(void)

  Summary:
    Initializes Ports System Service

  Description:
    This function initializes different port pins/channels to the desired state.
    It also remaps the pins to the desired specific function.

  Remarks:
    None.
*/
void SYS_PORTS_Initialize(void)
{
    /* AN and CN Pins Initialization */
    PLIB_PORTS_AnPinsModeSelect(PORTS_ID_0, SYS_PORT_AD1PCFG, PORTS_PIN_MODE_DIGITAL);
    PLIB_PORTS_CnPinsPullUpEnable(PORTS_ID_0, SYS_PORT_CNPUE);
    PLIB_PORTS_CnPinsEnable(PORTS_ID_0, SYS_PORT_CNEN);
    PLIB_PORTS_ChangeNoticeEnable(PORTS_ID_0);


    /* PORT A Initialization */
    PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_A, SYS_PORT_A_ODC);
    PLIB_PORTS_Write( PORTS_ID_0, PORT_CHANNEL_A,  SYS_PORT_A_LAT);
	PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_A,  SYS_PORT_A_TRIS ^ 0xFFFF);

    /* PORT B Initialization */
    PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_B, SYS_PORT_B_ODC);
    PLIB_PORTS_Write( PORTS_ID_0, PORT_CHANNEL_B,  SYS_PORT_B_LAT);
	PLIB_PORTS_DirectionOutputSet( PORTS_ID_0, PORT_CHANNEL_B,  SYS_PORT_B_TRIS ^ 0xFFFF);

}

/******************************************************************************
  Function:
    PORTS_DATA_TYPE SYS_PORTS_Read( PORTS_MODULE_ID index, PORTS_CHANNEL channel )

  Summary:
    Reads the data from the I/O port.

  Description:
    This function reads the data from the I/O port.

  Remarks:
    None.
*/

PORTS_DATA_TYPE SYS_PORTS_Read( PORTS_MODULE_ID index, PORTS_CHANNEL channel )
{
    return PLIB_PORTS_Read( index, channel );
}


/******************************************************************************
  Function:
    void SYS_PORTS_Write( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                      		PORTS_DATA_TYPE value )

  Summary:
    Writes the data from the I/O port.

  Description:
    This function writes the data to the I/O port.

  Remarks:
    None.
*/

void SYS_PORTS_Write( PORTS_MODULE_ID index,
                      PORTS_CHANNEL channel,
                      PORTS_DATA_TYPE value )
{
    PLIB_PORTS_Write( index, channel, value );
}


/******************************************************************************
  Function:
    void SYS_PORTS_Set( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
	                         PORTS_DATA_TYPE value,
                         PORTS_DATA_MASK mask )

  Summary:
    Sets the selected digital port/latch based on the mask.

  Description:
    This function sets the selected digital port/latch relative to the mask.

  Remarks:
    None.
*/

void SYS_PORTS_Set( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                    PORTS_DATA_TYPE value,
                    PORTS_DATA_MASK mask )
{
    PLIB_PORTS_Set( index, channel, value, mask );
}


/******************************************************************************
  Function:
    void SYS_PORTS_Clear ( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                           PORTS_DATA_MASK clearMask )

  Summary:
    Clears the selected digital port.

  Description:
    This function clears the selected digital port.

  Remarks:
    None.
*/

void SYS_PORTS_Clear ( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                           PORTS_DATA_MASK clearMask )
{
    PLIB_PORTS_Clear ( index, channel, clearMask );
}


/******************************************************************************
  Function:
    void SYS_PORTS_DirectionSelect( PORTS_MODULE_ID index,
								SYS_PORTS_PIN_DIRECTION pinDir,
								PORTS_CHANNEL channel,
								PORTS_DATA_MASK mask )
  Summary:
    Enables the direction for the selected port.

  Description:
    This function enables the direction for the selected port.

  Remarks:
    None.
*/

void SYS_PORTS_DirectionSelect( PORTS_MODULE_ID index,
                             SYS_PORTS_PIN_DIRECTION pinDir,
                             PORTS_CHANNEL channel,
                             PORTS_DATA_MASK mask )
{
    if (pinDir == SYS_PORTS_DIRECTION_INPUT)
    {
        PLIB_PORTS_DirectionInputSet(index, channel, mask);
    }
    else
    {
        PLIB_PORTS_DirectionOutputSet(index, channel, mask);
    }
}


/******************************************************************************
  Function:
    PORTS_DATA_MASK SYS_PORTS_DirectionGet( PORTS_MODULE_ID index,
                                            PORTS_CHANNEL channel )

  Summary:
    Reads the port direction for the selected port.

  Description:
    This function reads the port direction for the selected port.

  Remarks:
    None.
*/

PORTS_DATA_MASK SYS_PORTS_DirectionGet( PORTS_MODULE_ID index, PORTS_CHANNEL channel )
{
    return PLIB_PORTS_DirectionGet( index, channel );
}


/******************************************************************************
  Function:
    void SYS_PORTS_Toggle( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                           PORTS_DATA_MASK toggleMask )

  Summary:
    Toggles the selected digital port pins.

  Description:
    This function toggles the selected digital port pins.

  Remarks:
    None.
*/

void SYS_PORTS_Toggle( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                       PORTS_DATA_MASK toggleMask )
{
    PLIB_PORTS_Toggle( index, channel, toggleMask );
}


/******************************************************************************
  Function:
    void SYS_PORTS_OpenDrainEnable( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                                     PORTS_DATA_MASK mask )

  Summary:
    Enables the open drain functionality for the selected port.

  Description:
    This function enables the open drain functionality for the selected port.

  Remarks:
    None.
*/

void SYS_PORTS_OpenDrainEnable( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                                PORTS_DATA_MASK mask )
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
    if(PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
        PLIB_PORTS_OpenDrainEnable( index, channel, mask );
    }
#endif
}


/******************************************************************************
  Function:
    void SYS_PORTS_OpenDrainDisable( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                                     PORTS_DATA_MASK mask )

  Summary:
    Disables the open drain functionality for the selected port.

  Description:
    This function disables the open drain functionality for the selected port.

  Remarks:
    None.
*/

void SYS_PORTS_OpenDrainDisable( PORTS_MODULE_ID index, PORTS_CHANNEL channel,
                                 PORTS_DATA_MASK mask )
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
    if(PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
        PLIB_PORTS_OpenDrainDisable( index, channel, mask );
    }
#endif
}


// *****************************************************************************
// *****************************************************************************
// Section: SYS Change Notification Pins Routines
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
  Function:
    void SYS_PORTS_ChangeNotificationGlobalEnable( PORTS_MODULE_ID index )

  Summary:
    Globally enables the change notification.

  Description:
    This function globally enables the change notification.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationGlobalEnable( PORTS_MODULE_ID index )
{
#if defined(PLIB_PORTS_ExistsChangeNotice)
    if(PLIB_PORTS_ExistsChangeNotice(index))
    {
        PLIB_PORTS_ChangeNoticeEnable( index );
    }
#endif
}

/******************************************************************************
  Function:
    void SYS_PORTS_ChangeNotificationGlobalDisable( PORTS_MODULE_ID index )

  Summary:
    Globally disables the change notification.

  Description:
    This function globally disables the change notification.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationGlobalDisable( PORTS_MODULE_ID index )
{
#if defined(PLIB_PORTS_ExistsChangeNotice)
    if(PLIB_PORTS_ExistsChangeNotice(index))
    {
        PLIB_PORTS_ChangeNoticeDisable( index );
    }
#endif
}

/******************************************************************************
  Function:
    void SYS_PORTS_GlobalChangeNotificationDisable( PORTS_MODULE_ID index )

  Summary:
    Globally disables the change notification for the selected port.

  Description:
    This function globally disables the change notification for the selected port.

  Remarks:
    None.
*/

void SYS_PORTS_GlobalChangeNotificationDisable( PORTS_MODULE_ID index )
{
#if defined(PLIB_PORTS_ExistsChangeNotice)
    if(PLIB_PORTS_ExistsChangeNotice(index))
    {
        PLIB_PORTS_ChangeNoticeDisable( index );
    }
#endif
}


/******************************************************************************
  Function:
    void SYS_PORTS_ChangeNotificationEnable( PORTS_MODULE_ID index,
                                             PORTS_CHANGE_NOTICE_PIN pinNum,
                                             SYS_PORTS_PULLUP_PULLDOWN_STATUS value )

  Summary:
    Enables the change notification for the selected port.

  Description:
    This function enables the change notification for the selected port.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationEnable( PORTS_MODULE_ID index,
                                         PORTS_CHANGE_NOTICE_PIN pinNum,
                                         SYS_PORTS_PULLUP_PULLDOWN_STATUS value )
{
#if defined(PLIB_PORTS_ExistsChangeNoticePullUp)
    if(PLIB_PORTS_ExistsChangeNoticePullUp(index))
    {
        switch(value)
        {
            case SYS_PORTS_PULLUP_DISABLE:
                PLIB_PORTS_ChangeNoticePullUpDisable(index, pinNum);
                break;
            case SYS_PORTS_PULLUP_ENABLE:
                PLIB_PORTS_ChangeNoticePullUpEnable(index, pinNum);
                break;
        }
    }
#endif

#if defined(PLIB_PORTS_ExistsPinChangeNotice)
    if(PLIB_PORTS_ExistsPinChangeNotice(index))
    {
        PLIB_PORTS_PinChangeNoticeEnable( index, pinNum );
    }
#endif
}


/******************************************************************************
  Function:
    void SYS_PORTS_ChangeNotificationDisable( PORTS_MODULE_ID index,
                                              PORTS_CHANGE_NOTICE_PIN pinNum )

  Summary:
    Disables the change notification for the selected port.

  Description:
    This function disables the change notification for the selected port.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationDisable( PORTS_MODULE_ID index,
                                          PORTS_CHANGE_NOTICE_PIN pinNum )
{
#if defined(PLIB_PORTS_ExistsPinChangeNotice)
    if(PLIB_PORTS_ExistsPinChangeNotice(index))
    {
        PLIB_PORTS_PinChangeNoticeDisable( index, pinNum );
    }
#endif
}


/******************************************************************************
  Function:
    void SYS_PORTS_ChangeNotificationInIdleModeEnable( PORTS_MODULE_ID index )

  Summary:
    Enables the change notification for the selected port in Sleep or Idle mode.

  Description:
    This function enables the change notification for the selected port in Sleep
    or Idle mode.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationInIdleModeEnable( PORTS_MODULE_ID index )
{
#if defined(PLIB_PORTS_ExistsChangeNoticeInIdle)
    if(PLIB_PORTS_ExistsChangeNoticeInIdle(index))
    {
        PLIB_PORTS_ChangeNoticeInIdleEnable( index );
    }
#endif
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_ChangeNotificationInIdleModeDisable( PORTS_MODULE_ID index)

  Summary:
    Disables the change notification for the selected port in Sleep or Idle mode.

  Description:
    This function disables the change notification for the selected port in Sleep
    or Idle mode.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationInIdleModeDisable( PORTS_MODULE_ID index )
{
#if defined(PLIB_PORTS_ExistsChangeNoticeInIdle)
    if(PLIB_PORTS_ExistsChangeNoticeInIdle(index))
    {
        PLIB_PORTS_ChangeNoticeInIdleDisable( index );
    }
#endif
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_ChangeNotificationPullUpEnable ( PORTS_MODULE_ID         index,
                                                    PORTS_CHANGE_NOTICE_PIN pinNum )

  Summary:
    Enables weak pull-up on change notification pin.

  Description:
    This function enables weak pull-up on change notification pin.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationPullUpEnable ( PORTS_MODULE_ID         index,
                                                PORTS_CHANGE_NOTICE_PIN pinNum )
{
#if defined(PLIB_PORTS_ExistsChangeNoticePullUp)
    if(PLIB_PORTS_ExistsChangeNoticePullUp(index))
    {
        PLIB_PORTS_ChangeNoticePullUpEnable ( index, pinNum );
    }
#endif
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_ChangeNotificationPullUpDisable ( PORTS_MODULE_ID         index,
                                                    PORTS_CHANGE_NOTICE_PIN pinNum )

  Summary:
    Disables pull-up on input change.

  Description:
    This function disables pull-up on input change.

  Remarks:
    None.
*/

void SYS_PORTS_ChangeNotificationPullUpDisable ( PORTS_MODULE_ID         index,
                                                PORTS_CHANGE_NOTICE_PIN pinNum )
{
#if defined(PLIB_PORTS_ExistsChangeNoticePullUp)
    if(PLIB_PORTS_ExistsChangeNoticePullUp(index))
    {
        PLIB_PORTS_ChangeNoticePullUpDisable ( index, pinNum );
    }
#endif
}


// *****************************************************************************
// *****************************************************************************
// Section: SYS PORT PINS Control Routines
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SYS_PORTS_PinModeSelect ( PORTS_MODULE_ID index, PORTS_ANALOG_PIN pin,
    								PORTS_PIN_MODE mode)

  Summary:
    Enables the selected pin as analog or digital.

  Description:
    This function enables the selected pin as analog or digital.

 Remarks:
    None.
*/

void SYS_PORTS_PinModeSelect ( PORTS_MODULE_ID index, PORTS_ANALOG_PIN pin,
								PORTS_PIN_MODE mode)
{
#if defined(PLIB_PORTS_ExistsPinMode)
    if(PLIB_PORTS_ExistsPinMode(index))
    {
        PLIB_PORTS_PinModeSelect ( index, pin, mode);
    }
#endif
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinWrite ( PORTS_MODULE_ID index,
                              PORTS_CHANNEL channel,
                              PORTS_BIT_POS bitPos
                              bool value )
  Summary:
    Writes the selected digital pin.

  Description:
    This function writes the selected digital pin.

  Remarks:
    None.
*/

void SYS_PORTS_PinWrite ( PORTS_MODULE_ID index,
                          PORTS_CHANNEL channel,
                          PORTS_BIT_POS bitPos,
                          bool value )
{
    PLIB_PORTS_PinWrite ( index, channel, bitPos, value );
}


// *****************************************************************************
/* Function:
    bool SYS_PORTS_PinRead ( PORTS_MODULE_ID index,
                             PORTS_CHANNEL channel,
                             PORTS_BIT_POS bitPos )

  Summary:
    Reads the selected digital pin.

  Description:
    This function reads the selected digital pin.

  Remarks:
    None.
*/

bool SYS_PORTS_PinRead ( PORTS_MODULE_ID index,
                         PORTS_CHANNEL channel,
                         PORTS_BIT_POS bitPos )
{
    return PLIB_PORTS_PinGet ( index, channel, bitPos );
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinToggle ( PORTS_MODULE_ID index,
                               PORTS_CHANNEL channel,
                               PORTS_BIT_POS bitPos )

  Summary:
    Toggles the selected digital pin.

  Description:
    This function toggles the selected digital pin.

  Remarks:
    None.
*/

void SYS_PORTS_PinToggle ( PORTS_MODULE_ID index,
                           PORTS_CHANNEL channel,
                           PORTS_BIT_POS bitPos )
{
    PLIB_PORTS_PinToggle ( index, channel, bitPos );
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinSet( PORTS_MODULE_ID index,
                           PORTS_CHANNEL channel,
                           PORTS_BIT_POS bitPos )

  Summary:
    Sets the selected digital pin/latch.

  Description:
    This function sets the selected digital pin/latch.

  Remarks:
    None.
*/

void SYS_PORTS_PinSet( PORTS_MODULE_ID index,
                       PORTS_CHANNEL channel,
                       PORTS_BIT_POS bitPos )
{
    PLIB_PORTS_PinSet( index, channel, bitPos );
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinClear ( PORTS_MODULE_ID index,
                              PORTS_CHANNEL channel,
                              PORTS_BIT_POS bitPos )

  Summary:
    Clears the selected digital pin.

  Description:
    This function clears the selected digital pin.

  Remarks:
    None.
*/

void SYS_PORTS_PinClear ( PORTS_MODULE_ID index,
                          PORTS_CHANNEL channel,
                          PORTS_BIT_POS bitPos )
{
    PLIB_PORTS_PinClear ( index, channel, bitPos );
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinDirectionSelect ( PORTS_MODULE_ID index,
                                     SYS_PORTS_PIN_DIRECTION pinDir,
                                     PORTS_CHANNEL channel,
                                     PORTS_BIT_POS bitPos )
  Summary:
    Enables the direction for the selected pin.

  Description:
    This function enables the direction for the selected pin.

  Remarks:
    None.
*/

void SYS_PORTS_PinDirectionSelect ( PORTS_MODULE_ID index,
                                 SYS_PORTS_PIN_DIRECTION pinDir,
                                 PORTS_CHANNEL channel,
                                 PORTS_BIT_POS bitPos )
{
    if (pinDir == SYS_PORTS_DIRECTION_OUTPUT)
    {
        PLIB_PORTS_PinDirectionOutputSet(index, channel, bitPos);
    }
    else
    {
        PLIB_PORTS_PinDirectionInputSet(index, channel, bitPos);
    }
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinOpenDrainEnable ( PORTS_MODULE_ID index,
                                        PORTS_CHANNEL channel,
                                        PORTS_BIT_POS bitPos )

  Summary:
    Enables the open-drain functionality for the selected pin.

  Description:
    This function enables the open-drain functionality for the selected pin.

  Remarks:
    None.
*/

void SYS_PORTS_PinOpenDrainEnable ( PORTS_MODULE_ID index,
                                    PORTS_CHANNEL channel,
                                    PORTS_BIT_POS bitPos )
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
    if(PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
        PLIB_PORTS_PinOpenDrainEnable ( index, channel, bitPos );
    }
#endif
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_PinOpenDrainDisable ( PORTS_MODULE_ID index,
                                         PORTS_CHANNEL channel,
                                         PORTS_BIT_POS bitPos )

  Summary:
    Disables the open-drain functionality for the selected pin.

  Description:
    This function disables the open-drain functionality for the selected pin.

  Remarks:
    None.
*/

void SYS_PORTS_PinOpenDrainDisable ( PORTS_MODULE_ID index,
                                     PORTS_CHANNEL channel,
                                     PORTS_BIT_POS bitPos )
{
#if defined(PLIB_PORTS_ExistsPortsOpenDrain)
    if(PLIB_PORTS_ExistsPortsOpenDrain(index))
    {
        PLIB_PORTS_PinOpenDrainDisable ( index, channel, bitPos );
    }
#endif
}


// *****************************************************************************
/* Function:
    void SYS_PORTS_RemapInput( PORTS_MODULE_ID      index,
							  PORTS_REMAP_INPUT_FUNCTION function,
							  PORTS_REMAP_INPUT_PIN      remapPin )

  Summary:
    Input/Output (I/O) function remapping.

  Description:
    This function controls the I/O function remapping.

  Precondition:
    None.
*/
void SYS_PORTS_RemapInput( PORTS_MODULE_ID      index,
						   PORTS_REMAP_INPUT_FUNCTION function,
						   PORTS_REMAP_INPUT_PIN      remapPin )
{
#if defined(PLIB_PORTS_ExistsRemapInput)
    if(PLIB_PORTS_ExistsRemapInput(index))
    {
        PLIB_DEVCON_SystemUnlock(DEVCON_ID_0);
        PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_ID_0, DEVCON_PPS_REGISTERS);
        PLIB_PORTS_RemapInput( index, function, remapPin);
    }
#endif
}

// *****************************************************************************
/* Function:
    void SYS_PORTS_RemapOutput( PORTS_MODULE_ID      index,
                                      PORTS_REMAP_OUTPUT_FUNCTION function,
                                      PORTS_REMAP_OUTPUT_PIN      remapPin )

  Summary:
    Input/Output (I/O) function remapping.

  Description:
    This function controls the I/O function remapping.

  Precondition:
    None.
*/
void SYS_PORTS_RemapOutput( PORTS_MODULE_ID      index,
						    PORTS_REMAP_OUTPUT_FUNCTION function,
						    PORTS_REMAP_OUTPUT_PIN      remapPin )
{
#if defined(PLIB_PORTS_ExistsRemapOutput)
    if(PLIB_PORTS_ExistsRemapOutput(index))
    {
        PLIB_DEVCON_SystemUnlock(DEVCON_ID_0);
        PLIB_DEVCON_DeviceRegistersUnlock(DEVCON_ID_0, DEVCON_PPS_REGISTERS);
        PLIB_PORTS_RemapOutput( index, function, remapPin);
    }
#endif
}

/*******************************************************************************
 End of File
*/
