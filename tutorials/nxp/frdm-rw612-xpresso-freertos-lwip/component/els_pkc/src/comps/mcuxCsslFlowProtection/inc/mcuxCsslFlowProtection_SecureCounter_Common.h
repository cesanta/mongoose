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
 * \file  mcuxCsslFlowProtection_SecureCounter_Common.h
 * \brief Counter based implementation for the flow protection mechanism, for a local security counter.
 */

#ifndef MCUX_CSSL_FLOW_PROTECTION_SECURE_COUNTER_COMMON_H_
#define MCUX_CSSL_FLOW_PROTECTION_SECURE_COUNTER_COMMON_H_

/* Include the CSSL C pre-processor support functionality. */
#include <mcuxCsslCPreProcessor.h>
#include <mcuxCsslAnalysis.h>

/* Include the CSSL secure counter mechanism as basic building block. */
#include <mcuxCsslSecureCounter.h>

/* Include the C99 standard integer types. */
#include <stdint.h>

/**
 * \addtogroup mcuxCsslIMPL MCUX CSSL -- Implementations
 *
 * \defgroup mcuxCsslFlowProtection_SecureCounter Flow Protection: Secure Counter
 * \brief Secure counter based implementation for the flow protection mechanism.
 * \ingroup mcuxCsslIMPL
 */


/**
 * \defgroup csslFpCntCore Flow protection core functionality
 * \brief Flow protection handling core functionality.
 * \ingroup mcuxCsslFlowProtection_SecureCounter
 *
 * \todo Extend this description of the core functionality which relies
 *       basically on the function calling flow protection.
 *
 * \declaration{MCUX_CSSL_FP_FUNCTION_DECL_IMPL}
 * \event{MCUX_CSSL_FP_FUNCTION_CALL_IMPL}
 * \expectation{MCUX_CSSL_FP_FUNCTION_CALLED_IMPL}
 */

/**
 * \def MCUX_CSSL_FP_DECL_NAME
 * \brief Construct a name based on type and id.
 * \ingroup csslFpCntCore
 *
 * \param type Indicator for the type of declaration.
 * \param id   Identifier for the flow protected entity.
 * \return     CSSL flow protection entity name for given \p type and \p id.
 */
#define MCUX_CSSL_FP_DECL_NAME(type, id) \
  MCUX_CSSL_CPP_CAT4(mcuxCsslFlowProtection_, type, _, id)

/**
 * \def MCUX_CSSL_FP_DECL_IMPL
 * \brief Generic flow protected entity declaration implementation.
 * \ingroup csslFpCntCore
 *
 * \param type Indicator for the type of declaration.
 * \param id   Identifier for the flow protected entity.
 * \return     CSSL flow protection entity declaration.
 */
#define MCUX_CSSL_FP_DECL_IMPL(type, id) \
  MCUX_CSSL_SC_VALUE_TYPE MCUX_CSSL_FP_DECL_NAME(type, id) = \
    MCUX_CSSL_CPP_CAT3(MCUX_CSSL_FP_, type, _ID)(id)

/**
 * \def MCUX_CSSL_FP_ID_IMPL
 * \brief Generic identifier generator based on current line number.
 * \ingroup csslFpCntCore
 *
 * \return Counter value based on the current line number.
 */
#define MCUX_CSSL_FP_ID_IMPL() \
  MCUX_CSSL_CPP_CAT(__LINE__, u)



/**
 * \defgroup csslFpCntExpect Expectation handling
 * \brief Expectation handling support functionality.
 * \ingroup mcuxCsslFlowProtection_SecureCounter
 */

/**
 * \def MCUX_CSSL_FP_EXPECTATIONS
 * \brief Expectation aggregation.
 * \ingroup csslFpCntExpect
 *
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 * \return       Aggregated counter value for the given expectations.
 */
#define MCUX_CSSL_FP_EXPECTATIONS(...) \
  ((uint32_t) 0u + (MCUX_CSSL_CPP_MAP(MCUX_CSSL_CPP_ADD, __VA_ARGS__)))

/**
 * @def MCUX_CSSL_FP_COUNTER_STMT
 * @brief A statement which is only evaluated if a secure counter is used.
 * @api
 * @ingroup csslFpCntCore
 *
 * This macro can be used to create counting variables that are only present if
 * the active configuration uses a secure counter, to avoid warnings about
 * unused variables.
 *
 * @param statement The statement to be conditionally included.
 */
