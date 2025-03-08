/*--------------------------------------------------------------------------*/
/* Copyright 2019-2020, 2023 NXP                                            */
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

#ifndef MCUX_CSSL_C_PRE_PROCESSOR_H_
#define MCUX_CSSL_C_PRE_PROCESSOR_H_

/**
 * @file  mcuxCsslCPreProcessor.h
 * @brief The default implementation is based on standard C preprocessor
 * functionality
 */

#define MCUX_CSSL_CPP_STR(a) #a

#define MCUX_CSSL_CPP_ADD(a) + (a)

#define MCUX_CSSL_CPP_CAT_IMPL(a, b) a##b

#define MCUX_CSSL_CPP_CAT(a, b) \
  MCUX_CSSL_CPP_CAT_IMPL(a, b)

#define MCUX_CSSL_CPP_CAT3(a, b, c) \
  MCUX_CSSL_CPP_CAT(MCUX_CSSL_CPP_CAT(a, b), c)

#define MCUX_CSSL_CPP_CAT4(a, b, c, d) \
  MCUX_CSSL_CPP_CAT(MCUX_CSSL_CPP_CAT(a, b), MCUX_CSSL_CPP_CAT(c, d))

#define MCUX_CSSL_CPP_CAT6(a, b, c, d, e, f) \
  MCUX_CSSL_CPP_CAT3( \
    MCUX_CSSL_CPP_CAT(a, b), \
    MCUX_CSSL_CPP_CAT(c, d), \
    MCUX_CSSL_CPP_CAT(e, f))

#define MCUX_CSSL_CPP_SEQUENCE_64TO0() \
  64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, \
  48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, \
  32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
  16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 0

#define MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_5TO0() \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, 5, 4, 3, 2, 1, 0

#define MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_4TO0() \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, 4, 3, 2, 1, 0

#define MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_3TO0() \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, 3, 2, 1, 0

#define MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_2TO0() \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, \
  n, n, n, n, n, n, n, n, n, n, n, n, n, n, 2, 1, 0

#define MCUX_CSSL_CPP_ARG_N( \
  _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, \
  _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, \
  _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, \
  _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, \
  N, ...) \
    N

#define MCUX_CSSL_CPP_ARGCOUNT_IMPL(...) \
  MCUX_CSSL_CPP_ARG_N(__VA_ARGS__)

#define MCUX_CSSL_CPP_ARGCOUNT(...) \
  MCUX_CSSL_CPP_ARGCOUNT_IMPL(__VA_ARGS__,MCUX_CSSL_CPP_SEQUENCE_64TO0())

#define MCUX_CSSL_CPP_ARGCOUNT_2N(...) \
  MCUX_CSSL_CPP_ARGCOUNT_IMPL(__VA_ARGS__,MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_2TO0())

#define MCUX_CSSL_CPP_ARGCOUNT_3N(...) \
  MCUX_CSSL_CPP_ARGCOUNT_IMPL(__VA_ARGS__,MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_3TO0())

#define MCUX_CSSL_CPP_ARGCOUNT_4N(...) \
  MCUX_CSSL_CPP_ARGCOUNT_IMPL(__VA_ARGS__,MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_4TO0())

#define MCUX_CSSL_CPP_ARGCOUNT_5N(...) \
  MCUX_CSSL_CPP_ARGCOUNT_IMPL(__VA_ARGS__,MCUX_CSSL_CPP_SEQUENCE_N_UNTIL_5TO0())

#define MCUX_CSSL_CPP_OVERLOADED_IMPL(name, n) MCUX_CSSL_CPP_CAT_IMPL(name, n)

#define MCUX_CSSL_CPP_OVERLOADED(name, ...) \
  MCUX_CSSL_CPP_DEFER2(MCUX_CSSL_CPP_OVERLOADED_IMPL)()(name, MCUX_CSSL_CPP_ARGCOUNT(__VA_ARGS__))

#define MCUX_CSSL_CPP_OVERLOADED1(name, ...) \
  MCUX_CSSL_CPP_IF_ELSE(MCUX_CSSL_CPP_HAS_ONE_ARG(__VA_ARGS__))( \
    /* If only one arg, use the 1 version */ \
    MCUX_CSSL_CPP_CAT(name,1)(MCUX_CSSL_CPP_FIRST(__VA_ARGS__, /* ensure extra argument: */ 0)) \
  )( \
    /* Otherwise the n version */ \
    MCUX_CSSL_CPP_CAT(name,n)(__VA_ARGS__) \
  )

