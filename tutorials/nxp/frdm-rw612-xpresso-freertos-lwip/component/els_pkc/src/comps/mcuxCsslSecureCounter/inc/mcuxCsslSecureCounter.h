/*--------------------------------------------------------------------------*/
/* Copyright 2020-2023 NXP                                                  */
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
 * @file  mcuxCsslSecureCounter.h
 * @brief Provides the API for the CSSL secure counter mechanism.
 */

#ifndef MCUXCSSLSECURECOUNTER_H_
#define MCUXCSSLSECURECOUNTER_H_

/* Include the actual implementation of the secure counter mechanism. */
#include <mcuxCsslSecureCounter_Impl.h>

/**
 * @addtogroup mcuxCsslAPI MCUX CSSL -- API
 *
 * @defgroup mcuxCsslSecureCounter Secure Counter API
 * @brief Secure counter mechanism.
 * @ingroup mcuxCsslAPI
 */


/**
 * @defgroup scCore Secure counter core functionality
 * @brief Secure counter handling core functionality.
 * @ingroup mcuxCsslSecureCounter
 *
 * @todo Extend this description of the core functionality.
 */

/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/**
 * @def MCUX_CSSL_SC_CHECK_PASSED
 * @brief Positive comparison result value.
 * @api
 * @ingroup scCore
 */
#define MCUX_CSSL_SC_CHECK_PASSED \
  MCUX_CSSL_SC_CHECK_PASSED_IMPL

/**
 * @def MCUX_CSSL_SC_CHECK_FAILED
 * @brief Negative comparison result value.
 * @api
 * @ingroup scCore
 */
#define MCUX_CSSL_SC_CHECK_FAILED \
  MCUX_CSSL_SC_CHECK_FAILED_IMPL

/**
 * @def MCUX_CSSL_SC_VALUE_TYPE
 * @brief Data type used for the secure counter values.
 * @api
 * @ingroup scCore
 */
#define MCUX_CSSL_SC_VALUE_TYPE \
  MCUX_CSSL_SC_VALUE_TYPE_IMPL

/****************************************************************************/
/* Initialization                                                           */
/****************************************************************************/

/**
 * @def MCUX_CSSL_SC_ALLOC
 * @brief Allocation operation for the secure counter.
 * @api
 * @ingroup scCore
 */
#define MCUX_CSSL_SC_ALLOC() \
  MCUX_CSSL_SC_ALLOC_IMPL()

/**
 * @def MCUX_CSSL_SC_INIT
 * @brief Initialization operation for the secure counter.
 * @api
 * @ingroup scCore
 *
 * @param value Value with which the secure counter must be initialized.
 */
#define MCUX_CSSL_SC_INIT(value) \
  MCUX_CSSL_SC_INIT_IMPL(value)

/****************************************************************************/
/* Check                                                                    */
/****************************************************************************/

/**
 * @def MCUX_CSSL_SC_CHECK
 * @brief Comparison operation for the secure counter.
 * @api
 * @ingroup scCore
 *
 * @param reference Reference value to compare the secure counter value against.
 * @return          Either #MCUX_CSSL_SC_CHECK_PASSED, if the value matches, or
 *                  #MCUX_CSSL_SC_CHECK_FAILED if the value is different.
 */
#define MCUX_CSSL_SC_CHECK(reference) \
  MCUX_CSSL_SC_CHECK_IMPL(reference)

/****************************************************************************/
/* Counter increment                                                        */
/****************************************************************************/
/**
 * @defgroup scInc Secure counter increment
 * @brief Support for incrementing the secure counter.
 * @ingroup mcuxCsslSecureCounter
 */

/**
 * @def MCUX_CSSL_SC_ADD
 * @brief Increment the secure counter with @p value.
 * @api
 * @ingroup scInc
 *
 * @see MCUX_CSSL_SC_ADD_0x1
 * @see MCUX_CSSL_SC_ADD_0x10
 * @see MCUX_CSSL_SC_ADD_0x100
 * @see MCUX_CSSL_SC_SUB
 *
 * @param value Value with which the secure counter must be incremented.
 */
#define MCUX_CSSL_SC_ADD(value) \
  MCUX_CSSL_SC_ADD_IMPL(value)

/**
 * @def MCUX_CSSL_SC_ADD_ON_CALL
 * @brief Increment the secure counter with @p value in case of function call.
 * @api
 * @ingroup scInc
 *
 * @see MCUX_CSSL_SC_ADD
 *
 * @param value Value with which the secure counter must be incremented.
 */
#define MCUX_CSSL_SC_ADD_ON_CALL(value) \
  MCUX_CSSL_SC_ADD_ON_CALL_IMPL(value)

/**
 * @def MCUX_CSSL_SC_ADD_0x1
 * @brief Increment the secure counter with 0x1.
 * @api
 * @ingroup scInc
 *
 * @see MCUX_CSSL_SC_ADD
 * @see MCUX_CSSL_SC_ADD_0x10
 * @see MCUX_CSSL_SC_ADD_0x100
 * @see MCUX_CSSL_SC_SUB_0x1
 */
#define MCUX_CSSL_SC_ADD_0x1() \
  MCUX_CSSL_SC_ADD_0x1_IMPL()