#define MCUX_CSSL_FP_COUNTER_STMT_IMPL(statement) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  statement \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/**
 * \def MCUX_CSSL_FP_CONDITIONAL_IMPL
 * \brief Conditional expectation aggregation.
 * \ingroup csslFpCntCore
 *
 * \param condition Condition under which the given expectations apply
 * \param expect    One or more (comma separated) declarations of expected code
 *                  code flow behavior.
 * \return          Aggregated counter value for the given expectations, if
 *                  condition is satisfied. Otherwise 0.
 */
#define MCUX_CSSL_FP_CONDITIONAL_IMPL(condition, ...) \
  (MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) & ((condition) ? ((uint32_t) UINT32_MAX) : ((uint32_t) 0)))

/**
 * @def MCUX_CSSL_FP_VOID_EXPECTATION_IMPL
 * @brief Implementation of expectation of nothing
 * @api
 * @ingroup csslFpCntExpect
 *
 * This expectation macro indicates to the flow protection mechanism that nothing
 * is expected to happen. This is mainly intended for internal use (to ensure at
 * least one expectation is passed).
 */
#define MCUX_CSSL_FP_VOID_EXPECTATION_IMPL() \
  (0u)

/**
 * \def MCUX_CSSL_FP_EXPECT_IMPL
 * \brief Declaration(s) of expected code flow behavior.
 * \ingroup csslFpCntExpect
 *
 * This macro can be used to indicate expectations in the function body at
 * another location than the function entry or exit.
 *
 * \see MCUX_CSSL_FP_EXPECTATIONS
 *
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior.
 */
#define MCUX_CSSL_FP_EXPECT_IMPL(...) \
  MCUX_CSSL_SC_SUB( \
    MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) \
  )



/**
 * \defgroup csslFpCntFunction Function calling flow protection
 * \brief Support for flow protected functions.
 * \ingroup mcuxCsslFlowProtection_SecureCounter
 *
 * \declaration{MCUX_CSSL_FP_FUNCTION_DECL_IMPL}
 * \event{MCUX_CSSL_FP_FUNCTION_CALL_IMPL}
 * \expectation{MCUX_CSSL_FP_FUNCTION_CALLED_IMPL}
 */

/**
 * \def MCUX_CSSL_FP_FUNCTION_ID
 * \brief Generator for function identifiers.
 * \ingroup csslFpCntFunction
 *
 * \param id Identifier for the flow protected function.
 */
#define MCUX_CSSL_FP_FUNCTION_ID(id) \
  MCUX_CSSL_CPP_CAT(MCUX_CSSL_FP_FUNCID_, id)

/**
 * \def MCUX_CSSL_FP_FUNCTION_ID_ENTRY_MASK
 * \brief Mask to be used to derive entry part from a function identifier
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_FUNCTION_ID_ENTRY_MASK \
  (0x5A5A5A5Au)

/**
 * \def MCUX_CSSL_FP_FUNCTION_ID_ENTRY_PART
 * \brief Part of the function identifier to be used at function entry.
 * \ingroup csslFpCntFunction
 *
 * \param id Identifier for the flow protected function.
 */
#define MCUX_CSSL_FP_FUNCTION_ID_ENTRY_PART(id) \
  (MCUX_CSSL_FP_FUNCTION_VALUE(id) & MCUX_CSSL_FP_FUNCTION_ID_ENTRY_MASK)

/**
 * \def MCUX_CSSL_FP_FUNCTION_ID_EXIT_PART
 * \brief Part of the function identifier to be used at function exit.
 * \ingroup csslFpCntFunction
 *
 * \param id Identifier for the flow protected function.
 */
#define MCUX_CSSL_FP_FUNCTION_ID_EXIT_PART(id) \
  (MCUX_CSSL_FP_FUNCTION_VALUE(id) - MCUX_CSSL_FP_FUNCTION_ID_ENTRY_PART(id))

/**
 * \def MCUX_CSSL_FP_FUNCTION_DECL_IMPL
 * \brief Declaration implementation of a flow protected function.
 * \ingroup csslFpCntFunction
 *
 * \event{MCUX_CSSL_FP_FUNCTION_CALL_IMPL}
 * \expectation{MCUX_CSSL_FP_FUNCTION_CALLED_IMPL}
 *
 * @param id Identifier for the function that is flow protected.
 * @param ptrType Optional, pointer type matching this function.
 */
#define MCUX_CSSL_FP_FUNCTION_DECL_IMPL(...) \
  /* Intentionally empty */

/**
 * \def MCUX_CSSL_FP_FUNCTION_VALUE
 * \brief Macro to get the value for a given function.
 * \ingroup csslFpCntFunction
 *
 * \param id Identifier for the function that is flow protected.
 * \return   The counter value for the given function \p id.
 */
