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
 * \file  mcuxCsslDataIntegrity_None.h
 * \brief Implementation that disables the CSSL data integrity mechanism.
 */

#ifndef MCUXCSSLDATAINTEGRITY_NONE_H_
#define MCUXCSSLDATAINTEGRITY_NONE_H_

/**
 * \addtogroup mcuxCsslIMPL MCUX CSSL -- Implementations
 *
 * \defgroup mcuxCsslDataIntegrity_None Data Integrity: Disabled
 * \brief Disable the data integrity mechanism.
 * \ingroup mcuxCsslIMPL
 */


/**
 * \defgroup diNoneCore Data integrity core functionality
 * \brief Data integrity handling core functionality, when data integrity is disabled.
 * \ingroup mcuxCsslDataIntegrity_None
 */

/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/**
 * \def MCUX_CSSL_DI_CHECK_PASSED_IMPL
 * \brief Positive comparison result value.
 * \ingroup diNoneCore
 */
#define MCUX_CSSL_DI_CHECK_PASSED_IMPL (MCUX_CSSL_SC_CHECK_PASSED)

/**
 * \def MCUX_CSSL_DI_CHECK_FAILED_IMPL
 * \brief Negative comparison result value.
 * \ingroup diNoneCore
 */
#define MCUX_CSSL_DI_CHECK_FAILED_IMPL (MCUX_CSSL_SC_CHECK_FAILED)

/****************************************************************************/
/* Initialization                                                           */
/****************************************************************************/

/**
 * \def MCUX_CSSL_DI_INIT_IMPL
 * \brief Initialize the backup of the data integrity value to zero.
 * \ingroup diNoneCore
 *
 * @param backupValue Fresh variable name to store the current DI value.
 */
#define MCUX_CSSL_DI_INIT_IMPL(backupValue) \
  uint32_t backupValue = 0u

/****************************************************************************/
/* Check                                                                    */
/****************************************************************************/

/**
 * \def MCUX_CSSL_DI_CHECK_IMPL
 * \brief Comparison operation implementation for the data integrity.
 * \ingroup diNoneCore
 *
 * \param reference Reference initial value to compare the data integrity value against (ignored).
 * \return          Always #MCUX_CSSL_DI_CHECK_PASSED.
 */
#define MCUX_CSSL_DI_CHECK_IMPL(reference) \
  (MCUX_CSSL_DI_CHECK_PASSED_IMPL)

/**
 * \def MCUX_CSSL_DI_CHECK_EXIT_IMPL
 * \brief Comparison operation implementation for the data integrity.
 *        It has no impact on the normal execution of the calling function.
 * \ingroup diNoneCore
 *
 * \param id        Identifier of the function from which we will exit (ignored).
 * \param reference Reference initial value to compare the data integrity value against.
 * \param fail      Result that should be returned if the data integrity check failed (ignored).
 */
#define MCUX_CSSL_DI_CHECK_EXIT_IMPL(id, reference, fail) \
  (void)(reference)

/****************************************************************************/
/* Updates                                                                  */
/****************************************************************************/

/**
 * \defgroup diNoneUpdate Data integrity record
 * \brief Support for recording a value in the data integrity register, when data integrity is disabled.
 * \ingroup mcuxCsslDataIntegrity_None
 */

/**
 * \def MCUX_CSSL_DI_RECORD_IMPL
 * \brief Implementation: Record the value for data integrity checking.
 * \ingroup diNoneUpdate
 *
 * \param identifier Identifier for the value that will be recorded.
 * \param value      Value which needs to be recorded for the given identifier.
 */
#define MCUX_CSSL_DI_RECORD_IMPL(identifier, value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_DI_CAST_POINTERS() \
  (void)(value) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_DI_CAST_POINTERS()

/**
 * \def MCUX_CSSL_DI_EXPUNGE_IMPL
 * \brief Implementation: Expunge the record for value.
 * \ingroup diNoneUpdate
 *
 * \param identifier Identifier for the value that will be expunged.
 * \param value      Expected value that was recorded for the given identifier.
 */
#define MCUX_CSSL_DI_EXPUNGE_IMPL(identifier, value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_DI_CAST_POINTERS() \
  (void)(value) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_DI_CAST_POINTERS()


#endif /* MCUXCSSLDATAINTEGRITY_NONE_H_ */
