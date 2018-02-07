/*
 * Copyright (c) 2015 Cesanta Software Limited
 * All rights reserved
 * This software is dual-licensed: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation. For the terms of this
 * license, see <http://www.gnu.org/licenses/>.
 *
 * You are free to use this software under the terms of the GNU General
 * Public License, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * Alternatively, you can license this software under a commercial
 * license, as set out in <https://www.cesanta.com/license>.
 */

#ifndef CS_COMMON_TEST_UTIL_H_
#define CS_COMMON_TEST_UTIL_H_

#include <string.h>
#include <stdio.h>

#include "common/cs_time.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int g_num_tests;

#ifdef MG_TEST_ABORT_ON_FAIL
#define MG_TEST_ABORT abort()
#else
#define MG_TEST_ABORT
#endif

int _assert_streq(const char *actual, const char *expected);
int _assert_streq_nz(const char *actual, const char *expected);
void _strfail(const char *a, const char *e, int len);

#define FAIL(str, line)                                              \
  do {                                                               \
    printf("%s:%d:1 [%s] (in %s)\n", __FILE__, line, str, __func__); \
    MG_TEST_ABORT;                                                   \
    return str;                                                      \
  } while (0)

#define ASSERT(expr)                    \
  do {                                  \
    g_num_tests++;                      \
    if (!(expr)) FAIL(#expr, __LINE__); \
  } while (0)
#define ASSERT_TRUE(expr) ASSERT(expr)
#define ASSERT_FALSE(expr) ASSERT(!(expr))

/*
 * Run test function, use its name as the test name to print
 */
#define RUN_TEST(test) RUN_TEST_WNAME(test, #test)

/*
 * Run test function, use the provided name as the test name to print
 */
#define RUN_TEST_WNAME(test, test_name)            \
  do {                                             \
    const char *msg = NULL;                        \
    if (strstr(test_name, filter)) {               \
      double elapsed = cs_time();                  \
      msg = test();                                \
      elapsed = cs_time() - elapsed;               \
      printf("  [%.3f] %s\n", elapsed, test_name); \
      fflush(stdout);                              \
    }                                              \
    if (msg) return msg;                           \
  } while (0)

/* VC6 doesn't know how to cast an unsigned 64-bit int to double */
#if (defined(_MSC_VER) && _MSC_VER <= 1200)
#define AS_DOUBLE(d) (double)(int64_t)(d)
#else
#define AS_DOUBLE(d) (double)(d)
#endif

/*
 * Numeric equality assertion. Comparison is made in native types but for
 * printing both are convetrted to double.
 */
#define ASSERT_EQ(actual, expected)                                 \
  do {                                                              \
    g_num_tests++;                                                  \
    if (!((actual) == (expected))) {                                \
      printf("%f != %f\n", AS_DOUBLE(actual), AS_DOUBLE(expected)); \
      FAIL(#actual " == " #expected, __LINE__);                     \
    }                                                               \
  } while (0)

/* "Less than" assertion. */
#define ASSERT_LT(a, b)                                 \
  do {                                                  \
    g_num_tests++;                                      \
    if (!((a) < (b))) {                                 \
      printf("%f >= %f\n", AS_DOUBLE(a), AS_DOUBLE(b)); \
      FAIL(#a " < " #b, __LINE__);                      \
    }                                                   \
  } while (0)

/* "Greater than" assertion. */
#define ASSERT_GT(a, b)                                 \
  do {                                                  \
    g_num_tests++;                                      \
    if (!((a) > (b))) {                                 \
      printf("%f <= %f\n", AS_DOUBLE(a), AS_DOUBLE(b)); \
      FAIL(#a " > " #b, __LINE__);                      \
    }                                                   \
  } while (0)

/* Assert that actual == expected, where both are NUL-terminated. */
#define ASSERT_STREQ(actual, expected)                            \
  do {                                                            \
    g_num_tests++;                                                \
    if (!_assert_streq(actual, expected)) {                       \
      FAIL("ASSERT_STREQ(" #actual ", " #expected ")", __LINE__); \
    }                                                             \
  } while (0)

/* Assert that actual == expected, where both are pointers */
#define ASSERT_PTREQ(actual, expected)                            \
  do {                                                            \
    g_num_tests++;                                                \
    if (actual != expected) {                                     \
      printf("%p != %p\n", actual, expected);                     \
      FAIL("ASSERT_PTREQ(" #actual ", " #expected ")", __LINE__); \
    }                                                             \
  } while (0)

/* Assert that actual != expected, where both are pointers */
#define ASSERT_PTRNEQ(actual, expected)                            \
  do {                                                             \
    g_num_tests++;                                                 \
    if (actual == expected) {                                      \
      printf("%p == %p\n", actual, expected);                      \
      FAIL("ASSERT_PTRNEQ(" #actual ", " #expected ")", __LINE__); \
    }                                                              \
  } while (0)

/* Same as STREQ, but only expected is NUL-terminated. */
#define ASSERT_STREQ_NZ(actual, expected)                            \
  do {                                                               \
    g_num_tests++;                                                   \
    if (!_assert_streq_nz(actual, expected)) {                       \
      FAIL("ASSERT_STREQ_NZ(" #actual ", " #expected ")", __LINE__); \
    }                                                                \
  } while (0)

#define ASSERT_MG_STREQ(actual, expected)                            \
  do {                                                               \
    g_num_tests++;                                                   \
    if ((actual).len != strlen(expected) ||                          \
        memcmp((actual).p, expected, (actual).len) != 0) {           \
      printf("'%.*s' (%d) != '%s'\n", (int)(actual).len, (actual).p, \
             (int)(actual).len, expected);                           \
      FAIL("ASSERT_MG_STREQ(" #actual ", " #expected ")", __LINE__); \
    }                                                                \
  } while (0)

#define CHECK_CALL(call)                                                  \
  do {                                                                    \
    const char *r = (call);                                               \
    if (r != NULL) {                                                      \
      fprintf(stderr, "... %s:%d in %s\n", __FILE__, __LINE__, __func__); \
      return r;                                                           \
    }                                                                     \
  } while (0)

#ifndef MG_ENABLE_POLL_UNTIL
#define MG_ENABLE_POLL_UNTIL 0
#endif

#if MG_ENABLE_POLL_UNTIL
#include "mongoose.h"
int c_str_ne(void *a, void *b);
int c_int_ne(void *a, void *b);
int c_int_eq(void *a, void *b);
void poll_until(struct mg_mgr *mgr, double timeout, int (*cond)(void *, void *),
                void *cond_arg1, void *cond_arg2);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CS_COMMON_TEST_UTIL_H_ */
