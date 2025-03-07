/*--------------------------------------------------------------------------*/
/* Copyright 2020 NXP                                                       */
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
 * \file  mcuxCsslFlowProtection_Cfg.h
 * \brief Configuration of the implementation for the flow protection mechanism.
 */

#ifndef MCUX_CSSL_FLOW_PROTECTION_CFG_H_
#define MCUX_CSSL_FLOW_PROTECTION_CFG_H_

/**
 * \addtogroup mcuxCsslCFG MCUX CSSL -- Configurations
 *
 * \defgroup mcuxCsslFlowProtection_CFG Flow Protection Configuration
 * \brief Configuration options for the flow protection mechanism.
 * \ingroup mcuxCsslCFG
 */

/**
 * \def MCUX_CSSL_FP_USE_CODE_SIGNATURE
 * \brief If set to 1, use the flow protection mechanism implementation based on
 *        the Zen-V code signature HW mechanism.
 * \ingroup mcuxCsslFlowProtection_CFG
 */
    #define MCUX_CSSL_FP_USE_CODE_SIGNATURE      0

/**
 * \def MCUX_CSSL_FP_USE_SECURE_COUNTER
 * \brief If set to 1, use the flow protection mechanism implementation based on
 *        the CSSL secure counter mechanism.
 * \ingroup mcuxCsslFlowProtection_CFG
 */
    #define MCUX_CSSL_FP_USE_SECURE_COUNTER      1

/**
 * \def MCUX_CSSL_FP_USE_NONE
 * \brief If set to 1, do not use the flow protection mechanism.
 * \ingroup mcuxCsslFlowProtection_CFG
 */
    #define MCUX_CSSL_FP_USE_NONE      0

/* Basic configuration sanity check */

#endif /* MCUX_CSSL_FLOW_PROTECTION_CFG_H_ */