#define MCUX_CSSL_FP_FUNCTION_VALUE(id) \
  ((uint32_t) MCUX_CSSL_FP_FUNCTION_ID(id))

/**
 * \def MCUX_CSSL_FP_FUNCTION_DEF_IMPL
 * \brief Definition implementation of a flow protected function.
 * \ingroup csslFpCntFunction
 *
 * Not used in the current implementation.
 *
 * @param id Identifier for the function that is flow protected.
 * @param ptrType Optional, pointer type matching this function.
 */
#define MCUX_CSSL_FP_FUNCTION_DEF_IMPL(...) \
  /* Intentionally empty. */

/**
 * \def MCUX_CSSL_FP_FUNCTION_POINTER_IMPL
 * \brief Definition implementation of a flow protected function pointer.
 * \ingroup csslFpNoneFunction
 *
 * @param type Identifier for the function pointer type that is flow protected.
 * @param definition Actual type definition of the function pointer type.
 */
#define MCUX_CSSL_FP_FUNCTION_POINTER_IMPL(type, definition) \
  definition

/**
 * \def MCUX_CSSL_FP_RESULT_OFFSET
 * \brief Offset of the result in the return value.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_RESULT_OFFSET \
  (0u)

/**
 * \def MCUX_CSSL_FP_RESULT_MASK
 * \brief Bitmask of the result in the return value.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_RESULT_MASK \
  (0xFFFFFFFFuLL)

/**
 * \def MCUX_CSSL_FP_RESULT_VALUE
 * \brief Encode a result value for a protected return value.
 * \ingroup csslFpCntFunction
 *
 * \param result The result that needs to be encoded.
 */
#define MCUX_CSSL_FP_RESULT_VALUE(result) \
  (((uint64_t)(result) & MCUX_CSSL_FP_RESULT_MASK) << MCUX_CSSL_FP_RESULT_OFFSET)

/**
 * \def MCUX_CSSL_FP_RESULT_IMPL2
 * \brief Extract the result value from a protected \p return value.
 * \ingroup csslFpCntFunction
 *
 * \param type   Type of the result.
 * \param return The protected return value which contains the result.
 */
#define MCUX_CSSL_FP_RESULT_IMPL2(type, return) \
  ((type)(((return) >> MCUX_CSSL_FP_RESULT_OFFSET) & MCUX_CSSL_FP_RESULT_MASK))

/**
 * \def MCUX_CSSL_FP_RESULT_IMPL1
 * \brief Extract the result value from a protected \p return value.
 * \ingroup csslFpCntFunction
 *
 * \param return The protected return value which contains the result.
 */
#define MCUX_CSSL_FP_RESULT_IMPL1(return) \
  MCUX_CSSL_FP_RESULT_IMPL2(uint32_t,return)

/**
 * \def MCUX_CSSL_FP_RESULT_IMPL
 * \brief Extract the result value from a protected \p return value.
 * \ingroup csslFpCntFunction
 *
 * \param type   Optional, type of the result.
 * \param return The protected return value which contains the result.
 */
#define MCUX_CSSL_FP_RESULT_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED2(MCUX_CSSL_FP_RESULT_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_PROTECTION_OFFSET
 * \brief Offset of the protection token in the return value.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_PROTECTION_OFFSET \
  (32u)

/**
 * \def MCUX_CSSL_FP_PROTECTION_MASK
 * \brief Bitmask of the protection token in the return value.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_PROTECTION_MASK \
  ((uint64_t) 0xFFFFFFFFuLL)

/**
 * \def MCUX_CSSL_FP_PROTECTION_TOKEN_VALUE
 * \brief Encode a protection token for a protected return value.
 * \ingroup csslFpCntFunction
 *
 * Note that this macro is only used with a local security counter,
 * e.g. for configuration CSSL_SC_USE_SW_LOCAL
 *
 * \param token The protection token that needs to be encoded.
 */
#define MCUX_CSSL_FP_PROTECTION_TOKEN_VALUE(token) \
  ((((uint64_t)(token) & MCUX_CSSL_FP_PROTECTION_MASK)) << MCUX_CSSL_FP_PROTECTION_OFFSET)

/**
 * \def MCUX_CSSL_FP_PROTECTION_TOKEN_IMPL
 * \brief Extract the protection token value from a protected \p return value.
 * \ingroup csslFpCntFunction
 *
 * Note that this macro is only used with a local security counter,
 * e.g. for configuration CSSL_SC_USE_SW_LOCAL
 *
 * \param return The protected return value which contains the protection token.
 */
