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
 * \file  mcuxCsslDataIntegrity_Impl.h
 * \brief Selection of the implementation for the data integrity mechanism.
 */

#ifndef MCUXCSSLDATAINTEGRITY_IMPL_H_
#define MCUXCSSLDATAINTEGRITY_IMPL_H_

/* Include the configuration for the data integrity mechanism. */
#include <mcuxCsslDataIntegrity_Cfg.h>

/* Include the selected implementation of the data integrity mechanism. */
#if defined(MCUX_CSSL_DI_USE_SECURE_COUNTER) && (1 == MCUX_CSSL_DI_USE_SECURE_COUNTER)
#  include <mcuxCsslDataIntegrity_SecureCounter.h>
#elif defined(MCUX_CSSL_DI_USE_NONE) && (1 == MCUX_CSSL_DI_USE_NONE)
#  include <mcuxCsslDataIntegrity_None.h>
#else
  #error "No data integrity implementation found/configured."
#endif

#endif /* MCUXCSSLDATAINTEGRITY_IMPL_H_ */
