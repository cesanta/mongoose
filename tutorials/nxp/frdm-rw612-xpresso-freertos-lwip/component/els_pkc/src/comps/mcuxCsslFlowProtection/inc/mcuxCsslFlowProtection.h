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
 * @file  mcuxCsslFlowProtection.h
 * @brief Provides the API for the CSSL flow protection mechanism.
 */

#ifndef MCUX_CSSL_FLOW_PROTECTION_H_
#define MCUX_CSSL_FLOW_PROTECTION_H_

#include <mcuxCsslAnalysis.h>

/* Include the actual implementation of the flow protection mechanism. */
#include <mcuxCsslFlowProtection_Impl.h>

/**
 * @addtogroup mcuxCsslAPI MCUX CSSL -- API
 *
 * @defgroup mcuxCsslFlowProtection Flow Protection API
 * @brief Flow protection mechanism.
 * @ingroup mcuxCsslAPI
 */


/**
 * @defgroup csslFpCore Flow protection core functionality
 * @brief Flow protection handling core functionality.
 * @ingroup mcuxCsslFlowProtection
 *
 * @todo Extend this description of the core functionality which relies
 *       basically on the function calling flow protection.
 *
 * @declaration{MCUX_CSSL_FP_FUNCTION_DECL}
 * @event{MCUX_CSSL_FP_FUNCTION_CALL}
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED}
 */

/**
 * @defgroup csslFpFunction Function calling flow protection
 * @brief Support for flow protected functions.
 * @ingroup mcuxCsslFlowProtection
 *
 * @declaration{MCUX_CSSL_FP_FUNCTION_DECL}
 * @event{MCUX_CSSL_FP_FUNCTION_CALL}
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED}
 */


MCUX_CSSL_ANALYSIS_START_SUPPRESS_TEXT_IN_COMMENTS("Comments outline example sequences. For more readability, additional inner comments might be added.")
/**
 * @def MCUX_CSSL_FP_PROTECTED_TYPE
 * @brief Based on a given base type, builds a return type with flow
 *        protection.
 * @ingroup csslFpFunction
 *
 * This macro must be used to wrap the function return type. For example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DECL(someFunction)
 * MCUX_CSSL_FP_PROTECTED_TYPE(uint32_t) someFunction(void);
 * @endcode
 *
 * Note that depending on the selected flow protection mechanism, the width of
 * the result type may be limited to 32 bits or less to allow encoding a
 * protection token in the other half of a 64-bit return value.
 *
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 *
 * @param resultType The type to be converted into a protected type.
 */
#define MCUX_CSSL_FP_PROTECTED_TYPE(resultType) \
  MCUX_CSSL_FP_PROTECTED_TYPE_IMPL(resultType)

/**
 * @def MCUX_CSSL_FP_COUNTER_STMT
 * @brief A statement which is only evaluated if a secure counter is used.
 * @api
 * @ingroup csslFpFunction
 *
 * This macro can be used to create counting variables that are only present if
 * the active configuration uses a secure counter, to avoid warnings about
 * unused variables.
 *
 * @param statement The statement to be conditionally included.
 */
#define MCUX_CSSL_FP_COUNTER_STMT(statement) \
  MCUX_CSSL_FP_COUNTER_STMT_IMPL(statement)


/**
 * @def MCUX_CSSL_FP_FUNCTION_DECL
 * @brief Declaration of a flow protected function.
 * @api
 * @ingroup csslFpFunction
 *
 * This declaration must be placed just in front of the actual function
 * declaration. For example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DECL(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void);
 * @endcode
 *
 * @event{MCUX_CSSL_FP_FUNCTION_CALL}
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED}
 *
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 * @see MCUX_CSSL_FP_FUNCTION_POINTER
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 *
 * @param id Identifier for the function that is flow protected.
 * @param ptrType Optional, pointer type matching this function.
 */
#define MCUX_CSSL_FP_FUNCTION_DECL(...) \
  MCUX_CSSL_FP_FUNCTION_DECL_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_DEF
 * @brief Definition of a flow protected function.
 * @api
 * @ingroup csslFpFunction
 *
 * This definition macro must be placed just in front of the actual function
 * definition, that has been previously declared as flow protected using
 * #MCUX_CSSL_FP_FUNCTION_DECL. For example:
 * @code
 * // someHeader.h
 * MCUX_CSSL_FP_FUNCTION_DECL(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void);
 *
 * // someFile.c
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   // some function body
 * }
 * @endcode
 *
 * @see MCUX_CSSL_FP_FUNCTION_DECL
 * @see MCUX_CSSL_FP_FUNCTION_POINTER
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 *
 * @param id Identifier for the function that is flow protected.
 * @param ptrType Optional, pointer type matching this function.
 */
#define MCUX_CSSL_FP_FUNCTION_DEF(...) \
  MCUX_CSSL_FP_FUNCTION_DEF_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_POINTER
 * @brief Definition of a flow protected function pointer.
 * @api
 * @ingroup csslFpFunction
 *
 * This definition macro must be placed around a function pointer
 * definition. For example:
 * @code
 * // someHeader.h
 * MCUX_CSSL_FP_FUNCTION_POINTER(ptrType,
 * typedef void (*ptrType)(void));
 *
 * MCUX_CSSL_FP_FUNCTION_DECL(someFunction, ptrType) // Note: no semicolon here
 * uint32_t someFunction(void);
 *
 * // someFile.c
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction, ptrType) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   // some function body
 * }
 * @endcode
 *
 * @see MCUX_CSSL_FP_FUNCTION_DECL
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 *
 * @param type Identifier for the function pointer type that is flow protected.
 * @param definition Actual type definition of the function pointer type.
 */