#define MCUX_CSSL_CPP_OVERLOADED2(name, ...) \
  MCUX_CSSL_CPP_OVERLOADED_IMPL(name, MCUX_CSSL_CPP_ARGCOUNT_2N(__VA_ARGS__))(__VA_ARGS__)

#define MCUX_CSSL_CPP_OVERLOADED3(name, ...) \
  MCUX_CSSL_CPP_OVERLOADED_IMPL(name, MCUX_CSSL_CPP_ARGCOUNT_3N(__VA_ARGS__))(__VA_ARGS__)

#define MCUX_CSSL_CPP_OVERLOADED4(name, ...) \
  MCUX_CSSL_CPP_OVERLOADED_IMPL(name, MCUX_CSSL_CPP_ARGCOUNT_4N(__VA_ARGS__))(__VA_ARGS__)

#define MCUX_CSSL_CPP_OVERLOADED5(name, ...) \
  MCUX_CSSL_CPP_OVERLOADED_IMPL(name, MCUX_CSSL_CPP_ARGCOUNT_5N(__VA_ARGS__))(__VA_ARGS__)

/*****************************************************************************
 * Helper macros                                                             *
 *****************************************************************************/

/* Apply a macro to all arguments */
#define MCUX_CSSL_CPP_MAP(__macro, ...) \
  MCUX_CSSL_CPP_EVAL(MCUX_CSSL_CPP_MAP_IMPL(__macro, __VA_ARGS__))

/* Evaluate a complex macro (which needs multiple expansions to be processed) */
#define MCUX_CSSL_CPP_EVAL(...)     MCUX_CSSL_CPP_EVAL1024(__VA_ARGS__)
#define MCUX_CSSL_CPP_EVAL1024(...) MCUX_CSSL_CPP_EVAL512(MCUX_CSSL_CPP_EVAL512(__VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL512(...)  MCUX_CSSL_CPP_EVAL256(MCUX_CSSL_CPP_EVAL256(__VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL256(...)  MCUX_CSSL_CPP_EVAL128(MCUX_CSSL_CPP_EVAL128(__VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL128(...)  MCUX_CSSL_CPP_EVAL64( MCUX_CSSL_CPP_EVAL64( __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL64(...)   MCUX_CSSL_CPP_EVAL32( MCUX_CSSL_CPP_EVAL32( __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL32(...)   MCUX_CSSL_CPP_EVAL16( MCUX_CSSL_CPP_EVAL16( __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL16(...)   MCUX_CSSL_CPP_EVAL8(  MCUX_CSSL_CPP_EVAL8(  __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL8(...)    MCUX_CSSL_CPP_EVAL4(  MCUX_CSSL_CPP_EVAL4(  __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL4(...)    MCUX_CSSL_CPP_EVAL2(  MCUX_CSSL_CPP_EVAL2(  __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL2(...)    MCUX_CSSL_CPP_EVAL1(  MCUX_CSSL_CPP_EVAL1(  __VA_ARGS__))
#define MCUX_CSSL_CPP_EVAL1(...)    __VA_ARGS__

/* Recursive definition of map macro, assumes at least one argument */
#define MCUX_CSSL_CPP_MAP_IMPL(__macro, ...) \
  /* Apply the macro to the first argument from the list */\
  __macro(MCUX_CSSL_CPP_FIRST(__VA_ARGS__, /* ensure second argument: */ 0)) \
  /* Only proceed if there are additional arguments */\
  MCUX_CSSL_CPP_IF(MCUX_CSSL_CPP_HAS_MORE_ARGS(__VA_ARGS__))( \
    /* Recursive call for remaining arguments */\
    MCUX_CSSL_CPP_DEFER2(MCUX_CSSL_CPP_MAP_IMPL_)()(__macro, \
      MCUX_CSSL_CPP_NEXT(__VA_ARGS__)) \
  )
#define MCUX_CSSL_CPP_MAP_IMPL_() MCUX_CSSL_CPP_MAP_IMPL

/* Extract first argument (requires at least two arguments to be present) */
#define MCUX_CSSL_CPP_FIRST(a, ...) a
/* Extract second argument (requires at least three arguments to be present) */
#define MCUX_CSSL_CPP_SECOND(a, b, ...) b
/* Extract third argument (requires at least four arguments to be present) */
#define MCUX_CSSL_CPP_THIRD(a, b, c, ...) c
/* Extract fourth argument (requires at least five arguments to be present) */
#define MCUX_CSSL_CPP_FOURTH(a, b, c, d, ...) d
/* Remove the first argument from the list (requires at least two arguments to be present) */
#define MCUX_CSSL_CPP_NEXT(...) MCUX_CSSL_CPP_NEXT_()(__VA_ARGS__)
#define MCUX_CSSL_CPP_NEXT_() MCUX_CSSL_CPP_NEXT__
#define MCUX_CSSL_CPP_NEXT__(x, ...) __VA_ARGS__