#define MCUX_CSSL_FP_PROTECTION_TOKEN_IMPL(return) \
  (uint32_t)(((return) >> MCUX_CSSL_FP_PROTECTION_OFFSET) & MCUX_CSSL_FP_PROTECTION_MASK)

/**
 * \def MCUX_CSSL_FP_COUNTER_COMPRESSED
 * \brief Compressed version of the secure counter that can be used as a
 *        protection token.
 * \ingroup csslFpCntFunction
 */
#define MCUX_CSSL_FP_COUNTER_COMPRESSED() \
  MCUX_CSSL_SC_VALUE()


/**
 * \def MCUX_CSSL_FP_FUNCTION_CALLED_IMPL
 * \brief Expectation implementation of a called function.
 * \ingroup csslFpCntFunction
 *
 * \declaration{MCUX_CSSL_FP_FUNCTION_DECL_IMPL}
 * \event{MCUX_CSSL_FP_FUNCTION_CALL_IMPL}
 *
 * \see MCUX_CSSL_FP_FUNCTION_VALUE
 *
 * \param id Identifier of the function that is expected to be called.
 * \return   Counter value for the given function.
 */
#define MCUX_CSSL_FP_FUNCTION_CALLED_IMPL(id) \
  MCUX_CSSL_FP_FUNCTION_VALUE(id)


/**
 * \def MCUX_CSSL_FP_FUNCTION_ENTERED_IMPL
 * \brief Expectation implementation of an entered (but not exited) function.
 * \ingroup csslFpCntFunction
 *
 * \declaration{MCUX_CSSL_FP_FUNCTION_DECL_IMPL}
 * \event{MCUX_CSSL_FP_FUNCTION_CALL_IMPL}
 *
 * \see MCUX_CSSL_FP_FUNCTION_VALUE
 *
 * \param id Identifier of the function that is expected to be entered.
 * \return   Counter value for the given function.
 */
#define MCUX_CSSL_FP_FUNCTION_ENTERED_IMPL(id) \
  MCUX_CSSL_FP_FUNCTION_ID_ENTRY_PART(id)


/**
 * \defgroup csslFpCntLoop Looping flow protection
 * \brief Support for flow protected loops.
 * \ingroup mcuxCsslFlowProtection_SecureCounter
 *
 * \declaration{MCUX_CSSL_FP_LOOP_DECL_IMPL}
 * \event{MCUX_CSSL_FP_LOOP_ITERATION_IMPL}
 * \expectation{MCUX_CSSL_FP_LOOP_ITERATIONS_IMPL}
 */

/**
 * \def MCUX_CSSL_FP_LOOP_ID
 * \brief Generator for loop identifiers.
 * \ingroup csslFpCntLoop
 *
 * \param id Identifier for the flow protected loop.
 * \return   Counter value for the given loop.
 */
#define MCUX_CSSL_FP_LOOP_ID(id) \
  MCUX_CSSL_FP_ID_IMPL()

/**
 * \def MCUX_CSSL_FP_LOOP_DECL_IMPL
 * \brief Declaration implementation of a flow protected loop.
 * \ingroup csslFpCntLoop
 *
 * \param id Identifier for the loop that is flow protected.
 */
#define MCUX_CSSL_FP_LOOP_DECL_IMPL(id) \
  MCUX_CSSL_FP_DECL_IMPL(LOOP, id)

/**
 * \def MCUX_CSSL_FP_LOOP_VALUE
 * \brief Macro to get the value for a given loop.
 * \ingroup csslFpCntLoop
 *
 * \param id Identifier for the loop that is flow protected.
 * \return   The counter value for the given loop \p id.
 */
#define MCUX_CSSL_FP_LOOP_VALUE(id) \
  MCUX_CSSL_FP_DECL_NAME(LOOP, id)

/**
 * \def MCUX_CSSL_FP_LOOP_ITERATION_IMPLn
 * \brief Event implementation of a loop iteration (with expectations).
 * \ingroup csslFpCntLoop
 *
 * \see MCUX_CSSL_FP_LOOP_ITERATION_IMPL
 *
 * \param id     Identifier for the loop that is flow protected.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_LOOP_ITERATION_IMPLn(id, ...) \
  MCUX_CSSL_SC_ADD( \
    MCUX_CSSL_FP_LOOP_VALUE(id) \
    - MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) \
  )

/**
 * \def MCUX_CSSL_FP_LOOP_ITERATION_IMPL1
 * \brief Event implementation of a loop iteration (without expectations).
 * \ingroup csslFpCntLoop
 *
 * \see MCUX_CSSL_FP_LOOP_ITERATION_IMPL
 * \see MCUX_CSSL_FP_LOOP_ITERATION_IMPLn
 *
 * \param id Identifier for the loop that is flow protected.
 */
