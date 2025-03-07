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
 * \file  mcuxCsslDataIntegrity_Cfg.h
 * \brief Configuration of the implementation for the data integrity mechanism.
 */

#ifndef MCUXCSSLDATAINTEGRITY_CFG_H_
#define MCUXCSSLDATAINTEGRITY_CFG_H_

/**
 * \addtogroup mcuxCsslCFG MCUX CSSL -- Configurations
 *
 * \defgroup mcuxCsslDataIntegrity_CFG Data Integrity Configuration
 * \brief Configuration options for the data integrity mechanism.
 * \ingroup mcuxCsslCFG
 */

/**
 * \def MCUX_CSSL_DI_USE_SECURE_COUNTER
 * \brief If set to 1, use the data integrity mechanism implementation based on
 *        the CSSL secure counter mechanism.
 * \ingroup mcuxCsslDataIntegrity_CFG
 */
    #define MCUX_CSSL_DI_USE_SECURE_COUNTER      0

/**
 * \def MCUX_CSSL_DI_USE_NONE
 * \brief If set to 1, do not use the data integrity mechanism.
 * \ingroup mcuxCsslDataIntegrity_CFG
 */
    #define MCUX_CSSL_DI_USE_NONE    1

/* Basic configuration sanity check */

#endif /* MCUXCSSLDATAINTEGRITY_CFG_H_ */
