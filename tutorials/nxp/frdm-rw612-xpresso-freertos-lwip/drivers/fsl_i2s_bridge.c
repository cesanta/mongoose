/*
 * Copyright 2023 NXP
 *  
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_i2s_bridge.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.i2s_bridge"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void I2S_BRIDGE_SetFlexcommSignalShareSet(uint32_t flexCommIndex, i2s_bridge_signal_t signal, uint32_t set)
{
    uint32_t tempReg = SYSCTL1->FCCTRLSEL[flexCommIndex];

    tempReg &= ~((uint32_t)SYSCTL1_FCCTRLSEL_SCKINSEL_MASK << ((uint32_t)signal * 8U));
    tempReg |= set << ((uint32_t)signal * 8U);

    SYSCTL1->FCCTRLSEL[flexCommIndex] = tempReg;
}

void I2S_BRIDGE_SetFlexcommShareSet(
    uint32_t flexCommIndex, uint32_t sckSet, uint32_t wsSet, uint32_t dataInSet, uint32_t dataOutSet)
{
    uint32_t tempReg = SYSCTL1->FCCTRLSEL[flexCommIndex];

    tempReg &= ~(SYSCTL1_FCCTRLSEL_SCKINSEL_MASK | SYSCTL1_FCCTRLSEL_WSINSEL_MASK | SYSCTL1_FCCTRLSEL_DATAINSEL_MASK |
                 SYSCTL1_FCCTRLSEL_DATAOUTSEL_MASK);
    tempReg |= SYSCTL1_FCCTRLSEL_SCKINSEL(sckSet) | SYSCTL1_FCCTRLSEL_WSINSEL(wsSet) |
               SYSCTL1_FCCTRLSEL_DATAINSEL(dataInSet) | SYSCTL1_FCCTRLSEL_DATAOUTSEL(dataOutSet);

    SYSCTL1->FCCTRLSEL[flexCommIndex] = tempReg;
}

void I2S_BRIDGE_SetShareSetSrc(
    uint32_t setIndex, uint32_t sckShareSrc, uint32_t wsShareSrc, uint32_t dataInShareSrc, uint32_t dataOutShareSrc)
{
    if (setIndex > 0U)
    {
        uint32_t tempReg = 0U;

        /* WS,SCK,DATA IN */
        tempReg |= SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL(sckShareSrc) | SYSCTL1_SHAREDCTRLSET_SHAREDWSSEL(wsShareSrc) |
                   SYSCTL1_SHAREDCTRLSET_SHAREDDATASEL(dataInShareSrc);
        /* data out */
        tempReg |= dataOutShareSrc;

        SYSCTL1->SHAREDCTRLSET[setIndex - 1U] = tempReg;
    }
}

void I2S_BRIDGE_SetShareSignalSrc(uint32_t setIndex, i2s_bridge_signal_t signal, uint32_t shareSrc)
{
    if (setIndex > 0U)
    {
        uint32_t tempReg = SYSCTL1->SHAREDCTRLSET[setIndex - 1U];

        if (signal == kI2S_BRIDGE_SignalDataOut)
        {
            tempReg |= 1UL << (SYSCTL1_SHAREDCTRLSET_FC0DATAOUTEN_SHIFT + shareSrc);
        }
        else
        {
            tempReg &= ~((uint32_t)SYSCTL1_SHAREDCTRLSET_SHAREDSCKSEL_MASK << ((uint32_t)signal * 4U));
            tempReg |= shareSrc << ((uint32_t)signal * 4U);
        }

        SYSCTL1->SHAREDCTRLSET[setIndex - 1U] = tempReg;
    }
}