#define MCUX_CSSL_FP_FUNCTION_POINTER(type, definition) \
  MCUX_CSSL_FP_FUNCTION_POINTER_IMPL(type, definition)

/**
 * @def MCUX_CSSL_FP_FUNCTION_ENTRY
 * @brief Flow protection handler for the function entry point.
 * @api
 * @ingroup csslFpFunction
 *
 * This entry macro should be placed at the start of the function body that
 * needs to be protected. The function must have been declared before as flow
 * protected using #MCUX_CSSL_FP_FUNCTION_DECL. For example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // remainder of the function body
 * }
 * @endcode
 *
 * The only statements that should be placed before this one, are declarations
 * for flow protected operations that are already used as expectations in this
 * macro. For example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(uint32_t count)
 * {
 *   MCUX_CSSL_FP_LOOP_DECL(someLoop);
 *   MCUX_CSSL_FP_LOOP_DECL(otherLoop);
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction,
 *     MCUX_CSSL_FP_LOOP_ITERATIONS(someLoop, count),
 *     MCUX_CSSL_FP_LOOP_ITERATIONS(otherLoop, 2u * count)
 *   );
 *   // Remainder of the function body, where someLoop makes count iterations,
 *   // and otherLoop 2*count iterations.
 * }
 * @endcode
 *
 * @see MCUX_CSSL_FP_FUNCTION_DECL
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:     Identifier of the function that has just been entered. <br>
 *        - expect: Zero or more (comma separated) declarations of expected code
 *               flow behavior.
 */
#define MCUX_CSSL_FP_FUNCTION_ENTRY(...) \
  MCUX_CSSL_FP_FUNCTION_ENTRY_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_FUNCTION_EXIT
 * @brief Flow protection handler for the function exit point.
 * @api
 * @ingroup csslFpFunction
 *
 * This exit macro must replace the regular \c return statements of a protected
 * function. Given the following unprotected example:
 * @code
 * uint32_t someFunction(void)
 * {
 *   // some function body
 *   return 0;
 * }
 * @endcode
 * The protected version would become:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // remainder of the function body
 *   MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0);
 * }
 * @endcode
 *
 * @see MCUX_CSSL_FP_FUNCTION_DECL
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:     Identifier of the function from which we will exit.
 *        - result: Result that should be encoded in the return value.
 *        - expect: Zero or more (comma separated) declarations of expected code
 *               flow behavior.
 * @return       A value in which both \p result and a flow protection token
 *               are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT(...) \
  MCUX_CSSL_FP_FUNCTION_EXIT_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @brief Flow protection handler for the function exit point which includes
 *        an actual check of the code flow.
 * @api
 * @ingroup csslFpFunction
 *
 * This exit macro must replace the regular \c return statements of a protected
 * function. In addition to #MCUX_CSSL_FP_FUNCTION_EXIT it also checks the flow
 * protection, and selects the return value accordingly. For example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // remainder of the function body
 *   MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK(someFunction, 0, 0xFAu);
 * }
 * @endcode
 *
 * @see MCUX_CSSL_FP_FUNCTION_DECL
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:      Identifier of the function from which we will exit.
 *        - pass:    Result that should be encoded in the return value if the flow
 *                   protection check passed.
 *        - fail:    Result that should be encoded in the return value if the flow
 *                   protection check failed.
 *        - expect:  Zero or more (comma separated) declarations of expected code
 *                  flow behavior.
 * @return       A value in which both the result (either \p pass or \p fail)
 *               and a flow protection token are encoded.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK(...) \
  MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_FUNCTION_EXIT_VOID
 * @brief Flow protection handler for the exit point of functions with the
 *        return type \c void.
 * @api
 * @ingroup csslFpFunction
 *
 * This exit macro must replace the regular \c return statements of a protected
 * void function. Given the following unprotected example:
 * @code
 * void someFunction(void)
 * {
 *   // some function body
 *   return 0;
 * }
 * @endcode
 * The protected version would become:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * void someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // remainder of the function body
 *   MCUX_CSSL_FP_FUNCTION_EXIT_VOID(someFunction);
 * }
 * @endcode
 *
 * @see MCUX_CSSL_FP_FUNCTION_DECL
 * @see MCUX_CSSL_FP_FUNCTION_DEF
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 *
 * @param ...       The following parameters need to be passed (comma separated):
 *        - id:     Identifier of the function from which we will exit.
 *        - expect: Zero or more (comma separated) declarations of expected code
 *                  flow behavior.
 * @return       A protected return value of type void.
 */
#define MCUX_CSSL_FP_FUNCTION_EXIT_VOID(...) \
  MCUX_CSSL_FP_FUNCTION_EXIT_VOID_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_RESULT
 * @brief Extract the result value from a protected \p return value.
 * @ingroup csslFpFunction
 *
 * This macro should mainly be used internally to extract the original return value
 * from a protected value, e.g., in MCUX_CSSL_FP_FUNCTION_CALL_IMPL.
 *
 * @param ...       The following parameters need to be passed (comma seperated):
 *        - type:   Optional, type of the result (default: uint32_t).
 *        - return: The protected return value which contains the result.
 */