/* Check whether there is more then one argument */
#define MCUX_CSSL_CPP_HAS_MORE_ARGS(...) \
  MCUX_CSSL_CPP_NOT(MCUX_CSSL_CPP_IS_MARKER(MCUX_CSSL_CPP_SECOND(__VA_ARGS__, MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER())))

#define MCUX_CSSL_CPP_HAS_ONE_ARG(...) \
  MCUX_CSSL_CPP_IS_MARKER(MCUX_CSSL_CPP_SECOND(__VA_ARGS__, MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER()))

#define MCUX_CSSL_CPP_HAS_TWO_ARGS(...) \
  MCUX_CSSL_CPP_IS_MARKER(MCUX_CSSL_CPP_THIRD(__VA_ARGS__, MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER()))

#define MCUX_CSSL_CPP_HAS_THREE_ARGS(...) \
  MCUX_CSSL_CPP_IS_MARKER(MCUX_CSSL_CPP_FOURTH(__VA_ARGS__, MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER(), MCUX_CSSL_CPP_MARKER()))


/* Check whether the argument is MCUX_CSSL_CPP_MARKER(), return 1 if it is */
#define MCUX_CSSL_CPP_IS_MARKER(...) \
  MCUX_CSSL_CPP_SECOND(__VA_ARGS__, 0, 0)
#define MCUX_CSSL_CPP_MARKER() \
  ~, 1

/* Convert any argument into a bool (either 0 or 1), by double negation */
#define MCUX_CSSL_CPP_BOOL(x) MCUX_CSSL_CPP_NOT(MCUX_CSSL_CPP_NOT(x))

/* Boolean negation (map value 0 to the marker, and check if we have the marker) */
#define MCUX_CSSL_CPP_NOT(x) MCUX_CSSL_CPP_IS_MARKER(MCUX_CSSL_CPP_CAT(MCUX_CSSL_CPP_NOT_BOOL_, x))
#define MCUX_CSSL_CPP_NOT_BOOL_0 MCUX_CSSL_CPP_MARKER()

/* Convert condition to bool */
#define MCUX_CSSL_CPP_IF(condition) MCUX_CSSL_CPP_IF_(MCUX_CSSL_CPP_BOOL(condition))
/* Convert bool to decision defines */
#define MCUX_CSSL_CPP_IF_(condition) MCUX_CSSL_CPP_CAT(MCUX_CSSL_CPP_IF_BOOL_, condition)
/* If 0, do nothing*/
#define MCUX_CSSL_CPP_IF_BOOL_0(...)
/* If 1, perform action */
#define MCUX_CSSL_CPP_IF_BOOL_1(...) __VA_ARGS__

/* Convert condition to bool */
#define MCUX_CSSL_CPP_IF_ELSE(condition) MCUX_CSSL_CPP_IF_ELSE_IMPL(MCUX_CSSL_CPP_BOOL(condition))
/* Convert bool to decision defines */
#define MCUX_CSSL_CPP_IF_ELSE_IMPL(condition) MCUX_CSSL_CPP_CAT(MCUX_CSSL_CPP_IFE_BOOL_, condition)
/* If 0, ignore action */
#define MCUX_CSSL_CPP_IFE_BOOL_0(...)             MCUX_CSSL_CPP_IFE_BOOL_0_ELSE
/* Else 0, perform action */
#define MCUX_CSSL_CPP_IFE_BOOL_0_ELSE(...) __VA_ARGS__
/* If 1, perform action */
#define MCUX_CSSL_CPP_IFE_BOOL_1(...) __VA_ARGS__ MCUX_CSSL_CPP_IFE_BOOL_1_ELSE
/* Else 1, ignore action */

#define MCUX_CSSL_CPP_IFE_BOOL_1_ELSE(...)

/* Defer macro expansion */
#define MCUX_CSSL_CPP_EMPTY()
#define MCUX_CSSL_CPP_DEFER1(macro) macro MCUX_CSSL_CPP_EMPTY()
#define MCUX_CSSL_CPP_DEFER2(macro) macro MCUX_CSSL_CPP_EMPTY MCUX_CSSL_CPP_EMPTY()()

#endif /* MCUX_CSSL_C_PRE_PROCESSOR_H_ */
