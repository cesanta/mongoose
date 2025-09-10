/*--------------------------------------------------------------------------*/
/* Copyright 2020-2021, 2023 NXP                                            */
/*                                                                          */
/* NXP Confidential. This software is owned or controlled by NXP and may    */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms. If you do not agree to be bound by the applicable license */
/* terms, then you may not retain, install, activate or otherwise use the   */
/* software.                                                                */
/*--------------------------------------------------------------------------*/

/**
 * @file  mcuxClMemory_Internal.h
 * @brief Top-level include file for the internal memory operations.
 */

#ifndef MCUXCLMEMORY_INTERNAL_H
#define MCUXCLMEMORY_INTERNAL_H

#include <mcuxClConfig.h> // Exported features flags header
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h>
#include <mcuxClMemory_Constants.h>
#include <mcuxClMemory_Types.h>

#include <internal/mcuxClMemory_Clear_Internal.h>
#include <internal/mcuxClMemory_Compare_Internal.h>
#include <internal/mcuxClMemory_Copy_Internal.h>
#include <internal/mcuxClMemory_Set_Internal.h>
#include <internal/mcuxClMemory_Copy_Reversed_Internal.h>
#include <internal/mcuxClMemory_CopyWords_Internal.h>

#include <internal/mcuxClMemory_ClearSecure_Internal.h>
#include <internal/mcuxClMemory_CompareDPASecure_Internal.h>
#include <internal/mcuxClMemory_CompareSecure_Internal.h>
#include <internal/mcuxClMemory_CopySecure_Internal.h>
#include <internal/mcuxClMemory_SetSecure_Internal.h>
#include <internal/mcuxClMemory_CopySecure_Reversed_Internal.h>
#include <internal/mcuxClMemory_CopySecurePow2_Internal.h>

#endif /* MCUXCLMEMORY_INTERNAL_H */