#define MCUX_CSSL_FP_RESULT(...) \
  MCUX_CSSL_FP_RESULT_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_PROTECTION_TOKEN
 * @brief Extract the protection token value from a protected \p return value.
 * @ingroup csslFpFunction
 *
 * Note that this macro is only used with a local security counter,
 * e.g. for configuration CSSL_SC_USE_SW_LOCAL
 *
 * @param return The protected return value which contains the protection token.
 */
#define MCUX_CSSL_FP_PROTECTION_TOKEN(return) \
  MCUX_CSSL_FP_PROTECTION_TOKEN_IMPL(return)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL
 * @brief Call a flow protected function.
 * @api
 * @ingroup csslFpFunction
 *
 * This function call macro encapsulates the flow protection handling needed
 * for calling a function. In particular it takes care of extracting the flow
 * protection token from the return value (which has been inserted by
 * #MCUX_CSSL_FP_FUNCTION_EXIT or #MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK) and
 * incorporating that in the flow protection of the current function. For
 * example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL(result, otherFunction());
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *     MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction)
 *   );
 * }
 * @endcode
 *
 * For functions returning void, the macro #MCUX_CSSL_FP_FUNCTION_CALL_VOID
 * exists.
 *
 * @declaration{MCUX_CSSL_FP_FUNCTION_DECL}
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - result: Fresh variable name to store the result of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_VOID
 * @brief Call a flow protected void function.
 * @api
 * @ingroup csslFpFunction
 *
 * This function call macro encapsulates the flow protection handling needed
 * for calling a void function. In particular it takes care of extracting the
 * flow protection token from the return value (which has been inserted by
 * #MCUX_CSSL_FP_FUNCTION_EXIT or #MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK) and
 * incorporating that in the flow protection of the current function. For
 * example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_VOID(otherFunction());
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *     MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction)
 *   );
 * }
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_FUNCTION_DECL}
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - call:   The (protected) void function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_VOID_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED
 * @brief Call a flow protected function from unprotected code.
 * @api
 * @ingroup csslFpFunction
 *
 * This function call macro encapsulates the flow protection handling needed
 * for calling a function from within a function which does not have local
 * flow protection, or which uses a different flow protection mechanism than
 * the one provided by CSSL. In particular it takes care of extracting the
 * protection token and result from the return value (which has been inserted
 * by #MCUX_CSSL_FP_FUNCTION_EXIT or #MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK).
 * For example:
 * @code
 * uint32_t someUnprotectedFunction(void)
 * {
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(
 *     result,
 *     token,
 *     otherFunction());
 *   // Check the protection token
 *   if(MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction) != token)
 *   {
 *     return FAULT;
 *   }
 *   // ... The following code may use result as a variable ...
 * }
 * @endcode
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - result: Fresh variable name to store the result of \p call.
 *        - token:  Fresh variable name to store the protection token of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED_IMPL(__VA_ARGS__)

  /**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_VOID_PROTECTED
 * @brief Call a flow protected void function from unprotected code.
 * @api
 * @ingroup csslFpFunction
 *
 * This function call macro encapsulates the flow protection handling needed
 * for calling a void function from within a function which does not have flow
 * protection, or which uses a different flow protection mechanism than the one
 * provided by CSSL. In particular it takes care of extracting the protection
 * token and result from the return value (which has been inserted by
 * #MCUX_CSSL_FP_FUNCTION_EXIT or #MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK).
 * For example:
 * @code
 * uint32_t someUnprotectedFunction(void)
 * {
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_PROTECTED(
 *     token,
 *     protectedVoidFunction());
 *   // Check the protection token
 *   if(MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction) != token)
 *   {
 *     return FAULT;
 *   }
 *   // ...
 * }
 * @endcode
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - token:  Fresh variable name to store the protection token of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_PROTECTED(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_VOID_PROTECTED_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_BEGIN
 * @brief Call a flow protected function and check the protection token.
 * @api
 * @ingroup csslFpFunction
 *
 * This function call macro encapsulates the flow protection handling needed
 * for calling a function from within a function which does not have local
 * flow protection, or which uses a different flow protection mechanism than
 * the one provided by CSSL. In particular it takes care of extracting the
 * protection token and result from the return value (which has been inserted
 * by #MCUX_CSSL_FP_FUNCTION_EXIT or #MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK).
 * For example:
 * @code
 * uint32_t someUnprotectedFunction(void)
 * {
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
 *     result,
 *     token,
 *     otherFunction());
 *   // Check the protection token
 *   if(MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction) != token)
 *   {
 *     return FAULT;
 *   }
 *   // ... The following code may use result as a variable ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_END();
 *   // ... result is invalid here ...
 * }
 * @endcode
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - result: Fresh variable name to store the result of \p call.
 *        - token:  Fresh variable name to store the protection token of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_BEGIN_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_END
 * @brief End a function call section started by
 * #MCUX_CSSL_FP_FUNCTION_CALL_BEGIN.
 * @api
 * @ingroup csslFpFunction
 *
 * Example:
 * @code
 * uint32_t someUnprotectedFunction(void)
 * {
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(
 *     result,
 *     token,
 *     otherFunction());
 *   // Check the protection token
 *   if(MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction) != token)
 *   {
 *     return FAULT;
 *   }
 *   // ... The following code may use result as a variable ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_END();
 *   // ... result is invalid here ...
 * }
 * @endcode
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - result: Fresh variable name to store the result of \p call.
 *        - token:  Fresh variable name to store the protection token of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_END(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_END_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN
 * @brief Call a flow protected void function and check the protection token.
 * @api
 * @ingroup csslFpFunction
 *
 * This function call macro encapsulates the flow protection handling needed
 * for calling a void function from within a function which does not have local
 * flow protection, or which uses a different flow protection mechanism than
 * the one provided by CSSL. In particular it takes care of extracting the
 * protection token from the return value (which has been inserted
 * by #MCUX_CSSL_FP_FUNCTION_EXIT or #MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK).
 * For example:
 * @code
 * uint32_t someUnprotectedFunction(void)
 * {
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN(
 *     token,
 *     otherFunction());
 *   // Check the protection token
 *   if(MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction) != token)
 *   {
 *     return FAULT;
 *   }
 *   MCUX_CSSL_FP_FUNCTION_CALL_VOID_END();
 * }
 * @endcode
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - token:  Fresh variable name to store the protection token of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALL_VOID_END
 * @brief End a void function call section started by
 * #MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN.
 * @api
 * @ingroup csslFpFunction
 *
 * Example:
 * @code
 * uint32_t someUnprotectedFunction(void)
 * {
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL_VOID_BEGIN(
 *     token,
 *     otherFunction());
 *   // Check the protection token
 *   if(MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction) != token)
 *   {
 *     return FAULT;
 *   }
 *   MCUX_CSSL_FP_FUNCTION_CALL_VOID_END();
 * }
 * @endcode
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - token:  Fresh variable name to store the protection token of \p call.
 *        - call:   The (protected) function call that must be performed.
 */
