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
 * \file  mcuxCsslSecureCounter_SW_Local.h
 * \brief SW implementation of the CSSL secure counter mechanism (using a local
 *        variable).
 */

#ifndef MCUXCSSLSECURECOUNTER_SW_LOCAL_H_
#define MCUXCSSLSECURECOUNTER_SW_LOCAL_H_

/**
 * \addtogroup mcuxCsslIMPL MCUX CSSL -- Implementations
 *
 * \defgroup mcuxCsslSecureCounter_SwLocal Secure Counter: SW Local
 * \brief Secure counter mechanism implementation using a local variable.
 * \ingroup mcuxCsslIMPL
 */


/**
 * \defgroup scSwlCore Secure counter core functionality
 * \brief Secure counter handling core functionality.
 * \ingroup mcuxCsslSecureCounter_SwLocal
 *
 * \todo Extend this description of the core functionality.
 */

/**
 * \def MCUX_CSSL_SC_COUNTER_NAME
 * \brief Variable name to use for storing the secure counter value.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_COUNTER_NAME \
  mcuxCsslSecureCounter

/****************************************************************************/
/* Constants                                                                */
/****************************************************************************/

/**
 * \def MCUX_CSSL_SC_CHECK_PASSED_IMPL
 * \brief Positive comparison result value.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_CHECK_PASSED_IMPL (0xA5A5A5A5u)

/**
 * \def MCUX_CSSL_SC_CHECK_FAILED_IMPL
 * \brief Negative comparison result value.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_CHECK_FAILED_IMPL (~ MCUX_CSSL_SC_CHECK_PASSED_IMPL)

/**
 * \def MCUX_CSSL_SC_COUNTER_TYPE_IMPL
 * \brief Data type used for the secure counter.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_COUNTER_TYPE_IMPL \
  uint32_t

/**
 * \def MCUX_CSSL_SC_VALUE_TYPE_IMPL
 * \brief Data type used for the secure counter values.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_VALUE_TYPE_IMPL \
  static const uint32_t

/**
 * \def MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL
 * \brief Data type used for properly casting the secure counter balancing values.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL \
  uint32_t

/****************************************************************************/
/* Initialization                                                           */
/****************************************************************************/

/**
 * \def MCUX_CSSL_SC_ALLOC_IMPL
 * \brief Allocation operation implementation for the secure counter.
 * \ingroup scSwlCore
 */
#define MCUX_CSSL_SC_ALLOC_IMPL() \
  MCUX_CSSL_SC_COUNTER_TYPE_IMPL MCUX_CSSL_SC_COUNTER_NAME

/**
 * \def MCUX_CSSL_SC_INIT_IMPL
 * \brief Initialization operation implementation for the secure counter.
 * \ingroup scSwlCore
 *
 * \param value Value with which the secure counter must be initialized.
 */
#define MCUX_CSSL_SC_INIT_IMPL(value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  MCUX_CSSL_SC_ALLOC_IMPL() = ((MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL) (value)) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/****************************************************************************/
/* Check                                                                    */
/****************************************************************************/

/**
 * \def MCUX_CSSL_SC_CHECK_IMPL
 * \brief Comparison operation implementation for the secure counter.
 * \ingroup scSwlCore
 *
 * \param value     Reference value to compare the secure counter value against.
 * \return          Either #MCUX_CSSL_SC_CHECK_PASSED, if the value matches, or
 *                  #MCUX_CSSL_SC_CHECK_FAILED if the value is different.
 */
#define MCUX_CSSL_SC_CHECK_IMPL(value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  (MCUX_CSSL_SC_CHECK_FAILED_IMPL ^ (MCUX_CSSL_SC_COUNTER_NAME - (((MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL) (value)) + 1u))) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/****************************************************************************/
/* Counter increment                                                        */
/****************************************************************************/
/**
 * \defgroup scSwlInc Secure counter increment
 * \brief Support for incrementing the secure counter.
 * \ingroup mcuxCsslSecureCounter_SwLocal
 */

/**
 * \def MCUX_CSSL_SC_ADD_IMPL
 * \brief Increment the secure counter with \p value.
 * \ingroup scSwlInc
 *
 * \see MCUX_CSSL_SC_SUB_IMPL
 *
 * \param value Value with which the secure counter must be incremented.
 */
#define MCUX_CSSL_SC_ADD_IMPL(value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  MCUX_CSSL_SC_COUNTER_NAME += (MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL) (value) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/**
 * \def MCUX_CSSL_SC_ADD_ON_CALL_IMPL
 * \brief Increment the secure counter with \p value in case of function call.
 * \ingroup scSwlInc
 *
 * \see MCUX_CSSL_SC_ADD_IMPL
 *
 * \param value Value with which the secure counter must be incremented.
 */
#define MCUX_CSSL_SC_ADD_ON_CALL_IMPL(value) \
  MCUX_CSSL_SC_ADD_IMPL(value)