/**
 * @def MCUX_CSSL_SC_ADD_0x10
 * @brief Increment the secure counter with 0x10.
 * @api
 * @ingroup scInc
 *
 * @see MCUX_CSSL_SC_ADD
 * @see MCUX_CSSL_SC_ADD_0x1
 * @see MCUX_CSSL_SC_ADD_0x100
 * @see MCUX_CSSL_SC_SUB_0x10
 */
#define MCUX_CSSL_SC_ADD_0x10() \
  MCUX_CSSL_SC_ADD_0x10_IMPL()

/**
 * @def MCUX_CSSL_SC_ADD_0x100
 * @brief Increment the secure counter with 0x100.
 * @api
 * @ingroup scInc
 *
 * @see MCUX_CSSL_SC_ADD
 * @see MCUX_CSSL_SC_ADD_0x1
 * @see MCUX_CSSL_SC_ADD_0x10
 * @see MCUX_CSSL_SC_SUB_0x100
 */
#define MCUX_CSSL_SC_ADD_0x100() \
  MCUX_CSSL_SC_ADD_0x100_IMPL()

/****************************************************************************/
/* Counter decrement                                                        */
/****************************************************************************/
/**
 * @defgroup scDec Secure counter decrement
 * @brief Support for decrementing the secure counter.
 * @ingroup mcuxCsslSecureCounter
 */

/**
 * @def MCUX_CSSL_SC_SUB
 * @brief Decrement the secure counter with @p value.
 * @api
 * @ingroup scDec
 *
 * @see MCUX_CSSL_SC_SUB_0x1
 * @see MCUX_CSSL_SC_SUB_0x10
 * @see MCUX_CSSL_SC_SUB_0x100
 * @see MCUX_CSSL_SC_ADD
 *
 * @param value Value with which the secure counter must be decremented.
 */
#define MCUX_CSSL_SC_SUB(value) \
  MCUX_CSSL_SC_SUB_IMPL(value)

/**
 * @def MCUX_CSSL_SC_SUB_0x1
 * @brief Decrement the secure counter with 0x1.
 * @api
 * @ingroup scDec
 *
 * @see MCUX_CSSL_SC_SUB
 * @see MCUX_CSSL_SC_SUB_0x10
 * @see MCUX_CSSL_SC_SUB_0x100
 * @see MCUX_CSSL_SC_ADD_0x1
 */
#define MCUX_CSSL_SC_SUB_0x1() \
  MCUX_CSSL_SC_SUB_0x1_IMPL()

/**
 * @def MCUX_CSSL_SC_SUB_0x10
 * @brief Decrement the secure counter with 0x10.
 * @api
 * @ingroup scDec
 *
 * @see MCUX_CSSL_SC_SUB
 * @see MCUX_CSSL_SC_SUB_0x1
 * @see MCUX_CSSL_SC_SUB_0x100
 * @see MCUX_CSSL_SC_ADD_0x10
 */
#define MCUX_CSSL_SC_SUB_0x10() \
  MCUX_CSSL_SC_SUB_0x10_IMPL()

/**
 * @def MCUX_CSSL_SC_SUB_0x100
 * @brief Decrement the secure counter with 0x100.
 * @api
 * @ingroup scDec
 *
 * @see MCUX_CSSL_SC_SUB
 * @see MCUX_CSSL_SC_SUB_0x1
 * @see MCUX_CSSL_SC_SUB_0x10
 * @see MCUX_CSSL_SC_ADD_0x100
 */
#define MCUX_CSSL_SC_SUB_0x100() \
  MCUX_CSSL_SC_SUB_0x100_IMPL()

/****************************************************************************/
/* Direct access (optional)                                                 */
/****************************************************************************/
/**
 * @defgroup scDirect Secure counter direct access
 * @brief Support for directly accessing the secure counter.
 * @ingroup mcuxCsslSecureCounter
 *
 * @warning Access to the secure counter is generally restricted, and generic
 * access might not be allowed.
 */

/**
 * @def MCUX_CSSL_SC_VALUE
 * @brief Access operation for the current secure counter value.
 * @api
 * @ingroup scDirect
 *
 * @warning Access to the secure counter is generally restricted, and generic
 * access might not be allowed. For portable code it is best to only rely on
 * the check operation to verify the secure counter value.
 *
 * @return The current value of the secure counter.
 */
#define MCUX_CSSL_SC_VALUE() \
  MCUX_CSSL_SC_VALUE_IMPL()

/**
 * @def MCUX_CSSL_SC_ASSIGN
 * @brief Assignment operation for the secure counter.
 * @api
 * @ingroup scDirect
 *
 * @warning Access to the secure counter is generally restricted, and generic
 * assignment might not be allowed. For portable code it is best to only rely
 * on the initialization, increment and decrement operations to change the
 * secure counter value.
 *
 * @param value Value that needs to be assigned to the secure counter.
 */
#define MCUX_CSSL_SC_ASSIGN(value) \
  MCUX_CSSL_SC_ASSIGN_IMPL(value)

#endif /* MCUXCSSLSECURECOUNTER_H_ */