#define MCUX_CSSL_FP_FUNCTION_CALL_VOID_END(...) \
  MCUX_CSSL_FP_FUNCTION_CALL_VOID_END_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_FUNCTION_CALLED
 * @brief Expectation of a called function.
 * @api
 * @ingroup csslFpFunction
 *
 * This expectation macro indicates to the flow protection mechanism that a
 * function call is expected to happen (if placed before the actual call), for
 * example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction,
 *     MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction)
 *   );
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL(result, otherFunction());
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0);
 * }
 * @endcode
 * Or that a function call has happened (if placed after the actual call), for
 * example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_CALL(result, otherFunction());
 *   // ...
 *   MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *     MCUX_CSSL_FP_FUNCTION_CALLED(otherFunction)
 *   );
 * }
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_FUNCTION_DECL}
 * @event{MCUX_CSSL_FP_FUNCTION_CALL}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        -id: Identifier of the function that is expected to be called.
 */
#define MCUX_CSSL_FP_FUNCTION_CALLED(...) \
  MCUX_CSSL_FP_FUNCTION_CALLED_IMPL(__VA_ARGS__)


/**
 * @def MCUX_CSSL_FP_FUNCTION_ENTERED
 * @brief Expectation implementation of an entered (but not exited) function.
 * @ingroup csslFpFunction
 *
 * This expectation macro indicates to the flow protection mechanism that a
 * function entry has happened, for example:
 * @code
 * MCUX_CSSL_FP_FUNCTION_DEF(someFunction) // Note: no semicolon here
 * uint32_t someFunction(void)
 * {
 *   MCUX_CSSL_FP_FUNCTION_ENTRY(someFunction);
 *   // ...
 *   MCUX_CSSL_FP_ASSERT(MCUX_CSSL_FP_FUNCTION_ENTERED(someFunction);
 *   // ...
 * }
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_FUNCTION_DECL}
 * @event{MCUX_CSSL_FP_FUNCTION_CALL}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 * @see MCUX_CSSL_FP_FUNCTION_CALLED
 * @see MCUX_CSSL_FP_ASSERT
 *
 * @param id Identifier of the function that is expected to be entered.
 * @return   Counter value for the given function.
 */
#define MCUX_CSSL_FP_FUNCTION_ENTERED(id) \
  MCUX_CSSL_FP_FUNCTION_ENTERED_IMPL(id)


/**
 * @defgroup csslFpLoop Looping flow protection
 * @brief Support for flow protected loops.
 * @ingroup mcuxCsslFlowProtection
 *
 * @declaration{MCUX_CSSL_FP_LOOP_DECL}
 * @event{MCUX_CSSL_FP_LOOP_ITERATION}
 * @expectation{MCUX_CSSL_FP_LOOP_ITERATIONS}
 */

/**
 * @def MCUX_CSSL_FP_LOOP_DECL
 * @brief Declaration of a flow protected loop.
 * @api
 * @ingroup csslFpLoop
 *
 * To inform the flow protection mechanism about a loop that needs to be
 * protected, a loop identifier needs to be declared. This identifier can then
 * be used in the event and expectation macros. For example:
 * @code
 * MCUX_CSSL_FP_LOOP_DECL(someLoopIdentifier);
 * for (uint32_t i = 0; i < 8; ++i)
 * {
 *   MCUX_CSSL_FP_LOOP_ITERATION(someLoopIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   MCUX_CSSL_FP_LOOP_ITERATIONS(someLoopIdentifier, 8)
 * );
 * @endcode
 *
 * @event{MCUX_CSSL_FP_LOOP_ITERATION}
 * @expectation{MCUX_CSSL_FP_LOOP_ITERATIONS}
 *
 * @param id Identifier for the loop that is flow protected.
 */
#define MCUX_CSSL_FP_LOOP_DECL(id) \
  MCUX_CSSL_FP_LOOP_DECL_IMPL(id)