#define MCUX_CSSL_FP_LOOP_ITERATION_IMPL1(id) \
  MCUX_CSSL_FP_LOOP_ITERATION_IMPLn(id, 0u)

/**
 * \def MCUX_CSSL_FP_LOOP_ITERATION_IMPL
 * \brief Event implementation of a loop iteration.
 * \ingroup csslFpCntLoop
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_LOOP_ITERATION_IMPL1
 * \see MCUX_CSSL_FP_LOOP_ITERATION_IMPLn
 *
 * \param id     Identifier for the loop that is flow protected.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_LOOP_ITERATION_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED1(MCUX_CSSL_FP_LOOP_ITERATION_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_LOOP_ITERATIONS_IMPL
 * \brief Expectation implementation of a number of loop iterations.
 * \ingroup csslFpCntLoop
 *
 * \param id    Identifier of the flow protected loop.
 * \param count Number of expected iterations.
 */
#define MCUX_CSSL_FP_LOOP_ITERATIONS_IMPL(id, count) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  ((count) * MCUX_CSSL_FP_LOOP_VALUE(id)) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()



/**
 * \defgroup csslFpCntBranch Branching flow protection
 * \brief Support for flow protected branches.
 * \ingroup mcuxCsslFlowProtection_SecureCounter
 *
 * \declaration{MCUX_CSSL_FP_BRANCH_DECL_IMPL}
 * \event{MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL,MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL}
 * \expectation{MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL,MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL}
 */

/**
 * \def MCUX_CSSL_FP_BRANCH_ID
 * \brief Generator for branch identifiers.
 * \ingroup csslFpCntBranch
 *
 * \param id Identifier for the flow protected branch.
 * \return   Counter value for the given branch.
 */
#define MCUX_CSSL_FP_BRANCH_ID(id) \
  MCUX_CSSL_FP_ID_IMPL()

/**
 * \def MCUX_CSSL_FP_BRANCH_DECL_IMPL
 * \brief Declaration implementation of a flow protected branch.
 * \ingroup csslFpCntBranch
 *
 * \param id Identifier for the branch that is flow protected.
 */
#define MCUX_CSSL_FP_BRANCH_DECL_IMPL(id) \
  MCUX_CSSL_FP_DECL_IMPL(BRANCH, id)

/**
 * \def MCUX_CSSL_FP_BRANCH_VALUE
 * \brief Macro to get the value for a given branch.
 * \ingroup csslFpCntBranch
 *
 * \param id Identifier for the branch that is flow protected.
 * \return   The counter value for the given branch \p id.
 */
#define MCUX_CSSL_FP_BRANCH_VALUE(id) \
  MCUX_CSSL_FP_DECL_NAME(BRANCH, id)

/**
 * \def MCUX_CSSL_FP_BRANCH_POSITIVE_VALUE
 * \brief Value to use for the positive scenario.
 * \ingroup csslFpCntBranch
 */
#define MCUX_CSSL_FP_BRANCH_POSITIVE_VALUE 0x5u

/**
 * \def MCUX_CSSL_FP_BRANCH_NEGATIVE_VALUE
 * \brief Value to use for the negative scenario.
 * \ingroup csslFpCntBranch
 */
#define MCUX_CSSL_FP_BRANCH_NEGATIVE_VALUE 0xAu

/**
 * \def MCUX_CSSL_FP_BRANCH_SCENARIO_IMPL
 * \brief Event implementation for the execution of a specified branch scenario.
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL
 *
 * \param id       Identifier for the branch for which the given \p scenario is
 *                 executed.
 * \param scenario The scenario for a branch is either positive or negative.
 * \param expect   One or more (comma separated) declarations of expected code
 *                 flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_SCENARIO_IMPL(id, scenario, ...) \
  MCUX_CSSL_SC_ADD( \
    (MCUX_CSSL_FP_BRANCH_VALUE(id) * (scenario)) \
    - MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) \
  )

/**
 * \def MCUX_CSSL_FP_BRANCH_POSITIVE_IMPLn
 * \brief Event implementation for the execution of a positive branch scenario
 *        (with expectations).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL1
 *
 * \param id     Identifier for the branch for which the positive scenario is
 *               executed.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_POSITIVE_IMPLn(id, ...) \
  MCUX_CSSL_FP_BRANCH_SCENARIO_IMPL(id, MCUX_CSSL_FP_BRANCH_POSITIVE_VALUE, \
    __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL1
 * \brief Event implementation for the execution of a positive branch scenario
 *        (without expectations).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPLn
 *
 * \param id Identifier for the branch for which the positive scenario is
 *           executed.
 */