/**
 * \def MCUX_CSSL_SC_ADD_0X1_IMPL
 * \brief Increment the secure counter with 0x1.
 * \ingroup scSwlInc
 *
 * \see MCUX_CSSL_SC_ADD_IMPL
 */
#define MCUX_CSSL_SC_ADD_0X1_IMPL() \
  MCUX_CSSL_SC_ADD_IMPL(0x1u)

/**
 * \def MCUX_CSSL_SC_ADD_0X10_IMPL
 * \brief Increment the secure counter with 0x10.
 * \ingroup scSwlInc
 *
 * \see MCUX_CSSL_SC_ADD_IMPL
 */
#define MCUX_CSSL_SC_ADD_0X10_IMPL() \
  MCUX_CSSL_SC_ADD_IMPL(0x10u)

/**
 * \def MCUX_CSSL_SC_ADD_0X100_IMPL
 * \brief Increment the secure counter with 0x100.
 * \ingroup scSwlInc
 *
 * \see MCUX_CSSL_SC_ADD_IMPL
 */
#define MCUX_CSSL_SC_ADD_0X100_IMPL() \
  MCUX_CSSL_SC_ADD_IMPL(0x100u)

/****************************************************************************/
/* Counter decrement                                                        */
/****************************************************************************/
/**
 * \defgroup scSwlDec Secure counter decrement
 * \brief Support for decrementing the secure counter.
 * \ingroup mcuxCsslSecureCounter_SwLocal
 */

/**
 * \def MCUX_CSSL_SC_SUB_IMPL
 * \brief Decrement the secure counter with \p value.
 * \ingroup scSwlDec
 *
 * \see MCUX_CSSL_SC_ADD_IMPL
 *
 * \param value Value with which the secure counter must be decremented.
 */
#define MCUX_CSSL_SC_SUB_IMPL(value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  MCUX_CSSL_SC_COUNTER_NAME -= (MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL) (value) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/**
 * \def MCUX_CSSL_SC_SUB_0X1_IMPL
 * \brief Decrement the secure counter with 0x1.
 * \ingroup scSwlDec
 *
 * \see MCUX_CSSL_SC_SUB_IMPL
 */
#define MCUX_CSSL_SC_SUB_0X1_IMPL() \
  MCUX_CSSL_SC_SUB_IMPL(0x1u)

/**
 * \def MCUX_CSSL_SC_SUB_0X10_IMPL
 * \brief Decrement the secure counter with 0x10.
 * \ingroup scSwlDec
 *
 * \see MCUX_CSSL_SC_SUB_IMPL
 */
#define MCUX_CSSL_SC_SUB_0X10_IMPL() \
  MCUX_CSSL_SC_SUB_IMPL(0x10u)

/**
 * \def MCUX_CSSL_SC_SUB_0X100_IMPL
 * \brief Decrement the secure counter with 0x100.
 * \ingroup scSwlDec
 *
 * \see MCUX_CSSL_SC_SUB_IMPL
 */
#define MCUX_CSSL_SC_SUB_0X100_IMPL() \
  MCUX_CSSL_SC_SUB_IMPL(0x100u)

/****************************************************************************/
/* Direct access (optional)                                                 */
/****************************************************************************/
/**
 * \defgroup scSwlDirect Secure counter direct access
 * \brief Support for directly accessing the secure counter.
 * \ingroup mcuxCsslSecureCounter_SwLocal
 *
 * \warning Access to the secure counter is generally restricted, and generic
 * access might not be allowed.
 */

/**
 * \def MCUX_CSSL_SC_VALUE_IMPL
 * \brief Access operation for the current secure counter value.
 * \ingroup scSwlDirect
 *
 * \warning Access to the secure counter is generally restricted, and generic
 * access might not be allowed. For portable code it is best to only rely on
 * the check operation to verify the secure counter value.
 *
 * \return The current value of the secure counter.
 */
#define MCUX_CSSL_SC_VALUE_IMPL() \
  MCUX_CSSL_SC_COUNTER_NAME

/**
 * \def MCUX_CSSL_SC_ASSIGN_IMPL
 * \brief Assignment operation for the secure counter.
 * \ingroup scSwlDirect
 *
 * \warning Access to the secure counter is generally restricted, and generic
 * assignment might not be allowed. For portable code it is best to only rely
 * on the initialization, increment and decrement operations to change the
 * secure counter value.
 *
 * \param value Value that needs to be assigned to the secure counter.
 */
#define MCUX_CSSL_SC_ASSIGN_IMPL(value) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  MCUX_CSSL_SC_COUNTER_NAME = (MCUX_CSSL_SC_BALANCING_VALUE_TYPE_IMPL) (value) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

#endif /* MCUXCSSLSECURECOUNTER_SW_LOCAL_H_ */
