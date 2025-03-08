/*--------------------------------------------------------------------------*/
/* Copyright 2022-2023 NXP                                                  */
/*                                                                          */
/* NXP Proprietary. This software is owned or controlled by NXP and may     */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms.  If you do not agree to be bound by the applicable        */
/* license terms, then you may not retain, install, activate or otherwise   */
/* use the software.                                                        */
/*--------------------------------------------------------------------------*/

#ifndef MCUXCLCORE_TOOLCHAIN_H_
#define MCUXCLCORE_TOOLCHAIN_H_

#include <mcuxClCore_Platform.h>
#include <mcuxCsslFlowProtection.h>

MCUX_CSSL_FP_FUNCTION_DEF(mcuxCl_Core_Swap64)
static inline uint64_t mcuxCl_Core_Swap64(uint64_t value)
{
    return __builtin_bswap64(value);
}

MCUX_CSSL_FP_FUNCTION_DEF(mcuxCl_Core_Swap32)
static inline uint32_t mcuxCl_Core_Swap32(uint32_t value)
{
    return __builtin_bswap32(value);
}


#endif /* MCUXCLCORE_TOOLCHAIN_H_ */