#define MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL1(id) \
  MCUX_CSSL_FP_BRANCH_SCENARIO_IMPL(id, MCUX_CSSL_FP_BRANCH_POSITIVE_VALUE, 0u)

/**
 * \def MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL
 * \brief Event implementation for the execution of a positive branch scenario.
 * \ingroup csslFpCntBranch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL1
 * \see MCUX_CSSL_FP_BRANCH_POSITIVE_IMPLn
 *
 * \param id     Identifier for the branch for which the positive scenario is
 *               executed.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED1(MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPLn
 * \brief Event implementation for the execution of a negative branch scenario
 *        (with expectations).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL1
 *
 * \param id     Identifier for the branch for which the negative scenario is
 *               executed.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPLn(id, ...) \
  MCUX_CSSL_FP_BRANCH_SCENARIO_IMPL(id, MCUX_CSSL_FP_BRANCH_NEGATIVE_VALUE, \
    __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL1
 * \brief Event implementation for the execution of a negative branch scenario
 *        (without expectations).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPLn
 *
 * \param id Identifier for the branch for which the negative scenario is
 *           executed.
 */
#define MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL1(id) \
  MCUX_CSSL_FP_BRANCH_SCENARIO_IMPL(id, MCUX_CSSL_FP_BRANCH_NEGATIVE_VALUE, 0u)

/**
 * \def MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL
 * \brief Event implementation for the execution of a negative branch scenario.
 * \ingroup csslFpCntBranch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL1
 * \see MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPLn
 *
 * \param id     Identifier for the branch for which the negative scenario is
 *               executed.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED1(MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_IMPL
 * \brief Expectation implementation of an executed specified branch scenario.
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL
 *
 * \param id        Identifier of the flow protected branch.
 * \param scenario The scenario for a branch is either positive or negative.
 * \param condition Condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_IMPL(id, scenario, condition) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  MCUX_CSSL_FP_CONDITIONAL_IMPL(condition, \
    MCUX_CSSL_FP_BRANCH_VALUE(id) * (scenario)) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL2
 * \brief Expectation implementation of an executed positive branch (with
 *        condition).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL1
 *
 * \param id        Identifier of the flow protected branch.
 * \param condition Condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL2(id, condition) \
  MCUX_CSSL_FP_BRANCH_TAKEN_IMPL(id, MCUX_CSSL_FP_BRANCH_POSITIVE_VALUE, condition)

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL1
 * \brief Expectation implementation of an executed positive branch (without
 *        condition).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL2
 *
 * \param id Identifier of the flow protected branch.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL1(id) \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8, "The macro does not contain a composite expression.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_DEVIATE(MISRA_C_2012_Rule_14_3, "The usage of an invariant condition here is intended to keep the macro structures more clear.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1, "True is of boolean type.") \
    MCUX_CSSL_FP_BRANCH_TAKEN_IMPL(id, MCUX_CSSL_FP_BRANCH_POSITIVE_VALUE, true) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_DEVIATE(MISRA_C_2012_Rule_14_3) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8)

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL
 * \brief Expectation implementation of an executed positive branch.
 * \ingroup csslFpCntBranch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL1
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL2
 *
 * \param id        Identifier of the flow protected branch.
 * \param condition Optional, condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED2(MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL2
 * \brief Expectation implementation of an executed negative branch (with
 *        condition).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL1
 *
 * \param id        Identifier of the flow protected branch.
 * \param condition Condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL2(id, condition) \
  MCUX_CSSL_FP_BRANCH_TAKEN_IMPL(id, MCUX_CSSL_FP_BRANCH_NEGATIVE_VALUE, condition)

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL1
 * \brief Expectation implementation of an executed negative branch (without
 *        condition).
 * \ingroup csslFpCntBranch
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL2
 *
 * \param id Identifier of the flow protected branch.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL1(id) \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8, "The macro does not contain a composite expression.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_DEVIATE(MISRA_C_2012_Rule_14_3, "The usage of an invariant condition here is intended to keep the macro structures more clear.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1, "True is of boolean type.") \
    MCUX_CSSL_FP_BRANCH_TAKEN_IMPL(id, MCUX_CSSL_FP_BRANCH_NEGATIVE_VALUE, true) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_DEVIATE(MISRA_C_2012_Rule_14_3) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8)

/**
 * \def MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL
 * \brief Expectation implementation of an executed negative branch.
 * \ingroup csslFpCntBranch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL1
 * \see MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL2
 *
 * \param id        Identifier of the flow protected branch.
 * \param condition Optional, condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED2(MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL, __VA_ARGS__)



/**
 * \defgroup csslFpCntSwitch Switching flow protection
 * \brief Support for flow protected switches.
 * \ingroup mcuxCsslFlowProtection_SecureCounter
 *
 * \declaration{MCUX_CSSL_FP_SWITCH_DECL_IMPL}
 * \event{MCUX_CSSL_FP_SWITCH_CASE_IMPL,MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL}
 * \expectation{MCUX_CSSL_FP_SWITCH_TAKEN_IMPL,MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL}
 */

