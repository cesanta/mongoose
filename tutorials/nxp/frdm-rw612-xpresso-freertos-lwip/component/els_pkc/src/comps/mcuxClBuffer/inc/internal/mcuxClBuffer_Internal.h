/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
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
 * \file  mcuxClBuffer_Internal.h
 * \brief Internal functions of the mcuxClBuffer component
 */

#ifndef MCUXCLBUFFER_INTERNAL_H_
#define MCUXCLBUFFER_INTERNAL_H_

#include <mcuxClCore_Platform.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Include the configuration for the buffer types. */
#include <mcuxClBuffer_Cfg.h>

/* Include the selected implementation of the buffer types. */
#if defined(MCUXCLBUFFER_USE_OBJECT) && (1 == MCUXCLBUFFER_USE_OBJECT)
#  include <internal/mcuxClBuffer_Internal_Object.h>
#elif defined(MCUXCLBUFFER_USE_POINTER) && (1 == MCUXCLBUFFER_USE_POINTER)
#  include <internal/mcuxClBuffer_Internal_Pointer.h>
#else
#  error "No buffer type implementation found/configured."
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLBUFFER_INTERNAL_H_ */

