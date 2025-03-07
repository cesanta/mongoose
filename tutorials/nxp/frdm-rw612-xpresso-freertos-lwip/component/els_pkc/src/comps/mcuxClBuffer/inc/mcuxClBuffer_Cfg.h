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

/**
 * \file  mcuxClBuffer_Cfg.h
 * \brief Configuration of the implementation for the buffer types.
 */

#ifndef MCUXCLBUFFER_CFG_H_
#define MCUXCLBUFFER_CFG_H_

#include <mcuxClCore_Platform.h>

/**
 * \addtogroup mcuxClCFG MCUX CL -- Configurations
 *
 * \defgroup mcuxClBuffer_CFG Buffer Configuration
 * \brief Configuration options for the buffer types.
 * \ingroup mcuxClCFG
 */

/**
 * \def MCUXCLBUFFER_USE_OBJECT
 * \brief If set to 1, use the object oriented buffer implementation.
 * \ingroup mcuxClBuffer_CFG
 */
    #define MCUXCLBUFFER_USE_OBJECT 0

/**
 * \def MCUXCLBUFFER_USE_POINTER
 * \brief If set to 1, use the plain C pointer buffer implementation.
 * \ingroup mcuxClBuffer_CFG
 */
    #define MCUXCLBUFFER_USE_POINTER 1

/* Basic configuration sanity check */

#endif /* MCUXCLBUFFER_CFG_H_ */