/**
 * \def MCUX_CSSL_FP_SWITCH_ID
 * \brief Generator for switch identifiers.
 * \ingroup csslFpCntSwitch
 *
 * \param id Identifier for the flow protected switch.
 * \return   Counter value for the given loop.
 */
#define MCUX_CSSL_FP_SWITCH_ID(id) \
  MCUX_CSSL_FP_ID_IMPL()

/**
 * \def MCUX_CSSL_FP_SWITCH_DECL_IMPL
 * \brief Declaration implementation of a flow protected switch.
 * \ingroup csslFpCntSwitch
 *
 * \param id Identifier for the switch that is flow protected.
 */
#define MCUX_CSSL_FP_SWITCH_DECL_IMPL(id) \
    MCUX_CSSL_FP_DECL_IMPL(SWITCH, id)

/**
 * \def MCUX_CSSL_FP_SWITCH_VALUE
 * \brief Macro to get the value for a given switch.
 * \ingroup csslFpCntSwitch
 *
 * \param id Identifier for the switch that is flow protected.
 * \return   The counter value for the given switch \p id.
 */
#define MCUX_CSSL_FP_SWITCH_VALUE(id) \
  MCUX_CSSL_FP_DECL_NAME(SWITCH, id)

/**
 * \def MCUX_CSSL_FP_SWITCH_CASE_IMPLn
 * \brief Case that is being handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_CASE_IMPL
 * \see MCUX_CSSL_FP_SWITCH_CASE_IMPL2
 *
 * \param id     Identifier of the flow protected switch.
 * \param case   Case value that is chosen in the switch.
 * \param expect One or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_SWITCH_CASE_IMPLn(id, case, ...) \
  MCUX_CSSL_SC_ADD( \
    (MCUX_CSSL_FP_SWITCH_VALUE(id) * (case)) \
    - MCUX_CSSL_FP_EXPECTATIONS(__VA_ARGS__) \
  )

/**
 * \def MCUX_CSSL_FP_SWITCH_CASE_IMPL2
 * \brief Case that is being handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_CASE_IMPL
 * \see MCUX_CSSL_FP_SWITCH_CASE_IMPLn
 *
 * \param id   Identifier of the flow protected switch.
 * \param case Case value that is chosen in the switch.
 */
#define MCUX_CSSL_FP_SWITCH_CASE_IMPL2(id, case) \
  MCUX_CSSL_FP_SWITCH_CASE_IMPLn(id, case, 0u)

/**
 * \def MCUX_CSSL_FP_SWITCH_CASE_IMPL
 * \brief Case that is being handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_SWITCH_CASE_IMPL2
 * \see MCUX_CSSL_FP_SWITCH_CASE_IMPLn
 *
 * \param id     Identifier of the flow protected switch.
 * \param case   Case value that is chosen in the switch.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_SWITCH_CASE_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED2(MCUX_CSSL_FP_SWITCH_CASE_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_SWITCH_DEFAULT_VALUE
 * \brief Value to use for default case.
 * \ingroup csslFpCntSwitch
 */
#define MCUX_CSSL_FP_SWITCH_DEFAULT_VALUE \
  (0xDEFAu)

/**
 * \def MCUX_CSSL_FP_SWITCH_DEFAULT_IMPLn
 * \brief Case that is being handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL
 * \see MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL1
 *
 * \param id     Identifier of the flow protected switch.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_SWITCH_DEFAULT_IMPLn(id, ...) \
  MCUX_CSSL_FP_SWITCH_CASE_IMPLn( \
    id, MCUX_CSSL_FP_SWITCH_DEFAULT_VALUE, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL1
 * \brief Case that is being handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL
 * \see MCUX_CSSL_FP_SWITCH_DEFAULT_IMPLn
 *
 * \param id Identifier of the flow protected switch.
 */
#define MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL1(id) \
  MCUX_CSSL_FP_SWITCH_DEFAULT_IMPLn(id, 0u) \

/**
 * \def MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL
 * \brief Case that is being handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL1
 * \see MCUX_CSSL_FP_SWITCH_DEFAULT_IMPLn
 *
 * \param id     Identifier of the flow protected switch.
 * \param expect Zero or more (comma separated) declarations of expected code
 *               flow behavior related to this event.
 */