/**
 * @def MCUX_CSSL_FP_LOOP_ITERATION
 * @brief Perform a loop iteration.
 * @api
 * @ingroup csslFpLoop
 *
 * This loop iteration macro informs the flow mechanism that an iteration event
 * is performed for the loop declared by #MCUX_CSSL_FP_LOOP_DECL with the given
 * \p id. For example:
 * @code
 * MCUX_CSSL_FP_LOOP_DECL(someLoopIdentifier);
 * for (uint32_t i = 0; i < 8; ++i)
 * {
 *   MCUX_CSSL_FP_LOOP_ITERATION(someLoopIdentifier);
 * }
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_LOOP_DECL}
 * @expectation{MCUX_CSSL_FP_LOOP_ITERATIONS}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:      Identifier for the loop that is flow protected.
 *        - expect:  Zero or more (comma separated) declarations of expected code
 *                   flow behavior related to this event.
 */
#define MCUX_CSSL_FP_LOOP_ITERATION(...) \
  MCUX_CSSL_FP_LOOP_ITERATION_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_LOOP_ITERATIONS
 * @brief Expected number of loop iterations.
 * @api
 * @ingroup csslFpLoop
 *
 * This expectation macro indicates to the flow protection mechanism that the
 * loop declared by #MCUX_CSSL_FP_LOOP_DECL with the given \p id has made
 * \p count iterations. For example:
 * @code
 * MCUX_CSSL_FP_LOOP_DECL(someLoopIdentifier);
 * for (uint32_t i = 0; i < 8; ++i)
 * {
 *   MCUX_CSSL_FP_LOOP_ITERATION(someLoopIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   MCUX_CSSL_FP_LOOP_ITERATIONS(someLoopIdentifier, 8)
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_LOOP_DECL}
 * @event{MCUX_CSSL_FP_LOOP_ITERATION}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 *
 * @param id    Identifier of the flow protected loop.
 * @param count Number of expected iterations.
 */
#define MCUX_CSSL_FP_LOOP_ITERATIONS(id, count) \
  MCUX_CSSL_FP_LOOP_ITERATIONS_IMPL(id, count)



/**
 * @defgroup csslFpBranch Branching flow protection
 * @brief Support for flow protected branches.
 * @ingroup mcuxCsslFlowProtection
 *
 * @declaration{MCUX_CSSL_FP_BRANCH_DECL}
 * @event{MCUX_CSSL_FP_BRANCH_POSITIVE,MCUX_CSSL_FP_BRANCH_NEGATIVE}
 * @expectation{MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE,MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE}
 */

/**
 * @def MCUX_CSSL_FP_BRANCH_DECL
 * @brief Declaration of a flow protected branch.
 * @api
 * @ingroup csslFpBranch
 *
 * To inform the flow protection mechanism about a branch that needs to be
 * protected, a branch identifier needs to be declared. This identifier can
 * then be used in the events and expectation macros. For example:
 * @code
 * MCUX_CSSL_FP_BRANCH_DECL(someBranchIdentifier);
 * if (condition)
 * {
 *   MCUX_CSSL_FP_BRANCH_POSITIVE(someBranchIdentifier);
 * }
 * else
 * {
 *   MCUX_CSSL_FP_BRANCH_NEGATIVE(someBranchIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   MCUX_CSSL_FP_BRANCH_TAKEN(someBranchIdentifier,
 *     MCUX_CSSL_FP_BRANCH_POSITIVE_SCENARIO, condition)
 * );
 * @endcode
 *
 * @event{MCUX_CSSL_FP_BRANCH_POSITIVE,MCUX_CSSL_FP_BRANCH_NEGATIVE}
 * @expectation{MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE,MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE}
 *
 * @param id Identifier for the branch that is flow protected.
 */
#define MCUX_CSSL_FP_BRANCH_DECL(id) \
  MCUX_CSSL_FP_BRANCH_DECL_IMPL(id)

/**
 * @def MCUX_CSSL_FP_BRANCH_POSITIVE
 * @brief Positive scenario for a branch is executed.
 * @api
 * @ingroup csslFpBranch
 *
 * This branch event macro informs the flow mechanism that the positive scenario
 * of the branch is executed for the branch declared by
 * #MCUX_CSSL_FP_BRANCH_DECL with the given \p id. For example:
 * @code
 * MCUX_CSSL_FP_BRANCH_DECL(someBranchIdentifier);
 * if (condition)
 * {
 *   MCUX_CSSL_FP_BRANCH_POSITIVE(someBranchIdentifier);
 * }
 * else
 * {
 *   MCUX_CSSL_FP_BRANCH_NEGATIVE(someBranchIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   MCUX_CSSL_FP_CONDITIONAL_IMPL(!condition,
 *     MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE(someBranchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_BRANCH_DECL}
 * @expectation{MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:     Identifier for the branch for which the positive scenario is
 *                  executed.
 *        - expect: Zero or more (comma separated) declarations of expected code
 *                  flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_POSITIVE(...) \
  MCUX_CSSL_FP_BRANCH_POSITIVE_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_BRANCH_NEGATIVE
 * @brief Negative scenario of a branch is executed.
 * @api
 * @ingroup csslFpBranch
 *
 * This branch event macro informs the flow mechanism that the positive scenario
 * of the branch is executed for the branch declared by
 * #MCUX_CSSL_FP_BRANCH_DECL with the given \p id. For example:
 * @code
 * MCUX_CSSL_FP_BRANCH_DECL(someBranchIdentifier);
 * if (condition)
 * {
 *   MCUX_CSSL_FP_BRANCH_POSITIVE(someBranchIdentifier);
 * }
 * else
 * {
 *   MCUX_CSSL_FP_BRANCH_NEGATIVE(someBranchIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   MCUX_CSSL_FP_CONDITIONAL_IMPL(!condition,
 *     MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE(someBranchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_BRANCH_DECL}
 * @expectation{MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:     Identifier for the branch for which the negative scenario is
 *                  executed.
 *        - expect: Zero or more (comma separated) declarations of expected code
 *                  flow behavior related to this event.
 */
#define MCUX_CSSL_FP_BRANCH_NEGATIVE(...) \
  MCUX_CSSL_FP_BRANCH_NEGATIVE_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE
 * @brief Expectation that positive branch has been taken.
 * @api
 * @ingroup csslFpBranch
 *
 * This expectation macro indicates to the flow protection mechanism that the
 * branch declared by #MCUX_CSSL_FP_BRANCH_DECL with the given \p id has
 * executed the positive scenario (under the given \p condition). For example:
 * @code
 * MCUX_CSSL_FP_BRANCH_DECL(someBranchIdentifier);
 * if (condition)
 * {
 *   MCUX_CSSL_FP_BRANCH_POSITIVE(someBranchIdentifier);
 * }
 * else
 * {
 *   MCUX_CSSL_FP_BRANCH_NEGATIVE(someBranchIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   // Providing the condition as part of the branch expectation.
 *   // Alternatively, the expectation can be placed in a conditional block.
 *   MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE(someBranchIdentifier, condition)
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_BRANCH_DECL}
 * @event{MCUX_CSSL_FP_BRANCH_POSITIVE}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 * @see MCUX_CSSL_FP_CONDITIONAL
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:        Identifier of the flow protected branch.
 *        - condition: Optional, condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE(...) \
  MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE
 * @brief Expectation that negative branch has been taken.
 * @api
 * @ingroup csslFpBranch
 *
 * This expectation macro indicates to the flow protection mechanism that the
 * branch declared by #MCUX_CSSL_FP_BRANCH_DECL with the given \p id has
 * executed the negative scenario (under the given \p condition). For example:
 * @code
 * MCUX_CSSL_FP_BRANCH_DECL(someBranchIdentifier);
 * if (condition)
 * {
 *   MCUX_CSSL_FP_BRANCH_POSITIVE(someBranchIdentifier);
 * }
 * else
 * {
 *   MCUX_CSSL_FP_BRANCH_NEGATIVE(someBranchIdentifier);
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, 0,
 *   // Providing the branch expectation as part of a conditional block.
 *   // Alternatively, the condition can be provided in the branch expectation.
 *   MCUX_CSSL_FP_CONDITIONAL(!condition,
 *     MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE(someBranchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_BRANCH_DECL}
 * @event{MCUX_CSSL_FP_BRANCH_NEGATIVE}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 * @see MCUX_CSSL_FP_CONDITIONAL
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:        Identifier of the flow protected branch.
 *        - condition: Optional, condition under which this branch is taken.
 */
#define MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE(...) \
  MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE_IMPL(__VA_ARGS__)



/**
 * @defgroup csslFpSwitch Switching flow protection
 * @brief Support for flow protected switches.
 * @ingroup mcuxCsslFlowProtection
 *
 * @declaration{MCUX_CSSL_FP_SWITCH_DECL}
 * @event{MCUX_CSSL_FP_SWITCH_CASE,MCUX_CSSL_FP_SWITCH_DEFAULT}
 * @expectation{MCUX_CSSL_FP_SWITCH_TAKEN,MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT}
 */

/**
 * @def MCUX_CSSL_FP_SWITCH_DECL
 * @brief Declaration of a flow protected switch.
 * @api
 * @ingroup csslFpSwitch
 *
 * To inform the flow protection mechanism about a switch that needs to be
 * protected, a switch identifier needs to be declared. This identifier can
 * then be used in the events and expectation macros. For example:
 * @code
 * MCUX_CSSL_FP_SWITCH_DECL(someSwitchIdentifier);
 * switch (arg)
 * {
 *   case 0xC0DEu:
 *   {
 *     result = 0xC0DEu;
 *     MCUX_CSSL_FP_SWITCH_CASE(someSwitchIdentifier, 0xC0DEu);
 *     break;
 *   }
 *   default:
 *   {
 *     result = 0;
 *     MCUX_CSSL_FP_SWITCH_DEFAULT(someSwitchIdentifier);
 *     break;
 *   }
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, result,
 *   // Option 1: provide the condition as part of the switch expectation.
 *   MCUX_CSSL_FP_SWITCH_TAKEN(someSwitchIdentifier, 0xC0DEu, 0xC0DEu == arg),
 *   // Option 2: place the switch expectation in a conditional block.
 *   MCUX_CSSL_FP_CONDITIONAL(0xC0DEu != arg),
 *     MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT(someSwitchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @event{MCUX_CSSL_FP_SWITCH_CASE,MCUX_CSSL_FP_SWITCH_DEFAULT}
 * @expectation{MCUX_CSSL_FP_SWITCH_TAKEN,MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT}
 *
 * @param id Identifier for the switch that is flow protected.
 */
#define MCUX_CSSL_FP_SWITCH_DECL(id) \
  MCUX_CSSL_FP_SWITCH_DECL_IMPL(id)

/**
 * @def MCUX_CSSL_FP_SWITCH_CASE
 * @brief Case that is being handled from a switch.
 * @api
 * @ingroup csslFpSwitch
 *
 * This switch event macro informs the flow mechanism that the given \p case of
 * the switch is executed for the switch declared by #MCUX_CSSL_FP_SWITCH_DECL
 * with the given \p id. For example:
 * @code
 * MCUX_CSSL_FP_SWITCH_DECL(someSwitchIdentifier);
 * switch (arg)
 * {
 *   case 0xC0DEu:
 *   {
 *     result = 0xC0DEu;
 *     MCUX_CSSL_FP_SWITCH_CASE(someSwitchIdentifier, 0xC0DEu);
 *     break;
 *   }
 *   default:
 *   {
 *     result = 0;
 *     MCUX_CSSL_FP_SWITCH_DEFAULT(someSwitchIdentifier);
 *     break;
 *   }
 * }
 * // ...
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, result,
 *   // Option 1: provide the condition as part of the switch expectation.
 *   MCUX_CSSL_FP_SWITCH_TAKEN(someSwitchIdentifier, 0xC0DEu, 0xC0DEu == arg),
 *   // Option 2: place the switch expectation in a conditional block.
 *   MCUX_CSSL_FP_CONDITIONAL(0xC0DEu != arg),
 *     MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT(someSwitchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_SWITCH_DECL}
 * @expectation{MCUX_CSSL_FP_SWITCH_TAKEN}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:     Identifier of the flow protected switch.
 *        - case:   Case value that is chosen in the switch.
 *        - expect: Zero or more (comma separated) declarations of expected code
 *                  flow behavior related to this event.
 */
#define MCUX_CSSL_FP_SWITCH_CASE(...) \
  MCUX_CSSL_FP_SWITCH_CASE_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_SWITCH_DEFAULT
 * @brief Case that is being handled from a switch.
 * @api
 * @ingroup csslFpSwitch
 *
 * This switch event macro informs the flow mechanism that the default case of
 * the switch is executed for the switch declared by #MCUX_CSSL_FP_SWITCH_DECL
 * with the given \p id. For example:
 * @code
 * MCUX_CSSL_FP_SWITCH_DECL(someSwitchIdentifier);
 * switch (arg)
 * {
 *   case 0xC0DEu:
 *   {
 *     result = 0xC0DEu;
 *     MCUX_CSSL_FP_SWITCH_CASE(someSwitchIdentifier, 0xC0DEu);
 *     break;
 *   }
 *   default:
 *   {
 *     result = 0;
 *     MCUX_CSSL_FP_SWITCH_DEFAULT(someSwitchIdentifier);
 *     break;
 *   }
 * }
 *
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, result,
 *   // Option 1: provide the condition as part of the switch expectation.
 *   MCUX_CSSL_FP_SWITCH_TAKEN(argCheck, 0xC0DEu, 0xC0DEu == arg),
 *   // Option 2: place the switch expectation in a conditional block.
 *   MCUX_CSSL_FP_CONDITIONAL(0xC0DEu != arg),
 *     MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT(someSwitchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_SWITCH_DECL}
 * @expectation{MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT}
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:     Identifier of the flow protected switch.
 *        - expect: Zero or more (comma separated) declarations of expected code
 *                  flow behavior related to this event.
 */
#define MCUX_CSSL_FP_SWITCH_DEFAULT(...) \
  MCUX_CSSL_FP_SWITCH_DEFAULT_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_SWITCH_TAKEN
 * @brief Expected that a specific case is handled from a switch.
 * @api
 * @ingroup csslFpSwitch
 *
 * This expectation macro indicates to the flow protection mechanism that the
 * switch declared by #MCUX_CSSL_FP_SWITCH_DECL with the given \p id has
 * executed the \p case (under the given \p condition). For example:
 * @code
 * MCUX_CSSL_FP_SWITCH_DECL(someSwitchIdentifier);
 * switch (arg)
 * {
 *   case 0xC0DEu:
 *   {
 *     result = 0xC0DEu;
 *     MCUX_CSSL_FP_SWITCH_CASE(someSwitchIdentifier, 0xC0DEu);
 *     break;
 *   }
 *   default:
 *   {
 *     result = 0;
 *     MCUX_CSSL_FP_SWITCH_DEFAULT(someSwitchIdentifier);
 *     break;
 *   }
 * }
 *
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, result,
 *   // Option 1: provide the condition as part of the switch expectation.
 *   MCUX_CSSL_FP_SWITCH_TAKEN(argCheck, 0xC0DEu, 0xC0DEu == arg),
 *   // Option 2: place the switch expectation in a conditional block.
 *   MCUX_CSSL_FP_CONDITIONAL(0xC0DEu != arg),
 *     MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT(someSwitchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_SWITCH_DECL}
 * @event{MCUX_CSSL_FP_SWITCH_CASE}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:        Identifier of the flow protected switch.
 *        - case:      Value of the case that is expected to be chosen in the
 *                     switch.
 *        - condition: Optional, condition under which the \p case is taken.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN(...) \
  MCUX_CSSL_FP_SWITCH_TAKEN_IMPL(__VA_ARGS__)

/**
 * @def MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT
 * @brief Expected that default case is handled from a switch.
 * @api
 * @ingroup csslFpSwitch
 *
 * This expectation macro indicates to the flow protection mechanism that the
 * switch declared by #MCUX_CSSL_FP_SWITCH_DECL with the given \p id has
 * executed the default case (under the given \p condition). For example:
 * @code
 * MCUX_CSSL_FP_SWITCH_DECL(someSwitchIdentifier);
 * switch (arg)
 * {
 *   case 0xC0DEu:
 *   {
 *     result = 0xC0DEu;
 *     MCUX_CSSL_FP_SWITCH_CASE(someSwitchIdentifier, 0xC0DEu);
 *     break;
 *   }
 *   default:
 *   {
 *     result = 0;
 *     MCUX_CSSL_FP_SWITCH_DEFAULT(someSwitchIdentifier);
 *     break;
 *   }
 * }
 *
 * MCUX_CSSL_FP_FUNCTION_EXIT(someFunction, result,
 *   // Option 1: provide the condition as part of the switch expectation.
 *   MCUX_CSSL_FP_SWITCH_TAKEN(argCheck, 0xC0DEu, 0xC0DEu == arg),
 *   // Option 2: place the switch expectation in a conditional block.
 *   MCUX_CSSL_FP_CONDITIONAL(0xC0DEu != arg),
 *     MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT(someSwitchIdentifier)
 *   )
 * );
 * @endcode
 *
 * @declaration{MCUX_CSSL_FP_SWITCH_DECL}
 * @event{MCUX_CSSL_FP_SWITCH_DEFAULT}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - id:        Identifier of the flow protected switch.
 *        - condition: Optional, condition under which the default case is taken.
 */
#define MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT(...) \
  MCUX_CSSL_FP_SWITCH_TAKEN_DEFAULT_IMPL(__VA_ARGS__)



/**
 * @defgroup csslFpExpect Expectation handling
 * @brief Expectation handling support functionality.
 *
 * @ingroup mcuxCsslFlowProtection
 */

/**
 * @def MCUX_CSSL_FP_EXPECT
 * @brief Declaration(s) of expected code flow behavior.
 * @api
 * @ingroup csslFpExpect
 *
 * This macro can be used to indicate expectations in the function body at
 * another location than the function entry or exit.
 *
 * @note In general the use of this macro is discouraged, to avoid a potential
 * security and/or code-size impact. However, it may be usefull for complex
 * code, where an intermediate update can actually save code, since conditions
 * for expectations can than be locallized.
 *
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED,MCUX_CSSL_FP_LOOP_ITERATIONS,MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE,MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE,MCUX_CSSL_FP_SWITCH_TAKEN}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_CONDITIONAL
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - expect: One or more (comma separated) declarations of expected code
 *                  flow behavior.
 */
#define MCUX_CSSL_FP_EXPECT(...) \
  MCUX_CSSL_FP_EXPECT_IMPL(__VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

/**
 * @def MCUX_CSSL_FP_CONDITIONAL
 * @brief Handling of conditionally expected code flow behavior.
 * @api
 * @ingroup csslFpExpect
 *
 * This macro can be used to indicate expectations that are only true under a
 * given \p condition.
 *
 * @expectation{MCUX_CSSL_FP_FUNCTION_CALLED,MCUX_CSSL_FP_LOOP_ITERATIONS,MCUX_CSSL_FP_BRANCH_TAKEN_POSITIVE,MCUX_CSSL_FP_BRANCH_TAKEN_NEGATIVE,MCUX_CSSL_FP_SWITCH_TAKEN}
 *
 * @see MCUX_CSSL_FP_FUNCTION_ENTRY
 * @see MCUX_CSSL_FP_FUNCTION_EXIT
 * @see MCUX_CSSL_FP_FUNCTION_EXIT_WITH_CHECK
 * @see MCUX_CSSL_FP_EXPECT
 *
 * @param condition Condition under which the given expectations apply.
 * @param ...    One or more (comma separated) declarations of expected code
 *               flow behavior.
 */
#define MCUX_CSSL_FP_CONDITIONAL(condition, ...) \
  MCUX_CSSL_FP_CONDITIONAL_IMPL((condition), __VA_ARGS__, MCUX_CSSL_FP_VOID_EXPECTATION_IMPL())

MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_TEXT_IN_COMMENTS()

/**
 * @def MCUX_CSSL_FP_ASSERT
 * @brief Assert an expected state of the code flow.
 * @api
 * @ingroup csslFpExpect
 *
 * This macro can be used to check whether the code flow up to this point
 * matches the expected state. Unlike the #MCUX_CSSL_FP_EXPECT macro, it will
 * not update the expectations, but merely perform a check on the recorded
 * events against the already recorded expectations plus the ones provided as
 * parameters.
 *
 * If the check fails, the code defined in #MCUX_CSSL_FP_ASSERT_CALLBACK() will
 * be executed.
 *
 * @note #MCUX_CSSL_FP_ASSERT_CALLBACK() must be defined before including the
 * CSSL flow protection headers, otherwise a default implementation could be
 * used.
 *
 * @see MCUX_CSSL_FP_EXPECT
 *
 * @param ...    The following parameters need to be passed (comma separated):
 *        - expect: One or more (comma separated) declarations of expected code
 *                  flow behavior.
 */
#define MCUX_CSSL_FP_ASSERT(...) \
  MCUX_CSSL_FP_ASSERT_IMPL(__VA_ARGS__)

#endif /* MCUX_CSSL_FLOW_PROTECTION_H_ */

