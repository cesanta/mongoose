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

/** \file  mcuxClBuffer_Constants.h
 *  \brief Constants for use with the mcuxClBuffer component */

#ifndef MCUXCLBUFFER_CONSTANTS_H_
#define MCUXCLBUFFER_CONSTANTS_H_

#include <mcuxClCore_Platform.h>

/**
 * \defgroup clBufferConstants Buffer constants
 * \brief Buffer constants
 * \ingroup mcuxClBuffer
 */

/**
 * \brief Buffer status code
 *
 * This type provides information about the status of the Buffer operation that
 * has been performed.
 */
typedef uint32_t mcuxClBuffer_Status_t;

/* Error codes */
#define MCUXCLBUFFER_STATUS_ERROR                        ((mcuxClBuffer_Status_t) 0x01235330u)
#define MCUXCLBUFFER_STATUS_OK                           ((mcuxClBuffer_Status_t) 0x01232E03u)
#define MCUXCLBUFFER_STATUS_FAULT                        ((mcuxClBuffer_Status_t) 0x0123F0F0u)

#endif /* MCUXCLBUFFER_CONSTANTS_H_ */