#define MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED1(MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_SWITCH_TAKEN_IMPL3
 * \brief Expected that a specific case is handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_IMPL
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_IMPL2
 *
 * \param id        Identifier of the flow protected switch.
 * \param case      Value of the case that is expected to be chosen in the
 *                  switch.
 * \param condition Optional, condition under which the \p case is taken.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_IMPL3(id, case, condition) \
  MCUX_CSSL_ANALYSIS_START_PATTERN_SC_INTEGER_OVERFLOW() \
  MCUX_CSSL_FP_CONDITIONAL_IMPL(condition, \
    MCUX_CSSL_FP_SWITCH_VALUE(id) * (case)) \
  MCUX_CSSL_ANALYSIS_STOP_PATTERN_SC_INTEGER_OVERFLOW()

/**
 * \def MCUX_CSSL_FP_SWITCH_TAKEN_IMPL2
 * \brief Expected that a specific case is handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_IMPL
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_IMPL3
 *
 * \param id   Identifier of the flow protected switch.
 * \param case Value of the case that is expected to be chosen in the switch.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_IMPL2(id, case) \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8, "The macro does not contain a composite expression.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_DEVIATE(MISRA_C_2012_Rule_14_3, "The usage of an invariant condition here is intended to keep the macro structures more clear.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1, "True is of boolean type.") \
    MCUX_CSSL_FP_SWITCH_TAKEN_IMPL3(id, case, true) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_DEVIATE(MISRA_C_2012_Rule_14_3) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8)

/**
 * \def MCUX_CSSL_FP_SWITCH_TAKEN_IMPL
 * \brief Expected that a specific case is handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_IMPL2
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_IMPL3
 *
 * \param id        Identifier of the flow protected switch.
 * \param case      Value of the case that is expected to be chosen in the
 *                  switch.
 * \param condition Optional, condition under which the \p case is taken.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED3(MCUX_CSSL_FP_SWITCH_TAKEN_IMPL, __VA_ARGS__)

/**
 * \def MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL2
 * \brief Expected that default case is handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL1
 *
 * \param id        Identifier of the flow protected switch.
 * \param condition Condition under which the default case is taken.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL2(id, condition) \
  MCUX_CSSL_FP_SWITCH_TAKEN_IMPL3(id, MCUX_CSSL_FP_SWITCH_DEFAULT_VALUE, condition)

/**
 * \def MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL1
 * \brief Expected that default case is handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL2
 *
 * \param id        Identifier of the flow protected switch.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL1(id) \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8, "The macro does not contain a composite expression.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_DEVIATE(MISRA_C_2012_Rule_14_3, "The usage of an invariant condition here is intended to keep the macro structures more clear.") \
  MCUX_CSSL_ANALYSIS_COVERITY_START_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1, "True is of boolean type.") \
    MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL2(id, true) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_1) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_DEVIATE(MISRA_C_2012_Rule_14_3) \
  MCUX_CSSL_ANALYSIS_COVERITY_STOP_FALSE_POSITIVE(MISRA_C_2012_Rule_10_8)

/**
 * \def MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL
 * \brief Expected that default case is handled from a switch.
 * \ingroup csslFpCntSwitch
 *
 * Implemented as an overloaded macro to simplify the use of the API.
 *
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL1
 * \see MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL2
 *
 * \param id        Identifier of the flow protected switch.
 * \param condition Optional, condition under which the default case is taken.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL(...) \
  MCUX_CSSL_CPP_OVERLOADED2(MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL, __VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_ASSERT_CALLBACK
 * @brief Fallback assert callback implementation.
 * @api
 * @ingroup csslFpCntExpect
 *
 * This macro will be executed if an #MCUX_CSSL_FP_ASSERT fails. In general this
 * behavior should be defined by the user. This implementation is only in place
 * to ensure that an implementation is always available.
 *
 * This is implemented a division by 0, which should trigger a compiler warning
 * when used, to inform the user that the default implementation is used.
 * Additionally, when still used at run-time it should trigger some system
 * exception.
 *
 * \see MCUX_CSSL_FP_ASSERT
 */
#ifndef MCUX_CSSL_FP_ASSERT_CALLBACK
  #define MCUX_CSSL_FP_ASSERT_CALLBACK() \
    return 1/0 /* Fallback ASSERT callback is used, please provide your own. */
#endif

#endif /* MCUX_CSSL_FLOW_PROTECTION_SECURE_COUNTER_COMMON_H_ */
