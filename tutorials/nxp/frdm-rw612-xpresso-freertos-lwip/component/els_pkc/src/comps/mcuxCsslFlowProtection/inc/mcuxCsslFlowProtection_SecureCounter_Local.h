/*--------------------------------------------------------------------------*/
/* Copyright 2020-2024 NXP                                                  */
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
 * \file  mcuxCsslFlowProtection_SecureCounter_Local.h
 * \brief Counter based implementation for the flow protection mechanism, for a local security counter.
 */

#ifndef MCUX_CSSL_FLOW_PROTECTION_SECURE_COUNTER_LOCAL_H_
#define MCUX_CSSL_FLOW_PROTECTION_SECURE_COUNTER_LOCAL_H_

/* Include the CSSL C pre-processor support functionality. */
#include <mcuxCsslCPreProcessor.h>

/* Include the CSSL secure counter mechanism as basic building block. */
#include <mcuxCsslSecureCounter.h>

/* Include the C99 standard integer types. */
#include <stdint.h>

/* Include standard boolean types */
#include <stdbool.h>


/**
 * \def MCUX_CSSL_FP_PROTECTED_TYPE_IMPL
 * \brief Based on a given base type, builds a return type with flow
 *        protection.
 * \ingroup csslFpCntFunction
 *
 * \see MCUX_CSSL_FP_FUNCTION_DEF_IMPL
 *
 * \param resultType The type to be converted into a protected type.
 */
#define MCUX_CSSL_FP_PROTECTED_TYPE_IMPL(resultType) \
  uint64_t

/**
 * \def MCUX_CSSL_FP_FUNCTION_ENTRY_IMPLn
 * \brief Flow protection handler implementation for the function entry point.
 * \ingroup csslFpCntFunction
 *
 * Initialize the counter with the entry part of the function identifier, and
 * include expectations in the initialization value.
 *
 * \see MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL
 *
 * \param id     Identifier of the function that has just been entered.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 */
#define MCUX_CSSL_FP_FUNCTION_ENTRY_IMPLn(function, ...) \
  MCUX_CSSL_SC_INIT( \
    MCUX_CSSL_FP_FUNCTION_ID_ENTRY_PART(function) \
    - (MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__)) \
  )

/**
 * \def MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL1
 * \brief Flow protection handler implementation for the function entry point.
 * \ingroup csslFpCntFunction
 *
 * Initialize the counter with the entry part of the function identifier,
 * without any potential expectations.
 *
 * \see MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL
 *
 * \param id Identifier of the function that has just been entered.
 */
#define MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL1(function) \
  MCUX_CSSL_FP_FUNCTION_ENTRY_IMPLn(function, 0u)

/**
 * \def MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL
 * \brief Flow protection handler implementation for the function entry point.
 * \ingroup csslFpCntFunction
 *
 * Initialize the counter with entry part of the function identifier, and
 * include potential expectations in the initialization value.
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL1
 * \see MCUX_CSSL_FP_FUNCTION_ENTRY_IMPLn
 *
 * \param id     Identifier of the function that has just been entered.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior.
 */
#define MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED1(MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL, __VA_ARGS__)


