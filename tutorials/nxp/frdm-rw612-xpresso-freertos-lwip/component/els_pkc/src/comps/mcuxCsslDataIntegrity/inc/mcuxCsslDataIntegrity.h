/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
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
 * @file  mcuxCsslDataIntegrity.h
 * @brief Provides the API for the CSSL data integrity mechanism.
 */

#ifndef MCUXCSSLDATAINTEGRITY_H_
#define MCUXCSSLDATAINTEGRITY_H_

/* Include the Secure Counter definitions */
#include <mcuxCsslSecureCounter.h>

/* Include the actual implementation of the data integrity mechanism. */
#include <mcuxCsslDataIntegrity_Impl.h>

/**
 * @addtogroup mcuxCsslAPI MCUX CSSL -- API
 *
 * @defgroup mcuxCsslDataIntegrity Data Integrity API
 * @brief Data integrity mechanism.
 * @ingroup mcuxCsslAPI
 */


/**
 * @defgroup diCore Data integrity core functionality
 * @brief Data integrity handling core functionality.
 * @ingroup mcuxCsslDataIntegrity
 */

/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/**
 * @def MCUX_CSSL_DI_CHECK_PASSED
 * @brief Positive comparison result value.
 * @api
 * @ingroup diCore
 */
#define MCUX_CSSL_DI_CHECK_PASSED \
  MCUX_CSSL_DI_CHECK_PASSED_IMPL

/**
 * @def MCUX_CSSL_DI_CHECK_FAILED
 * @brief Negative comparison result value.
 * @api
 * @ingroup diCore
 */
#define MCUX_CSSL_DI_CHECK_FAILED \
  MCUX_CSSL_DI_CHECK_FAILED_IMPL

/****************************************************************************/
/* Initialization                                                           */
/****************************************************************************/

/**
 * @def MCUX_CSSL_DI_INIT
 * @brief Backup of the current data integrity value, that will be checked later
 *        on with MCUX_CSSL_SC_CHECK.
 *        Note that in case the Security Counter back-end requires allocation, it
 *        is expected that this will be handled by the Flow Protection mechanism
 *        before the initialization of the Data Integrity.
 * @api
 * @ingroup diCore
 *
 * @param backupValue Fresh variable name to store the current DI value.
 */
#define MCUX_CSSL_DI_INIT(backupValue) \
  MCUX_CSSL_DI_INIT_IMPL(backupValue)

/****************************************************************************/
/* Check                                                                    */
/****************************************************************************/

/**
 * @def MCUX_CSSL_DI_CHECK
 * @brief Comparison operation for the data integrity.
 * @api
 * @ingroup diCore
 *
 * @param reference Reference initial value to compare the data integrity value against.
 * @return          Either #MCUX_CSSL_DI_CHECK_PASSED, if the value matches, or
 *                  #MCUX_CSSL_DI_CHECK_FAILED if the value is different.
 */
#define MCUX_CSSL_DI_CHECK(reference) \
  MCUX_CSSL_DI_CHECK_IMPL(reference)

/**
 * @def MCUX_CSSL_DI_CHECK_EXIT
 * @brief Comparison operation for the data integrity.
 *        It compares the data integrity value to reference initial value,
 *        and exits with the given fault status code if the comparison fails.
 *        The reference value shall be the initial value that was backed up
 *        with MCUX_CSSL_DI_INIT.
 *        If the comparison succeeds, the normal execution will continue.
 * @api
 * @ingroup diCore
 *
 * @param id        Identifier of the function from which we will exit.
 * @param reference Reference initial value to compare the data integrity value against.
 * @param fail      Result that should be returned if the data integrity check failed.
 */
#define MCUX_CSSL_DI_CHECK_EXIT(id, reference, fail) \
  MCUX_CSSL_DI_CHECK_EXIT_IMPL(id, reference, fail)

/****************************************************************************/
/* Updates                                                                  */
/****************************************************************************/

/**
 * @defgroup diUpdate Data integrity record
 * @brief Support for recording a value in the data integrity register
 * @ingroup mcuxCsslDataIntegrity
 */

/**
 * @def MCUX_CSSL_DI_RECORD
 * @brief Record the @p value for data integrity checking.
 * @api
 * @ingroup diUpdate
 *
 * @param identifier Identifier for the @p value that will be recorded.
 * @param value Value which needs to be recorded for the given @p identifier.
 */
#define MCUX_CSSL_DI_RECORD(identifier, value) \
  MCUX_CSSL_DI_RECORD_IMPL(identifier, value)

/**
 * @def MCUX_CSSL_DI_EXPUNGE
 * @brief Expunge the record for @p value.
 * @api
 * @ingroup diUpdate
 *
 * @param identifier Identifier for the @p value that will be expunged.
 * @param value Expected value that was recorded for the given @p identifier.
 */
#define MCUX_CSSL_DI_EXPUNGE(identifier, value) \
  MCUX_CSSL_DI_EXPUNGE_IMPL(identifier, value)

#endif /* MCUXCSSLDATAINTEGRITY_H_ */