/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_IMPLn
 * \brief Flow protection handler implementation for the function exit point.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier, and
 * include potential expectations in the adjustment value. Return the counter
 * value together with the \p result via the function return value.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_IMPL
 *
 * \param id     Identifier of the function from which we will exit.
 * \param result Result that should be encoded in the return value.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       A value in which both \p result and a flow protection token
 *               are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_IMPLn(id, result, ...) \
  MCUX_CSSL_SC_ADD( \
    MCUX_CSSL_FP_FUNCTION_ID_EXIT_PART(id) \
    - MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) \
  ); \
  return (MCUX_CSSL_FP_RESULT_VALUE(result) \
    | MCUX_CSSL_FP_PROTECTION_TOKEN_VALUE(MCUX_CSSL_FP_COUNTER_COMPRESSED()))

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_IMPL1
 * \brief Flow protection handler implementation for the function exit point.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier, without
 * any potential expectations in the adjustment value. Return the counter value
 * via the function return value.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_IMPL
 *
 * \param id Identifier of the function from which we will exit.
 * \return   A value in which a flow protection token is encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_IMPL1(id) \
  MCUX_CSSL_FP_FUNCTION_EXIT_IMPLn(id, 0u, 0u)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_IMPL2
 * \brief Flow protection handler implementation for the function exit point.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier, without
 * any potential expectations in the adjustment value. Return the counter value
 * together with the \p result via the function return value.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_IMPL
 *
 * \param id     Identifier of the function from which we will exit.
 * \param result Result that should be encoded in the return value.
 * \return       A value in which both \p result and a flow protection token
 *               are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_IMPL2(id, result) \
  MCUX_CSSL_FP_FUNCTION_EXIT_IMPLn(id, result, 0u)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_IMPL
 * \brief Flow protection handler implementation for the function exit point.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier, and
 * include potential expectations in the adjustment value. Return the counter
 * value together with the \p result via the function return value.
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_IMPL1
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_IMPL2
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_IMPLn
 *
 * \param id     Identifier of the function from which we will exit.
 * \param result Result that should be encoded in the return value.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       A value in which both \p result and a flow protection token
 *               are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED2(MCUX_CSSL_FP_FUNCTION_EXIT_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPLn
 * \brief Flow protection handler implementation for the function exit point
 *        which includes an actual check of the code flow.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier, and
 * include potential expectations in the adjustment value. Check whether the
 * counter matches the expected value, and choose the result from \p pass and
 * \p fail and return it together with the counter value via the function
 * return value.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL3
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPLn
 *
 * \param id     Identifier of the function from which we will exit.
 * \param pass   Result that should be encoded in the return value if the flow
 *               protection check passed.
 * \param fail   Result that should be encoded in the return value if the flow
 *               protection check failed.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       A value in which both the result (either \p pass or \p fail)
 *               and a flow protection token are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPLn(id, pass, fail, ...) \
  MCUX_CSSL_SC_ADD( \
    MCUX_CSSL_FP_FUNCTION_ID_EXIT_PART(id) \
    - MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) \
  ); \
  return (MCUX_CSSL_FP_RESULT_VALUE( \
    (MCUX_CSSL_SC_CHECK_PASSED == \
        MCUX_CSSL_SC_CHECK(MCUX_CSSL_FP_FUNCTION_VALUE(id))) \
    ? pass : fail) \
    | MCUX_CSSL_FP_PROTECTION_TOKEN_VALUE(MCUX_CSSL_FP_COUNTER_COMPRESSED()))

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL3
 * \brief Flow protection handler implementation for the function exit point
 *        which includes an actual check of the code flow.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier. Check
 * whether the counter matches the expected value, and choose the result from
 * \p pass and \p fail and return it together with the counter value via the
 * function return value.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL
 *
 * \param id     Identifier of the function from which we will exit.
 * \param pass   Result that should be encoded in the return value if the flow
 *               protection check passed.
 * \param fail   Result that should be encoded in the return value if the flow
 *               protection check failed.
 * \return       A value in which both the result (either \p pass or \p fail)
 *               and a flow protection token are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL3(id, pass, fail) \
  MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPLn(id, pass, fail, 0u)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL
 * \brief Flow protection handler implementation for the function exit point
 *        which includes an actual check of the code flow.
 * \ingroup csslFpCntFunction
 *
 * Adjust the counter with the exit part of the function identifier, and
 * include potential expectations in the adjustment value. Check whether the
 * counter matches the expected value, and choose the result from \p pass and
 * \p fail and return it together with the counter value via the function
 * return value.
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL3
 * \see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPLn
 *
 * \param id     Identifier of the function from which we will exit.
 * \param pass   Result that should be encoded in the return value if the flow
 *               protection check passed.
 * \param fail   Result that should be encoded in the return value if the flow
 *               protection check failed.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       A value in which both the result (either \p pass or \p fail)
 *               and a flow protection token are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED3(MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL1
 * \brief Flow protection handler for the exit point of functions with the
 *        return type \c void.
 * \ingroup csslFpCntFunction
 *
 * \param id     Identifier of the function from which we will exit.
 * \return       A protected return value of type void.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL1(id) \
  MCUX_CSSL_FP_FUNCTION_EXIT_IMPL(id, 0U)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL2
 * \brief Flow protection handler for the exit point of functions with the
 *        return type \c void.
 * \ingroup csslFpCntFunction
 *
 * \param id     Identifier of the function from which we will exit.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       A protected return value of type void.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPLn(id, ...) \
  MCUX_CSSL_FP_FUNCTION_EXIT_IMPL(id, 0U, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL
 * \brief Flow protection handler for the exit point of functions with the
 *        return type \c void.
 * \ingroup csslFpCntFunction
 *
 * \param id     Identifier of the function from which we will exit.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       A protected return value of type void.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED1(MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_IMPL3
 * \brief Event implementation of a flow protected function call.
 * \ingroup csslFpCntFunction
 *
 * \param type   Type of the \p result variable.
 * \param result Fresh variable name to store the result of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_IMPL3(type, result, call) \
  const uint64_t MCUX_CSSL_CPP_CAT(result, _protected) = (call); \
  MCUX_CSSL_SC_ADD_ON_CALL( \
    MCUX_CSSL_FP_PROTECTION_TOKEN(MCUX_CSSL_CPP_CAT(result, _protected))); \
  type const result = MCUX_CSSL_FP_RESULT(type, \
    MCUX_CSSL_CPP_CAT(result, _protected))

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_IMPL2
 * \brief Event implementation of a flow protected function call.
 * \ingroup csslFpCntFunction
 *
 * \param result Fresh variable name to store the result of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_IMPL2(result, call) \
  MCUX_CSSL_FP_FUNCTION_CALL_IMPL3(uint32_t, result, call)

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_IMPL
 * \brief Event implementation of a flow protected function call.
 * \ingroup csslFpCntFunction
 *
 * \declaration{MCUX_CSSL_FP_FUNCTION_DECL_IMPL}
 * \expectation{MCUX_CSSL_FP_FUNCTION_CALLED_IMPL}
 *
 * \param type   Optional, type of the \p result variable.
 * \param result Fresh variable name to store the result of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED3(MCUX_CSSL_FP_FUNCTION_CALL_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_VOID_IMPL
 * \brief Event implementation of a flow protected void function call.
 * \ingroup csslFpCntFunction
 *
 * \declaration{MCUX_CSSL_FP_FUNCTION_DECL_IMPL}
 * \expectation{MCUX_CSSL_FP_FUNCTION_CALLED_IMPL}
 *
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_IMPL(call) \
  { \
  const uint64_t MCUX_CSSL_CPP_CAT(result, _protected) = (call); \
  MCUX_CSSL_SC_ADD_ON_CALL( \
    MCUX_CSSL_FP_PROTECTION_TOKEN(MCUX_CSSL_CPP_CAT(result, _protected))); \
  }

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED_IMPL
 * \brief Implementation of a flow protected function call meant to be used
 *        from within an unprotected function
 * \ingroup csslFpCntFunction
 *
 *
 * \param result Fresh variable name to store the result of \p call.
 * \param token  Fresh variable name to store the protection token of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED_IMPL(result, token, call) \
  const uint64_t MCUX_CSSL_CPP_CAT(result, _protected) = (call); \
  const uint32_t token = MCUX_CSSL_FP_PROTECTION_TOKEN( \
    MCUX_CSSL_CPP_CAT(result, _protected)); \
  const uint32_t result = MCUX_CSSL_FP_RESULT( \
    MCUX_CSSL_CPP_CAT(result, _protected))

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_VOID_PROTECTED_IMPL
 * \brief Implementation of a flow protected void function call meant to be
 *        used from within an unprotected function
 * \ingroup csslFpCntFunction
 *
 *
 * \param token  Fresh variable name to store the protection token of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_PROTECTED_IMPL(token, call) \
  const uint64_t MCUX_CSSL_CPP_CAT(token, _protected) = (call); \
  const uint32_t token = MCUX_CSSL_FP_PROTECTION_TOKEN( \
    MCUX_CSSL_CPP_CAT(token, _protected));

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_BEGIN_IMPL
 * \brief Implementation of a flow protected function call meant to be used
 *        from within an unprotected function, that must be terminated by
 *        #MCUX_CSSL_FP_FUNCTION_CALL_END_IMPL.
 * \ingroup csslFpCntFunction
 *
 *
 * \param result Fresh variable name to store the result of \p call.
 * \param token  Fresh variable name to store the protection token of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_BEGIN_IMPL(result, token, call)   \
do                                                                  \
{                                                                   \
    MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED_IMPL(result, token, call)

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_END_IMPL
 * \brief Implementation of the end of a section started by
 * #MCUX_CSSL_FP_FUNCTION_CALL_BEGIN_IMPL.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_END_IMPL() \
MCUX_CSSL_ANALYSIS_START_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION() \
} while (false) \
MCUX_CSSL_ANALYSIS_STOP_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION()

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN_IMPL
 * \brief Implementation of a flow protected void function call meant to be used
 *        from within an unprotected function, that must be terminated by
 *        #MCUX_CSSL_FP_FUNCTION_CALL_VOID_END_IMPL.
 * \ingroup csslFpCntFunction
 *
 *
 * \param token  Fresh variable name to store the protection token of \p call.
 * \param call   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN_IMPL(token, call)   \
do                                                                  \
{                                                                   \
    MCUX_CSSL_FP_FUNCTION_CALL_VOID_PROTECTED_IMPL(token, call)

/**
 * \def MCUX_CSSL_FP_FUNCTION_CALL_VOID_END_IMPL
 * \brief Implementation of the end of a section started by
 * #MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN_IMPL.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_END_IMPL() \
MCUX_CSSL_ANALYSIS_START_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION() \
} while (false) \
MCUX_CSSL_ANALYSIS_STOP_PATTERN_BOOLEAN_TYPE_FOR_CONDITIONAL_EXPRESSION()

/**
 * @def MCUX_CSSL_FP_ASSERT_IMPL
 * @brief Assert an expected state of the code flow.
 * @api
 * @ingroup csslFpCntExpect
 *
 * This macro can be used to check whether the code flow up to this point
 * matches the expected state. Unlike the #MCUX_CSSL_FP_EXPECT macro, it will
 * not update the expectations, but merely perform a check on the recorded
 * events against the already recorded expectations plus the ones provided as
 * parameters.
 *
 * If the check fails, the code defined in MCUX_CSSL_FP_ASSERT_CALLBACK will be
 * executed.
 *
 * \see MCUX_CSSL_FP_EXPECTATIONS
 *
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 */
#define MCUX_CSSL_FP_ASSERT_IMPL(...) \
  if (MCUX_CSSL_SC_CHECK_PASSED != \
        MCUX_CSSL_SC_CHECK(MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__))) \
  { \
    MCUX_CSSL_FP_ASSERT_CALLBACK(); \
  } \
  else if (MCUX_CSSL_SC_CHECK_PASSED != \
        MCUX_CSSL_SC_CHECK(MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__))) \
  { \
    MCUX_CSSL_FP_ASSERT_CALLBACK(); \
  } \
  else {/*empty*/}

#endif /* MCUX_CSSL_FLOW_PROTECTION_SECURE_COUNTER_LOCAL_H_ */
